#include "stm32f10x.h"
#include <bitset>

#define TIM_1  1
#define TIM_2  2
#define TIM_3  3
#define TIM_4  4
#define TIM_5  5
#define TIM_6  6
#define TIM_7  7
#define TIM_8  8
#define TIM_9  9
#define TIM_10 10
#define TIM_11 11
#define TIM_12 12
#define TIM_13 13
#define TIM_14 14

#define FROZEN 	   0
#define SET_HIGH   1
#define SET_LOW    2
#define TOGGLE     3
#define FORCE_LOW  4
#define FORCE_HIGH 5
#define PWM_1 	   6
#define PWM_2 	   7

void delay(int count);

class Timer_Base;
