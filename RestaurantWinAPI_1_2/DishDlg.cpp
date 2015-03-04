#include"header.h"

DishDlg* DishDlg::ptr = NULL;

time_t DishDlg::t(time(NULL));
struct tm DishDlg::DateTimeEnd;
COLORREF DishDlg::textColor(RGB(0, 0, 0));

DishDlg::DishDlg():
Dish(),
menuForDishDlg(),
changeDish(FALSE)
{
	ptr = this;
}

DishDlg::DishDlg(const DishContainer& menu) :
Dish(),
menuForDishDlg(menu),
changeDish(FALSE)
{
	ptr = this;
}

DishDlg::DishDlg(const Dish& dish) :
Dish(dish),
menuForDishDlg(),
changeDish(FALSE)
{
	ptr = this;
}

DishDlg::~DishDlg()
{
}

void DishDlg::Cls_OnClose(HWND hwnd)
{
	EndDialog(hwnd, 0);
}

void DishDlg::Cls_OnInitDialog(HWND hwnd, LPARAM flag)
{
	hDishDlg = hwnd;

	if (LOWORD(flag) == 1)//Редактирование
	{
		hIsDish = GetDlgItem(hDishDlg, IDC_IS_DISH);

		SendDlgItemMessage(hDishDlg, IDC_SPIN_COST, UDM_SETRANGE32, 0, 1000);
		UDACCEL pAccelPrice[5] = { { 1, 1 }, { 3, 10 }, { 6, 100 } };
		SendDlgItemMessage(hDishDlg, IDC_SPIN_COST, UDM_SETACCEL, 3, LPARAM(pAccelPrice));
		SendDlgItemMessage(hDishDlg, IDC_SPIN_COST,
			UDM_SETPOS32, 0, LPARAM(price_ * 100));

		SendDlgItemMessage(hDishDlg, IDC_SPIN_PORTION, UDM_SETRANGE32, 0, 1000);
		UDACCEL pAccelPortion[2] = { { 1, 1 }, { 2, 10 } };
		SendDlgItemMessage(hDishDlg, IDC_SPIN_PORTION, UDM_SETACCEL, 2, LPARAM(pAccelPortion));
		SendDlgItemMessage(hDishDlg, IDC_SPIN_PORTION, UDM_SETPOS32, 0, portion_);

		hTypeDish = GetDlgItem(hDishDlg, IDC_COMBO_TYPE);
		SendMessage(hTypeDish, CB_ADDSTRING, 0, LPARAM(TEXT("Холодное блюдо")));
		SendMessage(hTypeDish, CB_ADDSTRING, 0, LPARAM(TEXT("Горячее блюдо")));
		SendMessage(hTypeDish, CB_ADDSTRING, 0, LPARAM(TEXT("Десерт")));
		switch (type_)
		{
		case COLD :
			SendMessage(hTypeDish, CB_SETCURSEL, 0, 0);
			break;
		case HOT :
			SendMessage(hTypeDish, CB_SETCURSEL, 1, 0);
			break;
		case DESERT :
			SendMessage(hTypeDish, CB_SETCURSEL, 2, 0);
			break;
		}

		hTimeDish = GetDlgItem(hDishDlg, IDC_COMBO_TIME);
		SendMessage(hTimeDish, CB_ADDSTRING, 0, LPARAM(TEXT("Любое время")));
		SendMessage(hTimeDish, CB_ADDSTRING, 0, LPARAM(TEXT("Утро")));
		SendMessage(hTimeDish, CB_ADDSTRING, 0, LPARAM(TEXT("День")));
		SendMessage(hTimeDish, CB_ADDSTRING, 0, LPARAM(TEXT("Вечер")));
		switch (timeDay_)
		{
		case ALL :
			SendMessage(hTimeDish, CB_SETCURSEL, 0, 0);
			break;
		case MORNING :
			SendMessage(hTimeDish, CB_SETCURSEL, 1, 0);
			break;
		case AFTERNOON :
			SendMessage(hTimeDish, CB_SETCURSEL, 2, 0);
			break;
		case EVENING :
			SendMessage(hTimeDish, CB_SETCURSEL, 3, 0);
			break;
		}
	}
	else if (LOWORD(flag) == 0)//Просмотр
	{
		hShowType = GetDlgItem(hDishDlg, IDC_SHOW_TYPE);
		switch (type_)
		{
		case COLD:
			SetWindowText(hShowType, TEXT("Холодное блюдо"));
			break;
		case HOT:
			SetWindowText(hShowType, TEXT("Горячее блюдо"));
			break;
		case DESERT:
			SetWindowText(hShowType, TEXT("Десерт"));
			break;
		}

		hShowTime = GetDlgItem(hDishDlg, IDC_SHOW_TIME);
		switch (timeDay_)
		{
		case ALL:
			SetWindowText(hShowTime, TEXT("Любое время"));
			break;
		case MORNING:
			SetWindowText(hShowTime, TEXT("Утро"));
			break;
		case AFTERNOON:
			SetWindowText(hShowTime, TEXT("День"));
			break;
		case EVENING:
			SetWindowText(hShowTime, TEXT("Вечер"));
			break;
		}

		if (HIWORD(flag) == WAITER)
		{
			EnableWindow(GetDlgItem(hDishDlg, ID_EDIT_DISH), 0);
			ShowWindow(GetDlgItem(hDishDlg, ID_EDIT_DISH), SW_HIDE);
		}
	}

	//Общее

	hNameDish = GetDlgItem(hDishDlg, IDC_NAME_DISH);
	SetWindowText(hNameDish, title_.c_str());

	hPriceDish = GetDlgItem(hDishDlg, IDC_DISH_PRICE);
	TCHAR buf[20];
	swprintf_s(buf, 20, L"%.2lf", price_);
	SetWindowText(hPriceDish, buf);

	hPortionDish = GetDlgItem(hDishDlg, IDC_DISH_PORTION);
	swprintf_s(buf, 20, L"%d", portion_);
	SetWindowText(hPortionDish, buf);

	hDescriptionDish = GetDlgItem(hDishDlg, IDC_DISH_DESCRIPTION);
	SetWindowText(hDescriptionDish, description_.c_str());

	hPictureDish = GetDlgItem(hDishDlg, IDC_IMAGE_DISH);
	HANDLE hBitmap = LoadImage(NULL, picPath_.c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	SendMessage(hPictureDish, STM_SETIMAGE, IMAGE_BITMAP, LPARAM(hBitmap));

	hStatus = CreateStatusWindow(WS_CHILD | WS_VISIBLE | CCS_BOTTOM |
		SBARS_TOOLTIPS | SBARS_SIZEGRIP, 0, hDishDlg, WM_USER);
	int parts[3] = { 200, -1 };//-1 правая граница
	SendMessage(hStatus, SB_SETPARTS, 2, (LPARAM)parts);
	SetTimer(hwnd, 1, 1000, 0);
}

void DishDlg::Cls_OnCommand(HWND hwnd, WPARAM fact)
{
	if (LOWORD(fact) == IDC_SAVE_DISH)
	{
		TCHAR err[50] = { 0 };
		int length = SendMessage(hNameDish, WM_GETTEXTLENGTH, 0, 0);
		if (length > 50)
		{
			lstrcpy(err, L"Название блюда не может быть больше 50 символов!");
			MessageBox(hDishDlg, err, L"Не все поля заполнены правильно", MB_OK | MB_ICONERROR | MB_APPLMODAL);
			SetFocus(hNameDish);
			return;
		}

		TCHAR buf[50];
		GetWindowText(hNameDish, buf, 50);
		wstring title = buf;
		if (menuForDishDlg.findFullTitle(title) >= 0)
		{
			lstrcpy(err, L"Блюдо с аналогичным названием уже существует!\nВведите оригинальное название!");
			MessageBox(hDishDlg, err, L"Не все поля заполнены правильно", MB_OK | MB_ICONERROR | MB_APPLMODAL);
			SetFocus(hNameDish);
			return;
		}
		title_ = title;

		GetWindowText(hPriceDish, buf, 10);
		double price = _wtof(buf);
		if (price == 0.)
		{
			lstrcpy(err, L"Цена не может быть равна 0!");
			MessageBox(hDishDlg, err, L"Не все поля заполнены правильно", MB_OK | MB_ICONERROR | MB_APPLMODAL);
			SetFocus(hPriceDish);
			return;
		}
		price_ = price;

		GetWindowText(hPortionDish, buf, 10);
		int portion = _wtoi(buf);
		if (portion == 0)
		{
			lstrcpy(err, L"Порция не может быть равна 0!");
			MessageBox(hDishDlg, err, L"Не все поля заполнены правильно", MB_OK | MB_ICONERROR | MB_APPLMODAL);
			SetFocus(hPortionDish);
			return;
		}
		portion_ = portion;

		type_ = (TypeDish)(SendMessage(hTypeDish, CB_GETCURSEL, 0, 0));
		timeDay_ = (TimeDay)(SendMessage(hTimeDish, CB_GETCURSEL, 0, 0));

		length = SendMessage(hDescriptionDish, WM_GETTEXTLENGTH, 0, 0);
		TCHAR* bufDescr = new TCHAR[length + 1];
		memset(bufDescr, 0, length + 1);
		GetWindowText(hDescriptionDish, bufDescr, length + 1);
		if (!lstrcmp(bufDescr, L"N/A"))
		{
			int result = MessageBox(hDishDlg,
				L"Вы не ввели описание блюда!\nСохранить без описания?",
				L"Не все поля заполнены",
				MB_YESNO | MB_ICONERROR | MB_APPLMODAL);
			if (result == IDNO)
			{
				lstrcpy(err, L"");
				SetFocus(hDescriptionDish);
				return;
			}
		}
		wstring wBufDescr = bufDescr;
		for (size_t i(0); i < wBufDescr.size(); i++)
			if (wBufDescr[i] == '\n' || wBufDescr[i] == '\r')
				wBufDescr[i] = ' ';
		description_ = wBufDescr;
		delete bufDescr;

		if (picPath_ == L"N/A")
		{
			int result = MessageBox(hDishDlg,
				L"Вы не загрузили изображение блюда!\nСохранить без изображения?",
				L"Не все поля заполнены",
				MB_YESNO | MB_ICONERROR | MB_APPLMODAL);
			if (result == IDNO)
			{
				lstrcpy(err, L"");
				SetFocus(GetDlgItem(hDishDlg, IDC_DOWNLOAD_IMAGE));
				return;
			}
		}

		EndDialog(hwnd, 0);
	}

	if (LOWORD(fact) == IDC_NAME_DISH &&
		HIWORD(fact) == EN_UPDATE)
	{
		TCHAR nameDish[50];
		GetWindowText(hNameDish, nameDish, 50);
		wstring title = nameDish;
		if (title.size() > 50)
		{
			textColor = RGB(255, 0, 0);
			SetWindowText(hIsDish, L"Название блюда не может быть больше 50 символов!");
			InvalidateRect(hIsDish, 0, TRUE);
		}
		else if (menuForDishDlg.findFullTitle(title) >= 0)
		{
			textColor = RGB(255, 0, 0);
			SetWindowText(hIsDish, L"Блюдо с аналогичным названием существует");
			InvalidateRect(hIsDish, 0, TRUE);
		}
		else if (title == L"N/A" || title.empty())
		{
			textColor = RGB(255, 0, 0);
			SetWindowText(hIsDish, L"Введите название блюда");
			InvalidateRect(hIsDish, 0, TRUE);
		}
		else
		{
			textColor = RGB(0, 255, 0);
			if (lstrlen(nameDish) != 0)
				SetWindowText(hIsDish, L"Название блюда возможно");
			InvalidateRect(hIsDish, NULL, TRUE);
		}
		return;
	}

	if (LOWORD(fact) == IDC_DOWNLOAD_IMAGE)
	{
		TCHAR FullPath[MAX_PATH] = { 0 };
		TCHAR fName[MAX_PATH] = { 0 };

		OPENFILENAME open = { sizeof(OPENFILENAME) };
		open.hwndOwner = hwnd;
		open.lpstrFilter = TEXT("Картинки(*.bmp)\0*.bmp\0");
		open.lpstrFile = FullPath;
		open.nMaxFile = MAX_PATH;
		open.lpstrFileTitle = fName;
		open.nMaxFileTitle = MAX_PATH;
		open.Flags = OFN_FILEMUSTEXIST;
		wchar_t* currPath = new wchar_t[MAX_PATH];
		_wgetcwd(currPath, MAX_PATH);
		if (GetOpenFileName(&open))
		{
			picPath_ = L"images\\" + (wstring)fName;
			TCHAR* newName = new TCHAR[lstrlen(currPath) + picPath_.size() + 1];
			lstrcpy(newName, currPath);
			lstrcat(newName, L"\\images\\");
			lstrcat(newName, fName);
			CopyFile(FullPath, newName, FALSE);
			HANDLE hBitmap = LoadImage(NULL, FullPath,
				IMAGE_BITMAP, 0, 0,
				LR_LOADFROMFILE);
			SendMessage(hPictureDish, STM_SETIMAGE, IMAGE_BITMAP, LPARAM(hBitmap));
		}
		_wchdir(currPath);
		delete[] currPath;
		return;
	}

	if (LOWORD(fact) == ID_EDIT_DISH)
	{
		changeDish = TRUE;
		EndDialog(hwnd, 0);
	}

	if (LOWORD(fact) == IDC_DISH_CANCEL)
	{
		EndDialog(hwnd, 0);
	}
}

void DishDlg::Cls_OnVscroll(HWND hwnd, WPARAM fact, LPARAM id)
{
	if (GetDlgCtrlID((HWND)id) == IDC_SPIN_COST	&&
		LOWORD(fact) == TB_THUMBPOSITION)
	{
		TCHAR buf[20];
		GetWindowText(hPriceDish, buf, 20);
		int pos = SendDlgItemMessage(hDishDlg, IDC_SPIN_COST,
			UDM_GETPOS32, 0, 0);
		swprintf_s(buf, 20, L"%.2lf", (double)pos / (double)100);
		SetWindowText(hPriceDish, buf);
	}
}

void DishDlg::Cls_OnTimer(HWND hwnd, WPARAM idTimer)
{
	if (idTimer == 1)
	{
		time_t CurrentTime(time(NULL));
		localtime_s(&DateTimeEnd, &CurrentTime);
		TCHAR str[100];
		_tcsftime(str, 100, TEXT("%H:%M:%S, %d.%m.%Y"), &DateTimeEnd);
		SendMessage(hStatus, SB_SETTEXT, 0, (LPARAM)str);
	}
}

DWORD DishDlg::Cls_OnCtlColorStatic(HWND hwnd, WPARAM idStatic, LPARAM hStatic)
{
	if ((HWND)hStatic == hIsDish)
	{
		SetTextColor((HDC)idStatic, textColor);
		SetBkMode((HDC)idStatic, TRANSPARENT);
		return (DWORD)GetSysColorBrush(COLOR_3DFACE);
	}
	return (DWORD)GetSysColorBrush(COLOR_3DFACE);
}


BOOL CALLBACK DishDlg::DlgProc(HWND hwnd, UINT message,
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
	case WM_VSCROLL:
		ptr->Cls_OnVscroll(hwnd, wParam, lParam);
		return TRUE;
	case WM_TIMER:
		ptr->Cls_OnTimer(hwnd, wParam);
		return TRUE;
	case WM_CTLCOLORSTATIC:
		ptr->Cls_OnCtlColorStatic(hwnd, wParam, lParam);
		return TRUE;
	}
	return FALSE;
}