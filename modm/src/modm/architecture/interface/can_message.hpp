/*
 * Copyright (c) 2014, 2016, Niklas Hauser
 * Copyright (c) 2015-2016, Sascha Schade
 *
 * This file is part of the modm project.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */
// ----------------------------------------------------------------------------

#ifndef MODM_CAN_MESSAGE_HPP
#define MODM_CAN_MESSAGE_HPP

#include <stdint.h>
#include <modm/architecture/utils.hpp>
#include <array>
#include <modm/debug/logger.hpp>

namespace modm::can
{

/// Representation of a CAN message
/// @ingroup modm_architecture_can
struct IMessage
{
	inline IMessage(uint32_t inIdentifier = 0, uint8_t inLength = 0) :
		identifier(inIdentifier), flags(), length(inLength)
	{
	}

	inline uint32_t
	getIdentifier() const
	{
		return identifier;
	}

	inline void
	setIdentifier(uint32_t id)
	{
		identifier = id;
	}

	inline void
	setExtended(bool extended = true)
	{
		flags.extended = (extended) ? 1 : 0;
	}

	inline bool
	isExtended() const
	{
		return (flags.extended != 0);
	}

	inline void
	setRemoteTransmitRequest(bool rtr = true)
	{
		flags.rtr = (rtr) ? 1 : 0;
	}

	inline bool
	isRemoteTransmitRequest() const
	{
		return (flags.rtr != 0);
	}

	inline void setFlexibleData(bool fd = true)
	{
		flags.fd = fd;
	}

	inline bool isFlexibleData() const
	{
		return flags.fd;
	}

	inline void setBitRateSwitching(bool brs = true)
	{
		flags.brs = brs;
	}

	inline bool isBitRateSwitching() const
	{
		return flags.brs;
	}

	inline uint8_t
	getLength() const
	{
		return length;
	}

	inline void
	setLength(uint8_t len)
	{
		length = std::min(len, capacity);
	}
	
	inline uint8_t
	getDLC() const
	{
		//MODM_LOG_INFO << "L: " << length << modm::endl;
		if(length <= 8) {
			return length;
		}
		else if(length <= 24) {
			return 8 + ((length - 8 + 4 - 1) >> 2); 
		}
		else {
			return 12 + ((length - 8 + 8 - 1) >> 4);
		}
	}

	inline void
	setDLC(uint8_t dlc)
	{
		setLength(dlcLookup.at(dlc));
	}

	inline uint8_t 
	getCapacity() const
	{
		return capacity;
	}

public:
	uint32_t identifier;
	struct Flags
	{
		Flags() :
			rtr(0), extended(1), fd(0), brs(0)
		{
		}

		bool rtr : 1;
		bool extended : 1;
		bool fd : 1;
		bool brs : 1;
	} flags;
	uint8_t length = 0;
protected:
    uint8_t capacity = 0;
public:
    uint8_t *data = nullptr;
	
	bool
	operator == (const IMessage& rhs) const;
	
	bool
	operator < (const IMessage& rhs) const;

	void operator= (const IMessage& rhs);

protected:
	static constexpr std::array<uint8_t, 16> dlcLookup = {{0, 1, 2, 3, 4, 5, 6, 7, 8, 12, 16, 20, 24, 32, 48, 64}};
};

struct LongMessage; // Forward decleration

struct Message : IMessage
{
	inline Message(uint32_t inIdentifier = 0, uint8_t inLength = 0) : IMessage(inIdentifier, inLength) {
		this->capacity = 8;
		this->data = data_holder;
	}
private:
	uint8_t modm_aligned(4) data_holder[8];
public:
	bool
	operator == (const modm::can::Message& rhs) const{return IMessage::operator==(reinterpret_cast<const IMessage&>(rhs));}
	
	bool
	operator < (const modm::can::Message& rhs) const{return IMessage::operator<(reinterpret_cast<const IMessage&>(rhs));}
	
	void
	operator = (const modm::can::Message& rhs) {IMessage::operator=(reinterpret_cast<const IMessage&>(rhs));}
	
	void
	operator = (const modm::can::LongMessage& rhs) {IMessage::operator=(reinterpret_cast<const IMessage&>(rhs));}
	
	void
	operator = (const modm::can::IMessage& rhs) {IMessage::operator=(rhs);}
};

struct LongMessage: IMessage
{
	inline LongMessage(uint32_t inIdentifier = 0, uint8_t inLength = 0) : IMessage(inIdentifier, inLength) {
		this->capacity = 64;
		this->data = data_holder;
}
private:
	uint8_t modm_aligned(4) data_holder[64];
public:
	bool
	operator == (const modm::can::LongMessage& rhs) const{return IMessage::operator==(reinterpret_cast<const IMessage&>(rhs));}
	
	bool
	operator < (const modm::can::LongMessage& rhs) const{return IMessage::operator<(reinterpret_cast<const IMessage&>(rhs));}
	
	void
	operator = (const modm::can::Message& rhs) {IMessage::operator=(reinterpret_cast<const IMessage&>(rhs));}

	void
	operator = (const modm::can::LongMessage& rhs) {IMessage::operator=(reinterpret_cast<const IMessage&>(rhs));}
	
	void
	operator = (const modm::can::IMessage& rhs) {IMessage::operator=(rhs);}
};

}	// namespace modm::can

#include <modm/io/iostream.hpp>

modm::IOStream&
operator << (modm::IOStream& s, const modm::can::Message m);
#endif // MODM_CAN_MESSAGE_HPP
