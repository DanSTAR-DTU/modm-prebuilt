/*
 * Copyright (c) 2010-2011, Fabian Greif
 * Copyright (c) 2012-2014, Niklas Hauser
 * Copyright (c) 2014, Sascha Schade
 *
 * This file is part of the modm project.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */
// ----------------------------------------------------------------------------

#ifndef MODM_SOFTWARE_BITBANG_ONE_WIRE_HPP
	#error	"Don't include this file directly, use 'bitbang_master.hpp' instead!"
#endif

// ----------------------------------------------------------------------------
template <typename Pin> uint8_t modm::platform::BitBangOneWireMaster<Pin>::lastDiscrepancy;
template <typename Pin> uint8_t modm::platform::BitBangOneWireMaster<Pin>::lastFamilyDiscrepancy;
template <typename Pin> bool    modm::platform::BitBangOneWireMaster<Pin>::lastDeviceFlag;
template <typename Pin> uint8_t modm::platform::BitBangOneWireMaster<Pin>::crc8;
template <typename Pin> uint8_t modm::platform::BitBangOneWireMaster<Pin>::romBuffer[8];

// ----------------------------------------------------------------------------
template <typename Pin>
bool
modm::platform::BitBangOneWireMaster<Pin>::touchReset()
{
	delayMicroseconds(G);
	PIN::reset();	// drives the bus low
	delayMicroseconds(H);
	PIN::set();		// releases the bus
	delayMicroseconds(I);

	// sample for presence pulse from slave
	bool result = !PIN::read();

	delayMicroseconds(J);			// complete the reset sequence recovery

	return result;
}

template <typename Pin>
void
modm::platform::BitBangOneWireMaster<Pin>::writeBit(bool bit)
{
	if (bit)
	{
		PIN::reset();	// Drives bus low
		delayMicroseconds(A);
		PIN::set();		// Releases the bus
		delayMicroseconds(B);		// Complete the time slot and 10us recovery
	}
	else
	{
		PIN::reset();
		delayMicroseconds(C);
		PIN::set();
		delayMicroseconds(D);
	}
}

template <typename Pin>
bool
modm::platform::BitBangOneWireMaster<Pin>::readBit()
{
	PIN::reset();	// drives the bus low
	delayMicroseconds(A);
	PIN::set();		// releases the bus
	delayMicroseconds(E);

	// Sample the bit value from the slave
	bool result = PIN::read();

	delayMicroseconds(F);			// complete the reset sequence recovery

	return result;
}

// ----------------------------------------------------------------------------
template <typename Pin>
void
modm::platform::BitBangOneWireMaster<Pin>::writeByte(uint8_t data)
{
	// Loop to write each bit in the byte, LS-bit first
	for (uint8_t i = 0; i < 8; ++i)
	{
		writeBit(data & 0x01);
		data >>= 1;
	}
}

template <typename Pin>
uint8_t
modm::platform::BitBangOneWireMaster<Pin>::readByte()
{
	uint8_t result = 0;
	for (uint8_t i = 0; i < 8; ++i)
	{
		result >>= 1;
		if (readBit()) {
			result |= 0x80;
		}
	}

	return result;
}

// ----------------------------------------------------------------------------
template <typename Pin>
uint8_t
modm::platform::BitBangOneWireMaster<Pin>::touchByte(uint8_t data)
{
	uint8_t result = 0;
	for (uint8_t i = 0; i < 8; ++i)
	{
		result >>= 1;

		// If sending a '1' then read a bit else write a '0'
		if (data & 0x01)
		{
			if (readBit()) {
				result |= 0x80;
			}
		}
		else {
			writeBit(0);
		}

		data >>= 1;
	}

	return result;
}

// ----------------------------------------------------------------------------
template <typename Pin>
void
modm::platform::BitBangOneWireMaster<Pin>::resetSearch()
{
	// reset the search state
	lastDiscrepancy = 0;
	lastFamilyDiscrepancy = 0;
	lastDeviceFlag = false;
}

template <typename Pin>
void
modm::platform::BitBangOneWireMaster<Pin>::resetSearch(uint8_t familyCode)
{
	// set the search state to find family type devices
	romBuffer[0] = familyCode;
	for (uint8_t i = 1; i < 8; ++i) {
		romBuffer[i] = 0;
	}

	lastDiscrepancy = 64;
	lastFamilyDiscrepancy = 0;
	lastDeviceFlag = false;
}

template <typename Pin>
bool
modm::platform::BitBangOneWireMaster<Pin>::searchNext(uint8_t *rom)
{
	if (performSearch())
	{
		for (uint8_t i = 0; i < 8; ++i) {
			rom[i] = romBuffer[i];
		}
		return true;
	}
	return false;
}

template <typename Pin>
void
modm::platform::BitBangOneWireMaster<Pin>::searchSkipCurrentFamily()
{
	// set the Last discrepancy to last family discrepancy
	lastDiscrepancy = lastFamilyDiscrepancy;
	lastFamilyDiscrepancy = 0;

	// check for end of list
	if (lastDiscrepancy == 0) {
		lastDeviceFlag = true;
	}
}

template <typename Pin>
bool
modm::platform::BitBangOneWireMaster<Pin>::verifyDevice(const uint8_t *rom)
{
	uint8_t romBufferBackup[8];
	bool result;

	// keep a backup copy of the current state
	for (uint8_t i = 0; i < 8; i++) {
		romBufferBackup[i] = romBuffer[i];
	}
	uint16_t ld_backup = lastDiscrepancy;
	bool ldf_backup = lastDeviceFlag;
	uint16_t lfd_backup = lastFamilyDiscrepancy;


	// set search to find the same device
	lastDiscrepancy = 64;
	lastDeviceFlag = false;
	if (performSearch())
	{
		// check if same device found
		result = true;
		for (uint8_t i = 0; i < 8; i++)
		{
			if (rom[i] != romBuffer[i])
			{
				result = false;
				break;
			}
		}
	}
	else {
		result = false;
	}

	// restore the search state
	for (uint8_t i = 0; i < 8; i++) {
		romBuffer[i] = romBufferBackup[i];
	}
	lastDiscrepancy = ld_backup;
	lastDeviceFlag = ldf_backup;
	lastFamilyDiscrepancy = lfd_backup;

	// return the result of the verify
	return result;
}

// ----------------------------------------------------------------------------
template <typename Pin>
uint8_t
modm::platform::BitBangOneWireMaster<Pin>::crcUpdate(uint8_t crc, uint8_t data)
{
#ifdef __AVR__
	return _crc_ibutton_update(crc, data);
#else
	crc = crc ^ data;
	for (uint_fast8_t i = 0; i < 8; ++i)
	{
		if (crc & 0x01) {
			crc = (crc >> 1) ^ 0x8C;
		}
		else {
			crc >>= 1;
		}
	}
	return crc;
#endif
}

// ----------------------------------------------------------------------------
template <typename Pin>
bool
modm::platform::BitBangOneWireMaster<Pin>::performSearch()
{
	bool searchResult = false;

	// if the last call was not the last one
	if (!lastDeviceFlag)
	{
		// 1-Wire reset
		if (!touchReset())
		{
			// reset the search
			lastDiscrepancy = 0;
			lastDeviceFlag = false;
			lastFamilyDiscrepancy = 0;
			return false;
		}

		// issue the search command
		writeByte(0xF0);

		// initialize for search
		uint8_t idBitNumber = 1;
		uint8_t lastZeroBit = 0;
		uint8_t romByteNumber = 0;
		uint8_t romByteMask = 1;
		bool searchDirection;

		crc8 = 0;

		// loop to do the search
		do
		{
			// read a bit and its complement
			bool idBit = readBit();
			bool complementIdBit = readBit();

			// check for no devices on 1-wire
			if ((idBit == true) && (complementIdBit == true)) {
				break;
			}
			else
			{
				// all devices coupled have 0 or 1
				if (idBit != complementIdBit) {
					searchDirection = idBit; // bit write value for search
				}
				else
				{
					// if this discrepancy if before the Last Discrepancy
					// on a previous next then pick the same as last time
					if (idBitNumber < lastDiscrepancy) {
						searchDirection = ((romBuffer[romByteNumber] & romByteMask) > 0);
					}
					else {
						// if equal to last pick 1, if not then pick 0
						searchDirection = (idBitNumber == lastDiscrepancy);
					}

					// if 0 was picked then record its position in LastZero
					if (searchDirection == false)
					{
						lastZeroBit = idBitNumber;
						// check for Last discrepancy in family
						if (lastZeroBit < 9)
							lastFamilyDiscrepancy = lastZeroBit;
					}
				}

				// set or clear the bit in the ROM byte rom_byte_number
				// with mask rom_byte_mask
				if (searchDirection == true) {
					romBuffer[romByteNumber] |= romByteMask;
				}
				else {
					romBuffer[romByteNumber] &= ~romByteMask;
				}

				// serial number search direction write bit
				writeBit(searchDirection);

				// increment the byte counter id_bit_number
				// and shift the mask rom_byte_mask
				idBitNumber++;
				romByteMask <<= 1;

				// if the mask is 0 then go to new SerialNum byte rom_byte_number and reset mask
				if (romByteMask == 0)
				{
					crc8 = crcUpdate(crc8, romBuffer[romByteNumber]); // accumulate the CRC
					romByteNumber++;
					romByteMask = 1;
				}
			}
		}
		while(romByteNumber < 8);  // loop until through all ROM bytes 0-7

		// if the search was successful then
		if (!((idBitNumber < 65) || (crc8 != 0)))
		{
			// search successful
			lastDiscrepancy = lastZeroBit;
			// check for last device
			if (lastDiscrepancy == 0) {
				lastDeviceFlag = true;
			}
			searchResult = true;
		}
	}

	// if no device found then reset counters so next 'search' will be like a first
	if (!searchResult || !romBuffer[0])
	{
		lastDiscrepancy = 0;
		lastDeviceFlag = false;
		lastFamilyDiscrepancy = 0;
		searchResult = false;
	}

	return searchResult;
}