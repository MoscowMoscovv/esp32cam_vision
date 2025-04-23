/**
 * \file smif_func_test.h
 * \author AMasterov
 * \brief Файл содержит объявление функций управления 
 * поворотным механизмом SMIF и инициализацию структур данных и функций
 * \version 0.1
 * \date 2025-03-05
 *
 * \copyright Copyright (c) 2025
 *
 */

// Защита от множественной инициализации
#ifndef SMIF_FUNC_TEST_H
#define SMIF_FUNC_TEST_H

/* Подключение заголовочного файла со структуры данных и функций для 
взаимодействия с поворотным механизмом платформы SMIF */
#include "smif_data_test.h"

/**
 * \brief Функция запуска вращения двигателя
 * \param [in] bts7960_pins Структура данных о подключении к МК
 * \param [out] bts7960_data Структура данных о управлении драйвером двигателя
 * \return None
 */
void motorMove(const driver_pins_io *bts7960_pins,
               motor_driver_param *bts7960_data);

/**
 * \brief Функция остановки вращения двигателя
 * \param [in] bts7960_pins Структура данных о подключении к МК
 * \param [out] bts7960_data Структура данных о управлении драйвером двигателя
 * \return None
 */
void motorStop( const driver_pins_io *bts7960_pins,
                motor_driver_param *bts7960_data);

/**
 * \brief Выполнение инициализции данных и настройки I/O
 * \param moveData Структура параметров управления двигателем
 * \param moveFunc Структура функций управления двигателем в составе поворотного 
 * механизма
 * \return None
 */
void driversInit(   motorDrivers_param *moveData,
                    motor_driver_func *moveFunc);

/**
 * \brief Выполнение задания остановки вращения
 * \param moveData Структура параметров управления двигателем
 * \return None
 */
void driversStop(motorDrivers_param *moveData);

/**
 * \brief Выполнение задания запуска вращения для перемещения робота вперёд
 * \param moveData Структура параметров управления двигателем
 * \return None
 */
void driversForward(motorDrivers_param *moveData);

/**
 * \brief Выполнение задания запуска вращения для перемещения робота назад
 * \param moveData Структура параметров управления двигателем
 * \return None
 */
void driversBackward(motorDrivers_param *moveData);

/**
 * \brief Выполнение задания запуска вращения для поворота робота вокруг своей оси влево
 * \param moveData Структура параметров управления двигателем
 * \return None
 */
void driversSpinLeft(motorDrivers_param *moveData);

/**
 * \brief Выполнение задания запуска вращения для поворота робота вокруг своей оси вправо
 * \param moveData Структура параметров управления двигателем
 * \return None
 */
void driversSpinRight(motorDrivers_param *moveData);

/**
 * \brief Выполнение задания запуска вращения для поворота робота по заданному радиусу влево-вперёд
 * \param moveData Структура параметров управления двигателем
 * \return None
 */
void driversTurnLeftForward(motorDrivers_param *moveData);

/**
 * \brief Выполнение задания запуска вращения для поворота робота по заданному радиусу вправо-вперёд
 * \param moveData Структура параметров управления двигателем
 * \return None
 */
void driversTurnRightForward(motorDrivers_param *moveData);

/**
 * \brief Выполнение задания запуска вращения для поворота робота по заданному радиусу влево-назад
 * \param moveData Структура параметров управления двигателем
 * \return None
 */
void driversTurnLeftBacward(motorDrivers_param *moveData);

/**
 * \brief Выполнение задания запуска вращения для поворота робота по заданному радиусу вправо-назад
 * \param moveData Структура параметров управления двигателем
 * \return None
 */
void driversTurnRightBackward(motorDrivers_param *moveData);

/**
 * \brief Выполнение задания запуска вращения для движения робота в омни-режиме боком влево
 * \param moveData Структура параметров управления двигателем
 * \return None
 */
void driversOmniLeft(motorDrivers_param *moveData);

/**
 * \brief Выполнение задания запуска вращения для движения робота в омни-режиме боком вправо
 * \param moveData Структура параметров управления двигателем
 * \return None
 */
void driversOmniRight(motorDrivers_param *moveData);

// Структура с номерами I/O МК для драйвера двигателя
static  driver_pins_io bts7960_pins[4] = {
    {
        /* .l_pwm =*/ 0,
        /* .r_pwm =*/ 0,
        /* .l_en = */0,
        /* .r_en = */0,
        /* .l_is = */0,
        /* .r_is = */0,
    },
    {
        /* .l_pwm =*/ 0,
        /* .r_pwm =*/ 0,
       /* .l_en = */ 0,
       /* .r_en = */ 0,
       /* .l_is = */ 0,
       /* .r_is = */ 0,
    },
    {
        /* .l_pwm =*/0,
        /* .r_pwm =*/0,
        /* .l_en = */0,
        /* .r_en = */0,
        /* .l_is = */0,
        /* .r_is = */0,
    },
    {
        /* .l_pwm =*/0,
        /* .r_pwm =*/0,
        /* .l_en = */0,
        /* .r_en = */0,
        /* .l_is = */0,
        /* .r_is = */0,
    }
    
};


// Структура с параметрами контроллера вращения ДПТ
static motor_driver_param bts7960_dat[4] = {
    {
        .l_pwm = 0,
        .r_pwm = 0,
        .l_is = 0,
        .r_is = 0,
        .speed = 70,
        .en = DRIVER_PWR_NO_POWER,
    },
    {
        .l_pwm = 0,
        .r_pwm = 0,
        .l_is = 0,
        .r_is = 0,
        .speed = 70,
        .en = DRIVER_PWR_NO_POWER,
    },
    {
        .l_pwm = 0,
        .r_pwm = 0,
        .l_is = 0,
        .r_is = 0,
        .speed = 70,
        .en = DRIVER_PWR_NO_POWER,
    },
    {
        .l_pwm = 0,
        .r_pwm = 0,
        .l_is = 0,
        .r_is = 0,
        .speed = 70,
        .en = DRIVER_PWR_NO_POWER,
    }
};


// Структура функций управления контроллером вращения ДПТ
static motor_driver_func bts7960_func = {
    .move = motorMove,
    .stop = motorStop,
};

// Структура параметров управления двигателем
static motorDrivers_param mDriver_dat = {
    /*.driver_pins =*/ { &bts7960_pins[0],&bts7960_pins[1],&bts7960_pins[2],&bts7960_pins[3]},
    /*.driver      =*/ {&bts7960_dat[0],&bts7960_dat[1],&bts7960_dat[2],&bts7960_dat[3]},
    /*.motor_func  =*/ &bts7960_func
};

// Структура функций управления двигателем в составе поворотного механизма
static motorDrivers_func mDriver_func = {
    .init = driversInit,
    .stop = driversStop,
    .forward = driversForward,
    .backward = driversBackward,
    .spin_left = driversSpinLeft,
    .spin_right = driversSpinRight,
    .turn_left_forward = driversTurnLeftForward,
    .turn_right_forward = driversTurnRightForward,
    .turn_left_backward = driversTurnLeftBacward,
    .turn_right_backward = driversTurnRightBackward,
    .omni_left = driversOmniLeft,
    .omni_right = driversOmniRight,
};

#endif