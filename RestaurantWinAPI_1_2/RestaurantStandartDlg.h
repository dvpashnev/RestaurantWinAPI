#pragma once
using namespace std;

class RestaurantStandartDlg:
	public RestaurantStandart
{
public:
	RestaurantStandartDlg();
	~RestaurantStandartDlg();

	RestaurantStandartDlg& operator=(Restaurant*&);	//Оператор присваивания

	static BOOL CALLBACK DlgProc(HWND hWnd, UINT mes, WPARAM wp, LPARAM lp);
	static RestaurantStandartDlg* ptr;

	static BOOL flicker;
	static time_t t;
	static struct tm DateTimeEnd;

	size_t nTable;

	void refreshDlgManager();
	void refreshDlgWaiter();
	void checkStatus();


	void Cls_OnInitDialog(HWND hwnd, LPARAM);
	void Cls_OnSize(HWND hwnd);
	void Cls_OnContextMenu(HWND hwnd, LPARAM, WPARAM);
	void Cls_OnVscroll(HWND hwnd, WPARAM);
	void Cls_OnCommand(HWND hwnd, WPARAM);
	void Cls_OnNotify(HWND hwnd, LPARAM);
	void Cls_OnClose(HWND hwnd);
	void Cls_OnTimer(HWND hwnd, WPARAM);

private:

	HWND hRestaurantStandartDlg,
		hNDishesInMenu, hNColdDishesInMenu, hNHotDishesInMenu, hNDesertsInMenu,//Дескрипторы для менеджера
		hNMorningDishesInMenu, hNAfternoonDishesInMenu, hNEveningInMenu,
		hNTables, hNTablesFree, 
		hNOpenOrders, hNOpenOrdersTotals,
		hNClosedOrders, hNClosedOrdersTotals,
		hFindNOrder, hBtnFind, hListOrders,			//Дескрипторы для официанта
		hStatus, hAlarm;
	HMENU  hContextMenu;
	HIMAGELIST himlLarge;
	//Roles role;
};