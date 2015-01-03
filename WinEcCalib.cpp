#include "WinEcCalib.h"

WinEcCalib::WinEcCalib(UTFT *lcd, UTouch *touch, Sensors *sensors, Settings *settings)
: Window(lcd,touch,sensors,settings) { }

WinEcCalib::WinEcCalib(const WinEcCalib &other) : Window(other) { }

WinEcCalib& WinEcCalib::operator=(const WinEcCalib& other) {
	_lcd = other._lcd;
	_touch = other._touch;
	_sensors = other._sensors;
	_settings = other._settings;
	_buttons = other._buttons;
	return *this;
}

WinEcCalib::~WinEcCalib() {}

Window::Screen WinEcCalib::getType() const {
	return Window::EcCalib;
}

void WinEcCalib::print() {
	_lcd->setColor(lightGreen[0],lightGreen[1],lightGreen[2]);
	_lcd->setBackColor(VGA_WHITE);
	//Print bulletpoints
	_lcd->setFont(various_symbols);
	//Before the buttons were adding there are the flow buttons
	for (int i = 0; i < nSensorCalibrationButtons - _nFlowButtons; i++) {
		_lcd->print(bulletStr,_xMenu,_yThreeLnsFirst+_bigFontSize*_yFactor3lines*i);
	}
	//Make menu buttons
	for (int i = 0; i < nSensorCalibrationButtons - _nFlowButtons; i++) {
		sensorCalibrationButtons[i + _nFlowButtons] = _buttons.addButton(_xMenu+_bigFontSize*2,_yThreeLnsFirst+_bigFontSize*_yFactor3lines*i,sensorCalibrationButtonsText[i]);
	}
}

//Draws entire screen Sensor Calibration
void WinEcCalib::draw() {
	_lcd->fillScr(VGA_WHITE);
	_buttons.deleteAllButtons();
	printMenuHeader(nameWinEcCalib);
	addFlowButtons(true,false,true,ecCalibrationButtons);
	print();
	_buttons.drawButtons();
}

Window::Screen WinEcCalib::processTouch(const int x, const int y) {
	int buttonIndex = _buttons.checkButtons(x,y);
	//Back
	if (buttonIndex == sensorCalibrationButtons[0])
		return Reservoir;
	//Exit
	else if (buttonIndex == sensorCalibrationButtons[2])
		return MainScreen;
	else if (buttonIndex == sensorCalibrationButtons[3])
		return LvlCalib;
	else if (buttonIndex == sensorCalibrationButtons[4])
		return PhCalib;
	else if (buttonIndex == sensorCalibrationButtons[5])
		return EcCalib;
	return None;
}