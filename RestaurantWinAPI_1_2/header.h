#pragma once

enum Roles { MANAGER, WAITER };
enum Modes { NEW, SHOW, EDIT };


#include <windows.h>
#include <commctrl.h>
#include <ctime>
#include <tchar.h>
#include <locale>
#include <fstream>		//Запись/чтение файл
#include <io.h>			//fileinfo
#include <direct.h>		//Каталоги
#include <typeinfo>		//Определение типа данных
#include <sstream>		//wstringstream - для конвертации int wstring

#include <string>		//Контейнеры
#include <vector>
#include <map>

#include <algorithm>	//Алгоритмы find, for_each, sort, transform, unique, copy_if
#include <functional>	//greater, less
#include <numeric>		//accumulate

#include "resource.h"

#include "Dish.h"					//Класс Блюдо
#include "Menu.h"					//Класс Меню
#include "DishDlg.h"				//Окно Блюдо
#include "MenuDlg.h"				//Окно Меню
#include "Order.h"					//Класс Заказ
#include "OrderDlg.h"				//Окно заказа

#include "Restaurant.h"				//Класс Ресторан базовый
#include "RestaurantStandart.h"		//Класс стандарт-ресторан наследник Ресторана
#include "RestaurantStandartDlg.h"	//Окно стандарт-ресторан

#include "ChainRestaurants.h"		//Класс Сеть ресторанов, окно внутри (выбор и создание ресторана)

#include "NewRestaurantDlg.h"		//Окно создания ресторана

#include "Enter.h"					//Окно Входа

#pragma comment(lib,"comctl32")

using namespace std;

extern HWND hTransfer;

tm* curTime();						//Выдаёт текущее время

wstring strDate(tm* curTM);			//Создаёт из даты строку (год+месяц+день)

bool isFile(const wstring& fileName);	//Проверка существование файла

void removeDir(const wstring&);	//Удаляет указанную папку со всей информацией (рекурсия)