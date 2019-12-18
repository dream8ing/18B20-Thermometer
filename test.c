#include <reg51.h>
#include <intrins.h>
sbit DQ = P2^7 ;
unsigned char dis[]={0xc0,0xf9,0xa4,0xb0,0x99,
0x92,0x82,0xf8,0x80,0x90};

unsigned char temp;

void delayms(unsigned int j)
{
	unsigned char i;
	while(j--)
	{
		for(i=0;i<125;i++);
	}
}

void delay5(unsigned char n)
{
	do
	{
		_nop_();
		_nop_();
		_nop_();
		n--;
	}while(n);
}

/* 两位温度 */
void display()
{
	unsigned char ge,shi,bai,a;
	if(temp<=255&temp>128)
	{temp=256-temp;
	 P2=0X08;
	P0=0Xbf;

	}  delayms(3);

	ge = temp % 10;
    a = temp / 10;
	shi= a %10;
	bai= temp / 100;
	/*个位*/
	P2 = 0x01;	//0000 0100
	P0 = dis[ge];	
	delayms(3);
	/*十位*/
	P2 = 0x02;	//0000 0010
	P0 = dis[shi];
	delayms(3);
	P2=0x04;	 //0000 0001
	P0=	dis[bai];
	delayms(3);
	
}

void init_ds18b20()
{
	unsigned char x = 0;
	DQ = 0;
	delay5(120);
	DQ = 1;
	delay5(16);
	delay5(80);
}

unsigned char readbyte()
{
	unsigned char i = 0;
	unsigned char date = 0;
	for(i=8;i>0;i--)
	{
		DQ = 0;
		delay5(1);
		DQ = 1;
		date >>= 1;
		if(DQ)
		date |= 0x80;
		delay5(11);
	}
	return(date);
}

void writebyte(unsigned char dat)
{
	unsigned char i = 0;
	for(i=8;i>0;i--)
	{
		DQ = 0;
		DQ = dat & 0x01;
		delay5(12);
		DQ = 1;
		dat >>= 1;
		delay5(5);
	}
}

unsigned char read_temp()
{
	unsigned char t,a,b;
	unsigned int tt;
	init_ds18b20();		//1、初始化DS18B20
	writebyte(0xcc);	//2、跳过读序列号
	writebyte(0x44);	//3、启动温度转换
	delayms(10);
	init_ds18b20();		//4、初始化DS18B20
	writebyte(0xcc);	//5、跳过读序列号
	writebyte(0xbe);	//6、发送读温度的命令给DS18B20
	a = readbyte();		//7、单片机接收温度16位
	b = readbyte();
	tt = b;
	tt = tt << 8;		//'b' + 0000 0000
	tt = tt | a; 		//'b' + 'a'
	t = tt / 16;
	return(t);
}

void main()
{
	while(1)
	{
		temp = read_temp();
		display();
	}
}