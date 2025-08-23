#include "Motor.h"
static void Motor_GPIO_Init(void);
static void Motor_Time_Init(uint16_t arr, uint16_t psc);
Motor_TypeDef MotorA_Structure, MotorB_Structure, MotorC_Structure, MotorD_Structure;
void Motor_Init(void)
{

    // 电机初始化
    MotorA_Structure.Motor_Direction = CW;
    MotorA_Structure.Motor_Num = MotorA;
    MotorA_Structure.Motor_Speed = 0;
    MotorB_Structure.Motor_Direction = CW;
    MotorB_Structure.Motor_Num = MotorB;
    MotorB_Structure.Motor_Speed = 0;
    MotorC_Structure.Motor_Direction = CW;
    MotorC_Structure.Motor_Num = MotorC;
    MotorC_Structure.Motor_Speed = 0;
    MotorD_Structure.Motor_Direction = CW;
    MotorD_Structure.Motor_Num = MotorD;
    MotorD_Structure.Motor_Speed = 0;

    // GPIO初始化
    Motor_GPIO_Init();
    Motor_Time_Init(1000, 72 - 1);
}

static void Motor_GPIO_Init(void)
{

    MOTOR_GPIO_CLK_ENABLE();
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    //////电机的XIN输出接口
    GPIO_InitStructure.GPIO_Pin = MOTORA_AIN1 | MOTORA_AIN2;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(MOTORA_Port, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = MOTORB_BIN1 | MOTORB_BIN2;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(MOTORB_Port, &GPIO_InitStructure);
    ////////////电机C，D类似

    // 初始化TIM2，TIM3的编码接口引脚
    GPIO_InitStructure.GPIO_Pin = MOTORA_ENCODER_A_PIN | MOTORA_ENCODER_B_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(MOTORA_ENCODER_Port, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = MOTORB_ENCODER_A_PIN | MOTORB_ENCODER_B_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(MOTORB_ENCODER_Port, &GPIO_InitStructure);
}

static void Motor_Time_Init(uint16_t arr, uint16_t psc)
{
    ///////////////电机的pwm信号生成，AB用TIM4
    Motor_Time_CLK_ENABLE();
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseStructure.TIM_Period = arr;
    TIM_TimeBaseStructure.TIM_Prescaler = psc;
    TIM_TimeBaseStructure.TIM_RepetitionCounter = 0; /////////////??????????????????????????
    TIM_TimeBaseInit(Motor_PWM_Time, &TIM_TimeBaseStructure);
    TIM_CtrlPWMOutputs(Motor_PWM_Time, ENABLE); // TIM4不需要

    TIM_OCInitTypeDef TIM_OCInitStructure;
    TIM_OCStructInit(&TIM_OCInitStructure);
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OCInitStructure.TIM_Pulse =arr/2;
    //////////通道初始化
    TIM_OC1Init(Motor_PWM_Time, &TIM_OCInitStructure);
    TIM_OC2Init(Motor_PWM_Time, &TIM_OCInitStructure);
    TIM_OC3Init(Motor_PWM_Time, &TIM_OCInitStructure);
    TIM_OC4Init(Motor_PWM_Time, &TIM_OCInitStructure);

    TIM_CtrlPWMOutputs(TIM4, ENABLE); // 高级定时器在用oc输出pwm时需加此句

    ///////////////////ch预装载使能
    TIM_OC1PreloadConfig(Motor_PWM_Time, TIM_OCPreload_Enable);
    TIM_OC2PreloadConfig(Motor_PWM_Time, TIM_OCPreload_Enable);
    TIM_OC3PreloadConfig(Motor_PWM_Time, TIM_OCPreload_Enable);
    TIM_OC4PreloadConfig(Motor_PWM_Time, TIM_OCPreload_Enable);

    ///////////////////tim预装载使能
    TIM_ARRPreloadConfig(Motor_PWM_Time, ENABLE);

    TIM_Cmd(Motor_PWM_Time, ENABLE);

   
 // 配置TIM2，TIM3编码接口
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_Prescaler = 1 - 1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseStructure.TIM_Period = 65536 - 1;
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

    TIM_ICInitTypeDef TIM_ICInitStructure;
    TIM_ICStructInit(&TIM_ICInitStructure);
    TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;
    TIM_ICInitStructure.TIM_ICFilter = 0xf;
    // TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;  //encoder函数那配置了
    TIM_ICInit(TIM3, &TIM_ICInitStructure);
    TIM_ICInit(TIM2, &TIM_ICInitStructure);
    TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;
    TIM_ICInitStructure.TIM_ICFilter = 0xf;
    TIM_ICInit(TIM3, &TIM_ICInitStructure);
    TIM_ICInit(TIM2, &TIM_ICInitStructure);

    TIM_EncoderInterfaceConfig(TIM2, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising); // 必须再ICInit函数下面   cnt用Motor_GetConter函数获取
    TIM_EncoderInterfaceConfig(TIM3, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising); // 必须再ICInit函数下面   cnt用Motor_GetConter函数获取
    TIM_Cmd(TIM2, ENABLE);
    TIM_Cmd(TIM3, ENABLE);
}

/**
 * @brief 获取电机状态函数
 * @param Motor 指向电机结构体的指针，用于标识要操作的电机
 * @retval 无返回值
 *
 * 该函数根据电机编号获取对应电机的状态信息，
 * 通过读取定时器计数器值来获取电机的相关参数
 */
void Motor_GetState(Motor_TypeDef *Motor)
{
    uint16_t EncoderA[2];
    uint16_t EncoderB[2];

    switch (Motor->Motor_Num)
    {
    case MotorA:
        EncoderA[0] = Motor_GetCounter(&MotorA_Structure);
        delay_ms(10);
        EncoderA[1] = Motor_GetCounter(&MotorA_Structure);
        if (EncoderA[0] < EncoderA[1])
        {
            Motor->Motor_Direction = CW;
            Motor->Motor_Speed = (EncoderA[1] - EncoderA[0]) / 0.01 * e; // 计算速度公式speed=n/t*(2*PI*r/N) n捕获的转子，N总转子，r轮子半径
        }
        else
        {
            Motor->Motor_Direction = CCW;
            Motor->Motor_Speed = (EncoderA[0] - EncoderA[1]) / 0.01 * e; // 计算速度公式speed=n/t*(2*PI*r/N) n捕获的转子，N总转子，r轮子半径
        }
        break;

    case MotorB:
        EncoderB[0] = Motor_GetCounter(&MotorB_Structure);
        delay_ms(10);
        EncoderB[1] = Motor_GetCounter(&MotorB_Structure);
        if (EncoderB[0] < EncoderB[1])
        {
            Motor->Motor_Direction = CW;
            Motor->Motor_Speed = (EncoderB[1] - EncoderB[0]) / 0.01 * e; // 计算速度公式speed=n/t*(2*PI*r/N) n捕获的转子，N总转子，r轮子半径
        }
        else
        {
            Motor->Motor_Direction = CCW;
            Motor->Motor_Speed = (EncoderB[0] - EncoderB[1]) / 0.01 * e; // 计算速度公式speed=n/t*(2*PI*r/N) n捕获的转子，N总转子，r轮子半径
        }
        break;
        // case MotorC:

        //     break;
        // case MotorD:

        //     break;

    default:
        break;
    }
}
void Motor_SetSpeed(Motor_TypeDef *Motor)
{
    switch (Motor->Motor_Num)
    {
    case MotorA:
        if (Motor->Motor_Direction == CW)
        {
            //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!需要测试，看正反转
            
            TIM_SetCompare1(Motor_PWM_Time, Motor->Motor_Speed);
            TIM_SetCompare2(Motor_PWM_Time, 0);
        }
        else
        {
            GPIO_SetBits(MOTORA_Port, MOTORA_AIN1);
            GPIO_ResetBits(MOTORA_Port, MOTORA_AIN2);
            TIM_SetCompare1(Motor_PWM_Time, Motor->Motor_Speed);
        }
        break;

    case MotorB:

        if (Motor->Motor_Direction == CW)
        {
            //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!需要测试，看正反转
            GPIO_ResetBits(MOTORA_Port, MOTORB_BIN1);
            GPIO_SetBits(MOTORA_Port, MOTORB_BIN2);
            TIM_SetCompare2(Motor_PWM_Time, Motor->Motor_Speed);
        }
        else
        {
            GPIO_SetBits(MOTORA_Port, MOTORB_BIN1);
            GPIO_ResetBits(MOTORA_Port, MOTORB_BIN2);
            TIM_SetCompare2(Motor_PWM_Time, Motor->Motor_Speed);
        }
        break;
        // case MotorC:

        //     break;
        // case MotorD:

        //     break;
    default:
        break;
    }
}
int16_t Motor_GetCounter(Motor_TypeDef *Motor)
{
    int16_t cnt;
    switch (Motor->Motor_Num)
    {
    case MotorA:
        cnt = TIM_GetCounter(TIM2);
        if (cnt > 30000 || cnt < -30000)
        {
            TIM_SetCounter(TIM2, 0);
            return 0;
        }
        return cnt;

    case MotorB:

        cnt = TIM_GetCounter(TIM3);
        if (cnt > 30000 || cnt < -30000)
        {
            TIM_SetCounter(TIM3, 0);
            return 0;
        }
        return cnt;

        /*
        case MotorC:

         cnt = TIM_GetConter(TIM2);
        if (cnt > 30000 || cnt < -30000)
        {
            TIM_SetCounter(TIM2, 0);return 0;
            }
            return cnt;

            break;
            case MotorD:
         cnt = TIM_GetConter(TIM2);
            if (cnt > 30000 || cnt < -30000)
            {
                TIM_SetCounter(TIM2, 0);return 0;
            }
            return cnt;


            break;
            */

    default:
        break;
    }
}
