#include "user.h"

//float Kp = 0.7, Ki = 10, Kd;
//BOOL ctrl_en = TRUE;
//float sp_val = 600;
//float cur_val;

float Kp = 0.1, Ki = 0, Kd;
BOOL ctrl_en = TRUE;
float sp_val = 360;
float cur_val;

void CLK_Init(void)
{
    /* Periph clock enable */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
}

void PIO_Init(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;

    /* Configure PD12, PD13, PD14 and PD15 in output pushpull mode */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13| GPIO_Pin_14| GPIO_Pin_15;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOD, &GPIO_InitStructure);

    /* Configure PA0 input mode */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

}

void PWM_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_OCInitTypeDef TIM_OCInitStructure;

    /* TIM clock enable */
    RCC_APB1PeriphClockCmd(PWM_TIM_CLK, ENABLE);
    
    /* Pulse pin configuration */
    GPIO_InitStructure.GPIO_Pin = PWM_PULSE;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;
    GPIO_Init(PWM_PULSE_PORT, &GPIO_InitStructure); 
    GPIO_PinAFConfig(PWM_PULSE_PORT, PWM_PULSE_SOURCE, PWM_PULSE_AF);

    /* DIR, EN pins configuration */
    GPIO_InitStructure.GPIO_Pin = PWM_DIR;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(PWM_DIR_PORT, &GPIO_InitStructure); 

    GPIO_InitStructure.GPIO_Pin = PWM_EN;
    GPIO_Init(PWM_EN_PORT, &GPIO_InitStructure);

    /* TIM CH1 configuration */
    TIM_TimeBaseStructure.TIM_Period = PWM_PERIOD;
    TIM_TimeBaseStructure.TIM_Prescaler = PWM_PRESCALER - 1;
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(PWM_TIM, &TIM_TimeBaseStructure);

    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse = 0;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OC1Init(PWM_TIM, &TIM_OCInitStructure);
    TIM_OC1PreloadConfig(PWM_TIM, TIM_OCPreload_Enable);

    TIM_Cmd(PWM_TIM, ENABLE);
}


void ENC_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    /* TIM clock enable */
    RCC_APB2PeriphClockCmd(ENC_TIM_CLK, ENABLE);

    /* TIM channel1,2 configuration */
    GPIO_InitStructure.GPIO_Pin   = ENC_A | ENC_B;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
    GPIO_Init(ENC_PORT, &GPIO_InitStructure);

    GPIO_PinAFConfig(ENC_PORT, ENC_A_SOURCE, ENC_AF);
    GPIO_PinAFConfig(ENC_PORT, ENC_B_SOURCE, ENC_AF);

    TIM_EncoderInterfaceConfig(ENC_TIM, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);

    /* TIM enable counter */
    TIM_Cmd(ENC_TIM, ENABLE);

}

void TIM_Init(void)
{
    TIM_TimeBaseInitTypeDef TimStruct;
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);

    // 1ms for TIM 84Mhz
    TimStruct.TIM_CounterMode = TIM_CounterMode_Up;
    TimStruct.TIM_Period = 8400;
    TimStruct.TIM_Prescaler = 9;
    TimStruct.TIM_ClockDivision = 0;
    TimStruct.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIM4, &TimStruct);
}

void BRD_Init(void)
{
    CLK_Init();
    PIO_Init();
    TIM_Init();
    PWM_Init();
    ENC_Init();
}

int32_t ENC_Get(void)
{
    return (int32_t)ENC_TIM->CNT;
}

void PWM_Set(int16_t d)
{
    if (d == 0)
    {
        PWM_EN_PORT->BSRRH = PWM_EN; // disable
    }
    else if (d > 0)
    {
        PWM_EN_PORT->BSRRL = PWM_EN; // enable
        PWM_DIR_PORT->BSRRL = PWM_DIR; //1, forward
    }
    else // (d<0)
    {
        PWM_EN_PORT->BSRRL = PWM_EN; // enable
        PWM_DIR_PORT->BSRRH = PWM_DIR; //0,  backward
        d = -d;
    }
    PWM_TIM->CCR1 = (uint32_t)d;

}


void DelayTms(uint32_t T)
{
    while(T--)
    {
        //Delay 1ms
        TIM_Cmd(TIM4, ENABLE);
        while (TIM_GetFlagStatus(TIM4, TIM_FLAG_Update) != SET);
        TIM_ClearFlag(TIM4, TIM_FLAG_Update);
        TIM_Cmd(TIM4, DISABLE);
    };
}


BOOL BUT_IsPress(BOOL debounce)
{
    BOOL res = FALSE;
    if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0))
    {
        if(debounce) 
        {
            DelayTms(200);
            if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0))
                res = TRUE;
        }
        else
        {
            res = TRUE;
        }
    }
    return res;
}

void PID_Speed_Ctrl(void)
{
   
    // PID PARAMETERS
    static float u = 0;
    static float e_0 = 0, e_1 = 0;
    static float Ppart = 0, Ipart = 0, Dpart = 0;
    static float DT = 0.005; //5ms

    static float speed = 0;
    static int32_t p_0 = 0, p_1 = 0;
    float dp;

    // Get speed
    p_0 = ENC_Get();
    dp = (float)(p_0 - p_1);
    if (dp > DELTA_PULSE_THRES)
        dp -= 65536;
    else if (dp < (-DELTA_PULSE_THRES))
        dp += 65536;
    speed = (dp * 7.5f); //rpm
    p_1 = p_0;


    /* @PID state: 
     */
    if (ctrl_en)
    {
        // PID control
        cur_val = speed;
        e_0 = (sp_val - cur_val);
        Ppart = Kp * e_0;
        Ipart += Ki * DT * (e_0 + e_1) / 2; // integration approximation
        Dpart = Kd * (e_0 - e_1) / DT;
        u = Ppart + Ipart + Dpart;
        e_1 = e_0;

        // PWM saturation
        if (u > PWM_OUT_MAX) u = PWM_OUT_MAX;
        else if (u < PWM_OUT_MIN) u = PWM_OUT_MIN;
    }
    else
    {
        // Stop period
        Ppart = 0;
        Ipart = 0;
        Dpart = 0;
        e_1 = 0;
        u = 0;
    }
    
    // PWM output
    PWM_Set((int16_t)u);
}


void PID_Pos_Ctrl(void)
{
    // PID PARAMETERS
    static float u = 0;
    static float e_0 = 0, e_1 = 0;
    static float Ppart = 0, Ipart = 0, Dpart = 0;
    static float DT = 0.005; //5ms

    static float pos = 0;
    static int32_t p_0 = 0, p_1 = 0;
    float dp;

    // Get speed
    p_0 = ENC_Get();
    dp = (float)(p_0 - p_1);
    if (dp > DELTA_PULSE_THRES)
        dp -= 65536;
    else if (dp < (-DELTA_PULSE_THRES))
        dp += 65536;
    pos += (dp / 1600 * 360); //deg: *1600/360
    p_1 = p_0;


    /* @PID state: 
     */
    if (ctrl_en)
    {
        // PID control
        cur_val = pos;
        e_0 = (sp_val - cur_val);
        Ppart = Kp * e_0;
        Ipart += Ki * DT * (e_0 + e_1) / 2; // integration approximation
        Dpart = Kd * (e_0 - e_1) / DT;
        u = Ppart + Ipart + Dpart;
        e_1 = e_0;

        // PWM saturation
        if (u > PWM_OUT_MAX) u = PWM_OUT_MAX;
        else if (u < PWM_OUT_MIN) u = PWM_OUT_MIN;
    }
    else
    {
        // Stop period
        Ppart = 0;
        Ipart = 0;
        Dpart = 0;
        e_1 = 0;
        u = 0;
        
        pos = 0;
    }
    
    // PWM output
    PWM_Set((int16_t)u);
}

void PID_Start(void)
{
    ctrl_en = TRUE;
}
void PID_Stop(void)
{
    ctrl_en = FALSE;
}
