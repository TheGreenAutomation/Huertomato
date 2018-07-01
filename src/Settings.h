// #############################################################################
//
// # Name       : Settings
// # Version    : 1.7
//
// # Author     : Juan L. Perez Diez <ender.vs.melkor at gmail>
// # Date       : 20.06.2016
//
// # Description: Settings class for Huertomato
// # Stores all the system's current settings. Its in charge of reading and storing in EEPROM 
//
// #  This program is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with this program.  If not, see <http://www.gnu.org/licenses/>.
//
// #############################################################################

#ifndef SETTINGS_H
#define SETTINGS_H

#include <Arduino.h>
#include <EEPROMEx.h>
#include <Time.h>
#include <DS1307RTC.h>

extern EEPROMClassEx EEPROM;

extern const float versionNumber;

// *********************************************
class Settings {
  public:
	enum Setting {
		None,
		WaterTimed,
		WaterHour,
		WaterMinute,
		FloodMinute,
		PHalarmUp,
		PHalarmDown,
		ECalarmUp,
		ECalarmDown,
		WaterAlarm,
		NightWatering,
		LightThreshold,
		MaxWaterLvl,
		MinWaterLvl,
		PumpProtection,
		PumpProtectionLvl,
		SensorSecond,
		SDactive,
		SDhour,
		SDminute,
		Sound,
		Led,
		Celsius,
		SerialDebug,
		ReservoirModule,
		NextWhour,
		NextWminute,
		NightWateringStopped,
		WateringPlants,
		AlarmTriggered,
		Version
	};
	
	//Constructors
    Settings();
	Settings(const Settings &other);
	Settings& operator=(const Settings &other);
	//Destructor
	~Settings();
	
	//Loads defaults to EEPROM and updates object
	void loadDefaults();
	
    //Setters - These store settings to EEPROM too
    //System Settings
    boolean setWaterTimed(const boolean);
    boolean setWaterHour(const uint8_t);
    boolean setWaterMinute(const uint8_t);
    boolean setFloodMinute(const uint8_t);
    boolean setPHalarmUp(const float);
    boolean setPHalarmDown(const float);
    boolean setECalarmUp(const float);
    boolean setECalarmDown(const float);
    boolean setWaterAlarm(const uint8_t);
    boolean setNightWatering(const boolean);
	boolean setLightThreshold(const uint16_t);
	boolean setMaxWaterLvl(const uint16_t);
	boolean setMinWaterLvl(const uint16_t);
	boolean setPumpProtection(const boolean);
	boolean setPumpProtectionLvl(const uint8_t);
    
    //Controller Settings
    boolean setSensorSecond(const uint8_t);
    boolean setSDactive(const boolean);
    boolean setSDhour(const uint8_t);
    boolean setSDminute(const uint8_t);
    boolean setSound(const boolean);
	boolean setLed(const boolean);
	boolean setCelsius(const boolean);
    boolean setSerialDebug(const boolean);
	boolean setReservoirModule(const boolean);
    
    //State vars - These are not written to EEPROM
	//They are meant to be set only by main .ino logic, not by user!
    void setNextWhour(const uint8_t);
    void setNextWminute(const uint8_t);
    void setNightWateringStopped(const boolean);
    void setWateringPlants(const boolean);
    void setAlarmTriggered(const boolean);
	void setPumpProtected(const boolean);
    
    //Getters
    //System Settings
    boolean getWaterTimed() const;
    uint8_t getWaterHour() const;
    uint8_t getWaterMinute() const;
    uint8_t getFloodMinute() const;
    float getPHalarmUp() const;
    float getPHalarmDown() const;
    float getECalarmUp() const;
    float getECalarmDown() const;
    uint8_t getWaterAlarm() const;
    boolean getNightWatering() const;
	uint16_t getLightThreshold() const;
	uint16_t getMaxWaterLvl() const;
	uint16_t getMinWaterLvl() const;
	boolean getPumpProtection() const;
	uint8_t getPumpProtectionLvl() const;
    
    //Controller Settings
    uint8_t getSensorSecond() const;
    boolean getSDactive() const;
    uint8_t getSDhour() const;
    uint8_t getSDminute() const;
    boolean getSound() const;
	boolean getLed() const;
	boolean getCelsius() const;
    boolean getSerialDebug() const;
	boolean getReservoirModule() const;
    
    //Status vars
    uint8_t getNextWhour() const;
    uint8_t getNextWminute() const;
    boolean getNightWateringStopped() const;
    boolean getWateringPlants() const;
    boolean getAlarmTriggered() const;
	boolean getPumpProtected() const;
	//Internal EEPROM version
	float getVersion() const;
	
	//These return value and go to false
	boolean systemStateChanged();
	boolean waterSettingsChanged();
	boolean sdSettingsChanged();
	boolean sensorPollingChanged();
	boolean serialDebugChanged();
	boolean moduleChanged();
	
	//Updates RTC internal time
	//Breaks encapsulation as it uses RTC object directly 
	//Needed for it to be called both by GUI and main .ino
	void setRTCtime(uint8_t, uint8_t, uint8_t, uint8_t, uint8_t, int);
     
	//void debugEEPROM();
	  
  private:
	void setEEPROMaddresses();
    void readEEPROMvars();
	
    //System Settings
    //Watering Cycle
    boolean _waterTimed;
    uint8_t _waterHour;
    uint8_t _waterMinute;
    uint8_t _floodMinute;
    //Sensor Alarms
    float _phAlarmUp;
    float _phAlarmDown;
    float _ecAlarmUp;
    float _ecAlarmDown;
    uint8_t _waterAlarm;
    //Water at night - Informs the system that watering timers are stopped for the night
    boolean _nightWatering;
	//Sensor Calibrations
	uint16_t _lightThreshold;
	uint16_t _maxWaterLvl;
	uint16_t _minWaterLvl;
	//Pump protection
	boolean _pumpProtection;
	uint8_t _pumpProtectionLvl;
    
    //Controller settings
    //Time & Date - Handled outside in RTC Lib
    //Sensor Polling
    uint8_t _sensorSecond;  
    //SD Card
    boolean _sdActive;
    uint8_t _sdHour;
    uint8_t _sdMinute;
    //Sound toggle
    boolean _sound;
	//Leds toggle
	boolean _led;
	//Celsius toggle
	boolean _celsius;
    //Serial Debugging
    boolean _serialDebug;
	//Module present
	boolean _reservoirModule;
    
    //Status variables - Not read from EEPROM
    //Time next watering will happen
    uint8_t _nextWhour;
    uint8_t _nextWminute;
    //Informs the system that watering timers are stopped for the night
    boolean _nightWateringStopped;
    //Turned on when plants are being watered
    boolean _wateringPlants;
    //Informs if theres an alarm triggered
    boolean _alarmTriggered;
	//True if pump is off for protection
	boolean _pumpProtected;
	//True if state changed
	boolean _systemStateChanged;
	//Tells if water settings have been changed
	boolean _waterSettingsChanged;
	//SD options changed
	boolean _sdSettingsChanged;
	//Sensor polling changed
	boolean _sensorPollingChanged;
	//Serial debug toggle
	boolean _serialDebugChanged;
	//Module config changed
	boolean _moduleChanged;
	//Version number
	float _version;
    
    //EEPROM addresses for all settings
    int _addressWaterTimed;
    int _addressWaterHour;
    int _addressWaterMinute;
    int _addressFloodMinute;
    int _addressPHalarmUp;
    int _addressPHalarmDown;
    int _addressECalarmUp;
    int _adressECalarmDown;
    int _addressWaterAlarm;
    int _addressNightWatering;  
    int _addressSensorMinute;
    int _addressSensorSecond;  
    int _addressSDactive;
    int _addressSDhour;
    int _addressSDminute;
    int _addressSound;
	int _addressLed;
	int _addressCelsius;
    int _addressSerialDebug;
	int _addressLightThreshold;
	int _addressReservoirModule;
	int _addressMaxWaterLvl;
	int _addressMinWaterLvl;
	int _addressPumpProtectionLvl;
	int _addressPumpProtection;
	int _addressVersion;
};

#endif