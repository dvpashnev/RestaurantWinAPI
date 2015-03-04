#include"header.h"

Dish::Dish(const wstring& title /*= "N/A"*/,
	double price /*= 0.0*/,
	int portion /*= 0*/,
	TypeDish type /*= COLD*/,
	TimeDay tDD /*= MORNING*/,
	const wstring&	 description /*= "N/A"*/,
	const wstring&	 picPath /*= "N/A"*/,
	int num /*= 1*/) :
		title_(title),
		price_(price),
		portion_(portion),
		type_(type),
		timeDay_(tDD),
		description_(description),
		picPath_(picPath),
		number_(num)
	{}

Dish::Dish(const Dish& obj) :
	title_(obj.title_),
	price_(obj.price_),
	portion_(obj.portion_),
	type_(obj.type_),
	timeDay_(obj.timeDay_),
	description_(obj.description_),
	picPath_(obj.picPath_),
	number_(obj.number_)
	{}

Dish::~Dish()
{}

void Dish::setTitle(const wstring& title)
{
	title_ = title;
}

void Dish::setPrice(double price)
{
	price_ = price;
}

void Dish::setPortion(int portion)
{
	portion_ = portion;
}

void Dish::setType(TypeDish type)
{
	type_ = type;
}

void Dish::setTime(TimeDay tDD)
{
	timeDay_ = tDD;
}

void Dish::setDescription(const wstring& description)
{
	description_ = description;
}

void Dish::setPicPath(const wstring& picPath)
{
	picPath_ = picPath;
}

void Dish::setNumber(int num)
{
	number_ = num;
}

void Dish::setDish(const wstring& title,
	double price, 
	int portion,
	TypeDish type, 
	TimeDay tDD, 
	const wstring& description,
	const wstring& picPath,
	int num)
{
	title_ = title;
	price_ = price;
	portion_ = portion;
	type_ = type;
	timeDay_ = tDD;
	description_ = description;
	picPath_ = picPath;
	number_ = num;
}

wstring Dish::getTitle() const
{
	return title_;
}

double Dish::getPrice() const
{
	return price_;
}

int Dish::getPortion() const
{
	return portion_;
}

wstring Dish::getPriceStr() const
{
	TCHAR buf[10];
	swprintf_s(buf, 10, L"%.2lf", price_);
	return buf;
}

wstring Dish::getPortionStr() const
{
	TCHAR buf[10];
	wsprintf(buf, L"%d", portion_);
	return buf;
}

TypeDish Dish::getType() const
{
	return type_;
}

TimeDay Dish::getTime() const
{
	return timeDay_;
}

wstring Dish::getTypeStr() const
{
	wstring str;
	switch (type_)
	{
	case COLD :
		str = TEXT("Холодное блюдо");
		break;
	case HOT:
		str = TEXT("Горячее блюдо");
		break;
	case DESERT:
		str = TEXT("Десерт");
		break;
	}
	return str;
}
wstring Dish::getTimeStr() const
{
	wstring str;
	switch (timeDay_)
	{
	case ALL:
		str = TEXT("Любое время");
		break;
	case MORNING:
		str = TEXT("Утро");
		break;
	case AFTERNOON:
		str = TEXT("День");
		break;
	case EVENING:
		str = TEXT("Вечер");
		break;
	}
	return str;
}


wstring Dish::getDescription() const
{
	return description_;
}

wstring Dish::getPicPath() const
{
	return picPath_;
}

int Dish::getNumber() const
{
	return number_;
}

wstring	Dish::getNumberStr() const
{
	TCHAR buf[10];
	wsprintf(buf, L"%d", number_);
	return buf;
}

wstring	Dish::getTotalsStr() const
{
	TCHAR buf[10];
	swprintf_s(buf, 10, L"%.2lf", (double)number_*price_);
	return buf;
}

wostream& operator<<(wostream& os, const Dish& obj)
{
	os.imbue(locale("rus_rus.866"));
	os << obj.title_ << endl;
	os << obj.price_ << endl;
	os << obj.portion_ << endl;
	os << (int)obj.type_ << endl;
	os << (int)obj.timeDay_ << endl;
	os << obj.description_ << endl;
	os << obj.picPath_ << endl;
	os << obj.number_ << endl;
	return os;
}

wistream& operator>>(wistream& is, Dish& obj)
{
	is.imbue(locale("rus_rus.866"));
	getline(is, obj.title_);
	is >> obj.price_;
	is >> obj.portion_;
	int pro;
	is >> pro;
	obj.type_ = (TypeDish)pro;
	(is >> pro).get();
	obj.timeDay_ = (TimeDay)pro;
	getline(is, obj.description_);
	getline(is, obj.picPath_);
	(is >> obj.number_).get();
	return is;
}

bool Dish::operator==(const Dish& obj)
{
	return (title_ == obj.title_ 
		&& price_ == obj.price_ 
		&& portion_ == obj.portion_
		&& type_ == obj.type_
		&& timeDay_ == obj.timeDay_
		&& description_ == obj.description_
		&& picPath_ == obj.picPath_
		&& number_ == obj.number_);
}

Dish& Dish::operator=(const Dish& obj)
{
	if (this == &obj)
		return *this;
	title_ = obj.title_;
	price_ = obj.price_; 
	portion_ = obj.portion_;
	type_ = obj.type_;
	timeDay_ = obj.timeDay_;
	description_ = obj.description_;
	picPath_ = obj.picPath_;
	number_ = obj.number_;
	return *this;
}

bool operator<(const Dish& el1, const Dish& el2)
{//Для сравнения при сортировке по алфавиту
	int length = WideCharToMultiByte(CP_ACP, 0,
		el1.title_.c_str(),
		-1, NULL, 0, 0, 0);
	char *ptr = new char[length];
	WideCharToMultiByte(CP_ACP, 0,
		el1.title_.c_str(),
		-1, ptr, length, 0, 0);
	string sMb1 = ptr;
	delete ptr;
	length = WideCharToMultiByte(CP_ACP, 0,
		el2.title_.c_str(),
		-1, NULL, 0, 0, 0);
	ptr = new char[length];
	WideCharToMultiByte(CP_ACP, 0,
		el2.title_.c_str(),
		-1, ptr, length, 0, 0);
	string sMb2 = ptr;
	delete ptr;
	transform(sMb1.begin(), sMb1.end(), sMb1.begin(), tolower);	//независимо
	transform(sMb2.begin(), sMb2.end(), sMb2.begin(), tolower);	//от регистра
	return (sMb1 < sMb2);										//больше по алфавиту.
}

bool operator>(const Dish& el1, const Dish& el2)
{//В обратном порядке алфавита
	int length = WideCharToMultiByte(CP_ACP, 0,
		el1.title_.c_str(),
		-1, NULL, 0, 0, 0);
	char *ptr = new char[length];
	WideCharToMultiByte(CP_ACP, 0,
		el1.title_.c_str(),
		-1, ptr, length, 0, 0);
	string sMb1 = ptr;
	delete ptr;
	length = WideCharToMultiByte(CP_ACP, 0,
		el2.title_.c_str(),
		-1, NULL, 0, 0, 0);
	ptr = new char[length];
	WideCharToMultiByte(CP_ACP, 0,
		el2.title_.c_str(),
		-1, ptr, length, 0, 0);
	string sMb2 = ptr;
	delete ptr;
	transform(sMb1.begin(), sMb1.end(), sMb1.begin(), tolower);
	transform(sMb2.begin(), sMb2.end(), sMb2.begin(), tolower);
	return (sMb1 > sMb2);
}