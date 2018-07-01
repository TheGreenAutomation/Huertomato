#include "WinAlarms.h"

WinAlarms::WinAlarms(UTFT *lcd, UTouch *touch, Sensors *sensors, Settings *settings) 
: Window(lcd,touch,sensors,settings) { }

WinAlarms::WinAlarms(const WinAlarms &other) : Window(other) {
	for (uint8_t i = 0; i < _nSensorAlarmsButtons; i++) {
		_sensorAlarmsButtons[i] = other._sensorAlarmsButtons[i];
	}
}
	
WinAlarms& WinAlarms::operator=(const WinAlarms& other) {
	_lcd = other._lcd;
	_touch = other._touch;
	_sensors = other._sensors;
	_settings = other._settings;
	_buttons = other._buttons;
	for (uint8_t i = 0; i < _nSensorAlarmsButtons; i++) {
		_sensorAlarmsButtons[i] = other._sensorAlarmsButtons[i];
	}
	return *this;
}

WinAlarms::~WinAlarms() {}

Window::Screen WinAlarms::getType() const {
	return Window::Alarms;
}

void WinAlarms::print() {	
	_lcd->setColor(lightGreen[0],lightGreen[1],lightGreen[2]);
	_lcd->setBackColor(VGA_WHITE);
	_lcd->setFont(various_symbols);
	//Make menu buttons with it's bulletpoints
	//Before the buttons were adding there are the flow buttons
	for (uint8_t i = 0; i < _nSensorAlarmsButtons - _nFlowButtons; i++) {
		_lcd->print(pmChar(bulletStr),_xMenu,_yThreeLnsFirst+_bigFontSize*_yFactor3lines*i);
		_sensorAlarmsButtons[i + _nFlowButtons] = _buttons.addButton(_xMenu+_bigFontSize*2,_yThreeLnsFirst+_bigFontSize*_yFactor3lines*i,(char*)pgm_read_word(&sensorAlarmsButtonsText[i]));
	}
}
 
//Draws entire screen Sensor Alarms
void WinAlarms::draw() {
	_lcd->fillScr(VGA_WHITE);
	_buttons.deleteAllButtons();
	printMenuHeader(nameWinAlarms);
	addFlowButtons(true,false,true,_sensorAlarmsButtons);
	print();
	_buttons.drawButtons();
}

Window::Screen WinAlarms::processTouch(const int x, const int y) {
	int buttonIndex = _buttons.checkButtons(x,y);
	//Back
	if (buttonIndex == _sensorAlarmsButtons[0])
		return Reservoir;
	//Exit
	else if (buttonIndex == _sensorAlarmsButtons[2])
		return MainScreen;
	//pH Thresholds
	else if (buttonIndex == _sensorAlarmsButtons[3])
		return PhAlarms;
	//EC Thresholds
	else if (buttonIndex == _sensorAlarmsButtons[4])
		return EcAlarms;
	//Nutrient Level
	else if (buttonIndex == _sensorAlarmsButtons[5]) 
		return LvlAlarms;
	return None;
}
