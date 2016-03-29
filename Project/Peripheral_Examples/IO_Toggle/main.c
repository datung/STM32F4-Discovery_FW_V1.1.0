#include "user.h"

static float Speed; //rpm
static BOOL Enable;
static int32_t pulse, pulse_1;

int main(void)
{
    
	BRD_Init();

    while (1)
    {
        if (BUT_IsPress(TRUE)) Enable ^= 1; // not
        
        if (Enable) PWM_Set(PWM_DUTY);
        else PWM_Set(0);
        
        DelayTms(10);
        pulse = ENC_Get();
        Speed = (float)(pulse - pulse_1)*6000/1600;
        pulse_1 = pulse;
        GPIO_ToggleBits(GPIOD, GPIO_Pin_14);
    }
}

