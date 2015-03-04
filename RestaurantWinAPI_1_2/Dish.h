using namespace std;

enum TypeDish { COLD, HOT,       DESERT };			//Типы блюд
enum TimeDay  { ALL,  MORNING,   AFTERNOON, EVENING };//Время суток блюд

class Dish
{											//БЛЮДО
public:

	Dish(
		const wstring& title = L"N/A",
		double price = 0.0,
		int portion_ = 0,
		TypeDish type = COLD,
		TimeDay tDD = ALL,
		const wstring& description = L"N/A",
		const wstring& picPath = L"N/A",
		int num = 1
		);
	Dish( const Dish& );
	~Dish();

	//----УСТАНОВКА ПОЛЕЙ--------- 
	void setTitle(const wstring&);
	void setPrice(double);
	void setPortion(int);
	void setType(TypeDish);
	void setTime(TimeDay);
	void setDescription(const wstring&);
	void setPicPath(const wstring&);
	void setNumber(int);
	void setDish(
		const wstring&,
		double, 
		int,
		TypeDish, 
		TimeDay, 
		const wstring&,
		const wstring&,
		int
		);

	//----ДОСТУП К ПОЛЯМ--------- 
	wstring  getTitle() const;
	double   getPrice() const;
	wstring  getPriceStr() const;
	int		 getPortion() const;
	wstring	 getPortionStr() const;
	TypeDish getType() const;
	wstring  getTypeStr() const;
	TimeDay  getTime() const;
	wstring  getTimeStr() const;
	wstring  getDescription() const;
	wstring	 getPicPath() const;
	int		 getNumber() const;
	wstring	 getNumberStr() const;
	wstring	 getTotalsStr() const;

	//--ПЕРЕГРУЗКИ ОПЕРАТОРОВ---
	friend wostream& operator<<(wostream&,
		const Dish& );				//Распечатка названия и цены (для меню)
	friend wistream& operator>>(wistream&,
		Dish&);
	bool operator==(const Dish&);	//Эквивалент двух блюд

	Dish& operator=(const Dish&);	//Присваивание

	//--ПРЕДИКАТЫ ДЛЯ СТАНДАРТНОЙ СОРТИРОВКИ--
	friend bool operator<(const Dish&, 
		const Dish&);				//less: по типам + по алфавиту (название блюда) независимо от регистра
	friend bool operator>(const Dish&, 
		const Dish&);				//greater: то же, но в обратном порядке алфавита

protected:

	wstring  title_;			//Название
	double    price_;			//Цена, грн
	int		 portion_;			//Порция, грамм
	TypeDish type_;				//Тип
	TimeDay  timeDay_;			//Время суток (для составления текущего меню)
	wstring	 description_;		//Описание блюда
	wstring	 picPath_;			//Путь к файлу картинки блюда
	int		 number_;			//Количество (для заказа)

};