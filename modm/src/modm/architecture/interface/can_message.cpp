/*
 * Copyright (c) 2015-2017, Sascha Schade
 *
 * This file is part of the modm project.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */
// ----------------------------------------------------------------------------

#include "can_message.hpp"
#include <cstring>

bool
modm::can::IMessage::operator == (const modm::can::IMessage& rhs) const
{
	if ((this->identifier     == rhs.identifier) and
		(this->length         == rhs.length)     and
		(this->flags.rtr      == rhs.flags.rtr)  and
		(this->flags.extended == rhs.flags.extended))
	{
		bool eq = true;
		for (uint8_t ii = 0; ii < this->length; ++ii)
		{
			if (this->data[ii] != rhs.data[ii]) {
				eq = false;
			}
		}
		return eq;
	}
	return false;
}

bool
modm::can::IMessage::operator < (const modm::can::IMessage& rhs) const
{
	return (this->identifier << (this->flags.extended ? 0 : 18))
		< (rhs.identifier << (rhs.flags.extended ? 0 : 18));
}

void
modm::can::IMessage::operator= (const modm::can::IMessage& rhs)
{
	this->identifier = rhs.identifier;
	this->flags = rhs.flags;
	this->setLength(rhs.length);
	if(this->data != nullptr and rhs.data != nullptr)
		std::memcpy(this->data, rhs.data, this->length);
}


#include <inttypes.h>
#include <modm/io/iostream.hpp>

modm::IOStream&
operator << (modm::IOStream& s, const modm::can::Message m)
{
	s.printf("id = %04" PRIx32 ", len = ", m.identifier);
	s << m.length;
	s.printf(", flags = %c%c, data = ",
			 m.flags.rtr ? 'R' : 'r',
			 m.flags.extended ? 'E' : 'e');
	if (not m.isRemoteTransmitRequest()) {
		for (uint_fast8_t ii = 0; ii < m.length; ++ii) {
			s.printf("%02x ", m.data[ii]);
		}
	}
	return s;
}

modm::IOStream&
operator << (modm::IOStream& s, const modm::can::LongMessage m)
{
	s.printf("id = %04" PRIx32 ", len = ", m.identifier);
	s << m.length;
	s.printf(", flags = %c%c, data = ",
			 m.flags.rtr ? 'R' : 'r',
			 m.flags.extended ? 'E' : 'e');
	if (not m.isRemoteTransmitRequest()) {
		for (uint_fast8_t ii = 0; ii < m.length; ++ii) {
			s.printf("%02x ", m.data[ii]);
		}
	}
	return s;
}
