#include "MyI2C.h"
void MyI2C_W_CLK(uint8_t BitValue)
{
    GPIO_WriteBit(I2C_Pport, I2C_SCL, (BitAction)BitValue);
    delay_us(10);
}
void MyI2C_W_SDA(uint8_t BitValue)
{
    GPIO_WriteBit(I2C_Pport, I2C_SDA, (BitAction)BitValue);
    delay_us(10);
}
uint8_t MyI2C_R_SDA(void)
{
    uint8_t BitValue;
    BitValue = GPIO_ReadInputDataBit(I2C_Pport, I2C_SDA);
    delay_us(10);
    return BitValue;
}
void MyI2C_Init(void)
{
    I2C_GPIO_CLK_ENABLE();
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
    GPIO_InitStructure.GPIO_Pin = I2C_SCL | I2C_SDA;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(I2C_Pport, &GPIO_InitStructure);
    GPIO_SetBits(I2C_Pport, I2C_SCL | I2C_SDA);
}

void MyI2C_Start(void)
{
    MyI2C_W_CLK(1);
    MyI2C_W_SDA(1);
    MyI2C_W_SDA(0);
    MyI2C_W_CLK(0);
}
void MyI2C_Stop(void)
{
    MyI2C_W_SDA(0);
    MyI2C_W_CLK(1);
    MyI2C_W_SDA(1);
}
void MyI2C_SendByte(uint8_t Byte)
{

    for (uint8_t i = 0; i < 8; i++)
    {
        MyI2C_W_SDA(Byte & (0x80 >> i));
        MyI2C_W_CLK(1);
        MyI2C_W_CLK(0);
    }
}
uint8_t MyI2C_ReceiveByte(void)

{
    uint8_t Byte = 0;
    MyI2C_W_SDA(1);
    for (uint8_t i = 0; i < 8; i++)
    {
        MyI2C_W_CLK(1);
        if (MyI2C_R_SDA())
        {
            Byte |= (0x80 >> i);
        }
        MyI2C_W_CLK(0);
    }
    return Byte;
}
void MyI2C_SendAck(uint8_t AckBit)
{

    MyI2C_W_SDA(AckBit);
    MyI2C_W_CLK(1);
    MyI2C_W_CLK(0);
}
uint8_t MyI2C_ReceiveAck(void)

{
    uint8_t AckBit = 0;
    MyI2C_W_SDA(1);

    MyI2C_W_CLK(1);
    AckBit = MyI2C_R_SDA();
    MyI2C_W_CLK(0);

    return AckBit;
}

/*
 将addr 和 w/r合并
 */
uint8_t MyI2C_ADDWR(uint8_t addr, uint8_t w_r)
{
    if (w_r==1)
        return ((addr<<1) | 0x01 );
    else
        return (addr<<1);
}
