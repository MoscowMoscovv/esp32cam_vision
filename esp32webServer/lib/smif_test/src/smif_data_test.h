/**
 * \file smif_test.h
 * \author AMasterov
 * \brief Файл содержит структуры данных и функций для взаимодействия с
 * поворотным механизмом платформы SMIF
 * \version 0.1
 * \date 2025-03-04
 * 
 * \copyright Copyright (c) 2025
 * 
 */

// Защита от множественной инициализации
#ifndef SMIF_DATA_TEST_H
#define SMIF_DATA_TEST_H

// Подключение фенкций и данных фреймворка Arduino
#include <Arduino.h>
// Функиции интерфейса I2C для работы с build-in RGB-LED
#include <Wire.h>
// Макросы и переменные для контроля работы SMIF 
#include "smif_cfg.h"

/**
 * \brief Перечисление методов подачи питания на ДПТ 
 */
typedef enum
{
    DRIVER_PWR_NO_POWER, // Нет питания двигателя
    DRIVER_PWR_L_POWER,  // Питание left
    DRIVER_PWR_R_POWER,  // Питание right
    DRIVER_PWR_ALL_POWER // Питание right и left
} DRIVER_PWR_ST;


/**
 * @brief перечисление направлений движения устройства в результате полученной команды от пользователя
 *
 */
typedef enum
{
    STOP,                // нет движения
    FORWARD,             // движение вперёд
    BACKWARD,            // движение назад
    TURN_LEFT_FORWARD,   // поворот налево-вперёд по заданному радиусу
    TURN_RIGHT_FORWARD,  // поворот направо-вперёд по заданному радиусу
    TURN_LEFT_BACKWARD,  // поворот налево--назад по заданному радиусу
    TURN_RIGHT_BACKWARD, // поворот направо-назад по заданному радиусу
    SPIN_LEFT,           // поворот вокруг соей оси против часовой
    SPIN_RIGHT,          // поворот вокруг своей оси по часовой
    OMNI_LEFT,           // движение боком влево
    OMNI_RIGHT           // движение боеом вправо
} MOVING_DERECTION;

/**
 * \brief Структура с номерами I/O МК для драйвера двигателя
 * \param l_pwm Номер PWM I/O МК для right 
 * \param r_pwm Номер PWM I/O МК для left
 * \param l_en Номер Digital I/O МК для right
 * \param r_en Номер Digital I/O МК для left
 * \param l_is Номер Analog I/O МК для right
 * \param r_is Номер Analog I/O МК для left
 */
typedef struct
{
    uint8_t l_pwm;  // Пин МК для L_PWM
    uint8_t r_pwm;  // Пин МК для R_PWM
    uint8_t l_en;   // Пин МК для L_EN
    uint8_t r_en;   // Пин МК для R_EN
    uint8_t l_is;   // Пин МК для L_IS
    uint8_t r_is;   // Пин МК для R_IS
} driver_pins_io;

/**
 * \brief Структура с параметрами контроллера вращения ДПТ
 * \param l_pwm ШИМ для скорости вращения
 * \param r_pwm ШИМ для скорости вращения
 * \param l_is Ток при вращении left
 * \param r_is Ток при вращении right
 * \param en Доступ питания к обмоткам двигателя
 */
typedef struct
{
    uint8_t l_pwm;    // ШИМ для скорости вращения left
    uint8_t r_pwm;    // ШИМ для скорости вращения right
    float l_is;       // Ток при вращении left
    float r_is;       // Ток при вращении right
    float speed;      // Скорость
    DRIVER_PWR_ST en; // Доступ питания к обмоткам двигателя
} motor_driver_param;

/**
 * \brief Структура функций управления контроллером вращения ДПТ
 * \param move(...) Функция выполнения вращения
 * \param stop(...) Функция выполнения остановки вращения
 */
typedef struct
{
    // Функция выполнения вращения
    void (*move)(const driver_pins_io*, motor_driver_param*);
    // Функция выполнения остановки вращения
    void (*stop)(const driver_pins_io*, motor_driver_param*);
} motor_driver_func;

/**
 * \brief Структура параметров управления двигателем
 * \param driver_pins Структура номеров I/O МК для драйвера двигателя
 * \param driver Структура параметров управления драйвером двигателя
 * \param motor_func Структура функций управления драйвером двигателя
 */
typedef struct
{
    driver_pins_io* driver_pins[4];
    motor_driver_param* driver[4];
    motor_driver_func* motor_func;
    uint8_t turn_radius;
} motorDrivers_param;

/**
 * \brief Структура функций управления двигателем в составе поворотного 
 * механизма
 * \param init(...) Функция инициализции данных и настройки I/O
 * \param stop(...) Функция задания остановки вращения
 * \param forward(...) Функция запуска движения робота вперёд
 * \param backward(...) Функция запуска движения робота назад
 * \param spin_left(...) Функция запуска вращения робота вокруг своей оси против часовой
 * \param spin_right(...) Функция запуска вращения робота вокруг своей оси по часовой
 * \param turn_left_forward(...) Функция движения робота по радиусу влево-вперёд
 * \param turn_right_forward(...) Функция движения робота по радиусу вправо-вперёд
 * \param turn_left_backward(...) Функция движения робота по радиусу влево-назад
 * \param turn_right_backward(...) Функция движения робота по радиусу вправо-назад
 * \param omni_left(...) Функция движения робота в омни-режиме вбок влево
 * \param omni_right(...) Функция движения робота в омни-режиме вбок вправо
 */
typedef struct
{
    // Функция инициализции данных и настройки I/O
    void (*init)(motorDrivers_param*, motor_driver_func*);
    // Функция остановки вращения
    void (*stop)(motorDrivers_param*);
    // Функция запуска движения робота вперёд
    void (*forward)(motorDrivers_param*);
    // Функция запуска движения робота назад
    void (*backward)(motorDrivers_param*);
    // Функция запуска вращения робота вокруг своей оси против часовой
    void (*spin_left)(motorDrivers_param*);
    // Функция запуска вращения робота вокруг своей оси по часовой
    void (*spin_right)(motorDrivers_param*);
    // Функция движения робота по радиусу влево-вперёд
    void (*turn_left_forward)(motorDrivers_param*);
    // Функция движения робота по радиусу вправо-вперёд
    void (*turn_right_forward)(motorDrivers_param*);
    // Функция движения робота по радиусу влево-назад
    void (*turn_left_backward)(motorDrivers_param*);
    // Функция движения робота по радиусу вправо-назад
    void (*turn_right_backward)(motorDrivers_param*);
    // Функция движения робота в омни-режиме вбок влево
    void (*omni_left)(motorDrivers_param*);
    // Функция движения робота в омни-режиме вбок вправо
    void (*omni_right)(motorDrivers_param*);
} motorDrivers_func;

#endif