using namespace std;

class RestaurantStandart : 		//КЛАСС РЕСТОРАН СТАНДАРТНЫЙ
	public Restaurant
{
public:

	RestaurantStandart(const wstring& n = L"Standart");
	virtual ~RestaurantStandart();

	virtual wstring nameOfFile(const wstring& s);//Создаёт из строки путь файла под папку ресторана

};