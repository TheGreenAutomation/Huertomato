#include "SensorEC.h"

#include "SensorLight.h"

SensorEC::SensorEC(const int pin)
: Sensor(pin) {
	_serialDbg = false;
	_iSample = 0;
	for (uint8_t i = 0; i < _numSamples; i++) {
		_ecs[i] = 0;
	}
	_calibratingEc = false;
}

SensorEC::SensorEC(const SensorEC &other) : Sensor(other) {
	_serialDbg = other._serialDbg;
	_iSample = other._iSample;
	for (uint8_t i = 0; i < _numSamples; i++) {
		_ecs[i] = other._ecs[i];
	}
	_ec = other._ec;
	_calibratingEc = false;
}

SensorEC& SensorEC::operator =(const SensorEC &other) {
	_serialDbg = other._serialDbg;
	_pin = other._pin;
	_iSample = other._iSample;
	for (uint8_t i = 0; i < _numSamples; i++) {
		_ecs[i] = other._ecs[i];
	}
	_ec = other._ec;
	return *this;
}

SensorEC::~SensorEC() {}

Sensor::SensName SensorEC::getType() const {
	return Sensor::Ec;
}

void SensorEC::init() {
	//Open communication
	Serial1.begin(9600);
	setResponse(false);
	//Set to continuous mode (needs 20-25 readings of 1000ms to stabilize reading)
	//setContinuous();
	setStandby();
	Serial1.print("R\r");
}

void SensorEC::update() {
	_ecs[_iSample] = getRaw();
	_iSample++;
	if (_iSample >= _numSamples)
		_iSample = 0;
	smooth();
}

void SensorEC::fastUpdate() {
	float e = getRaw();
	for (uint8_t i = 0; i < _numSamples; i++) {
		_ecs[i] = e;
	}
	smooth();
}

float SensorEC::get() const {
	return _ec;
}

//Returns EC in mSiemens. Reading takes 1000ms
float SensorEC::getRaw() const {
	//If sensors being calibrated we return previous values
	if (!_calibratingEc) {
		Serial1.print("R\r");
		if (Serial1.available() > 0) {
			uint32_t res = Serial1.parseInt();
			//Clear buffer of remaining data
			while (Serial1.available() > 0)
				Serial1.read();
			return (float)res/1000.0;
		}
		//Buffer has been emptied before and circuit still hasn't put data into it again
		return 0;
	} else
	return _ec;
}

void SensorEC::setSerialDebug(boolean s) {
	_serialDbg = s;
}

//This should be set while calibrating to prevent messing up circuits if update() called
void SensorEC::calibrating(boolean c) {
	_calibratingEc = c;
}

void SensorEC::resetToFactory() {
	clearECbuffer();
	Serial1.print("Factory\r");
	//Give time for reset
	delay(2750);
	ecToSerial();
}

void SensorEC::resetCalibration() {
	Serial2.print("Cal,clear\r");
}

void SensorEC::getInfo() {
	clearECbuffer();
	Serial1.print("I\r");
	delay(100);
	ecToSerial();	
}

void SensorEC::getStatus() {
	clearECbuffer();
	Serial1.print("STATUS\r");
	//Leave some time for data to be received
	delay(100);
	ecToSerial();
}

void SensorEC::setLed(const boolean state) {
	(state) ? Serial1.print("L,1\r") : Serial1.print("L,0\r");	
}

void SensorEC::setResponse(const boolean state) {
	(state) ? Serial1.print("RESPONSE,1\r") : Serial2.print("RESPONSE,0\r");
}

void SensorEC::setContinuous() {
	Serial1.print("C,1\r");	
}

void SensorEC::setStandby() {
	Serial1.print("C,0\r");	
}

void SensorEC::setProbeType() {
	Serial1.print("K,1.00\r");
	if (_serialDbg)
		Serial.println("k1.0");
}

void SensorEC::setDry() {
	Serial1.print("Cal,dry\r");
 	if (_serialDbg)
 		Serial.println("dry cal");	
}

void SensorEC::setLowCalib() {
	Serial1.print("Cal,low,12880\r");
 	if (_serialDbg)
 		Serial.println("12,880 uS cal");
}

void SensorEC::setHighCalib() {
	Serial1.print("Cal,high,80000\r");
 	if (_serialDbg)
 		Serial.println("80,000 uS cal");
}

//Adjusts EC sensor readings to given temperature
void SensorEC::adjustTemp(float tempt) {
	if ((tempt != 0) && (!_calibratingEc)) {
		//Convert temp from float to char*
		char tempArray[4];
		dtostrf(tempt,4,2,tempArray);
		String command = "T," + (String)tempArray + "\r";
		Serial1.print(command);
	}
}

void SensorEC::smooth() {
	float res = 0;
	for (uint8_t i = 0; i < _numSamples; i++) { res += _ecs[i]; }
	_ec = (float)(res / _numSamples);
}


//Clears incoming buffers
void SensorEC::clearECbuffer() {
	while (Serial1.available() > 0)
		Serial1.read();	
}

//Output EC circuit's response to serial
void SensorEC::ecToSerial() {
	if (_serialDbg) {
		if (Serial1.available() > 0) {
			String sensorString = "";
			sensorString.reserve(30);
			//Read data from sensor
			char inchar;
			while (Serial1.peek() != '\r') {
				inchar = (char)Serial1.read();
				sensorString += inchar;
			}
			//Discard <CR>
			Serial1.read();
			Serial.println(sensorString);
		}
	}
}
