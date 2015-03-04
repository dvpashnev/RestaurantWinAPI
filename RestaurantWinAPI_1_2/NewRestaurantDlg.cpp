class NewRestaurantDlg
{
public:
	NewRestaurantDlg();
	~NewRestaurantDlg();

	TCHAR* getName();
	TCHAR* getType();

	static BOOL CALLBACK DlgProc(HWND hWnd, UINT mes, WPARAM wp, LPARAM lp);
	static NewRestaurantDlg *ptr;

	void Cls_OnInitDialog(HWND hwnd);
	void Cls_OnCommand(HWND hwnd, WPARAM fact);
	void Cls_OnClose(HWND hwnd);

private:

	HWND hNewRestaurantDlg, hOk, hName, hTypeVip, hTypeStandart, hTypeFastFood;
	TCHAR *wsName_, *wsType_;
};