/**
 * \file smif_test.c
 * \author AMasterov
 * \brief Файл содержит описание функций (инструкции) управления
 * поворотным механизмом SMIF
 * \version 0.1
 * \date 2025-03-04
 *
 * \copyright Copyright (c) 2025
 *
 */

// Подключение заголовочного файла с объявлением функций и структур
#include "smif_func_test.h"
//подключение библиотеки математичесских формул для расчёта коэфициента скорости при повороте по заданному радиусу
#include <cmath>



// Функция выполнения вращения
void motorMove(const driver_pins_io *bts7960_pins,
               motor_driver_param *bts7960_data)
{
    // Если питание left активирано
    if (bts7960_data->en == DRIVER_PWR_L_POWER)
    {
        // Выбор направления вращения ДПТ
        digitalWrite(bts7960_pins->l_en, HIGH);
        digitalWrite(bts7960_pins->r_en, LOW);
        // Задание ШИМ-сигналов
        analogWrite(bts7960_pins->l_pwm, bts7960_data->l_pwm);
        analogWrite(bts7960_pins->r_pwm, 0);
    }
    // Если питание right активирано
    else if (bts7960_data->en == DRIVER_PWR_R_POWER)
    {
        // Выбор направления вращения ДПТ
        digitalWrite(bts7960_pins->l_en, LOW);
        digitalWrite(bts7960_pins->r_en, HIGH);
        // Задание ШИМ-сигналов
        analogWrite(bts7960_pins->l_pwm, 0);
        analogWrite(bts7960_pins->r_pwm, bts7960_data->r_pwm);
    }
    // Если питание left и right активирано
    else if (bts7960_data->en == DRIVER_PWR_ALL_POWER)
    {
        // Выбор направления вращения ДПТ
        digitalWrite(bts7960_pins->l_en, HIGH);
        digitalWrite(bts7960_pins->r_en, HIGH);
        // Задание ШИМ-сигналов
        analogWrite(bts7960_pins->l_pwm, 0);
        analogWrite(bts7960_pins->r_pwm, 0);
    }
    // Если питание не активирано
    else if (bts7960_data->en == DRIVER_PWR_NO_POWER)
    {
        // Выбор направления вращения ДПТ
        digitalWrite(bts7960_pins->l_en, LOW);
        digitalWrite(bts7960_pins->r_en, LOW);
        // Задание ШИМ-сигналов
        analogWrite(bts7960_pins->l_pwm, 0);
        analogWrite(bts7960_pins->r_pwm, 0);
    }
    // В случае некорректных данных
    else
    {
        // Ассемблерная вставка “Nо OPeration“, холостой такт процессора
        asm("nop"); // Пропуск времени t = 1 / F_CPU, (секунд)
    }
}

// Функция выполнения остановки вращения
void motorStop(const driver_pins_io *bts7960_pins,
               motor_driver_param *bts7960_data)
{
    // Питание обмоток ДПТ не активирано
    bts7960_data->en = DRIVER_PWR_NO_POWER;
    // Выбор направления вращения ДПТ
    digitalWrite(bts7960_pins->l_en, LOW);
    digitalWrite(bts7960_pins->r_en, LOW);
    // Задание ШИМ-сигналов
    bts7960_data->l_pwm = 0;
    bts7960_data->r_pwm = 0;
    analogWrite(bts7960_pins->l_pwm, bts7960_data->l_pwm);
    analogWrite(bts7960_pins->r_pwm, bts7960_data->r_pwm);
}

// Функция инициализции данных и настройки I/O
void driversInit(motorDrivers_param *moveData, motor_driver_func *moveFunc)
{
    // initialize EEPROM with predefined size
    EEPROM.begin(EEPROM_SIZE);
    moveData->driver_pins[0]->l_pwm = EEPROM.read(1);
    moveData->driver_pins[0]->r_pwm = EEPROM.read(2);
    moveData->driver_pins[0]->l_en = EEPROM.read(3);
    moveData->driver_pins[0]->r_en = EEPROM.read(4);
    moveData->driver_pins[0]->l_is = EEPROM.read(5);
    moveData->driver_pins[0]->r_is = EEPROM.read(6);

    moveData->driver_pins[1]->l_pwm = EEPROM.read(7);
    moveData->driver_pins[1]->r_pwm = EEPROM.read(8);
    moveData->driver_pins[1]->l_en = EEPROM.read(9);
    moveData->driver_pins[1]->r_en = EEPROM.read(10);
    moveData->driver_pins[1]->l_is = EEPROM.read(11);
    moveData->driver_pins[1]->r_is = EEPROM.read(12);

    moveData->driver_pins[2]->l_pwm = EEPROM.read(13);
    moveData->driver_pins[2]->r_pwm = EEPROM.read(14);
    moveData->driver_pins[2]->l_en = EEPROM.read(15);
    moveData->driver_pins[2]->r_en = EEPROM.read(16);
    moveData->driver_pins[2]->l_is = EEPROM.read(17);
    moveData->driver_pins[2]->r_is = EEPROM.read(18);

    moveData->driver_pins[3]->l_pwm = EEPROM.read(19);
    moveData->driver_pins[3]->r_pwm = EEPROM.read(20);
    moveData->driver_pins[3]->l_en = EEPROM.read(21);
    moveData->driver_pins[3]->r_en = EEPROM.read(22);
    moveData->driver_pins[3]->l_is = EEPROM.read(23);
    moveData->driver_pins[3]->r_is = EEPROM.read(24);


    // Настройка работы I/O, используемых для контроллера ДПТ
    pinMode(moveData->driver_pins[0]->l_pwm, OUTPUT);
    pinMode(moveData->driver_pins[0]->r_pwm, OUTPUT);
    pinMode(moveData->driver_pins[1]->l_pwm, OUTPUT);
    pinMode(moveData->driver_pins[1]->r_pwm, OUTPUT);
    pinMode(moveData->driver_pins[2]->l_pwm, OUTPUT);
    pinMode(moveData->driver_pins[2]->r_pwm, OUTPUT);
    pinMode(moveData->driver_pins[3]->l_pwm, OUTPUT);
    pinMode(moveData->driver_pins[3]->r_pwm, OUTPUT);
    // Настройка разрешения в битах для сигнала ШИМ
    analogWriteResolution(8);
    // Настройка частоты сигнала ШИМ
    analogWriteFrequency(25000U);

    // Включение buid-in RGB-LED
    neopixelWrite(48, 0x00, 0xFF, 0x00);
}

// Функция задания остановки вращения
void driversStop(motorDrivers_param *moveData)
{
    Serial.println("Stop...");
    // Условие для запуска вращения вала ДПТ
    //if (motion_type == FORWARD)
    //{
        // Структура параметров I/O
        driver_pins_io *pins_0 = moveData->driver_pins[0];
        driver_pins_io *pins_1 = moveData->driver_pins[1];
        driver_pins_io *pins_2 = moveData->driver_pins[2];
        driver_pins_io *pins_3 = moveData->driver_pins[3];
        // Структура параметров контроллера ДПТ
        motor_driver_param *dr_par_0 = moveData->driver[0];
        motor_driver_param *dr_par_1 = moveData->driver[1];
        motor_driver_param *dr_par_2 = moveData->driver[2];
        motor_driver_param *dr_par_3 = moveData->driver[3];
        // Установка коэф. заполнения импулься ШИМ
        dr_par_0->l_pwm = (uint8_t)((dr_par_0->speed) * 0 * 255);
        dr_par_1->l_pwm = (uint8_t)((dr_par_1->speed) * 0 * 255);
        dr_par_2->l_pwm = (uint8_t)((dr_par_2->speed) * 0 * 255);
        dr_par_3->l_pwm = (uint8_t)((dr_par_3->speed) * 0 * 255);
        // Установка сигнала EN для вращения по часовой
        dr_par_0->en = DRIVER_PWR_NO_POWER;
        dr_par_1->en = DRIVER_PWR_NO_POWER;
        dr_par_2->en = DRIVER_PWR_NO_POWER;
        dr_par_3->en = DRIVER_PWR_NO_POWER;

        /* Begin SMIF area */

        // Включение buid-in RGB-LED
        neopixelWrite(48, 0xFF, 0x00, 0xFF);
        // Запуск вращения ДПТ
        moveData->motor_func->move(pins_0, dr_par_0);
        moveData->motor_func->move(pins_1, dr_par_1);
        moveData->motor_func->move(pins_2, dr_par_2);
        moveData->motor_func->move(pins_3, dr_par_3);

        /* while (true)
        {

            // Вывод по UART (в монитор порта, если подключено к ПК)
            Serial.print("Forward moving, stop: ");
            // Вывод по UART (в монитор порта, если подключено к ПК)
            Serial.println(String(STOP));

            // Условие окончания работы ДПТ
            if (motion_type == STOP)
            {
                // Запуска процесса остановки работы ДПТ
                moveData->motor_func->stop(pins_0, dr_par_0);
                moveData->motor_func->stop(pins_0, dr_par_1);
                moveData->motor_func->stop(pins_0, dr_par_2);
                moveData->motor_func->stop(pins_0, dr_par_3);
                break;
            }
        } */

        // Сохранение изменений в структуру параметров
        moveData->driver[0] = dr_par_0;
        moveData->driver[1] = dr_par_1;
        moveData->driver[2] = dr_par_2;
        moveData->driver[3] = dr_par_3;
}

// Функция запуска движения робота вперёд
void driversForward(motorDrivers_param *moveData)
{
    Serial.println("Forward...");
    // Условие для запуска вращения вала ДПТ
    //if (motion_type == FORWARD)
    //{
        // Структура параметров I/O
        driver_pins_io *pins_0 = moveData->driver_pins[0];
        driver_pins_io *pins_1 = moveData->driver_pins[1];
        driver_pins_io *pins_2 = moveData->driver_pins[2];
        driver_pins_io *pins_3 = moveData->driver_pins[3];
        // Структура параметров контроллера ДПТ
        motor_driver_param *dr_par_0 = moveData->driver[0];
        motor_driver_param *dr_par_1 = moveData->driver[1];
        motor_driver_param *dr_par_2 = moveData->driver[2];
        motor_driver_param *dr_par_3 = moveData->driver[3];
        // Установка коэф. заполнения импулься ШИМ
        dr_par_0->l_pwm = (uint8_t)((dr_par_0->speed) * 0.01 * 255);
        dr_par_1->l_pwm = (uint8_t)((dr_par_1->speed) * 0.01 * 255);
        dr_par_2->l_pwm = (uint8_t)((dr_par_2->speed) * 0.01 * 255);
        dr_par_3->l_pwm = (uint8_t)((dr_par_3->speed) * 0.01 * 255);
        // Установка сигнала EN для вращения по часовой
        dr_par_0->en = DRIVER_PWR_L_POWER;
        dr_par_1->en = DRIVER_PWR_L_POWER;
        dr_par_2->en = DRIVER_PWR_L_POWER;
        dr_par_3->en = DRIVER_PWR_L_POWER;

        /* Begin SMIF area */

        // Включение buid-in RGB-LED
        neopixelWrite(48, 0xFF, 0x00, 0xFF);
        // Запуск вращения ДПТ
        moveData->motor_func->move(pins_0, dr_par_0);
        moveData->motor_func->move(pins_1, dr_par_1);
        moveData->motor_func->move(pins_2, dr_par_2);
        moveData->motor_func->move(pins_3, dr_par_3);

        /* while (true)
        {

            // Вывод по UART (в монитор порта, если подключено к ПК)
            Serial.print("Forward moving, stop: ");
            // Вывод по UART (в монитор порта, если подключено к ПК)
            Serial.println(String(STOP));

            // Условие окончания работы ДПТ
            if (motion_type == STOP)
            {
                // Запуска процесса остановки работы ДПТ
                moveData->motor_func->stop(pins_0, dr_par_0);
                moveData->motor_func->stop(pins_0, dr_par_1);
                moveData->motor_func->stop(pins_0, dr_par_2);
                moveData->motor_func->stop(pins_0, dr_par_3);
                break;
            }
        } */

        // Сохранение изменений в структуру параметров
        moveData->driver[0] = dr_par_0;
        moveData->driver[1] = dr_par_1;
        moveData->driver[2] = dr_par_2;
        moveData->driver[3] = dr_par_3;

        /* End SMIF area */
    //}
    /* else
    {
        // Ассемблерная вставка “Nо OPeration“, холостой такт процессора
        asm("nop"); // Пропуск времени t = 1 / F_CPU, (секунд)
    } */
}

// Функция запуска движения робота назад
void driversBackward(motorDrivers_param *moveData)
{
    Serial.println("Backward...");
    /* // Условие для запуска вращения вала ДПТ
    if (motion_type == BACKWARD)
    { */
        // Структура параметров I/O
        driver_pins_io *pins_0 = moveData->driver_pins[0];
        driver_pins_io *pins_1 = moveData->driver_pins[1];
        driver_pins_io *pins_2 = moveData->driver_pins[2];
        driver_pins_io *pins_3 = moveData->driver_pins[3];
        // Структура параметров контроллера ДПТ
        motor_driver_param *dr_par_0 = moveData->driver[0];
        motor_driver_param *dr_par_1 = moveData->driver[1];
        motor_driver_param *dr_par_2 = moveData->driver[2];
        motor_driver_param *dr_par_3 = moveData->driver[3];
        // Установка коэф. заполнения импулься ШИМ
        dr_par_0->r_pwm = (uint8_t)((dr_par_0->speed) * 0.01 * 255);
        dr_par_1->r_pwm = (uint8_t)((dr_par_1->speed) * 0.01 * 255);
        dr_par_2->r_pwm = (uint8_t)((dr_par_2->speed) * 0.01 * 255);
        dr_par_3->r_pwm = (uint8_t)((dr_par_3->speed) * 0.01 * 255);

        // Установка сигнала EN для вращения против часовой
        dr_par_0->en = DRIVER_PWR_R_POWER;
        dr_par_1->en = DRIVER_PWR_R_POWER;
        dr_par_2->en = DRIVER_PWR_R_POWER;
        dr_par_3->en = DRIVER_PWR_R_POWER;

        /* Begin SMIF area */

        // Включение buid-in RGB-LED
        neopixelWrite(48, 0xFF, 0xFF, 0x00);
        // Запуск вращения ДПТ
        moveData->motor_func->move(pins_0, dr_par_0);
        moveData->motor_func->move(pins_1, dr_par_1);
        moveData->motor_func->move(pins_2, dr_par_2);
        moveData->motor_func->move(pins_3, dr_par_3);

        /* while (true)
        {
            // Вывод по UART (в монитор порта, если подключено к ПК)
            Serial.print("Backward moving, stop: ");
            // Вывод по UART (в монитор порта, если подключено к ПК)
            Serial.println(String(STOP));
            // Условие окончания работы ДПТ
            if (motion_type == STOP)
            {
                // Запуска процесса остановки работы ДПТ
                moveData->motor_func->stop(pins_0, dr_par_0);
                moveData->motor_func->stop(pins_0, dr_par_1);
                moveData->motor_func->stop(pins_0, dr_par_2);
                moveData->motor_func->stop(pins_0, dr_par_3);
                break;
            }
        } */

        // Сохранение изменений в структуру параметров
        moveData->driver[0] = dr_par_0;
        moveData->driver[1] = dr_par_1;
        moveData->driver[2] = dr_par_2;
        moveData->driver[3] = dr_par_3;

        /* End SMIF area */
    //}
    /* else
    {
        // Ассемблерная вставка “Nо OPeration“, холостой такт процессора
        asm("nop"); // Пропуск времени t = 1 / F_CPU, (секунд)
    } */
}

//Функция запуска вращения робота вокруг своей оси против часовой
void driversSpinLeft(motorDrivers_param *moveData)
{
    Serial.println("Spin Left...");
    /* // Условие для запуска вращения вала ДПТ
    if (motion_type == SPIN_LEFT)
    { */
        // Структура параметров I/O
        driver_pins_io *pins_0 = moveData->driver_pins[0];
        driver_pins_io *pins_1 = moveData->driver_pins[1];
        driver_pins_io *pins_2 = moveData->driver_pins[2];
        driver_pins_io *pins_3 = moveData->driver_pins[3];
        // Структура параметров контроллера ДПТ
        motor_driver_param *dr_par_0 = moveData->driver[0];
        motor_driver_param *dr_par_1 = moveData->driver[1];
        motor_driver_param *dr_par_2 = moveData->driver[2];
        motor_driver_param *dr_par_3 = moveData->driver[3];
        // Установка коэф. заполнения импулься ШИМ
        dr_par_0->l_pwm = (uint8_t)((dr_par_0->speed) * 0.01 * 255);
        dr_par_1->l_pwm = (uint8_t)((dr_par_1->speed) * 0.01 * 255);
        dr_par_2->r_pwm = (uint8_t)((dr_par_2->speed) * 0.01 * 255);
        dr_par_3->r_pwm = (uint8_t)((dr_par_3->speed) * 0.01 * 255);

        // Установка сигнала EN для вращения 
        dr_par_0->en = DRIVER_PWR_L_POWER;
        dr_par_1->en = DRIVER_PWR_L_POWER;
        dr_par_2->en = DRIVER_PWR_R_POWER;
        dr_par_3->en = DRIVER_PWR_R_POWER;

        /* Begin SMIF area */

        // Включение buid-in RGB-LED
        neopixelWrite(48, 0xFF, 0xFF, 0x00);
        // Запуск вращения ДПТ
        moveData->motor_func->move(pins_0, dr_par_0);
        moveData->motor_func->move(pins_1, dr_par_1);
        moveData->motor_func->move(pins_2, dr_par_2);
        moveData->motor_func->move(pins_3, dr_par_3);

        /* while (true)
        {
            // Вывод по UART (в монитор порта, если подключено к ПК)
            Serial.print("SpinL moving, stop: ");
            // Вывод по UART (в монитор порта, если подключено к ПК)
            Serial.println(String(STOP));
            // Условие окончания работы ДПТ
            if (motion_type == STOP)
            {
                // Запуска процесса остановки работы ДПТ
                moveData->motor_func->stop(pins_0, dr_par_0);
                moveData->motor_func->stop(pins_0, dr_par_1);
                moveData->motor_func->stop(pins_0, dr_par_2);
                moveData->motor_func->stop(pins_0, dr_par_3);
                break;
            }
        } */

        // Сохранение изменений в структуру параметров
        moveData->driver[0] = dr_par_0;
        moveData->driver[1] = dr_par_1;
        moveData->driver[2] = dr_par_2;
        moveData->driver[3] = dr_par_3;

        /* End SMIF area */
    /* }
    else
    {
        // Ассемблерная вставка “Nо OPeration“, холостой такт процессора
        asm("nop"); // Пропуск времени t = 1 / F_CPU, (секунд)
    } */
}

// Функция запуска вращения робота вокруг своей оси по часовой
void driversSpinRight(motorDrivers_param *moveData)
{
    Serial.println("Spin Right...");
    /* // Условие для запуска вращения вала ДПТ
    if (motion_type == SPIN_RIGHT)
    { */
        // Структура параметров I/O
        driver_pins_io *pins_0 = moveData->driver_pins[0];
        driver_pins_io *pins_1 = moveData->driver_pins[1];
        driver_pins_io *pins_2 = moveData->driver_pins[2];
        driver_pins_io *pins_3 = moveData->driver_pins[3];
        // Структура параметров контроллера ДПТ
        motor_driver_param *dr_par_0 = moveData->driver[0];
        motor_driver_param *dr_par_1 = moveData->driver[1];
        motor_driver_param *dr_par_2 = moveData->driver[2];
        motor_driver_param *dr_par_3 = moveData->driver[3];
        // Установка коэф. заполнения импулься ШИМ
        dr_par_0->r_pwm = (uint8_t)((dr_par_0->speed) * 0.01 * 255);
        dr_par_1->r_pwm = (uint8_t)((dr_par_1->speed) * 0.01 * 255);
        dr_par_2->l_pwm = (uint8_t)((dr_par_2->speed) * 0.01 * 255);
        dr_par_3->l_pwm = (uint8_t)((dr_par_3->speed) * 0.01 * 255);

        // Установка сигнала EN для вращения
        dr_par_0->en = DRIVER_PWR_R_POWER;
        dr_par_1->en = DRIVER_PWR_R_POWER;
        dr_par_2->en = DRIVER_PWR_L_POWER;
        dr_par_3->en = DRIVER_PWR_L_POWER;

        /* Begin SMIF area */

        // Включение buid-in RGB-LED
        neopixelWrite(48, 0xFF, 0xFF, 0x00);
        // Запуск вращения ДПТ
        moveData->motor_func->move(pins_0, dr_par_0);
        moveData->motor_func->move(pins_1, dr_par_1);
        moveData->motor_func->move(pins_2, dr_par_2);
        moveData->motor_func->move(pins_3, dr_par_3);

        /* while (true)
        {
            // Вывод по UART (в монитор порта, если подключено к ПК)
            Serial.print("SpinR moving, stop: ");
            // Вывод по UART (в монитор порта, если подключено к ПК)
            Serial.println(String(STOP));
            // Условие окончания работы ДПТ
            if (motion_type == STOP)
            {
                // Запуска процесса остановки работы ДПТ
                moveData->motor_func->stop(pins_0, dr_par_0);
                moveData->motor_func->stop(pins_0, dr_par_1);
                moveData->motor_func->stop(pins_0, dr_par_2);
                moveData->motor_func->stop(pins_0, dr_par_3);
                break;
            }
        } */

        // Сохранение изменений в структуру параметров
        moveData->driver[0] = dr_par_0;
        moveData->driver[1] = dr_par_1;
        moveData->driver[2] = dr_par_2;
        moveData->driver[3] = dr_par_3;

        /* End SMIF area */
    /* }
    else
    {
        // Ассемблерная вставка “Nо OPeration“, холостой такт процессора
        asm("nop"); // Пропуск времени t = 1 / F_CPU, (секунд)
    } */
}

// Функция движения робота по радиусу влево-вперёд
void driversTurnLeftForward(motorDrivers_param *moveData)
{
    Serial.println("Turn Left Forward...");
    /* // Условие для запуска вращения вала ДПТ
    if (motion_type == TURN_LEFT_FORWARD)
    { */
        // Структура параметров I/O
        driver_pins_io *pins_0 = moveData->driver_pins[0];
        driver_pins_io *pins_1 = moveData->driver_pins[1];
        driver_pins_io *pins_2 = moveData->driver_pins[2];
        driver_pins_io *pins_3 = moveData->driver_pins[3];
        // Структура параметров контроллера ДПТ
        motor_driver_param *dr_par_0 = moveData->driver[0];
        motor_driver_param *dr_par_1 = moveData->driver[1];
        motor_driver_param *dr_par_2 = moveData->driver[2];
        motor_driver_param *dr_par_3 = moveData->driver[3];
        
        // Установка коэф. заполнения импулься ШИМ
        // moveData->turn_radius = 40;
        //ПЕРЕНЕСТИ В ОТДЕЛЬНЫЙ ФАЙЛ!!!!!!!!
        uint8_t k; // инициализация коэффициента
        k = (sqrt((40+10)*(40+10)+17*17))/(sqrt(40-10)*(40-10)+17*17);

        dr_par_0->l_pwm = (uint8_t)((dr_par_0->speed)*k * 0.01 * 255);
        dr_par_1->l_pwm = (uint8_t)((dr_par_1->speed)*k * 0.01 * 255);  
        dr_par_2->l_pwm = (uint8_t)((dr_par_2->speed) * 0.01 * 255); 
        dr_par_3->l_pwm = (uint8_t)((dr_par_3->speed) * 0.01 * 255);

        // Установка сигнала EN для вращения по часовой
        dr_par_0->en = DRIVER_PWR_L_POWER;
        dr_par_1->en = DRIVER_PWR_L_POWER;
        dr_par_2->en = DRIVER_PWR_L_POWER;
        dr_par_3->en = DRIVER_PWR_L_POWER;

        /* Begin SMIF area */

        // Включение buid-in RGB-LED
        neopixelWrite(48, 0xFF, 0xFF, 0x00);
        // Запуск вращения ДПТ
        moveData->motor_func->move(pins_0, dr_par_0);
        moveData->motor_func->move(pins_1, dr_par_1);
        moveData->motor_func->move(pins_2, dr_par_2);
        moveData->motor_func->move(pins_3, dr_par_3);

        /* while (true)
        {
            // Вывод по UART (в монитор порта, если подключено к ПК)
            Serial.print("TurnLF moving, stop: ");
            // Вывод по UART (в монитор порта, если подключено к ПК)
            Serial.println(String(STOP));
            // Условие окончания работы ДПТ
            if (motion_type == STOP)
            {
                // Запуска процесса остановки работы ДПТ
                moveData->motor_func->stop(pins_0, dr_par_0);
                moveData->motor_func->stop(pins_0, dr_par_1);
                moveData->motor_func->stop(pins_0, dr_par_2);
                moveData->motor_func->stop(pins_0, dr_par_3);
                break;
            }
        } */

        // Сохранение изменений в структуру параметров
        moveData->driver[0] = dr_par_0;
        moveData->driver[1] = dr_par_1;
        moveData->driver[2] = dr_par_2;
        moveData->driver[3] = dr_par_3;

        /* End SMIF area */
    /* }
    else
    {
        // Ассемблерная вставка “Nо OPeration“, холостой такт процессора
        asm("nop"); // Пропуск времени t = 1 / F_CPU, (секунд)
    } */
}

// Функция движения робота по радиусу вправо-вперёд
void driversTurnRightForward(motorDrivers_param *moveData)
{
    Serial.println("Turn Right Forward...");
    /* // Условие для запуска вращения вала ДПТ
    if (motion_type == TURN_RIGHT_FORWARD)
    { */
        // Структура параметров I/O
        driver_pins_io *pins_0 = moveData->driver_pins[0];
        driver_pins_io *pins_1 = moveData->driver_pins[1];
        driver_pins_io *pins_2 = moveData->driver_pins[2];
        driver_pins_io *pins_3 = moveData->driver_pins[3];
        // Структура параметров контроллера ДПТ
        motor_driver_param *dr_par_0 = moveData->driver[0];
        motor_driver_param *dr_par_1 = moveData->driver[1];
        motor_driver_param *dr_par_2 = moveData->driver[2];
        motor_driver_param *dr_par_3 = moveData->driver[3];
        
        // Установка коэф. заполнения импулься ШИМ
        // moveData->turn_radius = 40;
        //ПЕРЕНЕСТИ В ОТДЕЛЬНЫЙ ФАЙЛ!!!!!!!!
        uint8_t k; // инициализация коэффициента
        k = (sqrt((40+10)*(40+10)+17*17))/(sqrt(40-10)*(40-10)+17*17);

        dr_par_0->l_pwm = (uint8_t)((dr_par_0->speed) * 0.01 * 255);
        dr_par_1->l_pwm = (uint8_t)((dr_par_1->speed) * 0.01 * 255);  
        dr_par_2->l_pwm = (uint8_t)((dr_par_2->speed)*k * 0.01 * 255); 
        dr_par_3->l_pwm = (uint8_t)((dr_par_3->speed)*k * 0.01 * 255);

        // Установка сигнала EN для вращения по часовой
        dr_par_0->en = DRIVER_PWR_L_POWER;
        dr_par_1->en = DRIVER_PWR_L_POWER;
        dr_par_2->en = DRIVER_PWR_L_POWER;
        dr_par_3->en = DRIVER_PWR_L_POWER;

        /* Begin SMIF area */

        // Включение buid-in RGB-LED
        neopixelWrite(48, 0xFF, 0xFF, 0x00);
        // Запуск вращения ДПТ
        moveData->motor_func->move(pins_0, dr_par_0);
        moveData->motor_func->move(pins_1, dr_par_1);
        moveData->motor_func->move(pins_2, dr_par_2);
        moveData->motor_func->move(pins_3, dr_par_3);

       /*  while (true)
        {
            // Вывод по UART (в монитор порта, если подключено к ПК)
            Serial.print("TurnRF moving, stop: ");
            // Вывод по UART (в монитор порта, если подключено к ПК)
            Serial.println(String(STOP));
            // Условие окончания работы ДПТ
            if (motion_type == STOP)
            {
                // Запуска процесса остановки работы ДПТ
                moveData->motor_func->stop(pins_0, dr_par_0);
                moveData->motor_func->stop(pins_0, dr_par_1);
                moveData->motor_func->stop(pins_0, dr_par_2);
                moveData->motor_func->stop(pins_0, dr_par_3);
                break;
            }
        }
 */
        // Сохранение изменений в структуру параметров
        moveData->driver[0] = dr_par_0;
        moveData->driver[1] = dr_par_1;
        moveData->driver[2] = dr_par_2;
        moveData->driver[3] = dr_par_3;

        /* End SMIF area */
    /* }
    else
    {
        // Ассемблерная вставка “Nо OPeration“, холостой такт процессора
        asm("nop"); // Пропуск времени t = 1 / F_CPU, (секунд)
    } */
}

// Функция движения робота по радиусу влево-назад
void driversTurnLeftBacward(motorDrivers_param *moveData)
{
    Serial.println("Turn Left Backward...");
    /* // Условие для запуска вращения вала ДПТ
    if (motion_type == TURN_LEFT_BACKWARD)
    { */
        // Структура параметров I/O
        driver_pins_io *pins_0 = moveData->driver_pins[0];
        driver_pins_io *pins_1 = moveData->driver_pins[1];
        driver_pins_io *pins_2 = moveData->driver_pins[2];
        driver_pins_io *pins_3 = moveData->driver_pins[3];
        // Структура параметров контроллера ДПТ
        motor_driver_param *dr_par_0 = moveData->driver[0];
        motor_driver_param *dr_par_1 = moveData->driver[1];
        motor_driver_param *dr_par_2 = moveData->driver[2];
        motor_driver_param *dr_par_3 = moveData->driver[3];
        
        // Установка коэф. заполнения импулься ШИМ
        // moveData->turn_radius = 40;
        //ПЕРЕНЕСТИ В ОТДЕЛЬНЫЙ ФАЙЛ!!!!!!!!
        uint8_t k; // инициализация коэффициента
        k = (sqrt((40+10)*(40+10)+17*17))/(sqrt(40-10)*(40-10)+17*17);

        dr_par_0->r_pwm = (uint8_t)((dr_par_0->speed)*k * 0.01 * 255);
        dr_par_1->r_pwm = (uint8_t)((dr_par_1->speed)*k * 0.01 * 255);  
        dr_par_2->r_pwm = (uint8_t)((dr_par_2->speed) * 0.01 * 255); 
        dr_par_3->r_pwm = (uint8_t)((dr_par_3->speed) * 0.01 * 255);

        // Установка сигнала EN для вращения против часовой
        dr_par_0->en = DRIVER_PWR_R_POWER;
        dr_par_1->en = DRIVER_PWR_R_POWER;
        dr_par_2->en = DRIVER_PWR_R_POWER;
        dr_par_3->en = DRIVER_PWR_R_POWER;

        /* Begin SMIF area */

        // Включение buid-in RGB-LED
        neopixelWrite(48, 0xFF, 0xFF, 0x00);
        // Запуск вращения ДПТ
        moveData->motor_func->move(pins_0, dr_par_0);
        moveData->motor_func->move(pins_1, dr_par_1);
        moveData->motor_func->move(pins_2, dr_par_2);
        moveData->motor_func->move(pins_3, dr_par_3);

        /* while (true)
        {
            // Вывод по UART (в монитор порта, если подключено к ПК)
            Serial.print("TurnLB moving, stop: ");
            // Вывод по UART (в монитор порта, если подключено к ПК)
            Serial.println(String(STOP));
            // Условие окончания работы ДПТ
            if (motion_type == STOP)
            {
                // Запуска процесса остановки работы ДПТ
                moveData->motor_func->stop(pins_0, dr_par_0);
                moveData->motor_func->stop(pins_0, dr_par_1);
                moveData->motor_func->stop(pins_0, dr_par_2);
                moveData->motor_func->stop(pins_0, dr_par_3);
                break;
            }
        } */

        // Сохранение изменений в структуру параметров
        moveData->driver[0] = dr_par_0;
        moveData->driver[1] = dr_par_1;
        moveData->driver[2] = dr_par_2;
        moveData->driver[3] = dr_par_3;

        /* End SMIF area */
    /* }
    else
    {
        // Ассемблерная вставка “Nо OPeration“, холостой такт процессора
        asm("nop"); // Пропуск времени t = 1 / F_CPU, (секунд)
    } */
}

// Функция движения робота по радиусу вправо-назад
void driversTurnRightBackward(motorDrivers_param *moveData)
{
    Serial.println("Turn Right Backward...");
    /* // Условие для запуска вращения вала ДПТ
    if (motion_type == TURN_RIGHT_BACKWARD)
    { */
        // Структура параметров I/O
        driver_pins_io *pins_0 = moveData->driver_pins[0];
        driver_pins_io *pins_1 = moveData->driver_pins[1];
        driver_pins_io *pins_2 = moveData->driver_pins[2];
        driver_pins_io *pins_3 = moveData->driver_pins[3];
        // Структура параметров контроллера ДПТ
        motor_driver_param *dr_par_0 = moveData->driver[0];
        motor_driver_param *dr_par_1 = moveData->driver[1];
        motor_driver_param *dr_par_2 = moveData->driver[2];
        motor_driver_param *dr_par_3 = moveData->driver[3];
        
        // Установка коэф. заполнения импулься ШИМ
        //moveData->turn_radius = 40;
        //ПЕРЕНЕСТИ В ОТДЕЛЬНЫЙ ФАЙЛ!!!!!!!!
        uint8_t k; // инициализация коэффициента
        k = (sqrt((40+10)*(40+10)+17*17))/(sqrt(40-10)*(40-10)+17*17);

        dr_par_0->r_pwm = (uint8_t)((dr_par_0->speed) * 0.01 * 255);
        dr_par_1->r_pwm = (uint8_t)((dr_par_1->speed) * 0.01 * 255);  
        dr_par_2->r_pwm = (uint8_t)((dr_par_2->speed)*k * 0.01 * 255); 
        dr_par_3->r_pwm = (uint8_t)((dr_par_3->speed)*k * 0.01 * 255);

        // Установка сигнала EN для вращения против часовой
        dr_par_0->en = DRIVER_PWR_R_POWER;
        dr_par_1->en = DRIVER_PWR_R_POWER;
        dr_par_2->en = DRIVER_PWR_R_POWER;
        dr_par_3->en = DRIVER_PWR_R_POWER;

        /* Begin SMIF area */

        // Включение buid-in RGB-LED
        neopixelWrite(48, 0xFF, 0xFF, 0x00);
        // Запуск вращения ДПТ
        moveData->motor_func->move(pins_0, dr_par_0);
        moveData->motor_func->move(pins_1, dr_par_1);
        moveData->motor_func->move(pins_2, dr_par_2);
        moveData->motor_func->move(pins_3, dr_par_3);

        /* while (true)
        {
            // Вывод по UART (в монитор порта, если подключено к ПК)
            Serial.print("TurnRB moving, stop: ");
            // Вывод по UART (в монитор порта, если подключено к ПК)
            Serial.println(String(STOP));
            // Условие окончания работы ДПТ
            if (motion_type == STOP)
            {
                // Запуска процесса остановки работы ДПТ
                moveData->motor_func->stop(pins_0, dr_par_0);
                moveData->motor_func->stop(pins_0, dr_par_1);
                moveData->motor_func->stop(pins_0, dr_par_2);
                moveData->motor_func->stop(pins_0, dr_par_3);
                break;
            }
        } */

        // Сохранение изменений в структуру параметров
        moveData->driver[0] = dr_par_0;
        moveData->driver[1] = dr_par_1;
        moveData->driver[2] = dr_par_2;
        moveData->driver[3] = dr_par_3;

        /* End SMIF area */
    /* }
    else
    {
        // Ассемблерная вставка “Nо OPeration“, холостой такт процессора
        asm("nop"); // Пропуск времени t = 1 / F_CPU, (секунд)
    } */
}

// Функция движения робота в омни-режиме вбок влево
void driversOmniLeft(motorDrivers_param *moveData)
{
    Serial.println("Omni Left...");
    /* // Условие для запуска вращения вала ДПТ
    if (motion_type == OMNI_LEFT)
    { */
        // Структура параметров I/O
        driver_pins_io *pins_0 = moveData->driver_pins[0];
        driver_pins_io *pins_1 = moveData->driver_pins[1];
        driver_pins_io *pins_2 = moveData->driver_pins[2];
        driver_pins_io *pins_3 = moveData->driver_pins[3];
        // Структура параметров контроллера ДПТ
        motor_driver_param *dr_par_0 = moveData->driver[0];
        motor_driver_param *dr_par_1 = moveData->driver[1];
        motor_driver_param *dr_par_2 = moveData->driver[2];
        motor_driver_param *dr_par_3 = moveData->driver[3];
        
        // Установка коэф. заполнения импулься ШИМ
        dr_par_0->l_pwm = (uint8_t)((dr_par_0->speed) * 0.01 * 255);
        dr_par_1->r_pwm = (uint8_t)((dr_par_1->speed) * 0.01 * 255);  
        dr_par_2->l_pwm = (uint8_t)((dr_par_2->speed) * 0.01 * 255); 
        dr_par_3->r_pwm = (uint8_t)((dr_par_3->speed) * 0.01 * 255);

        // Установка сигнала EN для вращения против часовой
        dr_par_0->en = DRIVER_PWR_L_POWER;
        dr_par_1->en = DRIVER_PWR_R_POWER;
        dr_par_2->en = DRIVER_PWR_L_POWER;
        dr_par_3->en = DRIVER_PWR_R_POWER;

        /* Begin SMIF area */

        // Включение buid-in RGB-LED
        neopixelWrite(48, 0xFF, 0xFF, 0x00);
        // Запуск вращения ДПТ
        moveData->motor_func->move(pins_0, dr_par_0);
        moveData->motor_func->move(pins_1, dr_par_1);
        moveData->motor_func->move(pins_2, dr_par_2);
        moveData->motor_func->move(pins_3, dr_par_3);

        /* while (true)
        {
            // Вывод по UART (в монитор порта, если подключено к ПК)
            Serial.print("Omni Left moving, stop: ");
            // Вывод по UART (в монитор порта, если подключено к ПК)
            Serial.println(String(STOP));
            // Условие окончания работы ДПТ
            if (motion_type == STOP)
            {
                // Запуска процесса остановки работы ДПТ
                moveData->motor_func->stop(pins_0, dr_par_0);
                moveData->motor_func->stop(pins_0, dr_par_1);
                moveData->motor_func->stop(pins_0, dr_par_2);
                moveData->motor_func->stop(pins_0, dr_par_3);
                break;
            }
        }
 */
        // Сохранение изменений в структуру параметров
        moveData->driver[0] = dr_par_0;
        moveData->driver[1] = dr_par_1;
        moveData->driver[2] = dr_par_2;
        moveData->driver[3] = dr_par_3;

        /* End SMIF area */
    /* }
    else
    {
        // Ассемблерная вставка “Nо OPeration“, холостой такт процессора
        asm("nop"); // Пропуск времени t = 1 / F_CPU, (секунд)
    } */
}

// Функция движения робота в омни-режиме вбок вправо
void driversOmniRight(motorDrivers_param *moveData)
{
    Serial.println("Omni Right...");
    /* // Условие для запуска вращения вала ДПТ
    if (motion_type == OMNI_RIGHT)
    { */
        // Структура параметров I/O
        driver_pins_io *pins_0 = moveData->driver_pins[0];
        driver_pins_io *pins_1 = moveData->driver_pins[1];
        driver_pins_io *pins_2 = moveData->driver_pins[2];
        driver_pins_io *pins_3 = moveData->driver_pins[3];
        // Структура параметров контроллера ДПТ
        motor_driver_param *dr_par_0 = moveData->driver[0];
        motor_driver_param *dr_par_1 = moveData->driver[1];
        motor_driver_param *dr_par_2 = moveData->driver[2];
        motor_driver_param *dr_par_3 = moveData->driver[3];
        
        // Установка коэф. заполнения импулься ШИМ
        dr_par_0->r_pwm = (uint8_t)((dr_par_0->speed) * 0.01 * 255);
        dr_par_1->l_pwm = (uint8_t)((dr_par_1->speed) * 0.01 * 255);  
        dr_par_2->r_pwm = (uint8_t)((dr_par_2->speed) * 0.01 * 255); 
        dr_par_3->l_pwm = (uint8_t)((dr_par_3->speed) * 0.01 * 255);

        // Установка сигнала EN для вращения против часовой
        dr_par_0->en = DRIVER_PWR_R_POWER;
        dr_par_1->en = DRIVER_PWR_L_POWER;
        dr_par_2->en = DRIVER_PWR_R_POWER;
        dr_par_3->en = DRIVER_PWR_L_POWER;

        /* Begin SMIF area */

        // Включение buid-in RGB-LED
        neopixelWrite(48, 0xFF, 0xFF, 0x00);
        // Запуск вращения ДПТ
        moveData->motor_func->move(pins_0, dr_par_0);
        moveData->motor_func->move(pins_1, dr_par_1);
        moveData->motor_func->move(pins_2, dr_par_2);
        moveData->motor_func->move(pins_3, dr_par_3);

   /*      while (true)
        {
            // Вывод по UART (в монитор порта, если подключено к ПК)
            Serial.print("Omni Right moving, stop: ");
            // Вывод по UART (в монитор порта, если подключено к ПК)
            Serial.println(String(STOP));
            // Условие окончания работы ДПТ
            if (motion_type == STOP)
            {
                // Запуска процесса остановки работы ДПТ
                moveData->motor_func->stop(pins_0, dr_par_0);
                moveData->motor_func->stop(pins_0, dr_par_1);
                moveData->motor_func->stop(pins_0, dr_par_2);
                moveData->motor_func->stop(pins_0, dr_par_3);
                break;
            }
        }
 */
        // Сохранение изменений в структуру параметров
        moveData->driver[0] = dr_par_0;
        moveData->driver[1] = dr_par_1;
        moveData->driver[2] = dr_par_2;
        moveData->driver[3] = dr_par_3;

        /* End SMIF area */
    /* }
    else
    {
        // Ассемблерная вставка “Nо OPeration“, холостой такт процессора
        asm("nop"); // Пропуск времени t = 1 / F_CPU, (секунд)
    } */
}