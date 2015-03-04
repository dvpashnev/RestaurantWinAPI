#pragma once

class OrderDlg:
	public Order
{
public:
	OrderDlg();
	OrderDlg::OrderDlg(const Order&, DishContainer);//Создание с копией заказа и меню ресторана
	OrderDlg(DishContainer obj);					//Создание из списка блюд
	~OrderDlg();

	bool getCheck() const;	//Получение флага выдачи чека
	bool getChange() const;	//Получение флага изменений

	static BOOL CALLBACK DlgProc(HWND hWnd, UINT mes, WPARAM wp, LPARAM lp);
	static OrderDlg *ptr;

	static time_t t;
	static struct tm DateTimeEnd;

	void refreshListViewItems();	//Обновление списка блюд
	void checkStatus();				//Проверка и установка статуса кнопок
			
	void Cls_OnInitDialog(HWND hwnd, LPARAM);
	void Cls_OnCommand(HWND hwnd, WPARAM, LPARAM);
	void Cls_OnNotify(HWND hwnd, LPARAM);
	void Cls_OnClose(HWND hwnd);
	void Cls_OnContextMenu(HWND hwnd, LPARAM, WPARAM);
	void Cls_OnVscroll(HWND hwnd, WPARAM);
	void Cls_OnTimer(HWND hwnd, WPARAM);

private:

	Menu menuForOrderListDlg;							//Меню ресторана для занесения блюд в список заказа
	HWND hOrderDlg, hOrderN, hOrderTable, hOrderTotals, //Дескрипторы окна и элементов управления
		hOrderWaiter, hOrderTime, hOrderDate, 
		hNumberDish, hOrderList, hStatus;
	HMENU hContextMenu;									//Дескриптор контекстного меню
	Modes mode;											//Режим работы (новый заказ, просмотр,  редактирование)
	bool check;											//Флаг нажатия кнопки выдачи чека 
	bool change;										//Флаг необходимости сохранения изменений
};