void Open(){			//开门
	for (int i=0;i<27;i++)		//2毫秒高电平，18毫秒低电平，舵机正转（舵机时序为20毫秒）
	{							//循环27次，相当于延时，到达准确位置
	P1OUT|=BIT6;
	delay(2);
	P1OUT&=~BIT6;
	delay(18);

	}
}
void Close(){				//关门
	for (int i=0;i<20;i++)		//0.5毫秒高电平，19.5毫秒低电平，舵机反转
	{							//循环20次，回到原位
	P1OUT|=BIT6;
	delay(0.5);
	P1OUT&=~BIT6;
	delay(19.5);
	}

}

void Timer_A1_2_init()    //定时器初始化
{
	TA1CTL|= TASSEL_1+MC_1;//ACLK,增计数
	TA1CCTL2=OUTMOD_7;//输出模式为复位/置位，注意CCTL2要写为TA1CCTL2
 
	TA1CCR0=4;//时钟频率为32768HZ，波形32768/CCR0=8192HZ
	TA1CCR2=2;//占空比CCR2/CCR0=2/4，注意CCR2要写成TA1CCR2       TA1.2由P2.4 P2.5输出，选择了P2.4

 	P2SEL|=BIT4;//设置P2.4功能为输出TA1.2信号

	TA1CCTL1=OUTMOD_7;//输出模式为复位/置位
	P2SEL|=BIT1;//设置P2.1功能为输出TA1.1信号
}

void Timer_A1_2_open()//开启扬声器
{
	TA1CCR0=4;//时钟频率为32768HZ，波形32768/CCR0=8192HZ
	TA1CCR2=2;//占空比CCR2/CCR0=2/4，注意CCR2要写成TA1CCR2       TA1.2由P2.4 P2.5输出，选择了P2.4
	//重新执行初始化的设置，防止偶然错误
	P2DIR|=BIT4;//设置P2.4输入输出模式为输出，真正开始输出信号
}

void Timer_A1_2_close()//关闭扬声器
{
	P2DIR&=~BIT4;//设置P2.4输入输出模式为输入，停止输出信号
}

void Motor_Come(){//电机正转，推动开锁装置到位
	P1OUT&=~BIT7;//P1.7低电平，负责转换电机旋转方向
	
	TA1CCR0=100;//PWM调制，原本用来调节电机速度，后来发现电机力量不够，于是选择99%输出
	TA1CCR1=99;

	P2DIR|=BIT1;//P2.1选择输出，真正开始输出信号

	delay(3000);//延时三秒

	P2DIR&=~BIT1;//P2.1选择输入，停止输出信号
}

void Motor_Back(){//电机反转，开锁装置复位
	P1OUT|=BIT7;//P1.7高电平，转换电机选择方向
	TA1CCR0=100;//PWM调制，因为乐高电机根据两端口电压差转动，所以选择1%输出，对于乐高电机是99%的输出
	TA1CCR1=1;

	P2DIR|=BIT1;//P2.1选择输出，乐高电机开始反向转动

	delay(1000);//延时一秒
	P1OUT&=~BIT7;//P1.7低电平
	P2DIR&=~BIT1;//P2.1选择输入，电机停转

}

void setup()
{
  	Serial.begin(9600);//串口通讯，用于与蓝牙通讯，9600波特率
	P1DIR|=BIT6+BIT4+BIT7;//P1.4 1.6 1.7选择输出
	P1OUT&=~(BIT6+BIT7);//P1.6 1.7低电平
	P1OUT|=BIT4;//P1.4 高电平，用作电源vcc因为没有多余vcc口可用
	Timer_A1_2_init();//定时器初始化
}

void loop()
{
  
  if (Serial.available() > 0) {//如果串口有消息 
	   if(Serial.read()=='O') P1OUT|=BIT6;//校验信号
	   Serial.println('Y');//返回信号给手机端
	   Motor_Come();		//正转电机，开锁装置到位
	   delay(100);			//舵机反应有一定延时，延时100ms等待开锁装置完全到位
	   Open();				//舵机正转，开锁
	   P1OUT|=BIT6;			//持续高电平，使舵机保持不动
	   delay(400);			//延时一段时间，使提示音更明显
	   Timer_A1_2_open();	//打开扬声器
	   delay(100);			//100ms的提示音
	   Timer_A1_2_close();	//关闭扬声器
	   delay(4500);			//延时4.5s，加上之前的总共5s，给用户足够时间进门
	   Close();				//舵机反转，关锁
	   delay(100);			//延时100ms，等待舵机完全复位
	   Motor_Back();		//电机反转，整个装置完全复位

  }
}

