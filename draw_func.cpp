/*********************************************************************************************************************
* Данный модуль содержит описание функций, предназначенных для терминальной графики и отрисовки шаблона таблицы
* (рисование линий, изменение цветов, габаритных параметров терминала, сетка и заголовок таблицы)
*********************************************************************************************************************/

/* Подключение модулей */
#include "draw_func.h"
#include "definitions.h"
#include <string>
#include <iostream>
#include <sys/ioctl.h>


void GetWinSize(int &width, int &height){
    /*****************************************************************************************************************
    * Цель: Получить данные о текущих размерах окна терминала и присвоить их соответствующим переменным по ссылкам.
    * Исходные данные:
    *   width  - ссылка на переменную, предназначенную для хранения исходной ширины окна терминала
    *   height - ссылка на переменную, предназначенную для хранения исходной высоты окна терминала
    * Результат: Данные о размерах окна присвоены соответствующим переменным по ссылкам.
    * Вызываемые модули: <sys/ioctl.h>
    * Описание алгоритма:
    * Для получения необходимых данных используется структура winsize, определенная в ioctl-types.h (<sys/ioctl.h>)
    * 1) Создается экземпляр структуры winsize (default_window)
    * 2) С помощью функции ioctl() и запроса TIOCGWINSZ данные о терминале сохраняются в структуре default_window.
    * 3) Выполняется возвращаемого значения функции ioctl():
    *   - в случае успешного выполнения переменным width и height по ссылкам присваиваются соответствующие значения
    *   - в случае ошибки выводится сообщение об ошибке, программа завершается с кодом -1.
    * Дата: 15 / 05 / 2022 Версия 1.01
    * Автор: Студент НГТУ ИРИТ, Халеев А. А. гр. 21-ИВТз
    * Исправления: нет
    *****************************************************************************************************************/

    struct winsize default_window{}; // создание экземпляра структуры
    if(ioctl(File_Descriptor, TIOCGWINSZ, &default_window) >= 0) { // запрос и проверка
        width = default_window.ws_col;
        height = default_window.ws_row;
    }else {
        perror("ioctl: ");
        exit(-1);
    }
}

void CustomizeTerminal(const std::string& font, const std::string& back){
    /*****************************************************************************************************************
    * Цель: Изменить параметры терминала  для корректной работы программы в соответствии с заданием.
    * Исходные данные:
    *   font - цвет текста терминала
    *   back - цвет фона терминала
    * Результат: Терминал готов к работе программы с новыми параметрами.
    * Вызываемые модули: <definitions.h>
    * Описание алгоритма:
    * Для изменения необходимых настроек терминала используются макроопределения, описанные в модуле definitions.h
    * 1) С помощью значений, соответствующих требованиям к работе программы, изменяется размер окна терминала
    * 2) Применяются новые настройки цветовой палитры терминала.
    * 3) Курсор перемещается в левый верхний угол, производится очистка экрана и буфера с заполнением фона с учетом
    * новых настроек цветовой палитры.
    * Дата: 15 / 05 / 2022 Версия 1.01
    * Автор: Студент НГТУ ИРИТ, Халеев А. А. гр. 21-ИВТз
    * Исправления: нет
    *****************************************************************************************************************/
    set_window_size(SCREEN_WIDTH, SCREEN_HEIGHT);
    set_display_atrib(font, back);
    clear_screen();
}

void ResetTerminal(int def_width, int def_height){
    /*****************************************************************************************************************
    * Цель: Восстановить исходные настройки терминала.
    * Исходные данные:
    *   def_width  - исходная ширина окна терминала
    *   def_height - исходная высота окна терминала
    * Результат:
    *   - восстановлен исходный размер окна
    *   - восстановлены цвета текста и фона терминала
    *   - очищены буфер прокрутки и содержимое окна терминала
    * Вызываемые модули: <definitions.h>
    * Описание алгоритма:
    * Для изменения необходимых настроек терминала используются макроопределения, описанные в модуле definitions.h
    * 1) С помощью полученных значений, соответствующих исходным, устанавливается размер окна терминала
    * 2) Выполняется сброс всех параметров терминала в значения по умолчанию
    * Дата: 15 / 05 / 2022 Версия 1.01
    * Автор: Студент НГТУ ИРИТ, Халеев А. А. гр. 21-ИВТз
    * Исправления: нет
    *****************************************************************************************************************/
    set_window_size(def_width, def_height);
    reset_screen();
}

void DrawStartBlock(){
    /*****************************************************************************************************************
    * Цель: Отрисовка первой ячейки в строке таблицы
    * Исходные данные:
    * Результат:
    *   в терминал выведена рамка первой ячейки в строке таблицы
    * Вызываемые модули: <definitions.h>
    * Описание алгоритма:
    * Отличительная особенность данной рамки для ячейки:
    *   - "┌/┐"-образные верхние границы
    *   - "└/┘"-образные нижние границы
    * 1) Вывод верхней части рамки
    * 2) Возврат курсора на размер ячейки (BLOCK_WIDTH) и перемещение вниз на одну строку
    * 3) Вывод средней части ячейки
    * 4) Возврат курсора на размер ячейки (BLOCK_WIDTH) и перемещение вниз на одну строку
    * 5) Вывод нижней части ячейки
    * 6) Возврат курсора вверх на 2 строки и влево на 1 позицию для корректной отрисовки следующей ячейки
    * Дата: 15 / 05 / 2022 Версия 1.01
    * Автор: Студент НГТУ ИРИТ, Халеев А. А. гр. 21-ИВТз
    * Исправления: нет
    *****************************************************************************************************************/

    std::cout << "┌────────────────┐";
    move_left(BLOCK_WIDTH);
    move_down(1);
    std::cout << "│                │";
    move_left(BLOCK_WIDTH);
    move_down(1);
    std::cout << "└────────────────┘";
    move_up(2);
    move_left(1);
}

void DrawMidBlock(){
    /*****************************************************************************************************************
    * Цель: Отрисовка промежуточных ячеек в строке таблицы
    * Исходные данные:
    * Результат:
    *   в терминал выведена рамка первой ячейки в строке таблицы
    * Вызываемые модули: <definitions.h>
    * Описание алгоритма:
    * Отличительная особенность данной рамки для ячейки:
    *   - "┬"-образные верхние границы
    *   - "┴"-образные нижние границы
    * 1) Вывод верхней части рамки
    * 2) Возврат курсора на размер ячейки (BLOCK_WIDTH) и перемещение вниз на одну строку
    * 3) Вывод средней части ячейки
    * 4) Возврат курсора на размер ячейки (BLOCK_WIDTH) и перемещение вниз на одну строку
    * 5) Вывод нижней части ячейки
    * 6) Возврат курсора вверх на 2 строки и влево на 1 позицию для корректной отрисовки следующей ячейки
    * Дата: 15 / 05 / 2022 Версия 1.01
    * Автор: Студент НГТУ ИРИТ, Халеев А. А. гр. 21-ИВТз
    * Исправления: нет
    *****************************************************************************************************************/
    std::cout << "┬────────────────┬";
    move_left(BLOCK_WIDTH);
    move_down(1);
    std::cout << "│                │";
    move_left(BLOCK_WIDTH);
    move_down(1);
    std::cout << "┴────────────────┴";
    move_up(2);
    move_left(1);
}

void DrawEndBlock(){
    /*****************************************************************************************************************
    * Цель: Отрисовка последней ячейки в строке таблицы
    * Исходные данные:
    * Результат:
    *   в терминал выведена рамка первой ячейки в строке таблицы
    * Вызываемые модули: <definitions.h>
    * Описание алгоритма:
    * Отличительная особенность данной рамки для ячейки:
    *   - "┬/┐"-образные верхние границы
    *   - "┴/┘"-образные нижние границы
    * 1) Вывод верхней части рамки
    * 2) Возврат курсора на размер ячейки (BLOCK_WIDTH) и перемещение вниз на одну строку
    * 3) Вывод средней части ячейки
    * 4) Возврат курсора на размер ячейки (BLOCK_WIDTH) и перемещение вниз на одну строку
    * 5) Вывод нижней части ячейки
    * 6) Возврат курсора вверх на 2 строки и влево на 1 позицию для корректной отрисовки следующей ячейки
    * Дата: 15 / 05 / 2022 Версия 1.01
    * Автор: Студент НГТУ ИРИТ, Халеев А. А. гр. 21-ИВТз
    * Исправления: нет
    *****************************************************************************************************************/
    std::cout << "┬────────────────┐";
    move_left(BLOCK_WIDTH);
    move_down(1);
    std::cout << "│                │";
    move_left(BLOCK_WIDTH);
    move_down(1);
    std::cout << "┴────────────────┘";
    move_up(2);
    move_left(1);
}

void DrawFrame(int rows){
    /*****************************************************************************************************************
    * Цель: Отрисовка границ таблицы (рамка)
    * Исходные данные:
    *   rows - количество строк в таблице
    * Результат:
    *   в терминал выведена рамка таблицы
    * Вызываемые модули: <definitions.h>, "draw_func.h"
    * Описание алгоритма:
    * Отличительная особенность данной рамки для ячейки:
    *   - "┬/┐"-образные верхние границы
    *   - "┴/┘"-образные нижние границы
    * 1) Вывод верхней части рамки
    * 2) Возврат курсора на размер ячейки (BLOCK_WIDTH) и перемещение вниз на одну строку
    * 3) Вывод средней части ячейки
    * 4) Возврат курсора на размер ячейки (BLOCK_WIDTH) и перемещение вниз на одну строку
    * 5) Вывод нижней части ячейки
    * 6) Возврат курсора вверх на 2 строки и влево на 1 позицию для корректной отрисовки следующей ячейки
    * Дата: 15 / 05 / 2022 Версия 1.01
    * Автор: Студент НГТУ ИРИТ, Халеев А. А. гр. 21-ИВТз
    * Исправления: нет
    *****************************************************************************************************************/
    move_cursor(START_ROW_FR, START_COL_FR);
    for (int i=0; i < rows; i++){
        for (int j=0; j < TABLE_COLS; j++){
            switch(j){
                case 0: DrawStartBlock(); break;
                case (TABLE_COLS - 1): DrawEndBlock(); break;
                default: DrawMidBlock(); break;
            }
        }
        move_start_down(3);
        move_right(5);
    }
}

void DrawHead(){
    /*****************************************************************************************************************
    * Цель: Вывод заголовков столбцов таблицы в соответствующие ячейки
    * Исходные данные:
    * Результат: В первую строку таблицы выведены заголовки столбцов
    * Вызываемые модули: <string>, <iostream>, "definitions.h"
    * Описание алгоритма:
    * 1) Формируется массив строк типа string (head[]), содержащий заголовки столбцов таблицы
    * 2) Устанавливается жирный шрифт вывода в терминале
    * 3) В цикле изменяются 2 переменные:
    *   col: от 0 до TABLE_COLS (не включительно) - соответствует индексу заголовков в массиве head[]/столбцу таблицы
    *   out_pos: от START_COL до завершения цикла с шагом STEP_COL - позиция курсора для вывода очередного заголовка
    * 4) На каждой итерации цикла курсор перемещается в заданную позицию:
    *   строка курсора  - START_ROW_HEAD (не изменяется)
    *   позиция курсора - out_pos
    * 5) После перемещения курсора в соответствующей позиции выводится элемент массива head с индексом col
    * 6) Отменяется установка жирного шрифта для вывода терминала
    * Дата: 15 / 05 / 2022 Версия 1.01
    * Автор: Студент НГТУ ИРИТ, Халеев А. А. гр. 21-ИВТз
    * Исправления: нет
    *****************************************************************************************************************/

    /* Массив заголовков */
    std::string head[] = {"N","CPU Type","CPU Freq. (MHz)","Memory (MB)","Storage (GB)",
                          "Display"};
    bright_on(); // жирный шрифт
    /* Цикл перемещения курсора и вывода заголовков */
    for(int col=0, out_pos=START_COL; col < TABLE_COLS; out_pos+=(STEP_COL), col++){
        move_cursor(START_ROW_HEAD, out_pos);
        std::cout << head[col];
    }
    bright_off(); // обычный шрифт
}

void DrawTable(){
    /*****************************************************************************************************************
    * Цель: Отрисовка шаблона таблицы (рамка + заголовки столбцов)
    * Исходные данные:
    * Результат:
    *   в терминал выведен пустой шаблон таблицы
    * Вызываемые модули: <definitions.h>, "draw_func.h"
    * Описание алгоритма:
    * Последовательно вызываются 2 функции:
    *   1) DrawFrame - рамка, в качестве аргумента передается  константа MAX_ROW_FR соответствующая максимальному
     *   отображаемых строк на дисплее
    *   2) DrawHead  - заголовки столбцов
    * Дата: 15 / 05 / 2022 Версия 1.01
    * Автор: Студент НГТУ ИРИТ, Халеев А. А. гр. 21-ИВТз
    * Исправления: нет
    *****************************************************************************************************************/
    DrawFrame(MAX_ROWS_FR); // рамка
    DrawHead();                  // заголовки
}