// win01.cpp

#define HH 10

#include <Windows.h>
#include <iostream>

//#include "resource.h"

#include <time.h>


#define ID_MYTIMER 100
#define ID_MYTIMER2 200





LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
ATOM InitApp(HINSTANCE);
BOOL InitInstance(HINSTANCE, int);
HFONT MyCreateFont(int, DWORD, LPCTSTR);
void Data(int);
void EData(int);
void Ftime(int, HWND, RECT, HDC);

LPCTSTR lpszClassName = TEXT("win01");	//ウィンドウクラス


HINSTANCE hInst;

int WINAPI WinMain(HINSTANCE hCurInst, HINSTANCE hPrevInst,
	LPSTR lpsCmdLine, int nCmdShow)
{

	hInst = hCurInst;


	MSG msg;
	BOOL bRet;

	if (!InitApp(hCurInst))
		return FALSE;
	if (!InitInstance(hCurInst, nCmdShow))
		return FALSE;
	while ((bRet = GetMessage(&msg, NULL, 0, 0)) != 0) {
		if (bRet == -1) {
			break;
		}
		else {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	return (int)msg.wParam;
}

//ウィンドウ・クラスの登録

ATOM InitApp(HINSTANCE hInst)
{
	WNDCLASSEX wc;
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WndProc;	//プロシージャ名
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInst;//インスタンス
	wc.hIcon = (HICON)LoadImage(NULL,
		MAKEINTRESOURCE(IDI_APPLICATION),
		IMAGE_ICON,
		0,
		0,
		LR_DEFAULTSIZE | LR_SHARED);
	wc.hCursor = (HCURSOR)LoadImage(NULL,
		MAKEINTRESOURCE(IDC_ARROW),
		IMAGE_CURSOR,
		0,
		0,
		LR_DEFAULTSIZE | LR_SHARED);
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName = TEXT("MYMENU");	//メニュー名
	wc.lpszClassName = lpszClassName;
	wc.hIconSm = (HICON)LoadImage(NULL,
		MAKEINTRESOURCE(IDI_APPLICATION),
		IMAGE_ICON,
		0,
		0,
		LR_DEFAULTSIZE | LR_SHARED);

	return (RegisterClassEx(&wc));
}

//ウィンドウの生成
HWND hWnd;
BOOL InitInstance(HINSTANCE hInst, int nCmdShow)
{

	int dispx = GetSystemMetrics(SM_CXSCREEN);
	int dispy = GetSystemMetrics(SM_CYSCREEN);



	hWnd = CreateWindow(lpszClassName,
		TEXT("社会部　文化祭展示　株価格推移"), //タイトルバーに表示されます
		WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX, //ウィンドウの種類
		CW_USEDEFAULT,	//Ｘ座標
		CW_USEDEFAULT,	//Ｙ座標
		dispx,	//幅
		dispy,	//高さ
		NULL, //親ウィンドウのハンドル、親を作るときはNULL
		NULL, //メニューハンドル、クラスメニューを使うときはNULL
		hInst, //インスタンスハンドル
		NULL);
	if (!hWnd)
		return FALSE;
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);
	return TRUE;
}
int Spa;
static int M = 1;
static int A, B, C, D, E;
//ウィンドウプロシージャ
static int price[5][HH + 1];/*株価の数+1*/
int his = HH;
static int EV = 0;
TCHAR S[100];
TCHAR Sb[30];
static int F = 1;
static int number = 6;/*株価の数*/
int spread[5] = { 10, 20, 30, 40, 50 };
int DI[5];
static POINT pt[5][HH];
int rR, rG, rB;
int SP;
int NE, nNE, mNE;
HBRUSH hBrushw, hBrush;
static HFONT hFont1, hFont2, hFont3;
static HFONT hFont4, hFont5, hFont6;
PAINTSTRUCT ps;
HDC hdc;
int Tm, DTM;
static TCHAR lpOut[10];
TCHAR szPR[25][10];
TCHAR Rate[15], Yrate[4];
TCHAR Urate[] = TEXT("為替レート\n次回予測");
TCHAR CON[] = TEXT("\n今からの取引はできません。\n少々お待ちください。");
TCHAR YOM[] = TEXT("\n読み込み中です。\n少々お待ちください。");
int TEST = 1;

int Rmax = 135;
int Rmin = 85;
int Rpr = 110;
int Spr = 110;
int nD = 98;
int nLOOP[130];


RECT rc23, rc24, rc25, rc26;
int CT;
static int AAB, XO;
HPEN hPen4, hPen5;
int Qw;
int DQ = 0;
int EnD = 10;
static int AS[5];

static HBITMAP hBmp;
BITMAP bmp_info;
static HDC hdc_mem;
int bmpw, bmph;
HPEN hPen10;
int YXO;

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp)
{


	RECT rc;
	RECT rc0, rc1, rc2, rc3;
	RECT rc4, rc6;
	RECT rc7, rc8, rc9, rc10;
	RECT rc11, rc12, rc13, rc14;
	RECT rc15, rc16, rc17, rc18;
	RECT rc19, rc20, rc21, rc22;




	int i, q;
	i = 0;
	q = 0;
	int o;
	int x;
	static int test = 100;
	int id;
	SYSTEMTIME st;



	static int Max, min;
	static int bpty[10];



	static int XX;




	/*数字処理start*/


	GetClientRect(hWnd, &rc);
	/*
	rc0.left=0;
	rc0.top=rc.bottom/16;
	rc0.right=rc.right/number;
	rc0.bottom=rc.bottom/9;
	rc1.left=rc0.right;
	rc1.top=rc0.top;
	rc1.right=rc.right/number*2;
	rc1.bottom=rc0.bottom;
	rc2.top=2;
	rc2.left=2;
	rc2.bottom=rc0.top-rc.bottom/80;
	rc2.right=rc1.right;
	rc3.top=rc0.bottom+rc.bottom/40;
	rc3.left=rc0.left+rc.right/80;
	rc3.bottom=rc.bottom/2-rc.bottom/40;
	rc3.right=rc1.right-rc.right/80;
	rc7.left=rc.right*2/3;
	rc7.top=rc.bottom/16;
	rc7.right=rc7.left+rc.right/number;
	rc7.bottom=rc.bottom/9;
	rc8.left=rc7.right;
	rc8.top=rc7.top;
	rc8.right=rc.right*2/3+rc.right/number*2;
	rc8.bottom=rc0.bottom;
	rc9.top=2;
	rc9.left=2+rc.right*2/3;
	rc9.bottom=rc0.top-rc.bottom/80;
	rc9.right=rc8.right;
	rc10.top=rc0.bottom+rc.bottom/40;
	rc10.left=rc0.left+rc.right/80+rc.right*2/3;
	rc10.bottom=rc.bottom/2-rc.bottom/40;
	rc10.right=rc1.right-rc.right/80+rc.right*2/3;
	rc11.left=0;
	rc11.top=rc.bottom/16+rc.bottom/2;
	rc11.right=rc.right/number;
	rc11.bottom=rc.bottom/9+rc.bottom/2;
	rc12.left=rc0.right;
	rc12.top=rc0.top+rc.bottom/2;
	rc12.right=rc.right/number*2;
	rc12.bottom=rc0.bottom+rc.bottom/2;
	rc13.top=2+rc.bottom/2;
	rc13.left=2;
	rc13.bottom=rc0.top-rc.bottom/80+rc.bottom/2;
	rc13.right=rc1.right;
	rc14.top=rc0.bottom+rc.bottom/40+rc.bottom/2;
	rc14.left=rc0.left+rc.right/80;
	rc14.bottom=rc.bottom/2-rc.bottom/40+rc.bottom/2;
	rc14.right=rc1.right-rc.right/80;
	rc15.left=0+rc.right/3;
	rc15.top=rc.bottom/16+rc.bottom/2;
	rc15.right=rc.right/number+rc.right/3;
	rc15.bottom=rc.bottom/9+rc.bottom/2;
	rc16.left=rc0.right+rc.right/3;
	rc16.top=rc0.top+rc.bottom/2;
	rc16.right=rc.right/number*2+rc.right/3;
	rc16.bottom=rc0.bottom+rc.bottom/2;
	rc17.top=2+rc.bottom/2;
	rc17.left=2+rc.right/3;
	rc17.bottom=rc0.top-rc.bottom/80+rc.bottom/2;
	rc17.right=rc1.right+rc.right/3;
	rc18.top=rc0.bottom+rc.bottom/40+rc.bottom/2;
	rc18.left=rc0.left+rc.right/80+rc.right/3;
	rc18.bottom=rc.bottom/2-rc.bottom/40+rc.bottom/2;
	rc18.right=rc1.right-rc.right/80+rc.right/3;
	rc19.left=rc7.left;
	rc19.top=rc.bottom/16+rc.bottom/2;
	rc19.right=rc7.right;
	rc19.bottom=rc.bottom/9+rc.bottom/2;
	rc20.left=rc8.left;
	rc20.top=rc0.top+rc.bottom/2;
	rc20.right=rc8.right;
	rc20.bottom=rc0.bottom+rc.bottom/2;
	rc21.top=2+rc.bottom/2;
	rc21.left=rc9.left;
	rc21.bottom=rc0.top-rc.bottom/80+rc.bottom/2;
	rc21.right=rc9.right;
	rc22.top=rc0.bottom+rc.bottom/40+rc.bottom/2;
	rc22.left=rc10.left;
	rc22.bottom=rc.bottom/2-rc.bottom/40+rc.bottom/2;
	rc22.right=rc10.right;
	*/
	/*
	rc4.left=rc1.right+rc.right/20;
	rc4.top=rc.bottom/60;
	rc4.bottom=rc0.top;
	rc4.right=rc.right/number*2*2-rc.right/100;
	*/

	/*数字処理end*/




	switch (msg) {

		/*
		case WM_COMMAND:
		switch(LOWORD(wp)){
		case IDM_START:
		XO=1;

		InvalidateRect(hWnd,&rc,0);
		break;
		case IDM_END:
		XO=0;
		break;
		case IDM_TIME1:
		DTM+=10000;
		Tm=DTM;
		InvalidateRect(hWnd,NULL,0);
		break;
		case IDM_TIME2:
		DTM+=60000;
		Tm=DTM;
		InvalidateRect(hWnd,NULL,0);
		break;
		case IDM_TIME3:
		DTM-=10000;
		if(DTM<1000){
		DTM=1000;
		}
		InvalidateRect(hWnd,NULL,0);
		Tm=DTM;
		break;
		case IDM_TIME4:
		DTM-=60000;
		if(DTM<1000){
		DTM=1000;
		}
		InvalidateRect(hWnd,NULL,0);
		Tm=DTM;
		break;
		case IDM_NOISE:
		M=1;
		break;
		case IDM_MUSIC:
		M=0;
		break;
		case IDM_EASY:
		DQ=1;
		break;
		case IDM_HARD:
		DQ=0;
		break;
		default:
		return (DefWindowProc(hWnd, msg, wp, lp));

		}
		break;

		*/
	case WM_CREATE:
		for (o = 0; o<nD; o++){
			nLOOP[o] = 0;
		}
		hPen10 = CreatePen(PS_SOLID, 0, RGB(0, 0, 0));
		hPen5 = (HPEN)GetStockObject(BLACK_PEN);
		hPen4 = (HPEN)GetStockObject(NULL_PEN);
		hBrushw = (HBRUSH)GetStockObject(WHITE_BRUSH);
		hBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
		DTM = 10000;
		Tm = DTM;
		XO = 1;
		TEST = 0;
		Rpr = 110;
		Spr = 110;
		rc0.left = 0;
		rc0.top = rc.bottom / 16;
		rc0.right = rc.right / number;
		rc0.bottom = rc.bottom / 9;
		rc1.left = rc0.right;
		rc1.top = rc0.top;
		rc1.right = rc.right / number * 2;
		rc1.bottom = rc0.bottom;
		rc2.top = 2;
		rc2.left = 2;
		rc2.bottom = rc0.top - rc.bottom / 80;
		rc2.right = rc1.right;
		rc3.top = rc0.bottom + rc.bottom / 40;
		rc3.left = rc0.left + rc.right / 160;
		rc3.bottom = rc.bottom / 2 - rc.bottom / 40;
		rc3.right = rc1.right - rc.right / 35;
		/*
		rc4.left=rc1.right+rc.right/20;
		rc4.top=rc.bottom/60;
		rc4.bottom=rc0.top;
		rc4.right=rc.right/number*2*2-rc.right/100;
		*/

		for (o = 0; o<HH; o++){
			price[0][o] = 15000;
		}
		for (o = 0; o<HH; o++){
			price[1][o] = 15000;
		}
		for (o = 0; o<HH; o++){
			price[2][o] = 15000;
		}
		for (o = 0; o<HH; o++){
			price[3][o] = 15000;
		}
		for (o = 0; o<HH; o++){
			price[4][o] = 15000;
		}
		/*
		price[20]=6000;
		*/
		SetTimer(hWnd, ID_MYTIMER, 200, NULL);

		srand((unsigned)time(NULL));
		for (o = 0; o<5; o++){
			wsprintf((LPWSTR)szPR[0 + o * 2], TEXT("売；%d"), price[0 + o][his - 1] - spread[o]);
			wsprintf((LPWSTR)szPR[1 + o * 2], TEXT("買；%d"), price[0 + o][his - 1] + spread[o]);
		}
		wsprintf((LPWSTR)szPR[10], TEXT("トヨタ自動車"));
		wsprintf((LPWSTR)szPR[11], TEXT("ユニクロ"));
		wsprintf((LPWSTR)szPR[12], TEXT("森永製菓"));
		wsprintf((LPWSTR)szPR[13], TEXT("イトーヨーカドー"));
		wsprintf((LPWSTR)szPR[14], TEXT("ＪＴＢ"));

		hFont1 = MyCreateFont(36, SHIFTJIS_CHARSET, TEXT("ＭＳ 明朝"));
		hFont2 = MyCreateFont(15, SHIFTJIS_CHARSET, TEXT("ＭＳ 明朝"));
		hFont3 = MyCreateFont(10, SHIFTJIS_CHARSET, TEXT("ＭＳ 明朝"));
		hFont4 = MyCreateFont(40, SHIFTJIS_CHARSET, TEXT("ＭＳ 明朝"));
		hFont5 = MyCreateFont(50, SHIFTJIS_CHARSET, TEXT("ＭＳ 明朝"));
		hFont6 = MyCreateFont(25, SHIFTJIS_CHARSET, TEXT("ＭＳ 明朝"));

		for (o = 0; o<his; o++){
			pt[0][o].y = (rc3.bottom - rc3.top) / 2 + rc3.top;
			pt[0][o].x = (rc3.right - rc3.left) / (his)*(o + 1) + rc3.left;

		}
		for (o = 0; o<his; o++){
			pt[1][o].y = (rc3.bottom - rc3.top) / 2 + rc3.top;
			pt[1][o].x = (rc3.right - rc3.left) / (his)*(o + 1) + rc3.left + rc.right * 2 / 3;

		}
		for (o = 0; o<his; o++){
			pt[2][o].y = (rc3.bottom - rc3.top) / 2 + rc3.top + rc.bottom / 2;
			pt[2][o].x = (rc3.right - rc3.left) / (his)*(o + 1) + rc3.left;

		}
		for (o = 0; o<his; o++){
			pt[3][o].y = (rc3.bottom - rc3.top) / 2 + rc3.top + rc.bottom / 2;
			pt[3][o].x = (rc3.right - rc3.left) / (his)*(o + 1) + rc3.left + rc.right / 3;

		}
		for (o = 0; o<his; o++){
			pt[4][o].y = (rc3.bottom - rc3.top) / 2 + rc3.top + rc.bottom / 2;
			pt[4][o].x = (rc3.right - rc3.left) / (his)*(o + 1) + rc3.left + rc.right * 2 / 3;

		}

		hBmp = (HBITMAP)LoadImage(hInst,	//インスタンスハンドル
			TEXT("ReSt.bmp"),						//イメージの名前
			IMAGE_BITMAP,					//イメージタイプ
			150,								//幅
			150,								//高さ
			LR_LOADFROMFILE);				//ロードオプション

		GetObject(hBmp, (int)sizeof(bmp_info), &bmp_info);
		bmpw = bmp_info.bmWidth;
		bmph = bmp_info.bmHeight;

		hdc_mem = CreateCompatibleDC(hdc);
		SelectObject(hdc_mem, hBmp);
		/*
		hdc = GetDC( hWnd );
		// メモリデバイスコンテキストを作成する
		hdc_mem = CreateCompatibleDC( hdc );
		// ロードしたビットマップを選択する
		SelectObject( hdc_mem, hBmp );
		*/

		break;
	case WM_CLOSE:
		id = MessageBox(hWnd,
			TEXT("終了してもよろしいですか"),
			TEXT("確認"),
			MB_YESNO | MB_ICONQUESTION);
		if (id == IDYES) {
			/*
			if (KillTimer(hWnd, ID_MYTIMER2) == 0) {
			MessageBox(hWnd,
			TEXT("KillTimer Error!"),
			TEXT("Error"),
			MB_OK | MB_ICONEXCLAMATION);
			}
			*/
			DestroyWindow(hWnd);
		}
		break;
	case WM_DESTROY:
		KillTimer(hWnd, ID_MYTIMER);
		KillTimer(hWnd, ID_MYTIMER2);
		DeleteObject(hFont1);
		DeleteObject(hFont2);
		DeleteObject(hFont3);
		DeleteObject(hFont4);
		DeleteObject(hFont5);
		DeleteObject(hPen4);
		DeleteObject(hBrushw);
		DeleteObject(hBmp);
		DeleteDC(hdc_mem);
		PostQuitMessage(0);
		break;
	case WM_PAINT:
		YXO = 0;
		hdc = BeginPaint(hWnd, &ps);


		SelectObject(hdc, hPen4);
		SelectObject(hdc, hBrushw);
		Rectangle(hdc, rc.left, rc.top, rc.right, rc.bottom);

		if (!(EV<his + 3)){
			if (XO == 1){
				Ftime(1, hWnd, rc, hdc);
				Ftime(2, hWnd, rc, hdc);
				if (DQ != 1){
					Ftime(3, hWnd, rc, hdc);
					Ftime(5, hWnd, rc, hdc);
				}
				Ftime(4, hWnd, rc, hdc);
				Ftime(6, hWnd, rc, hdc);
			}



			if (XO == 0){
				bmph = rc.bottom / 9;
				bmpw = bmph;
				SelectObject(hdc, hBrushw);
				Rectangle(hdc, rc.left, rc.top, rc.right, rc.bottom);
				SelectObject(hdc, hFont5);
				wsprintf((LPWSTR)CON, TEXT("\n今からの取引はできません。\n少々お待ちください。"));
				DrawText(hdc, CON, -1, &rc, DT_CENTER);
				wsprintf((LPWSTR)CON, TEXT("\n\n\n\n\n次回の制限時間；%d秒"), DTM / 1000);
				DrawText(hdc, CON, -1, &rc, DT_CENTER);



				SelectObject(hdc_mem, hBmp);


				Rectangle(hdc, rc.right / 2 - bmpw / 2, rc.bottom * 3 / 4 - bmph, rc.right / 2 + bmpw / 2, rc.bottom * 3 / 4);

				StretchBlt(hdc,		//転送先
					rc.right / 2 - bmpw / 2,	//転送先x座標
					rc.bottom * 3 / 4 - bmph,	//転送先y座標
					bmpw,		//転送先長方形の幅 
					bmph,		//転送先長方形の高さ 
					hdc_mem,	//転送元
					0,			//転送元x座標
					0,
					150,
					150,//転送元y座標
					SRCCOPY);	//ラスターオペレーションコード

				YXO = 1;
			}


		}
		SelectObject(hdc, hBrush);




		if (EV<his + 3){


			SelectObject(hdc, hBrushw);

			Rectangle(hdc, rc.left, rc.top, rc.right, rc.bottom);
			SelectObject(hdc, hPen5);
			SelectObject(hdc, hFont5);
			wsprintf((LPWSTR)YOM, TEXT("\n読み込み中です。\n少々お待ちください。"));
			DrawText(hdc, YOM, -1, &rc, DT_CENTER);
			wsprintf((LPWSTR)YOM, TEXT("\n\n\n\n\n%d％完了"), Qw * 100 / (his + 1 + DTM / 1000));
			DrawText(hdc, YOM, -1, &rc, DT_CENTER);
		}




		EndPaint(hWnd, &ps);

		break;




	case WM_TIMER:
		if (EV == his + 3){
			KillTimer(hWnd, ID_MYTIMER);
			SetTimer(hWnd, ID_MYTIMER2, 1000, NULL);
		}
		Qw++;
		/*
		if(TEST==1&&SP==1){
		XO=1;
		TEST=0;
		}
		*/
		if (XO == 1){

			rc0.left = 0;
			rc0.top = rc.bottom / 16;
			rc0.right = rc.right / number;
			rc0.bottom = rc.bottom / 9;
			rc1.left = rc0.right;
			rc1.top = rc0.top;
			rc1.right = rc.right / number * 2;
			rc1.bottom = rc0.bottom;
			rc2.top = 2;
			rc2.left = 2;
			rc2.bottom = rc0.top - rc.bottom / 80;
			rc2.right = rc1.right;
			rc3.top = rc0.bottom + rc.bottom / 40;
			rc3.left = rc0.left + rc.right / 80;
			rc3.bottom = rc.bottom / 2 - rc.bottom / 40;
			rc3.right = rc1.right - rc.right / 80;

			rc4.left = rc1.right + rc.right / 20;
			rc4.top = rc.bottom / 60;
			rc4.bottom = rc0.top;
			rc4.right = rc.right / number * 2 * 2 - rc.right / 100;

			if (EV<his + 2){
				Tm -= DTM - 1000;

			}
			/*
			if(EV==his+2){
			price[0][his]=15000;
			price[1][his]=15000;
			price[2][his]=15000;
			price[3][his]=15000;
			price[4][his]=15000;



			for(o=0;o<nD;o++){
			nLOOP[o]=0;
			}
			}

			*/

			Tm -= 1000;

			RECT rc5;
			rc5.left = rc.right / 3;
			rc5.right = rc4.right;
			rc5.top = rc4.top;
			rc5.bottom = rc4.bottom;


			InvalidateRect(hWnd, &rc5, 0);

			if (EV<his + 3){
				InvalidateRect(hWnd, NULL, 0);
			}


			if (M == 0){
				/*
				if(Tm==152000){
				PlaySound(
				TEXT("M1.wav"),NULL,SND_ASYNC|SND_FILENAME);
				}

				if(Tm==81000){
				PlaySound(
				TEXT("M2.wav"),NULL,SND_ASYNC|SND_FILENAME);
				}
				*/
				/*if(Tm==111000){
				PlaySound(
				TEXT("M1.wav"),NULL,SND_ASYNC|SND_FILENAME);
				}

				if(Tm==40000){
				PlaySound(
				TEXT("M2.wav"),NULL,SND_ASYNC|SND_FILENAME);
				}*/





			}










			if (Tm == 0){


				if (!(EV<his + 2)){
					XO = 0;
					TEST = 1;
				}
				/*株価・チャート移動のためのスクリプトstart*/
				/*
				Max=0;
				min=0;
				*/

				EV++;

				if (EV == 2){
					price[0][his] = 15000;
					price[1][his] = 15000;
					price[2][his] = 15000;
					price[3][his] = 15000;
					price[4][his] = 15000;
				}

				o = 0;

				/**goto文を使っています。注意！**/

				if (DQ != 1){

				loop:
					XX = rand() % nD;

					if (XX == 108){
						goto loop;
					}
					if (XX == 110){
						goto loop;
					}
					if (XX == 111){
						goto loop;
					}
					if (XX == 112){
						goto loop;
					}
					if (XX >= 123 && XX <= 130){
						goto loop;
					}


					if (nLOOP[XX] == 1){
						o++;
						if (o == 1000000){
							Data(1000);
							goto out;
						}
						goto loop;
					}
					if (!(EV<his + 3)){
						nLOOP[XX] = 1;
					}

					if (!(EV<his + 3)){
						if (NE == 36){

							Data(34);
							nLOOP[34] = 1;
							NE = 0;
						}
						else{
							Data(XX);
						}
					}
				out:
					o = 1;
				}

				if (DQ == 1){

				Eloop:
					XX = rand() % EnD;
					if (nLOOP[XX] == 1){
						o++;
						if (o == 100000000){
							EData(1000);
							goto Eout;
						}
						goto Eloop;
					}
					if (!(EV<his + 3)){
						nLOOP[XX] = 1;
					}
					EData(XX);
				Eout:
					o = 1;
				}












				/**goto文を使っています。注意！**/


				/////////////////////////////////////////////////////////////
				/*	XX=rand()%nD;*/


				for (q = 0; q<5; q++){

					for (o = 0; o<his; o++){
						price[q][o] = price[q][o + 1];
					}



					wsprintf((LPWSTR)szPR[0], TEXT("株価；%d"), price[q][his - 1] - spread[q]);
					/*
					wsprintf((LPWSTR)szPR[1],TEXT("買；%d"),price[q][his-1]+spread[q]);
					*/




					for (o = 0; o<his; o++){
						if (o == 0){
							Max = price[q][o];
						}
						else if (price[q][o]>Max){
							Max = price[q][o];
						}
					}

					for (o = 0; o<his; o++){
						if (o == 0){
							min = price[q][o];
						}
						else if (price[q][o]<min){
							min = price[q][o];
						}
					}

					if (Max != min){
						if (q<2){
							for (o = 0; o<his; o++){
								i = (price[q][o] - min) * 100 / (Max - min)*((rc.bottom / 2 - rc.bottom / 30) - (rc0.bottom + rc.bottom / 30));
								pt[q][o].y = rc.bottom / 2 - rc.bottom / 30 - i / 100;
							}
						}
						else{
							for (o = 0; o<his; o++){
								i = (price[q][o] - min) * 100 / (Max - min)*((rc.bottom / 2 - rc.bottom / 30) - (rc0.bottom + rc.bottom / 30));
								pt[q][o].y = rc.bottom / 2 - rc.bottom / 30 - i / 100 + rc.bottom / 2;
							}
						}


					}
					else{
						if (q<2){
							for (o = 0; o<his; o++){
								pt[q][o].y = (rc3.bottom - rc3.top) / 2 + rc3.top;
							}
						}
						else{
							for (o = 0; o<his; o++){
								pt[q][o].y = (rc3.bottom - rc3.top) / 2 + rc3.top + rc.bottom / 2;
							}
						}
					}


					wsprintf(szPR[q * 2 + 15], TEXT("%d"), Max);
					wsprintf(szPR[q * 2 + 16], TEXT("%d"), min);

				}







				Tm = DTM;



				if (M == 0){

					/*PlaySound(
					TEXT("music2.wav"),NULL,SND_ASYNC|SND_FILENAME);
					*/
				}
				/*為替start*/

				Spr = Rpr;

				int Z;
				int ZZ, SZ;
				Z = rand() % 12;
				if (DQ != 1){
					if (Z == 0){
						if (Rpr<130){
							Rpr += 0;
							A += 0;
							B -= 0;

							D -= 0;
							E -= 0;
						}
						if (Rpr == 130){
							Rpr += 0;
							A += 0;
							B -= 0;

							D -= 0;
							E -= 0;
						}
					}
					else if (Z <= 2){
						if (Rpr<135){
							Rpr += 5;
							A += 0;
							B -= 0;

							D -= 0;
							E -= 0;
						}
					}
					else if (Z <= 8){
					}
					else if (Z <= 10){
						if (Rpr>85){
							Rpr -= 5;
							A -= 0;
							B += 0;

							D += 0;
							E += 0;
						}
					}
					else if (Z == 11){
						if (Rpr>90){
							Rpr -= 10;
							A -= 0;
							B += 0;
							D += 0;
							E += 0;
						}
						if (Rpr == 90){
							Rpr -= 5;
							A -= 0;
							B += 0;

							D += 0;
							E += 0;
						}
					}
				}


				ZZ = rand() % 5;
				SZ = Z + ZZ - 2;

				if (SZ <= 0){
					wsprintf((LPWSTR)Yrate, TEXT(""));
					rR = 255;
					rG = 0;
					rB = 51;
					if (Rpr>135){
						wsprintf((LPWSTR)Yrate, TEXT(""));
						rR = 51;
						rG = 255;
						rB = 0;
					}
				}
				else if (SZ <= 2){
					wsprintf((LPWSTR)Yrate, TEXT(""));
					rR = 255;
					rG = 204;
					rB = 0;
				}
				else if (SZ <= 7){
					wsprintf((LPWSTR)Yrate, TEXT(""));
					rR = 51;
					rG = 255;
					rB = 0;
				}
				else if (SZ <= 10){
					wsprintf((LPWSTR)Yrate, TEXT(""));
					rR = 0;
					rG = 102;
					rB = 153;
				}
				else if (SZ >= 11){
					wsprintf((LPWSTR)Yrate, TEXT(""));
					rR = 0;
					rG = 0;
					rB = 51;
					if (Rpr<85){
						wsprintf((LPWSTR)Yrate, TEXT(""));
						rR = 51;
						rG = 255;
						rB = 0;
					}
				}

				if (Rpr>Rmax){
					Rpr = 135;
				}
				if (Rpr<Rmin){
					Rpr = 85;
				}

				if (NE == 36){
					Rpr = 150;
				}

				if (nNE == 1){
					Rpr = 100;
				}

				if (nNE == 1 || NE == 32){
					wsprintf((LPWSTR)Yrate, TEXT(""));
					rR = 0;
					rG = 0;
					rB = 51;
					if (nNE == 11){
						nNE = 0;
					}
					if (NE == 32){
						mNE = 1;
						o = 50;
					}
				}

				/*if(NE==36||(mNE==1&&o!=50)){
				Rpr=150;
				wsprintf((LPWSTR)Yrate,TEXT("↑"));
				rR=255;
				rG=0;
				rB=51;
				if(nNE==11){
				nNE=1;
				}
				if(NE==32){
				mNE=0;
				}
				}*/

				o = 0;


				wsprintf((LPWSTR)Rate, TEXT(""), Spr);

				/*Unknown*/
				price[0][his] = price[0][his - 1] + A;
				price[1][his] = price[1][his - 1] + B;
				price[2][his] = price[2][his - 1] + C;
				price[3][his] = price[3][his - 1] + D;
				price[4][his] = price[4][his - 1] + E;
				/*
				for(o=0;o<5;o++){
				if(price[o][his]<1000){
				price[o][his]=1000;
				}
				}
				*/

				/*Unknown*/

				/*為替+ニュースまとめstart*/



				/*
				q=0;
				q=A%1000;
				if(q<500){
				A+=1000-q;
				}else{
				A-=q;
				}

				q=0;
				q=B%1000;
				if(q<500){
				B+=1000-q;
				}else{
				B-=q;
				}



				q=0;
				q=C%1000;
				if(q<500){
				C+=1000-q;
				}else{
				C-=q;
				}



				q=0;
				q=D%1000;
				if(q<500){
				D+=1000-q;
				}else{
				D-=q;
				}



				q=0;
				q=E%1000;
				if(q<500){
				E+=1000-q;
				}else{
				E-=q;
				}


				*/
				/*

				for(o=0;o<5;o++){
				price[o][HH]+=rand()%961-480;
				}
				*/

				for (o = 0; o<5; o++){
					q = price[o][HH] % 1000;
					if (q >= 500){
						price[o][HH] += 1000;
					}
					price[o][HH] -= q;
				}


				if (EV<his + 3){
					for (o = 0; o<5; o++){
						if (price[o][HH]<12000){
							price[o][HH] = 12000;
						}
					}

					for (o = 0; o<5; o++){
						if (price[o][HH]>18000){
							price[o][HH] = 18000;
						}
					}

				}

				if (EV>HH - 2){
					for (o = 0; o<5; o++){
						if (price[o][his - 1]<100){
							AS[o] = 2;
						}

						if (AS[o]>0){ AS[o]--; }

					}
				}
				int r;
				int w;
				for (w = 0; w<5; w++){
					if (AS[w]>0){
						for (o = 0; o<HH + 1; o++){
							r = rand() % 4001 - 2000;
							price[w][o] = 15000 + r;



							q = price[w][o] % 1000;
							if (q >= 500){
								price[w][o] += 1000;
							}
							price[w][o] -= q;
						}
					}
				}

				/*為替+ニュースまとめend*/


				/*為替end*/



				InvalidateRect(hWnd, NULL, 0);


			}/**Tm==0**/
			/*株価・チャート移動のためのスクリプトend*/



		}
		else
		if (TEST == 200){
			SendMessage(hWnd, WM_PAINT, 0, 0);
		}
		break;
		/*
		case WM_KEYDOWN:

		switch(wp)
		{
		case VK_CONTROL:
		CT=1;
		break;
		case VK_RETURN:
		if(CT==1){
		SP=1;
		}
		break;

		default:
		return (DefWindowProc(hWnd, msg, wp, lp));

		}

		case WM_KEYUP:

		switch(wp)
		{
		case VK_CONTROL:
		CT=0;
		break;
		case VK_RETURN:
		SP=0;

		break;

		default:
		return (DefWindowProc(hWnd, msg, wp, lp));

		}
		*/




	case WM_KEYDOWN:
		if (wp == VK_SPACE){
			Spa = 1;
		}
		break;

	case WM_KEYUP:
		if (wp == VK_SPACE){
			Spa = 0;
		}

		break;


	case WM_CHAR:
		if (wp == VK_ESCAPE){
			if (XO == 0){ XO = 1; InvalidateRect(hWnd, &rc, 0); break; }
			if (XO == 1){ XO = 0; }
		}
		if (wp == VK_RETURN){
			DTM += 10000;
			Tm = DTM;
			InvalidateRect(hWnd, NULL, 0);

		}
		if (wp == VK_SPACE){
			DTM -= 10000;
			if (DTM<1000){
				DTM = 1000;
			}
			InvalidateRect(hWnd, NULL, 0);
			Tm = DTM;


		}

		break;	// マウスカーソルが移動したときに送られてくる
		// 移動先の座標を取得
		/*if(Spa==1){
		int Mx,My;
		Mx = LOWORD( lp );
		My = HIWORD( lp );
		if(Mx>rc.right/2-bmpw/2&&Mx<rc.right/2-bmpw/2+bmpw&&My>rc.bottom*3/4-bmph&&My<rc.bottom*3/4){
		//if(YXO==1){
		XO=1;
		InvalidateRect(hWnd,NULL,0);
		//}
		}
		}
		break;*/
		/*
		rc.right/2-bmpw/2,	//転送先x座標
		rc.bottom*3/4-bmph,	//転送先y座標
		bmpw,		//転送先長方形の幅

		bmph,
		*/




	default:
		return (DefWindowProc(hWnd, msg, wp, lp));
	}
	return 0;
}


HFONT MyCreateFont(int nHeight, DWORD dwCharSet, LPCTSTR lpName)
{
	return(CreateFont(nHeight, 0, 0, 0,
		FW_DONTCARE,
		FALSE, FALSE, FALSE,
		dwCharSet,
		OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY,
		DEFAULT_PITCH | FF_DONTCARE,
		lpName));
}

void Data(int XX){
	/*if(F==1){
	A=15000;
	B=15000;
	C=15000;
	D=15000;
	E=15000;

	F=0;
	}else*/ if (EV<his + 3){
		A = (rand() % 7 - 3) * 700;
		B = (rand() % 7 - 3) * 700;
		C = (rand() % 7 - 3) * 700;
		D = (rand() % 7 - 3) * 700;
		E = (rand() % 7 - 3) * 700;


	}
	else{
		if (XX == 0) {
			wsprintf(S, TEXT("本州と九州を結ぶ新しい橋が完成"), XX);
			wsprintf(Sb, TEXT("\n\n\n\n\n\nイベント番号；%d"), XX);
			A = 000;
			B = 000;
			C = 1000;
			D = 1000;
			E = 3000;
		}
		if (XX == 1) {
			wsprintf(S, TEXT("南海トラフ地震が発生"), XX);
			wsprintf(Sb, TEXT("\n\n\n\n\n\nイベント番号；%d"), XX);
			A = -0;
			B = -3000;
			C = -4000;
			D = -3000;
			E = -5000;
		}
		if (XX == 2) {
			wsprintf(S, TEXT("タイで大洪水が発生"), XX);
			wsprintf(Sb, TEXT("\n\n\n\n\n\nイベント番号；%d"), XX);
			A = -4000;
			B = -1000;
			C = 000;
			D = 000;
			E = -2000;
		}
		if (XX == 3) {
			wsprintf(S, TEXT("日本全国で米が不作"), XX);
			wsprintf(Sb, TEXT("\n\n\n\n\n\nイベント番号；%d"), XX);
			A = 000;
			B = 000;
			C = 000;
			D = -2000;
			E = -1000;
		}
		if (XX == 4) {
			wsprintf(S, TEXT("愛知県に巨大台風が上陸"), XX);
			wsprintf(Sb, TEXT("\n\n\n\n\n\nイベント番号；%d"), XX);
			A = -3000;
			B = -1000;
			C = -1000;
			D = -1000;
			E = -1000;
		}
		if (XX == 5) {
			wsprintf(S, TEXT("ハワイで大規模な火山噴火"), XX);
			wsprintf(Sb, TEXT("\n\n\n\n\n\nイベント番号；%d"), XX);
			A = 000;
			B = 000;
			C = 000;
			D = 000;
			E = -5000;
		}
		if (XX == 6) {
			wsprintf(S, TEXT("九州地方で地震が発生"), XX);
			wsprintf(Sb, TEXT("\n\n\n\n\n\nイベント番号；%d"), XX);
			A = -2000;
			B = -1000;
			C = -1000;
			D = -1000;
			E = -2000;
		}
		if (XX == 7) {
			wsprintf(S, TEXT("京都が集中豪雨で大きな被害"), XX);
			wsprintf(Sb, TEXT("\n\n\n\n\n\nイベント番号；%d"), XX);
			A = -2000;
			B = 000;
			C = 000;
			D = 000;
			E = -2000;
		}
		if (XX == 8) {
			wsprintf(S, TEXT("鎌倉が世界遺産に"), XX);
			wsprintf(Sb, TEXT("\n\n\n\n\n\nイベント番号；%d"), XX);
			A = 000;
			B = 000;
			C = 000;
			D = 000;
			E = 3000;
		}
		if (XX == 9) {
			wsprintf(S, TEXT("富士山が世界遺産に"), XX);
			wsprintf(Sb, TEXT("\n\n\n\n\n\nイベント番号；%d"), XX);
			A = 000;
			B = 000;
			C = 000;
			D = 000;
			E = 3000;
		}
		if (XX == 10) {
			wsprintf(S, TEXT("日本を訪れる外国人が3000万人を超える"), XX);
			wsprintf(Sb, TEXT("\n\n\n\n\n\nイベント番号；%d"), XX);
			A = 000;
			B = 1000;
			C = 1000;
			D = 1000;
			E = 6000;
		}
		if (XX == 11) {
			wsprintf(S, TEXT("伊勢志摩サミットが開催"), XX);
			wsprintf(Sb, TEXT("\n\n\n\n\n\nイベント番号；%d"), XX);
			A = 1000;
			B = 000;
			C = 000;
			D = 000;
			E = 2000;
		}
		if (XX == 12) {
			wsprintf(S, TEXT("フランスでテロ"), XX);
			wsprintf(Sb, TEXT("\n\n\n\n\n\nイベント番号；%d"), XX);
			A = -2000;
			B = 000;
			C = 000;
			D = -1000;
			E = -4000;
		}
		if (XX == 13) {
			wsprintf(S, TEXT("電気料金が値上げ"), XX);
			wsprintf(Sb, TEXT("\n\n\n\n\n\nイベント番号；%d"), XX);
			A = -2000;
			B = -1000;
			C = -1000;
			D = -2000;
			E = -1000;
		}
		if (XX == 14) {
			wsprintf(S, TEXT("石油の値段が上がる"), XX);
			wsprintf(Sb, TEXT("\n\n\n\n\n\nイベント番号；%d"), XX);
			A = -1000;
			B = -1000;
			C = -1000;
			D = -1000;
			E = -3000;
		}
		if (XX == 15) {
			wsprintf(S, TEXT("石油の値段が下がる"), XX);
			wsprintf(Sb, TEXT("\n\n\n\n\n\nイベント番号；%d"), XX);
			A = 1000;
			B = 1000;
			C = 1000;
			D = 1000;
			E = 3000;
		}
		if (XX == 16) {
			wsprintf(S, TEXT("鉄の値段が上がる"), XX);
			wsprintf(Sb, TEXT("\n\n\n\n\n\nイベント番号；%d"), XX);
			A = -2000;
			B = 000;
			C = 000;
			D = 000;
			E = 000;
		}
		if (XX == 17) {
			wsprintf(S, TEXT("鉄の値段が下がる"), XX);
			wsprintf(Sb, TEXT("\n\n\n\n\n\nイベント番号；%d"), XX);
			A = 2000;
			B = 000;
			C = 000;
			D = 000;
			E = 000;
		}
		if (XX == 18) {
			wsprintf(S, TEXT("今年の冬は暖冬になると予想"), XX);
			wsprintf(Sb, TEXT("\n\n\n\n\n\nイベント番号；%d"), XX);
			A = 000;
			B = -2000;
			C = 000;
			D = 1000;
			E = -1000;
		}
		if (XX == 19) {
			wsprintf(S, TEXT("今年の夏は過去最高の猛暑になると予想"), XX);
			wsprintf(Sb, TEXT("\n\n\n\n\n\nイベント番号；%d"), XX);
			A = 000;
			B = 1000;
			C = 2000;
			D = 2000;
			E = 1000;
		}
		if (XX == 20) {
			wsprintf(S, TEXT("北陸新幹線が開通"), XX);
			wsprintf(Sb, TEXT("\n\n\n\n\n\nイベント番号；%d"), XX);
			A = 000;
			B = 000;
			C = 000;
			D = 000;
			E = 3000;
		}
		if (XX == 21) {
			wsprintf(S, TEXT("海外で日本食ブーム"), XX);
			wsprintf(Sb, TEXT("\n\n\n\n\n\nイベント番号；%d"), XX);
			A = 000;
			B = 000;
			C = 1000;
			D = 1000;
			E = 2000;
		}
		if (XX == 22) {
			wsprintf(S, TEXT("日本を舞台にしたディズニー映画が大ヒット"), XX);
			wsprintf(Sb, TEXT("\n\n\n\n\n\nイベント番号；%d"), XX);
			A = 1000;
			B = 1000;
			C = 1000;
			D = 2000;
			E = 2000;
		}
		if (XX == 23) {
			wsprintf(S, TEXT("ハワイでテロ"), XX);
			wsprintf(Sb, TEXT("\n\n\n\n\n\nイベント番号；%d"), XX);
			A = -1000;
			B = -1000;
			C = -1000;
			D = -1000;
			E = -5000;
		}
		if (XX == 24) {
			wsprintf(S, TEXT("ドローンの新技術が開発"), XX);
			wsprintf(Sb, TEXT("\n\n\n\n\n\nイベント番号；%d"), XX);
			A = 1000;
			B = 1000;
			C = 000;
			D = 1000;
			E = 000;
		}
		if (XX == 25) {
			wsprintf(S, TEXT("錦織圭がオリンピックで金メダル"), XX);
			wsprintf(Sb, TEXT("\n\n\n\n\n\nイベント番号；%d"), XX);
			A = 000;
			B = 5000;
			C = 000;
			D = 000;
			E = 2000;
		}
		if (XX == 26) {
			wsprintf(S, TEXT("中国の軍艦と日本の漁船が衝突"), XX);
			wsprintf(Sb, TEXT("\n\n\n\n\n\nイベント番号；%d"), XX);
			A = -3000;
			B = -3000;
			C = 000;
			D = -1000;
			E = -3000;
		}
		if (XX == 27) {
			wsprintf(S, TEXT("日本海で大量の石油が発見"), XX);
			wsprintf(Sb, TEXT("\n\n\n\n\n\nイベント番号；%d"), XX);
			A = 2000;
			B = 2000;
			C = 1000;
			D = 1000;
			E = 2000;
		}
		if (XX == 28) {
			wsprintf(S, TEXT("富士山噴火の危険が高まる"), XX);
			wsprintf(Sb, TEXT("\n\n\n\n\n\nイベント番号；%d"), XX);
			A = 000;
			B = 000;
			C = 000;
			D = 000;
			E = -3000;
		}
		if (XX == 29) {
			wsprintf(S, TEXT("中国で日本製品が大ブーム"), XX);
			wsprintf(Sb, TEXT("\n\n\n\n\n\nイベント番号；%d"), XX);
			A = 5000;
			B = 4000;
			C = 3000;
			D = 2000;
			E = 3000;
		}
		if (XX == 30) {
			wsprintf(S, TEXT("中国人の爆買いが流行"), XX);
			wsprintf(Sb, TEXT("\n\n\n\n\n\nイベント番号；%d"), XX);
			A = 000;
			B = 1000;
			B = -3000;
			C = 1000;
			D = 3000;
			E = 5000;
		}
		if (XX == 31) {
			wsprintf(S, TEXT("首相が靖国神社参拝"), XX);
			wsprintf(Sb, TEXT("\n\n\n\n\n\nイベント番号；%d"), XX);
			A = -4000;
			C = -1000;
			D = -1000;
			E = -3000;
		}
		if (XX == 32) {
			wsprintf(S, TEXT("「石油が20年後に無くなる」と国連が発表"), XX);
			wsprintf(Sb, TEXT("\n\n\n\n\n\nイベント番号；%d"), XX);
			A = 1000;
			B = -2000;
			C = -2000;
			D = -3000;
			E = -5000;
		}
		if (XX == 33) {
			wsprintf(S, TEXT("中国が歴史問題を理由に日中間の往来を制限"), XX);
			wsprintf(Sb, TEXT("\n\n\n\n\n\nイベント番号；%d"), XX);
			A = -4000;
			B = -4000;
			C = -1000;
			D = -2000;
			E = -6000;
		}
		if (XX == 34) {
			wsprintf(S, TEXT("日本で中華料理が大ブーム"), XX);
			wsprintf(Sb, TEXT("\n\n\n\n\n\nイベント番号；%d"), XX);
			A = 1000;
			B = 1000;
			C = 000;
			D = 2000;
			E = 3000;
		}
		if (XX == 35) {
			wsprintf(S, TEXT("イタリアで大地震が発生"), XX);
			wsprintf(Sb, TEXT("\n\n\n\n\n\nイベント番号；%d"), XX);
			A = 000;
			B = 000;
			C = 000;
			D = 000;
			E = -3000;
		}
		if (XX == 36) {
			wsprintf(S, TEXT("消費税が15％に増税"), XX);
			wsprintf(Sb, TEXT("\n\n\n\n\n\nイベント番号；%d"), XX);
			A = -3000;
			B = -2000;
			C = -2000;
			D = -2000;
			E = -3000;
		}
		if (XX == 37) {
			wsprintf(S, TEXT("韓国で新型インフルエンザが大流行"), XX);
			wsprintf(Sb, TEXT("\n\n\n\n\n\nイベント番号；%d"), XX);
			A = -1000;
			B = -1000;
			C = 000;
			D = 1000;
			E = 2000;
		}
		if (XX == 38) {
			wsprintf(S, TEXT("本州で大規模な停電"), XX);
			wsprintf(Sb, TEXT("\n\n\n\n\n\nイベント番号；%d"), XX);
			A = -3000;
			B = -2000;
			C = -3000;
			D = -3000;
			E = -3000;
		}
		if (XX == 39) {
			wsprintf(S, TEXT("サッカーワールドカップが大阪で開催"), XX);
			wsprintf(Sb, TEXT("\n\n\n\n\n\nイベント番号；%d"), XX);
			A = 000;
			B = 1000;
			C = 000;
			D = 1000;
			E = 4000;
		}
		if (XX == 40) {
			wsprintf(S, TEXT("広島で土砂災害"), XX);
			wsprintf(Sb, TEXT("\n\n\n\n\n\nイベント番号；%d"), XX);
			A = -1000;
			B = -1000;
			C = -1000;
			D = -1000;
			E = -1000;
		}
		if (XX == 41) {
			wsprintf(S, TEXT("エボラ出血熱がヨーロッパで大流行"), XX);
			wsprintf(Sb, TEXT("\n\n\n\n\n\nイベント番号；%d"), XX);
			A = -5000;
			B = -2000;
			C = -2000;
			D = -2000;
			E = -7000;
		}
		if (XX == 42) {
			wsprintf(S, TEXT("高速道路料金が無料に"), XX);
			wsprintf(Sb, TEXT("\n\n\n\n\n\nイベント番号；%d"), XX);
			A = 2000;
			B = 1000;
			C = 1000;
			D = 1000;
			E = 2000;
		}
		if (XX == 43) {
			wsprintf(S, TEXT("新幹線の事故が相次ぐ"), XX);
			wsprintf(Sb, TEXT("\n\n\n\n\n\nイベント番号；%d"), XX);
			A = 1000;
			B = 000;
			C = 000;
			D = 000;
			E = -2000;
		}
		if (XX == 44) {
			wsprintf(S, TEXT("北朝鮮のミサイルが神奈川県沖に落下"), XX);
			wsprintf(Sb, TEXT("\n\n\n\n\n\nイベント番号；%d"), XX);
			A = -3000;
			B = -3000;
			C = -3000;
			D = -3000;
			E = -3000;
		}
		if (XX == 45) {
			wsprintf(S, TEXT("上野動物園でパンダの赤ちゃんが生まれる"), XX);
			wsprintf(Sb, TEXT("\n\n\n\n\n\nイベント番号；%d"), XX);
			A = 1000;
			B = 1000;
			C = 000;
			D = 000;
			E = 2000;
		}
		if (XX == 46) {
			wsprintf(S, TEXT("富岡製糸場が世界遺産に"), XX);
			wsprintf(Sb, TEXT("\n\n\n\n\n\nイベント番号；%d"), XX);
			A = 000;
			B = 1000;
			C = 000;
			D = 000;
			E = 3000;
		}
		if (XX == 47) {
			wsprintf(S, TEXT("リニアモーターカーが開通"), XX);
			wsprintf(Sb, TEXT("\n\n\n\n\n\nイベント番号；%d"), XX);
			A = 000;
			B = 000;
			C = 000;
			D = 1000;
			E = 4000;
		}
		if (XX == 48) {
			wsprintf(S, TEXT("成田国際空港の国際線が2倍に増える"), XX);
			wsprintf(Sb, TEXT("\n\n\n\n\n\nイベント番号；%d"), XX);
			A = 000;
			B = 000;
			C = 000;
			D = 000;
			E = 5000;
		}
		if (XX == 49) {
			wsprintf(S, TEXT("中東で大規模な反政府デモ"), XX);
			wsprintf(Sb, TEXT("\n\n\n\n\n\nイベント番号；%d"), XX);
			A = -2000;
			B = -2000;
			C = -1000;
			D = -1000;
			E = -2000;
		}
		if (XX == 50) {
			wsprintf(S, TEXT("インドの道路が整備される"), XX);
			wsprintf(Sb, TEXT("\n\n\n\n\n\nイベント番号；%d"), XX);
			A = 2000;
			B = 2000;
			C = 000;
			D = 000;
			E = 2000;
		}
		if (XX == 51) {
			wsprintf(S, TEXT("アメリカが「30年でCO2排出量を半分にする」と発表"), XX);
			wsprintf(Sb, TEXT("\n\n\n\n\n\nイベント番号；%d"), XX);
			A = 7000;
			B = 000;
			C = 000;
			D = 000;
			E = 000;
		}
		if (XX == 52) {
			wsprintf(S, TEXT("野菜の価格が昨年の2倍に"), XX);
			wsprintf(Sb, TEXT("\n\n\n\n\n\nイベント番号；%d"), XX);
			A = 000;
			B = 000;
			C = -1000;
			D = -2000;
			E = -1000;
		}
		if (XX == 53) {
			wsprintf(S, TEXT("デング熱の大流行により東南アジアへの渡航が制限"), XX);
			wsprintf(Sb, TEXT("\n\n\n\n\n\nイベント番号；%d"), XX);
			A = -3000;
			B = -3000;
			C = -1000;
			D = -1000;
			E = -3000;
		}
		if (XX == 54) {
			wsprintf(S, TEXT("ソマリアの海賊が消滅"), XX);
			wsprintf(Sb, TEXT("\n\n\n\n\n\nイベント番号；%d"), XX);
			A = 2000;
			B = 2000;
			C = 1000;
			D = 1000;
			E = 1000;
		}
		if (XX == 55) {
			wsprintf(S, TEXT("日本の人口が増加"), XX);
			wsprintf(Sb, TEXT("\n\n\n\n\n\nイベント番号；%d"), XX);
			A = 5000;
			B = 4000;
			C = 4000;
			D = 3000;
			E = 4000;
		}
		if (XX == 56) {
			wsprintf(S, TEXT("札幌オリンピック開催が決定"), XX);
			wsprintf(Sb, TEXT("\n\n\n\n\n\nイベント番号；%d"), XX);
			A = 4000;
			B = 3000;
			C = 2000;
			D = 2000;
			E = 4000;
		}
		if (XX == 57) {
			wsprintf(S, TEXT("東京スカイツリーが事故により営業を停止"), XX);
			wsprintf(Sb, TEXT("\n\n\n\n\n\nイベント番号；%d"), XX);
			A = 000;
			B = 000;
			C = 000;
			D = -1000;
			E = -2000;
		}
		if (XX == 58) {
			wsprintf(S, TEXT("ガソリン税が廃止"), XX);
			wsprintf(Sb, TEXT("\n\n\n\n\n\nイベント番号；%d"), XX);
			A = 4000;
			B = 1000;
			C = 1000;
			D = 1000;
			E = 4000;
		}
		if (XX == 59) {
			wsprintf(S, TEXT("ディズニーランドにリニューアルオープン"), XX);
			wsprintf(Sb, TEXT("\n\n\n\n\n\nイベント番号；%d"), XX);
			A = 000;
			B = 1000;
			C = 0000;
			D = 1000;
			E = 3000;
		}
		if (XX == 60) {
			wsprintf(S, TEXT("石油の新しい採掘方法が開発"), XX);
			wsprintf(Sb, TEXT("\n\n\n\n\n\nイベント番号；%d"), XX);
			A = 3000;
			B = 1000;
			C = 1000;
			D = 1000;
			E = 3000;
		}
		if (XX == 61) {
			wsprintf(S, TEXT("燃費不正問題でヨーロッパの大手自動車メーカーが倒産"), XX);
			wsprintf(Sb, TEXT("\n\n\n\n\n\nイベント番号；%d"), XX);
			A = 5000;
			B = 000;
			C = 000;
			D = 000;
			E = 000;
		}
		if (XX == 62) {
			wsprintf(S, TEXT("アメリカの大統領が広島訪問"), XX);
			wsprintf(Sb, TEXT("\n\n\n\n\n\nイベント番号；%d"), XX);
			A = 000;
			B = 000;
			C = 000;
			D = 000;
			E = 2000;
		}
		if (XX == 63) {
			wsprintf(S, TEXT("コーヒー豆の値段が下がる"), XX);
			wsprintf(Sb, TEXT("\n\n\n\n\n\nイベント番号；%d"), XX);
			A = 000;
			B = 000;
			C = 2000;
			D = 2000;
			E = 000;
		}
		if (XX == 64) {
			wsprintf(S, TEXT("国連が「排気ガスを40年間で50％削減する」と発表"), XX);
			wsprintf(Sb, TEXT("\n\n\n\n\n\nイベント番号；%d"), XX);
			A = 6000;
			B = 000;
			C = 000;
			D = 000;
			E = 000;
		}
		if (XX == 65) {
			wsprintf(S, TEXT("アメリカで忍者ブーム"), XX);
			wsprintf(Sb, TEXT("\n\n\n\n\n\nイベント番号；%d"), XX);
			A = 000;
			B = 000;
			C = 000;
			D = 000;
			E = 2000;
		}
		if (XX == 66) {
			wsprintf(S, TEXT("中国で大規模な反日デモ"), XX);
			wsprintf(Sb, TEXT("\n\n\n\n\n\nイベント番号；%d"), XX);
			A = -2000;
			B = -3000;
			C = 000;
			D = 000;
			E = -3000;
		}
		if (XX == 67) {
			wsprintf(S, TEXT("ヨーロッパで日本のファッションが注目"), XX);
			wsprintf(Sb, TEXT("\n\n\n\n\n\nイベント番号；%d"), XX);
			A = 1000;
			B = 5000;
			C = 1000;
			D = 2000;
			E = 2000;
		}
		if (XX == 68) {
			wsprintf(S, TEXT("新国立競技場が完成"), XX);
			wsprintf(Sb, TEXT("\n\n\n\n\n\nイベント番号；%d"), XX);
			A = 5000;
			B = 5000;
			C = 5000;
			D = 5000;
			E = 5000;
		}
		if (XX == 69) {
			wsprintf(S, TEXT("政府が「50年間で日本の車の半分をエコカーにする」と発表"), XX);
			wsprintf(Sb, TEXT("\n\n\n\n\n\nイベント番号；%d"), XX);
			A = 7000;
			B = 000;
			C = 000;
			D = 000;
			E = 000;
		}
		if (XX == 70) {
			wsprintf(S, TEXT("東南アジアで低価格の洋服が流行"), XX);
			wsprintf(Sb, TEXT("\n\n\n\n\n\nイベント番号；%d"), XX);
			A = 000;
			B = 4000;
			C = 000;
			D = 1000;
			E = 000;
		}
		if (XX == 71) {
			wsprintf(S, TEXT("「チョコレートががん予防になる」という研究結果が発表"), XX);
			wsprintf(Sb, TEXT("\n\n\n\n\n\nイベント番号；%d"), XX);
			A = 000;
			B = 000;
			C = 3000;
			D = 2000;
			E = 000;
		}
		if (XX == 72) {
			wsprintf(S, TEXT("アメリカでとんこつラーメンが大ヒット"), XX);
			wsprintf(Sb, TEXT("\n\n\n\n\n\nイベント番号；%d"), XX);
			A = 000;
			B = 000;
			C = 000;
			D = 2000;
			E = 2000;
		}
		if (XX == 73) {
			wsprintf(S, TEXT("冷凍食品による食中毒が多発"), XX);
			wsprintf(Sb, TEXT("\n\n\n\n\n\nイベント番号；%d"), XX);
			A = 000;
			B = 000;
			C = 000;
			D = -3000;
			E = 000;
		}
		if (XX == 74) {
			wsprintf(S, TEXT("スターウォーズの新作が公開"), XX);
			wsprintf(Sb, TEXT("\n\n\n\n\n\nイベント番号；%d"), XX);
			A = 000;
			B = 1000;
			C = 000;
			D = 1000;
			E = 1000;
		}
		if (XX == 75) {
			wsprintf(S, TEXT("大手企業が会員の個人情報を流出"), XX);
			wsprintf(Sb, TEXT("\n\n\n\n\n\nイベント番号；%d"), XX);
			A = -2000;
			B = -1000;
			C = -1000;
			D = -2000;
			E = -2000;
		}
		if (XX == 76) {
			wsprintf(S, TEXT("日本の化粧品の新商品がアジアでヒット"), XX);
			wsprintf(Sb, TEXT("\n\n\n\n\n\nイベント番号；%d"), XX);
			A = 000;
			B = 000;
			C = 000;
			D = 2000;
			E = 2000;
		}
		if (XX == 77) {
			wsprintf(S, TEXT("サウジアラビアで自爆テロが多発"), XX);
			wsprintf(Sb, TEXT("\n\n\n\n\n\nイベント番号；%d"), XX);
			A = -1000;
			B = -2000;
			C = -1000;
			D = -1000;
			E = -2000;
		}
		if (XX == 78) {
			wsprintf(S, TEXT("中国の人件費が上がる"), XX);
			wsprintf(Sb, TEXT("\n\n\n\n\n\nイベント番号；%d"), XX);
			A = -2000;
			B = -3000;
			C = -1000;
			D = -1000;
			E = -1000;
		}
		if (XX == 79) {
			wsprintf(S, TEXT("駄菓子メーカーの賞味期限偽装が発覚"), XX);
			wsprintf(Sb, TEXT("\n\n\n\n\n\nイベント番号；%d"), XX);
			A = 000;
			B = 000;
			C = -5000;
			D = -1000;
			E = 000;
		}
		if (XX == 80) {
			wsprintf(S, TEXT("日本のオリンピック選手が国民栄誉賞"), XX);
			wsprintf(Sb, TEXT("\n\n\n\n\n\nイベント番号；%d"), XX);
			A = 1000;
			B = 1000;
			C = 1000;
			D = 1000;
			E = 1000;
		}
		if (XX == 81) {
			wsprintf(S, TEXT("人工知能の開発をする企業への補助金制度が決定"), XX);
			wsprintf(Sb, TEXT("\n\n\n\n\n\nイベント番号；%d"), XX);
			A = 4000;
			B = 000;
			C = 000;
			D = 1000;
			E = 1000;
		}
		if (XX == 82) {
			wsprintf(S, TEXT("アメリカ政府が海外からの輸入を大幅に制限"), XX);
			wsprintf(Sb, TEXT("\n\n\n\n\n\nイベント番号；%d"), XX);
			A = -7000;
			B = -1000;
			C = -2000;
			D = -1000;
			E = 000;
		}
		if (XX == 83) {
			wsprintf(S, TEXT("ガソリンが大幅に値上げ"), XX);
			wsprintf(Sb, TEXT("\n\n\n\n\n\nイベント番号；%d"), XX);
			A = -3000;
			B = -2000;
			C = -2000;
			D = -3000;
			E = -3000;
		}
		if (XX == 84) {
			wsprintf(S, TEXT("東京に乗り物博物館がオープン"), XX);
			wsprintf(Sb, TEXT("\n\n\n\n\n\nイベント番号；%d"), XX);
			A = 1000;
			B = 000;
			C = 000;
			D = 000;
			E = 1000;
		}
		if (XX == 85) {
			wsprintf(S, TEXT("東京でピカソ展が開催"), XX);
			wsprintf(Sb, TEXT("\n\n\n\n\n\nイベント番号；%d"), XX);
			A = 000;
			B = 000;
			C = 000;
			D = 1000;
			E = 2000;
		}
		if (XX == 86) {
			wsprintf(S, TEXT("安全上の問題からドローンの使用が禁止に"), XX);
			wsprintf(Sb, TEXT("\n\n\n\n\n\nイベント番号；%d"), XX);
			A = -1000;
			B = 000;
			C = 000;
			D = -1000;
			E = 000;
		}
		if (XX == 87) {
			wsprintf(S, TEXT("大雪で成田空港の飛行機が1日中欠航"), XX);
			wsprintf(Sb, TEXT("\n\n\n\n\n\nイベント番号；%d"), XX);
			A = 000;
			B = 000;
			C = 000;
			D = 000;
			E = -4000;
		}
		if (XX == 88) {
			wsprintf(S, TEXT("箱根で火山が噴火"), XX);
			wsprintf(Sb, TEXT("\n\n\n\n\n\nイベント番号；%d"), XX);
			A = 000;
			B = 000;
			C = 000;
			D = 000;
			E = -4000;
		}
		if (XX == 89) {
			wsprintf(S, TEXT("韓国の大統領が竹島に上陸"), XX);
			wsprintf(Sb, TEXT("\n\n\n\n\n\nイベント番号；%d"), XX);
			A = -1000;
			B = -1000;
			C = 000;
			D = 000;
			E = -3000;
		}
		if (XX == 90) {
			wsprintf(S, TEXT("北海道新幹線が開通"), XX);
			wsprintf(Sb, TEXT("\n\n\n\n\n\nイベント番号；%d"), XX);
			A = 000;
			B = 000;
			C = 000;
			D = 000;
			E = 3000;
		}
		if (XX == 91) {
			wsprintf(S, TEXT("マグロが例年の2倍発生"), XX);
			wsprintf(Sb, TEXT("\n\n\n\n\n\nイベント番号；%d"), XX);
			A = 000;
			B = 000;
			C = 000;
			D = 1000;
			E = 2000;
		}
		if (XX == 92) {
			wsprintf(S, TEXT("フィリピンで軍事クーデターが発生"), XX);
			wsprintf(Sb, TEXT("\n\n\n\n\n\nイベント番号；%d"), XX);
			A = -1000;
			B = -2000;
			C = -1000;
			D = -1000;
			E = -1000;
		}
		if (XX == 93) {
			wsprintf(S, TEXT("養殖によるウナギの大量生産が始まる"), XX);
			wsprintf(Sb, TEXT("\n\n\n\n\n\nイベント番号；%d"), XX);
			A = 000;
			B = 000;
			C = 000;
			D = 1000;
			E = 2000;
		}
		if (XX == 94) {
			wsprintf(S, TEXT("名古屋港が2倍に広がる"), XX);
			wsprintf(Sb, TEXT("\n\n\n\n\n\nイベント番号；%d"), XX);
			A = 6000;
			B = 000;
			C = 000;
			D = 000;
			E = 000;
		}
		if (XX == 95) {
			wsprintf(S, TEXT("明日はバレンタインデーだ"), XX);
			wsprintf(Sb, TEXT("\n\n\n\n\n\nイベント番号；%d"), XX);
			A = 000;
			B = 000;
			C = 3000;
			D = 2000;
			E = 000;
		}
		if (XX == 96) {
			wsprintf(S, TEXT("牛乳の価格が下がる"), XX);
			wsprintf(Sb, TEXT("\n\n\n\n\n\nイベント番号；%d"), XX);
			A = 000;
			B = 000;
			C = 1000;
			D = 1000;
			E = 000;
		}
		if (XX == 97) {
			wsprintf(S, TEXT("牛乳の価格が上がる"), XX);
			wsprintf(Sb, TEXT("\n\n\n\n\n\nイベント番号；%d"), XX);
			A = 000;
			B = 000;
			C = -1000;
			D = -1000;
			E = 000;
		}
		if (XX == 98) {
			wsprintf(S, TEXT("本州と九州を結ぶ新しい橋が完成"), XX);
			wsprintf(Sb, TEXT("\n\n\n\n\n\nイベント番号；%d"), XX);
			A = 000;
			B = 000;
			C = 1000;
			D = 1000;
			E = 3000;
		}








		NE = XX;


		if (XX == 1000){
			wsprintf(S, TEXT("最後の株価を表示しています。"), XX);
			wsprintf(Sb, TEXT("\n\n\n\n\n\nイベント番号；%d"), XX);
			//KillTimer(hWnd,ID_MYTIMER2);
		}

	}



	return;
}



void EData(int XX){
	/*if(F==1){
	A=15000;
	B=15000;
	C=15000;
	D=15000;
	E=15000;

	F=0;
	}else*/ if (EV<his + 3){
		A = (rand() % 7 - 3) * 700;
		B = (rand() % 7 - 3) * 700;
		C = (rand() % 7 - 3) * 700;
		D = (rand() % 7 - 3) * 700;
		E = (rand() % 7 - 3) * 700;


	}
	else{
		A = 0;
		B = 0;
		D = 0;
		if (XX == 0){
			wsprintf(S, TEXT("スーパーハイブリッド車が開発された！"), XX);
			wsprintf(Sb, TEXT("\n\n\n\n\n\nイベント番号；%d"), XX);
			A = 2000;

		}
		if (XX == 1){
			wsprintf(S, TEXT("汗をかくと冷たくなる「スーパークールシャツ」が開発された！"), XX);
			wsprintf(Sb, TEXT("\n\n\n\n\n\nイベント番号；%d"), XX);

			D = 2000;

		}
		if (XX == 2){

			wsprintf(S, TEXT("すべての高層ビルに震度7に耐える耐震工事を義務付ける法律ができた！"), XX);
			wsprintf(Sb, TEXT("\n\n\n\n\n\nイベント番号；%d"), XX);
			B = 2000;
		}
		if (XX == 3){
			wsprintf(S, TEXT("東南海大地震が起きて名古屋中心の沿岸部が壊滅的な被害を受けた！"), XX);
			wsprintf(Sb, TEXT("\n\n\n\n\n\nイベント番号；%d"), XX);
			A = -4000;
			B = 1000;

			D = 1000;
		}
		if (XX == 4){

			wsprintf(S, TEXT("太陽光で動くソーラー自動車が発売された！"), XX);
			wsprintf(Sb, TEXT("\n\n\n\n\n\nイベント番号；%d"), XX);
			A = 2000;

		}
		if (XX == 5){
			wsprintf(S, TEXT("世界的に景気が良くなった！"), XX);
			wsprintf(Sb, TEXT("\n\n\n\n\n\nイベント番号；%d"), XX);
			A = 3000;
			B = 3000;

			D = -1000;

		}
		if (XX == 6){

			wsprintf(S, TEXT("円安になった！"), XX);
			wsprintf(Sb, TEXT("\n\n\n\n\n\nイベント番号；%d"), XX);
			A = 3000;

			D = -2000;

		}
		if (XX == 7){
			wsprintf(S, TEXT("中国と日本の関係が悪化し、一切の貿易が停止した！"), XX);
			wsprintf(Sb, TEXT("\n\n\n\n\n\nイベント番号；%d"), XX);
			A = -1000;
			B = -2000;
			D = -8000;
		}
		if (XX == 8){
			wsprintf(S, TEXT("所得税が上がった！"), XX);
			wsprintf(Sb, TEXT("\n\n\n\n\n\nイベント番号；%d"), XX);
			A = -1000;
			B = -1000;

		}
		if (XX == 9){
			wsprintf(S, TEXT("急激な円高になった！"), XX);
			wsprintf(Sb, TEXT("\n\n\n\n\n\nイベント番号；%d"), XX);
			A = -3000;
			B = 0;

			D = 2000;

		}
		if (XX == 1000){
			wsprintf(S, TEXT("最後の株価を表示しています。"), XX);
			wsprintf(Sb, TEXT("\n\n\n\n\n\nイベント番号；%d"), XX);
			KillTimer(hWnd, ID_MYTIMER2);
		}






	}


	return;
}

void Ftime(int A, HWND hWnd, RECT rc, HDC hdc){

	HPEN hPen, hPen2, hPen3;
	HBRUSH hBrush, hBrushW, hBrushT;
	RECT rc0, rc1, rc2, rc3, rc4, rc6;
	TCHAR szPRL[5][10];
	POINT Lpt[HH];
	int o, i, q;

	hPen = CreatePen(PS_SOLID, 0, RGB(0, 0, 0));
	hBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
	hBrushW = (HBRUSH)GetStockObject(WHITE_BRUSH);
	hBrushT = (HBRUSH)GetStockObject(DC_BRUSH);
	hPen3 = (HPEN)GetStockObject(NULL_PEN);
	hPen2 = CreatePen(PS_SOLID, 4, RGB(0, 0, 0));
	int Tr = 0, Tg = 0, Tb = 0;

	if (A != 6){
		SelectObject(hdc, hBrush);

		if (A == 1){
			rc0.left = 0;
			rc0.top = rc.bottom / 16;
			rc0.right = rc.right / number;
			rc0.bottom = rc.bottom / 9;
			rc1.left = rc0.right;
			rc1.top = rc0.top;
			rc1.right = rc.right / number * 2;
			rc1.bottom = rc0.bottom;
			rc2.top = 2;
			rc2.left = 2;
			rc2.bottom = rc0.top - rc.bottom / 80;
			rc2.right = rc1.right;
			rc3.top = rc0.bottom + rc.bottom / 40;
			rc3.left = rc0.left + rc.right / 40;
			rc3.bottom = rc.bottom / 2 - rc.bottom / 40;
			rc3.right = rc1.right - rc.right / 40;
			wsprintf((LPWSTR)szPRL[2], TEXT("トヨタ自動車"));
			Tr = 204;





		}
		if (A == 2){

			rc0.left = rc.right * 2 / 3;
			rc0.top = rc.bottom / 16;
			rc0.right = rc.right * 2 / 3 + rc.right / number;
			rc0.bottom = rc.bottom / 9;
			rc1.left = rc.right * 2 / 3 + rc.right / number;
			rc1.top = rc.bottom / 16;
			rc1.right = rc.right * 2 / 3 + rc.right / number * 2;
			rc1.bottom = rc0.bottom;
			rc2.top = 2;
			rc2.left = 2 + rc.right * 2 / 3;
			rc2.bottom = rc0.top - rc.bottom / 80;
			rc2.right = rc1.right;
			rc3.top = rc0.bottom + rc.bottom / 40;
			rc3.left = rc0.left + rc.right / 40;
			rc3.bottom = rc.bottom / 2 - rc.bottom / 40;
			rc3.right = rc1.right - rc.right / 40;
			wsprintf((LPWSTR)szPRL[2], TEXT("ユニクロ"));
			if (DQ == 1){ wsprintf((LPWSTR)szPRL[2], TEXT("鹿島建設")); }
			Tr = 255;
			Tg = 255;


		}
		if (A == 3){
			rc0.left = 0;
			rc0.top = rc.bottom / 16 + rc.bottom / 2;
			rc0.right = rc.right / number;
			rc0.bottom = rc.bottom / 9 + rc.bottom / 2;
			rc1.left = rc0.right;
			rc1.top = rc.bottom / 16 + rc.bottom / 2;
			rc1.right = rc.right / number * 2;
			rc1.bottom = rc.bottom / 9 + rc.bottom / 2;
			rc2.top = 2 + rc.bottom / 2;
			rc2.left = 2;
			rc2.bottom = rc0.top - rc.bottom / 80;
			rc2.right = rc1.right;
			rc3.top = rc0.bottom + rc.bottom / 40;
			rc3.left = rc0.left + rc.right / 40;
			rc3.bottom = rc.bottom / 2 - rc.bottom / 40 + rc.bottom / 2;
			rc3.right = rc1.right - rc.right / 40;
			wsprintf((LPWSTR)szPRL[2], TEXT("森永製菓"));

			Tr = 102;
			Tg = 255;
			Tb = 255;
		}
		if (A == 4){
			rc0.left = 0 + rc.right / 3;
			rc0.top = rc.bottom / 16 + rc.bottom / 2;
			rc0.right = rc.right / number + rc.right / 3;
			rc0.bottom = rc.bottom / 9 + rc.bottom / 2;
			rc1.left = rc0.right;
			rc1.top = rc0.top;
			rc1.right = rc.right / number * 2 + rc.right / 3;
			rc1.bottom = rc0.bottom;
			rc2.top = 2 + rc.bottom / 2;
			rc2.left = 2 + rc.right / 3;
			rc2.bottom = rc0.top - rc.bottom / 80;
			rc2.right = rc1.right;
			rc3.top = rc0.bottom + rc.bottom / 40;
			rc3.left = rc0.left + rc.right / 40;
			rc3.bottom = rc.bottom / 2 - rc.bottom / 40 + rc.bottom / 2;
			rc3.right = rc1.right - rc.right / 40;
			wsprintf((LPWSTR)szPRL[2], TEXT("イトーヨーカドー"));
			if (DQ == 1){ wsprintf((LPWSTR)szPRL[2], TEXT("ユニクロ")); }
			Tr = 51;
			Tg = 204;


		}
		if (A == 5){
			rc0.left = rc.right * 2 / 3;
			rc0.top = rc.bottom / 16 + rc.bottom / 2;
			rc0.right = rc.right * 2 / 3 + rc.right / number;
			rc0.bottom = rc.bottom / 9 + rc.bottom / 2;
			rc1.left = rc.right * 2 / 3 + rc.right / number;
			rc1.top = rc0.top;
			rc1.right = rc.right * 2 / 3 + rc.right / number * 2;
			rc1.bottom = rc0.bottom;
			rc2.top = 2 + rc.bottom / 2;
			rc2.left = 2 + rc.right * 2 / 3;
			rc2.bottom = rc0.top - rc.bottom / 80;
			rc2.right = rc1.right;
			rc3.top = rc0.bottom + rc.bottom / 40;
			rc3.left = rc0.left + rc.right / 40;
			rc3.bottom = rc.bottom / 2 - rc.bottom / 40 + rc.bottom / 2;
			rc3.right = rc1.right - rc.right / 40;
			wsprintf((LPWSTR)szPRL[2], TEXT("ＪＴＢ"));
			Tr = 211;
			Tg = 211;
			Tb = 211;



		}
		SetDCBrushColor(hdc, RGB(Tr, Tg, Tb));

		rc2.left += rc.right / 15;
		rc2.right -= rc.right / 15;
		rc2.top += rc.bottom / 200;
		rc2.top += rc.bottom / 200;
		rc2.bottom += rc.bottom / 200;
		/*
		for(o=0;o<his;o++){
		Lpt[o].y=(rc3.bottom-rc3.top)/2+rc3.top;
		}
		*/
		wsprintf((LPWSTR)szPRL[0], TEXT("株価；%d"), price[A - 1][his - 1]);
		/*
		wsprintf((LPWSTR)szPRL[1],TEXT("買；%d"),price[A-1][his-1]+spread[A-1]);
		*/
		wsprintf((LPWSTR)szPRL[3], szPR[A * 2 + 13]);
		wsprintf((LPWSTR)szPRL[4], szPR[A * 2 + 14]);








		/*
		rc0.left=0;
		rc0.top=rc.bottom/16;
		rc0.right=rc.right/number;
		rc0.bottom=rc.bottom/9;
		rc1.left=rc0.right;
		rc1.top=rc0.top;
		rc1.right=rc.right/number*2;
		rc1.bottom=rc0.bottom;
		rc2.top=2;
		rc2.left=2;
		rc2.bottom=rc0.top-rc.bottom/80;
		rc2.right=rc1.right;
		rc3.top=rc0.bottom+rc.bottom/40;
		rc3.left=rc0.left+rc.right/160;
		rc3.bottom=rc.bottom/2-rc.bottom/40;
		rc3.right=rc1.right-rc.right/35;
		*/







		/*初期の枠を作るスクリプト開始*/

		for (o = 0; o<his; o++){
			pt[A - 1][o].x = (rc3.right - rc3.left) / (his)*(o + 1) + rc3.left - rc.right / 300;
		}

		/*
		for(o=0;o<10;o++){
		Lpt[o].x=(rc3.right-rc3.left)/(his)*(o)+rc3.left;
		}
		*/


		SelectObject(hdc, hPen);
		for (i = 0; i<2; i++){
			for (q = 0; q<number / 2; q++){
				Rectangle(hdc, rc.right*q * 2 / number, rc.bottom*i / 2, rc.right*(q + 1) * 2 / number, rc.bottom*(i + 1) / 2);
			}
		}

		/*初期の枠を作るスクリプト終了*/
		/*数字処理start*/

		SelectObject(hdc, hBrushW);
		SelectObject(hdc, hPen3);
		Rectangle(hdc, rc0.left, rc0.top, rc0.right, rc0.bottom);
		Rectangle(hdc, rc1.left, rc1.top, rc1.right, rc1.bottom);


		RECT rc40;
		rc40.left = (rc0.left + rc1.left) / 2;
		rc40.right = (rc0.right + rc1.right) / 2;
		rc40.bottom = (rc0.bottom + rc1.bottom) / 2;
		rc40.top = (rc0.top + rc1.top) / 2;

		RECT rc41;
		rc41.left = rc40.left;
		rc41.right = rc40.right;
		rc41.bottom = rc40.bottom;
		rc41.top = rc40.top + rc.bottom / 100;



		SelectObject(hdc, hBrushT);

		RoundRect(hdc, rc2.left, rc2.top - rc.bottom / 200, rc2.right, rc2.bottom, (rc.right + rc.bottom) / 50, (rc.right + rc.bottom) / 50);

		SelectObject(hdc, hPen);

		SelectObject(hdc, hFont6);


		SelectObject(hdc, hBrush);

		SetTextColor(hdc, RGB(0, 0, 0));

		DrawText(hdc, szPRL[2], -1, &rc2, DT_CENTER);

		/************/

		SelectObject(hdc, hBrushW);

		SelectObject(hdc, hPen);

		/*

		Rectangle(hdc,rc40.left,rc40.top,rc40.right,rc40.bottom);

		*/
		Rectangle(hdc, rc3.left, rc3.top, rc3.right, rc3.bottom);





		SetTextColor(hdc, RGB(0, 0, 255));
		DrawText(hdc, szPRL[0], -1, &rc41, DT_CENTER);
		/*
		SetTextColor(hdc,RGB(255,0,0));
		DrawText(hdc,szPRL[1],-1,&rc1,DT_CENTER);
		*/






		/*数字処理end*/


		SelectObject(hdc, hPen2);





		Polyline(hdc, pt[A - 1], HH);
		/*
		SelectObject(hdc,hBrush);
		*/
		SelectObject(hdc, hFont3);
		SetTextColor(hdc, RGB(255, 102, 51));
		TextOut(hdc, rc3.right, rc3.top, szPRL[3], lstrlen(szPRL[3]));
		SetTextColor(hdc, RGB(102, 102, 255));
		TextOut(hdc, rc3.right, rc3.bottom - 20, szPRL[4], lstrlen(szPRL[4]));

	}
	if (A == 6){
		rc0.left = 0;
		rc0.top = rc.bottom / 16;
		rc0.right = rc.right / number;
		rc0.bottom = rc.bottom / 9;
		rc1.left = rc0.right;
		rc1.top = rc0.top;
		rc1.right = rc.right / number * 2;
		rc1.bottom = rc0.bottom;
		rc2.top = 2;
		rc2.left = 2;
		rc2.bottom = rc0.top - rc.bottom / 80;
		rc2.right = rc1.right;
		rc3.top = rc0.bottom + rc.bottom / 40;
		rc3.left = rc0.left + rc.right / 80;
		rc3.bottom = rc.bottom / 2 - rc.bottom / 40;
		rc3.right = rc1.right - rc.right / 80;
		rc4.left = rc1.right + rc.right / 20;
		rc4.top = rc.bottom / 60;
		rc4.bottom = rc0.top;
		rc4.right = rc.right / number * 2 * 2 - rc.right / 100;
		/*時計処理start*/
		TCHAR sur[10];
		SelectObject(hdc, hFont2);
		wsprintf(sur, TEXT("%3d"), Tm / 1000);

		SetTextColor(hdc, RGB(0, 51, 0));
		TextOut(hdc, rc1.right + rc.right / 50, rc4.top + rc4.top * 3 / 4, sur, lstrlen(sur));
		SelectObject(hdc, hPen);
		SelectObject(hdc, hBrushW);
		Rectangle(hdc, rc4.left, rc4.top, rc4.right, rc4.bottom);
		hBrush = CreateSolidBrush(RGB(204, 255, 102));
		SelectObject(hdc, hBrush);


		Rectangle(hdc, rc4.left, rc4.top, rc4.right, rc4.bottom);

		SelectObject(hdc, hBrushW);

		Rectangle(hdc, rc4.right - (DTM - Tm)*(rc4.right - rc4.left) / DTM, rc4.top, rc4.right, rc4.bottom);

		/*時計処理end*/
		/*ニュース処理start*/
		SelectObject(hdc, hFont1);
		SetTextColor(hdc, RGB(153, 102, 0));
		TextOut(hdc, rc1.right + rc.right / 50, rc4.top + rc4.top * 3, TEXT("～ニュース～"), lstrlen(TEXT("～ニュース～")));

		rc6.top = rc0.bottom + rc.bottom / 40;
		rc6.left = rc0.left + rc.right / 160 + rc.right / 3;
		rc6.bottom = rc.bottom / 2 - rc.bottom / 10;
		rc6.right = rc1.right - rc.right / 32 + rc.right / 3;


		SelectObject(hdc, hFont6);
		DrawText(hdc, S, lstrlen(S), &rc6, DT_CENTER | DT_WORDBREAK);

		SelectObject(hdc, hFont3);
		DrawText(hdc, Sb, lstrlen(Sb), &rc6, DT_CENTER | DT_WORDBREAK);

		rc23.left = rc.right / 2 + rc.right / 10;
		rc23.right = rc.right * 2 / 3 - rc.right / 30;
		rc23.top = rc.bottom * 2 / 7;
		rc23.bottom = rc.bottom / 2 - rc.bottom / 10;

		rc24.left = rc.right / 3;
		rc24.right = rc.right * 3 / 5 - rc.right / 10;
		rc24.top = rc.bottom * 2 / 7;
		rc24.bottom = rc.bottom / 2;

		rc25.left = rc.right / 2 + rc.right / 20;
		rc25.right = rc.right * 2 / 3 + rc.right / 50;
		rc25.top = rc.bottom / 2 - rc.bottom / 10;
		rc25.bottom = rc.bottom / 2;


		if (DQ != 1){


			SelectObject(hdc, hBrushW);
			SelectObject(hdc, hPen3);


			Rectangle(hdc, rc23.left, rc23.top, rc23.right, rc23.bottom);

			Rectangle(hdc, rc24.left, rc24.top, rc24.right, rc24.bottom);

			SelectObject(hdc, hPen);
			Ellipse(hdc, rc23.left + (rc23.right - rc23.left) / 2 - ((rc23.bottom - rc.bottom / 23) - (rc23.top - rc.bottom / 100)) / 2 - rc.bottom / 100, rc23.top - rc.bottom / 100 - rc.bottom / 100, (rc23.right - rc23.left) / 2 + ((rc23.bottom - rc.bottom / 23) - (rc23.top - rc.bottom / 100)) / 2 + rc23.left + rc.bottom / 100, rc23.bottom - rc.bottom / 23 + rc.bottom / 100);



			SelectObject(hdc, hFont5);
			SelectObject(hdc, hPen);


			SetTextColor(hdc, RGB(rR, rG, rB));

			DrawText(hdc, Yrate, lstrlen(Yrate), &rc23, DT_CENTER | DT_WORDBREAK);

			SelectObject(hdc, hFont6);
			SetTextColor(hdc, RGB(102, 51, 255));

			DrawText(hdc, Rate, lstrlen(Rate), &rc24, DT_CENTER | DT_WORDBREAK);

			SetTextColor(hdc, RGB(51, 102, 0));
			SelectObject(hdc, hFont6);



			DrawText(hdc, Urate, lstrlen(Urate), &rc25, DT_CENTER | DT_WORDBREAK);

		}





	}

	if (A != 6){

		if (AS[A - 1]>0){
			SelectObject(hdc, hBrushw);
			Rectangle(hdc, rc0.left, rc0.top, rc1.right, rc3.bottom);
			SelectObject(hdc, hFont3);
			wsprintf((LPWSTR)CON, TEXT("\nこの会社は倒産しました。\n株のカードをボードに戻してください。"));
			DrawText(hdc, CON, -1, &rc3, DT_CENTER);


			/*
			price[A-1][HH]=15000+q;
			*/
		}


	}





	/****/




	DeleteObject(hBrush);
	DeleteObject(hBrushW);
	DeleteObject(hPen);
	DeleteObject(hPen2);
	/*
	DeleteObject(hFont4);
	DeleteObject(hFont5);
	*/
	return;
}
