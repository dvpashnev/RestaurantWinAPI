#include "header.h"

Menu::Menu()
{}

Menu::Menu(const DishContainer& obj)
{
	menu_ = obj;
}

Menu::Menu(const Menu& obj)
{
	menu_ = obj.menu_;
}

Menu::~Menu()
{
	menu_.clear();
}

void Menu::setMenu_(const DishContainer& menu)
{
	menu_ = menu;
}

DishContainer Menu::getMenu_() const
{
	return menu_;
}

DishContainer& Menu::operator()()
{
	return menu_;
}

Menu& Menu::operator = (const Menu& obj)
{
	menu_ = obj.menu_;
	return *this;
}

bool Menu::byPriceIncrease::operator()(const Dish& el1, const Dish& el2)
{//По возрастанию цены
		return (el1.getPrice() < el2.getPrice());
}

bool Menu::byPriceDecrease::operator()(const Dish& el1, const Dish& el2)
{//По убыванию цены
		return (el1.getPrice() > el2.getPrice());
}

bool Menu::byTypeIncrease::operator()(const Dish& el1, const Dish& el2)
{	//по возрастанию типа (от холодных к десертам)
	return ((int)el1.getType() < (int)el2.getType());
}

bool Menu::byTypeDecrease::operator()(const Dish& el1, const Dish& el2)
{//По убыванию типа
	return ((int)el1.getType() > (int)el2.getType());
}

bool Menu::byTimeIncrease::operator()(const Dish& el1, const Dish& el2)
{	//По возрастанию времени (утро-вечер)
	return ((int)el1.getTime() < (int)el2.getTime());
}

bool Menu::byTimeDecrease::operator()(const Dish& el1, const Dish& el2)
{//По убыванию времени (вечер-утро) 
	return ((int)el1.getTime() > (int)el2.getTime());
}

vecindex Menu::findTitle(const wstring& name)
{
	wstring s(name), sObject;
	string sMb, sMbObject;
	vecindex rezult;

		rezult.reserve(menu_.size() / 3);
		int length = WideCharToMultiByte(CP_ACP, 0,
			s.c_str(),
			-1, NULL, 0, 0, 0);
		char *ptr = new char[length];
		WideCharToMultiByte(CP_ACP, 0,
			s.c_str(),
			-1, ptr, length, 0, 0);
		sMb = ptr;
		delete ptr;
		transform(sMb.begin(), sMb.end(), sMb.begin(), tolower);
			for (size_t i = 0; i < menu_.size(); i++)
			{
				if (menu_[i].getTitle() == L"N/A")
					continue;//Исключаем пустые блюда в результатах предыдушего поиска
				sObject = menu_[i].getTitle();
				int length = WideCharToMultiByte(CP_ACP, 0,
					sObject.c_str(),
					-1, NULL, 0, 0, 0);
				ptr = new char[length];
				WideCharToMultiByte(CP_ACP, 0,
					sObject.c_str(),
					-1, ptr, length, 0, 0);
				sMbObject = ptr;
				transform(sMbObject.begin(), sMbObject.end(), sMbObject.begin(), tolower);
				if (sMbObject.find(sMb.c_str(), 0, sMb.size()) != wstring::npos)
					rezult.push_back(i);
			}

		if (!rezult.empty()) 
		{
			sort(rezult.begin(), rezult.end());
			rezult.resize(unique(rezult.begin(), 
				rezult.end()) - rezult.begin());//Удаляем повторяющиеся элементы
		}
	return rezult;
}


int Menu::findFullTitle(const wstring& name)
{
	wstring s(name), sObject;

	transform(s.begin(), s.end(), s.begin(), tolower);
	for (size_t i = 0; i < menu_.size(); i++)
		{
			sObject = menu_[i].getTitle();
			transform(sObject.begin(), sObject.end(), sObject.begin(), tolower);
			if (sObject==s)
				return i;
		}
	return -1;
}

vecindex Menu::findType(TypeDish type)
{
	vecindex rezult;
	rezult.reserve(menu_.size() / 3);

	size_t i(0);
	for (Dish d : menu_){
		if (d.getTitle() == L"N/A"){ //Исключаем пустые блюда в результатах предыдушего поиска
			i++;
			continue;
		}
		if (d.getType() == type)
			rezult.push_back(i);
		i++;
	}
	return rezult;
}

vecindex Menu::findType(int* type)
{
	vecindex result, rezultColds, resultHots, resultDeserts;
	result.reserve(menu_.size() / 3);
	rezultColds.reserve(menu_.size() / 3);
	resultHots.reserve(menu_.size() / 3);
	resultDeserts.reserve(menu_.size() / 3);

	if (type[0] == 1)
		rezultColds = findType(COLD);
	if (type[1] == 1)
		resultHots = findType(HOT);
	if (type[2] == 1)
		resultDeserts = findType(DESERT);

	if (!rezultColds.empty())
		result.insert(result.end(), rezultColds.begin(), rezultColds.end());
	if (!resultHots.empty())
		result.insert(result.end(), resultHots.begin(), resultHots.end());
	if (!resultDeserts.empty())
		result.insert(result.end(), resultDeserts.begin(), resultDeserts.end());
	sort(result.begin(), result.end());
	return result;
}

vecindex Menu::findTime(TimeDay ti)
{
	vecindex rezult;
	rezult.reserve(menu_.size()/2);

	size_t i(0);
	for (Dish d : menu_){
		if (d.getTitle() == L"N/A"){ //Исключаем пустые блюда в результатах предыдушего поиска
			i++;
			continue;
		}
		if (d.getTime() == ti || d.getTime() == ALL)
			rezult.push_back(i);
		i++;
	}
	return rezult;
}

Menu Menu::vectorToMenu(const vecindex& dishNos)
{//Используется для результатов поиска. 
	Menu tmp;	
	tmp.menu_.resize(menu_.size());
	for (size_t i : dishNos)//Из меню берутся соответствующие блюда по номерам в dishNos,
		tmp.menu_[i] = menu_[i];//остальные остаются изначальными (см. конструктор по умолчанию).
	return tmp;
}

void Menu::saveToFile(const wstring& nameFile)
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

		for (Dish d : menu_)
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
	delete [] ptr;
}

void Menu::readFromFile(const wstring& nameFile)
{
	wifstream f(nameFile.c_str());

	try{
		if (!f)
			throw 2;
		Dish tmp;
		while (!f.eof())
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
	sort(menu_.begin(), menu_.end());
}

void Menu::addDish(Dish& tmp)
{
	if (menu_.size() < 2)
	{
		menu_.push_back(tmp);
		sort(menu_.begin(), menu_.end());
		return;
	}
	if (menu_.begin()->getTitle() < (menu_.begin() + 1)->getTitle())//Если сортировка по алфавиту,
		menu_.insert(lower_bound(menu_.begin(), menu_.end(), tmp), tmp);//вставляем по алфавиту,
	else																		//нет -
		menu_.insert(lower_bound(menu_.begin(), menu_.end(), tmp, greater<Dish>()), tmp);//в обратном порядке
}

void Menu::delDish(size_t index)
{
	menu_.erase(menu_.begin() + index);
}