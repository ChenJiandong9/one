#include "BH1750.h"
void BH1750_Init(void)
{
    MyI2C_Init();
}

void BH1750_Power(void)
{
    MyI2C_Start();
    MyI2C_SendByte(MyI2C_ADDWR(BH1750_ADDRESS, 0));
    if (MyI2C_ReceiveAck())
    {
        MyI2C_Stop();
        return;
    }
    MyI2C_SendByte(BH1750_ORDER_POWER);
    if (MyI2C_ReceiveAck())
    {
        MyI2C_Stop();
        return;
    }
    MyI2C_Stop();
}
void BH1750_Measurement(void)
{
    MyI2C_Start();
    MyI2C_SendByte(MyI2C_ADDWR(BH1750_ADDRESS, 0));
    if (MyI2C_ReceiveAck())
    {
        MyI2C_Stop();
        return;
    }
    MyI2C_SendByte(BH1750_ORDER_MEASUREMENT);
    if (MyI2C_ReceiveAck())
    {
        MyI2C_Stop();
        return;
    }
    MyI2C_Stop();
}

/*
    读取光照强度
*/
float BH1750_ReadLux(void)
{
    uint16_t data;
    BH1750_Power();
    BH1750_Measurement();
    delay_ms(200);

    MyI2C_Start();
    MyI2C_SendByte(MyI2C_ADDWR(BH1750_ADDRESS, 1));
    if (MyI2C_ReceiveAck())
    {
        MyI2C_Stop();
        return 0;
    }
    data = MyI2C_ReceiveByte() << 8;
    MyI2C_SendAck(0);
    data |= MyI2C_ReceiveByte();
    MyI2C_SendAck(1);
    MyI2C_Stop();
    return data * BH1750_RESOLUTION / 1.2;
}
