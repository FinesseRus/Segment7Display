#pragma once
#include <Arduino.h>

class Segment7Display {
	public:
		// К левой стороне. Строка будет обрезана справа.
		static const byte ALIGN_LEFT = 0;
		// К правой стороне. Строка будет обрезана слева.
		static const byte ALIGN_RIGHT = 1;

		/*
 			Создаёт объект для управления 8-цифровый 7-сегментным индикатором.
			Параметры:
				pData — Номер пина, отвечающего за передачу данных (DIN);
				pClock — Номер пина, передающего такт (CLK);
				pStrobe — Номер пина, передающего сигнал окончания передачи пакета (CS);
		*/
		Segment7Display(int pData, int pClock, int pStrobe);
		
		/*
			Сбрасывает все параметри микросхемы до нормального состояния. Необходимо выполнить этот метод во время или после выполнения функции setup Arduino.
			Параметры:
				brightness — яркость индикаторов. От 0 до 1;
		*/
		void reset(float brightness = 1);

		/*
			Включает/выключает режим тестирования сегментов.
			Параметры:
				on — включить ли;
		*/
		void setTest(boolean on);

		/*
			Устанавливает количество используемых цифровых блоков.
			Параметры:
				amount — количество блоков. От 1 до 8;
		*/
		void setDigitsAmount(int amount);

		/*
			Включает/выключает режим декодирования.
			Параметры:
				on — включить ли;
		*/
		void setDecode(boolean on);

		/*
			Включает/выключает режим сна.
			Параметры:
				on — включить ли;
		*/
		void setShutdown(boolean on);

		/*
			Устанавливает яркость всех сегментов. 
			Параметры:
				brightness — яркость. От 0 до 1;
		*/
		void setBrightness(float brightness);

		/*
			Сбрасывает все индикаторы (выключает).
		*/
		void clear();

		/*
			Устанавливает набору индикаторов, отвечающему за цифру, состояние. То, какие индикаторы включатся, зависит от того, включен ли режим декодирования. Подробнее смотрите здесь: http://www.radiohlam.ru/raznoe/max7219_7221.htm
			Параметры:
				digit — номер блока-цифры. От 0 до 7;
				data — байт состояния. 0 — очистить цифру;
		*/
		void printByte(byte digit, byte data);

		/*
			Устанавливает набору индикаторов, отвечающему за цифру, конкретный человеко-понятный символ. Будут отображаться только символы, указанные в описании к методу char2byte, остальные будут заменены на пробел. После выполнения этой функции режим декодирования будет выключен.
			Параметры:
				digit — номер блока-цифры. От 0 до 7;
				symbol — символ, который нужно вывести;
				decimalPoint — нужно ли вместе с символом выводить десятичную точку;
		*/
		void printChar(byte digit, char symbol, boolean decimalPoint = false);

		/*
			Выводит на экран строку текста. Будут отображаться только символы, указанные в описании к методу char2byte, остальные будут заменены на пробел. После выполнения этой функции режим декодирования будет выключен.
			Параметры:
				str — строка, которую нужно вывести;
				align — сторона, к которой примыкает строка. Возможные варианты располагаются в константах ALIGN_* этого класса;
				clearExcess — очистить блоки цифр, не участвующие в написании этой строки;
				offset — сдвиг строки относительно прилегаемого края;
				maxLength — максимальная длина строки. Если указать значение меньше нуля, то ограничения по длине не будет;
		*/
		void printString(String str, byte align = ALIGN_RIGHT, boolean clearExcess = true, int offset = 0, int maxLength = -1);

		/*
			Выводит на экран число с плавающей точкой. После выполнения этой функции режим декодирования будет выключен.
			Параметры:
				num — число, которое нужно вывести;
				precision — количество цифр после запятой;
				E — выводить ли число в экспоненциальной форме;
				align — сторона, к которой примыкает строка. Возможные варианты располагаются в константах ALIGN_* этого класса;
				clearExcess — очистить блоки цифр, не участвующие в написании этой строки;
				offset — сдвиг строки относительно прилегаемого края;
				maxLength — максимальная длина строки. Если указать значение меньше нуля, то ограничения по длине не будет;
		*/
		void printFloat(float num, unsigned char precision = 2, boolean E = false, byte align = ALIGN_RIGHT, boolean clearExcess = true, int offset = 0, int maxLength = -1);

	protected:
		/*
			Переменные для запоминания пинов. Какие за что отвечают, смотрите в описании Segment7Display.
		*/
		int pinData, pinClock, pinStrobe;

		/*
			Включен ли режим декодирования
		*/
		boolean decodeModeOn;

		/*
			Таблица соответствия ASCII символов состояниям 7-сегментного идникатора
		*/
		static const byte charTable[];

		/*
			Подготавливает пакет для отправки в микросхему.
			Параметры:
				comand — код команды (используются только 4 младших бита);
			Возвращает:
				word Подготовленный пакет.
		*/
		static word preparePacket(byte comand, byte data);

		/*
			Отправляет пакет в микросхему.
			Параметры:
				packet — пакет для отправки;
		*/
		void sendPacket(word packet);

		/*
			Функция пропорционально переносит значение (value) из текущего диапазона значений (fromMin .. fromMax) в новый диапазон (toMin .. toMax), заданный параметрами.
			Как map, только дружит с дробными числами.
		*/
		static int mapNormal(float value, int fromMin, int fromMax, int toMin, int toMax);

		/*
			Преобразует символ в байт, обозначающий, какие сегменты надо зажечь для показа этого символа. Можно задать любой символ, но выведутся только следующие: 0 1 2 3 4 5 6 7 8 9 A b C c d E F G H h I i J j L l n O o p q r S t U u Y y Z . -. Пробел — погасить все индикаторы.
			Параметры:
				symbol — символ, который нужно преобразовать;
				decimalPoint — нужно ли добавлять символу дробную точку;
			Возвращает:
				byte Байт сегментов.
		*/
		static byte char2byte(char symbol, boolean decimalPoint = false);

		/*
			Преобразует строку в набор байтов для отображения на 7-сегментных индикаторах. Будут отображаться только символы, указанные в описании к методу char2byte, остальные будут заменены на пробел.
			Параметры:
				str — строка, которую нужно вывести;
				output — указатель на массив, куда будут писаться байты;
				maxLength — максимальная длина строки. Если указать значение меньше нуля, то ограничения по длине не будет;
				align — сторона, к которой примыкает строка. Возможные варианты располагаются в константах ALIGN_* этого класса;
			Возвращает:
				unsigned int Количество получившихся байт
		*/
		static unsigned int string2bytes(String str, byte *output, int maxLength, byte align = ALIGN_RIGHT);
};