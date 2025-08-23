#include "Motor.h"
static void Motor_GPIO_Init(void);
static void Motor_Time_Init(uint16_t arr, uint16_t psc);
Motor_TypeDef MotorA_Structure, MotorB_Structure, MotorC_Structure, MotorD_Structure;
void Motor_Init(void)
{

    // �����ʼ��
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

    // GPIO��ʼ��
    Motor_GPIO_Init();
    Motor_Time_Init(1000, 72 - 1);
}

static void Motor_GPIO_Init(void)
{

    MOTOR_GPIO_CLK_ENABLE();
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    //////�����XIN����ӿ�
    GPIO_InitStructure.GPIO_Pin = MOTORA_AIN1 | MOTORA_AIN2;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(MOTORA_Port, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = MOTORB_BIN1 | MOTORB_BIN2;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(MOTORB_Port, &GPIO_InitStructure);
    ////////////���C��D����

    // ��ʼ��TIM2��TIM3�ı���ӿ�����
    GPIO_InitStructure.GPIO_Pin = MOTORA_ENCODER_A_PIN | MOTORA_ENCODER_B_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(MOTORA_ENCODER_Port, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = MOTORB_ENCODER_A_PIN | MOTORB_ENCODER_B_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(MOTORB_ENCODER_Port, &GPIO_InitStructure);
}

static void Motor_Time_Init(uint16_t arr, uint16_t psc)
{
    ///////////////�����pwm�ź����ɣ�AB��TIM4
    Motor_Time_CLK_ENABLE();
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseStructure.TIM_Period = arr;
    TIM_TimeBaseStructure.TIM_Prescaler = psc;
    TIM_TimeBaseStructure.TIM_RepetitionCounter = 0; /////////////??????????????????????????
    TIM_TimeBaseInit(Motor_PWM_Time, &TIM_TimeBaseStructure);
    TIM_CtrlPWMOutputs(Motor_PWM_Time, ENABLE); // TIM4����Ҫ

    TIM_OCInitTypeDef TIM_OCInitStructure;
    TIM_OCStructInit(&TIM_OCInitStructure);
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OCInitStructure.TIM_Pulse =arr/2;
    //////////ͨ����ʼ��
    TIM_OC1Init(Motor_PWM_Time, &TIM_OCInitStructure);
    TIM_OC2Init(Motor_PWM_Time, &TIM_OCInitStructure);
    TIM_OC3Init(Motor_PWM_Time, &TIM_OCInitStructure);
    TIM_OC4Init(Motor_PWM_Time, &TIM_OCInitStructure);

    TIM_CtrlPWMOutputs(TIM4, ENABLE); // �߼���ʱ������oc���pwmʱ��Ӵ˾�

    ///////////////////chԤװ��ʹ��
    TIM_OC1PreloadConfig(Motor_PWM_Time, TIM_OCPreload_Enable);
    TIM_OC2PreloadConfig(Motor_PWM_Time, TIM_OCPreload_Enable);
    TIM_OC3PreloadConfig(Motor_PWM_Time, TIM_OCPreload_Enable);
    TIM_OC4PreloadConfig(Motor_PWM_Time, TIM_OCPreload_Enable);

    ///////////////////timԤװ��ʹ��
    TIM_ARRPreloadConfig(Motor_PWM_Time, ENABLE);

    TIM_Cmd(Motor_PWM_Time, ENABLE);

   
 // ����TIM2��TIM3����ӿ�
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
    // TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;  //encoder������������
    TIM_ICInit(TIM3, &TIM_ICInitStructure);
    TIM_ICInit(TIM2, &TIM_ICInitStructure);
    TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;
    TIM_ICInitStructure.TIM_ICFilter = 0xf;
    TIM_ICInit(TIM3, &TIM_ICInitStructure);
    TIM_ICInit(TIM2, &TIM_ICInitStructure);

    TIM_EncoderInterfaceConfig(TIM2, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising); // ������ICInit��������   cnt��Motor_GetConter������ȡ
    TIM_EncoderInterfaceConfig(TIM3, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising); // ������ICInit��������   cnt��Motor_GetConter������ȡ
    TIM_Cmd(TIM2, ENABLE);
    TIM_Cmd(TIM3, ENABLE);
}

/**
 * @brief ��ȡ���״̬����
 * @param Motor ָ�����ṹ���ָ�룬���ڱ�ʶҪ�����ĵ��
 * @retval �޷���ֵ
 *
 * �ú������ݵ����Ż�ȡ��Ӧ�����״̬��Ϣ��
 * ͨ����ȡ��ʱ��������ֵ����ȡ�������ز���
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
            Motor->Motor_Speed = (EncoderA[1] - EncoderA[0]) / 0.01 * e; // �����ٶȹ�ʽspeed=n/t*(2*PI*r/N) n�����ת�ӣ�N��ת�ӣ�r���Ӱ뾶
        }
        else
        {
            Motor->Motor_Direction = CCW;
            Motor->Motor_Speed = (EncoderA[0] - EncoderA[1]) / 0.01 * e; // �����ٶȹ�ʽspeed=n/t*(2*PI*r/N) n�����ת�ӣ�N��ת�ӣ�r���Ӱ뾶
        }
        break;

    case MotorB:
        EncoderB[0] = Motor_GetCounter(&MotorB_Structure);
        delay_ms(10);
        EncoderB[1] = Motor_GetCounter(&MotorB_Structure);
        if (EncoderB[0] < EncoderB[1])
        {
            Motor->Motor_Direction = CW;
            Motor->Motor_Speed = (EncoderB[1] - EncoderB[0]) / 0.01 * e; // �����ٶȹ�ʽspeed=n/t*(2*PI*r/N) n�����ת�ӣ�N��ת�ӣ�r���Ӱ뾶
        }
        else
        {
            Motor->Motor_Direction = CCW;
            Motor->Motor_Speed = (EncoderB[0] - EncoderB[1]) / 0.01 * e; // �����ٶȹ�ʽspeed=n/t*(2*PI*r/N) n�����ת�ӣ�N��ת�ӣ�r���Ӱ뾶
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
            //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!��Ҫ���ԣ�������ת
            
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
            //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!��Ҫ���ԣ�������ת
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
