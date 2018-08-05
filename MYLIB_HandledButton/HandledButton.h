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

#define BTN_ACTIVE_LOW 0 //���� ������ ��������� ������� �����, ����� ������� ������� = 0
#define BTN_ACTIVE_HIGH 1 //���� ������ ��������� ������� �����, ����� ������� ������� = 1
#define BTN_ACTIVE_HIGH_INVERTED 2 //���� ������ ��������� ������� �����, ����� ������� ������� = 1, �� ����� ������������� �������� � +5v

#define BTN_DEFAULT_HOLD_TIME 30 //����������� ����� ��� ��������� ��������� = 30 �����������.

#define DEV_BTN_MAX_COUNTER_VALUE 960000 //������������ ����� ��� ������� ���������. �������� �� ������������.

// ��������, ��� ���������� ��� �� ����������
#ifndef HandledButton_h // ���� ���������� �� ����������
#define HandledButton_h // ����� ���������� ��

#include "Arduino.h"

class HandledButton {
	public:
		HandledButton(byte pin, word timerInterval, unsigned long minimalHoldTime = BTN_DEFAULT_HOLD_TIME, byte buttonActiveState = BTN_ACTIVE_LOW);
		void attachHandlerToPushDown(void (*interruptFunc)());
		void attachHandlerToPullUp(void (*interruptFunc)());
		void attachHandlerToChange(void (*interruptFunc)());
		unsigned long getTimeInCurrentState();
		unsigned long getTimeInLastState();
		boolean isPressed();
		boolean isClicked();
		void processStep();
		void processHandlers();
	private:
		void (*pushDownHandler)();
		void (*pullUpHandler)();
		void (*changedHandler)();
		boolean dev_changed;
		boolean dev_lastReadedState;
		boolean dev_currentStableState;
		boolean dev_pushedDown;
		boolean dev_clicked;
		boolean dev_pulledUp;
		boolean dev_buttonActiveState;
		unsigned long dev_minimalHoldTime;
		word dev_timerInterval;
		unsigned long dev_holdStateCounter;
		unsigned long dev_timeInLastState;
		byte _pin;
};

#endif