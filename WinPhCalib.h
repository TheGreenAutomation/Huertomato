// #############################################################################
//
// # Name       : WinPhCalib
// # Version    : 1.0
//
// # Author     : Juan L. Perez Diez <ender.vs.melkor at gmail>
// # Date       : 03.01.2015
//
// # Description: pH circuit calibration menu window
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


#ifndef WINPHCALIB_H_
#define WINPHCALIB_H_

#include "Window.h"

const int nPHcalibrationButtons = 6;
static char* phCalibrationButtonsText[nPHcalibrationButtons] = {
	"1. Calibrate for pH7",
	"2. Calibrate for pH4",
	"3. Calibrate for pH10"
};
static int phCalibrationButtons[nPHcalibrationButtons];

static char* nameWinPhCalib = "- pH Circuit Calib. -";

class WinPhCalib: public Window {
	public:
		WinPhCalib(UTFT *lcd, UTouch *touch, Sensors *sensors, Settings *settings);
		WinPhCalib(const WinPhCalib &other);
		WinPhCalib& operator=(const WinPhCalib &other);
		~WinPhCalib();
		Screen getType() const;
		void draw();
		Window::Screen processTouch(const int x, const int y);
	
	protected:
		void print();
};


#endif