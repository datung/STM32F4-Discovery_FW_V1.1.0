#ifndef __USER_H
#define __USER_H

#include "stm32f4_discovery.h"
#include "stm32f4xx_tim.h"

/* 
 * 3-PWM
 */
#define PWM_TIM              TIM12 // Channel 1#define PWM_TIM_CLK          RCC_APB1Periph_TIM12
#define PWM_TIM_CLK          RCC_APB1Periph_TIM12
#define PWM_PULSE_PORT       GPIOB
#define PWM_PULSE            GPIO_Pin_14
#define PWM_PULSE_SOURCE     GPIO_PinSource14
#define PWM_PULSE_AF         GPIO_AF_TIM12
#define PWM_DIR_PORT         GPIOB
#define PWM_DIR              GPIO_Pin_15      
#define PWM_EN_PORT          GPIOD
#define PWM_EN               GPIO_Pin_10
#define PWM_PRESCALER        84 // 84M/84=1M
#define PWM_PERIOD           500 // 1M/500=2k
#define PWM_DUTY             250

/* 
 * 4-ENCODER:
 */
#define ENC_TIM              TIM1
#define ENC_TIM_CLK          RCC_APB2Periph_TIM1
#define ENC_PORT             GPIOE
#define ENC_A                GPIO_Pin_9
#define ENC_A_SOURCE         GPIO_PinSource9
#define ENC_B                GPIO_Pin_11
#define ENC_B_SOURCE         GPIO_PinSource11
#define ENC_AF               GPIO_AF_TIM1

#define ENC_CHANNEL          TIM_Channel_1
#define ENC_TIM_PRESCALE     336   //slow down the timer to 500kHz (Timer_FREQ=168M)
#define ENC_TIM_FREQ         500000 //500KHz    
#define ENC_Interrupt        TIM1_CC_IRQHandler
#define ENC_TIM_IRQ          TIM1_CC_IRQn
#define ENC_CAP_TIMEOUT      100   
#define CAPTURE_MAX_VAL       65535

/* PUMP PARAMS */
#define PUMP_SPEED_MIN     (int16_t)50   // Max set speed (rpm)
#define PUMP_SPEED_STOP    (int16_t)100  // Min speed can stop motor at once
#define PUMP_SPEED_MAX     (int16_t)1000 // Min set speed (rpm)
#define PUMP_SPEED_GEAR    5
// #define PULSE_PER_REV      (uint16_t)1600
#define DELTA_PULSE_THRES  (uint16_t)10000
#define PULSE_TO_RPM       (real32_t)7.5       //60 * 200 / (PULSE_PER_REV)
#define PWM_OUT_MAX        400
#define PWM_OUT_MIN        -400

typedef enum
{
    FALSE = (uint8_t)0,
    TRUE //  = (uint32_t)1
}BOOL;


void PIO_Init(void);
void TIM_Init(void);
void PWM_Init(void);
void ENC_Init(void);
void BRD_Init(void);
int32_t ENC_Get(void);
void PWM_Set(int16_t d);
void DelayTms(uint32_t T);
BOOL BUT_IsPress(BOOL debounce);
void PID_Speed_Ctrl(void);
void PID_Pos_Ctrl(void);
void PID_Start(void);
void PID_Stop(void);
#endif

