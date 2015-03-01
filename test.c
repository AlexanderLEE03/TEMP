#include "stdio.h"
#include "stdlib.h"
#include "string.h"
typedef unsigned char   uint8;
typedef unsigned short  uint16;
//#define TRUE 1
//#define FALSE 0
typedef enum _bool{FALSE=0,TRUE=1}bool;
#define HEX2ASCII(HEX,ASCH,ASCL)  {\
	ASCH=((HEX)&0xF0)>>4;\
	if(ASCH<10)	ASCH+='0';\
	else		ASCH+='7';\
	ASCL=((HEX)&0x0F);\
	if(ASCL<10)	ASCL+='0';\
	else		ASCL+='7';\
	}

//两个ASCII字符转为一字节十六进制数
#define ASCII2HEX(HEX,ASCH,ASCL,HIGH,LOW)  {\
	if(ASCH<'A')	HIGH=ASCH-'0';\
	else		HIGH=ASCH-'7';\
	if(ASCL<'A')	LOW=ASCL-'0';\
	else		LOW=ASCL-'7';\
	HEX=(HIGH<<4)|LOW;\
	}

uint8  buf[25]={0};
uint8  number=0;


uint8 CvtRecBuf(uint8 *asciibuf,uint8 *hexbuf,uint8 number)  //直接转换接收缓冲区，不传参数
{
	char i;
	uint8 tmp;
	uint8 index=0;
	uint8 high;
	uint8 low;

	for(i=0;i<=number-1;i+=2)	//每次转换两个ASCII字符到一个HEX字节
	{
		ASCII2HEX(tmp, asciibuf[i], asciibuf[i+1],high,low);
		hexbuf[index++]=tmp;	//存入其他的缓冲区
	}
	return index;//这个是去掉@ 和0d 后将剩余的ascii每两个转换成一个hex的数量。
	//RecCount=RecCount>>1;	//长度减半

}
void *osal_memset( void *dest, uint8 value, int len )
{
	return memset( dest, value, len );
}
void my_itoa(uint16 num, uint8 *buf, uint8 radix)
{
	uint8 c,i;
	uint8 *p, rst[5];

	p = rst;
	for ( i=0; i<5; i++,p++ )
	{
		c = num % radix;  // Isolate a digit
		*p = c + (( c < 10 ) ? '0' : '7');  // Convert to Ascii
		num /= radix;
		if ( !num )
			break;
	}


	for ( c=0 ; c<=i; c++ )
		*buf++ = *p--;  // Reverse character order

	*buf = '\0';
}
void D4toC(char * c,float d)
{
	/*BYTE*/char    i = 0;
	char Jiema = 0;
	char    inbyte1[30];
	bool    ShuFu = FALSE, JieFu = FALSE;
	int     inbyte2 = 0, inbyte3 = 0, inbyte4 = 0;
	char    afterbyte2[30], afterbyte3[30], afterbyte4[30];
	float   F_afterbyte2 = 0, F_afterbyte3 = 0; 
	float F_afterbyte4 = 0;
	int j;
	memset(inbyte1, 0x30, sizeof(inbyte1));
	memset(afterbyte2, 0x30, sizeof(afterbyte2));
	memset(afterbyte3, 0x30, sizeof(afterbyte3));
	memset(afterbyte4, 0x30, sizeof(afterbyte4));

	inbyte1[10] = 0x0;
	afterbyte2[10] = 0x0;
	afterbyte3[10] = 0x0;
	afterbyte4[10] = 0x0;

	if(d == 0)
	{
		for(j = 0; j < 8; j++)
			c[j] = 0x30;
		return;
	}
	if(d < 0)
	{
		ShuFu = TRUE;
		d = (-1) * d;
	}

	while(d > 1)
	{
		d =(float)(d / 2.0);
		i ++;
	}

	while(d <= 0.5)
	{
		JieFu = TRUE;
		d = (float)(d * 2.0);
		i ++;
	}

	if(d == 1)
	{
		for( j = 2; j < 8; j++)
			c[j] = 0x46;		
	}	
	else
	{	
		inbyte2 = (int)(d * 256);
		F_afterbyte2 = (d * 256) - (int)(d * 256);
		inbyte3 = (int)(F_afterbyte2 * 256);
		F_afterbyte3 = (F_afterbyte2 * 256) - (int)(F_afterbyte2 * 256);
		inbyte4 = (int)(F_afterbyte3 * 256);
		F_afterbyte4 = (F_afterbyte3 * 256) - (int)(F_afterbyte3 * 256);

		my_itoa(inbyte2, afterbyte2, 16);
		my_itoa(inbyte3, afterbyte3, 16);
		my_itoa(inbyte4, afterbyte4, 16);

		if(inbyte2 == 0)
		{
			c[2] = 0x30;
			c[3] = 0x30;
		}
		else if(inbyte2 < 16)
		{
			c[2] = 0x30;
			c[3] = afterbyte2[0];
		}
		else
		{
			c[2] = afterbyte2[0];
			c[3] = afterbyte2[1];
		}
		if(inbyte3 == 0)
		{
			c[4] = 0x30;
			c[5] = 0x30;
		}
		else if(inbyte3 < 16)
		{
			c[4] = 0x30;
			c[5] = afterbyte3[0];
		}
		else
		{
			c[4] = afterbyte3[0];
			c[5] = afterbyte3[1];
		}
		if(inbyte4 == 0)
		{
			c[6] = 0x30;
			c[7] = 0x30;
		}
		else if(inbyte4 < 16)
		{
			c[6] = 0x30;
			c[7] = afterbyte4[0];
		}
		else
		{
			c[6] = afterbyte4[0];
			c[7] = afterbyte4[1];
		}
	}

	if(JieFu)
	{
		if(i > 0x3f)
			i = 0x3f;
	}
	else if(i > 0x32)
		i = 32;
	if(ShuFu)
		i = i | 0x80;
	if(JieFu)
		i = i | 0x40;
	my_itoa(i, inbyte1, 16);

	if(inbyte1 == 0)
	{
		c[0] = 0x30;
		c[1] = 0x30;
	}
	else if(i < 16)
	{
		c[0] = 0x30;
		c[1] = inbyte1[0];
	}
	else
	{
		c[0] = inbyte1[0];
		c[1] = inbyte1[1];
	}

	for(i = 0; i < 8; i ++)
	{
		if((c[i] > 0x60) && (c[i] < 0x67))
			c[i] = c[i] - 0x20;
	}

	c[8] = 0x00;
}

int main()
{
	
	 float a;
	 uint8 s[10]={0};
L1:
	 scanf("%f",&a);

	 D4toC(s,a);
	 printf("%s\n",s);
	 goto L1;
}