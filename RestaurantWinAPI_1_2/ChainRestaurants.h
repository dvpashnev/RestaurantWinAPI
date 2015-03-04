using namespace std;

class ChainRestaurants
{										//СЕТЬ РЕСТОРАНОВ
public:

	ChainRestaurants();
	ChainRestaurants(const ChainRestaurants&);
	~ChainRestaurants();

	Restaurant* factory(const wstring&, const wstring&);	//Фабрика ресторанов.
	
	//-----ДОСТУП К ДАННЫМ---------------
	Restaurant* operator[](size_t index);	//Доступ к конкретному элементу по индексу.
	vector<Restaurant*>& operator()();		//Доступ к chain_ (= getChain, но не const).
	
	//-----ЧТЕНИЕ РЕСТОРАНОВ ИЗ ПАПОК----
	bool readChainRestaurantsFromDirs();	//Чтение всех ресторанов, которые есть в папке программы.
	
	//-----СОЗДАНИЕ РЕСТОРАНА------------
	void setNewRestaurant(const wstring&, const wstring&);	//Задаёт новый ресторан с именем и типом.

	//-----ФУНКТОРЫ----------------------
	//Удаляет ресторан из кучи по указателю
	class deleteRestaurant
	{
	public:
		void operator()(Restaurant* ptr);
	};
	//Удаляет все файлы и папку ресторана по указателю
	class removeRestaurant
	{
	public:
		void operator()(Restaurant* ptr);
	};

	//-ОСНОВНЫЕ ФУНКЦИИ РАБОТЫ В ЗАВИСИМОСТИ ОТ РОЛИ-
	void service(Roles);

	//Интерфейс
	static BOOL CALLBACK DlgProc(HWND hWnd, UINT mes, WPARAM wp, LPARAM lp);
	static ChainRestaurants* ptr;

	void Cls_OnInitDialog(HWND hwnd, LPARAM);
	void Cls_OnCommand(HWND hwnd, WPARAM);
	void Cls_OnClose(HWND hwnd);

	template<typename T>
	T& callDialog(T&);			//Функция вызова диалога работы с рестораном в зависимости от роли и вида ресторана

protected:
	
	vector<Restaurant*> chain_;				//Список ресторанов собственно.
	HWND hChainDlg, hListRestaurants;
	Roles role;
	int dlg;

};