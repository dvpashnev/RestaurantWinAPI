#pragma once

class DishDlg :
	public Dish
{
public:

	DishDlg();
	DishDlg(const DishContainer& menu);
	DishDlg(const Dish& obj);
	~DishDlg();

	static BOOL CALLBACK DlgProc(HWND hWnd, UINT mes, WPARAM wp, LPARAM lp);
	static DishDlg *ptr;
	static COLORREF textColor;

	static time_t t;
	static struct tm DateTimeEnd;

	bool changeDish;

	void Cls_OnInitDialog(HWND hwnd, LPARAM);
	void Cls_OnCommand(HWND hwnd, WPARAM);
	void Cls_OnVscroll(HWND hwnd, WPARAM, LPARAM);
	void Cls_OnClose(HWND hwnd);
	void Cls_OnTimer(HWND hwnd, WPARAM);
	DWORD Cls_OnCtlColorStatic(HWND hwnd, WPARAM, LPARAM);

private:

	Menu menuForDishDlg;
	HWND hDishDlg, hIsDish, hNameDish, hPriceDish,
		hPortionDish, hTypeDish, hTimeDish, hShowType, hShowTime,
		hDescriptionDish, hPictureDish, hStatus;
};

