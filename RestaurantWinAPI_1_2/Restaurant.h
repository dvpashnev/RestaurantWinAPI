using namespace std;

class Restaurant :			//КЛАСС РЕСТОРАН абстрактный
	public Menu
{
public:
	Restaurant(const wstring& n = L"Ресторан");
	Restaurant(const Restaurant&);
	virtual ~Restaurant()=0;

	wstring getName() const;		//Доступ к имени
	map <size_t, Order> getOpenedOrders() const;//Доступ к открытым заказам
	map <size_t, Order> getClosedOrders() const;//Доступ к закрытым заказам
	size_t getNOpenedOrders();		//Доступ к количеству открытых заказов
	size_t getNClosedOrders();		//Доступ к количеству закрытых заказов
	size_t getNDeletedOrders();		//Доступ к количеству удалённых заказов
	double getOpenedTotals();		//Доступ к Сумма заказов в работе
	double getClosedTotals();		//Доступ к Сумма оплаченных заказов (заработок на сегодня)
	int getNTables();				//Доступ к количеству столов

	void setName(const wstring&);	//Установка имени ресторана
	void setMenu(const Menu&);		//Установка меню

	Restaurant* operator=(const Restaurant&);	//Оператор присваивания

	//--------ФАЙЛЫ И ПАПКИ---------
	void saveStatus();				//Сохраняет в файл статус ресторана при выходе из него
	void readStatus();				//Читает из файла статус ресторана при входе

	//----ЧИСТО ВИРТУАЛЬНЫЕ ФУНКЦИИ-
	virtual wstring nameOfFile(const wstring&)=0;	//Создаёт путь файла под папку ресторана 

protected:

	wstring name_;						//Название ресторана
	map <size_t, Order> openedOrders_;	//Заказы в работе (Столик - заказ)
	map <size_t, Order> closedOrders_;	//Оплаченные заказы (!!!!!!!!!!!!!Номер!!!!!!!!!!!!! - заказ)
	size_t nOrdersOpened_;				//Количество заказов в работе
	double ordersOpenedTotals_;			//Сумма заказов в работе
	size_t nOrdersClosed_;				//Количество оплаченных заказов
	double ordersClosedTotals_;			//Сумма оплаченных заказов (заработок на сегодня)
	size_t nOrdersDeleted_;				//Количество удалённых заказов
	size_t nTables_;					//Количество столов

	Roles role;							//Роль пользователя

};