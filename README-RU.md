Документация на языках: [english](README.md), [русский](README-RU.md).

# Что это?

Это библиотека для управления блоком семисегментных индикаторов и матриц светодиодов на микросхеме MAX7219 для Arduino. Возможно, она работает и на других платформах.


# Как установить

1. Скачать библиотеку. Это можно сделать, нажав кнопку "Download ZIP" справа.
2. Установить библиотеку. В Arduino IDE: верхнее меню — Эскиз — Импорт библиотек... — Добавить библиотеку — выбрать скачанный архив или папку с библиотекой.
3. Подключить библиотеку. В Arduino IDE: верхнее меню — Эскиз — Импорт библиотек... — Segment7Display.


# Примеры

#### Инициализация

```c++
#include <Segment7Display.h>
#define PIN_DIN 2	// Пин для передачи данных
#define PIN_CLK 3	// Пин для тактового сигнала
#define PIN_CS 4	// Пин для сигнала строба
Segment7Display display(PIN_DIN, PIN_CLK, PIN_CS);

void setup() {
	// Для нормально работы дисплея этот метод надо вызвать после инициализации Arduino
	display.reset();
}
```

#### Установка блоку сегментов определённого состояния

```c++	
// Второму справа индикатору задаётся "полосатое" состояние
display.printData(1, 0b10101010);
```

#### Печать символа на блоке

```c++
display.printChar(1, 't');
```

#### Печать строки
	
```c++
display.printString("HELLO");
```

#### Печать дробного числа

```c++	
// Оставляется только 1 символ после запятой
display.printFloat(123.456, 1);
```


# API

## Методы

#### Конструктор

```c++
Segment7Display(int pData, int pClock, int pStrobe)
```

Параметры:
* `int pData` Номер пина, отвечающего за передачу данных (DIN)
* `int pClock` Номер пина, передающего такт (CLK)
* `int pStrobe` Номер пина, передающего сигнал окончания передачи пакета (CS)


#### reset

```c++
void reset(float brightness = 1)
```

Сбрасывает все параметри микросхемы до нормального состояния. Необходимо выполнить этот метод во время или после выполнения функции `setup` Arduino, иначе большая веростность неправильной работы.

Параметры:
* `float brightness` яркость индикаторов. От 0 до 1. По-умолчанию, максимальная.


#### setTest

```c++
void setTest(boolean on)
```

Включает/выключает режим тестирования сегментов. В режиме тестирования все сегменты включаются на полную яркость.

Параметры:
* `boolean on` true — включен, false — выключен


#### setDigitsAmount

```c++
void setDigitsAmount(int amount)
```

Устанавливает количество используемых блоков сегментов (блок — 8 сегментов, цифра).
	
Параметры:
* `amount` Количество блоков. От 1 (только правый) до 8 (все).


#### setDecode

```c++
void setDecode(boolean on)
```

Включает/выключает режим декодирования микросхемой.

В режиме декодирования передаются не состояния сегментов, а номер символа, в соответствии с которым микросхема зажигает определённые сегменты. Номер задаётся 4-мя младшими битами. Символы: `0 1 2 3 4 5 6 7 8 9 - E H L P (пусто)`. 8-й бит — состояние десятичной точки.

При выключенном режиме декодирования биты соответствуют состоянию сегментов. Сегменты указываются в следующем порядке (старший бит слева): DP A B C D E F G.

Параметры:
* `boolean on` true — включен, false — выключен


#### setShutdown

```c++
void setShutdown(boolean on)
```

Включает/выключает режим сна.

Параметры:
* `boolean on` true — включен, false — выключен


#### setBrightness

```c++
void setBrightness(float brightness)
```

Устанавливает яркость всех сегментов. 

Параметры:
* `float brightness` Яркость. От 0 до 1.


#### clear

```c++
void clear()
```

Выключает все сегменты.


#### printByte

```c++
void printByte(byte digit, byte data)
```

Устанавливает блоку сегментов (цифре) состояние. То, какие сегменты включатся, зависит от того, включен ли режим декодирования.

При выключенном режиме декодирования биты соответствуют состоянию сегментов. Сегменты указываются в следующем порядке (старший бит слева): DP A B C D E F G.

При включеном режиме декодирования биты задают номер символа. Номер задаётся 4-мя младшими битами, 8-й бит — состояние десятичной точки. Символы: `0 1 2 3 4 5 6 7 8 9 - E H L P (пусто)`.

Параметры:
* `byte digit` Номер блока. От 0 (правый) до 7 (левый).
* `byte data` Байт состояния. При отключенном режиме декодирования 0 — очистить цифру.


#### printChar

```c++
void printChar(byte digit, char symbol, boolean decimalPoint = false)
```

Устанавливает блоку сегментов (цифре) конкретный человеко-понятный символ. Будут отображаться только симсолы: `0 1 2 3 4 5 6 7 8 9 A b C c d E F G H h I i J j L l n O o p q r S t U u Y y Z . -`, остальные будут заменены на пробел. Буквы, которых нет в списке, но есть их вариант в другом регистре, будут заменены на другой регистр. После выполнения этого метода режим декодирования будет выключен.

Параметры:
* `byte digit` Номер блока. От 0 (правый) до 7 (левый).
* `char symbol` Символ, который нужно вывести
* `boolean decimalPoint` Нужно ли вместе с символом печатать десятичную точку. По-умолчанию, не печатается.


#### printString

```c++
void printString(String str, byte align = ALIGN_RIGHT, boolean clearExcess = true, int offset = 0, int maxLength = -1)
```

Выводит на дисплей строку текста. Будут отображаться только символы, указанные в описании к методу [printChar](#printchar), остальные будут заменены на пробел. После выполнения этого метода режим декодирования будет выключен.

Параметры:
* `String str` Строка, которую нужно вывести
* `byte align` Сторона, к которой примыкает строка. Возможные варианты располагаются в статичных константах [ALIGN_*](#align_) этого класса. По-умолчанию, строка примыкает к правой стороне.
* `boolean clearExcess` Нужно ли очистить блоки цифр, не участвующие в написании этой строки. По-умолчанию, очищаются.
* `int offset` Сдвиг строки относительно прилегаемого края, к которому она прилегает. Измеряется в количестве символов. По-умолчанию, сдвига нет.
* `int maxLength` Максимальная длина строки. Если указать значение меньше нуля, то ограничения по длине не будет. По-умолчанию, ограничения нет.


#### printFloat

```c++
void printFloat(float num, unsigned char precision = 2, boolean E = false, byte align = ALIGN_RIGHT, boolean clearExcess = true, int offset = 0, int maxLength = -1)
```

Выводит на экран число с плавающей точкой. После выполнения этого метода режим декодирования будет выключен.

Параметры:
* `float num` Число, которое нужно вывести
* `unsigned char precision` Количество цифр после запятой. Можно указать отрицательное значени. По-умолчанию, 2.
* `boolean E` true — выводить в экспоненциальной форме; false — выводить в обычной форме. По-умолчанию, обычная форма.
* `int align` Сторона, к которой примыкает строка. Возможные варианты располагаются в статичных константах [ALIGN_*](#align_) этого класса. По-умолчанию, строка примыкает к правой стороне.
* `boolean clearExcess` Нужно ли очистить блоки цифр, не участвующие в написании этой строки. По-умолчанию, очищаются.
* `int offset` Сдвиг строки относительно прилегаемого края, к которому она прилегает. Измеряется в количестве символов. По-умолчанию, сдвига нет.
* `int maxLength` Максимальная длина строки. Если указать значение меньше нуля, то ограничения по длине не будет. По-умолчанию, ограничения нет.


## Статичные константы

### ALIGN_*

Используются для передачи в методы [printString](#printstring) и [printFloat](#printfloat) для указания выравнивания текста.

* `ALIGN_LEFT` Строка будет примыкать к левой стороне. Лишние символы будут обрезаны справа.
* `ALIGN_RIGHT` Строка будет примыкать к правой стороне. Лишние символы будут обрезаны слева.


# Лицензия

MIT (делайте, что хотите).
