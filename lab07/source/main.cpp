#include <Windows.h>
#include "res.h"
#include <math.h>
HINSTANCE hInst;
#pragma comment(lib,"winmm.lib")
INT_PTR CALLBACK DialogProc (HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
switch(uMsg)
{
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
  SetForegroundWindow(hwndMainWindow);
  SetFocus(hwndMainWindow);

  WAVEFORMATEX pcmWaveFormat;
  pcmWaveFormat.wFormatTag=WAVE_FORMAT_PCM;
  pcmWaveFormat.nChannels=1;
  pcmWaveFormat.nSamplesPerSec=44100L;
  pcmWaveFormat.wBitsPerSample=8;
  pcmWaveFormat.nAvgBytesPerSec=44100L;
  pcmWaveFormat.nBlockAlign=1;
  pcmWaveFormat.cbSize=0;

  MMRESULT mmResult;
  HWAVEOUT hwo;
  UINT devId;

  for(devId=0;devId<waveOutGetNumDevs();devId++)
  {
    mmResult=waveOutOpen(&hwo,devId,&pcmWaveFormat,0,0,CALLBACK_NULL);
    if(mmResult==MMSYSERR_NOERROR);
  }
  if(mmResult!=MMSYSERR_NOERROR)
  {
    MessageBox(hwndMainWindow,TEXT("Nie znaleziono karty dzwiękowej o wymaganuch parametrach"),TEXT("Error"),MB_OK);
    return mmResult;
  }
  WAVEHDR whdr;
  ZeroMemory(&whdr,sizeof(WAVEHDR));
  whdr.lpData=new char[pcmWaveFormat.nAvgBytesPerSec*1];
  whdr.dwBufferLength=pcmWaveFormat.nAvgBytesPerSec*1;
  whdr.dwUser=0;
  whdr.dwFlags=0;
  whdr.dwLoops=0;
  whdr.dwBytesRecorded=0;
  whdr.lpNext=0;
  whdr.reserved=0;

  for(int i=0;i<whdr.dwBufferLength;i++)
  {
    whdr.lpData[i]=128*sin(i*880.0*2*3.14/(double)pcmWaveFormat.nSamplesPerSec)+128;
  }

  waveOutSetVolume(hwo,0xFFFFFFFF);
  mmResult=waveOutPrepareHeader(hwo,&whdr,sizeof(WAVEHDR));
  if(mmResult!=MMSYSERR_NOERROR)
  {
    MessageBox(hwndMainWindow,TEXT("Nie można zainicjować karty"),TEXT("Error"),MB_OK);
    return mmResult;
  }
  mmResult=waveOutWrite(hwo,&whdr,sizeof(WAVEHDR));
  if(mmResult!=MMSYSERR_NOERROR)
  {
    MessageBox(hwndMainWindow,TEXT("Nie załadowano próbek"),TEXT("Error"),MB_OK);
    return mmResult;
  }

  mmResult=waveOutUnprepareHeader(hwo,&whdr,sizeof(WAVEHDR));
  mmResult=waveOutClose(hwo);
  delete[] whdr.lpData;

  hInst=hinstance;
  MSG msg={};
  while(GetMessage(&msg, NULL,0,0))
  {
  TranslateMessage(&msg);
  DispatchMessage(&msg);
  }
  return 0;
}