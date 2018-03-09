#include<string>
#include <Windows.h>

void find(int a, int b) {
int c = (a + b) / 2;

char txt[100];

if (a == b) {
sprintf_s(txt, "Twoja liczba to  %d !", c);
MessageBox(0, txt, "Aplikacja", MB_OK);
return;
}

sprintf_s(txt, "Czy twoja liczba jest wieksza %d ?", c);
int odp = MessageBox(0, txt,"Aplikacja", MB_YESNO);

if (odp == IDYES) {
find(c + 1, b);
}
else {
find(a, c);
}
return;
}

int WINAPI WinMain(HINSTANCE hinstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow) {
int min = 1;
int max = 40;
MessageBox(0, "Wybierz liczbe od 1-40", "Aplikacja", MB_OK);
find(min, max);
return 0;
}