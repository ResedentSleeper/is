#include "hal.h"

int sw[] = {GPIO_PIN_4, GPIO_PIN_8, GPIO_PIN_10, GPIO_PIN_12};
int leds[] = {GPIO_PIN_3, GPIO_PIN_4, GPIO_PIN_5, GPIO_PIN_6,
					GPIO_PIN_8, GPIO_PIN_9, GPIO_PIN_11, GPIO_PIN_12};
					
int leds_num = 8;
int delay = 500;
int T = 150;
int step = 0;

void resetLEDS() {
	for (int i = 0; i < 8; i++)
		HAL_GPIO_WritePin(GPIOD, leds[i], GPIO_PIN_RESET);
}

int readSwitcherState() {
	int mul = 1;
	int value = 0;
	for (int i = 3; i >= 0; i--) {
		if (HAL_GPIO_ReadPin(GPIOE, sw[i]) == GPIO_PIN_SET) 
			value += mul;
		mul *= 2;
	}
	return value;
}

void animate(int i) {
	resetLEDS();
	if(i == 7){
		HAL_GPIO_WritePin(GPIOD, leds[leds_num-1-i], GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOD, leds[i], GPIO_PIN_SET);	
	} else {
		HAL_GPIO_WritePin(GPIOD, leds[leds_num-1-i], GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOD, leds[leds_num-2-i], GPIO_PIN_SET);
	}
}

void Lab_Handler() {
	WRITE_REG(TIM6_ARR, delay + T*readSwitcherState());
	if (step > 7)
		step = 0;
	animate(step);
	step++;
}

int umain() {	
	registerTIM6_IRQHandler(Lab_Handler);
	
	__enable_irq();
	
	WRITE_REG(TIM6_ARR, 500);
	WRITE_REG(TIM6_DIER, TIM_DIER_UIE);
	WRITE_REG(TIM6_PSC, 0);
	
	WRITE_REG(TIM6_CR1, TIM_CR1_CEN);
	
	return 0;
}

