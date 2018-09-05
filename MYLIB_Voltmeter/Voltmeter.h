/**
	Voltmeter_h - библиотека, позволяющая представить отдельный аналоговый пин, как вольтметр с делителем.
	Имеется внутренний фильтр значений с усреднением, позволяющий получить точный усреднённый результат даже тогда, когда производится измерение нестабильного
	источника напряжения с пульсациями.
	Плюс именно данной библиотеки в том, что вследствие прямой работы с регистрами микроконтроллера, его скорость даже с использованием усреднения весьма высока, 
	а размер при этом сильно уменьшается. Используется функция фонового преобразования, что позволяет измерять напряжение за примерно 116 микросекунд даже при высоком
	(порядка 20 - 30) количестве выборок. Однако учтите, что основные операции с фильтрацией производятся в методе getVoltage, поэтому большое количество выборок не только
	сильно увеличивает размер занимаемой оперативной памяти, но и негативно сказывается на скорости получения напряжения и, отчасти, на скорости его измерения.
	При создании указывается:
		* Пин, на котором будет висеть вольтметр.
		* Необязательный параметр - опорное напряжение АЦП
		* Необязательный параметр - сопротивление верхнего резистора делителя
		* Необязательный параметр - сопротивление нижнего резистора делителя
		* Необязательный параметр - количество выборок для стабилизации/фильтрации значений
		* Необязательный параметр - пороговый разброс между выборками. Если 0 - то будет искать одинаковые

	Если при создании указать только пин, вольтметр будет измерять текущее фактическое напряжение на указанном выводе, используя в качестве опорного напряжения 5 вольт.
	Если дополнительно указать и опорное напряжение, то результат будет рассчитываться исходя из него.
	Для проведения измерений напряжения большего, чем опорное, необходимо сделать резистивный делитель, а в конструкторе вводятся параметры верхнего и нижнего резистора.
	Можно дополнительно изменить количество выборок. Если необходимо отключить фильтрацию - укажите 1. В дополнение, можно изменить пороговый разброс при фильтрации.
	Также, изменение количества выборок и разброса возможно методом setFilterSamplesCount(byte countOfSamples, byte filterRange). 
	В качестве опорного напряжения лучше устанавливать внутреннее напряжение контроллера (1.1v), так как оно более стабильно.
	Для того, чтобы узнать верхнюю границу вольтметра с определёнными параметрами, используйте эту формулу:
	|-------------------------------|
	|	REF / (R2 / ( R1 + R2 ) )	|
	|-------------------------------|
	Где REF - опорное напряжение, а R1 и R2 - параметры верхнего и нижнего резисторов делителя соответственно.
	Для проведения очередного измерения и учёта его результата в усреднённом напряжении, в цикле/таймере/отдельном потоке вызывается метод processMeasurement()
	Для того, чтобы узнать текущий уровень напряжения на этом выводе, вызывается метод getVoltage().
	Написано за один вечер. ExtNeon. 05.11.2017
*/

// проверка, что библиотека еще не подключена
#ifndef Voltmeter_h // если библиотека не подключена
#define Voltmeter_h // тогда подключаем ее

#if defined (__AVR_ATmega328__) || defined (__AVR_ATmega328P__) || defined(__AVR_ATmega168__) || defined(__AVR_ATmega88__) 

#define shl(x) ((x << 1) | (x >> 7)) //Cyclic shift bits in byte to the left
#define shr(x) ((x >> 1) | (x << 7)) //Cyclic shift bits in byte to the right

#include "Arduino.h"
#include <avr/io.h>

class Voltmeter {
	public:
		Voltmeter(byte measurement_Pin, float ctrl_ref_voltage = 5., float rdiv_TopResistance = 0, float rdiv_BottomResistance = 1, byte filterCountOfSamples = 3, byte filterRange = 12);
		void setDividerParams(float rdiv_TopResistance, float rdiv_BottomResistance, float ctrl_ref_voltage);
		void setFilterSamplesCount(byte countOfSamples, byte filterRange);
		float getVoltage();
		void processMeasurement();
	private:
		byte _pin;
		//short *dev_vlmSumValue;
		byte dev_maxFilterSamplesCount; //Максимальное количество сложений для усреднения
		word* samples = NULL;
		//int dev_countOfMeasures = 0;
		//short dev_sum = 0;
		double dev_transferCoeff;
		boolean dev_changed = true;
		float dev_lastResult;
		byte _filterRange;
};
#else
#error  Ваш контроллер библиотекой Voltmeter Registers Operation не поддерживается
#endif
#endif