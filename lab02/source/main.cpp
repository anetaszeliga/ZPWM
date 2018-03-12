#include <Windows.h>
#include "res.h"
INT_PTR CALLBACK DialogProc (HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
switch(uMsg)
{
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