using namespace std;

typedef vector<size_t> vecindex;
typedef vector<Dish> DishContainer;
typedef DishContainer::iterator DCIterator;

class Menu
{								//КЛАСС МЕНЮ
public:

	Menu();
	Menu(const DishContainer&);
	Menu(const Menu&);
	~Menu();

	void setMenu_(const DishContainer& menu);
	DishContainer getMenu_() const;		//Доступ к списку меню
	DishContainer& operator()();		//Аналогично menu_
	Menu& operator=(const Menu&);		//Оператор присваивания

	//ФУНКТОРЫ-ПРЕДИКАТЫ ДЛЯ СОРТИРОВКИ (все сортировки разбивают меню по типам блюд)
	class byPriceIncrease
	{	//Сортировка по возрастанию цены + по возрастанию типа
	public:
		bool operator()(const Dish&, const Dish&);
	};
	class byPriceDecrease
	{	//Сортировка по убыванию цены + по возрастанию типа
	public:
		bool operator()(const Dish&, const Dish&);
	};
	class byTypeIncrease
	{	//Сортировка по возрастанию типа (не по алфавиту)
	public:
		bool operator()(const Dish&, const Dish&);
	};
	class byTypeDecrease
	{	//Сортировка по убыванию типа + по алфавиту
	public:
		bool operator()(const Dish&, const Dish&);
	};
	class byTimeIncrease
	{	//Сортировка по возрастанию типа (не по алфавиту)
	public:
		bool operator()(const Dish&, const Dish&);
	};
	class byTimeDecrease
	{	//Сортировка по убыванию типа + по алфавиту
	public:
		bool operator()(const Dish&, const Dish&);
	};

	//---------ПОИСК-----------
	vecindex findTitle(const wstring&);		//Ищет блюда по названию
	int findFullTitle(const wstring& name);	//Поиск блюда с таким же названием (для исключения блюд с одинаковым названием)

	vecindex findType(TypeDish);		//Ищет блюда по переданному типу
	vecindex findType(int* type);		//Ищет блюда по массиву флагов при поиске нескольких типов
	vecindex findTime(TimeDay);			//Ищет блюда по времени суток
	Menu vectorToMenu(const vecindex&);//Возвращает меню с блюдами из меню, из которого вызвана, по номерам в vecindex

	//--ФАЙЛЫ: ЗАПИСЬ И ЧТЕНИЕ-
	void saveToFile(const wstring&);	//Записывает меню ресторана в файл
	void readFromFile(const wstring&);	//Считывает меню ресторана из файла
	
	//РАБОТА С МЕНЮ
	void addDish(Dish& tmp);				//Добавить блюдо
	void delDish(size_t index);				//Удалить блюдо

protected:

	DishContainer menu_;				//Список меню
	
};