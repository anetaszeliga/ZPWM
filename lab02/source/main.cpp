#include <Windows.h>
#include "res.h"
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
    MessageBox(hwndDlg,TEXT("Test"),TEXT("Klikniecie"),MB_OK);
    HWND hwndEditBox=GetDlgItem(hwndDlg, IDC_EDIT1);
    int iTextLength=GetWindowTextLength(hwndEditBox);
    CHAR szText[500];
    GetWindowText(hwndEditBox,szText,iTextLength+1);
    SetWindowText((HWND)lParam,szText);
    return TRUE;
  }
  
}
return FALSE;
case WM_LBUTTONDOWN:
    CHAR szText[200];
    wsprintf(szText, "Kliknales myszka x=%d,y=%d",LOWORD(lParam),HIWORD(lParam));
    MessageBox(hwndDlg,szText, TEXT("Klikniecie"),MB_OK);
    return TRUE;
case WM_CLOSE:
  DestroyWindow(hwndDlg);
  PostQuitMessage(0);
  return TRUE;
}
return FALSE;
}
  int WINAPI WinMain(HINSTANCE hinstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow) 
{
  HWND hwndMainWindow = CreateDialog(hinstance, MAKEINTRESOURCE(IDD_MAINVIEW),NULL,DialogProc);
  ShowWindow(hwndMainWindow, iCmdShow);

  MSG msg={};
  while(GetMessage(&msg, NULL,0,0))
  {
  TranslateMessage(&msg);
  DispatchMessage(&msg);
  }
  return 0;
}