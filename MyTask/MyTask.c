#include "MyTask.h"

SemaphoreHandle_t xSemaphore_MyI2C;
TaskHandle_t ReadLuxTask_Handle, ReadTempAndHumTask_Handle;
void start_task(void *pvParameters)
{
    taskENTER_CRITICAL(); // �ٽ�����ʼ
    xSemaphore_MyI2C = xSemaphoreCreateBinary();
    xSemaphoreGive(xSemaphore_MyI2C);
    xTaskCreate(ReadLuxTask, "ReadLuxTask", STK_SIZE, NULL, MID_PRIORITY, &ReadLuxTask_Handle);
    xTaskCreate(DHT11Task, "DHT11Task", STK_SIZE, NULL, MID_PRIORITY, &ReadTempAndHumTask_Handle);

    vTaskDelete(NULL);   //??????????????
    taskEXIT_CRITICAL(); // �ٽ�������
}
void ReadTemAndHumTask(void *pvParameters)
{
    while (1)
    {
        DHT11_Rec_Data();
        // USART1_Send(rec_data, 4);
        vTaskDelay(1000);
        // delay_ms(1000);
    }
}

// ��ȡ����ǿ�ȣ�ʹ��i2c
void ReadLuxTask(void *pvParameters)
{
    uint8_t Buff[2];
    // ���Ͳ���ָ���ȡ
    uint8_t command = BH1750_ORDER_MEASUREMENT;

    // ͨ��xSemaphore_MyI2C�źű����ͻ
    xSemaphoreTake(xSemaphore_MyI2C, portMAX_DELAY);
    MyI2C_Write(BH1750_ADDRESS, &command, 1);
    xSemaphoreGive(xSemaphore_MyI2C);
    vTaskDelay(19);
    xSemaphoreTake(xSemaphore_MyI2C, portMAX_DELAY);
    MyI2c_Read(BH1750_ADDRESS, Buff, 2);
    xSemaphoreGive(xSemaphore_MyI2C);

    (float)((Buff[0] << 8) | Buff[1]); //!!!!!!!!!!!!!!!!!!
}