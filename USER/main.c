#include "stm32f10x.h"
#include "sys.h" 
#define TRIG PAout(3) //����˿�
#define ECHO PAin(2)  //����˿�
int overcount=0;      //��¼��ʱ���������
int length;

float Pitch,Roll,Yaw;		   	//�Ƕ�
short gyrox,gyroy,gyroz;		//������--���ٶ�
short aacx,aacy,aacz;			//���ٶ�
int Encoder_Left,Encoder_Right;	//���������ݣ��ٶȣ�

int PWM_MAX=5000,PWM_MIN=-5000;	//PWM�޷�����
int MOTO1,MOTO2;				//���װ�ر���

//�¶Ⱥ�ʪ�ȱ���
u8 temp,humi;

//����Ũ�ȱ���
u16 MQ2_value;
u8 MQ2_buff[30];//������ʾ��������
float MQ2_ppm;

//������������
u16 MQ135_value;
u8 MQ135_buff[30];//������ʾ��������
float MQ135_ppm;

//�������
float Voice_Value;
u8 LM1204_buff[30];         // ������ʾ��������
float voltage;       // �洢��ѹֵ
float dB_value;      // �洢�ֱ�ֵ

extern int Vertical_out,Velocity_out,Turn_out;
//��ຯ��
int Senor_Using(void);
//OLED��ҳҳ������5ҳ����1ҳ��ʾƽ�⳵���������2-5ҳ��ʾ��������
extern int page;
u8 lock=1;

int main(void)	
{
	delay_init();
	NVIC_Config();
	uart1_init(115200);	
	uart3_init(9600);
//	while(lock==1);
	
	EXTI_Key_Config();//OLED������ʼ��
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
	
	//��������ʼ��
	AD_Init();
	DHT11_Init();
  while(1)	
	{
		switch (page){
			case 0://��ʾƽ�⳵�������
				length = Senor_Using(); //���ò�ຯ��
				OLED_ShowString(0,2,"Angle:",16);
				OLED_ShowString(0,4,"Dist:",16);
				OLED_ShowString(0,6,"Speed:",16);
				//��ʾ�����ǽǶȡ����������뼰��ǰ���ҵ���ٶ�
				OLED_Float(3,60,Pitch,1);
				OLED_Float(5,60,length,1);
				OLED_Num3(10,6,Encoder_Left);	
				OLED_Num3(10,7,Encoder_Right);
				delay_ms(100);
				break;
			case 1://��ʾ��ʪ��
				DHT11_Read_Data(&temp,&humi);
				OLED_ShowString(0,2,"Temp:    C",16);
				OLED_Num2(8,3,temp); 
				OLED_ShowString(0,4,"Humi:    %",16);
				OLED_Num2(8,5,humi); 
				delay_ms(100);
				break;
			case 2://��ʾ��������
				MQ135_value = MQ135_GetData();
				MQ135_ppm = MQ135_GetData_PPM();   // ���� ppm ֵ
				OLED_ShowMQ135(0,2,0);
				OLED_ShowMQ135(20,2,1);
				OLED_ShowMQ135(40,2,2);
				OLED_ShowMQ135(60,2,3);
				OLED_ShowMQ135(80,2,4);
				OLED_Num4(100,3,MQ135_value);
				sprintf((char*)MQ135_buff, "%.2fppm    ",MQ135_ppm);
				OLED_ShowString(48,4,MQ135_buff,16);
				if(MQ135_ppm>450){
					OLED_ShowString(48,6,"Abnormal",16);//�쳣
				}
				else{
					OLED_ShowString(48,6,"Normal",16);//����
				}
				delay_ms(100);
				break;
			case 3://��ʾ����ǿ��
				Voice_Value = (float)LM1204_ADC_Read();
				voltage = (Voice_Value /4096) * 3.3;
				dB_value = calculate_dB(voltage);// ����ֱ�ֵ
				sprintf((char*)LM1204_buff, "Voice_val:%0.0f   ", Voice_Value);
				OLED_ShowString(0, 2, LM1204_buff, 16);
				//OLED_Num4(100,3,Voice_Value);
				// ��OLED����ʾ��ѹֵ
				sprintf((char*)LM1204_buff, "Voltage:%0.2fV   ", voltage);
				OLED_ShowString(0, 4, LM1204_buff, 16);
				// �� OLED ����ʾ�ֱ�ֵ
				sprintf((char*)LM1204_buff, "dB:%.2f", dB_value);
				OLED_ShowString(32, 6, LM1204_buff, 16);
				delay_ms(1000);
				break;
			case 4://��ʾ����ǿ��
				MQ2_value = MQ2_GetData();  // ��ȡ����Ũ�ȵ�ԭʼ����
				MQ2_ppm = MQ2_GetData_PPM();  // ���� ppm ֵ
				OLED_ShowMQ2(0,2,0);
				OLED_ShowMQ2(20,2,1);
				OLED_ShowMQ2(40,2,2);
				OLED_ShowMQ2(60,2,3);
				OLED_ShowMQ2(80,2,4);
				OLED_Num4(100,3,MQ2_value);
				sprintf((char*)MQ2_buff, "%.2fppm    ",MQ2_ppm);
				OLED_ShowString(48,4,MQ2_buff,16);
				if(MQ2_ppm>450){
					OLED_ShowString(48,6,"Abnormal",16);//�쳣
				}
				else{
					OLED_ShowString(48,6,"Normal",16);//����
				}
				delay_ms(100);
				break;
			default:
				page = 0;
				break;}		
	} 	
}


// ��ຯ��
int Senor_Using() {
    unsigned int sum = 0; // �����ۼ����β����ľ����
    unsigned int tim; // ���ڴ洢��ʱ���ļ���ֵ
    unsigned int i = 0; // ѭ��������
    unsigned int length; // ���ڴ洢���β����ľ���
    u16 cnt_i = 0; // ��ʱ������
    while (i != 3) { // �������β���
        TRIG = 1; // ���������������ź�
        delay_us(20); // ���ִ����ź�20΢��
        TRIG = 0; // �رմ����ź�
        cnt_i = 0; // ���ó�ʱ������
        while (ECHO == 0) { // �ȴ����յ��ز��ź�
           cnt_i++; // ��ʱ��������һ
           delay_us(1); 
           if (cnt_i > 30000) { // �����ʱ����������30000����ʾû�н��յ��ز��ź�
               TRIG = 1; // ���´��������������ź�
               delay_us(20); // ���ִ����ź�20΢��
               TRIG = 0; // �رմ����ź�
               cnt_i = 0; // ���ó�ʱ������
           }
        }
        TIM_Cmd(TIM3, ENABLE); // ������ʱ��3
        i += 1; // ����ѭ��������
        
        while (ECHO == 1); // �ȴ��ز��źŽ���
        TIM_Cmd(TIM3, DISABLE); // ֹͣ��ʱ��3
        
        tim = TIM_GetCounter(TIM3); // ��ȡ��ʱ���ļ���ֵ
        length = (tim * 100) / 58.0; // ���ݶ�ʱ������ֵ�������
        sum += length; // �����β����ľ���ӵ��ܺ���
        TIM3->CNT = 0; // ���ö�ʱ��3�ļ���ֵ
        overcount = 0; // �������������
        delay_ms(100); // ��ʱ100����������һ�β���
    }
    length = sum / 3; // �������β�����ƽ������
    return length; // ����ƽ������
}

