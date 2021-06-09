/*
 * Copyright (c) 2019, Raphael Lehmann
 * Copyright (c) 2021, Christopher Durand
 *
 * This file is part of the modm project.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */
// ----------------------------------------------------------------------------

#ifndef MODM_STM32_FDCAN1_HPP
#define MODM_STM32_FDCAN1_HPP

#include <modm/architecture/interface/can.hpp>
#include <modm/architecture/interface/can_filter.hpp>
#include <modm/platform/gpio/connector.hpp>
#include "../device.hpp"
#include <optional>

#include "can_bit_timings.hpp"
#include "message_ram.hpp"

namespace modm::platform
{
/**
 * @brief		FDCAN1 (CAN with Flexible Data-Rate)
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
 * @author		Christopher Durand <christopher.durand@rwth-aachen.de>
 * @ingroup		modm_platform_can_1
 */
class Fdcan1 : public ::modm::Can
{
public:
	enum class
	Mode : uint32_t
	{
		Normal,
		Restricted,
		Monitoring,
		Sleep,
		TestExternalLoopback,
		TestInternalLoopback,

		// for compatibility with bxCAN:
		ListenOnly = Monitoring, // no acknowledging
		LoopBack = TestInternalLoopback,

	};

public:
	// Expose jinja template parameters to be checked by e.g. drivers or application
	static constexpr size_t RxBufferSize = 32;
	static constexpr size_t TxBufferSize = 32;

	using ErrorCallback = void (*)();

private:
	using MessageRam = fdcan::MessageRam<0>;

	static inline volatile ErrorCallback errorCallback_ = nullptr;

	static void
	initializeWithPrescaler(
			CanBitTimingConfiguration standardTimings,
			std::optional<CanBitTimingConfiguration> fdDataTimings,
			uint32_t interruptPriority, Mode startupMode, bool overwriteOnOverrun
			);

public:
	template< template<Peripheral _> class... Signals >
	static void
	connect(Gpio::InputType inputType = Gpio::InputType::Floating)
	{
		using Connector = GpioConnector<Peripheral::Fdcan1, Signals...>;
		using Tx = typename Connector::template GetSignal< Gpio::Signal::Tx >;
		using Rx = typename Connector::template GetSignal< Gpio::Signal::Rx >;
		static_assert(Connector::template IsValid<Tx> and Connector::template IsValid<Rx> and sizeof...(Signals) == 2,
					  "Can1::connect() requires one Tx and one Rx signal!");

		Tx::setOutput(Gpio::OutputType::PushPull);
		Rx::setInput(inputType);
		Connector::connect();
	}

	/**
	 * Enables the clock for the CAN controller and resets all settings
	 *
	 * \tparam SystemClock
	 * 			System clock struct with an FDCAN1 member containing
	 * 			the clock speed supplied to the peripheral.
	 * 			\warning	The CAN subsystem prescaler can be configured using
	 * 						the RCC module and must be taken into account in
	 * 						the provided clock speed value.
	 * \tparam bitrate
	 * 			Nominal CAN bitrate
	 * \tparam tolerance
	 * 			Maximum relative deviation between requested and resulting
	 * 			CAN bitrates. If the tolerance is exceeded a compile-time
	 * 			assertion will be triggered.
	 * \tparam fastDataBitrate
	 * 			CAN bitrate for data in FD frames with bit rate switching
	 * 			Set to 0 to disable CANFD support.
	 *
	 * \param interruptPriority
	 * 			Interrupt vector priority (0=highest to 15=lowest)
	 * \param startupMode
	 * 			Mode of operation set after initialization
	 * 			\see Fdcan1::Mode
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
		bitrate_t fastDataBitrate=0 // 0: FDCAN mode disabled
	>
	static inline void
	initialize(	uint32_t interruptPriority, Mode startupMode = Mode::Normal,
				bool overwriteOnOverrun = true)
	{
		using Timings = CanBitTiming<
			SystemClock::Fdcan1,
			bitrate,
			9, 8, 7, 7
		>;
		Timings::template assertBitrateInTolerance<tolerance>();

		std::optional<CanBitTimingConfiguration> fastDataTimings = std::nullopt;

		if constexpr (fastDataBitrate != 0) {
			using DataTimings = CanBitTiming<
				SystemClock::Fdcan1,
				fastDataBitrate,
				5, 5, 4, 4
			>;
			DataTimings::template assertBitrateInTolerance<tolerance>();

			fastDataTimings = DataTimings::getBitTimings();
		}

		return initializeWithPrescaler(
			Timings::getBitTimings(),
			fastDataTimings,
			interruptPriority,
			startupMode,
			overwriteOnOverrun);
	}

	/**
	 * Set the operating mode.
	 *
	 * Default after initialization is the normal mode.
	 */
	static void
	setMode(Mode mode);

	static void
	setAutomaticRetransmission(bool retransmission);

public:
	// Can interface methods
	static bool
	isMessageAvailable();

	static bool
	getMessage(can::IMessage& message, uint8_t *filter_id=nullptr, uint16_t *timestamp=nullptr);

	static bool
	isReadyToSend();

	static bool
	sendMessage(const can::IMessage& message);

public:
	// Can filter configuration

	using FilterConfig = MessageRam::FilterConfig;

	static constexpr inline uint8_t StandardFilterCount{28};
	static constexpr inline uint8_t ExtendedFilterCount{8};

	/// Set standard filter with id and mask
	/// \param standardIndex Standard filter index 0..27
	/// \returns true if filter index is valid
	static bool
	setStandardFilter(uint8_t standardIndex, FilterConfig config,
		modm::can::StandardIdentifier id,
		modm::can::StandardMask mask);

	/// Set standard filter with dual ids
	/// Matches on any of both specified ids
	/// \param standardIndex Standard filter index 0..27
	/// \returns true if filter index is valid
	static bool
	setStandardFilter(uint8_t standardIndex, FilterConfig config,
		modm::can::StandardIdentifier id0,
		modm::can::StandardIdentifier id1);

	/// Set standard range filter
	/// Matches the inclusive range between both specified ids
	/// \param standardIndex Standard filter index 0..27
	/// \returns true if filter index is valid
	static bool
	setStandardRangeFilter(uint8_t standardIndex, FilterConfig config,
		modm::can::StandardIdentifier first,
		modm::can::StandardIdentifier last);

	/// Set extended filter with id and mask
	/// \param extendedIndex Extended filter index 0..7
	/// \returns true if filter index is valid
	static bool
	setExtendedFilter(uint8_t extendedIndex, FilterConfig config,
		modm::can::ExtendedIdentifier id,
		modm::can::ExtendedMask mask);

	/// Set standard filter with dual ids
	/// Matches on any of both specified ids
	/// \param extendedIndex Extended filter index 0..7
	/// \returns true if filter index is valid
	static bool
	setExtendedFilter(uint8_t extendedIndex, FilterConfig config,
		modm::can::ExtendedIdentifier id0,
		modm::can::ExtendedIdentifier id1);

	/// Set standard range filter
	/// Matches the inclusive range between both specified ids
	/// \param extendedIndex Extended filter index 0..7
	/// \returns true if filter index is valid
	static bool
	setExtendedRangeFilter(uint8_t extendedIndex, FilterConfig config,
		modm::can::ExtendedIdentifier first,
		modm::can::ExtendedIdentifier last);

	/// Disable all standard filters, receive no standard frames
	static void
	clearStandardFilters();

	/// Disable all extended filters, receive no extended frames
	static void
	clearExtendedFilters();

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
	static inline uint8_t
	getReceiveErrorCounter()
	{
		return ((FDCAN1->ECR >> 8) & 0xFF);
	}

	/**
	 * Get Transmit Error Counter.
	 *
	 */
	static inline uint8_t
	getTransmitErrorCounter()
	{
		return (FDCAN1->ECR & 0xFF);
	}

	static BusState
	getBusState();

	/**
	 * Set the error interrupt callback.
	 *
	 * It will be called on the following events:
	 * - The FDCAN peripheral enters ERROR_PASSIVE or BUS_OFF state
	 * - The error counter exceeds the warning limit
	 *
	 * To disable the interrupt set the callback to nullptr.
	 */
	static void
	setErrorInterruptCallback(ErrorCallback callback)
	{
		errorCallback_ = callback;
		if(callback) {
			FDCAN1->IE |=  (FDCAN_IE_BOE | FDCAN_IE_EPE | FDCAN_IE_EWE);
		} else {
			FDCAN1->IE &= ~(FDCAN_IE_BOE | FDCAN_IE_EPE | FDCAN_IE_EWE);
		}
	}

	static ErrorCallback
	getErrorInterruptCallback()
	{
		return errorCallback_;
	}

	static uint16_t
	getCurrentTimestamp()
	{
		return FDCAN1->TSCV;
	}

private:
	static void
	configureMode(Mode mode);

	static void
	configureInterrupts(uint32_t interruptPriority);

	struct EnterInitMode
	{
		EnterInitMode()
		{
			FDCAN1->CCCR |= FDCAN_CCCR_INIT;
			int deadlockPreventer = 10'000; // max ~10ms
			while (((FDCAN1->CCCR & FDCAN_CCCR_INIT) == 0) and (deadlockPreventer-- > 0)) {
				using namespace std::literals;
				modm::delay(1us);
				// Wait until the initialization mode is entered.
			}
			modm_assert(deadlockPreventer > 0, "can.1.init", "timeout expired");
			FDCAN1->CCCR |= FDCAN_CCCR_CCE;
		}

		~EnterInitMode()
		{
			// Switch to normal operation, automatically clears CCE flag
			FDCAN1->CCCR &= ~FDCAN_CCCR_INIT;
		}

		EnterInitMode(const EnterInitMode&) = delete;
		EnterInitMode& operator=(const EnterInitMode&) = delete;
	};
};

}	// namespace modm::platform

#endif	//  MODM_STM32_FDCAN1_HPP