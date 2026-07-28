#ifndef __24CXX_H
#define __24CXX_H
#include "iic.h"   

#define AT24C01		127
#define AT24C02		255
#define AT24C04		511
#define AT24C08		1023
#define AT24C16		2047
#define AT24C32		4095
#define AT24C64	    8191
#define AT24C128	16383
#define AT24C256	32767  
//Mini STM32开发板使用的是24c02，所以定义EE_TYPE为AT24C02
#define EE_TYPE AT24C02
					  
u8 AT24CXX_ReadOneByte(u16 ReadAddr);							//指定地址读取一个字节
void AT24CXX_WriteOneByte(u16 WriteAddr,u8 DataToWrite);		//指定地址写入一个字节
void AT24CXX_WriteLenByte(u16 WriteAddr,u32 DataToWrite,u8 Len);//指定地址开始写入指定长度的数据
u32 AT24CXX_ReadLenByte(u16 ReadAddr,u8 Len);					//指定地址开始读取指定长度数据
void AT24CXX_Write(u16 WriteAddr,u8 *pBuffer,u16 NumToWrite);	//从指定地址开始写入指定长度的数据
void AT24CXX_Read(u16 ReadAddr,u8 *pBuffer,u16 NumToRead);   	//从指定地址开始读出指定长度的数据

u8 AT24CXX_Check(void);  //检查器件
void AT24CXX_Init(void); //初始化IIC

/*OTA 升级标志位*/

#define OTA_SUCCESS_FLAG    0x33AA55AA      //OTA升级完成的标志0x55AA55AA：升级完成后会将这个标志写入24C02, 用于判断是否需要升级
#define OTA_CLEAR_FLAG      0x11111111    //清除OTA标志位

typedef struct
{
	u32 OTA_Flag;
} OTA_Struct;
	
extern OTA_Struct OTA_Info;


void OTA_Check_Flag(void); //检查OTA标志位
void OTA_Write_Flag(u32 OTA_Flag);   //写入OTA标志位
u32 OTA_Read_Flag(void);  //读取OTA标志位

#endif
















