#include "header.h"


Restaurant::Restaurant(const wstring& n) :
Menu(),
name_(n),
nOrdersOpened_(0),
ordersOpenedTotals_(0.0F),
nOrdersClosed_(0),
ordersClosedTotals_(0.0F),
nOrdersDeleted_(0),
nTables_(1),
role(MANAGER)
{
}

Restaurant::Restaurant(const Restaurant& r) :
Menu(r),
name_(r.name_),
nOrdersOpened_(r.nOrdersOpened_),
ordersOpenedTotals_(r.ordersOpenedTotals_),
nOrdersClosed_(r.nOrdersClosed_),
ordersClosedTotals_(r.ordersClosedTotals_),
nOrdersDeleted_(r.nOrdersDeleted_),
nTables_(r.nTables_),
role(r.role)
{}

Restaurant::~Restaurant(){}

wstring Restaurant::getName() const
{
	return name_;
}

map <size_t, Order> Restaurant::getOpenedOrders() const
{
	return openedOrders_;
}

map <size_t, Order> Restaurant::getClosedOrders() const
{
	return closedOrders_;
}

size_t Restaurant::getNOpenedOrders()
{
	return nOrdersOpened_;
}

size_t Restaurant::getNClosedOrders()
{
	return nOrdersClosed_;
}

size_t Restaurant::getNDeletedOrders()
{
	return nOrdersDeleted_;
}

double Restaurant::getOpenedTotals()
{
	return ordersOpenedTotals_;
}

double Restaurant::getClosedTotals()
{
	return ordersClosedTotals_;
}

int Restaurant::getNTables()
{
	return nTables_;
}

void Restaurant::setName(const wstring& n)
{
	name_ = n;
}

void Restaurant::setMenu(const Menu& m)
{
	menu_ = m.getMenu_();
}

Restaurant* Restaurant::operator=(const Restaurant& obj)
{
	name_ = obj.name_;
	openedOrders_ = obj.openedOrders_;
	closedOrders_ = obj.closedOrders_;
	nOrdersOpened_ = obj.nOrdersOpened_;
	ordersOpenedTotals_ = obj.ordersOpenedTotals_;
	nOrdersClosed_ = obj.nOrdersClosed_;
	ordersClosedTotals_ = obj.ordersClosedTotals_;
	nOrdersDeleted_ = obj.nOrdersDeleted_;
	nTables_ = obj.nTables_;
	menu_ = obj.menu_;
	role = obj.role;
	return this;
}

void Restaurant::saveStatus()
{
	wstring path = nameOfFile(name_);
	int length = WideCharToMultiByte(CP_ACP, 0,
		path.c_str(),
		-1, NULL, 0, 0, 0);
	char *ptr = new char[length];
	WideCharToMultiByte(CP_ACP, 0,
		path.c_str(),
		-1, ptr, length, 0, 0);

	wofstream f(path, ios::trunc);
	try{
		if (!f)
			throw 1;
		f << nOrdersOpened_ << endl;		// Записываем количество заказов
		f << ordersOpenedTotals_ << endl;	//и их суммы
		f << nOrdersClosed_ << endl;
		f << ordersClosedTotals_ << endl;
		f << nOrdersDeleted_ << endl;
		f << nTables_;						// Записываем количество столов
	}
	catch (int)
	{
		MessageBox(0, 
			TEXT("Файл не открылся для записи !!!\nСтатус ресторана не записан!"),
			TEXT("Ошибка записи!"), 
			MB_OK | MB_ICONERROR | MB_TASKMODAL);
	}
	f.close();
	delete[] ptr;

	for (pair<size_t, Order> order : openedOrders_)				//Записываем открытые и закрытые заказы
		order.second.saveToFile(nameOfFile(order.second.nameOfFile()));
	for (pair<size_t, Order> order : closedOrders_)
		order.second.saveToFile(nameOfFile(order.second.nameOfFile(L"bill")));

	saveToFile(nameOfFile(L"menu_"));							// Записываем меню ресторана в отдельный файл в его папке
}

void Restaurant::readStatus()
{
	wifstream f(nameOfFile(name_).c_str());
	try{
		if (!f)
			throw 2;
		f >> nOrdersOpened_;				// Считываем количество заказов
		f >> ordersOpenedTotals_;			//и их суммы
		f >> nOrdersClosed_;
		f >> ordersClosedTotals_;
		f >> nOrdersDeleted_;
		f >> nTables_;						// Считываем количество столов
	}
	catch (int)
	{
		MessageBox(0,
			TEXT("Файл не открылся для чтения !!!\nСтатус ресторана не считан!"),
			TEXT("Ошибка записи!"),
			MB_OK | MB_ICONERROR | MB_TASKMODAL);
	}
	f.close();

	for (size_t i(1); i <= (nOrdersDeleted_ + nOrdersOpened_ + nOrdersClosed_); i++)
	{
		Order tmp;
		tmp.setNo(i);

		if (isFile(nameOfFile(tmp.nameOfFile())))
		{
			tmp.readFromFile(nameOfFile(tmp.nameOfFile()));
			openedOrders_.insert(make_pair(tmp.getNoTable(), tmp));
		}
		else if (isFile(nameOfFile(tmp.nameOfFile(L"bill"))))
		{
			tmp.readFromFile(nameOfFile(tmp.nameOfFile(L"bill")));
			closedOrders_.insert(make_pair(tmp.getNoTable(), tmp));
		}
	}
	readFromFile(nameOfFile(L"menu_"));	// Считываем меню ресторана
}