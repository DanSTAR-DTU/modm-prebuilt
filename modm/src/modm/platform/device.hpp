/*
 * Copyright (c) 2009, Martin Rosekeit
 * Copyright (c) 2009, Thorsten Lajewski
 * Copyright (c) 2009-2010, 2016, Fabian Greif
 * Copyright (c) 2012-2013, 2016, 2018 Niklas Hauser
 * Copyright (c) 2013, Kevin Laeufer
 *
 * This file is part of the modm project.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */
// ----------------------------------------------------------------------------

#ifndef MODM_DEVICE_HPP
#define MODM_DEVICE_HPP

#define STM32G474xx 1
#include <stdint.h>
// Defines for example the modm_always_inline macro
#include <modm/architecture/utils.hpp>

// Include external device headers:
#include <stm32g474xx.h>
#include <system_stm32g4xx.h>
/// @cond
// This is a hack to make the *_Typedef's known to GDB, so that you can debug
// the peripherals directly in GDB in any context.
// Otherwise GDB would throw a "no symbol 'GPIO_TypeDef' in current context".
extern ADC_TypeDef				___ADC1			;
extern ADC_Common_TypeDef		___ADC12_COMMON	;
extern ADC_TypeDef				___ADC2			;
extern ADC_TypeDef				___ADC3			;
extern ADC_Common_TypeDef		___ADC345_COMMON	;
extern ADC_TypeDef				___ADC4			;
extern ADC_TypeDef				___ADC5			;
extern COMP_TypeDef			___COMP1			;
extern COMP_TypeDef			___COMP2			;
extern COMP_TypeDef			___COMP3			;
extern COMP_TypeDef			___COMP4			;
extern COMP_TypeDef			___COMP5			;
extern COMP_TypeDef			___COMP6			;
extern COMP_TypeDef			___COMP7			;
extern CORDIC_TypeDef			___CORDIC			;
extern CRC_TypeDef				___CRC				;
extern CRS_TypeDef				___CRS				;
extern CoreDebug_Type			___CoreDebug		;
extern DAC_TypeDef				___DAC				;
extern DAC_TypeDef				___DAC1			;
extern DAC_TypeDef				___DAC2			;
extern DAC_TypeDef				___DAC3			;
extern DAC_TypeDef				___DAC4			;
extern DBGMCU_TypeDef			___DBGMCU			;
extern DMA_TypeDef				___DMA1			;
extern DMA_Channel_TypeDef		___DMA1_Channel1	;
extern DMA_Channel_TypeDef		___DMA1_Channel2	;
extern DMA_Channel_TypeDef		___DMA1_Channel3	;
extern DMA_Channel_TypeDef		___DMA1_Channel4	;
extern DMA_Channel_TypeDef		___DMA1_Channel5	;
extern DMA_Channel_TypeDef		___DMA1_Channel6	;
extern DMA_Channel_TypeDef		___DMA1_Channel7	;
extern DMA_Channel_TypeDef		___DMA1_Channel8	;
extern DMA_TypeDef				___DMA2			;
extern DMA_Channel_TypeDef		___DMA2_Channel1	;
extern DMA_Channel_TypeDef		___DMA2_Channel2	;
extern DMA_Channel_TypeDef		___DMA2_Channel3	;
extern DMA_Channel_TypeDef		___DMA2_Channel4	;
extern DMA_Channel_TypeDef		___DMA2_Channel5	;
extern DMA_Channel_TypeDef		___DMA2_Channel6	;
extern DMA_Channel_TypeDef		___DMA2_Channel7	;
extern DMA_Channel_TypeDef		___DMA2_Channel8	;
extern DMAMUX_Channel_TypeDef	___DMAMUX1			;
extern DMAMUX_Channel_TypeDef	___DMAMUX1_Channel0;
extern DMAMUX_Channel_TypeDef	___DMAMUX1_Channel1;
extern DMAMUX_Channel_TypeDef	___DMAMUX1_Channel10;
extern DMAMUX_Channel_TypeDef	___DMAMUX1_Channel11;
extern DMAMUX_Channel_TypeDef	___DMAMUX1_Channel12;
extern DMAMUX_Channel_TypeDef	___DMAMUX1_Channel13;
extern DMAMUX_Channel_TypeDef	___DMAMUX1_Channel14;
extern DMAMUX_Channel_TypeDef	___DMAMUX1_Channel15;
extern DMAMUX_Channel_TypeDef	___DMAMUX1_Channel2;
extern DMAMUX_Channel_TypeDef	___DMAMUX1_Channel3;
extern DMAMUX_Channel_TypeDef	___DMAMUX1_Channel4;
extern DMAMUX_Channel_TypeDef	___DMAMUX1_Channel5;
extern DMAMUX_Channel_TypeDef	___DMAMUX1_Channel6;
extern DMAMUX_Channel_TypeDef	___DMAMUX1_Channel7;
extern DMAMUX_Channel_TypeDef	___DMAMUX1_Channel8;
extern DMAMUX_Channel_TypeDef	___DMAMUX1_Channel9;
extern DMAMUX_ChannelStatus_TypeDef	___DMAMUX1_ChannelStatus;
extern DMAMUX_RequestGenStatus_TypeDef	___DMAMUX1_RequestGenStatus;
extern DMAMUX_RequestGen_TypeDef	___DMAMUX1_RequestGenerator0;
extern DMAMUX_RequestGen_TypeDef	___DMAMUX1_RequestGenerator1;
extern DMAMUX_RequestGen_TypeDef	___DMAMUX1_RequestGenerator2;
extern DMAMUX_RequestGen_TypeDef	___DMAMUX1_RequestGenerator3;
extern DWT_Type				___DWT				;
extern EXTI_TypeDef			___EXTI			;
extern FDCAN_Config_TypeDef	___FDCAN_CONFIG	;
extern FLASH_TypeDef			___FLASH			;
extern FMAC_TypeDef			___FMAC			;
extern FMC_Bank1E_TypeDef		___FMC_Bank1E_R	;
extern FMC_Bank1_TypeDef		___FMC_Bank1_R		;
extern FMC_Bank3_TypeDef		___FMC_Bank3_R		;
extern FPU_Type				___FPU				;
extern GPIO_TypeDef			___GPIOA			;
extern GPIO_TypeDef			___GPIOB			;
extern GPIO_TypeDef			___GPIOC			;
extern GPIO_TypeDef			___GPIOD			;
extern GPIO_TypeDef			___GPIOE			;
extern GPIO_TypeDef			___GPIOF			;
extern GPIO_TypeDef			___GPIOG			;
extern HRTIM_TypeDef			___HRTIM1			;
extern HRTIM_Common_TypeDef	___HRTIM1_COMMON	;
extern HRTIM_Timerx_TypeDef	___HRTIM1_TIMA		;
extern HRTIM_Timerx_TypeDef	___HRTIM1_TIMB		;
extern HRTIM_Timerx_TypeDef	___HRTIM1_TIMC		;
extern HRTIM_Timerx_TypeDef	___HRTIM1_TIMD		;
extern HRTIM_Timerx_TypeDef	___HRTIM1_TIME		;
extern HRTIM_Timerx_TypeDef	___HRTIM1_TIMF		;
extern I2C_TypeDef				___I2C1			;
extern I2C_TypeDef				___I2C2			;
extern I2C_TypeDef				___I2C3			;
extern I2C_TypeDef				___I2C4			;
extern ITM_Type				___ITM				;
extern IWDG_TypeDef			___IWDG			;
extern LPTIM_TypeDef			___LPTIM1			;
extern USART_TypeDef			___LPUART1			;
extern MPU_Type				___MPU				;
extern NVIC_Type				___NVIC			;
extern OPAMP_TypeDef			___OPAMP			;
extern OPAMP_TypeDef			___OPAMP1			;
extern OPAMP_TypeDef			___OPAMP2			;
extern OPAMP_TypeDef			___OPAMP3			;
extern OPAMP_TypeDef			___OPAMP4			;
extern OPAMP_TypeDef			___OPAMP5			;
extern OPAMP_TypeDef			___OPAMP6			;
extern PWR_TypeDef				___PWR				;
extern QUADSPI_TypeDef			___QUADSPI			;
extern RCC_TypeDef				___RCC				;
extern RNG_TypeDef				___RNG				;
extern RTC_TypeDef				___RTC				;
extern SAI_TypeDef				___SAI1			;
extern SAI_Block_TypeDef		___SAI1_Block_A	;
extern SAI_Block_TypeDef		___SAI1_Block_B	;
extern SCB_Type				___SCB				;
extern SCnSCB_Type				___SCnSCB			;
extern SPI_TypeDef				___SPI1			;
extern SPI_TypeDef				___SPI2			;
extern SPI_TypeDef				___SPI3			;
extern SPI_TypeDef				___SPI4			;
extern SYSCFG_TypeDef			___SYSCFG			;
extern SysTick_Type			___SysTick			;
extern TAMP_TypeDef			___TAMP			;
extern TIM_TypeDef				___TIM1			;
extern TIM_TypeDef				___TIM15			;
extern TIM_TypeDef				___TIM16			;
extern TIM_TypeDef				___TIM17			;
extern TIM_TypeDef				___TIM2			;
extern TIM_TypeDef				___TIM20			;
extern TIM_TypeDef				___TIM3			;
extern TIM_TypeDef				___TIM4			;
extern TIM_TypeDef				___TIM5			;
extern TIM_TypeDef				___TIM6			;
extern TIM_TypeDef				___TIM7			;
extern TIM_TypeDef				___TIM8			;
extern TPI_Type				___TPI				;
extern USART_TypeDef			___UART4			;
extern USART_TypeDef			___UART5			;
extern UCPD_TypeDef			___UCPD1			;
extern USART_TypeDef			___USART1			;
extern USART_TypeDef			___USART2			;
extern USART_TypeDef			___USART3			;
extern USB_TypeDef				___USB				;
extern VREFBUF_TypeDef			___VREFBUF			;
extern WWDG_TypeDef			___WWDG			;
/// @endcond

#endif  // MODM_DEVICE_HPP