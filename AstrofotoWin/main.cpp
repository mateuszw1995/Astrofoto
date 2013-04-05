#pragma comment(lib, "comctl32.lib")
#pragma comment(linker,"\"/manifestdependency:type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#define _WIN32_IE 0x0600
#define _WIN32_WINNT 0x0500
#include <Windows.h>
#include <commctrl.h>
#include <Shobjidl.h>
#include <stdlib.h>
#include "img/image.h"
int some_cool_function(int foo) {
	int bar = 2;
	return bar*23978;
}
HINSTANCE* hInst;
MSG msgs;
HWND hButton, hOkno, hEdit, hCombo, hProgress;
HFONT hFont;
CHAR szClassName[]="classname";
WORD a;
INT index;
BOOL Win7LubNowszy;
image img;
HBITMAP bitmapa; //uchwyt naszej bitmapy
BITMAP info_bitmapy; //struktura inforamcyjna bitmapy

LRESULT CALLBACK WndProc(HWND hwnd,UINT msg,WPARAM wPar,LPARAM lPar)
{
	PAINTSTRUCT ps;
	HBRUSH brush;
	HPEN pen;
	HDC hdc,hdcBitmapy;
	//PAINTSTRUCT ps;
	static DWORD g_wmTBC =( DWORD ) - 1;
    if( msg == g_wmTBC )
	{
		ITaskbarList3 * ptl;
		if( CoCreateInstance( CLSID_TaskbarList, NULL, CLSCTX_ALL, IID_ITaskbarList3,( LPVOID * ) & ptl ) != S_OK )
			MessageBox(0, "error", 0, 0);
		ptl->SetProgressState( hwnd, TBPF_INDETERMINATE );
		img.from_file("a.jpg", 3);
		UpdateWindow(hOkno);
		ptl->SetProgressState( hwnd, TBPF_NOPROGRESS );
    }
	else
    {
		switch(msg)
		{
		case WM_CREATE:
			g_wmTBC = RegisterWindowMessage( "TaskbarButtonCreated" );
			break;
		case WM_PAINT:
			MessageBox(0, "ble", 0, 0);
			hdc=BeginPaint(hwnd,&ps); //kontekst okna uzyskujemy w momencie odrysowywania
			hdcBitmapy=CreateCompatibleDC(hdc); //teraz musimy utworzyæ kontekst pamiêciowy dla bitmapy
			bitmapa=(HBITMAP)SelectObject(hdcBitmapy,bitmapa); //zamieniamy konteksty wi¹¿¹c bitmapê z kontekstem
			for(int y=0;y<info_bitmapy.bmHeight;y++)
				for(int x=0;x<info_bitmapy.bmWidth;x++)
					SetPixelV(hdcBitmapy,x,y,RGB(*img(x,y,0),*img(x,y,1),*img(x,y,2)));
			StretchBlt(hdc,0,0,1000,700,hdcBitmapy,0,0,info_bitmapy.bmWidth,info_bitmapy.bmHeight,SRCCOPY);
			bitmapa=(HBITMAP)SelectObject(hdcBitmapy,bitmapa); //z powrotem zamieniamy bitmapy w kontekœcie
			DeleteDC(hdcBitmapy); //usuwamy kontekst pamiêciowy bitmapy
			EndPaint(hwnd,&ps);
			break;
		case WM_COMMAND:
			break;
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		case WM_SYSCOMMAND:
			switch (wPar)
			{
				case SC_CLOSE:
					PostQuitMessage (0);
					break;
				case SC_MINIMIZE:
					break;
 
				case SC_MAXIMIZE:
					break;
			}
		default:
			return DefWindowProc(hwnd,msg,wPar,lPar);
		}
	}
	return 0;
}
#include <sstream>
using namespace std;
template <class T>
string str(T val) {
	ostringstream ss;
	ss << val;
	return ss.str();
}

INT WINAPI WinMain(HINSTANCE hInstance,HINSTANCE,LPSTR lStart,INT nShow)
{
	//wczytywanie bitmapy z dysku i pobranie informacji
	bitmapa=(HBITMAP)LoadImage(0,"bitmapa.bmp",IMAGE_BITMAP,0,0,LR_LOADFROMFILE);
	if(bitmapa==0)
	{
		MessageBox(0,"Nie mo¿na otworzyæ pliku bitmap.bmp z folderu z aplikacj¹","Brak pliku",MB_ICONERROR);
		return 0;
	}
	GetObject(bitmapa,sizeof(BITMAP),&info_bitmapy); //pobieramy informacje o bitmapie, potrzebujemy jej wymiarów
	//char buffer[33];
	//itoa(45645,buffer,10);
	MessageBox(0,str<int>(435).c_str(),"dupa dupa",MB_ICONERROR);
	hInst=&hInstance;
	WNDCLASSEX okno;
	okno.hInstance = *hInst;
	okno.lpszClassName = szClassName;
	okno.lpfnWndProc = WndProc;
	okno.cbSize = sizeof(WNDCLASSEX);
	okno.style = CS_DBLCLKS;
	okno.lpszMenuName = "Menu_glowne";
	okno.hIcon = LoadIcon (hInstance,"Ikona");
	okno.hIconSm=LoadIcon(hInstance,"Ikona");
	okno.hCursor = LoadCursor (NULL,IDC_ARROW);
	okno.hbrBackground = CreateSolidBrush(RGB(255,255,254));
	okno.cbClsExtra = 0;
	okno.cbWndExtra = 0;
	if(!RegisterClassEx(&okno))return 0;
	hOkno = CreateWindowEx(0,szClassName,"Astrofoto",WS_OVERLAPPEDWINDOW,100,100,1000,700,HWND_DESKTOP,NULL,hInstance,NULL);
	ShowWindow(hOkno,nShow);
	while(GetMessage(&msgs, NULL, 0, 0))
	{
		TranslateMessage(&msgs);
		DispatchMessage(&msgs);
	}
	DeleteObject(bitmapa);
	return msgs.wParam;
}