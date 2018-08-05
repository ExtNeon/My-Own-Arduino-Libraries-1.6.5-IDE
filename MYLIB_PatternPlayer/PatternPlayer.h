/**
	PatternPlayer_h - ����������, ����������� �������������� ��������� ������� �������� ������������������ ������ � ����������.
	��� ������ �������������, ��������� ������� �������� ������������������ (�������). ����� �������������� ������� ��������, ���� �������� 
	������������ �����, � ������� �������� ����� ���� �������������� �������.
	� ������� �������� � ��������� ������������������ �������� � ����� SignalPattern.h
	������ ���������� ���������� ������ ������ ��� ������ ������� ��� �� 9 � 10 ����. �� ���� ��������� ����� ������� ���������� �� �����.
	��� �������� �����������:
		* ����� � ����� ����������� �������������� ����. ������ �������� - �������, ������ - ����������������� ���������������. ���� ����� ���������� �����, ����� ����������
			������������� ����.
		* ����� ��� ����������, ������ ����� ��������� ��������������� �����.
		* �������� ����� �������� ������ processStep().
	� ������������ ������ ����� ���������� �������� ����� processStep().
	��� ��������������� ����� ������������ ��������� ��������:
		* ����� ������ play, � ��������� �������� ����� ����������� �������� ������������� ������ � ���������, ������������� ������ � �������������� � �� �����.
			��������� ��� �������������� ��������� - ���������� �������� � ������������ ������������ ���������������. ���� ���������� �������� = 0, �� ������ �� ���������������.
			���� ������������ ������������ = 0, �� ������ ���������������, ���� ���������� ������� � ���������� ��������.
		* ����� ������ play c ��������� ��� � ��������� ����������������� ������ � �������. Ÿ, � ���� �������, ����� �������� � ������� ������ convertInpMelodyToStr, 
			������� ��� ��� ������� � ��������� ��� � �� ������������� � �� �����.
		���� ������� ���� = 0, �� ����� ���������������� ������.
	����� ���������� ���������������, ������ ����� stop(). ����� ����� ����� ��������� ������� ������� play(). ��� ���� ��������������� ������� � ����.
	������������� ��������������� ����� ������� pause(). ��� ����, ��� ����� ����� ���������� � ��� �� �����, ��������� ����� play().
	�������� �� ���� �����. ExtNeon. 08.11.2017
*/

// ��������, ��� ���������� ��� �� ����������
#ifndef PatternPlayer_h // ���� ���������� �� ����������
#define PatternPlayer_h // ����� ���������� ��

#include "Arduino.h"
//#include "SignalPattern.h"


#define PLAYING 1
#define PAUSED 2
#define STOPPED 0
#define DEV_PLAYER_STOPPED_PWM_VAL 0

class PatternPlayer {
	public:
		PatternPlayer(void (*toneFunc) (int frequency, int duration), void (*noToneFunc) (), unsigned int timerInterval);
		void play(int freqArray[], int durationArray[], int arrayLength, int repeatationCount = 1, unsigned int duration = 0);
		void play(String inputMelody, int repeatationCount = 1, unsigned int duration = 0);
		void play();
		void pause();
		void stop();
		byte getState();
		void processStep();
		void processStepMs(unsigned int ms);
	    static String convertInpMelodyToStr(int *freqArr, int *durationArr, int arrLength);
	private:
		unsigned int dev_stepCounter;
		unsigned int dev_currentStepIndex;
		unsigned int dev_timerInterval;
		unsigned int max_duration;
		unsigned int max_duration_timer;
		int dev_repeatationCount;
		int dev_repeatCounter;
		byte dev_currentState;
		int *dev_freqArray;
		int *dev_durationArray;
		int dev_arrayLength;
		boolean dev_notInitialized;
		void (*_toneFunc) (int frequency, int duration);
		void (*_noToneFunc) ();
		void dev_processStepOnly();
		void moveStep(unsigned int moveToMillis);
		void freeArrays();
		boolean newStep;
};

#endif