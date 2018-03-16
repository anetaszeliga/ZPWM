#include <Windows.h>
#include "res.h"
HINSTANCE hInst;
INT_PTR CALLBACK DialogProc (HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
switch(uMsg)
{
case WM_PAINT:
  HDC hDC;
  hDC=GetDC(hwndDlg);

  HBITMAP hBitmap;
  hBitmap=LoadBitmap(hInst,MAKEINTRESOURCE(IDB_BITMAP1));

  HDC hDCBitmap;
  hDCBitmap=CreateCompatibleDC(hDC);
  SelectObject(hDCBitmap,hBitmap);
  BitBlt(hDC,0,0,900,400,hDCBitmap,0,0,SRCCOPY);
  DeleteDC(hDCBitmap);
  DeleteObject(hBitmap);

  SelectObject(hDC,GetStockObject(ANSI_VAR_FONT));
  SetTextColor(hDC,RGB(255,0,0));
  TextOut(hDC,0,0,TEXT("Text output to client area."),27);
  ReleaseDC(hwndDlg,hDC);
  break;
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
  hInst=hinstance;
  MSG msg={};
  while(GetMessage(&msg, NULL,0,0))
  {
  TranslateMessage(&msg);
  DispatchMessage(&msg);
  }
  return 0;
}