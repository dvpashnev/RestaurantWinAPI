#include "header.h"

Order::Order() : 
Menu(),
table_(0),			//Номер стола
No_(0),				//Номер заказа
totals_(0.0),		//Сумма
waiter_(L"Иванов"),	//Официант
time_(L""),			//Время
date_(L"")			//Дата
{}

Order::Order(const Order& obj):
Menu(obj.menu_),
table_(obj.table_),		//Номер стола
No_(obj.No_),			//Номер заказа
totals_(obj.totals_),	//Сумма
waiter_(obj.waiter_),	//Официант
time_(obj.time_),		//Время
date_(obj.date_)		//Дата
{}

Order::~Order(){}

size_t Order::getNo()
{
	return No_;
}

size_t Order::getNoTable()
{
	return table_;
}

double Order::getTotals_()
{
	return totals_;
}

wstring Order::getWaiter_()
{
	return waiter_;

}
wstring Order::getTime_()
{
	return time_;

}
wstring Order::getDate_()
{
	return date_;

}


void Order::setNo(size_t n)
{
	No_ = n;
}

void Order::setNoTable(size_t n)
{
	table_ = n;
}

Order& Order::operator = (const Order& obj)
{
	table_ = obj.table_;	//Номер стола
	No_ = obj.No_;			//Номер заказа
	totals_ = obj.totals_;	//Сумма
	waiter_ = obj.waiter_;	//Официант
	time_ = obj.time_;		//Время
	date_ = obj.date_;		//Дата
	menu_ = obj.menu_;		//Список блюд
	return *this;
}

void Order::saveToFile(const wstring& nameFile)
{
	wstring filepath(nameFile);
	int length = WideCharToMultiByte(CP_ACP, 0,
		filepath.c_str(),
		-1, NULL, 0, 0, 0);
	char *ptr = new char[length];
	WideCharToMultiByte(CP_ACP, 0,
		filepath.c_str(),
		-1, ptr, length, 0, 0);

	wofstream f(ptr, ios::trunc);

	try{
		if (!f)
			throw 1;

		f.imbue(locale("rus_rus.866"));

		f << table_ << endl;		//Номер стола
		f << No_ << endl;			//Номер заказа
		f << totals_ << endl;		//Сумма
		f << waiter_ << endl;		//Официант
		f << time_ << endl;			//Время
		f << date_ << endl;			//Дата

		for (Dish d : menu_)		//Список блюд
		{
			f << d;
		}
	}
	catch (int)
	{
		MessageBox(0,
			TEXT("Файл не открылся для записи !!!"),
			TEXT("Ошибка записи!"),
			MB_OK | MB_ICONERROR | MB_TASKMODAL);
	}
	f.close();
	delete[] ptr;
}

void Order::readFromFile(const wstring& nameFile)
{
	wifstream f(nameFile.c_str());

	try{
		if (!f)
			throw 2;

		f.imbue(locale("rus_rus.866"));

		f >> table_;			//Номер стола
		f >> No_;				//Номер заказа
		(f >> totals_).get();	//Сумма
		getline(f, waiter_);	//Официант
		getline(f, time_);		//Время
		getline(f, date_);		//Дата

		Dish tmp;
		while (!f.eof())		//Список блюд
		{
			f >> tmp;
			if (f.good())
				menu_.push_back(tmp);
		}
	}
	catch (int)
	{
		MessageBox(0,
			TEXT("Файл не открылся для чтения !!!"),
			TEXT("Ошибка записи!"),
			MB_OK | MB_ICONERROR | MB_TASKMODAL);
	}
	f.close();
}

wstring Order::nameOfFile()
{
	wstringstream convert;
	convert << No_;
	wstring path = L"orders\\" + strDate(curTime()) + L"\\" + convert.str();
	return path;
}

wstring Order::nameOfFile(const wstring& s)
{
	return nameOfFile() + s;
}