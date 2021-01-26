/*
 * Copyright (c) 2019, Raphael Lehmann
 *
 * This file is part of the modm project.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */
// ----------------------------------------------------------------------------

#ifndef MODM_STM32_FDCAN2_HPP
#define MODM_STM32_FDCAN2_HPP

#include <modm/architecture/interface/can.hpp>
#include <modm/platform/gpio/connector.hpp>
#include "../device.hpp"

#include "can_bit_timings.hpp"


namespace modm::platform
{
/**
 * @brief		FDCAN2 (CAN with Flexible Data-Rate)
 *
 * The controller area network (CAN) subsystem consists of one CAN module,
 * a shared Message RAM memory and a configuration block.
 * The modules (FDCAN) are compliant with ISO 11898-1: 2015 (CAN protocol
 * specification version 2.0 part A, B) and CAN FD protocol specification version 1.0.
 * A 0.8 Kbyte Message RAM per FDCAN instance implements filters, receive FIFOs,
 * transmit event FIFOs and transmit FIFOs.
 *
 * ## Filter
 * Up to 28 filters can be defined for 11-bit IDs, up to 8 filters for 29-bit IDs.
 * The filter banks are not shared between the CAN instances.
 *
 * ## Configuration
 * You can set the buffer size using the `tx_buffer` and `rx_buffer` parameters.
 *
 * @author		Raphael Lehmann <raphael@rleh.de>
 * @ingroup		modm_platform_can_1
 */
class Fdcan2 : public ::modm::Can
{
public:
/**
 * Address and offset definitions for the CAN SRAM
 */
// if STM32G4
	static constexpr uint32_t FilterListStandardSize		= 28;   // Filter List Standard size
	static constexpr uint32_t FilterListStandardElementSize	= 1*4;  // Filter Standard Element Size in bytes
	static constexpr uint32_t FilterListExtendedSize		= 8;    // Filter List Extended size
	static constexpr uint32_t FilterListExtendedElementSize	= 2*4;  // Filter Extended Element Size in bytes
	static constexpr uint32_t RxFifo0Size					= 3;    // RX FIFO 0 size
	static constexpr uint32_t RxFifo1Size					= 3;    // RX FIFO 1 size
	static constexpr uint32_t RxFifoElementSize				= 18*4; // RX FIFO 1 size
	static constexpr uint32_t TxEventFifoSize				= 3;    // TX Event FIFO size
	static constexpr uint32_t TxEventFifoElementSize		= 2*4;  // TX Event FIFO element size
	static constexpr uint32_t TxFifoSize					= 3;    // TX FIFO/Queue size
	static constexpr uint32_t TxFifoElementSize				= 18*4; // TX FIFO/Queue element size
// endif
// TODO: else F7 ...

	static constexpr uint32_t RamSize =
		(FilterListStandardSize * FilterListStandardElementSize)
		+(FilterListExtendedSize * FilterListExtendedElementSize)
		+(RxFifo0Size * RxFifoElementSize)
		+(RxFifo1Size * RxFifoElementSize)
		+(TxEventFifoSize * TxEventFifoElementSize)
		+(TxFifoSize * TxFifoElementSize);
	static constexpr uint32_t RamBase = SRAMCAN_BASE + (RamSize * 1);
	static constexpr uint32_t FilterListStandard = RamBase;
	static constexpr uint32_t FilterListExtended = FilterListStandard + (FilterListStandardSize * FilterListStandardElementSize);
	static constexpr uint32_t RxFifo0 = FilterListExtended + (FilterListExtendedSize* FilterListExtendedElementSize);
	static constexpr uint32_t RxFifo1 = RxFifo0 + (RxFifo0Size * RxFifoElementSize);
	static constexpr uint32_t TxEventFifo = RxFifo1 + (RxFifo1Size * RxFifoElementSize);
	static constexpr uint32_t TxFifo = TxEventFifo + (TxEventFifoSize *TxEventFifoElementSize);

public:
	enum class
	Mode : uint32_t
	{
		//Configuration,
		Normal,
		Restricted,
		Monitoring,
		Sleep,
		// various test modes:
		TestExternalLoopback,
		TestInternalLoopback,

		// compatibility:
		//ListenOnly = Monitoring, // no acknowledging
		//LoopBack = TestInternalLoopback,
		//ListenOnlyLoopBack not supported(?)

	};

	enum class StandardFilterMode : uint8_t
	{
		Range = 0b00, //Range filter from SFID1 to SFID2 
		DualID = 0b01, //Dual ID filter for SFID1 or SFID2
		Classic = 0b10, //Classic filter: SFID1 = filter, SFID2 = mask
		Disabled = 0b11, //Filter element disabled
	};
	enum class ExtendedFilterMode : uint8_t
	{
		RangeWithXidam = 0b00, //Range filter from SFID1 to SFID2 with xidam mask
		DualID = 0b01, //Dual ID filter for SFID1 or SFID2
		Classic = 0b10, //Classic filter: SFID1 = filter, SFID2 = mask
		RangeWithoutXidam = 0b11, //range filter without xidam mask
	};

	enum class FilterConfig: uint8_t
	{
		Disable = 0b000, //Disable filter element
		StoreRX0 = 0b001, //Store in Rx FIFO 0 if filter matches
		StoreRX1 = 0b010, //Store in Rx FIFO 1 if filter matches
		RejectOnMatch = 0b011, //Reject ID if filter matches
		PriorityOnMatch = 0b100, //Set priority if filter matches
		PriorityAndStoreRx0 = 0b0101, //Set priority and store in FIFO 0 if filter matches
		PriorityAndStoreRx1 = 0b0110, //Set priority and store in FIFO 1 if filter matches
		NotUsed = 0b0111,
	};

	enum class
	Preprescaler : uint8_t
	{
		Preprescaler1  = 0b0000,
		Preprescaler2  = 0b0001,
		Preprescaler4  = 0b0010,
		Preprescaler6  = 0b0011,
		Preprescaler8  = 0b0100,
		Preprescaler10 = 0b0101,
		Preprescaler12 = 0b0110,
		Preprescaler14 = 0b0111,
		Preprescaler16 = 0b1000,
		Preprescaler18 = 0b1001,
		Preprescaler20 = 0b1010,
		Preprescaler22 = 0b1011,
		Preprescaler24 = 0b1100,
		Preprescaler26 = 0b1101,
		Preprescaler28 = 0b1110,
		Preprescaler30 = 0b1111,
	};
private:
	static constexpr uint8_t
	getPreprescalerValue(Preprescaler preprescaler)
	{
		if(preprescaler == Preprescaler::Preprescaler1) {
			return 1;
		}
		else {
			return static_cast<uint8_t>(preprescaler) * 2;
		}
	}

public:
	// Expose jinja template parameters to be checked by e.g. drivers or application
	static constexpr size_t RxBufferSize = 32;
	static constexpr size_t TxBufferSize = 32;

private:
	/// Private Initializer with computed prescaler and timing constants
	static void
	initializeWithPrescaler(
			uint16_t prescaler, uint8_t bs1, uint8_t bs2, uint8_t sjw,
			uint16_t dataPrescaler, uint8_t dataBs1, uint8_t dataBs2, uint8_t dataSjw,
			Preprescaler preprescaler,
			uint32_t interruptPriority, Mode startupMode, bool overwriteOnOverrun
			);

public:
	template< template<Peripheral _> class... Signals >
	static void
	connect(Gpio::InputType inputType = Gpio::InputType::Floating)
	{
		using Connector = GpioConnector<Peripheral::Fdcan2, Signals...>;
		using Tx = typename Connector::template GetSignal< Gpio::Signal::Tx >;
		using Rx = typename Connector::template GetSignal< Gpio::Signal::Rx >;
		static_assert(Connector::template IsValid<Tx> and Connector::template IsValid<Rx> and sizeof...(Signals) == 2,
					  "Can1::connect() requires one Tx and one Rx signal!");

		// Connector::disconnect();
		Tx::setOutput(Gpio::OutputType::PushPull);
		Rx::setInput(inputType);
		Connector::connect();
	}

	/**
	 * Enables the clock for the CAN controller and resets all settings
	 *
	 * \param bitrate
	 * 			CAN bitrate (defined in driver/connectivity/can/message.hpp)
	 * \param interruptPriority
	 * 			Interrupt vector priority (0=highest to 15=lowest)
	 * \param overwriteOnOverrun
	 * 			Once a receive FIFO is full the next incoming message
	 * 			will overwrite the previous one if \c true otherwise
	 * 			the incoming message will be discarded
	 *
	 * \warning	Has to called after connect(), but before any
	 * 			other function from this class!
	 */
	template<
		class SystemClock,
		bitrate_t bitrate=kbps(125),
		percent_t tolerance=pct(1),
		bitrate_t dataBitrate=bitrate,
		Preprescaler preprescaler=Preprescaler::Preprescaler16
	>
	static inline void
	initialize(	uint32_t interruptPriority, Mode startupMode = Mode::Normal,
				bool overwriteOnOverrun = true)
	{
		using Timings = CanBitTiming<
			SystemClock::Fdcan2 / getPreprescalerValue(preprescaler),
			bitrate,
			9, 8, 7, 7
		>;
		Timings::template assertBitrateInTolerance<tolerance>();

		using DataTimings = CanBitTiming<
			SystemClock::Fdcan2 / getPreprescalerValue(preprescaler),
			bitrate,
			5, 5, 4, 4
		>;
		DataTimings::template assertBitrateInTolerance<tolerance>();

		return initializeWithPrescaler(
			Timings::getPrescaler(),
			Timings::getBS1(),
			Timings::getBS2(),
			Timings::getSJW(),
			DataTimings::getPrescaler(),
			DataTimings::getBS1(),
			DataTimings::getBS2(),
			DataTimings::getSJW(),
			preprescaler,
			interruptPriority,
			startupMode,
			overwriteOnOverrun);
	}

	/**
	 * The the operating mode.
	 *
	 * Default after initialization is the normal mode.
	 */
	static void
	setMode(Mode mode);

	static void
	setAutomaticRetransmission(bool retransmission);

	static void setFilter(uint16_t sfid1, uint16_t sfid2, StandardFilterMode mode, FilterConfig config, uint8_t index);

	static void setExtendedFilter(uint32_t efid1, uint32_t efid2, ExtendedFilterMode mode, FilterConfig config, uint8_t index);

	static void setNumberOfFilters(uint8_t n);
	static void setNumberOfExtendedFilters(uint8_t n);


private:
	static void
	_setMode(Mode mode);

	static bool enableProtectedAccess();

	static void disableProtectedAccess();

public:
	// Can Interface Methods
	static bool
	isMessageAvailable();
	
	static bool
	getMessage(can::IMessage& message, uint8_t *filter_id=nullptr, uint16_t *timestamp=nullptr);
	
	static bool
	isReadyToSend();

	static bool
	sendMessage(const can::IMessage& message);
	
public:
	// Extended Functionality
	/**
	 * Get Receive Error Counter.
	 *
	 * In case of an error during reception, this counter is
	 * incremented by 1 or by 8 depending on the error condition as
	 * defined by the CAN standard. After every successful reception
	 * the counter is decremented by 1 or reset to 120 if its value
	 * was higher than 128. When the counter value exceeds 127, the
	 * CAN controller enters the error passive state.
	 */
	static inline uint32_t
	getReceiveErrorCounter()
	{
		return ((FDCAN2->ECR >> 8) & 0xFF);
	}

	/**
	 * Get Transmit Error Counter.
	 *
	 */
	static inline uint32_t
	getTransmitErrorCounter()
	{
		return (FDCAN2->ECR & 0xFF);
	}

	static BusState
	getBusState();

	/**
	 * Enable the error and status change interrupt.
	 *
	 * Can be generated by the following events:
	 * - Error condition, for more details on error conditions please
	 *   refer to the CAN Error Status register (CAN_ESR).
	 * - Wakeup condition, SOF monitored on the CAN Rx signal.
	 * - Entry into Sleep mode
	 *
	 * You need to create you own interrupt handler for this interrupt.
	 * The interrupt handler has a fixed name:
	 * \code
	 * MODM_ISR(CAN1_SCE)
	 * {
	 *     ...
	 *
	 *     // e.g. Acknowledge interrupt
	 *     CAN1->MSR = CAN_MSR_ERRI;
	 * }
	 * \endcode
	 *
	 * \param interruptEnable
	 * 			Upper 24-bit of the CAN_IER register. E.g.:
	 * 			 - CAN_IER_BOFIE
	 * 			 - CAN_IER_EPVIE
	 * 			 - ...
	 * 			See Reference Manual >> bxCAN >> CAN_IER Register
	 * \param interruptPriority
	 * 			Interrupt vector priority (0=highest to 15=lowest)
	 */
	static void
	enableStatusChangeInterrupt(uint32_t interruptEnable,
			uint32_t interruptPriority);

	/**
	 *
	 *
	 */
	static uint16_t
	getCurrentTimestamp()
	{
		return FDCAN2->TSCV;
	}


	static void printRegs();

private:
	static Mode last_set_mode;
};

}	// namespace modm::platform

#endif	//  MODM_STM32_FDCAN2_HPP
