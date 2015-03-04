#include "header.h"

ChainRestaurants* ChainRestaurants::ptr = NULL;

ChainRestaurants::ChainRestaurants()
	:
	role(MANAGER),
	dlg(0)
{
	chain_.reserve(1);	//Резервируем память под рестораны
	ptr = this;
}

ChainRestaurants::ChainRestaurants(const ChainRestaurants& rChain)
{
	chain_.reserve(rChain.chain_.size());
	string kindOfRest;
	dlg = rChain.dlg;
	role = rChain.role;

	for (Restaurant* r : rChain.chain_)
	{
		kindOfRest = typeid(*r).name();						//Узнаём тип ресторана под указателем,
		kindOfRest = kindOfRest.substr(16, 8);				//оставляем суффикс, к. показывает тип ресторана,

		int length = MultiByteToWideChar(CP_ACP, 0, kindOfRest.c_str(), -1, NULL, 0);
		wchar_t *wKindOfRest = new wchar_t[length];
		MultiByteToWideChar(CP_ACP, 0, kindOfRest.c_str(), -1, wKindOfRest, length);
		chain_.push_back(factory(r->getName(), wKindOfRest)); //создаём соотв. ресторан и добавляем в сеть,
		*chain_.back() = *r;								           //копируем ресторан в указателе.

		delete [] wKindOfRest;
	}
}

ChainRestaurants::~ChainRestaurants()
{
	for_each(chain_.begin(), chain_.end(), deleteRestaurant());
}

Restaurant* ChainRestaurants::factory(const wstring& name, const wstring& type)
{
	//if (type == L"Vip")
	//	return new RestaurantVip(name);
	//else 
	if (type == L"Standart")
		return new RestaurantStandart(name);
	//else if (type == L"FastFood")
	//	return new RestaurantFastFood(name);
	return new RestaurantStandart(name);
}

Restaurant* ChainRestaurants::operator[](size_t index)
{
	return chain_[index];
}

vector<Restaurant*>& ChainRestaurants::operator()()
{
	return chain_;
}

bool ChainRestaurants::readChainRestaurantsFromDirs()
{
	string rName;
	wstring typeRest;
	
	_finddata_t *fileinfo = new _finddata_t;

	long done = _findfirst("Restaurants/*", fileinfo);
	int MayWeWork = done;
	while (MayWeWork != -1)
	{
		rName = fileinfo->name;
		if (rName != "." && rName != ".."){//Исключаем Корневой и родительский каталог
			if (rName.find("Vip") != string::npos)
			{	//Папка Вип-ресторана, 
				rName.resize(strlen(fileinfo->name) - 3);//берём его имя (имя папки - "Vip") и тип и
				typeRest = L"Vip"; 
			}
			else if (rName.find("Standart") != string::npos)
			{	//Папка стандарт-ресторана, -//-
				rName.resize(strlen(fileinfo->name) - 8);
				typeRest = L"Standart";
			}
			else if (rName.find("FastFood") != string::npos)
			{	//Папка закусочной, -//-
				rName.resize(strlen(fileinfo->name) - 8);
				typeRest = L"FastFood";
			}
			int length = MultiByteToWideChar(CP_ACP /*ANSI code page*/, 0, rName.c_str(), -1, NULL, 0);

			wchar_t *ptr = new wchar_t[length];
			// конвертируем ANSI-строку в Unicode-строку
			MultiByteToWideChar(CP_ACP, 0, rName.c_str(), -1, ptr, length);

			back_inserter(chain_) = factory(ptr, typeRest);	//создаём соответствующий ресторан под
			delete [] ptr;
		}													//добавленным в chain_ указателем.
														
		MayWeWork = _findnext(done, fileinfo);
	}
	_findclose(done);

	for(Restaurant* r : chain_)
		r->readStatus();//Считываем статус каждого ресторана сети

	delete fileinfo;

	if (!chain_.empty())//Если на диске есть информация о ресторанах,
		return 1;//возвращаем 1
	else{
		return 0;//или наоборот
	}
}

void ChainRestaurants::setNewRestaurant(const wstring& name, const wstring& type)
{
	Restaurant* tmp = factory(name, type);
	size_t i(0);
	if (!chain_.empty())
	{
		vector<Restaurant*>::iterator it = chain_.begin();
		vector<Restaurant*>::iterator itBack = --chain_.end();
		if ((*itBack)->getName() < tmp->getName())
		{
			chain_.insert(chain_.end(), tmp);
			i = chain_.size() - 1;
		}
		else
			while (it != chain_.end())
			{
			if ((*it)->getName() > tmp->getName())
			{
				chain_.insert(it, tmp);
				break;
			}
			it++;
			i++;
			}
	}
	else
		chain_.push_back(tmp);
	
	int length = WideCharToMultiByte(CP_ACP , 0, 
		chain_[i]->nameOfFile(L"").c_str(),
		-1, NULL, 0, 0, 0);
	char *ptr = new char[length];
	WideCharToMultiByte(CP_ACP, 0, 
		chain_[i]->nameOfFile(L"").c_str(),
		-1, ptr, length, 0, 0);
	_mkdir(ptr);
	chain_[i]->saveStatus();
	delete [] ptr;
}

void ChainRestaurants::deleteRestaurant::operator()(Restaurant* ptr)
{
	delete ptr;
}

void ChainRestaurants::removeRestaurant::operator()(Restaurant* ptr)
{
	removeDir(ptr->nameOfFile(L""));
}

void ChainRestaurants::service(Roles role)
{
	_mkdir("Restaurants");		//Создаётся папка для хранения всех файлов программы,
								//если её ещё нет.
	readChainRestaurantsFromDirs();
	
	if (role == MANAGER)
	{
		dlg = IDD_CHOICE_RESTAURANT_MANAGER;
		DialogBoxParam(0,
		MAKEINTRESOURCE(IDD_CHOICE_RESTAURANT_MANAGER),
		0,
		ChainRestaurants::DlgProc,
		MAKELPARAM(role, 0));
	}
	else if (role == WAITER)
	{
		dlg = IDD_CHOICE_RESTAURANT_WAITER;
		DialogBoxParam(0,
		MAKEINTRESOURCE(IDD_CHOICE_RESTAURANT_WAITER),
		0,
		ChainRestaurants::DlgProc,
		MAKELPARAM(role, 0));
	}
	for_each(chain_.begin(),
		chain_.end(),
		removeRestaurant());
}

void ChainRestaurants::Cls_OnClose(HWND hwnd)
{
	EndDialog(hwnd, 0);
}

void ChainRestaurants::Cls_OnInitDialog(HWND hwnd, LPARAM lp)
{
	role = (Roles)LOWORD(lp);
	if (role == MANAGER)
		dlg = IDD_MANAGER;
	else if (role == WAITER)
		dlg = IDD_WAITER;
	hChainDlg = hwnd;
	hListRestaurants = GetDlgItem(hChainDlg, IDC_COMBO_RESTAURANTS);

	if (!chain_.empty())
	{
		int count(SendMessage(hListRestaurants, CB_GETCOUNT, 0, 0));
		if (count > 0)
			for (int i(0); i<count; i++)
				SendMessage(hListRestaurants, CB_DELETESTRING, 0, 0);
		
		string kindOfRestaurant;
		TCHAR szKindOfRestaurant[100];
		for (Restaurant* r : chain_)
		{
			kindOfRestaurant = typeid(*r).name();
			lstrcpy(szKindOfRestaurant, r->getName().c_str());
			if (kindOfRestaurant == "class RestaurantVip")
				lstrcat(szKindOfRestaurant, L" - Vip-ресторан");
			else if (kindOfRestaurant == "class RestaurantStandart")
				lstrcat(szKindOfRestaurant, L" - стандартный ресторан");
			else if (kindOfRestaurant == "class RestaurantFastFood")
				lstrcat(szKindOfRestaurant, L" - ресторан-закусочная");
			SendMessage(hListRestaurants, CB_ADDSTRING, 0, LPARAM(szKindOfRestaurant));
		}
		SendMessage(hListRestaurants, CB_SETCURSEL, 0, 0);
	}
}

void ChainRestaurants::Cls_OnCommand(HWND hwnd, WPARAM fact)
{
	if (LOWORD(fact) == IDCANCEL)
	{
		EndDialog(hwnd, 0);
	}

	if (LOWORD(fact) == IDOK)
	{
		EndDialog(hwnd, 0);
		int index = SendMessage(hListRestaurants, CB_GETCURSEL, 0, 0);

		string kindOfRestaurant = typeid(*chain_[index]).name();

		//if (kindOfRestaurant == "class RestaurantVip")
		//{
		//	RestaurantVipDlg tmp;
		//	tmp = chain_[index];
		//	*chain_[index] = callDialog(tmp);
		//}
		//else 
		if (kindOfRestaurant == "class RestaurantStandart")
		{
			RestaurantStandartDlg tmp;
			tmp = chain_[index];
			*chain_[index] = callDialog(tmp);
		}
		//else if (kindOfRestaurant == "class RestaurantFastFood")
		//{
		//	RestaurantFastFoodDlg tmp;
		//	tmp = chain_[index];
		//	*chain_[index] = callDialog(tmp);
		//}

		chain_[index]->saveStatus();

	}

	if (LOWORD(fact) == IDC_SET_NEW_RESTAURANT)
	{
		NewRestaurantDlg NewRest;
		DialogBox(GetModuleHandle(0),
			MAKEINTRESOURCE(IDD_SET_NEW_RESTAURANT),
			hwnd,
			NewRestaurantDlg::DlgProc);
		if (lstrcmp(NewRest.getName(), L"N/A"))
		{
			setNewRestaurant(NewRest.getName(), NewRest.getType());
			SendMessage(hChainDlg, WM_INITDIALOG, 0, 0);
		}
	}
}

template<typename T>
T& ChainRestaurants::callDialog(T& obj)
{
	DialogBoxParam(0,
		MAKEINTRESOURCE(dlg),
		0,
		T::DlgProc,
		MAKELPARAM(role, 0));
	return obj;
}

BOOL CALLBACK ChainRestaurants::DlgProc(HWND hwnd, UINT message,
	WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_CLOSE:
		ptr->Cls_OnClose(hwnd);
		return TRUE;
	case WM_INITDIALOG:
		ptr->Cls_OnInitDialog(hwnd, lParam);
		return TRUE;
	case WM_COMMAND:
		ptr->Cls_OnCommand(hwnd, wParam);
		return TRUE;
	}
	return FALSE;
}