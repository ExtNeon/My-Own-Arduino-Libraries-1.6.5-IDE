/**
	SevenSegmentsIndicator_h - ����������, ����������� ����������������� � �������������� �����������, ��� � ��� - ��������.
	��������� ���������� �������������� ���������� � ����� ����������� ���� � ����� ����������� ���������.
	����� �������� �����, � ��� ����� � �������, � ����� ��������� ����� (ABCDEFGHIJLNOPQRSTUVY) � ������� (-; [; ] � .). �����, ������� ����������� ������ ����������� ��������.
	��������� �������������� ������������ ��������. ��� ���������� ������ ���������� ����� ������������ �������� ����� refreshNext(). �� ����������� �������� ������ � ������� �� 
	���� ������, ����������� � ������� ������� ������� � ������. � ����������� �� ������� ������ ������� ������ ����� ���������� �������� ����� �������� ����������.
	�� ������������� �������� ��� ����, ��� � ������ 20 �����������, ��� ��� ��� ���������� ��������� ������ �������� ������� ������������ ��������.
	
	��� �������� �����������:
		* ������ ������, ������ �� ������� �������� ������� ���� ������������ ��������. ������������������ �����: A, B, C, D, E, F, G, dp.
		* ���������� ��������
		* ������ ������, ������ �� ������� �������� ������� ���� ������������ �������.
		* �������������� ��������: ��� ������� ������� ����������. ��������� ������ ��������. ���� � ������ ������� ���������� ������ ����������� ���������, 
			�� ����� ������� SSI_DGPIN_KATHODE. �� ��������� ����������� ��������, ��� ������� �������� ����������, � ������������ ������� ����������� ���������.
			���� �� ���������� �� ���������, �� ��������� �� ��������, ���������� ������� ��� ��������.
	
	��� ������ ���������� �� ������� ����� ������������ ��������� ������:
		* ����� print(String str, boolean shiftToRight). ��������� �� ���� ������, �������������� � � ���������� � ������ ��� ������. �������������� �������� - ����� ������������.
			���� ������� false, �� ��������� ����� ����� �������� �����, �� ��������� ������.
		* ����� setDigitValue(byte digitIndex, byte value). ��������� �� ���� ������ ������� � ��������������� ��������. ������������� ����������� �������� ������� �� �������.
			�������� ������� �������� � ����� �����, � ������� ������ ��� ����������� � ����������� ���������. 
			������������������ (������� �� �������� ����): A, B, C, D, E, F, G, dp.
		* ����� displayCustomSymbols(byte* symbols, byte countOfSymbols). ��������� ������� ������������������ ���������������� �������� � ������������� �����. ��������� �� ����
			������ ������, ������ ���� �� �������� ����������� � ����������� ��������, � ������ ��� � ����������� ��������� � ������ �������; � ��� �����.
			������������������ (������� �� �������� ����): A, B, C, D, E, F, G, dp.
		
	����� ��������� � ����� �������� ���������  ������� setPowerState(), ������������� ����� �������� ������� stopRefreshing() � ������������ � ������� resumeRefreshing().
	�����, ����� ��������� ��������� ��������� ����� ��� ������ ������ setPointShow(), ������� ��������� �� ���� ������ ��������.
	�������� �� ���� �����. ExtNeon. 06.06.2018
	
	
	����� ���������:

		|--A--|
		F	  B
		|--G--|
		E	  C
		|--D--|
				dp
		
*/

// ��������, ��� ���������� ��� �� ����������
#ifndef SevenSegmentsIndicator_h // ���� ���������� �� ����������
#define SevenSegmentsIndicator_h // ����� ���������� ��
#include "Arduino.h"

const byte SSI_DIGIT_ZERO = 0b11111100;
const byte SSI_DIGIT_ONE = 0b01100000;
const byte SSI_DIGIT_TWO = 0b11011010;
const byte SSI_DIGIT_THREE = 0b11110010;
const byte SSI_DIGIT_FOUR = 0b01100110;
const byte SSI_DIGIT_FIVE = 0b10110110;
const byte SSI_DIGIT_SIX = 0b10111110;
const byte SSI_DIGIT_SEVEN = 0b11100000;
const byte SSI_DIGIT_EIGHT = 0b11111110;
const byte SSI_DIGIT_NINE = 0b11110110;

const byte SSI_LETTER_A = 0b11101110;
const byte SSI_LETTER_B = 0b00111110;
const byte SSI_LETTER_C = 0b10011100;
const byte SSI_LETTER_D = 0b01111010;
const byte SSI_LETTER_E = 0b10011110;
const byte SSI_LETTER_F = 0b10001110;
const byte SSI_LETTER_G = 0b11011110;
const byte SSI_LETTER_H = 0b01101110;
const byte SSI_LETTER_I = SSI_DIGIT_ONE;
const byte SSI_LETTER_J = 0b01111000;
const byte SSI_LETTER_L = 0b00011100;
const byte SSI_LETTER_N = 0b11101100;
const byte SSI_LETTER_O = SSI_DIGIT_ZERO;
const byte SSI_LETTER_P = 0b11001110;
const byte SSI_LETTER_Q = 0b11100110;
const byte SSI_LETTER_R = 0b10001100;
const byte SSI_LETTER_S = SSI_DIGIT_FIVE;
const byte SSI_LETTER_T = 0b00011110;
const byte SSI_LETTER_U = 0b01111100;
const byte SSI_LETTER_V = SSI_LETTER_U;
const byte SSI_LETTER_Y = 0b01110110;

const byte SSI_SYMBOLS_MINUS = 0b00000010;
const byte SSI_SYMBOLS_OPEN_BRACKET = 0b10011100;
const byte SSI_SYMBOLS_CLOSED_BRACKET = 0b11110000;
const byte SSI_ADDITIVE_DOTPOINT = 0b00000001;


#define SSI_DGPIN_ANODE true
#define SSI_DGPIN_KATHODE false


class SevenSegmentsIndicator {
	public:
		SevenSegmentsIndicator(byte *segmentPins, byte countOfDigits, byte *digitsPins, boolean digitPinType = SSI_DGPIN_ANODE); //+
		SevenSegmentsIndicator();
		void refreshNext(); //+
		void print(String str, boolean shiftToRight = true); //+
		void displayCustomSymbols(byte* symbols, byte countOfSymbols); //++
		void setPowerState(boolean enabled); //+
		boolean getPowerState(); //+
		void stopRefreshing(); //+
		void resumeRefreshing(); //+
		void setDigitValue(byte digitIndex, byte value); //+
		void setPointShow(boolean enabled); //+
	private:
		byte *_segmentPins;
		byte *_digitPins;
		byte _currentActiveDigit = 0;
		byte _countOfDigits;
		byte *_indicatorMemory;
		boolean _enabled = true;
		boolean _refreshing = true;
		boolean _showPoint = true;
		boolean _digitPinType;
		void setSegmentsState(byte value); //+
		void insolateDigitPins(); //+
		byte interpretateSymbolToActiveSegments(char __inputSymbol ); //+
		boolean getBitState(byte input, byte bitIndex); //+
};

#endif