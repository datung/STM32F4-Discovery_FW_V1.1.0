#include "user.h"

static BOOL Enable;

int main(void)
{
    
	BRD_Init();

    while (1)
    {
        if (BUT_IsPress(TRUE)) Enable ^= 1; // not
        
        if (Enable) PID_Start();
        else PID_Stop();
        
        PID_Pos_Ctrl();

        DelayTms(5);

        GPIO_ToggleBits(GPIOD, GPIO_Pin_14);
    }
}

