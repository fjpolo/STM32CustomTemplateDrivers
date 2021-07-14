#include "gpioBSP.h"
#include "clock.h"
#include "gpio.h"


/** 
* \program: Driver to control GPIOs: LEDS at PB7, PI7, PG11. Button at PD7. 
 */


/**
* \brief Using template drivers
*/
#ifndef STM32F407_HAL

/**
* \brief Defines for C style memory mapped drivers
*/
//#define				RCC_AHB1ENR				(*((volatile unsigned int *)0x40023830U))
//#define				GPIOI_MODER				(*((volatile unsigned int *)0x40022000U))				
//#define				GPIOI_BSRR				(*((volatile unsigned int *)0x40022018U))				
//#define				GPIOI_ODR					(*((volatile unsigned int *)0x40022014U))


/**
* \fn main
*
* \brief 
*/
int main(void){
	
	/*Init clock*/
//	SystemClock_Config();	// 3558 bytes overhead in -O0
	
	
////	/**
////	* \brief Using C style memory mapped drivers
////	*/
////	RCC_AHB1ENR |= (1U<<8);	// Enable clocl GPIOI
////	GPIOI_MODER |= (1U<<(2*7U));
////	while(1){
////		GPIOI_ODR ^= (1U<<7U);
// 		}	
//	
//	/**
//	* \brief Using template memory mapped drivers
//	*/
//	/* 1. Enable clock access to GPIOI */
//	mcal::reg::reg_access<	
//												std::uint32_t, 
//												std::uint32_t, 
//												mcal::reg::ahb1enr,
//												(8U)
//											>::bit_set();	
//	/* 2. Set PI7 as output */
//	mcal::reg::reg_access<	
//												std::uint32_t, 
//												std::uint32_t, 
//												mcal::reg::gpioi_moder,
//												(2*7U)						// Pin 7
//											>::bit_set();
//	
//	/*Superloop*/
//	while(true){
//		/* 3. Toggle PI7 */
//		mcal::reg::reg_access<	
//													std::uint32_t, 
//													std::uint32_t, 
//													mcal::reg::gpioi_odr,
//													(7U)						// Pin 7 set
//												>::bit_not();	
//		for(int i=0;i<1000;i++);
//	}

	/**
	* \brief Using templates and structs memory mapped drivers
	*/
	
	/**Variables used**/
	/*RED Led PI7*/
	gpioBSP::gpioPin<gpioBSP::GPIO_I, gpioBSP::Pin7, gpioBSP::gpioOutput> RedLED{};
	/*GREEN Led PB7*/
	gpioBSP::gpioPin<gpioBSP::GPIO_B, gpioBSP::Pin7, gpioBSP::gpioOutput> GreenLED{};
	/*YELLOW Led PG11*/
	gpioBSP::gpioPin<gpioBSP::GPIO_G, gpioBSP::Pin11, gpioBSP::gpioOutput> YellowLED{};
	/*Button*/
	gpioBSP::gpioPin<gpioBSP::GPIO_D, gpioBSP::Pin7, gpioBSP::gpioInput> UserButton{};
		
	
		
	/*Superloop*/
	while(1){
		/*Read button*/
		if(!UserButton.read()){
			/*Set Green*/
			GreenLED.clear();
			/*Clear RedLED*/
			RedLED.set();
		}
		else{
			/*Clear GreenLED*/
			GreenLED.set();
			/*Set RedLED*/
			RedLED.clear();
		}
		/*Toggle YellowLED*/
		YellowLED.toggle();
		/*Delay*/
		for(uint32_t i=0;i<10000;++i);
	}
}
#endif // STM32F407_HAL

/**
* \brief Using ST HAL drivers
*/
#ifdef STM32F407_HAL
/**
* \fn main
*
* \brief 
*/
int main(void){
//	/*Init clock*/
//	SystemClock_Config();
	
	/*Init GPIOs*/
	MX_GPIO_Init();
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOG, GPIO_PIN_11, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOI, GPIO_PIN_7, GPIO_PIN_SET);
	
	/*Superloop*/
	while(1){
		/*Read button*/
		if(HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_7) == GPIO_PIN_SET){
			/*Set Green*/
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_SET);
			/*Clear RedLED*/
			HAL_GPIO_WritePin(GPIOI, GPIO_PIN_7, GPIO_PIN_RESET);
		}
		else{
			/*Clear GreenLED*/
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_RESET);
			/*Set RedLED*/
			HAL_GPIO_WritePin(GPIOI, GPIO_PIN_7, GPIO_PIN_SET);
		}
		/*Toggle YellowLED*/
		HAL_GPIO_TogglePin(GPIOG, GPIO_PIN_11);
		/*Delay*/
		for(uint32_t i=0;i<10000;++i);
	}
}
#endif // STM32F407_HAL


/**
* \fn SysTick_Handler
*
* \brief 
*/
void SysTick_Handler(){
//	HAL_IncTick();
//	HAL_SYSTICK_IRQHandler();
	while(1);
}