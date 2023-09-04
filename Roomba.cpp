// Roomba.cpp
//
// Copyright (C) 2010 Mike McCauley
// $Id: Roomba.cpp,v 1.1 2010/09/27 21:58:32 mikem Exp mikem $

#include "Roomba.h"

Roomba::Roomba()
{
 // _serial = serial;
  _baud = 115200;
  _pollState = PollStateIdle;
}

// Resets the
void Roomba::reset()
{
    Serial2.write(7);
}

// Start OI
// Changes mode to passive
void Roomba::start()
{
    Serial2.begin(_baud);
    Serial2.write(128);
}

void Roomba::stop()
{
    Serial2.write(173);
}

uint32_t Roomba::baudCodeToBaudRate(Baud baud)
{
    switch (baud)
    {
	case Baud300:
	    return 300;
	case Baud600:
	    return 600;
	case Baud1200:
	    return 1200;
	case Baud2400:
	    return 2400;
	case Baud4800:
	    return 4800;
	case Baud9600:
	    return 9600;
	case Baud14400:
	    return 14400;
	case Baud19200:
	    return 19200;
	case Baud28800:
	    return 28800;
	case Baud38400:
	    return 38400;
	case Baud57600:
	    return 57600;
	case Baud115200:
	    return 115200;
	default:
	    return 57600;
    }
}

void Roomba::baud(Baud baud)
{
    Serial2.write(129);
    Serial2.write(baud);

    _baud = baudCodeToBaudRate(baud);
    Serial2.begin(_baud);
}

void Roomba::safeMode()
{
  Serial2.write(131);
}

void Roomba::fullMode()
{
  Serial2.write(132);
}

void Roomba::power()
{
  Serial2.write(133);
}

void Roomba::dock()
{
  Serial2.write(143);
}

void Roomba::demo(Demo demo)
{
  Serial2.write(136);
  Serial2.write(demo);
}

void Roomba::cover()
{
  Serial2.write(135);
}

void Roomba::coverAndDock()
{
  Serial2.write(143);
}

void Roomba::spot()
{
  Serial2.write(134);
}

void Roomba::drive(int16_t velocity, int16_t radius)
{
  Serial2.write(137);
  Serial2.write((velocity & 0xff00) >> 8);
  Serial2.write(velocity & 0xff);
  Serial2.write((radius & 0xff00) >> 8);
  Serial2.write(radius & 0xff);
}

void Roomba::driveDirect(int16_t leftVelocity, int16_t rightVelocity)
{
  Serial2.write(145);
  Serial2.write((rightVelocity & 0xff00) >> 8);
  Serial2.write(rightVelocity & 0xff);
  Serial2.write((leftVelocity & 0xff00) >> 8);
  Serial2.write(leftVelocity & 0xff);
}

void Roomba::leds(uint8_t leds, uint8_t powerColour, uint8_t powerIntensity)
{
  Serial2.write(139);
  Serial2.write(leds);
  Serial2.write(powerColour);
  Serial2.write(powerIntensity);
}

void Roomba::digitLedsRaw(uint8_t digit3, uint8_t digit2, uint8_t digit1, uint8_t digit0)
{
  Serial2.write(163);
  Serial2.write(digit3);
  Serial2.write(digit2);
  Serial2.write(digit1);
  Serial2.write(digit0);
}

void Roomba::digitLedsASCII(uint8_t digit3, uint8_t digit2, uint8_t digit1, uint8_t digit0)
{
  Serial2.write(164);
  Serial2.write(digit3);
  Serial2.write(digit2);
  Serial2.write(digit1);
  Serial2.write(digit0);
}

void Roomba::digitalOut(uint8_t out)
{
  Serial2.write(147);
  Serial2.write(out);
}

// Sets PWM duty cycles on low side drivers
void Roomba::pwmDrivers(uint8_t dutyCycle0, uint8_t dutyCycle1, uint8_t dutyCycle2)
{
  Serial2.write(144);
  Serial2.write(dutyCycle2);
  Serial2.write(dutyCycle1);
  Serial2.write(dutyCycle0);
}

// Sets low side driver outputs on or off
void Roomba::drivers(uint8_t out)
{
  Serial2.write(138);
  Serial2.write(out);
}

// Modulates low side driver 1 (pin 23 on Cargo Bay Connector)
// with the given IR command
void Roomba::sendIR(uint8_t data)
{
  Serial2.write(151);
  Serial2.write(data);
}

// Define a song
// Data is 2 bytes per note
void Roomba::song(uint8_t songNumber, const uint8_t* data, int len)
{
    Serial2.write(140);
    Serial2.write(songNumber);
    Serial2.write(len >> 1); // 2 bytes per note
    Serial2.write(data, len);
}

void Roomba::playSong(uint8_t songNumber)
{
  Serial2.write(141);
  Serial2.write(songNumber);
}

// Start a stream of sensor data with the specified packet IDs in it
void Roomba::stream(const uint8_t* packetIDs, int len)
{
  Serial2.write(148);
  Serial2.write(packetIDs, len);
}

// One of StreamCommand*
void Roomba::streamCommand(StreamCommand command)
{
  Serial2.write(150);
  Serial2.write(command);
}

// Use len=0 to clear the script
void Roomba::script(const uint8_t* script, uint8_t len)
{
  Serial2.write(152);
  Serial2.write(len);
  Serial2.write(script, len);
}

void Roomba::playScript()
{
  Serial2.write(153);
}

// Each tick is 15ms
void Roomba::wait(uint8_t ticks)
{
  Serial2.write(155);
  Serial2.write(ticks);
}

void Roomba::waitDistance(int16_t mm)
{
  Serial2.write(156);
  Serial2.write((mm & 0xff00) >> 8);
  Serial2.write(mm & 0xff);
}

void Roomba::waitAngle(int16_t degrees)
{
  Serial2.write(157);
  Serial2.write((degrees & 0xff00) >> 8);
  Serial2.write(degrees & 0xff);
}

// Can use the negative of an event type to wait for the inverse of an event
void Roomba::waitEvent(EventType type)
{
  Serial2.write(158);
  Serial2.write(type);
}

// Reads at most len bytes and stores them to dest
// If successful, returns true.
// If there is a timeout, returns false
// Blocks until all bytes are read
// Caller must ensure there is sufficient space in dest
bool Roomba::getData(uint8_t* dest, uint8_t len)
{
  while (len-- > 0)
  {
    unsigned long startTime = millis();
    while (!Serial2.available())
    {
      // Look for a timeout
      if (millis() > startTime + ROOMBA_READ_TIMEOUT)
        return false; // Timed out
    }
    *dest++ = Serial2.read();
  }
  return true;
}

bool Roomba::getSensors(uint8_t packetID, uint8_t* dest, uint8_t len)
{
  Serial2.write(142);
  Serial2.write(packetID);
  return getData(dest, len);
}

bool Roomba::getSensorsList(uint8_t* packetIDs, uint8_t numPacketIDs, uint8_t* dest, uint8_t len)
{
  Serial2.write(149);
  Serial2.write(numPacketIDs);
  Serial2.write(packetIDs, numPacketIDs);
  return getData(dest, len);
}

// Simple state machine to read sensor data and discard everything else
bool Roomba::pollSensors(uint8_t* dest, uint8_t len)
{
    while (Serial2.available())
    {
	uint8_t ch = Serial2.read();
	switch (_pollState)
	{
	    case PollStateIdle:
		    if (ch == 19)
		      _pollState = PollStateWaitCount;
          
          _pollChecksum = ch;
		    break;

	    case PollStateWaitCount:
                _pollChecksum += ch;
                _pollSize = ch;
		_pollCount = 0;
		_pollState = PollStateWaitBytes;
		break;

	    case PollStateWaitBytes:
		_pollChecksum += ch;
		if (_pollCount < len)
		    dest[_pollCount] = ch;
		if (++_pollCount >= _pollSize)
		    _pollState = PollStateWaitChecksum;
		break;

	    case PollStateWaitChecksum:
		_pollChecksum += ch;
		_pollState = PollStateIdle;
		return (_pollChecksum == 0);
		break;
	}
    }
    return false;
}

// Returns the number of bytes in the script, or 0 on errors
// Only saves at most len bytes to dest
// Calling with len = 0 will return the amount of space required without actually storing anything
uint8_t Roomba::getScript(uint8_t* dest, uint8_t len)
{
  Serial2.write(154);

  unsigned long startTime = millis();
  while (!Serial2.available())
  {
    // Look for a timeout
    if (millis() > startTime + ROOMBA_READ_TIMEOUT)
      return 0; // Timed out
  }

  int count = Serial2.read();
  if (count > 100 || count < 0)
    return 0; // Something wrong. Cant have such big scripts!!

  // Get all the data, saving as much as we can
  uint8_t i;
  for (i = 0; i < count; i++)
  {
    startTime = millis();
    while (!Serial2.available())
    {
      // Look for a timeout
      if (millis() > startTime + ROOMBA_READ_TIMEOUT)
        return 0; // Timed out
    }
    uint8_t data = Serial2.read();
    if (i < len)
      *dest++ = data;
  }

  return count;
}
