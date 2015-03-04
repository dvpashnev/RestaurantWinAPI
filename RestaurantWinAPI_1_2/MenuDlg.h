#pragma once

class MenuDlg
	: public Menu
{
public:
	MenuDlg();
	MenuDlg(DishContainer&);
	~MenuDlg();
	static BOOL CALLBACK DlgProc(HWND hWnd, UINT mes, WPARAM wp, LPARAM lp);
	static MenuDlg *ptr;

	static wstring currentName;
	static int currentType[3];
	static TimeDay currentTimeDay;
	static int currentSort[2];
	static Roles role;
	
	static time_t t;
	static struct tm DateTimeEnd;

	Menu refreshListDish();
	void refreshListViewItems();
	int getIndexItemInMenu();

	void Cls_OnInitDialog(HWND hwnd, LPARAM);
	void Cls_OnCommand(HWND hwnd, WPARAM);
	void Cls_OnClose(HWND hwnd);
	void Cls_OnContextMenu(HWND hwnd, LPARAM, WPARAM);
	void Cls_OnNotify(HWND hwnd, LPARAM);
	void Cls_OnTimer(HWND hwnd, WPARAM);

private:

	HWND hMenuDlg, hFindTitle, hColds, hHots, hDeserts,
		hAllDay, hMorning, hAfternoon, hEvening, hNumberDish,
		hDishesInType, hStatus;
	HMENU hContextMenu;

};