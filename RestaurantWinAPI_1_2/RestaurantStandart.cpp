#include "header.h"

RestaurantStandart::RestaurantStandart(const wstring& n) :
Restaurant(n)
{
	wstring dir = nameOfFile(L"orders");
	int length = WideCharToMultiByte(CP_ACP, 0,
		dir.c_str(),
		-1, NULL, 0, 0, 0);
	char *ptr = new char[length];
	WideCharToMultiByte(CP_ACP, 0,
		dir.c_str(),
		-1, ptr, length, 0, 0);

	_mkdir(ptr);		//Создаётся папка для хранения файлов заказов
}

RestaurantStandart::~RestaurantStandart(){}

wstring RestaurantStandart::nameOfFile(const wstring& s)
{
	wstring path = L"Restaurants\\" + name_ + L"Standart\\" + s;
	return path;
}