/**
 * \file smif_cfg.h
 * \author AMasterov
 * \brief Файл содержит настраиваемые параметры, относящиеся непостредственно
 * к "функционалу" SMIF. Здесь номера I/O для подключения контроллера вращения ДПТ
 * BTS7960 и концевиков 2шт. TCRT5000, 1шт. TCST2103.
 * Уканы переменные для хранения состояния работы концевиков с асинхронным доступом.
 * \version 0.1
 * \date 2025-03-04
 * 
 * \copyright Copyright (c) 2025
 * 
 */

// Защита от множественной инициализации
#ifndef SMIF_CONFIG_H
#define SMIF_CONFIG_H

// include library to read and write from flash memory
#include <EEPROM.h>

// Переменная для получения команд от пользователя
//static volatile 
static uint8_t motion_type = 0;

// define the number of bytes you want to access
#define EEPROM_SIZE 25

#endif