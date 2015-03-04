#include "header.h"

RestaurantStandartDlg* RestaurantStandartDlg::ptr = NULL;

BOOL RestaurantStandartDlg::flicker(TRUE);

time_t RestaurantStandartDlg::t(time(NULL));
struct tm RestaurantStandartDlg::DateTimeEnd;

RestaurantStandartDlg::RestaurantStandartDlg():
RestaurantStandart(),
nTable(1)
{
	ptr = this;
}

RestaurantStandartDlg::~RestaurantStandartDlg()
{}

RestaurantStandartDlg& RestaurantStandartDlg::operator=(Restaurant*& obj)
{
	name_ = (*obj).getName();
	openedOrders_ = (*obj).getOpenedOrders();
	closedOrders_ = (*obj).getClosedOrders();
	nOrdersOpened_ = (*obj).getNOpenedOrders();
	ordersOpenedTotals_ = (*obj).getOpenedTotals();
	nOrdersClosed_ = (*obj).getNClosedOrders();
	ordersClosedTotals_ = (*obj).getClosedTotals();
	nOrdersDeleted_ = (*obj).getNDeletedOrders();
	nTables_ = (*obj).getNTables();
	menu_ = (*obj).getMenu_();
	return *this;
}

void RestaurantStandartDlg::refreshDlgManager()
{
	TCHAR buf[100];
	wsprintf(buf, L"%d", menu_.size());
	SetWindowText(hNDishesInMenu, buf);
	wsprintf(buf, L"%d", findType(COLD).size());
	SetWindowText(hNColdDishesInMenu, buf);
	wsprintf(buf, L"%d", findType(HOT).size());
	SetWindowText(hNHotDishesInMenu, buf);
	wsprintf(buf, L"%d", findType(DESERT).size());
	SetWindowText(hNDesertsInMenu, buf);
	wsprintf(buf, L"%d", findType(DESERT).size());
	SetWindowText(hNMorningDishesInMenu, buf);
	wsprintf(buf, L"%d", findType(DESERT).size());
	SetWindowText(hNAfternoonDishesInMenu, buf);
	wsprintf(buf, L"%d", findType(DESERT).size());
	SetWindowText(hNEveningInMenu, buf);
	SendDlgItemMessage(hRestaurantStandartDlg, IDC_SPIN_N_TABLES, UDM_SETPOS32, 0, nTables_);
	wsprintf(buf, L"%d", nTables_ - nOrdersOpened_);
	SetWindowText(hNTablesFree, buf);
	wsprintf(buf, L"%d", nOrdersOpened_);
	SetWindowText(hNOpenOrders, buf);
	swprintf_s(buf, 100, L"%.2lf", ordersOpenedTotals_);
	SetWindowText(hNOpenOrdersTotals, buf);
	wsprintf(buf, L"%d", nOrdersClosed_);
	SetWindowText(hNClosedOrders, buf);
	swprintf_s(buf, 100, L"%.2lf", ordersClosedTotals_);
	SetWindowText(hNClosedOrdersTotals, buf);
	if (nOrdersOpened_ == 0)
		EnableWindow(GetDlgItem(hRestaurantStandartDlg,
		ID_REPORT_ORDERS_IN_WORK), 0);
	else
		EnableWindow(GetDlgItem(hRestaurantStandartDlg,
		ID_REPORT_ORDERS_IN_WORK), 1);

	if (nOrdersClosed_ == 0)
		EnableWindow(GetDlgItem(hRestaurantStandartDlg,
		ID_REPORT_CLOSEDORDERS), 0);
	else
		EnableWindow(GetDlgItem(hRestaurantStandartDlg,
		ID_REPORT_CLOSEDORDERS), 1);

}

void RestaurantStandartDlg::refreshDlgWaiter()
{
	ordersOpenedTotals_ = 0.;

	ListView_DeleteAllItems(hListOrders);

	LV_ITEM lvi;
	memset(&lvi, 0, sizeof(lvi));

	lvi.mask = LVIF_IMAGE | LVIF_TEXT;

	for (size_t i(1); i <= nTables_; i++)
	{
		lvi.iItem = i;
		lvi.iSubItem = 0;
		lvi.cchTextMax = 3;
		TCHAR buf[4];
		wsprintf(buf, L"%d", i);
		lvi.pszText = buf;

		if (openedOrders_.find(i) != openedOrders_.end())
		{
			lvi.iImage = 1;
			ordersOpenedTotals_ += openedOrders_[i].getTotals_();
		}
		else
			lvi.iImage = 0;
		ListView_InsertItem(hListOrders, &lvi);

		lvi.iSubItem = 1;
		if (openedOrders_.find(i) != openedOrders_.end())
		{
			lvi.pszText = L"занят";
		}
		else
			lvi.pszText = L"свободен";

		ListView_InsertItem(hListOrders, &lvi);

	}
	SetFocus(hListOrders);
	ListView_SetSelectionMark(hListOrders, 0);
}

void RestaurantStandartDlg::checkStatus()
{
	int indexSel = ListView_GetSelectionMark(hListOrders) + 1;
	if (openedOrders_.find(indexSel) == openedOrders_.end())
	{
		EnableWindow(GetDlgItem(hRestaurantStandartDlg,
			IDC_EDIT_ORDER), 0);
		EnableWindow(GetDlgItem(hRestaurantStandartDlg,
			IDC_DEL_ORDER), 0);
		EnableWindow(GetDlgItem(hRestaurantStandartDlg,
			IDC_PAY_ORDER), 0);
		EnableMenuItem(hContextMenu, IDC_EDIT_ORDER, 
			MF_BYCOMMAND | MF_DISABLED);
		EnableMenuItem(hContextMenu, IDC_DEL_ORDER,
			MF_BYCOMMAND | MF_DISABLED);
		EnableMenuItem(hContextMenu, IDC_PAY_ORDER,
			MF_BYCOMMAND | MF_DISABLED);
		HMENU mainMenu = GetMenu(hRestaurantStandartDlg);
		EnableMenuItem(mainMenu, IDC_EDIT_ORDER,
			MF_BYCOMMAND | MF_DISABLED);
		EnableMenuItem(mainMenu, IDC_DEL_ORDER,
			MF_BYCOMMAND | MF_DISABLED);
		EnableMenuItem(mainMenu, IDC_PAY_ORDER,
			MF_BYCOMMAND | MF_DISABLED);
	}
	else
	{
		EnableWindow(GetDlgItem(hRestaurantStandartDlg,
			IDC_EDIT_ORDER), 1);
		EnableWindow(GetDlgItem(hRestaurantStandartDlg,
			IDC_DEL_ORDER), 1);
		EnableWindow(GetDlgItem(hRestaurantStandartDlg,
			IDC_PAY_ORDER), 1);
		EnableMenuItem(hContextMenu, IDC_EDIT_ORDER,
			MF_BYCOMMAND | MF_ENABLED);
		EnableMenuItem(hContextMenu, IDC_DEL_ORDER,
			MF_BYCOMMAND | MF_ENABLED);
		EnableMenuItem(hContextMenu, IDC_PAY_ORDER,
			MF_BYCOMMAND | MF_ENABLED);
		HMENU mainMenu = GetMenu(hRestaurantStandartDlg);
		EnableMenuItem(mainMenu, IDC_EDIT_ORDER,
			MF_BYCOMMAND | MF_ENABLED);
		EnableMenuItem(mainMenu, IDC_DEL_ORDER,
			MF_BYCOMMAND | MF_ENABLED);
		EnableMenuItem(mainMenu, IDC_PAY_ORDER,
			MF_BYCOMMAND | MF_ENABLED);
	}
}

void RestaurantStandartDlg::Cls_OnClose(HWND hwnd)
{
	EndDialog(hwnd, 0);
}

void RestaurantStandartDlg::Cls_OnInitDialog(HWND hwnd, LPARAM lp)
{
	wstring filepath((nameOfFile(L"orders\\" + strDate(curTime()))));
	int length = WideCharToMultiByte(CP_ACP, 0,
		filepath.c_str(),
		-1, NULL, 0, 0, 0);
	char *ptr = new char[length];
	WideCharToMultiByte(CP_ACP, 0,
		filepath.c_str(),
		-1, ptr, length, 0, 0);

	if (_mkdir(ptr) != -1)	//Если папки с сегодняшней датой нет, создаём её и
	{		
		openedOrders_.clear();	//обнуляем Заказы в работе (Столик - заказ)
		closedOrders_.clear();	//обнуляем Оплаченные заказы (Столик - заказ)
		nOrdersOpened_ = 0;		//обнуляем количество заказов в работе.
		ordersOpenedTotals_ = 0.;//обнуляем Сумма заказов в работе
		nOrdersClosed_ = 0;		//обнуляем Количество оплаченных заказов
		ordersClosedTotals_ = 0.;//обнуляем Сумма оплаченных заказов (заработок на сегодня)
		nOrdersDeleted_ = 0;	//обнуляем Количество удаленных заказов
	}	

	hRestaurantStandartDlg = hwnd;

	role = (Roles)lp;

	//---------------MANAGER------------------
	if (role == MANAGER)
	{
		TCHAR buf[100];
		lstrcpy(buf, L"Ресторан ");
		lstrcat(buf, name_.c_str());
		SetWindowText(hRestaurantStandartDlg, buf);
		
		hNDishesInMenu = GetDlgItem(hRestaurantStandartDlg, IDC_N_DISH_IN_MENU);
		hNColdDishesInMenu = GetDlgItem(hRestaurantStandartDlg, IDC_N_COLD);
		hNHotDishesInMenu = GetDlgItem(hRestaurantStandartDlg, IDC_N_HOT);
		hNDesertsInMenu = GetDlgItem(hRestaurantStandartDlg, IDC_N_DESERTS);
		hNMorningDishesInMenu = GetDlgItem(hRestaurantStandartDlg, IDC_N_MORNING);
		hNAfternoonDishesInMenu = GetDlgItem(hRestaurantStandartDlg, IDC_N_AFTERNOON);
		hNEveningInMenu = GetDlgItem(hRestaurantStandartDlg, IDC_N_EVENING);
		hNTables = GetDlgItem(hRestaurantStandartDlg, IDC_N_TABLES);

		SendDlgItemMessage(hRestaurantStandartDlg, IDC_SPIN_N_TABLES, UDM_SETRANGE32, 1, 100);
		UDACCEL pAccelPortion[2] = { { 1, 1 }, { 2, 10 } };
		SendDlgItemMessage(hRestaurantStandartDlg, IDC_SPIN_N_TABLES, UDM_SETACCEL, 2, LPARAM(pAccelPortion));

		hNTablesFree = GetDlgItem(hRestaurantStandartDlg, IDC_N_TABLES_FREE);
		hNOpenOrders = GetDlgItem(hRestaurantStandartDlg, IDC_N_ORDERS_IN_WORK);
		hNOpenOrdersTotals = GetDlgItem(hRestaurantStandartDlg, IDC_TOTALS_IN_WORK);
		hNClosedOrders = GetDlgItem(hRestaurantStandartDlg, IDC_N_CLOSED_ORDERS);
		hNClosedOrdersTotals = GetDlgItem(hRestaurantStandartDlg, IDC_REVENUE);
		
		hAlarm = GetDlgItem(hRestaurantStandartDlg, IDC_ALARM);
		SendMessage(hAlarm, PBM_SETRANGE, 0, MAKELPARAM(0, 60));
		SendMessage(hAlarm, PBM_SETBARCOLOR, 0, LPARAM(RGB(255, 0, 0)));
		SendMessage(hAlarm, PBM_SETPOS, 0, 0);

		refreshDlgManager();
	}

	//---------------WAITER------------------
	else if (role == WAITER)
	{
		hFindNOrder = GetDlgItem(hRestaurantStandartDlg, IDC_N_ORDER);
		hBtnFind = GetDlgItem(hRestaurantStandartDlg, IDC_BUT_FIND_ORDER);

		RECT rc;
		GetClientRect(hRestaurantStandartDlg, &rc);
		hListOrders = CreateWindowEx(0L, WC_LISTVIEW, L"",
			WS_VISIBLE | WS_CHILD | WS_BORDER |
			LVS_ICON | LVS_ALIGNTOP | LVS_SINGLESEL | LVS_AUTOARRANGE | LVS_SHOWSELALWAYS,
			10, rc.top+90, rc.right - rc.left - 20, rc.bottom - rc.top - 200,
			hRestaurantStandartDlg, 0, 0, NULL);
		himlLarge = ImageList_Create(
			50, 50, //GetSystemMetrics(SM_CXICON), GetSystemMetrics(SM_CYICON),
			TRUE, 1, 1);
		HICON hIcon = LoadIcon(GetModuleHandle(0), MAKEINTRESOURCE(IDI_ICON_FREE));
		ImageList_AddIcon(himlLarge, hIcon);
		hIcon = LoadIcon(GetModuleHandle(0), MAKEINTRESOURCE(IDI_ICON_BUSY));
		ImageList_AddIcon(himlLarge, hIcon);
		ListView_SetImageList(hListOrders, himlLarge, LVSIL_NORMAL);

		LV_COLUMN lvc;
		memset(&lvc, 0, sizeof(lvc));

		lvc.mask = LVCF_FMT | LVCF_WIDTH;
		lvc.fmt = LVCFMT_LEFT;
		lvc.cx = (rc.right - rc.left) / 4;

		lvc.iSubItem = 0;
		ListView_InsertColumn(hListOrders, 0, &lvc);

		lvc.iSubItem = 1;
		ListView_InsertColumn(hListOrders, 1, &lvc);

		refreshDlgWaiter();
	}

	hContextMenu = GetSubMenu(LoadMenu(GetModuleHandle(0),
		MAKEINTRESOURCE(IDR_CONTEXT_WAITER)), 0);

	hStatus = CreateStatusWindow(WS_CHILD | WS_VISIBLE | CCS_BOTTOM | SBARS_TOOLTIPS |
		SBARS_SIZEGRIP, 0, hRestaurantStandartDlg, WM_USER);
	int parts[3] = { 200, -1 };//-1 правая граница
	SendMessage(hStatus, SB_SETPARTS, 2, (LPARAM)parts);

	SetTimer(hwnd, 3, 1000, 0);
}


void RestaurantStandartDlg::Cls_OnNotify(HWND hwnd, LPARAM lp)
{ 
	NMHDR* pnmhdr = (NMHDR*)lp;
	if (pnmhdr->idFrom == GetDlgCtrlID(hListOrders))
	{
		if (pnmhdr->code == NM_SETFOCUS 
			|| pnmhdr->code == NM_KILLFOCUS)
		{
			checkStatus();
		}
			
		if (pnmhdr->code == NM_CLICK)
		{
			LPNMITEMACTIVATE lpnmitem = (LPNMITEMACTIVATE)lp;
			checkStatus();
			return;
		}

		if (pnmhdr->code == NM_DBLCLK)
		{
			LPNMITEMACTIVATE lpnmitem = (LPNMITEMACTIVATE)lp;
			if (openedOrders_.find(lpnmitem->iItem + 1) == openedOrders_.end())
				SendMessage(hRestaurantStandartDlg, WM_COMMAND,
				MAKEWPARAM(IDC_NEW_ORDER, 0), 0);
			else
				SendMessage(hRestaurantStandartDlg, WM_COMMAND,
				MAKEWPARAM(IDC_EDIT_ORDER, 0), 0);
			return;
		}
	}
}

void RestaurantStandartDlg::Cls_OnSize(HWND hwnd)
{
	RECT rc;
	GetClientRect(hRestaurantStandartDlg, &rc);
	MoveWindow(hListOrders, 10, rc.top + 90, 
		rc.right - rc.left - 20, rc.bottom - rc.top - 160, 1);
}

void RestaurantStandartDlg::Cls_OnContextMenu(HWND hwnd, LPARAM pos, WPARAM idItem)
{
	if ((HWND)idItem == hListOrders)
	{
		TrackPopupMenu(hContextMenu, TPM_RIGHTBUTTON |
			TPM_TOPALIGN | TPM_LEFTALIGN, LOWORD(pos), HIWORD(pos), 0, hwnd, 0);
	}
}

void RestaurantStandartDlg::Cls_OnVscroll(HWND hwnd, WPARAM id)
{
	int pos = SendDlgItemMessage(hRestaurantStandartDlg, IDC_SPIN_N_TABLES,
		UDM_GETPOS32, 0, 0);
	nTables_ = pos;
}

void RestaurantStandartDlg::Cls_OnCommand(HWND hwnd, WPARAM fact)
{
	if (LOWORD(fact) == ID_EXIT)
	{
		EndDialog(hwnd, 0);
	}

	//---------------MANAGER------------------
	if (role == MANAGER)
	{
	if (LOWORD(fact) == ID_MENU_CHANGE)
	{
		ShowWindow(hwnd, SW_HIDE);
		MenuDlg tmp(menu_);
		DialogBoxParam(0,
			MAKEINTRESOURCE(IDD_EDIT_MENU),
			0,
			MenuDlg::DlgProc,
			MAKELPARAM(role, 0));
		menu_ = tmp.getMenu_();
		refreshDlgManager();
		ShowWindow(hwnd, SW_SHOWNORMAL);
		return;
	}

	if (LOWORD(fact) == IDC_N_TABLES &&
		HIWORD(fact) == EN_CHANGE)
	{
		TCHAR buf[4];
		GetWindowText(hNTables, buf, 4);
		int num = _wtoi(buf);
		if (num == 0)
			return;
		SendDlgItemMessage(hRestaurantStandartDlg, IDC_SPIN_N_TABLES,
			UDM_SETPOS32, 0, num);
		nTables_ = num;
		refreshDlgManager();
		return;
	}

	if (LOWORD(fact) == IDC_ALARM_No_TABLE &&
		HIWORD(fact) == EN_CHANGE)
	{
		SetTimer(hwnd, 2, 500, 0);
		return;
	}
	if (LOWORD(fact) == IDC_ALARM_WAITER &&
		HIWORD(fact) == EN_CHANGE)
	{
		KillTimer(hwnd, 2);
	}
	}

	//---------------WAITER------------------

	else if (role == WAITER)
	{
		if (LOWORD(fact) == IDC_N_ORDER &&
			HIWORD(fact) == EN_CHANGE)
		{
			TCHAR query[5];
			GetWindowText(hFindNOrder, query, 5);
			if (lstrlen(query) == 0)
			{
				SetWindowText(hBtnFind, L"<-Введите номер для поиска");
				return;
			}

			size_t findNo = _wtoi(query);
			if (findNo > (nOrdersDeleted_ + nOrdersOpened_ + nOrdersClosed_))
			{
				SetWindowText(hBtnFind, L"Номер не найден");
				SetFocus(hFindNOrder);
				return;
			}
			else
			{
				bool result(FALSE);
				for (pair<size_t, Order> order : openedOrders_)
					if (order.second.getNo() == findNo)
					{
						SetWindowText(hBtnFind, L"<-Открыть заказ");
						result = TRUE;
						nTable = order.second.getNoTable();
						return;
					}
				if (result == FALSE)
					SetWindowText(hBtnFind, L"Номер не найден");
			}
			return;
		}

		if (LOWORD(fact) == IDC_BUT_FIND_ORDER)
		{
			TCHAR btn[30];
			GetWindowText(hBtnFind, btn, 30);
			if (!lstrcmp(btn, L"<-Открыть заказ"))
			{
			TCHAR query[5];
			GetWindowText(hFindNOrder, query, 5);
			size_t findNo = _wtoi(query);
			SendMessage(hRestaurantStandartDlg, WM_COMMAND, 
				MAKEWPARAM(IDC_EDIT_ORDER, nTable), 0);
			}
			else if (!lstrcmp(btn, L"Номер не найден"))
				MessageBox(hRestaurantStandartDlg,
				TEXT("Заказ с таким номером не найден!\nПопробуйте ввести другой номер!"),
				TEXT("Заказ не найден!"),
				MB_OK | MB_ICONERROR | MB_TASKMODAL);
			else if (!lstrcmp(btn, L"<-Введите номер для поиска"))
				MessageBox(hRestaurantStandartDlg,
				TEXT("Введите номер для поиска заказа в поле слева от кнопки!"),
				TEXT("Не введён номер заказа!"),
				MB_OK | MB_ICONERROR | MB_TASKMODAL);
		}

		if (LOWORD(fact) == IDC_NEW_ORDER)
		{
			int indexSel = ListView_GetSelectionMark(hListOrders) + 1;
			if (openedOrders_.find(indexSel) != openedOrders_.end())
				MessageBox(hRestaurantStandartDlg,
					TEXT("Стол занят! Выберите свободный стол!"),
					TEXT("Заказ ещё не закрыт!"),
					MB_OK | MB_ICONERROR | MB_TASKMODAL);
			else
			{
			ShowWindow(hRestaurantStandartDlg, SW_HIDE);
			OrderDlg tmp(menu_);
			tmp.setNo(nOrdersDeleted_ + nOrdersOpened_ + nOrdersClosed_ + 1);
			tmp.setNoTable(indexSel);
			DialogBoxParam(GetModuleHandle(0),
				MAKEINTRESOURCE(IDD_ORDER),
				hRestaurantStandartDlg,
				OrderDlg::DlgProc,
				MAKELPARAM(NEW, 0));
			if (tmp.getTime_() != L"")
			{
				openedOrders_.insert(make_pair(tmp.getNoTable(), Order(tmp)));
				nOrdersOpened_++;
				LV_ITEM lvi;
				memset(&lvi, 0, sizeof(lvi));
				lvi.mask = LVIF_IMAGE | LVIF_TEXT;
				lvi.iItem = indexSel - 1;
				ListView_GetItem(hListOrders, &lvi);
				lvi.iSubItem = 0;
				lvi.iImage = 1;
				TCHAR buf[4];
				wsprintf(buf, L"%d", indexSel);
				lvi.pszText = buf;
				ListView_SetItem(hListOrders, &lvi);

				lvi.iSubItem = 1;
				lvi.pszText = L"занят";
				ListView_SetItem(hListOrders, &lvi);
			}
			ShowWindow(hRestaurantStandartDlg, SW_SHOWNORMAL);
			}
			SetFocus(hListOrders);
			return;
		}

		if (LOWORD(fact) == IDC_EDIT_ORDER)
		{
			int indexSel(0);
			if (HIWORD(fact) == 0)
				indexSel = ListView_GetSelectionMark(hListOrders) + 1;
			else
				indexSel = (int)HIWORD(fact);

			if (openedOrders_.find(indexSel) == openedOrders_.end())
				MessageBox(hRestaurantStandartDlg,
					TEXT("Стол свободен!\nВыберите занятый стол!"),
					TEXT("Заказа нет!"),
					MB_OK | MB_ICONERROR | MB_TASKMODAL);
			else
			{
				OrderDlg tmp(openedOrders_[indexSel], menu_);

				DialogBoxParam(0,
					MAKEINTRESOURCE(IDD_ORDER),
					0,
					OrderDlg::DlgProc,
					MAKELPARAM(EDIT, 0));
				if (tmp.getChange())
				{
				openedOrders_[tmp.getNoTable()] = Order(tmp);
				nOrdersOpened_ = openedOrders_.size();
				}
				if (tmp.getCheck() == TRUE)
				{
					SendMessage(hRestaurantStandartDlg, WM_COMMAND, 
						MAKEWPARAM(IDC_PAY_ORDER, 0), 0);
				}
				ShowWindow(hRestaurantStandartDlg, SW_SHOWNORMAL);
			}
			SetFocus(hListOrders);
			return;
		}

		if (LOWORD(fact) == IDC_DEL_ORDER)
		{
			int indexSel = ListView_GetSelectionMark(hListOrders) + 1;
			if (openedOrders_.find(indexSel) == openedOrders_.end())
			{
				MessageBox(hRestaurantStandartDlg,
				TEXT("Заказа за этим столом нет! Выберите занятый стол!"),
				TEXT("Заказа нет!"),
				MB_OK | MB_ICONERROR | MB_TASKMODAL);
				return;
			}

			int result = MessageBox(hRestaurantStandartDlg,
				L"Вы уверены, что хотите удалить заказ?",
				L"Запрос на удаление",
				MB_YESNO | MB_ICONQUESTION | MB_APPLMODAL);
			if (result == IDYES)
			{
				wstring path = nameOfFile(openedOrders_[indexSel].nameOfFile());
				int length = WideCharToMultiByte(CP_ACP, 0,
					path.c_str(),
					-1, NULL, 0, 0, 0);
				char *ptr = new char[length];
				WideCharToMultiByte(CP_ACP, 0,
					path.c_str(),
					-1, ptr, length, 0, 0);

				remove(ptr);	//Удаляется из файлов рабочих,
				saveToFile(nameOfFile(openedOrders_[indexSel].nameOfFile(L"del")));//записывается с суффиксом del.

				openedOrders_.erase(indexSel); //Удаляется из массива рабочих
				nOrdersDeleted_++;
				nOrdersOpened_--;

				LV_ITEM lvi;
				memset(&lvi, 0, sizeof(lvi));
				lvi.mask = LVIF_IMAGE | LVIF_TEXT;
				lvi.iItem = indexSel - 1;
				ListView_GetItem(hListOrders, &lvi);
				lvi.iSubItem = 0;
				lvi.iImage = 0;
				TCHAR buf[4];
				wsprintf(buf, L"%d", indexSel);
				lvi.pszText = buf;
				ListView_SetItem(hListOrders, &lvi);

				lvi.iSubItem = 1;
				lvi.pszText = L"свободен";
				ListView_SetItem(hListOrders, &lvi);
			}
			SetFocus(hListOrders);
			return;
		}

		if (LOWORD(fact) == IDC_PAY_ORDER)
		{
			int indexSel = ListView_GetSelectionMark(hListOrders) + 1;
			if (openedOrders_.find(indexSel) == openedOrders_.end())
				MessageBox(hRestaurantStandartDlg,
				TEXT("Заказа за этим столом нет! Выберите занятый стол!"),
				TEXT("Заказа нет!"),
				MB_OK | MB_ICONERROR | MB_TASKMODAL);
			else
			{
			wstring path = nameOfFile(openedOrders_[indexSel].nameOfFile());
			int length = WideCharToMultiByte(CP_ACP, 0,
				path.c_str(),
				-1, NULL, 0, 0, 0);
			char *ptr = new char[length];
			WideCharToMultiByte(CP_ACP, 0,
				path.c_str(),
				-1, ptr, length, 0, 0);

			remove(ptr);	//Удаляется из файлов рабочих,
			saveToFile(nameOfFile(openedOrders_[indexSel].nameOfFile(L"bill")));//записывается с суффиксом bill.
			closedOrders_.insert(make_pair(openedOrders_[indexSel].getNo(), openedOrders_[indexSel]));
			nOrdersClosed_++;
			ordersClosedTotals_ += openedOrders_[indexSel].getTotals_();

			wstring dishList;

			for (Dish d : openedOrders_[indexSel].getMenu_())
			{
				wstring dishTitle(d.getTitle());
				dishTitle.resize(20, ' ');
				wstring dishNumber(to_wstring(d.getNumber()));
				wstring dishPrice(to_wstring(d.getPrice()));
				size_t size = dishPrice.size();
				dishPrice.resize(size-4);
				wstring dishTotal(to_wstring(d.getPrice()*(double)d.getNumber()));
				size = dishTotal.size();
				dishTotal.resize(size - 4);
				
				dishList += dishTitle + L"\t" +
					dishNumber + L" шт * " +
					dishPrice + L" грн = " +
					dishTotal + L" грн\n";
			}

			wstring orderTotals(to_wstring(openedOrders_[indexSel].getTotals_()));
			size_t size = orderTotals.size();
			orderTotals.resize(size - 4);

			wstring check = 
				L"Номер стола: " + to_wstring(openedOrders_[indexSel].getNoTable()) + L"\t" +
				L"Номер заказа: " + to_wstring(openedOrders_[indexSel].getNo()) + L"\n\n" +
				dishList + L"\n" +
				L"Сумма заказа: " + 
				orderTotals + L" грн\n" +
				L"Официант: " + openedOrders_[indexSel].getWaiter_() + L"\n" +
				L"Время: " + openedOrders_[indexSel].getTime_() + L"\t" +
				L"Дата: " + openedOrders_[indexSel].getDate_() + L"\n";

			MessageBox(hRestaurantStandartDlg,
				check.c_str(),
				L"Чек",
				MB_OK | MB_ICONINFORMATION | MB_APPLMODAL);

			openedOrders_.erase(indexSel); //Удаляется из массива рабочих
			nOrdersOpened_--;

			LV_ITEM lvi;
			memset(&lvi, 0, sizeof(lvi));
			lvi.mask = LVIF_IMAGE | LVIF_TEXT;
			lvi.iItem = indexSel - 1;
			ListView_GetItem(hListOrders, &lvi);
			lvi.iSubItem = 0;
			lvi.iImage = 0;
			TCHAR buf[4];
			wsprintf(buf, L"%d", indexSel);
			lvi.pszText = buf;
			ListView_SetItem(hListOrders, &lvi);

			lvi.iSubItem = 1;
			lvi.pszText = L"свободен";
			ListView_SetItem(hListOrders, &lvi);
			}
			SetWindowText(hFindNOrder, L"");
			SetFocus(hListOrders);
			return;
		}
	}
}

void RestaurantStandartDlg::Cls_OnTimer(HWND hwnd, WPARAM idTimer)
{
	if (idTimer == 2)
	{
		if (flicker == TRUE)
		{
			SendMessage(hAlarm, PBM_SETPOS, 60, 0);
			flicker = FALSE;
		}
		else
		{
			SendMessage(hAlarm, PBM_SETPOS, 0, 0);
			flicker = TRUE;
		}
	}

	if (idTimer == 3)
	{
		time_t CurrentTime(time(NULL));
		localtime_s(&DateTimeEnd, &CurrentTime);
		TCHAR str[100];
		_tcsftime(str, 100, TEXT("%H:%M:%S, %d.%m.%Y"), &DateTimeEnd);
		SendMessage(hStatus, SB_SETTEXT, 0, (LPARAM)str);
	}
}

BOOL CALLBACK RestaurantStandartDlg::DlgProc(HWND hwnd, UINT message,
	WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_CLOSE:
		ptr->Cls_OnClose(hwnd);
		return TRUE;
	case WM_SIZE:
		ptr->Cls_OnSize(hwnd);
		return TRUE;
	case WM_INITDIALOG:
		ptr->Cls_OnInitDialog(hwnd, lParam);
		return TRUE;
	case WM_CONTEXTMENU:
		ptr->Cls_OnContextMenu(hwnd, lParam, wParam);
		return TRUE;
	case WM_COMMAND:
		ptr->Cls_OnCommand(hwnd, wParam);
		return TRUE;
	case WM_NOTIFY:
		ptr->Cls_OnNotify(hwnd, lParam);
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