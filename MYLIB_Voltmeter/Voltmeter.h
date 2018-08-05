/**
	Voltmeter_h - ����������, ����������� ����������� ��������� ���������� ���, ��� ��������� � ���������.
	������� ���������� ������ �������� � �����������, ����������� �������� ������ ����������� ��������� ���� �����, ����� ������������ ��������� �������������
	��������� ���������� � �����������.
	��� �������� �����������:
		* ���, �� ������� ����� ������ ���������.
		* �������������� �������� - ������� ���������� ���
		* �������������� �������� - ������������� �������� ��������� ��������
		* �������������� �������� - ������������� ������� ��������� ��������
		* �������������� �������� - ���������� ������� ��� ���������� ��������

	���� ��� �������� ������� ������ ���, ��������� ����� �������� ������� ����������� ���������� �� ��������� ������, ��������� � �������� �������� ���������� 5 �����.
	���� ������������� ������� � ������� ����������, �� ��������� ����� �������������� ������ �� ����.
	��� ���������� ��������� ���������� ��������, ��� �������, ���������� ������� ����������� ��������, � � ������������ �������� ��������� �������� � ������� ���������.
	����� ������������� �������� ���������� �������. ���� ���������� ��������� ���������� - ������� 1.
	�����, ��������� ���������� ������� �������� ������� setFilterSamplesCount(int countOfSamples). 
	� �������� �������� ���������� ����� ������������� ���������� ���������� �����������, ��� ��� ��� ����� ���������.
	��� ����, ����� ������ ������� ������� ���������� � ������������ �����������, ����������� ��� �������:
	|-------------------------------|
	|	REF / (R2 / ( R1 + R2 ) )	|
	|-------------------------------|
	��� REF - ������� ����������, � R1 � R2 - ��������� �������� � ������� ���������� �������� ��������������.
	��� ���������� ���������� ��������� � ����� ��� ���������� � ����������� ����������, � �����/�������/��������� ������ ���������� ����� processMeasurement()
	��� ����, ����� ������ ������� ������� ���������� �� ���� ������, ���������� ����� getVoltage().
	�������� �� ���� �����. ExtNeon. 05.11.2017
*/

// ��������, ��� ���������� ��� �� ����������
#ifndef Voltmeter_h // ���� ���������� �� ����������
#define Voltmeter_h // ����� ���������� ��

#include "Arduino.h"

class Voltmeter {
	public:
		Voltmeter(byte measurement_Pin, float ctrl_ref_voltage = 5., float rdiv_TopResistance = 0, float rdiv_BottomResistance = 1, int filterCountOfSamples = 6);
		void setDividerParams(float rdiv_TopResistance, float rdiv_BottomResistance, float ctrl_ref_voltage);
		void setFilterSamplesCount(int countOfSamples = 6);
		float getVoltage();
		void processMeasurement();
	private:
		byte _pin;
		//short *dev_vlmSumValue;
		int dev_maxFilterSamplesCount; //������������ ���������� �������� ��� ����������
		unsigned long dev_measuresSum = 0;
		int dev_countOfMeasures = 0;
		short dev_sum = 0;
		double dev_transferCoeff;
		boolean dev_changed = true;
		float dev_lastResult;
};

#endif