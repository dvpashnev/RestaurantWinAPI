using namespace std;

class Order :		//КЛАСС ЗАКАЗ 
	public Menu
{
public:

	Order();
	Order(const Order&);
	~Order();
	
	size_t getNo();			//Получение No_
	size_t getNoTable();	//Получение No стола
	double getTotals_();	//Сумма
	wstring getWaiter_();	//Официант
	wstring getTime_();		//Время
	wstring getDate_();		//Дата
	
	void setNo(size_t);		//Установка No_
	void setNoTable(size_t);//Установка No_ стола
	
	Order& operator = (const Order&);	//Оператор присваивания

	//ДЛЯ ЗАПИСИ/ЧТЕНИЯ С ДИСКА
	wstring nameOfFile();				//Создаёт из номера заказа путь файла: "текущая дата/число" (например: 20140815/1)
	wstring nameOfFile(const wstring&);	//То же из номера заказа и строки (для добавления к имени файла суффиксов состояния заказа (del, bill)) (например: 20140815/1bill)
	void saveToFile(const wstring&);	//Записывает заказ в файл
	void readFromFile(const wstring&);	//Считывает заказ из файла

protected:

	size_t table_;			//Номер стола
	size_t No_;				//Номер заказа
	double totals_;			//Сумма
	wstring waiter_;		//Официант
	wstring time_;			//Время
	wstring date_;			//Дата

};