/**
	Voltmeter_h - ����������, ����������� ����������� ��������� ���������� ���, ��� ��������� � ���������.
	������� ���������� ������ �������� � �����������, ����������� �������� ������ ���������� ��������� ���� �����, ����� ������������ ��������� �������������
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
	��� ���������� ���������� ��������� � ����� ��� ���������� � ���������� ����������, � �����/�������/��������� ������ ���������� ����� processMeasurement()
	��� ����, ����� ������ ������� ������� ���������� �� ���� ������, ���������� ����� getVoltage().
	�������� �� ���� �����. ExtNeon. 05.11.2017
*/

#include "Arduino.h"
#include "Voltmeter.h"

Voltmeter::Voltmeter(byte measurement_Pin, float ctrl_ref_voltage, float rdiv_TopResistance, float rdiv_BottomResistance, int filterCountOfSamples) {
	setDividerParams(rdiv_TopResistance, rdiv_BottomResistance, ctrl_ref_voltage);
	//dev_vlmSumValue = (short *) malloc(sizeof(short) * midMeasuresCount);
	/*for (int i = 0; i < midMeasuresCount; i++) {
		dev_vlmSumValue[i] = 0;
	}*/
	setFilterSamplesCount(filterCountOfSamples);
	_pin = measurement_Pin;
	pinMode(measurement_Pin, INPUT);
}

void Voltmeter::setDividerParams(float rdiv_TopResistance, float rdiv_BottomResistance, float ctrl_ref_voltage) {
	dev_transferCoeff = (ctrl_ref_voltage / 1024.) / (rdiv_BottomResistance / (rdiv_TopResistance + rdiv_BottomResistance));
}

void Voltmeter::setFilterSamplesCount(int countOfSamples) {
	dev_maxFilterSamplesCount = countOfSamples / 2; //�������� ������������
	dev_maxFilterSamplesCount = dev_maxFilterSamplesCount < 1 ? 1 : dev_maxFilterSamplesCount; //�� ��������� �������� �����. ����...
}

void Voltmeter::processMeasurement() {
	/*for (int i = dev_maxFilterSamplesCount - 2; i >= 0; i--) {
		dev_vlmSumValue[i] = dev_vlmSumValue[i+1];
	}*/
	//dev_vlmSumValue[dev_maxFilterSamplesCount-1] = analogRead(_pin);
	dev_measuresSum += analogRead(_pin);
	if (++dev_countOfMeasures >= dev_maxFilterSamplesCount) {
		short averageADCValue = (dev_measuresSum / dev_countOfMeasures);
		if (dev_maxFilterSamplesCount > 1) {
			dev_sum = (dev_sum + averageADCValue) / 2;
		} else {
			dev_sum = averageADCValue;
		}
		dev_measuresSum = 0;
		dev_countOfMeasures = 0;
		dev_changed = true;
	}
}

float Voltmeter::getVoltage() {
	if (!dev_changed) {
		return dev_lastResult;
	}
	/*for (int i = 0; i < dev_maxFilterSamplesCount; i++) {
		_sum += dev_vlmSumValue[i];
	}*/
	dev_lastResult = dev_sum * dev_transferCoeff;
	dev_changed = false;
	return dev_lastResult;
}