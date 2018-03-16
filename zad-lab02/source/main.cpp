#include<string>
#include <Windows.h>
#include "res.h"
#include <ctime>
#include <cstdlib>

int a = 0;
int x=0;
void test(int a, int b,int x)
{           
  char txt[100];
	sprintf_s(txt, "Liczba prob = %d", x);
//	MessageBox(0,txt,txt, MB_OK);
    if (a == b)
    {       sprintf_s(txt, "Liczba prob = %d", x);
        MessageBox(0,txt,"Wygrales!!!" , MB_OK);
     
    }
    else if (a > b)
    {
        MessageBox(0, "Moja liczba jest wieksza niz Twoja", "Gra", MB_OK);
    }
    else
    {
        MessageBox(0, "Moja liczba jest mniejsza niz Twoja", "Gra", MB_OK);
    }
    return;
}

INT_PTR CALLBACK DialogProc (HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{        
    switch(uMsg)
    {
    case WM_COMMAND:
        switch (HIWORD(wParam))
        {
        case BN_CLICKED:
          
            switch(LOWORD(wParam))
            {
            case IDC_BUTTON1:
                HWND hwndEditBox=GetDlgItem(hwndDlg, IDC_EDIT1);
                int iTextLength=GetWindowTextLength(hwndEditBox);
                CHAR szText[500];
                GetWindowText(hwndEditBox,szText,iTextLength+1);
                // SetWindowText((HWND)lParam,szText);
                int b = 0;
                int c = 1;

                for (int i = iTextLength-1; i >= 0; i--)
                {
                    int temp = szText[i] - '0';
                    b = b + temp* c;
                    c = c * 10;
                    
                }
                x++;
                c = 1;
                test(a, b,x);
                return TRUE;
            }

        }
        return FALSE;
    case WM_CLOSE:
        DestroyWindow(hwndDlg);
        PostQuitMessage(0);
        return TRUE;
    }
    return FALSE;
}
int WINAPI WinMain(HINSTANCE hinstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
    srand(time(NULL));
    a = rand()%40 + 1;
    HWND hwndMainWindow = CreateDialog(hinstance, MAKEINTRESOURCE(IDD_MAINVIEW),NULL,DialogProc);
    ShowWindow(hwndMainWindow, iCmdShow);

    MSG msg= {};
    while(GetMessage(&msg, NULL,0,0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return 0;
}