#include <Windows.h>
#include "res.h"
#include<time.h>
#include<stdlib.h>
#include<Windowsx.h>
#include<math.h>

#define ID_TIMER_MUCHA 2875
#define ID_TIMER_MOZG 2876
#define ID_TIMER_REBORN 2877

int iWindowWidth;
int iWindowHeight;

double dt=0.1;
double dMuchaIdiTime=0.5;
double dTimeReBorn=10;

HWND hwndButton;

class sMucha
{
public:
    int m_iMuchaX;
    int m_iMuchaY;
    int m_iMuchaAngle;
    int m_iMuchaPredkosc;
    bool m_bIsMuchaZywa;
    int m_iMuchaHeight;
    int m_iMuchaWidth;
    sMucha():m_bIsMuchaZywa(TRUE),m_iMuchaHeight(100),m_iMuchaWidth(100)
    {
        srand(time(NULL));
        m_iMuchaAngle=rand()%100;
        m_iMuchaPredkosc=150;
    }
    void ruch();
    void mozg();
    void narodziny();
    void display(HDC &hDC, HDC &hDCBitmap, HBITMAP &hBitmapMuchaZywa, HBITMAP &hBitmapMuchaTrup, HBITMAP &hBitmapMuchaZywaAlpha, HBITMAP &hBitmapMuchaTrupAlpha);
    void redraw(HDC hDC);
    bool click(int iMouseX, int iMouseY);

};


sMucha myMucha[10];

void sMucha::mozg()
{
    for (int i=0; i<10; i++)
    {
        m_iMuchaAngle+=rand()%10-5;
    }
}

void sMucha::ruch()
{
    if (m_bIsMuchaZywa)
    {
        m_iMuchaX += m_iMuchaPredkosc * cos(m_iMuchaAngle*3.1415 / 180)*dt;
        m_iMuchaY += m_iMuchaPredkosc * sin(m_iMuchaAngle*3.1415 / 180)*dt;
        if (m_iMuchaX > iWindowWidth - m_iMuchaWidth)
        {
            m_iMuchaAngle = (m_iMuchaAngle + 180) % 360;
            m_iMuchaX = iWindowWidth - m_iMuchaWidth;
        }
        if (m_iMuchaX < 0)
        {
            m_iMuchaAngle = (m_iMuchaAngle + 180) % 360;
        }
        if (m_iMuchaY > iWindowHeight - m_iMuchaHeight)
        {
            m_iMuchaAngle = (m_iMuchaAngle + 180) % 360;
            m_iMuchaY = iWindowHeight - m_iMuchaHeight;
        }
        if (m_iMuchaY < 0)
        {
            m_iMuchaAngle = (m_iMuchaAngle + 180) % 360;
        }
    }
    return;
}

void sMucha::narodziny()
{
    if (m_bIsMuchaZywa == FALSE)
        m_bIsMuchaZywa = TRUE;

    return;
}

void sMucha::display(HDC &hDC, HDC &hDCBitmap, HBITMAP &hBitmapMuchaZywa, HBITMAP &hBitmapMuchaTrup, HBITMAP &hBitmapMuchaZywaAlpha, HBITMAP &hBitmapMuchaTrupAlpha)
{
    if (m_bIsMuchaZywa)
    {
        SelectObject(hDCBitmap, hBitmapMuchaZywaAlpha);
    }
    else
    {
        SelectObject(hDCBitmap, hBitmapMuchaTrupAlpha);
    }
    BitBlt(hDC, m_iMuchaX, m_iMuchaY, m_iMuchaWidth, m_iMuchaHeight, hDCBitmap, 0, 0, SRCPAINT);
    if (m_bIsMuchaZywa)
    {
        SelectObject(hDCBitmap, hBitmapMuchaZywa);
    }
    else
    {
        SelectObject(hDCBitmap, hBitmapMuchaTrup);
    }
    BitBlt(hDC, m_iMuchaX, m_iMuchaY, m_iMuchaWidth, m_iMuchaHeight, hDCBitmap, 0, 0, SRCAND);
    return;
}

void sMucha::redraw(HDC hDC)
{
    Rectangle(hDC, m_iMuchaX - m_iMuchaPredkosc, m_iMuchaY - m_iMuchaPredkosc, m_iMuchaX + m_iMuchaWidth + m_iMuchaPredkosc, m_iMuchaY + m_iMuchaHeight + m_iMuchaPredkosc);
    Rectangle(hDC, 0, 0, iWindowWidth, iWindowHeight);
    return;
}

bool sMucha::click(int iMouseX, int iMouseY)
{
    if ((iMouseX > m_iMuchaX) && (iMouseX < m_iMuchaX + m_iMuchaWidth) && (iMouseY > m_iMuchaY) && (iMouseY < m_iMuchaY + m_iMuchaHeight))
    {
        return m_bIsMuchaZywa = false;
    }
}





HINSTANCE hInst;
INT_PTR CALLBACK DialogProc (HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch(uMsg)
    {
    case WM_SIZE:
        iWindowWidth=LOWORD(lParam);
        iWindowHeight=HIWORD(lParam);
        return FALSE;

    case WM_INITDIALOG:
    {
        {
            RECT rect;
            GetWindowRect(hwndDlg, &rect);
            iWindowWidth = abs(rect.right - rect.left) - 100;
            iWindowHeight = abs(rect.bottom - rect.top) - 100;
            hwndButton=CreateWindow("BUTTON","Reset",WS_CHILD | BS_PUSHBUTTON ,10,10,100,50,hwndDlg,NULL,hInst,NULL);
            ShowWindow(hwndButton,SW_SHOW);
            HRGN hrgnButton=CreateRoundRectRgn(0,0,iWindowWidth+200, iWindowHeight+200,20,20);
            HRGN hrgnElipse=CreateEllipticRgn(100,100,150,120);
            HRGN hrgnFinal=CreateRoundRectRgn(0,0,iWindowWidth+200, iWindowHeight+200,20,20);     //0,0,100,50,20,20
            POINT pointArray[6];
            pointArray[0].x=50;
            pointArray[0].y=0;
            pointArray[1].x=100;
            pointArray[1].y=50;
            pointArray[2].x=150;
            pointArray[2].y=0;
            pointArray[3].x=200;
            pointArray[3].y=50;
            pointArray[4].x=100;
            pointArray[4].y=200;
            pointArray[5].x=0;
            pointArray[5].y=50;
            hrgnFinal=CreatePolygonRgn(pointArray,6,WINDING);
            //CombineRgn(hrgnFinal,hrgnButton,hrgnElipse,RGN_DIFF);
            SetWindowRgn(hwndDlg,hrgnFinal,true);
            UpdateWindow(hwndDlg);                        //pozmieniac na hwndButton
        }
        return TRUE;
    }
    case WM_PAINT:
    {
        HDC hDC;
        hDC=GetDC(hwndDlg);

        SelectObject(hDC,GetStockObject(WHITE_BRUSH));
        SelectObject(hDC,GetStockObject(NULL_PEN));

        for (int i=0; i<10; i++)
        {
            myMucha[i].redraw(hDC);
        }
        HBITMAP hBitmapMuchaZywa;
        HBITMAP hBitmapMuchaTrup;
        HBITMAP hBitmapMuchaZywaAlpha;
        HBITMAP hBitmapMuchaTrupAlpha;
        hBitmapMuchaZywa=(HBITMAP)LoadImage(NULL,"..\\source\\zywa.bmp",IMAGE_BITMAP,0,0,LR_LOADFROMFILE);
        hBitmapMuchaTrup=(HBITMAP)LoadImage(NULL,"..\\source\\trup.bmp",IMAGE_BITMAP,0,0,LR_LOADFROMFILE);
        hBitmapMuchaZywaAlpha=(HBITMAP)LoadImage(NULL,"..\\source\\zywaAlpha.bmp",IMAGE_BITMAP,0,0,LR_LOADFROMFILE);
        hBitmapMuchaTrupAlpha=(HBITMAP)LoadImage(NULL,"..\\source\\trupAlpha.bmp",IMAGE_BITMAP,0,0,LR_LOADFROMFILE);
        HDC hDCBitmap;
        hDCBitmap=CreateCompatibleDC(hDC);
        for(int i=0; i<10; i++)
        {
            myMucha[i].display(hDC, hDCBitmap, hBitmapMuchaZywa, hBitmapMuchaTrup,hBitmapMuchaZywaAlpha, hBitmapMuchaTrupAlpha);
        }
        DeleteDC(hDCBitmap);
        DeleteObject(hBitmapMuchaZywa);
        DeleteObject(hBitmapMuchaTrup);
        ReleaseDC(hwndDlg,hDC);
    }
    break;
    return TRUE;
    case WM_LBUTTONDOWN:
    {
        for(int i=0; i<10; i++)
        {
            int iMouseX=GET_X_LPARAM(lParam);
            int iMouseY=GET_Y_LPARAM(lParam);
            if((iMouseX>myMucha[i].m_iMuchaX)&&(iMouseX<myMucha[i].m_iMuchaX+myMucha[i].m_iMuchaWidth)&&(iMouseY>myMucha[i].m_iMuchaY)&&(iMouseY<myMucha[i].m_iMuchaY+myMucha[i].m_iMuchaHeight))
            {
                myMucha[i].m_bIsMuchaZywa=false;
            }
            RedrawWindow(hwndDlg,NULL,NULL,RDW_INVALIDATE);
        }
    }
    return TRUE;
    case WM_TIMER:
        switch(wParam)
        {
        case ID_TIMER_MUCHA:
            for(int i=0; i<10; i++)
                myMucha[i].ruch();
            RedrawWindow(hwndDlg,NULL,NULL,RDW_INVALIDATE);
            return TRUE;


        case ID_TIMER_MOZG:
            for(int i=0; i<10; i++)
            {
                myMucha[i].mozg(); //liczby z przedzialu od 0 do 4
            }
            return TRUE;
        case ID_TIMER_REBORN:
        {
            for (int i = 0; i < 10; i++)
            {
                myMucha[i].narodziny();
            }
            return TRUE;

        }
        }
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
    SetTimer(hwndMainWindow,ID_TIMER_MUCHA,dt*1000,NULL);
    SetTimer(hwndMainWindow,ID_TIMER_MOZG,dMuchaIdiTime*1000,NULL);
    SetTimer(hwndMainWindow,ID_TIMER_REBORN,dTimeReBorn*1000,NULL);
    //HBRUSH brush=CreateSolidBrush(RGB(0,0,255));
    // SetClassLongPtr(hwndMainWindow,GLCP_HBRACKGROUND,(LONG)brush);
    ShowWindow(hwndMainWindow, iCmdShow);
    hInst=hinstance;
    MSG msg= {};
    while(GetMessage(&msg, NULL,0,0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return 0;
}
