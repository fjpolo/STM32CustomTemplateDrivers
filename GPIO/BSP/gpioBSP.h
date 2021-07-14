#pragma once

#include "mcal_reg.h"
#include "mcal_reg_access.h"
#include "mcal_reg_Access_dynamic.h"
#include <stdbool.h>
#include <stdint.h>


/**
	* \brief private anonymous namespace
	*/
	namespace{
		/**
			const std::uint32_t gpioa_base						= ahb1_periph_base + 0x0000U;
			const std::uint32_t gpiob_base						= ahb1_periph_base + 0x0400U;
			const std::uint32_t gpioc_base						= ahb1_periph_base + 0x0800U;
			const std::uint32_t gpiod_base						= ahb1_periph_base + 0x0C00U;
			const std::uint32_t gpioe_base						= ahb1_periph_base + 0x1000U;
			const std::uint32_t gpiof_base						= ahb1_periph_base + 0x1400U;
			const std::uint32_t gpiog_base						= ahb1_periph_base + 0x1800U;
			const std::uint32_t gpioh_base						= ahb1_periph_base + 0x1C00U;
			const std::uint32_t gpioi_base						= ahb1_periph_base + 0x2000U;
		*/
//		typedef enum{
//			gpioAbase = mcal::reg::ahb1_periph_base + 0x0000U,
//			gpioBbase = mcal::reg::ahb1_periph_base + 0x0400U,
//			gpioCbase = mcal::reg::ahb1_periph_base + 0x0800U,
//			gpioDbase = mcal::reg::ahb1_periph_base + 0x0C00U,
//			gpioEbase = mcal::reg::ahb1_periph_base + 0x1000U,
//			gpioFbase = mcal::reg::ahb1_periph_base + 0x1400U,
//			gpioGbase = mcal::reg::ahb1_periph_base + 0x1800U,
//			gpioHbase = mcal::reg::ahb1_periph_base + 0x1C00U,
//			gpioIbase = mcal::reg::ahb1_periph_base + 0x2000U,
//		}BaseAddresses_t;
		
		
			// GPIO
			template<std::uint32_t port>
			struct config_register{
				const std::uint32_t moder 					= mcal::reg::ahb1_periph_base + (port * 0x400) + mcal::reg::moder_offset;
				const std::uint32_t otyper					= mcal::reg::ahb1_periph_base + (port * 0x400) + mcal::reg::otyper_offset;
				const std::uint32_t ospeedr					= mcal::reg::ahb1_periph_base + (port * 0x400) + mcal::reg::ospeedr_offset;
				const std::uint32_t pupdr						= mcal::reg::ahb1_periph_base + (port * 0x400) + mcal::reg::pupdr_offset;
				const std::uint32_t idr							= mcal::reg::ahb1_periph_base + (port * 0x400) + mcal::reg::idr_offset;
				const std::uint32_t odr							= mcal::reg::ahb1_periph_base + (port * 0x400) + mcal::reg::odr_offset;
				const std::uint32_t bsrr						= mcal::reg::ahb1_periph_base + (port * 0x400) + mcal::reg::bssr_offset;
				const std::uint32_t lckr						= mcal::reg::ahb1_periph_base + (port * 0x400) + mcal::reg::lckr_offset;
				const std::uint32_t afrl						= mcal::reg::ahb1_periph_base + (port * 0x400) + mcal::reg::afrl_offset;
				const std::uint32_t afrh						= mcal::reg::ahb1_periph_base + (port * 0x400) + mcal::reg::afrh_offset;
			};
	}	
	
	
/**
* \namespace gpioBSP
*/
namespace gpioBSP{
	/**
	* \brief gpioPort_t
	*/
	#ifndef gpioPort_t
	typedef enum:uint32_t{
		GPIO_A = 0,
		GPIO_B,
		GPIO_C,
		GPIO_D,
		GPIO_E,
		GPIO_F,
		GPIO_G,
		GPIO_H,
		GPIO_I
	}gpioPort_t;
	#endif // gpioPort_t

	/**
	* \brief gpioPin_t
	*/
	#ifndef gpioPin_t
	typedef enum:uint32_t{
		Pin0 = 0,
		Pin1,
		Pin2,
		Pin3,
		Pin4,
		Pin5,
		Pin6,
		Pin7,
		Pin8,
		Pin9,
		Pin10,
		Pin11,
		Pin12,
		Pin13,
		Pin14,
		Pin15,
	}gpioPin_t;
	#endif // gpioPin_t
	
	
	/**
	* \brief gpioPinMode_t
	*/
	typedef enum{
		gpioInput = 0x00,
		gpioOutput,
		gpioAlternate,
		gpioAnalog
	}gpioPinMode_t;
	
	/*Single GPIO pin*/
	template<gpioBSP::gpioPort_t Port, gpioBSP::gpioPin_t Pin, gpioPinMode_t Mode>
	struct gpioPin{
		public:
			gpioPin(){
				using namespace mcal::reg;
				
				/*Configuration registers*/
				constexpr config_register<Port> ConfigRegisters;
				
				
				/*Enable clock access PORT*/
				reg_access<	std::uint32_t, 
										std::uint32_t, 
										mcal::reg::ahb1enr,
										(Port)
									>::bit_set();

				/*Input - Output*/
				if(Mode == gpioBSP::gpioInput){
					/*Set pin as input*/
					reg_access<	std::uint32_t, 
											std::uint32_t, 
											ConfigRegisters.moder,
											(2*Pin)								
										>::bit_clear();
					reg_access<	std::uint32_t, 
											std::uint32_t, 
											ConfigRegisters.moder,
											((2*Pin)+1UL)						
										>::bit_clear();
				}
				else if(Mode == gpioBSP::gpioOutput){
					/*Set pin as output*/
					reg_access<	std::uint32_t, 
											std::uint32_t, 
											ConfigRegisters.moder,
											(2*Pin)						
										>::bit_set();
				}
				
				/*Clear pin*/
				reg_access<	std::uint32_t, 
										std::uint32_t, 
										ConfigRegisters.bsrr,
										(16U+Pin)						
									>::bit_set();
				
			}
			~gpioPin(){}
			void set(){
				using namespace mcal::reg;
				/*Configuration registers*/
				constexpr config_register<Port> ConfigRegisters;
				/*Set*/
				reg_access<	std::uint32_t, 
										std::uint32_t, 
										ConfigRegisters.bsrr,
										(Pin)						
									>::bit_set();
			}
			void clear(){
				using namespace mcal::reg;
				/*Configuration registers*/
				constexpr config_register<Port> ConfigRegisters;
				/*Clear*/
				reg_access<	std::uint32_t, 
										std::uint32_t, 
										ConfigRegisters.bsrr,
										(16U+Pin)						
									>::bit_set();
			}
			bool read(){
				using namespace mcal::reg;
				/*Configuration registers*/
				constexpr config_register<Port> ConfigRegisters;
				/*Read*/
				uint32_t aux = reg_access<	std::uint32_t, 
																		std::uint32_t,
																		ConfigRegisters.idr,
																		(0U)
																>::reg_get();
				/**/
				return (aux & (1UL<<Pin));
			}
			static void toggle(){
				using namespace mcal::reg;
				/*Configuration registers*/
				constexpr config_register<Port> ConfigRegisters;
				/*Toggle*/
				reg_access<	std::uint32_t, 
										std::uint32_t, 
										ConfigRegisters.odr,
										(Pin)						
									>::bit_not();
			}
	};


//	/*Generic GPIO List*/	
//	template< typename... > struct all {};
//	template<> struct all<>{
//		void set(){}
//		void clear(){}
//	};
//	template< typename pin, typename... tail > struct all < pin, tail... >{  
//		void set(){
//      pin::set();
//      all< tail... >::set();
//		}
//		void clear(){
//			pin::clear();
//			all< tail... >::clear();
//		}
//	};
}







/**
* \brief Using lazy constructors
*/
//class gpioBSP{
//	public:
//		enum Ports{
//			GPIOA = 0,
//			GPIOB,
//			GPIOC,
//			GPIOD,
//			GPIOE,
//			GPIOF,
//			GPIOG,
//			GPIOH,
//			GPIOI
//		};
//		enum Pins{
//			Pin0 = 0,
//			Pin1,
//			Pin2,
//			Pin3,
//			Pin4,
//			Pin5,
//			Pin6,
//			Pin7,
//			Pin8,
//			Pin9,
//			Pin10,
//			Pin11,
//			Pin12,
//			Pin13,
//			Pin14,
//			Pin15,
//			Pin16
//		};
//		gpioBSP(gpioBSP::Ports _port, gpioBSP::Pins _pin):Port(_port), Pin(_pin){}
//		~gpioBSP(){}
//		
//	private:
//		Ports Port;
//		Pins Pin;
//};

//gpioBSP& GPIOA0()
//{
//	static gpioBSP obj{gpioBSP::Ports::GPIOA, gpioBSP::Pins::Pin0};
//    return obj;    
//}
//gpioBSP& GPIOA1()
//{
//	static gpioBSP obj{gpioBSP::Ports::GPIOA, gpioBSP::Pins::Pin1};
//    return obj;    
//}
//gpioBSP& GPIOA2()
//{
//	static gpioBSP obj{gpioBSP::Ports::GPIOA, gpioBSP::Pins::Pin2};
//    return obj;    
//}
//gpioBSP& GPIOA3()
//{
//	static gpioBSP obj{gpioBSP::Ports::GPIOA, gpioBSP::Pins::Pin3};
//    return obj;    
//}
//gpioBSP& GPIOA4()
//{
//	static gpioBSP obj{gpioBSP::Ports::GPIOA, gpioBSP::Pins::Pin4};
//    return obj;    
//}

