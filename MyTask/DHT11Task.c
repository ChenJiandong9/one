#include "DHT11Task.h"
void DHT11Task(void *pvParameters)
{
    while (1)
    {
        DHT11_Rec_Data();
        USART1_Send(rec_data, 4);
        vTaskDelay(1000);
        // delay_ms(1000);
    }
}
