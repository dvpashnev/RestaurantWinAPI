
class Enter
{
public:
	Enter();
	~Enter();
	static BOOL CALLBACK DlgProc(HWND hWnd, UINT mes, WPARAM wp, LPARAM lp);
	static Enter *ptr;

	void Cls_OnInitDialog(HWND hwnd);
	void Cls_OnCommand(HWND hwnd, WPARAM fact);
	void Cls_OnClose(HWND hwnd);

private:

	HWND hEnter, hOk, hLogin, hPassword;
	TCHAR login[20], password[20];

};

