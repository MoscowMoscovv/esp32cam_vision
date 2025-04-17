<h1>Библиотека для развертки веб-сервера с функционалом пульта управления на esp32</h1>

<ul>
    <title>
    оглавление
    </title>
    <li>
        <p>
            <a  href = "#s_1" style="color: rgb(35,162,145">
            Способ применения
            </a>
        </p>
    </li>
    <li>
        <p>
            <a href="#s_2" style="color: rgb(35,162,145)">
                Высокоуровневые функции
            </a>
        </p>
    </li>
    <li>
        <p>
            <a href="#s_3" style="color: rgb(35,162,145">
                Функции-обработчики uri
            </a>
        </p>
    </li>
    <li>
        <p>
            <a href="#s_4" style="color: rgb(35,162,145">
                Организации файлов библиотеки
            </a>
        </p>
    </li>
</ul>

<hr>
<h2 id = "s_1">
    <center> Способ применения <center>
</h2>
Для использования неоходимо использовать один из следующих редакторов кода <code>Visual Studio</code>, <code>Visual Studio Code</code>, <code>CLion</code>. В редакторе кода необходимо установить расширение PlatformIO и открыть папку с "проекта". При установленном расширении прочитается конфигурацонный файл сборки проекта platformio.ini и откроется доступ к инструментам, которые позволяют выполнять сборку, загрузку и монитор порта.
<h2>

<h2 id="s_2" style="color: rgb(35, 162, 145)">
    <center>Высокоуровневые функции</center>

</h2>

Функция подключения к вай-фаю

``` C
/**
 * \brief Функция подключения к доступной точке WiFi
 * \param [in] ssid имя сети
 * \param [in] bts7960_data пароль от сети
 * \param [in] nameDNS DNS
 * \return None
 */
void start_WIFI_in_client_mode(const char *ssid, const char *password, const char* nameDNS);
```

Функция включения ESP в режим WiFi хаба

``` C
/**
 * \brief Функция включения Esp в режим WiFi хаба
 * \param [in] ssid имя сети
 * \param [in] password пароль от сети
 * \param [in] local_ip ip платы в сети
 * \param [in] gateway шлюз
 * \param [in] subnet подсеть
 * \return None
 **/
 void start_WIFI_in_station_mode(const char *ssid,
    const char *password,
    IPAddress local_ip,
    IPAddress gateway,
    IPAddress subnet,
);
```

Функция запуска сервера
``` C
/**
 * \brief Функция запуска веб-сервера
 * \param [in] interface_handler функция обработки входящих с веб-интерфейса значений
 *      \param 0speed расстояния левого джойстика от центра
 *      \param 0angle угол отклонения левого джойстика от центра
 *      \param 1speed расстояния правого джойстика от центра
 *      \param 1angle угол отклонения правого джойстика от центра
 *      \return None
 * \param disconect_handler обработчик отключения вай-фая
 * \param disconect_handler обработчик переподключения вай-фая
 * \return WebServer
 **/
WebServer& start_server(std::function<void(int, int, int, int)> interface_handler, std::function<void()> disconect_handler, std::function<void()> reconect_handler);
```

Функция обработки входящих сигналов с клиентской стороны
``` C
/**
* \brief Функция вызывается в цикле loop(). Происходит вызов встроенного в библиотеку WebServer.h метода для обработки вхождящих запросов и проверка на то, когда последний раз приходили запросы на получение температуры. Если таковые были слишком давно - вызывает функцию-обработчик, переданную в start_server(). При восстановлении соединения вызывает соответственную переданную функцию.
* \return None
**/
void handleClient();
```


<h2 id="s_3" style="color: rgb(35, 162, 145)">
    <center>Функции-обработчики uri</center>
</h2>

Функция обработки главной страницы

```C
/**
 * \brief Обработка главной страницы: отправляет html-разметку главной страницы 
 * \return None
 **/
void handle_root();
```
Функция обработки /temp_sens
```C
/** 
 * \brief  обработчик uri /temp_sens, отправляет температуру процессора каждый раз, когда приходит запрос. Помимо этого при каждом получении запроса записывает время его получения в глобальную переменную
 *  \return None 
 **/
void temp_sens();
```

Функция обработки /joystic_pos
```C
/** 
 * \brief обработчик входящей на uri /joystic_pos информация, вызов передаваемой в StartServer функции
 *  \return None
*/
void interface_feedback_handler();
```


<h2 id="s_4" style="color: rgb(35, 162, 145)">
    <center>Организации файлов библиотеки</center>
</h2>


Ниже представлена смеха взаимодействия файлов библиотеки при сборке программы.
<table border="0">
<col width="2000" height="400">
    <tr>
        <th align="center">

```mermaid
sequenceDiagram
    include
        page.h ->> mainESPEWebServer.cpp : Импорт главной страницы
    src
        mainESPEWebServer.h ->> mainEspWebServer.cpp : Импорт в главный файл программы

```

</th>
    </tr>
    <tr>
        <th>
            <b><p style="text-align: center; font-size: 10pt">
                Рис. 1. Смеха взаимодействия файлов библиотеки
            </p></b>
        </th>
    </tr>
</table>