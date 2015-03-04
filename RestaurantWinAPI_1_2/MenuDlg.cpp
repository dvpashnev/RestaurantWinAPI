#include "header.h"

MenuDlg* MenuDlg::ptr = NULL;
wstring MenuDlg::currentName(L"");
int MenuDlg::currentType[3] = { 1, 1, 1 };
TimeDay MenuDlg::currentTimeDay(ALL);
int MenuDlg::currentSort[2] = { 0, 0 };
Roles MenuDlg::role(MANAGER);

time_t MenuDlg::t(time(NULL));
struct tm MenuDlg::DateTimeEnd;

MenuDlg::MenuDlg()
	:
	Menu()
{
	ptr = this;
}

MenuDlg::MenuDlg(DishContainer& obj)
	:
	Menu(obj)
	{
	ptr = this;
	}

MenuDlg::~MenuDlg()
{
}

void MenuDlg::Cls_OnClose(HWND hwnd)
{
	EndDialog(hwnd, 0);
}

Menu MenuDlg::refreshListDish()
{
	switch (currentSort[0])
	{
	case 0:
		if (currentSort[1] == 0)
		{
			sort(menu_.begin(), menu_.end());
		}
		else
		{
			sort(menu_.begin(), menu_.end(), greater<Dish>());
		}
		break;
	case 1:
		if (currentSort[1] == 0)
		{
			sort(menu_.begin(), menu_.end(), byPriceIncrease());
		}
		else
		{
			sort(menu_.begin(), menu_.end(), byPriceDecrease());
		}
		break;
	case 2:
		if (currentSort[1] == 0)
		{
			sort(menu_.begin(), menu_.end(), byTypeIncrease());
		}
		else
		{
			sort(menu_.begin(), menu_.end(), byTypeDecrease());
		}
		break;
	case 3:
		if (currentSort[1] == 0)
		{
			sort(menu_.begin(), menu_.end(), byTimeIncrease());
		}
		else
		{
			sort(menu_.begin(), menu_.end(), byTimeDecrease());
		}
		break;
	}

	Menu proMenu(menu_);
	vecindex findRezult;

	if (!currentName.empty())
	{
		findRezult = findTitle(currentName);
		proMenu = vectorToMenu(findRezult);
	}
	if (currentTimeDay != ALL)
	{
		findRezult = proMenu.findTime(currentTimeDay);
		proMenu = vectorToMenu(findRezult);
	}
	findRezult = proMenu.findType(currentType);
	proMenu = vectorToMenu(findRezult);
	return proMenu;
}

void MenuDlg::refreshListViewItems()
{
	ListView_DeleteAllItems(hDishesInType);

	LV_ITEM lvi;
	memset(&lvi, 0, sizeof(lvi));
	lvi.mask = LVIF_TEXT | LVCF_SUBITEM | LVIF_PARAM;
	lvi.pszText = LPSTR_TEXTCALLBACK;

	Menu proMenu = refreshListDish();

	for (size_t i(0); i < proMenu.getMenu_().size(); i++)
	{
		if (proMenu.getMenu_()[i].getTitle() != L"N/A")
		{
			lvi.iItem = i;
			lvi.iSubItem = 0;	//Название
			lvi.cchTextMax = 50;
			lvi.pszText = LPSTR_TEXTCALLBACK;
			lvi.lParam = (LPARAM)&menu_[i];
			ListView_InsertItem(hDishesInType, &lvi);

			lvi.iItem = i;
			lvi.iSubItem = 1;	//Цена
			ListView_InsertItem(hDishesInType, &lvi);

			lvi.iItem = i;
			lvi.iSubItem = 2;	//Порция
			ListView_InsertItem(hDishesInType, &lvi);

			lvi.iItem = i;
			lvi.iSubItem = 3;	//Тип
			ListView_InsertItem(hDishesInType, &lvi);

			lvi.iItem = i;
			lvi.iSubItem = 4;	//Время
			ListView_InsertItem(hDishesInType, &lvi);
		}
	}
	ListView_SetSelectionMark(hDishesInType, 0);
}

int MenuDlg::getIndexItemInMenu()
{
	int indexSel = ListView_GetSelectionMark(hDishesInType);
	LV_ITEM lvi;
	memset(&lvi, 0, sizeof(lvi));
	lvi.mask = LVIF_TEXT | LVIF_PARAM;
	lvi.iItem = indexSel;
	ListView_GetItem(hDishesInType, &lvi);
	Dish* pro = (Dish *)lvi.lParam;
	int ind = findFullTitle(pro->getTitle());
	return ind;
}

void MenuDlg::Cls_OnInitDialog(HWND hwnd, LPARAM lp)
{
	role = (Roles)lp;

	hMenuDlg = hwnd;
	hFindTitle = GetDlgItem(hMenuDlg, IDC_EDIT_FIND);
	
	SendDlgItemMessage(hMenuDlg, IDC_CHECK_COLD, BM_SETCHECK, WPARAM(BST_CHECKED), 0);
	SendDlgItemMessage(hMenuDlg, IDC_CHECK_HOT, BM_SETCHECK, WPARAM(BST_CHECKED), 0);
	SendDlgItemMessage(hMenuDlg, IDC_CHECK_DESERTS, BM_SETCHECK, WPARAM(BST_CHECKED), 0);

	CheckRadioButton(hMenuDlg, IDC_RADIO_ALLTIME, IDC_RADIO_EVENING, IDC_RADIO_ALLTIME);
	CheckRadioButton(hMenuDlg, IDC_SORT_TITLE, IDC_SORT_TIME, IDC_SORT_TITLE);
	SendDlgItemMessage(hMenuDlg, IDC_SORT_DOWN, BM_SETCHECK, WPARAM(BST_UNCHECKED), 0);

	RECT rc;
	GetClientRect(hMenuDlg, &rc);
	hDishesInType = CreateWindowEx(0L, WC_LISTVIEW, L"",
		WS_VISIBLE | WS_CHILD | WS_BORDER | LVS_REPORT | LVS_SHOWSELALWAYS,
		8, rc.top + 100, rc.right - rc.left - 170, rc.bottom - rc.top - 123,
		hMenuDlg, 0, 0, NULL);

	LV_COLUMN lvc;
	memset(&lvc, 0, sizeof(lvc));
	lvc.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	lvc.fmt = LVCFMT_LEFT;
	lvc.cx = (rc.right - rc.left - 170) / 3;

	lvc.iSubItem = 0;
	lvc.pszText = L"Название";
	ListView_InsertColumn(hDishesInType, 0, &lvc);

	lvc.iSubItem = 1;
	lvc.pszText = L"Цена, грн";
	ListView_InsertColumn(hDishesInType, 1, &lvc);
	ListView_SetColumnWidth(hDishesInType, 1, (rc.right - rc.left) / 12);

	lvc.iSubItem = 2;
	lvc.pszText = L"Порция, гр";
	ListView_InsertColumn(hDishesInType, 2, &lvc);
	ListView_SetColumnWidth(hDishesInType, 2, (rc.right - rc.left) / 12);

	lvc.iSubItem = 3;
	lvc.pszText = L"Тип";
	ListView_InsertColumn(hDishesInType, 3, &lvc);
	ListView_SetColumnWidth(hDishesInType, 3, (rc.right - rc.left) / 6);

	lvc.iSubItem = 4;
	lvc.pszText = L"Время";
	ListView_InsertColumn(hDishesInType, 4, &lvc);
	ListView_SetColumnWidth(hDishesInType, 4, (rc.right - rc.left) / 6);

	LV_ITEM lvi;
	memset(&lvi, 0, sizeof(lvi));
	lvi.mask = LVIF_TEXT | LVCF_SUBITEM | LVIF_PARAM;
	lvi.pszText = LPSTR_TEXTCALLBACK;

	Menu proMenu = refreshListDish();

	for (size_t i(0); i < proMenu.getMenu_().size(); i++)
	{
		if (proMenu.getMenu_()[i].getTitle() != L"N/A")
		{
		lvi.iItem = i;
		lvi.iSubItem = 0;	//Название
		lvi.cchTextMax = 50; 
		lvi.pszText = LPSTR_TEXTCALLBACK;
		lvi.lParam = (LPARAM)&menu_[i];
		ListView_InsertItem(hDishesInType, &lvi);

		lvi.iItem = i;
		lvi.iSubItem = 1;	//Цена
		ListView_InsertItem(hDishesInType, &lvi);

		lvi.iItem = i;
		lvi.iSubItem = 2;	//Порция
		ListView_InsertItem(hDishesInType, &lvi);

		lvi.iItem = i;
		lvi.iSubItem = 3;	//Тип
		ListView_InsertItem(hDishesInType, &lvi);

		lvi.iItem = i;
		lvi.iSubItem = 4;	//Время
		ListView_InsertItem(hDishesInType, &lvi);
		}
	}

	ListView_SetSelectionMark(hDishesInType, 0);
		
	hContextMenu = GetSubMenu(LoadMenu(GetModuleHandle(0),
		MAKEINTRESOURCE(IDR_MENU_CONTEXT)), 0);

	hStatus = CreateStatusWindow(WS_CHILD | WS_VISIBLE | CCS_BOTTOM | SBARS_TOOLTIPS |
		SBARS_SIZEGRIP, 0, hMenuDlg, WM_USER);
	int parts[3] = { 200, -1 };//-1 правая граница
	SendMessage(hStatus, SB_SETPARTS, 2, (LPARAM)parts);
	SetTimer(hwnd, 1, 1000, 0);

	if (role == WAITER)
	{
		RECT rc;
		GetWindowRect(hMenuDlg, &rc);
		MoveWindow(hMenuDlg, 100, 100, rc.right - rc.left, rc.bottom - rc.top, 1);
		DeleteMenu(hContextMenu, ID_EDIT_DISH, MF_BYCOMMAND);
		InsertMenu(hContextMenu, IDC_DEL_DISH, 
			MF_BYCOMMAND, IDC_ADD_TO_ORDER, L"Добавить в заказ");
		DeleteMenu(hContextMenu, IDC_DEL_DISH, MF_BYCOMMAND);
		DeleteMenu(hContextMenu, IDC_NEW_DISH, MF_BYCOMMAND);
		DeleteMenu(hContextMenu, 3, MF_BYPOSITION);

		hNumberDish = GetDlgItem(hMenuDlg, IDC_NUMBER_DISH);

		SendDlgItemMessage(hMenuDlg, IDC_SPIN_NUMBER_DISH, UDM_SETRANGE32, 1, 1000);
		UDACCEL pAccelPortion[2] = { { 1, 1 }, { 2, 10 } };
		SendDlgItemMessage(hMenuDlg, IDC_SPIN_NUMBER_DISH, UDM_SETACCEL, 2, LPARAM(pAccelPortion));
		SendDlgItemMessage(hMenuDlg, IDC_SPIN_NUMBER_DISH, UDM_SETPOS32, 0, 1);
		SetWindowText(GetDlgItem(hMenuDlg, IDC_EXIT_MENU), L"Закончить");
	}
}

void MenuDlg::Cls_OnCommand(HWND hwnd, WPARAM fact)
{
	if (LOWORD(fact) == IDC_EXIT_MENU ||
		LOWORD(fact) == IDCANCEL)
	{
		EndDialog(hwnd, 0);
	}

	if (LOWORD(fact) == IDC_EDIT_FIND &&
		HIWORD(fact) == EN_CHANGE)
	{
		TCHAR query[100];
		GetWindowText(hFindTitle, query, 100);
		currentName = query;
		refreshListViewItems();
		return;
	}
		
	if ((LOWORD(fact) == IDC_CHECK_COLD ||
		LOWORD(fact) == IDC_CHECK_HOT ||
		LOWORD(fact) == IDC_CHECK_DESERTS) &&
		HIWORD(fact) == BN_CLICKED)
	{
		switch LOWORD(fact)
		{
		case IDC_CHECK_COLD:
		{
			int ti = SendDlgItemMessage(hwnd, IDC_CHECK_COLD,
				BM_GETCHECK, 0, 0);
			if (ti == BST_CHECKED)
				currentType[0] = 1;
			else
				currentType[0] = 0;
		}
			break;
		case IDC_CHECK_HOT:
		{
			int ti = SendDlgItemMessage(hwnd, IDC_CHECK_HOT,
				BM_GETCHECK, 0, 0);
			if (ti == BST_CHECKED)
				currentType[1] = 1;
			else
				currentType[1] = 0;
		}
			break;
		case IDC_CHECK_DESERTS:
		{
			int ti = SendDlgItemMessage(hwnd, IDC_CHECK_DESERTS,
				BM_GETCHECK, 0, 0);
			if (ti == BST_CHECKED)
				currentType[2] = 1;
			else
				currentType[2] = 0;
		}
			break;
		}
		refreshListViewItems();
		return;
	}

	if ((LOWORD(fact) == IDC_RADIO_ALLTIME ||
		LOWORD(fact) ==  IDC_RADIO_MORNING ||
		LOWORD(fact) == IDC_RADIO_AFTERNOON ||
		LOWORD(fact) == IDC_RADIO_EVENING) &&
		HIWORD(fact) == BN_CLICKED)
	{
		switch LOWORD(fact)
		{
		case IDC_RADIO_ALLTIME:
			currentTimeDay = ALL;
			CheckRadioButton(hMenuDlg, IDC_RADIO_ALLTIME, IDC_RADIO_EVENING, IDC_RADIO_ALLTIME);
			break;
		case IDC_RADIO_MORNING:
			currentTimeDay = MORNING;
			CheckRadioButton(hMenuDlg, IDC_RADIO_ALLTIME, IDC_RADIO_EVENING, IDC_RADIO_MORNING);
			break;
		case IDC_RADIO_AFTERNOON:
			currentTimeDay = AFTERNOON;
			CheckRadioButton(hMenuDlg, IDC_RADIO_ALLTIME, IDC_RADIO_EVENING, IDC_RADIO_AFTERNOON);
			break;
		case IDC_RADIO_EVENING:
			currentTimeDay = EVENING;
			CheckRadioButton(hMenuDlg, IDC_RADIO_ALLTIME, IDC_RADIO_EVENING, IDC_RADIO_EVENING);
			break;
		}
		refreshListViewItems();
		return;
	}

	if ((LOWORD(fact) == IDC_SORT_TITLE ||
		LOWORD(fact) == IDC_SORT_PRICE ||
		LOWORD(fact) == IDC_SORT_TIME ||
		LOWORD(fact) == IDC_SORT_TYPE ||
		LOWORD(fact) == IDC_SORT_DOWN)&&
		HIWORD(fact) == BN_CLICKED)
	{
		switch LOWORD(fact)
		{
		case IDC_SORT_TITLE:
			currentSort[0] = 0;
			CheckRadioButton(hMenuDlg, IDC_SORT_TITLE, IDC_SORT_TIME, IDC_SORT_TITLE);
			break;
		case IDC_SORT_PRICE:
			currentSort[0] = 1;
			CheckRadioButton(hMenuDlg, IDC_SORT_TITLE, IDC_SORT_TIME, IDC_SORT_PRICE);
			break;
		case IDC_SORT_TYPE:
			currentSort[0] = 2;
			CheckRadioButton(hMenuDlg, IDC_SORT_TITLE, IDC_SORT_TIME, IDC_SORT_TYPE);
			break;
		case IDC_SORT_TIME:
			currentSort[0] = 3;
			CheckRadioButton(hMenuDlg, IDC_SORT_TITLE, IDC_SORT_TIME, IDC_SORT_TIME);
			break;
		case IDC_SORT_DOWN:
			int trendSort = SendDlgItemMessage(hMenuDlg, IDC_SORT_DOWN,
				BM_GETCHECK, 0, 0);
			if (trendSort == BST_CHECKED)
				currentSort[1] = 1;
			else
				currentSort[1] = 0;
			break;
		}
		refreshListViewItems();
		return;
	}
		
	if (LOWORD(fact) == IDC_RESET_ALL)
	{
		int length = SendDlgItemMessage(hMenuDlg, IDC_EDIT_FIND, WM_GETTEXTLENGTH, 0, 0);
		SendDlgItemMessage(hMenuDlg, IDC_EDIT_FIND, EM_SETSEL, 0, length);
		SendDlgItemMessage(hMenuDlg, IDC_EDIT_FIND, WM_CLEAR, 0, 0);
		for (int i(0);i<3;i++)
			currentType[i] = 1;
		currentTimeDay=ALL;
		for (int i(0); i<2; i++)
			currentSort[i] = 0;
		refreshListViewItems();
		return;
	}

	if (LOWORD(fact) == IDC_NUMBER_DISH &&
		HIWORD(fact) == EN_CHANGE)
	{
		TCHAR curDishN[4];
		GetWindowText(hNumberDish, curDishN, 4);
		int num = _wtoi(curDishN);
		if (num == 0)
			return;
		SendDlgItemMessage(hMenuDlg, IDC_SPIN_NUMBER_DISH,
			UDM_SETPOS32, 0, num);
		return;
	}

	if (LOWORD(fact) == IDC_ADD_TO_ORDER)
	{
		int indexSel = getIndexItemInMenu();
		TCHAR buf[5];
		GetWindowText(hNumberDish, buf, 5);
		int numberDish = _wtoi(buf);

		SendMessage(hTransfer, WM_COMMAND, 
			MAKEWPARAM(IDC_TRANSFER_DISH_IN_ORDER, 0), 
			MAKELPARAM(numberDish, indexSel));
		return;
	}
		
	if (LOWORD(fact) == IDC_NEW_DISH)
	{
		ShowWindow(hwnd, SW_HIDE);
		DishDlg tmp(menu_);
		DialogBoxParam(0,
			MAKEINTRESOURCE(IDD_EDIT_DISH),
			0,
			DishDlg::DlgProc,
			MAKELPARAM(1, 0));//1 - РЕДАКТИРОВАНИЕ
		if (tmp.getTitle() != L"N/A")
			addDish(tmp);
		refreshListViewItems();
		ShowWindow(hwnd, SW_SHOWNORMAL);
		return;
	}

	if (LOWORD(fact) == IDC_DEL_DISH)
	{
		int indexSel = getIndexItemInMenu();

		int result = MessageBox(hMenuDlg,
			TEXT("Вы уверены, что хотите удалить блюдо из меню?"),
			TEXT("Подтверждение"),
			MB_OKCANCEL | MB_ICONQUESTION | MB_TASKMODAL);
		if (result == IDOK)
		{
			delDish(indexSel);
			refreshListViewItems();
		}
		return;
	}

	if (LOWORD(fact) == ID_SHOW_DISH||
		LOWORD(fact) == IDOK)
	{
		int indexSel = getIndexItemInMenu();

		DishDlg tmp(menu_[indexSel]);
		DialogBoxParam(0,
			MAKEINTRESOURCE(IDD_SHOW_DISH),
			0,
			DishDlg::DlgProc,
			MAKELPARAM(0, role));//0 - просмотр
		if (tmp.changeDish == TRUE)
			SendMessage(hMenuDlg, WM_COMMAND, MAKEWPARAM(ID_EDIT_DISH, 0), 0);
		else
			return;
	}
	
	if (LOWORD(fact) == ID_EDIT_DISH ||
		(LOWORD(fact) == IDC_LIST_DISHES_MENU && HIWORD(fact) == LBN_DBLCLK))
	{
		int indexSel = getIndexItemInMenu();

		DishDlg tmp(menu_[indexSel]);
		DialogBoxParam(0,
			MAKEINTRESOURCE(IDD_EDIT_DISH),
			0,
			DishDlg::DlgProc,
			MAKELPARAM(1, 0));//1 - РЕДАКТИРОВАНИЕ
		if (tmp.getTitle() != menu_[indexSel].getTitle())
		{//Если было изменено название, то вставляем блюдо в место по сортировке
			menu_.erase(menu_.begin() + indexSel);
			if (menu_.begin()->getTitle() < (menu_.begin() + 1)->getTitle())//Если сортировка по алфавиту,
				menu_.insert(lower_bound(menu_.begin(), menu_.end(), tmp), tmp);//вставляем по алфавиту,
			else																		//нет -
				menu_.insert(lower_bound(menu_.begin(), menu_.end(), tmp, greater<Dish>()), tmp);//в обратном порядке
		}
		else
			menu_[indexSel] = tmp;
		refreshListViewItems();
		ListView_SetSelectionMark(hDishesInType, indexSel);
		return;
	}
}

void MenuDlg::Cls_OnContextMenu(HWND hwnd, LPARAM pos, WPARAM idItem)
{
	if ((HWND)idItem == hDishesInType)
	{
		TrackPopupMenu(hContextMenu, TPM_RIGHTBUTTON |
			TPM_TOPALIGN | TPM_LEFTALIGN, LOWORD(pos), HIWORD(pos), 0, hwnd, 0);
	}
}

void MenuDlg::Cls_OnNotify(HWND hwnd, LPARAM lp)
{
	NMHDR* pnmhdr = (NMHDR*)lp;
	if (pnmhdr->idFrom == GetDlgCtrlID(hDishesInType))
	{
		LV_DISPINFO * lpLvdi = (LV_DISPINFO *)pnmhdr;
		Dish * dish = (Dish *)(lpLvdi->item.lParam);

		NM_LISTVIEW *lpNm = (NM_LISTVIEW *)pnmhdr;

		TCHAR* buf(NULL);

		switch (pnmhdr->code)
		{
		case LVN_GETDISPINFO:
		{
			if (lpLvdi->item.mask & LVIF_TEXT)
			{
				switch (lpLvdi->item.iSubItem)
				{
				case 0:
					buf = new TCHAR[lstrlen(dish->getTitle().c_str()) + 1];
					lstrcpy(buf, dish->getTitle().c_str());
					lpLvdi->item.pszText = buf;
					break;

				case 1:
					buf = new TCHAR[lstrlen(dish->getPriceStr().c_str()) + 1];
					lstrcpy(buf, dish->getPriceStr().c_str());
					lpLvdi->item.pszText = buf;
					break;

				case 2:
					buf = new TCHAR[lstrlen(dish->getPortionStr().c_str()) + 1];
					lstrcpy(buf, dish->getPortionStr().c_str());
					lpLvdi->item.pszText = buf;
					break;

				case 3:
					buf = new TCHAR[lstrlen(dish->getTypeStr().c_str()) + 1];
					lstrcpy(buf, dish->getTypeStr().c_str());
					lpLvdi->item.pszText = buf;
					break;

				case 4:
					buf = new TCHAR[lstrlen(dish->getTimeStr().c_str()) + 1];
					lstrcpy(buf, dish->getTimeStr().c_str());
					lpLvdi->item.pszText = buf;
					break;

				default:
					break;
				}
				break;
			}
		}
		}
	}
}

void MenuDlg::Cls_OnTimer(HWND hwnd, WPARAM idTimer)
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

BOOL CALLBACK MenuDlg::DlgProc(HWND hwnd, UINT message,
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
	case WM_CONTEXTMENU :
		ptr->Cls_OnContextMenu(hwnd, lParam, wParam);
		return TRUE;
	case WM_NOTIFY:
		ptr->Cls_OnNotify(hwnd, lParam);
		return TRUE;
	case WM_TIMER:
		ptr->Cls_OnTimer(hwnd, wParam);
		return TRUE;
	}
	return FALSE;
}