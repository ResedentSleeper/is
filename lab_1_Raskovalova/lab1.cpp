#include "hal.h"

int sw[] = {GPIO_PIN_4, GPIO_PIN_8, GPIO_PIN_10, GPIO_PIN_12};
int leds_num[] = {GPIO_PIN_3, GPIO_PIN_4, GPIO_PIN_5, GPIO_PIN_6,
					GPIO_PIN_8, GPIO_PIN_9, GPIO_PIN_11, GPIO_PIN_12};
GPIO_PinState sw_state[4]; 


void resetLEDS() {
	for (int i = 0; i < 8; i++)
		HAL_GPIO_WritePin(GPIOD, leds_num[i], GPIO_PIN_RESET);
}

void lightLEDState() {
	for (int i = 0; i < 4; i++)
		HAL_GPIO_WritePin(GPIOD, leds_num[i], sw_state[i]);
}

void lightAnimState(int step) {
	if(step < 4){
		HAL_GPIO_WritePin(GPIOD, leds_num[3-step], GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOD, leds_num[4+step], GPIO_PIN_SET);
	} else {
		HAL_GPIO_WritePin(GPIOD, leds_num[7-step+4], GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOD, leds_num[step-4], GPIO_PIN_RESET);
	}
}

void allLightsAreGreen() {
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_15, GPIO_PIN_RESET);
}

void red() {
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_15, GPIO_PIN_SET);
}

void yellow() {
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_15, GPIO_PIN_RESET);
}

int umain() {
	GPIO_PinState state = GPIO_PIN_SET;
	int step = 0;
	int delay = 500;
	bool doAnimation = true;
	bool firstReset = true;
	
	while (1) {
		for (int i = 0; i < 4; i++) {
			sw_state[i] = HAL_GPIO_ReadPin(GPIOE, sw[i]);
		}
		state = HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_15);
		if (state == GPIO_PIN_RESET)
			doAnimation = !doAnimation;
		
		if (sw_state[0] == GPIO_PIN_RESET && sw_state[1] == GPIO_PIN_SET && sw_state[2] == GPIO_PIN_SET && sw_state[3] == GPIO_PIN_RESET) {
			if (!firstReset) {
				resetLEDS();
				lightAnimState(step);
			}
			firstReset = true;
			HAL_Delay(delay);


			if(step < 4){
				HAL_GPIO_WritePin(GPIOD, leds_num[3-step], GPIO_PIN_SET);
				HAL_GPIO_WritePin(GPIOD, leds_num[4+step], GPIO_PIN_SET);
			} else {
				HAL_GPIO_WritePin(GPIOD, leds_num[7-step+4], GPIO_PIN_RESET);
				HAL_GPIO_WritePin(GPIOD, leds_num[step-4], GPIO_PIN_RESET);
			}

			if (doAnimation) {
				allLightsAreGreen();
				if (step > 7) 
					step = 0;
				else 
					step++;
			} else {
				red();
			}
		} else {
			if (firstReset) {
				resetLEDS();
				firstReset = false;
			} else {
				yellow();
				lightLEDState();
			}
		}
	}
	
	return 0;
}

