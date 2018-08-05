/**
	HandledEventTimer_h - ����������, ����������� ��������� ������������ ������� � ����������� ��������� ����������
	� ������������ ������ ����� ���������� �������� ����� processStep().
	��� ��������� ������� ������������ (��������), ����� �������� ����� processHandlers(). ��������� ��� � loop().
	��� �������� �����������:
		* �������� ����� �������� ������ processStep().
	�����, ����� �������� �������. ��� ����� ���������� ����� createEvent(), ���������� �������� �������� �������� ������ ������� � ������� - ����������. �� ���������� ID �������.
	��� �������� �������������� �������, ���������� �������� createRepeatedEvent(). �����, ����� �������� ����� ������� ������� setRepeatability();
	������� ��������� ������� ����� ��������� � ������� ������ getEventState(), �������� ����� �������� ID �������. �� ���������� ������� ��������.
	��� ��� ������, ���� ������� ������������ � ����� ����������� ����� ��������.
	����� ����� �� ���� ������ ��������, �������������� ����������� �������, ������������� ���� ������ ������.
	�������� �� ���� �����. ExtNeon. 08.11.2017
*/

// ��������, ��� ���������� ��� �� ����������
#ifndef EventTimer_h // ���� ���������� �� ����������
#define EventTimer_h // ����� ���������� ��

#include "Arduino.h"

class HandledEvent {
	public:
		boolean flag;
		unsigned long interval;
		unsigned long counter;
		boolean active;
		boolean repeated;
		word maxRepeatCount;
		word repeatationCounter;
		void (*handleProcedure)();
};

class HandledEventTimer {
  private:
	word dev_eventCount;
	boolean enabled;
	HandledEvent *events;
	word dev_timerInterval;
  public:
	HandledEventTimer(word timerInterval);
	word createEvent(unsigned long eventInterval, void (*eventHandler)()); 
	word createRepeatedEvent(unsigned long eventInterval, void (*eventHandler)(), word repeatationCount); //������� ������������� �������.
	void reset(); //���������� ��� �������� ������� 
	void stop(); 
	void start();
	void disableEvent(word eventId);
	void enableEvent(word eventId);
	boolean isEventActive(word eventId); 
	boolean isEnabled();
	void setEventInterval(word eventId, unsigned long interval); 
	void setRepeatability(word eventId, boolean repeatIt, word repeatationCount);
	boolean getEventState(word eventId); //���������� ��������� �������, � ���������� ���. 
	void resetEvent(word eventId); //���������� ������� ������ ������� � ����� ��� ��������� 
	void processStep();
	void processMcsStep(word stepWidthMicros);
	void processStepOptimized();
	void processHandlers(); //���������� � ����.
};

#endif