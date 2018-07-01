#include "SensorWater.h"

SensorWater::SensorWater(const int pinTrigger, const int pinEcho)
: Sensor(0)/*, _pinTrigger(pinTrigger), _pinEcho(pinEcho) */{
	
	_max = 255;
	_min = 0;
	_iSample = 0;
	for (uint8_t i = 0; i < _numSamples; i++) {
		_waterLevels[i] = 0;
	}
}

SensorWater::SensorWater(const SensorWater &other) : Sensor(other) {
	// _pinTrigger = other._pinTrigger;
	// _pinEcho = other._pinEcho;
	_iSample = other._iSample;
	for (uint8_t i = 0; i < _numSamples; i++) {
		_waterLevels[i] = other._waterLevels[i];
	}
	_waterLevel = other._waterLevel;
}

SensorWater& SensorWater::operator =(const SensorWater &other) {
	// _pinTrigger = other._pinTrigger;
	// _pinEcho = other._pinEcho;
	_iSample = other._iSample;
	for (uint8_t i = 0; i < _numSamples; i++) {
		_waterLevels[i] = other._waterLevels[i];
	}
	_waterLevel = other._waterLevel;
	return *this;
}

SensorWater::~SensorWater() {}

Sensor::SensName SensorWater::getType() const {
	return Sensor::Level;
}

void SensorWater::init() {
	// pinMode(_pinTrigger, OUTPUT);
	// pinMode(_pinEcho, INPUT);
}

//Max and min should be set before calling this
void SensorWater::update() {
	_waterLevels[_iSample] = getPercent();
	_iSample++;
	if (_iSample >= _numSamples)
	_iSample = 0;
	smooth();
}

void SensorWater::fastUpdate() {
	uint8_t w = getPercent();
	for (uint8_t i = 0; i < _numSamples; i++) {
		_waterLevels[i] = w;
	}
	smooth();
}

uint8_t SensorWater::get() const {
	return _waterLevel;
}

//Returns raw distance in cm
uint16_t SensorWater::getRaw() const {
	digitalWrite(waterTrigger, LOW);
	delayMicroseconds(2);
	digitalWrite(waterTrigger, HIGH);
	delayMicroseconds(5);
	digitalWrite(waterTrigger, LOW);
	return (pulseIn(waterEcho, HIGH) / 29) / 2;
}

//Returns water reservoir % level
uint8_t SensorWater::getPercent() const {
	int duration, distance;
	digitalWrite(waterTrigger, LOW);
	delayMicroseconds(2);
	digitalWrite(waterTrigger, HIGH);
	delayMicroseconds(5);
	digitalWrite(waterTrigger, LOW);
	//Calc duration. Timeout of half second!
	duration = pulseIn(waterEcho, HIGH, 500000);
	distance = (duration / waterEcho) / 2;
	distance = constrain(distance, _max, _min);
	return map(distance, _max, _min, 100, 0);
}

void SensorWater::setMax(uint16_t max) {
	_max = max;
}

void SensorWater::setMin(uint16_t min) {
	_min = min;
}

void SensorWater::smooth() {
	uint16_t res = 0;
	for (uint8_t i = 0; i < _numSamples; i++) { res += _waterLevels[i]; }
	_waterLevel = (uint8_t)(res / _numSamples);
}