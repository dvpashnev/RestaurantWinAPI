#include "header.h"

OrderDlg* OrderDlg::ptr = NULL;

time_t OrderDlg::t(time(NULL));
struct tm OrderDlg::DateTimeEnd;

OrderDlg::OrderDlg():
Order(),
menuForOrderListDlg(),
change(TRUE),
check(FALSE)
{
	ptr = this;
}

OrderDlg::OrderDlg(const Order& obj, DishContainer menu) :
Order(obj),
menuForOrderListDlg(menu),
change(TRUE),
check(FALSE)
{
	ptr = this;
}

OrderDlg::OrderDlg(DishContainer obj) :
menuForOrderListDlg(obj),
change(TRUE),
check(FALSE)
{
	ptr = this;
}

OrderDlg::~OrderDlg()
{
}

bool OrderDlg::getCheck() const
{
	return check;
}

bool OrderDlg::getChange() const
{
	return change;
}

void OrderDlg::refreshListViewItems()
{
	ListView_DeleteAllItems(hOrderList);
	
	if (!menu_.empty())
	{
		EnableWindow(GetDlgItem(hOrderDlg, IDC_DEL_DISH_ORDER), 1);

		totals_ = 0.;
		ListView_DeleteAllItems(hOrderList);

		LV_ITEM lvi;
		memset(&lvi, 0, sizeof(lvi));
		lvi.mask = LVIF_TEXT | LVCF_SUBITEM | LVIF_PARAM;
		lvi.pszText = LPSTR_TEXTCALLBACK;
	
		for (size_t i(0); i < menu_.size(); i++)
			{
				totals_ += menu_[i].getPrice()*(double)menu_[i].getNumber();
				TCHAR buf[10];
				swprintf_s(buf, 10, L"%.2lf", totals_);
				SetWindowText(hOrderTotals, buf);

				lvi.iItem = i;
				lvi.iSubItem = 0;	//Название
				lvi.cchTextMax = 50;
				lvi.pszText = LPSTR_TEXTCALLBACK;
				lvi.lParam = (LPARAM)&menu_[i];
				ListView_InsertItem(hOrderList, &lvi);

				lvi.iItem = i;
				lvi.iSubItem = 1;	//Цена
				ListView_InsertItem(hOrderList, &lvi);

				lvi.iItem = i;
				lvi.iSubItem = 2;	//Кол-во
				ListView_InsertItem(hOrderList, &lvi);

				lvi.iItem = i;
				lvi.iSubItem = 3;	//Сумма
				ListView_InsertItem(hOrderList, &lvi);
			}
	}
}

void OrderDlg::Cls_OnClose(HWND hwnd)
{
	int result = MessageBox(hOrderDlg,
		L"Сохранить изменения в заказе?",
		L"Запрос на сохранение",
		MB_YESNO | MB_ICONQUESTION | MB_APPLMODAL);
	if (result == IDYES)
		SendMessage(hOrderDlg, WM_COMMAND, MAKEWPARAM(IDC_SAVE_ORDER, 0), 0);
	else
		EndDialog(hwnd, 0);
}

void OrderDlg::checkStatus()
{
	int indexSel = ListView_GetSelectionMark(hOrderList);
	if (indexSel < 0)
	{
		EnableWindow(GetDlgItem(hOrderDlg, IDC_DEL_DISH_ORDER), 0);
		EnableWindow(GetDlgItem(hOrderDlg, IDC_SPIN_NUMBER_DISH), 0);
	}
	else
	{
		EnableWindow(GetDlgItem(hOrderDlg, IDC_DEL_DISH_ORDER), 1);
		EnableWindow(GetDlgItem(hOrderDlg, IDC_SPIN_NUMBER_DISH), 1);
	}

	if (menu_.empty())
	{
		EnableWindow(GetDlgItem(hOrderDlg, IDC_CHECK), 0);
	}
	else
	{
		EnableWindow(GetDlgItem(hOrderDlg, IDC_CHECK), 1);
	}
}

HWND hTransfer(NULL);

void OrderDlg::Cls_OnInitDialog(HWND hwnd, LPARAM lp)
{
	hTransfer = hOrderDlg = hwnd;
	TCHAR buf[100];

	hOrderN = GetDlgItem(hOrderDlg, IDC_ORDER_No);
	wsprintf(buf, L"%d", No_);
	SetWindowText(hOrderN, buf);

	hOrderTable = GetDlgItem(hOrderDlg, IDC_ORDER_TABLE);
	wsprintf(buf, L"%d", table_);
	SetWindowText(hOrderTable, buf);

	hOrderTotals = GetDlgItem(hOrderDlg, IDC_ORDER_TOTALS);
	swprintf_s(buf, 100, L"%.2lf", totals_);
	SetWindowText(hOrderTotals, buf);

	hOrderWaiter = GetDlgItem(hOrderDlg, IDC_ORDER_WAITER);
	SetWindowText(hOrderWaiter, waiter_.c_str());

	hOrderTime = GetDlgItem(hOrderDlg, IDC_ORDER_TIME);
	SetWindowText(hOrderTime, time_.c_str());

	hOrderDate = GetDlgItem(hOrderDlg, IDC_ORDER_DATE);
	SetWindowText(hOrderDate, date_.c_str());

	hNumberDish = GetDlgItem(hOrderDlg, IDC_NUMBER_DISH);

	SendDlgItemMessage(hOrderDlg, IDC_SPIN_NUMBER_DISH, UDM_SETRANGE32, 1, 1000);
	UDACCEL pAccelPortion[2] = { { 1, 1 }, { 2, 10 } };
	SendDlgItemMessage(hOrderDlg, IDC_SPIN_NUMBER_DISH, UDM_SETACCEL, 2, LPARAM(pAccelPortion));

	hContextMenu = GetSubMenu(LoadMenu(GetModuleHandle(0),
		MAKEINTRESOURCE(IDR_CONTEXT_ORDER)), 0);

	RECT rc;
	GetClientRect(hOrderDlg, &rc);
	hOrderList = CreateWindowEx(0L, WC_LISTVIEW, L"",
		WS_VISIBLE | WS_CHILD | WS_BORDER | 
		LVS_REPORT | LVS_SINGLESEL,
		220, rc.top + 40, rc.right - rc.left - 230, rc.bottom - rc.top - 63,
		hOrderDlg, 0, 0, NULL);

	LV_COLUMN lvc;
	memset(&lvc, 0, sizeof(lvc));
	lvc.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	lvc.fmt = LVCFMT_LEFT;
	lvc.cx = (rc.right - rc.left - 170) / 3;

	lvc.iSubItem = 0;
	lvc.pszText = L"Название";
	ListView_InsertColumn(hOrderList, 0, &lvc);

	lvc.iSubItem = 1;
	lvc.pszText = L"Цена, грн";
	ListView_InsertColumn(hOrderList, 1, &lvc);
	ListView_SetColumnWidth(hOrderList, 1, (rc.right - rc.left) / 10);

	lvc.iSubItem = 2;
	lvc.pszText = L"Кол-во";
	ListView_InsertColumn(hOrderList, 2, &lvc);
	ListView_SetColumnWidth(hOrderList, 2, (rc.right - rc.left) / 11);

	lvc.iSubItem = 3;
	lvc.pszText = L"Сумма";
	ListView_InsertColumn(hOrderList, 3, &lvc);
	ListView_SetColumnWidth(hOrderList, 3, (rc.right - rc.left) / 5 + 10);

	refreshListViewItems();
	ListView_SetSelectionMark(hOrderList, 0);

	if (!menu_.empty())
	{
		int indexSel = ListView_GetSelectionMark(hOrderList);
		if (indexSel>=0)
			SendDlgItemMessage(hOrderDlg, IDC_SPIN_NUMBER_DISH, 
				UDM_SETPOS32, 0, menu_[indexSel].getNumber());
	}
	else
	{
		EnableWindow(GetDlgItem(hOrderDlg, IDC_DEL_DISH_ORDER), 0);
		SendDlgItemMessage(hOrderDlg, IDC_SPIN_NUMBER_DISH, UDM_SETPOS32, 0, 1);
	}
	
	hStatus = CreateStatusWindow(WS_CHILD | WS_VISIBLE | CCS_BOTTOM | SBARS_TOOLTIPS |
		SBARS_SIZEGRIP, 0, hOrderDlg, WM_USER);
	int parts[3] = { 200, -1 };//-1 правая граница
	SendMessage(hStatus, SB_SETPARTS, 2, (LPARAM)parts);
	SetTimer(hwnd, 1, 1000, 0);

	mode = (Modes)LOWORD(lp);
	if (mode == NEW)
	{
		ShowWindow(GetDlgItem(hOrderDlg, IDC_CHECK), SW_HIDE);
	}
}

void OrderDlg::Cls_OnCommand(HWND hwnd, WPARAM fact, LPARAM data)
{
	if (LOWORD(fact) == IDC_ADD_DISH_ORDER)
	{
		RECT rc;
		GetWindowRect(hOrderDlg, &rc);
		MoveWindow(hOrderDlg, 701, 100, 
			rc.right - rc.left, rc.bottom - rc.top, 1);

		MenuDlg menuForChoice(menuForOrderListDlg.getMenu_());
		DialogBoxParam(0,
			MAKEINTRESOURCE(IDD_MENU_FOR_ORDER),
			0,
			MenuDlg::DlgProc,
			MAKELPARAM(WAITER, 0));
		MoveWindow(hOrderDlg, rc.left, rc.top, 
			rc.right - rc.left, rc.bottom - rc.top, 1);
		return;
	}

	if (LOWORD(fact) == IDC_TRANSFER_DISH_IN_ORDER)
	{
		int i = (int)HIWORD(data);
		int index = findFullTitle(menuForOrderListDlg()[i].getTitle());
		if (index < 0)
		{
			menu_.push_back(menuForOrderListDlg()[i]);
			menu_.back().setNumber((int)LOWORD(data));
			refreshListViewItems();
		}
		else
		{
			menu_[index].setNumber(menu_[index].getNumber() + (int)LOWORD(data));
			refreshListViewItems();
			ListView_SetSelectionMark(hOrderList, index);
		}
		return;
	}

	if (LOWORD(fact) == ID_SHOW_DISH ||
		LOWORD(fact) == IDOK)
	{
		int indexSel = ListView_GetSelectionMark(hOrderList);
		if (indexSel<0)
			return;

		DishDlg tmp(menu_[indexSel]);
		DialogBoxParam(0,
			MAKEINTRESOURCE(IDD_SHOW_DISH),
			0,
			DishDlg::DlgProc,
			MAKELPARAM(0, WAITER));//0 - просмотр
		return;
	}

	if (LOWORD(fact) == IDC_DEL_DISH_ORDER)
	{
		int indexSel = ListView_GetSelectionMark(hOrderList);
		if (indexSel<0)
			return;

		int result = MessageBox(hOrderDlg,
			TEXT("Вы уверены, что хотите удалить блюдо из заказа?"),
			TEXT("Подтверждение"),
			MB_OKCANCEL | MB_ICONQUESTION | MB_TASKMODAL);
		if (result == IDOK)
		{
			delDish(indexSel);
			refreshListViewItems();
			ListView_SetSelectionMark(hOrderList, 0);
		}
		return;
	}

	if (LOWORD(fact) == IDC_NUMBER_DISH &&
		HIWORD(fact) == EN_CHANGE)
	{
		if (menu_.empty())
			return;
		int indexSel = ListView_GetSelectionMark(hOrderList);
		if (indexSel<0)
			return;

		TCHAR curDishN[4];
		GetWindowText(hNumberDish, curDishN, 4);
		int num = _wtoi(curDishN);
		if (num == 0)
			return;
		SendDlgItemMessage(hOrderDlg, IDC_SPIN_NUMBER_DISH, 
			UDM_SETPOS32, 0, num);
		menu_[indexSel].setNumber(num);
		refreshListViewItems();
		ListView_SetSelectionMark(hOrderList, indexSel);
		return;
	}

	if (LOWORD(fact) == IDC_SAVE_ORDER)
	{
		if (menu_.empty())
		{
			MessageBox(hOrderDlg,
			L"Заказ пуст!\nДобавьте блюда или нажмите Отмена", 
			L"Заказ пустой", 
			MB_OK | MB_ICONERROR | MB_APPLMODAL);
			SetFocus(GetDlgItem(hOrderDlg, IDC_CHANGE_LIST));
			return;
		}

		TCHAR err[50] = { 0 };
		TCHAR buf[15];
		GetWindowText(hOrderN, buf, 15);
		No_ = _wtoi(buf);
		GetWindowText(hOrderTable, buf, 15);
		table_ = _wtoi(buf);
		GetWindowText(hOrderTotals, buf, 15);
		totals_ = _wtof(buf);
		GetWindowText(hOrderWaiter, buf, 15);
		waiter_ = buf;
		_tcsftime(buf, 15, TEXT("%H:%M:%S"), &DateTimeEnd);
		time_ = buf;
		_tcsftime(buf, 15, TEXT("%d.%m.%Y"), &DateTimeEnd);
		date_ = buf;

		EndDialog(hwnd, 0);
	}

	if (LOWORD(fact) == IDC_ORDER_CANCEL||
		LOWORD(fact) == ID_EXIT_WAITER)
	{
		int result = MessageBox(hOrderDlg,
			L"Сохранить изменения в заказе?",
			L"Запрос на сохранение",
			MB_YESNO | MB_ICONQUESTION | MB_APPLMODAL);
		if (result == IDYES)
			SendMessage(hOrderDlg, WM_COMMAND, MAKEWPARAM(IDC_SAVE_ORDER, 0), 0);
		else
		{
			change = FALSE;
			EndDialog(hwnd, 0);
		}
	}

	if (LOWORD(fact) == IDC_CHECK)
	{
		check = TRUE;
		EndDialog(hwnd, 0);
	}
}

void OrderDlg::Cls_OnNotify(HWND hwnd, LPARAM lp)
{
	NMHDR* pnmhdr = (NMHDR*)lp;
	if (pnmhdr->idFrom == GetDlgCtrlID(hOrderList))
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
				case 0:			//Название
					buf = new TCHAR[lstrlen(dish->getTitle().c_str()) + 1];
					lstrcpy(buf, dish->getTitle().c_str());
					lpLvdi->item.pszText = buf;
					break;

				case 1:			//Цена
					buf = new TCHAR[lstrlen(dish->getPriceStr().c_str()) + 1];
					lstrcpy(buf, dish->getPriceStr().c_str());
					lpLvdi->item.pszText = buf;
					break;

				case 2:			//Кол-во
					buf = new TCHAR[lstrlen(dish->getNumberStr().c_str()) + 1];
					lstrcpy(buf, dish->getNumberStr().c_str());
					lpLvdi->item.pszText = buf;
					break;

				case 3:			//Сумма
					buf = new TCHAR[lstrlen(dish->getTotalsStr().c_str()) + 1];
					lstrcpy(buf, dish->getTotalsStr().c_str());
					lpLvdi->item.pszText = buf;
					break;
				}
			}
		}
			break;
		}

		if (pnmhdr->code == NM_CLICK)
		{
			LPNMITEMACTIVATE lpnmitem = (LPNMITEMACTIVATE)lp;
			if (lpnmitem->iItem >= 0)
			{
				SendDlgItemMessage(hOrderDlg, IDC_SPIN_NUMBER_DISH,
					UDM_SETPOS32, 0, menu_[lpnmitem->iItem].getNumber());
			}
			return;
		}
	}
}

void OrderDlg::Cls_OnContextMenu(HWND hwnd, LPARAM pos, WPARAM idItem)
{
	if ((HWND)idItem == hOrderList)
	{
		TrackPopupMenu(hContextMenu, TPM_RIGHTBUTTON |
			TPM_TOPALIGN | TPM_LEFTALIGN, LOWORD(pos), HIWORD(pos), 0, hwnd, 0);
	}
}

void OrderDlg::Cls_OnVscroll(HWND hwnd, WPARAM id)
{
	if (!menu_.empty())
	{
		int indexSel = ListView_GetNextItem(hOrderList,
			-1, LVNI_ALL | LVNI_SELECTED); //ListView_GetSelectionMark(hOrderList);
		if (indexSel < 0)
			return;
		int pos = SendDlgItemMessage(hOrderDlg, IDC_SPIN_NUMBER_DISH,
			UDM_GETPOS32, 0, 0);
		menu_[indexSel].setNumber(pos);
		refreshListViewItems();
		ListView_SetSelectionMark(hOrderList, indexSel);
	}
}

void OrderDlg::Cls_OnTimer(HWND hwnd, WPARAM idTimer)
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

BOOL CALLBACK OrderDlg::DlgProc(HWND hwnd, UINT message,
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
		ptr->Cls_OnCommand(hwnd, wParam, lParam);
		return TRUE;
	case WM_NOTIFY:
		ptr->Cls_OnNotify(hwnd, lParam);
		return TRUE;
	case WM_CONTEXTMENU:
		ptr->Cls_OnContextMenu(hwnd, lParam, wParam);
		return TRUE;
	case WM_VSCROLL:
		ptr->Cls_OnVscroll(hwnd, wParam);
		return TRUE;
	case WM_TIMER:
		ptr->Cls_OnTimer(hwnd, wParam);
		return TRUE;
	}
	return FALSE;
}