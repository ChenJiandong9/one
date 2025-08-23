// #include "KeyTask.h"
// void KeyTask(void *pvParameters)
// {

//     KEY1_GPIO_CLK_ENABLE();
//     KEY2_GPIO_CLK_ENABLE();
//     GPIO_InitTypeDef GPIO_InitStructure;
//     GPIO_InitStructure.GPIO_Pin = KEY1_Pin;
//     GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
//     GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//     GPIO_Init(KEY1_Port, &GPIO_InitStructure);
//     GPIO_InitStructure.GPIO_Pin = KEY2_Pin;
//     GPIO_Init(KEY2_Port, &GPIO_InitStructure);
//     extern TaskHandle_t OledShowTask1Handler;
//     extern TaskHandle_t OledShowTask2Handler;
//     while (1)
//     {
//         if (GPIO_ReadInputDataBit(KEY1_Port, KEY1_Pin) == 0)
//         {
//             vTaskDelay(20);
//             while (GPIO_ReadInputDataBit(KEY1_Port, KEY1_Pin) == 0)
//                 ;
//             vTaskDelay(20);

//             vTaskDelete(OledShowTask1Handler);
//             OledShowTask1Handler = NULL;
//         }
//         if (GPIO_ReadInputDataBit(KEY2_Port, KEY2_Pin) == 0)
//         {
//             vTaskDelay(10);
//             while (GPIO_ReadInputDataBit(KEY2_Port, KEY2_Pin) == 0)
//                 ;
//             vTaskDelay(10);

//             vTaskDelete(OledShowTask2Handler);
//             OledShowTask2Handler = NULL;
//         }
//     }
// }
