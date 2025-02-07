// #include <main.h>
#include <oled_RVV.h>
#include <encoder_RVV.h>
#include <motor_control_RVV.h>



#define POSITION_NOW 50    // позиция слова NOW по горизонтали 
#define POSITION_NEW 50    // позиция слова NEW по горизонтали
#define PAGE_NOW     1     // позиция слова NEW по вертикали (лист)
#define PAGE_NEW     3     // позиция слова NOW по вертикали (лист)

// позиции слов по вертикали (листы), они же номера состояний //
#define SETTINGS         0 
#define ANGLE            2 
#define ROTATION         3 
#define OSCILATION       4 
#define START            5 
#define STOP             6 
/*--------------------------------------------------*/



/* определения состояний (выбор опции или установка значений) */
#define STATE_SELECT 0
#define STATE_SET    1

/* Номера режимов работы (поворот, вращение, осциляция)  */
#define WORK_ANGLE    1
#define WORK_SPEED    2
#define WORK_OSCIL    3




void WRITE_MENU(); // инициализация дисплея и отрисовка в меню    
void write_SETTINGS(); // отрисовка соответствующих слов
void write_ANGLE();
void write_OSCIL();
void write_ROTAT();
void write_START();
void write_STOP();
void write_FOOTNOTE(unsigned char state); // отрисовка единиц измерения (получает состояние для отрисовки соотв. символов)
void write_FOOTNOTE_Clear();


// отрисовка слов нового и сторого значений (получают позиции, возвращают позицию по Х конца слова)
unsigned char write_NEW(unsigned char x, unsigned char page);  
unsigned char write_NOW(unsigned char x, unsigned char page);

/* отрисовка линии */
void write_line();

/* отрисовка ползунка выбора для визуализации состояния */
void clear_selection(unsigned char y);
void write_selection(unsigned char y);

/* отрисовка вещественного числа на позиции */
void write_number(float number, unsigned char x, unsigned char page);
void write_numberClear(unsigned char x, unsigned char page);

