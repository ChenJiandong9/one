#include "usart1.h"

uint8_t Usart1_RxData;
uint8_t Usart1_RxFlag;
void USART1_Init(u32 bound)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    USART_InitTypeDef USART_InitStructure;
    USART_InitStructure.USART_BaudRate = bound;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_Init(USART1, &USART_InitStructure);
    USART_Cmd(USART1, ENABLE);

    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_Init(&NVIC_InitStructure);
}

void USART1_SendByte(uint8_t byte)
{
    USART_SendData(USART1, byte);
    while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET)
        ;
}
void USART1_IRQHandler(void)
{
    if (USART_GetITStatus(USART1, USART_IT_RXNE) == SET)
    {

        USART_ClearITPendingBit(USART1, USART_IT_RXNE);
    }
}

uint8_t Usart1_GetRxFlag(void)
{
    if (Usart1_RxFlag == 1)
    {
        Usart1_RxFlag = 0;
        return 1;
    }
    return 0;
}
uint8_t Usart1_GetRxData(void)
{
    return Usart1_RxData;
}
uint8_t USART1_ReceiveByte(void)
{
    if (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == 1)
    {
        return USART_ReceiveData(USART1);
    }
    return 0;
}
void USART1_SendString(uint8_t *str)
{
    USART1_SendByte(0x40);
    while (*str)
    {
        USART1_SendByte(*str);
        str++;
    }
    USART1_SendByte(0x0D);
    USART1_SendByte(0x0A);
}
void USART1_Send(uint8_t *Data, uint16_t len)
{
    for (uint16_t i = 0; i < len; i++)
    {
        USART1_SendByte(Data[i]);
    }
}