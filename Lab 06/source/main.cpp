#include <Windows.h> 
#pragma comment(lib,"opengl32.lib")
#pragma comment(lib,"glu32.lib")
#pragma comment(lib,"glew32.lib")
#include "res.h"
#include <GL\glew.h>
#include<gl\GL.h>
#include<gl\GLU.h>
#include <math.h>
#include <stdio.h>
#define GL_GLEXT_PROTOTYPES

unsigned char* ReadBmpFromFile(const char* szFileName, int &riWidth, int &riHeight)
{
  BITMAPFILEHEADER     bfh;
  BITMAPINFOHEADER     bih;

  int                i, j, h, v, lev, l, ls;
  unsigned char*     buff = NULL;

  unsigned char* p_palette = NULL;
  unsigned short n_colors = 0;

  unsigned char* pRGBBuffer = NULL;

  FILE* hfile;
  fopen_s(&hfile,szFileName,  "rb");

  if (hfile != NULL)
  {
    fread(&bfh, sizeof(bfh), 1, hfile);
    if (!(bfh.bfType != 0x4d42 || (bfh.bfOffBits < (sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER)))))
    {
      fread(&bih, sizeof(bih), 1, hfile);
      v = bih.biWidth;
      h = bih.biHeight;
      lev = bih.biBitCount;

      riWidth = v;
      riHeight = h;
      pRGBBuffer = new unsigned char[riWidth*riHeight * 3]; //Zaalokowanie odpowiedniego buffora obrazu

                                                            //Za³aduj Palete barw jesli jest
      if ((lev == 1) || (lev == 4) || (lev == 8))
      {
        n_colors = 1 << lev;
        p_palette = new unsigned char[4 * n_colors];
        fread(p_palette, 4 * n_colors, 1, hfile);
      }

      fseek(hfile, bfh.bfOffBits, SEEK_SET);

      buff = new unsigned char[v * 4];

      switch (lev)
      {
      case 1:
        //Nie obs³ugiwane
        break;
      case 4:
        //nie Obs³ugiwane
        break;
      case 8: //Skala szaroœci
        ls = (v + 3) & 0xFFFFFFFC;
        for (j = (h - 1); j >= 0; j--)
        {
          fread(buff, ls, 1, hfile);
          for (i = 0, l = 0; i<v; i++)
          {
            pRGBBuffer[((j*riWidth) + i) * 3 + 2] = p_palette[(buff[i] << 2) + 2];//R
            pRGBBuffer[((j*riWidth) + i) * 3 + 1] = p_palette[(buff[i] << 2) + 1];//G
            pRGBBuffer[((j*riWidth) + i) * 3 + 0] = p_palette[(buff[i] << 2) + 0];//B
          }
        };
        break;
      case 24:
        //bitmapa RGB
        ls = (v * 3 + 3) & 0xFFFFFFFC;
        for (j = (h - 1); j >= 0; j--)
        {
          //x_fread(hfile,buff,ls);
          fread(buff, ls, 1, hfile);
          for (i = 0, l = 0; i<v; i++, l += 3)
          {
            pRGBBuffer[((j*riWidth) + i) * 3 + 0] = buff[l + 0];
            pRGBBuffer[((j*riWidth) + i) * 3 + 1] = buff[l + 1];
            pRGBBuffer[((j*riWidth) + i) * 3 + 2] = buff[l + 2];
          };
        };
        break;
      case 32:
        // RGBA bitmap 
        for (j = (h - 1); j >= 0; j--)
        {
          fread(buff, v * 4, 1, hfile);
          for (i = 0, l = 0; i<v; i++, l += 4)
          {
            pRGBBuffer[((j*riWidth) + i) * 3 + 0] = buff[l + 0];
            pRGBBuffer[((j*riWidth) + i) * 3 + 1] = buff[l + 1];
            pRGBBuffer[((j*riWidth) + i) * 3 + 2] = buff[l + 2];
          }
        };
        break;
      };
      delete buff;
      if (p_palette) delete p_palette;

    }
  }
  return pRGBBuffer;
}

INT_PTR CALLBACK DialogProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  switch (uMsg)
  {
  case WM_SIZE:
  {
    int width = LOWORD(lParam);
    int height = HIWORD(lParam);
    glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (float)width / height, 0.1, 1000.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    return TRUE;
  }
  case WM_CLOSE:
    DestroyWindow(hwndDlg);
    PostQuitMessage(0);
    return TRUE;
  }
  return FALSE;
}
void Axes(float a)
{
  glBegin(GL_LINES);

  //pierwsza linia
  glColor3f(0.0, 0.0, 0.0);
  glVertex3f(+a, 0, 0);
  glVertex3f(-a, 0, 0);

  //druga linia
  glColor3f(0.0, 1.0, 0.0);
  glVertex3f(0, +a, 0);
  glVertex3f(0, -a, 0);

  //trzecia linia
  glColor3f(0.0, 0.0, 1.0);
  glVertex3f(0, 0, +a);
  glVertex3f(0, 0, -a);

  glEnd();
}



void Box(float a, float c, float b)
{
  glBegin(GL_QUADS);
  //glColor3f(1.0, 1.0, 0.1);
  glTexCoord2f(0.0,0.0);
  glVertex3f(-a / 2, -c / 2, -b / 2);

  glTexCoord2f(1.0, 0.0);
  glVertex3f(+a / 2, -c / 2, -b / 2);

  glTexCoord2f(1.0, 1.0);
  glVertex3f(+a / 2, +c / 2, -b / 2);

  glTexCoord2f(0.0, 1.0);
  glVertex3f(-a / 2, +c / 2, -b / 2);

  //glColor3f(0.0, 1.0, 0.0);
  glTexCoord2f(1.0, 0.0);
  glVertex3f(+a / 2, +c / 2, -b / 2);

  glTexCoord2f(1.0, 1.0);
  glVertex3f(+a / 2, +c / 2, +b / 2);

  glTexCoord2f(0.0, 1.0);
  glVertex3f(+a / 2, -c / 2, +b / 2);

  glTexCoord2f(0.0, 0.0);
  glVertex3f(+a / 2, -c / 2, -b / 2);

  //glColor3f(0.0, 1.0, 1.0);
  glTexCoord2f(0.0, 0.0);
  glVertex3f(-a / 2, +c / 2, -b / 2);

  glTexCoord2f(0.0, 1.0);
  glVertex3f(-a / 2, +c / 2, +b / 2);

  glTexCoord2f(1.0, 1.0);
  glVertex3f(+a / 2, +c / 2, +b / 2);

  glTexCoord2f(1.0, 0.0);
  glVertex3f(+a / 2, +c / 2, -b / 2);

  //glColor3f(1.0, 1.0, 0.0);
  glTexCoord2f(0.0, 0.0);
  glVertex3f(-a / 2, -c / 2, +b / 2);

  glTexCoord2f(1.0, 0.0);
  glVertex3f(+a / 2, -c / 2, +b / 2);

  glTexCoord2f(1.0, 1.0);
  glVertex3f(+a / 2, +c / 2, +b / 2);

  glTexCoord2f(0.0, 1.0);
  glVertex3f(-a / 2, +c / 2, +b / 2);

  //glColor3f(1.0, 0.0, 1.0);
  glTexCoord2f(1.0, 0.0);
  glVertex3f(-a / 2, +c / 2, -b / 2);

  glTexCoord2f(1.0, 1.0);
  glVertex3f(-a / 2, +c / 2, +b / 2);

  glTexCoord2f(0.0, 1.0);
  glVertex3f(-a / 2, -c / 2, +b / 2);

  glTexCoord2f(0.0, 0.0);
  glVertex3f(-a / 2, -c / 2, -b / 2);

  //glColor3f(1.0, 1.0, 0.0);
  glTexCoord2f(0.0, 0.0);
  glVertex3f(-a / 2, -c / 2, -b / 2);

  glTexCoord2f(0.0, 1.0);
  glVertex3f(-a / 2, -c / 2, +b / 2);

  glTexCoord2f(1.0, 1.0);
  glVertex3f(+a / 2, -c / 2, +b / 2);

  glTexCoord2f(1.0, 0.0);
  glVertex3f(+a / 2, -c / 2, -b / 2);
  glEnd();
}

int iStartTime; 
GLuint iMyTextureEye; 
GLuint iMyTextureBody;
GLuint iMyTextureLegs;
GLuint iMyTextureWings;

void PrepareTextures() {
  glEnable(GL_TEXTURE_2D);
  //tworzenie tekstury
  glGenTextures(1, &iMyTextureEye);
  glGenTextures(1, &iMyTextureBody);
  glGenTextures(1, &iMyTextureLegs);
  glGenTextures(1, &iMyTextureWings);

  //ladowanie z pliku 
  int iWidth; 
  int iHeight; 
  unsigned char* pBMPEye = ReadBmpFromFile("Eye.bmp", iWidth, iHeight); 
  glBindTexture(GL_TEXTURE_2D, iMyTextureEye); 
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, iWidth, iHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, pBMPEye); 
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); 
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
}

void DrawFly(float fSize) {
  glPushMatrix();
  glScalef(fSize, fSize, fSize); 
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, iMyTextureEye);  
  //glColor3f(0.2, 0.2, 0.2); 
  Box(1.5, 1.0, 2.0);
  glPushMatrix();
  glTranslatef(-1.5 / 2+1.5/3/2,
              -1.0 / 2 - 1.0/3/2,
              -2.0/ 2+2.0/5/2);
  glColor3f(0.3, 0.3, 0.3);
  Box(1.5 / 3, 1.0 / 3, 2.0 / 5);
  glPopMatrix();

  glPushMatrix();
  glTranslatef(+1.5 / 2 - 1.5 / 3 / 2,
               -1.0 / 2 - 1.0 / 3 / 2,
               -2.0 / 2 + 2.0 / 5 / 2);
  Box(1.5 / 3, 1.0 / 3, 2.0 / 5);
  glPopMatrix();

  glPushMatrix();
  glTranslatef(-1.5 / 2 + 1.5 / 3 / 2,
               -1.0 / 2 - 1.0 / 3 / 2,
               +2.0 / 2 - 2.0 / 5 / 2);
  Box(1.5 / 3, 1.0 / 3, 2.0 / 5);
  glPopMatrix();

  glPushMatrix();
  glTranslatef(+1.5 / 2 - 1.5 / 3 / 2,
               -1.0 / 2 - 1.0 / 3 / 2,
               +2.0 / 2 - 2.0 / 5 / 2);
  Box(1.5 / 3, 1.0 / 3, 2.0 / 5);
  glPopMatrix();

  glPushMatrix();
  glTranslatef(-1.5 / 2 + 1.5 / 3 / 2,
               -1.0 / 2 - 1.0 / 3 / 2,
                0);
  Box(1.5 / 3, 1.0 / 3, 2.0 / 5);
  glPopMatrix();

  glPushMatrix();
  glTranslatef(+1.5 / 2 - 1.5 / 3 / 2,
               -1.0 / 2 - 1.0 / 3 / 2,
               0);
  Box(1.5 / 3, 1.0 / 3, 2.0 / 5);
  glPopMatrix();

  //oczy
  glPushMatrix();
  glTranslatef(-1.5 / 2, +1.0 / 2, -2.0 / 2); 
  glColor3f(0.5, 0.5, 0.5);
  Box(1.1, 1.1, 1.1); 
  glPopMatrix();

  glPushMatrix();
  glTranslatef(+1.5 / 2, +1.0 / 2, -2.0 / 2);
  glColor3f(0.5, 0.5, 0.5);
  Box(1.1, 1.1, 1.1);
  glPopMatrix();

  float fAngleFly = 0.0; 
  fAngleFly = 10.0*sin((GetTickCount() - iStartTime)*2*3.141592/1000); 
  //skrzyd³o
  glPushMatrix();
  glTranslatef(-1.5 / 2,
    +1.0 / 2, .0); 
  glRotatef(-45.0-fAngleFly, 0.0, 1.0, 0.0); 
  glTranslatef(0.0, 0.0, 1.0);
  glColor3f(0.25, 0.25, 0.25);
  Box(1.4, 0.2, 2.0); 
  glPopMatrix(); 

  glPushMatrix();
  glTranslatef(-1.5 / 2,
    +1.0 / 2, .0);
  glRotatef(+45.0+fAngleFly, 0.0, 1.0, 0.0);
  glTranslatef(0.0, 0.0, 1.0);
  glColor3f(0.25, 0.25, 0.25);
  Box(1.4, 0.2, 2.0);
  glPopMatrix();
  glPopMatrix();
}

void DrawScene()
{

  glClearColor(1.0, 0.0, 0.0, 0.0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glEnable(GL_DEPTH_TEST);

  glLoadIdentity();
  glTranslatef(0, 5, -10.0);

  //Axes(5.0);
  glTranslatef(0, -5, -10.0);
  
  static float fangle = 0.0;
  fangle += 0.01; 
  glRotatef(fangle, 0, 1, 0);
  //Box(2.0, 2.0, 2.0);
  //Axes(10.0); 

  DrawFly(1.0);
  
}




int WINAPI WinMain(HINSTANCE hinstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
  iStartTime = GetTickCount(); 
  HWND hwndMainWindow = CreateDialog(hinstance, MAKEINTRESOURCE(IDD_MAINVIEW), NULL, DialogProc);  //DialogProc ost
  static PIXELFORMATDESCRIPTOR pfd =
  {
    sizeof(PIXELFORMATDESCRIPTOR),
    1,
    PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
    PFD_TYPE_RGBA,
    16,
    0,0,0,0,0,0,
    0,0,0,
    0,0,0,0,
    16,
    0,0,
    PFD_MAIN_PLANE,
    0,
    0,0,0
  };
  HDC hDC = GetDC(hwndMainWindow);
  GLuint PixelFormat = ChoosePixelFormat(hDC, &pfd);
  SetPixelFormat(hDC, PixelFormat, &pfd);

  HGLRC hRC = wglCreateContext(hDC);
  wglMakeCurrent(hDC, hRC);

  ShowWindow(hwndMainWindow, iCmdShow);
  PrepareTextures(); 

  MSG msg = {};
  /*  while(GetMessage(&msg, NULL,0,0))
  {
  TranslateMessage(&msg);
  DispatchMessage(&msg);
  }
  */

  bool done = false;
  while (!done)
  {
    if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
    {
      //obsluga komunikatu
      if (msg.message == WM_QUIT) done = true;
      TranslateMessage(&msg);
      DispatchMessage(&msg);
    }
    else
    {
      //rysuj
      DrawScene();
      SwapBuffers(hDC);
    }
  }
  wglMakeCurrent(NULL, NULL);
  wglDeleteContext(hRC);
  ReleaseDC(hwndMainWindow, hDC);
  return 0;
}