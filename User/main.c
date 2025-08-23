#include "stm32f10x.h" // Device header
// #include "FreeRTOS.h"
// #include "Task.h"
#include "usart1.h"
// #include "DHT11Task.h"
#include "delay.h"
// #include "BH1750.h"
// #include "queue.h"
#include "RGB.h"

// typedef struct
// {
//     uint8_t x;
//     uint8_t y;
//     unsigned char name[16];
// } TaskPrintInfo;
// TaskHandle_t myTaskHandler;
// TaskHandle_t OledShowTask2Handler;
// TaskHandle_t OledShowTask1Handler;
// TaskHandle_t DHT11TaskHandler;
// TaskPrintInfo g_Task1Info = {0, 0, "Task1"};
// TaskPrintInfo g_Task2Info = {0, 16, "Task2"};
// TaskPrintInfo g_Task3Info = {0, 32, "Task3"};
// static uint8_t g_OledCanUse = 1;
// void myTask(void *argument)
// {
//     RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
//     GPIO_InitTypeDef GPIO_InitStructure;
//     GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
//     GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
//     GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//     GPIO_Init(GPIOB, &GPIO_InitStructure);

//     while (1)
//     {
//         GPIO_ResetBits(GPIOB, GPIO_Pin_5);
//         vTaskDelay(1000);
//         GPIO_SetBits(GPIOB, GPIO_Pin_5);
//         vTaskDelay(1000);
//     }
// }
// void myTask1(void *argument)
// {
//     RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
//     GPIO_InitTypeDef GPIO_InitStructure;
//     GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
//     GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
//     GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//     GPIO_Init(GPIOB, &GPIO_InitStructure);

//     while (1)
//     {
//         GPIO_ResetBits(GPIOB, GPIO_Pin_0);
//         vTaskDelay(1000);
//         GPIO_SetBits(GPIOB, GPIO_Pin_0);
//         vTaskDelay(1000);
//     }
// }
// // void OledShowTask(void *arguments)
// // {
// //     TaskPrintInfo *pInfo = (TaskPrintInfo *)arguments;

// //     uint16_t cnt = 0;
// //     while (1)
// //     {
// //         while (g_OledCanUse)
// //         {
// //             g_OledCanUse = 0;
// //             OLED_ShowString(pInfo->x, pInfo->y, pInfo->name, 16);
// //             OLED_ShowNum(48, pInfo->y, cnt, 5, 16);
// //         }
// //         g_OledCanUse = 1;
// //         cnt++;
// //         vTaskDelay(1000);
// //     }
// // }
// xQueueHandle LuxQueue;
// void BH1750Task(void *argument)
// {
//     float lux = 0;
//     while (1)
//     {
//         lux = BH1750_ReadLux();
//         xQueueOverwrite(LuxQueue, &lux);
//         vTaskDelay(5000);
//     }
// }
// void Usart1SendTask(void *argument)
// {
//     float lux = 0;
//     while (1)
//     {
//         xQueuePeek(LuxQueue, &lux, portMAX_DELAY);
//         USART1_Send((uint8_t *)&lux, sizeof(float));
//         vTaskDelay(1000);
//     }
// }
int main()
{

    // xTaskCreate(myTask1, "Task1", 128, NULL, 2, NULL);
    SystemInit();
    delay_Init();
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    GPIO_SetBits(GPIOA, GPIO_Pin_2);
    // BH1750_Init();
    // USART1_Init(115200);

    // 创建消息队列
    // LuxQueue = xQueueCreate(1, sizeof(float));

    // xTaskCreate(myTask, "Task", 128, NULL, 2, NULL);
    // xTaskCreate(BH1750Task, "DHT11Task", 128, NULL, 2, NULL);
    // xTaskCreate(Usart1SendTask, "DHT11Task", 128, NULL, 2, NULL);
    // vTaskStartScheduler();
    RGB_Init();
    // RGB_Set(50, 0, 255);
    extern uint8_t Usart1_RxFlag;
    extern uint8_t Usart1_RxData;
    uint8_t rgb[3];
    uint8_t i = 0;
    while (1)

    {
        if (Usart1_RxFlag == 1)
        {
            Usart1_RxFlag = 0;
            i++;
            i = i % 3;
            if (i == 0)
            {
                RGB_Set(rgb[0], rgb[1], rgb[2]);
            }
        }
        RGB_Set(255, 0, 0);
        delay_ms(1000);
    }
}
// extern TaskHandle_t myTaskHandler;
// if (myTaskHandler != NULL)
// {
//     vTaskDelete(myTaskHandler);
//     myTaskHandler = NULL;
// }
