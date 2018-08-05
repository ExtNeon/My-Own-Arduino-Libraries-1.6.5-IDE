/**
	HandledButton_h - ����������, ����������� ������������ ������, ������������ � ����������� �����, ��� �������. 
	� ����������� �������� ����� ���������� ����������� ��� ������� �� ���. 
	��� ��������� ������������ ������������, ����� �������� ����� processHandlers(). ��������� ��� � loop().
	� ������������ ������ ����� ���������� �������� ����� processStep(). ��������, � ����������� ���������� ������� �������.	
	��� �������� �����������:
		* ���, �� ������� ����� ������
		* �������� ����� �������� ������ processStep().
		* �����, � ������� �������� ������ ������ ���������� ���������� ��� ����� �����. �� ��������� ����� BTN_DEFAULT_HOLD_TIME = 30 ����������� (�����������)
		* ��� ������, � ������, � ����� ��������� ������ ���������� ����� ��� �������� ������. (�����������)
			1) BTN_ACTIVE_LOW - ���� ��������� ����������� ������ ���������� � ������ � � ����� - �� ���������, ������������� ��� �������� ������ �����
			2) BTN_ACTIVE_HIGH - ���� ��������� ����������� ������ ���������� � ������ � � +5v. ���������� ������ ������������� �������� �� �����.
			3) BTN_ACTIVE_HIGH_INVERTED - ���� ��������� ��������� ������ ���������� � ������ � � �����.
	����������� ����� ���� ���������� � ��������� ��������:
		* ������ ������ - attachHandlerToPushDown(void (*interruptFunc)());
		* ������ ������ - attachHandlerToPullUp(void (*interruptFunc)());
		* ������ �������� ��� ��������� - attachHandlerToChange(void (*interruptFunc)());
	� ��������� � ���� ������� ����� �������� �������, ������� ����� ���������� ��� ���������� �������. ����� ����������� ����� �������������� �� ���� �����, 
	��� ���������� ����� processHandlers(). 
	�����, ����� ������ ������� ��������� ������ ������� isPressed(). 
	���� �� ������ ������, ���� �� ������ ������ ��� ������������� ������������, �� ����������� ����� isClicked(). �� ���������� ���� �����, ��� ��� ��� ���������� ���������
	���������, ���� �� ������ ������������ ��� � ����������.
	���� �� ������ ��������� ���������� (������� ���������� ������� � ������), �� ����������� ����� getTimeInCurrentState(). �� ����� ���������� �������, � �������
	�������� ������ ��������� � ���������� ���������. �����, ���� �� ������ ������, ������� ������� ������ ���� � ���������� ���������, ����������� ����� getTimeInLastState().
	�������� �� ���� �����. ExtNeon. 07.12.2017
*/

#include "Arduino.h"
#include "HandledButton.h"



HandledButton::HandledButton(byte pin, word timerInterval, unsigned long minimalHoldTime, byte buttonActiveState) {
	_pin = pin;
	pinMode(_pin, buttonActiveState == 1 ? INPUT : INPUT_PULLUP);
	pushDownHandler = NULL;
	pullUpHandler = NULL;
	changedHandler = NULL;
	dev_changed = false;
	dev_pushedDown = false;
	dev_pulledUp = false;
	dev_lastReadedState = false;
	dev_currentStableState = false;
	dev_clicked = false;
	dev_buttonActiveState = buttonActiveState > 0;
	dev_minimalHoldTime = minimalHoldTime;
	dev_timerInterval = timerInterval;
	dev_holdStateCounter = 0;
	dev_timeInLastState = 0;
}

boolean HandledButton::isPressed() {
	return dev_currentStableState;
}

boolean HandledButton::isClicked() {
	boolean temp = dev_clicked;
	dev_clicked = false;
	return temp;
}

void HandledButton::attachHandlerToPushDown(void (*interruptFunc)()) {
	pushDownHandler = interruptFunc;
}

void HandledButton::attachHandlerToPullUp(void (*interruptFunc)()) {
	pullUpHandler = interruptFunc;
}

void HandledButton::attachHandlerToChange(void (*interruptFunc)()) {
	changedHandler = interruptFunc;
}

void HandledButton::processHandlers() {
	if (dev_pushedDown && pushDownHandler != NULL) {
		pushDownHandler();
		dev_pushedDown = false;
	}
	
	if (dev_pulledUp && pullUpHandler != NULL) {
		pullUpHandler();
		dev_pulledUp = false;
	}
	
	if (dev_changed && changedHandler != NULL) {
		changedHandler();
		dev_changed = false;
	}
}

void HandledButton::processStep() {
	boolean dev_gettedState = digitalRead(_pin);
	dev_gettedState = dev_buttonActiveState ? dev_gettedState : !dev_gettedState;
	
	if (dev_gettedState == dev_lastReadedState) {
		dev_holdStateCounter += dev_timerInterval;
	} else {
		if (dev_holdStateCounter > dev_minimalHoldTime) {
			dev_timeInLastState = dev_holdStateCounter;
		}
		dev_holdStateCounter = 0;
		dev_lastReadedState = dev_gettedState;
	}
	
	if (dev_holdStateCounter >= dev_minimalHoldTime && dev_currentStableState != dev_lastReadedState) {
		dev_changed = true;
		dev_currentStableState = dev_lastReadedState;
		if (dev_currentStableState) {
			dev_pushedDown = true;
		} else {
			dev_pulledUp = true;
			dev_clicked = true;
		}
	}
	
	if (dev_holdStateCounter >= DEV_BTN_MAX_COUNTER_VALUE) {
		dev_holdStateCounter = DEV_BTN_MAX_COUNTER_VALUE;
	}
}

unsigned long HandledButton::getTimeInCurrentState() {
	return dev_holdStateCounter > dev_minimalHoldTime ? dev_holdStateCounter - dev_minimalHoldTime : 0;
}

unsigned long HandledButton::getTimeInLastState() {
	return dev_timeInLastState;
}