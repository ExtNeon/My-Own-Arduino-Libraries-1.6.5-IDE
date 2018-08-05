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


#include "Arduino.h"
#include <PatternPlayer.h>
//#include <SignalPattern.h>


PatternPlayer::PatternPlayer(void (*toneFunc) (int frequency, int duration), void (*noToneFunc) (), unsigned int timerInterval) {
	dev_timerInterval = timerInterval;
	_toneFunc = toneFunc;
	_noToneFunc = noToneFunc;
	dev_currentState = STOPPED;
	dev_stepCounter = 0;
	dev_repeatCounter = 0;
	dev_currentStepIndex = 0;
	dev_notInitialized = true;
	dev_freqArray = NULL;
	dev_durationArray = NULL;
}

void PatternPlayer::play(int (*freqArray), int (*durationArray), int arrayLength , int repeatationCount, unsigned int duration) {
	dev_notInitialized = true;
	dev_freqArray = freqArray;
	dev_durationArray = durationArray;
	dev_arrayLength = arrayLength;
	dev_repeatationCount = repeatationCount;
	dev_stepCounter = 0;
	dev_repeatCounter = 0;
	dev_currentStepIndex = 0;
	max_duration_timer = 0;
	max_duration = duration;
	if (arrayLength == 0) {
		dev_currentState = STOPPED;
		return;
	}
	dev_currentState = PLAYING;
	dev_notInitialized = false;
	newStep = true;
}

/*
* 
*/
void PatternPlayer::processStep() {
	processStepMs(dev_timerInterval);
}

void PatternPlayer::processStepMs(unsigned int ms) {
	dev_processStepOnly();
	if (dev_currentState == PLAYING) moveStep(ms);
}

void PatternPlayer::dev_processStepOnly() {
	if (dev_currentState != PLAYING) return;
	boolean temp_durationCheck = (max_duration_timer >= max_duration && max_duration != 0);
	if (dev_currentStepIndex >= dev_arrayLength || temp_durationCheck) {
				if (temp_durationCheck) stop();
		if (dev_repeatationCount >= 1) {
			dev_currentStepIndex = 0;
			if (++dev_repeatCounter >= dev_repeatationCount) stop();
			return;
		} else {
			dev_currentStepIndex = 0;
		}
		stop();
		return;
	} else {
		if (newStep && dev_freqArray[dev_currentStepIndex] != 0) {
			//Serial.println(dev_currentStepIndex + "=" +dev_currentPattern.getStep(dev_currentStepIndex).timerInterval);
			_noToneFunc();
			_toneFunc(dev_freqArray[dev_currentStepIndex], dev_durationArray[dev_currentStepIndex]);
						/*setPeriod(dev_currentPattern.getStep(dev_currentStepIndex).timerInterval);
			analogWrite(_playerPin, dev_currentPattern.getStep(dev_currentStepIndex).PWM_value);*/
			newStep = false;
		} else {
			if (dev_stepCounter >= dev_durationArray[dev_currentStepIndex] && dev_durationArray[dev_currentStepIndex] != 0) {
				dev_stepCounter -= dev_durationArray[dev_currentStepIndex];
				dev_currentStepIndex++;
				newStep = true;
			}
		}
	}
}

void PatternPlayer::moveStep(unsigned int moveToMillis) {
	if (dev_durationArray[dev_currentStepIndex] != 0) {
		dev_stepCounter += moveToMillis;
	}
	if (max_duration > 0) {
		max_duration_timer += moveToMillis;
	}
}

void PatternPlayer::stop() {
	dev_currentState = STOPPED;
	dev_currentStepIndex = 0;
	dev_stepCounter = 0;
	dev_repeatCounter = 0;
	max_duration_timer = 0;
	_noToneFunc();
	//Timer1.disablePwm(_playerPin);
}

void PatternPlayer::pause() {
	dev_currentState = PAUSED;
	_noToneFunc();
}

void PatternPlayer::play() {
	if (dev_notInitialized) return; //?��� �� ������ ������������
	dev_currentState = PLAYING;
}

byte PatternPlayer::getState() {
	return dev_currentState;
}


String PatternPlayer::convertInpMelodyToStr(int *freqArr, int *durationArr, int arrLength) {
  //@N#f,d%f,d%!
  String tmp = '@' + String(arrLength) + '#';
  for (int i = 0; i < arrLength; i++) {
    tmp += String(freqArr[i]) + ',' + String(durationArr[i]) + '%';
  }
  tmp += '!';
  return tmp;
}

void PatternPlayer::play(String inputMelody, int repeatationCount, unsigned int duration) {
	dev_notInitialized = true;
	freeArrays();
	dev_arrayLength = inputMelody.substring(inputMelody.indexOf('@')+1, inputMelody.indexOf('#')).toInt();
	if (dev_arrayLength == 0) {
		dev_currentState = STOPPED;
		return;
	}
	dev_freqArray = (int*) malloc(sizeof(int) * dev_arrayLength);
	dev_durationArray = (int*) malloc(sizeof(int) * dev_arrayLength);
	inputMelody = inputMelody.substring(inputMelody.indexOf('#')+1, inputMelody.length());
	for (int i = 0; i < dev_arrayLength; i++) {
		if (inputMelody == "!") {
			freeArrays();
			dev_currentState = STOPPED;
			return;
		}
		dev_freqArray[i] = inputMelody.substring(0, inputMelody.indexOf(',')).toInt();
		dev_durationArray[i] = inputMelody.substring(inputMelody.indexOf(',') + 1, inputMelody.indexOf('%')).toInt();
		inputMelody = inputMelody.substring(inputMelody.indexOf('%')+1, inputMelody.length());
	}
	if (inputMelody.substring(0, 1) != "!") {
		freeArrays();
		dev_currentState = STOPPED;
		return;
	}
	dev_repeatationCount = repeatationCount;
	dev_stepCounter = 0;
	dev_repeatCounter = 0;
	dev_currentStepIndex = 0;
	max_duration_timer = 0;
	max_duration = duration;
	dev_currentState = PLAYING;
	dev_notInitialized = false;
	newStep = true;
}

void PatternPlayer::freeArrays() {
	free(dev_durationArray);
	dev_durationArray = NULL;
	free(dev_freqArray);
	dev_freqArray = NULL;
}