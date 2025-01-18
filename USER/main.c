#include "stm32f10x.h"
#include "sys.h" 
#define TRIG PAout(3) //输出端口
#define ECHO PAin(2)  //输入端口
int overcount=0;      //记录定时器溢出次数
int length;

float Pitch,Roll,Yaw;		   	//角度
short gyrox,gyroy,gyroz;		//陀螺仪--角速度
short aacx,aacy,aacz;			//加速度
int Encoder_Left,Encoder_Right;	//编码器数据（速度）

int PWM_MAX=5000,PWM_MIN=-5000;	//PWM限幅变量
int MOTO1,MOTO2;				//电机装载变量

//温度和湿度变量
u8 temp,humi;

//烟雾浓度变量
u16 MQ2_value;
u8 MQ2_buff[30];//参数显示缓存数组
float MQ2_ppm;

//空气质量变量
u16 MQ135_value;
u8 MQ135_buff[30];//参数显示缓存数组
float MQ135_ppm;

//声音检测
float Voice_Value;
u8 LM1204_buff[30];         // 参数显示缓存数组
float voltage;       // 存储电压值
float dB_value;      // 存储分贝值

extern int Vertical_out,Velocity_out,Turn_out;
//测距函数
int Senor_Using(void);
//OLED翻页页数，共5页，第1页显示平衡车自身参数，2-5页显示环境参数
extern int page;
u8 lock=1;

int main(void)	
{
	delay_init();
	NVIC_Config();
	uart1_init(115200);	
	uart3_init(9600);
//	while(lock==1);
	
	EXTI_Key_Config();//OLED按键初始化
	OLED_Init();
	OLED_Clear();
	OLED_Showtitle();
	
	MPU_Init();
	mpu_dmp_init();
	MPU6050_EXTI_Init();
	
	Encoder_TIM2_Init();
	Encoder_TIM4_Init();
	Motor_Init();
	PWM_Init_TIM1(0,7199);
	
	TIM3_Int_Init();
	
	//传感器初始化
	AD_Init();
	DHT11_Init();
  while(1)	
	{
		switch (page){
			case 0://显示平衡车自身参数
				length = Senor_Using(); //调用测距函数
				OLED_ShowString(0,2,"Angle:",16);
				OLED_ShowString(0,4,"Dist:",16);
				OLED_ShowString(0,6,"Speed:",16);
				//显示陀螺仪角度、超声波距离及当前左右电机速度
				OLED_Float(3,60,Pitch,1);
				OLED_Float(5,60,length,1);
				OLED_Num3(10,6,Encoder_Left);	
				OLED_Num3(10,7,Encoder_Right);
				delay_ms(100);
				break;
			case 1://显示温湿度
				DHT11_Read_Data(&temp,&humi);
				OLED_ShowString(0,2,"Temp:    C",16);
				OLED_Num2(8,3,temp); 
				OLED_ShowString(0,4,"Humi:    %",16);
				OLED_Num2(8,5,humi); 
				delay_ms(100);
				break;
			case 2://显示空气质量
				MQ135_value = MQ135_GetData();
				MQ135_ppm = MQ135_GetData_PPM();   // 计算 ppm 值
				OLED_ShowMQ135(0,2,0);
				OLED_ShowMQ135(20,2,1);
				OLED_ShowMQ135(40,2,2);
				OLED_ShowMQ135(60,2,3);
				OLED_ShowMQ135(80,2,4);
				OLED_Num4(100,3,MQ135_value);
				sprintf((char*)MQ135_buff, "%.2fppm    ",MQ135_ppm);
				OLED_ShowString(48,4,MQ135_buff,16);
				if(MQ135_ppm>450){
					OLED_ShowString(48,6,"Abnormal",16);//异常
				}
				else{
					OLED_ShowString(48,6,"Normal",16);//正常
				}
				delay_ms(100);
				break;
			case 3://显示声音强度
				Voice_Value = (float)LM1204_ADC_Read();
				voltage = (Voice_Value /4096) * 3.3;
				dB_value = calculate_dB(voltage);// 计算分贝值
				sprintf((char*)LM1204_buff, "Voice_val:%0.0f   ", Voice_Value);
				OLED_ShowString(0, 2, LM1204_buff, 16);
				//OLED_Num4(100,3,Voice_Value);
				// 在OLED上显示电压值
				sprintf((char*)LM1204_buff, "Voltage:%0.2fV   ", voltage);
				OLED_ShowString(0, 4, LM1204_buff, 16);
				// 在 OLED 上显示分贝值
				sprintf((char*)LM1204_buff, "dB:%.2f", dB_value);
				OLED_ShowString(32, 6, LM1204_buff, 16);
				delay_ms(1000);
				break;
			case 4://显示烟雾强度
				MQ2_value = MQ2_GetData();  // 获取烟雾浓度的原始数据
				MQ2_ppm = MQ2_GetData_PPM();  // 计算 ppm 值
				OLED_ShowMQ2(0,2,0);
				OLED_ShowMQ2(20,2,1);
				OLED_ShowMQ2(40,2,2);
				OLED_ShowMQ2(60,2,3);
				OLED_ShowMQ2(80,2,4);
				OLED_Num4(100,3,MQ2_value);
				sprintf((char*)MQ2_buff, "%.2fppm    ",MQ2_ppm);
				OLED_ShowString(48,4,MQ2_buff,16);
				if(MQ2_ppm>450){
					OLED_ShowString(48,6,"Abnormal",16);//异常
				}
				else{
					OLED_ShowString(48,6,"Normal",16);//正常
				}
				delay_ms(100);
				break;
			default:
				page = 0;
				break;}		
	} 	
}


// 测距函数
int Senor_Using() {
    unsigned int sum = 0; // 用于累计三次测量的距离和
    unsigned int tim; // 用于存储定时器的计数值
    unsigned int i = 0; // 循环计数器
    unsigned int length; // 用于存储单次测量的距离
    u16 cnt_i = 0; // 超时计数器
    while (i != 3) { // 进行三次测量
        TRIG = 1; // 触发超声波发送信号
        delay_us(20); // 保持触发信号20微秒
        TRIG = 0; // 关闭触发信号
        cnt_i = 0; // 重置超时计数器
        while (ECHO == 0) { // 等待接收到回波信号
           cnt_i++; // 超时计数器加一
           delay_us(1); 
           if (cnt_i > 30000) { // 如果超时计数器超过30000，表示没有接收到回波信号
               TRIG = 1; // 重新触发超声波发送信号
               delay_us(20); // 保持触发信号20微秒
               TRIG = 0; // 关闭触发信号
               cnt_i = 0; // 重置超时计数器
           }
        }
        TIM_Cmd(TIM3, ENABLE); // 启动定时器3
        i += 1; // 增加循环计数器
        
        while (ECHO == 1); // 等待回波信号结束
        TIM_Cmd(TIM3, DISABLE); // 停止定时器3
        
        tim = TIM_GetCounter(TIM3); // 获取定时器的计数值
        length = (tim * 100) / 58.0; // 根据定时器计数值计算距离
        sum += length; // 将本次测量的距离加到总和上
        TIM3->CNT = 0; // 重置定时器3的计数值
        overcount = 0; // 重置溢出计数器
        delay_ms(100); // 延时100毫秒后进行下一次测量
    }
    length = sum / 3; // 计算三次测量的平均距离
    return length; // 返回平均距离
}

