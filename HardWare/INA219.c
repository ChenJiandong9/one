#include "INA219.h"

void INA219_Init(void)
{
    // 初始化模式配置寄存器
    INA219_WriteReg(INA219_REG_CONFIG, INA219_CONFIG_VOLTAGE_RANGE_32V | INA219_CONFIG_SADCRES_12BIT_128S_69MS | INA219_CONFIG_MODE_SANDBVOLT_CONTINUOUS);
    // 初始化校准寄存器
    INA219_WriteReg(INA219_REG_CALIBRATION, 0x1000);
}

uint16_t INA219_WriteReg(uint8_t registerAdd, uint16_t data)
{

    MyI2C_Start();
    MyI2C_SendByte((INA219_ADDRESS << 1) | 0x01);
    if (MyI2C_ReceiveAck() == 1)
    {
        MyI2C_Stop();
        return 0;
    }
    MyI2C_SendByte(registerAdd);
    if (MyI2C_ReceiveAck() == 1)
    {
        MyI2C_Stop();
        return 0;
    }
    MyI2C_SendByte(data >> 8);
    if (MyI2C_ReceiveAck() == 1)
    {
        MyI2C_Stop();
        return 0;
    }
    MyI2C_SendByte(data);
    MyI2C_Stop();
}
uint16_t INA219_ReadReg(uint8_t registerAdd)
{
    uint16_t data = 0;
    MyI2C_Start();
    MyI2C_SendByte((INA219_ADDRESS << 1));
    if (MyI2C_ReceiveAck() == 1)
    {
        MyI2C_Stop();
        return 0;
    }
    MyI2C_SendByte(registerAdd);
    if (MyI2C_ReceiveAck() == 1)
    {
        MyI2C_Stop();
        return 0;
    }
    MyI2C_Start();
    MyI2C_SendByte((INA219_ADDRESS << 1) | 0x01);
    if (MyI2C_ReceiveAck() == 1)
    {
        MyI2C_Stop();
        return 0;
    }
    data = MyI2C_ReceiveByte() << 8;
    MyI2C_SendAck(0);
    data |= MyI2C_ReceiveByte();
    MyI2C_SendAck(1);
    MyI2C_Stop();
}