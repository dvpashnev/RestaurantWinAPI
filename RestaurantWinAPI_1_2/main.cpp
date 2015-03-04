#include "header.h"

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrev, LPSTR lpszCmdLine,
	int nCmdShow)
{
	INITCOMMONCONTROLSEX icc = { sizeof(INITCOMMONCONTROLSEX) };
	icc.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&icc);

	Enter Verification;
	return DialogBox(hInst, MAKEINTRESOURCE(IDD_ENTER), NULL, Enter::DlgProc);
}