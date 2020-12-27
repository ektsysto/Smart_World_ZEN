//**************************************************************************
// Учебный проект "Бегущие огни" для канала "Разумный мир" Yndex.ZEN
// Цикл статей "Микрокнтроллеры для начинающих"
// Версия 1 для STM8S003F3P6 (STM8S103F3P6)
// Компиляция sdcc версии 4.0 (sdcc -mstm8 run_light_stm8_v1.c)
//**************************************************************************
//
// Использование выводов микроконтроллера (в скобках номер вывода):
// PC3 (13) - F1
// PC4 (14) - F2
// PC5 (15) - F3
// PA1  (5) - кнопка
//**************************************************************************

#include <stdint.h>

//
// Подключенные светодиоды
//

typedef struct {
    unsigned                : 3;
    unsigned run_led_bits   : 3;        // Биты порта с подлюченными LED
    unsigned                : 2;
} run_led_t;

volatile run_led_t __at(0x500A)   PORTC_ODR;
volatile run_led_t __at(0x500C)   PORTC_DDR;
volatile run_led_t __at(0x500D)   PORTC_CR1;

#define RUN_LED     PORTC_ODR.run_led_bits

//
// Подключенная кнопка
//

typedef struct {
    unsigned        : 1;
    unsigned    btn : 1;                // Бит порта с подключенной кнопкой
    unsigned        : 6;
} btn_t;

volatile btn_t     __at(0x5001)   PORTA_IDR;

#define BTN         PORTA_IDR.btn

//--------------------------------------------------------------------------
// Программная задержка без использования таймера
//
// param - длительность задержки в циклах. 1000 циклов примерно соответсвуют
//         5,5 мс
//--------------------------------------------------------------------------

void Delay(uint16_t param) {
    volatile uint16_t counter;
    counter=param;
    while(counter) {
        counter--;
    }
}

//--------------------------------------------------------------------------
// Аппартно-зависимая процедура начальной инициализации микроконтроллера
// Выполняет начальную настройку оборудования
//--------------------------------------------------------------------------

void MCU_Init() {
    PORTC_DDR.run_led_bits=0b111;       // Режим выхода
    PORTC_CR1.run_led_bits=0b111;       // Двухтактный выход
}

//==========================================================================

void main() {
    uint8_t direction;              // Направление бега огня
    
    MCU_Init();
    
    if(BTN)
        direction=1;                // Кнопка не нажата, F1-F2-F3
    else
        direction=0;                // Кнопка нажата, F3-F2-F1
        
//
// Основной программный цикл
//

    while (1) {
        if(direction) {
            if(!RUN_LED)
                RUN_LED=0b001;
            else
                RUN_LED <<= 1;
        } else {
            if(!RUN_LED)
                RUN_LED=0b100;
            else
                RUN_LED >>= 1;
        }
        Delay(18182);               // Задержка примерно 100 мс
    }
}