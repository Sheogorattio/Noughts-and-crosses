#include <windows.h>
#include <tchar.h>
#include "resource.h"
#include<cmath>
const INT ButtonNumber = 9;
INT AnableFieldsNumber = ButtonNumber;
HWND hButtonArr[ButtonNumber], hNewGameButton;
BOOL Result = 0;

BOOL CALLBACK DlgProc(HWND, UINT, WPARAM, LPARAM);

BOOL bWhoWon(HWND* hButton, HWND hWnd/*, WPARAM wParam*/);

int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPTSTR lpszCmdLine, int nCmdShow) {
	return DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), NULL, DLGPROC(DlgProc));
}

BOOL CALLBACK DlgProc(HWND hWnd, UINT uMessage, WPARAM wParam, LPARAM lParam) {
	switch (uMessage) {
		case WM_INITDIALOG:
		{
			hNewGameButton = GetDlgItem(hWnd, IDC_BUTTON10);
			for (int i = 0; i < ButtonNumber; i++) {
				hButtonArr[i] = GetDlgItem(hWnd, IDC_BUTTON1 +i);
				SendMessage(hButtonArr[i], WM_SETTEXT, 0, LPARAM(TEXT(" ")));
			}
			return TRUE;
		}
		case WM_CLOSE:
		{
			EndDialog(hWnd, 0);
			return TRUE;
		}
		case WM_COMMAND:
		{
			if (AnableFieldsNumber % 2 == 0) {
				SetWindowText(hWnd, TEXT("Noughts and crosses  X turn"));
			}
			else {
				SetWindowText(hWnd, TEXT("Noughts and crosses  O turn"));
			}
			if (LOWORD(wParam) >= IDC_BUTTON1 && LOWORD(wParam) <= IDC_BUTTON9 && AnableFieldsNumber>0) {
				INT Position = 0;
				for (; LOWORD(wParam)!=IDC_BUTTON1 + Position; Position++) {}//определение индекса нажатой кнопки


				if(AnableFieldsNumber%2 == 0) SendMessage(hButtonArr[Position], WM_SETTEXT, 0, LPARAM(TEXT("O")));
				else SendMessage(hButtonArr[Position], WM_SETTEXT, 0, LPARAM(TEXT("X")));
				AnableFieldsNumber--;
				EnableWindow(hButtonArr[Position], FALSE);
				Result = bWhoWon(hButtonArr, hWnd);
				if (Result == 1) EnableWindow(hNewGameButton, TRUE);//если кто-то победил
				else if (AnableFieldsNumber == 0) {
					EnableWindow(hNewGameButton, TRUE);
					MessageBox(hWnd, TEXT("Draw"), TEXT("The end"), MB_OK);
				}
			}
			else if (LOWORD(wParam) == IDC_BUTTON10) {//в случае новой игры
				for (int i = 0; i < ButtonNumber; i++) {
					SendMessage(hButtonArr[i], WM_SETTEXT, 0, LPARAM(TEXT(" ")));
					EnableWindow(hButtonArr[i], TRUE);
				}
				EnableWindow(hNewGameButton, FALSE);
				AnableFieldsNumber = 9;
			}
			break;
		}
		
	}
	return 0;
}

BOOL bWhoWon(HWND* hButton, HWND hWnd) {//использовать после каждого нажатия кнопки
	BOOL Result = FALSE;
	TCHAR FieldsVal[ButtonNumber] = {}, ch, adapt[2];
	INT RowSize = sqrt(ButtonNumber);
	for (int i = 0; i < ButtonNumber; i++) {//чтение текста кнопок в массив
		SendMessage(hButton[i], WM_GETTEXT, 2, LPARAM(adapt));
		FieldsVal[i] = adapt[0];
	}
	for (int i = 0; i < 3; i++) {
		switch (i) {
			case 0://проверка по рядам
			{
				for (int j = 0; j < RowSize; j++) {
					if (FieldsVal[0 + j* RowSize] == FieldsVal[1 + j * RowSize] && FieldsVal[0 + j * RowSize] == FieldsVal[2 + j * RowSize] && FieldsVal[0 + j * RowSize]!=32) {
						ch = FieldsVal[0 + j * RowSize];
						Result = TRUE;
						break;
					}
				}
				break;
			}
			case 1://проверка по столбцам
			{
				for (int j = 0; j < RowSize; j++) {
					if (FieldsVal[0 + j] == FieldsVal[3 + j] && FieldsVal[0 + j] == FieldsVal[6 + j] && FieldsVal[0 + j]!=32) {
						ch = FieldsVal[0+j];
						Result = TRUE;
						break;
					}
				}
				break;
			}
			case 2://проверка по диагоналям
			{
				if (((FieldsVal[0] == FieldsVal[4]&& FieldsVal[4] == FieldsVal[8]) || (FieldsVal[2] == FieldsVal[4] && FieldsVal[4] == FieldsVal[6])) && FieldsVal[4]!=32) {
					ch = FieldsVal[4];
					Result = TRUE;
				}
				break;
			}
		}
	}
	if (Result == TRUE) {
		for (int i = 0; i < RowSize * RowSize; i++) {
			EnableWindow(hButton[i], FALSE);
		}
		TCHAR buff[50];
		_stprintf_s(buff, TEXT("%c -palyer won! Congratulations :D"), ch);
		MessageBox(hWnd, buff, TEXT("The end"), MB_OK);
	}
	return Result;
}