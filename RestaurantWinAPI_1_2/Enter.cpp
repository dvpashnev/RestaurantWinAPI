#include "header.h"

Enter* Enter::ptr = NULL;


Enter::Enter()
{
	ptr = this;
}

Enter::~Enter()
{
}

void Enter::Cls_OnClose(HWND hwnd)
{
	EndDialog(hwnd, 0);
}

void Enter::Cls_OnInitDialog(HWND hwnd)
{
	hEnter = hwnd;
	hOk = GetDlgItem(hEnter, IDOK);
	hLogin = GetDlgItem(hEnter, IDC_LOGIN);
	hPassword = GetDlgItem(hEnter, IDC_PASSWORD);
	SetFocus(hLogin);
	EnableWindow(hOk, 0);
}

void Enter::Cls_OnCommand(HWND hwnd, WPARAM fact)
{
	if ((LOWORD(fact) == IDC_LOGIN || LOWORD(fact) == IDC_PASSWORD) &&
		(HIWORD(fact) == EN_CHANGE))
	{
		GetWindowText(hLogin, login, 20);
		GetWindowText(hPassword, password, 20);

		if (lstrlen(login) == 0 || lstrlen(password) < 5)
			EnableWindow(hOk, 0);
		else
			EnableWindow(hOk, 1);
	}

	if (LOWORD(fact) == IDCANCEL)
	{
		EndDialog(hwnd, 0);
	}
	
	if (LOWORD(fact) == IDOK)
	{
		ShowWindow(hwnd, SW_HIDE);
		ChainRestaurants Restoran;
		if (!lstrcmp(login, L"manager") && !lstrcmp(password, L"manager"))
		{
			Restoran.service(MANAGER);
		}
		else if (!lstrcmp(login, L"waiter") && !lstrcmp(password, L"waiter"))
		{
			Restoran.service(WAITER);
		}
		else 
			MessageBox(hwnd, L"Пароль или логин введены неверно!",
			TEXT("Неправильный входные данные"),
			MB_OK | MB_ICONWARNING | MB_TASKMODAL);
		SetWindowText(hLogin, 0);
		SetWindowText(hPassword, 0);
		SetFocus(hLogin);
		EnableWindow(hOk, 0);
		ShowWindow(hwnd, SW_SHOWNORMAL);
	}
}

BOOL CALLBACK Enter::DlgProc(HWND hwnd, UINT message,
	WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_CLOSE:
		ptr->Cls_OnClose(hwnd);
		return TRUE;
	case WM_INITDIALOG:
		ptr->Cls_OnInitDialog(hwnd);
		return TRUE;
	case WM_COMMAND:
		ptr->Cls_OnCommand(hwnd, wParam);
		return TRUE;
	}
	return FALSE;
}