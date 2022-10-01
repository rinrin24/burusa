// win01.cpp

#define HH 10

#include <Windows.h>
#include <iostream>

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
//それぞれの企業の株価の増減分
static int companyAStockPriceChangeAmount, companyBStockPriceChangeAmount, companyCStockPriceChangeAmount, companyDStockPriceChangeAmount, companyEStockPriceChangeAmount;

//ウィンドウプロシージャ
//それぞれの企業の株価 一次元目で企業を指定、二次元目で期間を指定
static int stockPrices[5][HH + 1];

//画面に表示する期間の数
const int his = HH;
static int EV = 0;
TCHAR S[100];
TCHAR Sb[30];
static int number = 6;/*株価の数*/
int spread[5] = { 10, 20, 30, 40, 50 };
static POINT stockPriceChartGraphPoint[5][HH];
//文字色について扱うグローバル変数
int rR, rG, rB;
//nNEは意味のない？
int NE, nNE;
HBRUSH hBrushw, hBrush;
static HFONT hFont1, hFont2, hFont3;
static HFONT hFont4, hFont5, hFont6;
PAINTSTRUCT ps;
HDC hdc;
int currentRemainingTime, limitedTime;
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
	RECT clientRectangle;
	RECT rc0, rc1, rc2, rc3;
	RECT rc4;

	int i, q;
	i = 0;
	int o;
	int id;

	static int maxStockPriceInPeriod, minStockPriceInPeriod;

	static int XX;

	/*数字処理start*/

	GetClientRect(hWnd, &clientRectangle);

	/*数字処理end*/

	switch (msg) {
	case WM_CREATE:
		for (o = 0; o < nD; o++) {
			nLOOP[o] = 0;
		}
		hPen10 = CreatePen(PS_SOLID, 0, RGB(0, 0, 0));
		hPen5 = (HPEN)GetStockObject(BLACK_PEN);
		hPen4 = (HPEN)GetStockObject(NULL_PEN);
		hBrushw = (HBRUSH)GetStockObject(WHITE_BRUSH);
		hBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
		limitedTime = 10000;
		currentRemainingTime = limitedTime;
		XO = 1;
		TEST = 0;
		Rpr = 110;
		rc0.left = 0;
		rc0.top = clientRectangle.bottom / 16;
		rc0.right = clientRectangle.right / number;
		rc0.bottom = clientRectangle.bottom / 9;
		rc1.left = rc0.right;
		rc1.top = rc0.top;
		rc1.right = clientRectangle.right / number * 2;
		rc1.bottom = rc0.bottom;
		rc2.top = 2;
		rc2.left = 2;
		rc2.bottom = rc0.top - clientRectangle.bottom / 80;
		rc2.right = rc1.right;
		rc3.top = rc0.bottom + clientRectangle.bottom / 40;
		rc3.left = rc0.left + clientRectangle.right / 160;
		rc3.bottom = clientRectangle.bottom / 2 - clientRectangle.bottom / 40;
		rc3.right = rc1.right - clientRectangle.right / 35;

		/*それぞれの全期間での株価の初期化*/
		for (o = 0; o < HH; o++) {
			stockPrices[0][o] = 15000;
		}
		for (o = 0; o < HH; o++) {
			stockPrices[1][o] = 15000;
		}
		for (o = 0; o < HH; o++) {
			stockPrices[2][o] = 15000;
		}
		for (o = 0; o < HH; o++) {
			stockPrices[3][o] = 15000;
		}
		for (o = 0; o < HH; o++) {
			stockPrices[4][o] = 15000;
		}
		SetTimer(hWnd, ID_MYTIMER, 200, NULL);

		srand((unsigned)time(NULL));
		for (o = 0; o < 5; o++) {
			wsprintf((LPWSTR)szPR[0 + o * 2], TEXT("売；%d"), stockPrices[0 + o][his - 1] - spread[o]);
			wsprintf((LPWSTR)szPR[1 + o * 2], TEXT("買；%d"), stockPrices[0 + o][his - 1] + spread[o]);
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

		for (o = 0; o < his; o++) {
			stockPriceChartGraphPoint[0][o].y = (rc3.bottom - rc3.top) / 2 + rc3.top;
			stockPriceChartGraphPoint[0][o].x = (rc3.right - rc3.left) / (his) * (o + 1) + rc3.left;
			stockPriceChartGraphPoint[1][o].y = (rc3.bottom - rc3.top) / 2 + rc3.top;
			stockPriceChartGraphPoint[1][o].x = (rc3.right - rc3.left) / (his) * (o + 1) + rc3.left + clientRectangle.right * 2 / 3;
			stockPriceChartGraphPoint[2][o].y = (rc3.bottom - rc3.top) / 2 + rc3.top + clientRectangle.bottom / 2;
			stockPriceChartGraphPoint[2][o].x = (rc3.right - rc3.left) / (his) * (o + 1) + rc3.left;
			stockPriceChartGraphPoint[3][o].y = (rc3.bottom - rc3.top) / 2 + rc3.top + clientRectangle.bottom / 2;
			stockPriceChartGraphPoint[3][o].x = (rc3.right - rc3.left) / (his) * (o + 1) + rc3.left + clientRectangle.right / 3;
			stockPriceChartGraphPoint[4][o].y = (rc3.bottom - rc3.top) / 2 + rc3.top + clientRectangle.bottom / 2;
			stockPriceChartGraphPoint[4][o].x = (rc3.right - rc3.left) / (his) * (o + 1) + rc3.left + clientRectangle.right * 2 / 3;
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

		break;
	case WM_CLOSE:
		id = MessageBox(hWnd,
			TEXT("終了してもよろしいですか"),
			TEXT("確認"),
			MB_YESNO | MB_ICONQUESTION);
		if (id == IDYES) {
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
		Rectangle(hdc, clientRectangle.left, clientRectangle.top, clientRectangle.right, clientRectangle.bottom);

		if (!(EV < his + 3)) {
			if (XO == 1) {
				Ftime(1, hWnd, clientRectangle, hdc);
				Ftime(2, hWnd, clientRectangle, hdc);
				if (DQ != 1) {
					Ftime(3, hWnd, clientRectangle, hdc);
					Ftime(5, hWnd, clientRectangle, hdc);
				}
				Ftime(4, hWnd, clientRectangle, hdc);
				Ftime(6, hWnd, clientRectangle, hdc);
			}

			if (XO == 0) {
				bmph = clientRectangle.bottom / 9;
				bmpw = bmph;
				SelectObject(hdc, hBrushw);
				Rectangle(hdc, clientRectangle.left, clientRectangle.top, clientRectangle.right, clientRectangle.bottom);
				SelectObject(hdc, hFont5);
				wsprintf((LPWSTR)CON, TEXT("\n今からの取引はできません。\n少々お待ちください。"));
				DrawText(hdc, CON, -1, &clientRectangle, DT_CENTER);
				wsprintf((LPWSTR)CON, TEXT("\n\n\n\n\n次回の制限時間；%d秒"), limitedTime / 1000);
				DrawText(hdc, CON, -1, &clientRectangle, DT_CENTER);

				SelectObject(hdc_mem, hBmp);

				Rectangle(hdc, clientRectangle.right / 2 - bmpw / 2, clientRectangle.bottom * 3 / 4 - bmph, clientRectangle.right / 2 + bmpw / 2, clientRectangle.bottom * 3 / 4);
				StretchBlt(hdc,		//転送先
					clientRectangle.right / 2 - bmpw / 2,	//転送先x座標
					clientRectangle.bottom * 3 / 4 - bmph,	//転送先y座標
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

		if (EV < his + 3) {
			SelectObject(hdc, hBrushw);

			Rectangle(hdc, clientRectangle.left, clientRectangle.top, clientRectangle.right, clientRectangle.bottom);
			SelectObject(hdc, hPen5);
			SelectObject(hdc, hFont5);
			wsprintf((LPWSTR)YOM, TEXT("\n読み込み中です。\n少々お待ちください。"));
			DrawText(hdc, YOM, -1, &clientRectangle, DT_CENTER);
			wsprintf((LPWSTR)YOM, TEXT("\n\n\n\n\n%d％完了"), Qw * 100 / (his + 1 + limitedTime / 1000));
			DrawText(hdc, YOM, -1, &clientRectangle, DT_CENTER);
		}
		EndPaint(hWnd, &ps);

		break;
	case WM_TIMER:
		if (EV == his + 3) {
			KillTimer(hWnd, ID_MYTIMER);
			SetTimer(hWnd, ID_MYTIMER2, 1000, NULL);
		}
		Qw++;

		if (XO == 1) {

			rc0.left = 0;
			rc0.top = clientRectangle.bottom / 16;
			rc0.right = clientRectangle.right / number;
			rc0.bottom = clientRectangle.bottom / 9;
			rc1.left = rc0.right;
			rc1.top = rc0.top;
			rc1.right = clientRectangle.right / number * 2;
			rc1.bottom = rc0.bottom;
			rc2.top = 2;
			rc2.left = 2;
			rc2.bottom = rc0.top - clientRectangle.bottom / 80;
			rc2.right = rc1.right;
			rc3.top = rc0.bottom + clientRectangle.bottom / 40;
			rc3.left = rc0.left + clientRectangle.right / 80;
			rc3.bottom = clientRectangle.bottom / 2 - clientRectangle.bottom / 40;
			rc3.right = rc1.right - clientRectangle.right / 80;

			rc4.left = rc1.right + clientRectangle.right / 20;
			rc4.top = clientRectangle.bottom / 60;
			rc4.bottom = rc0.top;
			rc4.right = clientRectangle.right / number * 2 * 2 - clientRectangle.right / 100;

			if (EV < his + 2) {
				currentRemainingTime -= limitedTime - 1000;

			}

			currentRemainingTime -= 1000;

			RECT rc5;
			rc5.left = clientRectangle.right / 3;
			rc5.right = rc4.right;
			rc5.top = rc4.top;
			rc5.bottom = rc4.bottom;


			InvalidateRect(hWnd, &rc5, 0);

			if (EV < his + 3) {
				InvalidateRect(hWnd, NULL, 0);
			}

			if (currentRemainingTime == 0) {

				if (!(EV < his + 2)) {
					XO = 0;
					TEST = 1;
				}
				/*株価・チャート移動のためのスクリプトstart*/

				EV++;

				if (EV == 2) {
					stockPrices[0][his] = 15000;
					stockPrices[1][his] = 15000;
					stockPrices[2][his] = 15000;
					stockPrices[3][his] = 15000;
					stockPrices[4][his] = 15000;
				}

				o = 0;

				/**goto文を使っています。注意！**/
				if (DQ != 1) {
				loop:
					XX = rand() % nD;

					if (XX == 108) {
						goto loop;
					}
					if (XX == 110) {
						goto loop;
					}
					if (XX == 111) {
						goto loop;
					}
					if (XX == 112) {
						goto loop;
					}
					if (XX >= 123 && XX <= 130) {
						goto loop;
					}


					if (nLOOP[XX] == 1) {
						o++;
						if (o == 1000000) {
							Data(1000);
							goto out;
						}
						goto loop;
					}
					if (!(EV < his + 3)) {
						nLOOP[XX] = 1;
					}
					if (!(EV < his + 3)) {
						if (NE == 36) {
							Data(34);
							nLOOP[34] = 1;
							NE = 0;
						}
						else {
							Data(XX);
						}
					}
				out:
					/*
					try {
						while (true) {
							XX = rand() % nD;
							if (XX == 108) {
								throw std::string(std::to_string(XX));
								continue;
							}
							if (XX == 110) {
								throw std::string(std::to_string(XX));
								continue;
							}
							if (XX == 111) {
								throw std::string(std::to_string(XX));
								continue;
							}
							if (XX == 112) {
								throw std::string(std::to_string(XX));
								continue;
							}
							if (XX >= 123 && XX <= 130) {
								throw std::string(std::to_string(XX));
								continue;
							}


							if (nLOOP[XX] == 1) {
								o++;
								if (o == 1000000) {
									Data(1000);
									break;
								}
								continue;
							}
							if (!(EV < his + 3)) {
								nLOOP[XX] = 1;
							}
							if (!(EV < his + 3)) {
								if (NE == 36) {
									Data(34);
									nLOOP[34] = 1;
									NE = 0;
								}
								else {
									Data(XX);
								}
							}
						}
					}
					catch (std::string errorMessage) {
						std::cout << errorMessage << std::endl;
					}*/
					o = 1;
				}

				if (DQ == 1) {
				Eloop:
					XX = rand() % EnD;
					if (nLOOP[XX] == 1) {
						o++;
						if (o == 100000000) {
							EData(1000);
							goto Eout;
						}
						goto Eloop;
					}
					if (!(EV < his + 3)) {
						nLOOP[XX] = 1;
					}
					EData(XX);
				Eout:
					o = 1;
				}

				/**goto文を使っています。注意！**/

				/////////////////////////////////////////////////////////////

				for (q = 0; q < 5; q++) {

					for (o = 0; o < his; o++) {
						stockPrices[q][o] = stockPrices[q][o + 1];
					}

					wsprintf((LPWSTR)szPR[0], TEXT("株価；%d"), stockPrices[q][his - 1] - spread[q]);

					for (o = 0; o < his; o++) {
						if (o == 0) {
							maxStockPriceInPeriod = stockPrices[q][o];
						}
						else if (stockPrices[q][o] > maxStockPriceInPeriod) {
							maxStockPriceInPeriod = stockPrices[q][o];
						}
					}

					for (o = 0; o < his; o++) {
						if (o == 0) {
							minStockPriceInPeriod = stockPrices[q][o];
						}
						else if (stockPrices[q][o] < minStockPriceInPeriod) {
							minStockPriceInPeriod = stockPrices[q][o];
						}
					}

					if (maxStockPriceInPeriod != minStockPriceInPeriod) {
						if (q < 2) {
							for (o = 0; o < his; o++) {
								i = (stockPrices[q][o] - minStockPriceInPeriod) * 100 / (maxStockPriceInPeriod - minStockPriceInPeriod) * ((clientRectangle.bottom / 2 - clientRectangle.bottom / 30) - (rc0.bottom + clientRectangle.bottom / 30));
								stockPriceChartGraphPoint[q][o].y = clientRectangle.bottom / 2 - clientRectangle.bottom / 30 - i / 100;
							}
						}
						else {
							for (o = 0; o < his; o++) {
								i = (stockPrices[q][o] - minStockPriceInPeriod) * 100 / (maxStockPriceInPeriod - minStockPriceInPeriod) * ((clientRectangle.bottom / 2 - clientRectangle.bottom / 30) - (rc0.bottom + clientRectangle.bottom / 30));
								stockPriceChartGraphPoint[q][o].y = clientRectangle.bottom / 2 - clientRectangle.bottom / 30 - i / 100 + clientRectangle.bottom / 2;
							}
						}
					}
					else {
						if (q < 2) {
							for (o = 0; o < his; o++) {
								stockPriceChartGraphPoint[q][o].y = (rc3.bottom - rc3.top) / 2 + rc3.top;
							}
						}
						else {
							for (o = 0; o < his; o++) {
								stockPriceChartGraphPoint[q][o].y = (rc3.bottom - rc3.top) / 2 + rc3.top + clientRectangle.bottom / 2;
							}
						}
					}

					wsprintf(szPR[q * 2 + 15], TEXT("%d"), maxStockPriceInPeriod);
					wsprintf(szPR[q * 2 + 16], TEXT("%d"), minStockPriceInPeriod);

				}

				currentRemainingTime = limitedTime;

				/*Unknown*/
				stockPrices[0][his] = stockPrices[0][his - 1] + companyAStockPriceChangeAmount;
				stockPrices[1][his] = stockPrices[1][his - 1] + companyBStockPriceChangeAmount;
				stockPrices[2][his] = stockPrices[2][his - 1] + companyCStockPriceChangeAmount;
				stockPrices[3][his] = stockPrices[3][his - 1] + companyDStockPriceChangeAmount;
				stockPrices[4][his] = stockPrices[4][his - 1] + companyEStockPriceChangeAmount;

				/*Unknown*/

				/*為替+ニュースまとめstart*/

				for (o = 0; o < 5; o++) {
					q = stockPrices[o][HH] % 1000;
					if (q >= 500) {
						stockPrices[o][HH] += 1000;
					}
					stockPrices[o][HH] -= q;
				}

				if (EV < his + 3) {
					for (o = 0; o < 5; o++) {
						if (stockPrices[o][HH] < 12000) {
							stockPrices[o][HH] = 12000;
						}
					}

					for (o = 0; o < 5; o++) {
						if (stockPrices[o][HH] > 18000) {
							stockPrices[o][HH] = 18000;
						}
					}

				}
				if (EV > HH - 2) {
					for (o = 0; o < 5; o++) {
						if (stockPrices[o][his - 1] < 100) {
							AS[o] = 2;
						}
						if (AS[o] > 0) { AS[o]--; }
					}
				}
				int r;
				for (int w = 0; w < 5; w++) {
					if (AS[w] > 0) {
						for (o = 0; o < HH + 1; o++) {
							r = rand() % 4001 - 2000;
							stockPrices[w][o] = 15000 + r;

							q = stockPrices[w][o] % 1000;
							if (q >= 500) {
								stockPrices[w][o] += 1000;
							}
							stockPrices[w][o] -= q;
						}
					}
				}
				/*為替+ニュースまとめend*/
				/*為替end*/
				InvalidateRect(hWnd, NULL, 0);
			}/**currentRemainingTime==0**/
			/*株価・チャート移動のためのスクリプトend*/
		}
		break;
	case WM_CHAR:
		if (wp == VK_ESCAPE) {
			if (XO == 0) XO = 1; InvalidateRect(hWnd, &clientRectangle, 0); break;
			XO = 0;
		}
		if (wp == VK_RETURN) {
			if (limitedTime == 1000) limitedTime -= 1000;
			limitedTime += 10000;
			currentRemainingTime = limitedTime;
			InvalidateRect(hWnd, NULL, 0);

		}
		if (wp == VK_SPACE) {
			limitedTime -= 10000;
			if (limitedTime < 1000) {
				limitedTime = 1000;
			}
			InvalidateRect(hWnd, NULL, 0);
			currentRemainingTime = limitedTime;
		}
		break;
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

void Data(int XX) {
	if (EV < his + 3) {
		companyAStockPriceChangeAmount = (rand() % 7 - 3) * 700;
		companyBStockPriceChangeAmount = (rand() % 7 - 3) * 700;
		companyCStockPriceChangeAmount = (rand() % 7 - 3) * 700;
		companyDStockPriceChangeAmount = (rand() % 7 - 3) * 700;
		companyEStockPriceChangeAmount = (rand() % 7 - 3) * 700;


	}
	else {
		if (XX == 0) {
			wsprintf(S, TEXT("本州と九州を結ぶ新しい橋が完成"), XX);
			wsprintf(Sb, TEXT("\n\n\n\n\n\nイベント番号；%d"), XX);
			companyAStockPriceChangeAmount = 000;
			companyBStockPriceChangeAmount = 000;
			companyCStockPriceChangeAmount = 1000;
			companyDStockPriceChangeAmount = 1000;
			companyEStockPriceChangeAmount = 3000;
		}
		if (XX == 1) {
			wsprintf(S, TEXT("南海トラフ地震が発生"), XX);
			wsprintf(Sb, TEXT("\n\n\n\n\n\nイベント番号；%d"), XX);
			companyAStockPriceChangeAmount = -0;
			companyBStockPriceChangeAmount = -3000;
			companyCStockPriceChangeAmount = -4000;
			companyDStockPriceChangeAmount = -3000;
			companyEStockPriceChangeAmount = -5000;
		}
		if (XX == 2) {
			wsprintf(S, TEXT("タイで大洪水が発生"), XX);
			wsprintf(Sb, TEXT("\n\n\n\n\n\nイベント番号；%d"), XX);
			companyAStockPriceChangeAmount = -4000;
			companyBStockPriceChangeAmount = -1000;
			companyCStockPriceChangeAmount = 000;
			companyDStockPriceChangeAmount = 000;
			companyEStockPriceChangeAmount = -2000;
		}
		if (XX == 3) {
			wsprintf(S, TEXT("日本全国で米が不作"), XX);
			wsprintf(Sb, TEXT("\n\n\n\n\n\nイベント番号；%d"), XX);
			companyAStockPriceChangeAmount = 000;
			companyBStockPriceChangeAmount = 000;
			companyCStockPriceChangeAmount = 000;
			companyDStockPriceChangeAmount = -2000;
			companyEStockPriceChangeAmount = -1000;
		}
		if (XX == 4) {
			wsprintf(S, TEXT("愛知県に巨大台風が上陸"), XX);
			wsprintf(Sb, TEXT("\n\n\n\n\n\nイベント番号；%d"), XX);
			companyAStockPriceChangeAmount = -3000;
			companyBStockPriceChangeAmount = -1000;
			companyCStockPriceChangeAmount = -1000;
			companyDStockPriceChangeAmount = -1000;
			companyEStockPriceChangeAmount = -1000;
		}
		if (XX == 5) {
			wsprintf(S, TEXT("ハワイで大規模な火山噴火"), XX);
			wsprintf(Sb, TEXT("\n\n\n\n\n\nイベント番号；%d"), XX);
			companyAStockPriceChangeAmount = 000;
			companyBStockPriceChangeAmount = 000;
			companyCStockPriceChangeAmount = 000;
			companyDStockPriceChangeAmount = 000;
			companyEStockPriceChangeAmount = -5000;
		}
		if (XX == 6) {
			wsprintf(S, TEXT("九州地方で地震が発生"), XX);
			wsprintf(Sb, TEXT("\n\n\n\n\n\nイベント番号；%d"), XX);
			companyAStockPriceChangeAmount = -2000;
			companyBStockPriceChangeAmount = -1000;
			companyCStockPriceChangeAmount = -1000;
			companyDStockPriceChangeAmount = -1000;
			companyEStockPriceChangeAmount = -2000;
		}
		if (XX == 7) {
			wsprintf(S, TEXT("京都が集中豪雨で大きな被害"), XX);
			wsprintf(Sb, TEXT("\n\n\n\n\n\nイベント番号；%d"), XX);
			companyAStockPriceChangeAmount = -2000;
			companyBStockPriceChangeAmount = 000;
			companyCStockPriceChangeAmount = 000;
			companyDStockPriceChangeAmount = 000;
			companyEStockPriceChangeAmount = -2000;
		}
		if (XX == 8) {
			wsprintf(S, TEXT("鎌倉が世界遺産に"), XX);
			wsprintf(Sb, TEXT("\n\n\n\n\n\nイベント番号；%d"), XX);
			companyAStockPriceChangeAmount = 000;
			companyBStockPriceChangeAmount = 000;
			companyCStockPriceChangeAmount = 000;
			companyDStockPriceChangeAmount = 000;
			companyEStockPriceChangeAmount = 3000;
		}
		if (XX == 9) {
			wsprintf(S, TEXT("富士山が世界遺産に"), XX);
			wsprintf(Sb, TEXT("\n\n\n\n\n\nイベント番号；%d"), XX);
			companyAStockPriceChangeAmount = 000;
			companyBStockPriceChangeAmount = 000;
			companyCStockPriceChangeAmount = 000;
			companyDStockPriceChangeAmount = 000;
			companyEStockPriceChangeAmount = 3000;
		}
		if (XX == 10) {
			wsprintf(S, TEXT("日本を訪れる外国人が3000万人を超える"), XX);
			wsprintf(Sb, TEXT("\n\n\n\n\n\nイベント番号；%d"), XX);
			companyAStockPriceChangeAmount = 000;
			companyBStockPriceChangeAmount = 1000;
			companyCStockPriceChangeAmount = 1000;
			companyDStockPriceChangeAmount = 1000;
			companyEStockPriceChangeAmount = 6000;
		}
		if (XX == 11) {
			wsprintf(S, TEXT("伊勢志摩サミットが開催"), XX);
			wsprintf(Sb, TEXT("\n\n\n\n\n\nイベント番号；%d"), XX);
			companyAStockPriceChangeAmount = 1000;
			companyBStockPriceChangeAmount = 000;
			companyCStockPriceChangeAmount = 000;
			companyDStockPriceChangeAmount = 000;
			companyEStockPriceChangeAmount = 2000;
		}
		if (XX == 12) {
			wsprintf(S, TEXT("フランスでテロ"), XX);
			wsprintf(Sb, TEXT("\n\n\n\n\n\nイベント番号；%d"), XX);
			companyAStockPriceChangeAmount = -2000;
			companyBStockPriceChangeAmount = 000;
			companyCStockPriceChangeAmount = 000;
			companyDStockPriceChangeAmount = -1000;
			companyEStockPriceChangeAmount = -4000;
		}
		if (XX == 13) {
			wsprintf(S, TEXT("電気料金が値上げ"), XX);
			wsprintf(Sb, TEXT("\n\n\n\n\n\nイベント番号；%d"), XX);
			companyAStockPriceChangeAmount = -2000;
			companyBStockPriceChangeAmount = -1000;
			companyCStockPriceChangeAmount = -1000;
			companyDStockPriceChangeAmount = -2000;
			companyEStockPriceChangeAmount = -1000;
		}
		if (XX == 14) {
			wsprintf(S, TEXT("石油の値段が上がる"), XX);
			wsprintf(Sb, TEXT("\n\n\n\n\n\nイベント番号；%d"), XX);
			companyAStockPriceChangeAmount = -1000;
			companyBStockPriceChangeAmount = -1000;
			companyCStockPriceChangeAmount = -1000;
			companyDStockPriceChangeAmount = -1000;
			companyEStockPriceChangeAmount = -3000;
		}
		if (XX == 15) {
			wsprintf(S, TEXT("石油の値段が下がる"), XX);
			wsprintf(Sb, TEXT("\n\n\n\n\n\nイベント番号；%d"), XX);
			companyAStockPriceChangeAmount = 1000;
			companyBStockPriceChangeAmount = 1000;
			companyCStockPriceChangeAmount = 1000;
			companyDStockPriceChangeAmount = 1000;
			companyEStockPriceChangeAmount = 3000;
		}
		if (XX == 16) {
			wsprintf(S, TEXT("鉄の値段が上がる"), XX);
			wsprintf(Sb, TEXT("\n\n\n\n\n\nイベント番号；%d"), XX);
			companyAStockPriceChangeAmount = -2000;
			companyBStockPriceChangeAmount = 000;
			companyCStockPriceChangeAmount = 000;
			companyDStockPriceChangeAmount = 000;
			companyEStockPriceChangeAmount = 000;
		}
		if (XX == 17) {
			wsprintf(S, TEXT("鉄の値段が下がる"), XX);
			wsprintf(Sb, TEXT("\n\n\n\n\n\nイベント番号；%d"), XX);
			companyAStockPriceChangeAmount = 2000;
			companyBStockPriceChangeAmount = 000;
			companyCStockPriceChangeAmount = 000;
			companyDStockPriceChangeAmount = 000;
			companyEStockPriceChangeAmount = 000;
		}
		if (XX == 18) {
			wsprintf(S, TEXT("今年の冬は暖冬になると予想"), XX);
			wsprintf(Sb, TEXT("\n\n\n\n\n\nイベント番号；%d"), XX);
			companyAStockPriceChangeAmount = 000;
			companyBStockPriceChangeAmount = -2000;
			companyCStockPriceChangeAmount = 000;
			companyDStockPriceChangeAmount = 1000;
			companyEStockPriceChangeAmount = -1000;
		}
		if (XX == 19) {
			wsprintf(S, TEXT("今年の夏は過去最高の猛暑になると予想"), XX);
			wsprintf(Sb, TEXT("\n\n\n\n\n\nイベント番号；%d"), XX);
			companyAStockPriceChangeAmount = 000;
			companyBStockPriceChangeAmount = 1000;
			companyCStockPriceChangeAmount = 2000;
			companyDStockPriceChangeAmount = 2000;
			companyEStockPriceChangeAmount = 1000;
		}
		if (XX == 20) {
			wsprintf(S, TEXT("北陸新幹線が開通"), XX);
			wsprintf(Sb, TEXT("\n\n\n\n\n\nイベント番号；%d"), XX);
			companyAStockPriceChangeAmount = 000;
			companyBStockPriceChangeAmount = 000;
			companyCStockPriceChangeAmount = 000;
			companyDStockPriceChangeAmount = 000;
			companyEStockPriceChangeAmount = 3000;
		}
		if (XX == 21) {
			wsprintf(S, TEXT("海外で日本食ブーム"), XX);
			wsprintf(Sb, TEXT("\n\n\n\n\n\nイベント番号；%d"), XX);
			companyAStockPriceChangeAmount = 000;
			companyBStockPriceChangeAmount = 000;
			companyCStockPriceChangeAmount = 1000;
			companyDStockPriceChangeAmount = 1000;
			companyEStockPriceChangeAmount = 2000;
		}
		if (XX == 22) {
			wsprintf(S, TEXT("日本を舞台にしたディズニー映画が大ヒット"), XX);
			wsprintf(Sb, TEXT("\n\n\n\n\n\nイベント番号；%d"), XX);
			companyAStockPriceChangeAmount = 1000;
			companyBStockPriceChangeAmount = 1000;
			companyCStockPriceChangeAmount = 1000;
			companyDStockPriceChangeAmount = 2000;
			companyEStockPriceChangeAmount = 2000;
		}
		if (XX == 23) {
			wsprintf(S, TEXT("ハワイでテロ"), XX);
			wsprintf(Sb, TEXT("\n\n\n\n\n\nイベント番号；%d"), XX);
			companyAStockPriceChangeAmount = -1000;
			companyBStockPriceChangeAmount = -1000;
			companyCStockPriceChangeAmount = -1000;
			companyDStockPriceChangeAmount = -1000;
			companyEStockPriceChangeAmount = -5000;
		}
		if (XX == 24) {
			wsprintf(S, TEXT("ドローンの新技術が開発"), XX);
			wsprintf(Sb, TEXT("\n\n\n\n\n\nイベント番号；%d"), XX);
			companyAStockPriceChangeAmount = 1000;
			companyBStockPriceChangeAmount = 1000;
			companyCStockPriceChangeAmount = 000;
			companyDStockPriceChangeAmount = 1000;
			companyEStockPriceChangeAmount = 000;
		}
		if (XX == 25) {
			wsprintf(S, TEXT("錦織圭がオリンピックで金メダル"), XX);
			wsprintf(Sb, TEXT("\n\n\n\n\n\nイベント番号；%d"), XX);
			companyAStockPriceChangeAmount = 000;
			companyBStockPriceChangeAmount = 5000;
			companyCStockPriceChangeAmount = 000;
			companyDStockPriceChangeAmount = 000;
			companyEStockPriceChangeAmount = 2000;
		}
		if (XX == 26) {
			wsprintf(S, TEXT("中国の軍艦と日本の漁船が衝突"), XX);
			wsprintf(Sb, TEXT("\n\n\n\n\n\nイベント番号；%d"), XX);
			companyAStockPriceChangeAmount = -3000;
			companyBStockPriceChangeAmount = -3000;
			companyCStockPriceChangeAmount = 000;
			companyDStockPriceChangeAmount = -1000;
			companyEStockPriceChangeAmount = -3000;
		}
		if (XX == 27) {
			wsprintf(S, TEXT("日本海で大量の石油が発見"), XX);
			wsprintf(Sb, TEXT("\n\n\n\n\n\nイベント番号；%d"), XX);
			companyAStockPriceChangeAmount = 2000;
			companyBStockPriceChangeAmount = 2000;
			companyCStockPriceChangeAmount = 1000;
			companyDStockPriceChangeAmount = 1000;
			companyEStockPriceChangeAmount = 2000;
		}
		if (XX == 28) {
			wsprintf(S, TEXT("富士山噴火の危険が高まる"), XX);
			wsprintf(Sb, TEXT("\n\n\n\n\n\nイベント番号；%d"), XX);
			companyAStockPriceChangeAmount = 000;
			companyBStockPriceChangeAmount = 000;
			companyCStockPriceChangeAmount = 000;
			companyDStockPriceChangeAmount = 000;
			companyEStockPriceChangeAmount = -3000;
		}
		if (XX == 29) {
			wsprintf(S, TEXT("中国で日本製品が大ブーム"), XX);
			wsprintf(Sb, TEXT("\n\n\n\n\n\nイベント番号；%d"), XX);
			companyAStockPriceChangeAmount = 5000;
			companyBStockPriceChangeAmount = 4000;
			companyCStockPriceChangeAmount = 3000;
			companyDStockPriceChangeAmount = 2000;
			companyEStockPriceChangeAmount = 3000;
		}
		if (XX == 30) {
			wsprintf(S, TEXT("中国人の爆買いが流行"), XX);
			wsprintf(Sb, TEXT("\n\n\n\n\n\nイベント番号；%d"), XX);
			companyAStockPriceChangeAmount = 000;
			companyBStockPriceChangeAmount = 1000;
			companyBStockPriceChangeAmount = -3000;
			companyCStockPriceChangeAmount = 1000;
			companyDStockPriceChangeAmount = 3000;
			companyEStockPriceChangeAmount = 5000;
		}
		if (XX == 31) {
			wsprintf(S, TEXT("首相が靖国神社参拝"), XX);
			wsprintf(Sb, TEXT("\n\n\n\n\n\nイベント番号；%d"), XX);
			companyAStockPriceChangeAmount = -4000;
			companyCStockPriceChangeAmount = -1000;
			companyDStockPriceChangeAmount = -1000;
			companyEStockPriceChangeAmount = -3000;
		}
		if (XX == 32) {
			wsprintf(S, TEXT("「石油が20年後に無くなる」と国連が発表"), XX);
			wsprintf(Sb, TEXT("\n\n\n\n\n\nイベント番号；%d"), XX);
			companyAStockPriceChangeAmount = 1000;
			companyBStockPriceChangeAmount = -2000;
			companyCStockPriceChangeAmount = -2000;
			companyDStockPriceChangeAmount = -3000;
			companyEStockPriceChangeAmount = -5000;
		}
		if (XX == 33) {
			wsprintf(S, TEXT("中国が歴史問題を理由に日中間の往来を制限"), XX);
			wsprintf(Sb, TEXT("\n\n\n\n\n\nイベント番号；%d"), XX);
			companyAStockPriceChangeAmount = -4000;
			companyBStockPriceChangeAmount = -4000;
			companyCStockPriceChangeAmount = -1000;
			companyDStockPriceChangeAmount = -2000;
			companyEStockPriceChangeAmount = -6000;
		}
		if (XX == 34) {
			wsprintf(S, TEXT("日本で中華料理が大ブーム"), XX);
			wsprintf(Sb, TEXT("\n\n\n\n\n\nイベント番号；%d"), XX);
			companyAStockPriceChangeAmount = 1000;
			companyBStockPriceChangeAmount = 1000;
			companyCStockPriceChangeAmount = 000;
			companyDStockPriceChangeAmount = 2000;
			companyEStockPriceChangeAmount = 3000;
		}
		if (XX == 35) {
			wsprintf(S, TEXT("イタリアで大地震が発生"), XX);
			wsprintf(Sb, TEXT("\n\n\n\n\n\nイベント番号；%d"), XX);
			companyAStockPriceChangeAmount = 000;
			companyBStockPriceChangeAmount = 000;
			companyCStockPriceChangeAmount = 000;
			companyDStockPriceChangeAmount = 000;
			companyEStockPriceChangeAmount = -3000;
		}
		if (XX == 36) {
			wsprintf(S, TEXT("消費税が15％に増税"), XX);
			wsprintf(Sb, TEXT("\n\n\n\n\n\nイベント番号；%d"), XX);
			companyAStockPriceChangeAmount = -3000;
			companyBStockPriceChangeAmount = -2000;
			companyCStockPriceChangeAmount = -2000;
			companyDStockPriceChangeAmount = -2000;
			companyEStockPriceChangeAmount = -3000;
		}
		if (XX == 37) {
			wsprintf(S, TEXT("韓国で新型インフルエンザが大流行"), XX);
			wsprintf(Sb, TEXT("\n\n\n\n\n\nイベント番号；%d"), XX);
			companyAStockPriceChangeAmount = -1000;
			companyBStockPriceChangeAmount = -1000;
			companyCStockPriceChangeAmount = 000;
			companyDStockPriceChangeAmount = 1000;
			companyEStockPriceChangeAmount = 2000;
		}
		if (XX == 38) {
			wsprintf(S, TEXT("本州で大規模な停電"), XX);
			wsprintf(Sb, TEXT("\n\n\n\n\n\nイベント番号；%d"), XX);
			companyAStockPriceChangeAmount = -3000;
			companyBStockPriceChangeAmount = -2000;
			companyCStockPriceChangeAmount = -3000;
			companyDStockPriceChangeAmount = -3000;
			companyEStockPriceChangeAmount = -3000;
		}
		if (XX == 39) {
			wsprintf(S, TEXT("サッカーワールドカップが大阪で開催"), XX);
			wsprintf(Sb, TEXT("\n\n\n\n\n\nイベント番号；%d"), XX);
			companyAStockPriceChangeAmount = 000;
			companyBStockPriceChangeAmount = 1000;
			companyCStockPriceChangeAmount = 000;
			companyDStockPriceChangeAmount = 1000;
			companyEStockPriceChangeAmount = 4000;
		}
		if (XX == 40) {
			wsprintf(S, TEXT("広島で土砂災害"), XX);
			wsprintf(Sb, TEXT("\n\n\n\n\n\nイベント番号；%d"), XX);
			companyAStockPriceChangeAmount = -1000;
			companyBStockPriceChangeAmount = -1000;
			companyCStockPriceChangeAmount = -1000;
			companyDStockPriceChangeAmount = -1000;
			companyEStockPriceChangeAmount = -1000;
		}
		if (XX == 41) {
			wsprintf(S, TEXT("エボラ出血熱がヨーロッパで大流行"), XX);
			wsprintf(Sb, TEXT("\n\n\n\n\n\nイベント番号；%d"), XX);
			companyAStockPriceChangeAmount = -5000;
			companyBStockPriceChangeAmount = -2000;
			companyCStockPriceChangeAmount = -2000;
			companyDStockPriceChangeAmount = -2000;
			companyEStockPriceChangeAmount = -7000;
		}
		if (XX == 42) {
			wsprintf(S, TEXT("高速道路料金が無料に"), XX);
			wsprintf(Sb, TEXT("\n\n\n\n\n\nイベント番号；%d"), XX);
			companyAStockPriceChangeAmount = 2000;
			companyBStockPriceChangeAmount = 1000;
			companyCStockPriceChangeAmount = 1000;
			companyDStockPriceChangeAmount = 1000;
			companyEStockPriceChangeAmount = 2000;
		}
		if (XX == 43) {
			wsprintf(S, TEXT("新幹線の事故が相次ぐ"), XX);
			wsprintf(Sb, TEXT("\n\n\n\n\n\nイベント番号；%d"), XX);
			companyAStockPriceChangeAmount = 1000;
			companyBStockPriceChangeAmount = 000;
			companyCStockPriceChangeAmount = 000;
			companyDStockPriceChangeAmount = 000;
			companyEStockPriceChangeAmount = -2000;
		}
		if (XX == 44) {
			wsprintf(S, TEXT("北朝鮮のミサイルが神奈川県沖に落下"), XX);
			wsprintf(Sb, TEXT("\n\n\n\n\n\nイベント番号；%d"), XX);
			companyAStockPriceChangeAmount = -3000;
			companyBStockPriceChangeAmount = -3000;
			companyCStockPriceChangeAmount = -3000;
			companyDStockPriceChangeAmount = -3000;
			companyEStockPriceChangeAmount = -3000;
		}
		if (XX == 45) {
			wsprintf(S, TEXT("上野動物園でパンダの赤ちゃんが生まれる"), XX);
			wsprintf(Sb, TEXT("\n\n\n\n\n\nイベント番号；%d"), XX);
			companyAStockPriceChangeAmount = 1000;
			companyBStockPriceChangeAmount = 1000;
			companyCStockPriceChangeAmount = 000;
			companyDStockPriceChangeAmount = 000;
			companyEStockPriceChangeAmount = 2000;
		}
		if (XX == 46) {
			wsprintf(S, TEXT("富岡製糸場が世界遺産に"), XX);
			wsprintf(Sb, TEXT("\n\n\n\n\n\nイベント番号；%d"), XX);
			companyAStockPriceChangeAmount = 000;
			companyBStockPriceChangeAmount = 1000;
			companyCStockPriceChangeAmount = 000;
			companyDStockPriceChangeAmount = 000;
			companyEStockPriceChangeAmount = 3000;
		}
		if (XX == 47) {
			wsprintf(S, TEXT("リニアモーターカーが開通"), XX);
			wsprintf(Sb, TEXT("\n\n\n\n\n\nイベント番号；%d"), XX);
			companyAStockPriceChangeAmount = 000;
			companyBStockPriceChangeAmount = 000;
			companyCStockPriceChangeAmount = 000;
			companyDStockPriceChangeAmount = 1000;
			companyEStockPriceChangeAmount = 4000;
		}
		if (XX == 48) {
			wsprintf(S, TEXT("成田国際空港の国際線が2倍に増える"), XX);
			wsprintf(Sb, TEXT("\n\n\n\n\n\nイベント番号；%d"), XX);
			companyAStockPriceChangeAmount = 000;
			companyBStockPriceChangeAmount = 000;
			companyCStockPriceChangeAmount = 000;
			companyDStockPriceChangeAmount = 000;
			companyEStockPriceChangeAmount = 5000;
		}
		if (XX == 49) {
			wsprintf(S, TEXT("中東で大規模な反政府デモ"), XX);
			wsprintf(Sb, TEXT("\n\n\n\n\n\nイベント番号；%d"), XX);
			companyAStockPriceChangeAmount = -2000;
			companyBStockPriceChangeAmount = -2000;
			companyCStockPriceChangeAmount = -1000;
			companyDStockPriceChangeAmount = -1000;
			companyEStockPriceChangeAmount = -2000;
		}
		if (XX == 50) {
			wsprintf(S, TEXT("インドの道路が整備される"), XX);
			wsprintf(Sb, TEXT("\n\n\n\n\n\nイベント番号；%d"), XX);
			companyAStockPriceChangeAmount = 2000;
			companyBStockPriceChangeAmount = 2000;
			companyCStockPriceChangeAmount = 000;
			companyDStockPriceChangeAmount = 000;
			companyEStockPriceChangeAmount = 2000;
		}
		if (XX == 51) {
			wsprintf(S, TEXT("アメリカが「30年でCO2排出量を半分にする」と発表"), XX);
			wsprintf(Sb, TEXT("\n\n\n\n\n\nイベント番号；%d"), XX);
			companyAStockPriceChangeAmount = 7000;
			companyBStockPriceChangeAmount = 000;
			companyCStockPriceChangeAmount = 000;
			companyDStockPriceChangeAmount = 000;
			companyEStockPriceChangeAmount = 000;
		}
		if (XX == 52) {
			wsprintf(S, TEXT("野菜の価格が昨年の2倍に"), XX);
			wsprintf(Sb, TEXT("\n\n\n\n\n\nイベント番号；%d"), XX);
			companyAStockPriceChangeAmount = 000;
			companyBStockPriceChangeAmount = 000;
			companyCStockPriceChangeAmount = -1000;
			companyDStockPriceChangeAmount = -2000;
			companyEStockPriceChangeAmount = -1000;
		}
		if (XX == 53) {
			wsprintf(S, TEXT("デング熱の大流行により東南アジアへの渡航が制限"), XX);
			wsprintf(Sb, TEXT("\n\n\n\n\n\nイベント番号；%d"), XX);
			companyAStockPriceChangeAmount = -3000;
			companyBStockPriceChangeAmount = -3000;
			companyCStockPriceChangeAmount = -1000;
			companyDStockPriceChangeAmount = -1000;
			companyEStockPriceChangeAmount = -3000;
		}
		if (XX == 54) {
			wsprintf(S, TEXT("ソマリアの海賊が消滅"), XX);
			wsprintf(Sb, TEXT("\n\n\n\n\n\nイベント番号；%d"), XX);
			companyAStockPriceChangeAmount = 2000;
			companyBStockPriceChangeAmount = 2000;
			companyCStockPriceChangeAmount = 1000;
			companyDStockPriceChangeAmount = 1000;
			companyEStockPriceChangeAmount = 1000;
		}
		if (XX == 55) {
			wsprintf(S, TEXT("日本の人口が増加"), XX);
			wsprintf(Sb, TEXT("\n\n\n\n\n\nイベント番号；%d"), XX);
			companyAStockPriceChangeAmount = 5000;
			companyBStockPriceChangeAmount = 4000;
			companyCStockPriceChangeAmount = 4000;
			companyDStockPriceChangeAmount = 3000;
			companyEStockPriceChangeAmount = 4000;
		}
		if (XX == 56) {
			wsprintf(S, TEXT("札幌オリンピック開催が決定"), XX);
			wsprintf(Sb, TEXT("\n\n\n\n\n\nイベント番号；%d"), XX);
			companyAStockPriceChangeAmount = 4000;
			companyBStockPriceChangeAmount = 3000;
			companyCStockPriceChangeAmount = 2000;
			companyDStockPriceChangeAmount = 2000;
			companyEStockPriceChangeAmount = 4000;
		}
		if (XX == 57) {
			wsprintf(S, TEXT("東京スカイツリーが事故により営業を停止"), XX);
			wsprintf(Sb, TEXT("\n\n\n\n\n\nイベント番号；%d"), XX);
			companyAStockPriceChangeAmount = 000;
			companyBStockPriceChangeAmount = 000;
			companyCStockPriceChangeAmount = 000;
			companyDStockPriceChangeAmount = -1000;
			companyEStockPriceChangeAmount = -2000;
		}
		if (XX == 58) {
			wsprintf(S, TEXT("ガソリン税が廃止"), XX);
			wsprintf(Sb, TEXT("\n\n\n\n\n\nイベント番号；%d"), XX);
			companyAStockPriceChangeAmount = 4000;
			companyBStockPriceChangeAmount = 1000;
			companyCStockPriceChangeAmount = 1000;
			companyDStockPriceChangeAmount = 1000;
			companyEStockPriceChangeAmount = 4000;
		}
		if (XX == 59) {
			wsprintf(S, TEXT("ディズニーランドにリニューアルオープン"), XX);
			wsprintf(Sb, TEXT("\n\n\n\n\n\nイベント番号；%d"), XX);
			companyAStockPriceChangeAmount = 000;
			companyBStockPriceChangeAmount = 1000;
			companyCStockPriceChangeAmount = 0000;
			companyDStockPriceChangeAmount = 1000;
			companyEStockPriceChangeAmount = 3000;
		}
		if (XX == 60) {
			wsprintf(S, TEXT("石油の新しい採掘方法が開発"), XX);
			wsprintf(Sb, TEXT("\n\n\n\n\n\nイベント番号；%d"), XX);
			companyAStockPriceChangeAmount = 3000;
			companyBStockPriceChangeAmount = 1000;
			companyCStockPriceChangeAmount = 1000;
			companyDStockPriceChangeAmount = 1000;
			companyEStockPriceChangeAmount = 3000;
		}
		if (XX == 61) {
			wsprintf(S, TEXT("燃費不正問題でヨーロッパの大手自動車メーカーが倒産"), XX);
			wsprintf(Sb, TEXT("\n\n\n\n\n\nイベント番号；%d"), XX);
			companyAStockPriceChangeAmount = 5000;
			companyBStockPriceChangeAmount = 000;
			companyCStockPriceChangeAmount = 000;
			companyDStockPriceChangeAmount = 000;
			companyEStockPriceChangeAmount = 000;
		}
		if (XX == 62) {
			wsprintf(S, TEXT("アメリカの大統領が広島訪問"), XX);
			wsprintf(Sb, TEXT("\n\n\n\n\n\nイベント番号；%d"), XX);
			companyAStockPriceChangeAmount = 000;
			companyBStockPriceChangeAmount = 000;
			companyCStockPriceChangeAmount = 000;
			companyDStockPriceChangeAmount = 000;
			companyEStockPriceChangeAmount = 2000;
		}
		if (XX == 63) {
			wsprintf(S, TEXT("コーヒー豆の値段が下がる"), XX);
			wsprintf(Sb, TEXT("\n\n\n\n\n\nイベント番号；%d"), XX);
			companyAStockPriceChangeAmount = 000;
			companyBStockPriceChangeAmount = 000;
			companyCStockPriceChangeAmount = 2000;
			companyDStockPriceChangeAmount = 2000;
			companyEStockPriceChangeAmount = 000;
		}
		if (XX == 64) {
			wsprintf(S, TEXT("国連が「排気ガスを40年間で50％削減する」と発表"), XX);
			wsprintf(Sb, TEXT("\n\n\n\n\n\nイベント番号；%d"), XX);
			companyAStockPriceChangeAmount = 6000;
			companyBStockPriceChangeAmount = 000;
			companyCStockPriceChangeAmount = 000;
			companyDStockPriceChangeAmount = 000;
			companyEStockPriceChangeAmount = 000;
		}
		if (XX == 65) {
			wsprintf(S, TEXT("アメリカで忍者ブーム"), XX);
			wsprintf(Sb, TEXT("\n\n\n\n\n\nイベント番号；%d"), XX);
			companyAStockPriceChangeAmount = 000;
			companyBStockPriceChangeAmount = 000;
			companyCStockPriceChangeAmount = 000;
			companyDStockPriceChangeAmount = 000;
			companyEStockPriceChangeAmount = 2000;
		}
		if (XX == 66) {
			wsprintf(S, TEXT("中国で大規模な反日デモ"), XX);
			wsprintf(Sb, TEXT("\n\n\n\n\n\nイベント番号；%d"), XX);
			companyAStockPriceChangeAmount = -2000;
			companyBStockPriceChangeAmount = -3000;
			companyCStockPriceChangeAmount = 000;
			companyDStockPriceChangeAmount = 000;
			companyEStockPriceChangeAmount = -3000;
		}
		if (XX == 67) {
			wsprintf(S, TEXT("ヨーロッパで日本のファッションが注目"), XX);
			wsprintf(Sb, TEXT("\n\n\n\n\n\nイベント番号；%d"), XX);
			companyAStockPriceChangeAmount = 1000;
			companyBStockPriceChangeAmount = 5000;
			companyCStockPriceChangeAmount = 1000;
			companyDStockPriceChangeAmount = 2000;
			companyEStockPriceChangeAmount = 2000;
		}
		if (XX == 68) {
			wsprintf(S, TEXT("新国立競技場が完成"), XX);
			wsprintf(Sb, TEXT("\n\n\n\n\n\nイベント番号；%d"), XX);
			companyAStockPriceChangeAmount = 5000;
			companyBStockPriceChangeAmount = 5000;
			companyCStockPriceChangeAmount = 5000;
			companyDStockPriceChangeAmount = 5000;
			companyEStockPriceChangeAmount = 5000;
		}
		if (XX == 69) {
			wsprintf(S, TEXT("政府が「50年間で日本の車の半分をエコカーにする」と発表"), XX);
			wsprintf(Sb, TEXT("\n\n\n\n\n\nイベント番号；%d"), XX);
			companyAStockPriceChangeAmount = 7000;
			companyBStockPriceChangeAmount = 000;
			companyCStockPriceChangeAmount = 000;
			companyDStockPriceChangeAmount = 000;
			companyEStockPriceChangeAmount = 000;
		}
		if (XX == 70) {
			wsprintf(S, TEXT("東南アジアで低価格の洋服が流行"), XX);
			wsprintf(Sb, TEXT("\n\n\n\n\n\nイベント番号；%d"), XX);
			companyAStockPriceChangeAmount = 000;
			companyBStockPriceChangeAmount = 4000;
			companyCStockPriceChangeAmount = 000;
			companyDStockPriceChangeAmount = 1000;
			companyEStockPriceChangeAmount = 000;
		}
		if (XX == 71) {
			wsprintf(S, TEXT("「チョコレートががん予防になる」という研究結果が発表"), XX);
			wsprintf(Sb, TEXT("\n\n\n\n\n\nイベント番号；%d"), XX);
			companyAStockPriceChangeAmount = 000;
			companyBStockPriceChangeAmount = 000;
			companyCStockPriceChangeAmount = 3000;
			companyDStockPriceChangeAmount = 2000;
			companyEStockPriceChangeAmount = 000;
		}
		if (XX == 72) {
			wsprintf(S, TEXT("アメリカでとんこつラーメンが大ヒット"), XX);
			wsprintf(Sb, TEXT("\n\n\n\n\n\nイベント番号；%d"), XX);
			companyAStockPriceChangeAmount = 000;
			companyBStockPriceChangeAmount = 000;
			companyCStockPriceChangeAmount = 000;
			companyDStockPriceChangeAmount = 2000;
			companyEStockPriceChangeAmount = 2000;
		}
		if (XX == 73) {
			wsprintf(S, TEXT("冷凍食品による食中毒が多発"), XX);
			wsprintf(Sb, TEXT("\n\n\n\n\n\nイベント番号；%d"), XX);
			companyAStockPriceChangeAmount = 000;
			companyBStockPriceChangeAmount = 000;
			companyCStockPriceChangeAmount = 000;
			companyDStockPriceChangeAmount = -3000;
			companyEStockPriceChangeAmount = 000;
		}
		if (XX == 74) {
			wsprintf(S, TEXT("スターウォーズの新作が公開"), XX);
			wsprintf(Sb, TEXT("\n\n\n\n\n\nイベント番号；%d"), XX);
			companyAStockPriceChangeAmount = 000;
			companyBStockPriceChangeAmount = 1000;
			companyCStockPriceChangeAmount = 000;
			companyDStockPriceChangeAmount = 1000;
			companyEStockPriceChangeAmount = 1000;
		}
		if (XX == 75) {
			wsprintf(S, TEXT("大手企業が会員の個人情報を流出"), XX);
			wsprintf(Sb, TEXT("\n\n\n\n\n\nイベント番号；%d"), XX);
			companyAStockPriceChangeAmount = -2000;
			companyBStockPriceChangeAmount = -1000;
			companyCStockPriceChangeAmount = -1000;
			companyDStockPriceChangeAmount = -2000;
			companyEStockPriceChangeAmount = -2000;
		}
		if (XX == 76) {
			wsprintf(S, TEXT("日本の化粧品の新商品がアジアでヒット"), XX);
			wsprintf(Sb, TEXT("\n\n\n\n\n\nイベント番号；%d"), XX);
			companyAStockPriceChangeAmount = 000;
			companyBStockPriceChangeAmount = 000;
			companyCStockPriceChangeAmount = 000;
			companyDStockPriceChangeAmount = 2000;
			companyEStockPriceChangeAmount = 2000;
		}
		if (XX == 77) {
			wsprintf(S, TEXT("サウジアラビアで自爆テロが多発"), XX);
			wsprintf(Sb, TEXT("\n\n\n\n\n\nイベント番号；%d"), XX);
			companyAStockPriceChangeAmount = -1000;
			companyBStockPriceChangeAmount = -2000;
			companyCStockPriceChangeAmount = -1000;
			companyDStockPriceChangeAmount = -1000;
			companyEStockPriceChangeAmount = -2000;
		}
		if (XX == 78) {
			wsprintf(S, TEXT("中国の人件費が上がる"), XX);
			wsprintf(Sb, TEXT("\n\n\n\n\n\nイベント番号；%d"), XX);
			companyAStockPriceChangeAmount = -2000;
			companyBStockPriceChangeAmount = -3000;
			companyCStockPriceChangeAmount = -1000;
			companyDStockPriceChangeAmount = -1000;
			companyEStockPriceChangeAmount = -1000;
		}
		if (XX == 79) {
			wsprintf(S, TEXT("駄菓子メーカーの賞味期限偽装が発覚"), XX);
			wsprintf(Sb, TEXT("\n\n\n\n\n\nイベント番号；%d"), XX);
			companyAStockPriceChangeAmount = 000;
			companyBStockPriceChangeAmount = 000;
			companyCStockPriceChangeAmount = -5000;
			companyDStockPriceChangeAmount = -1000;
			companyEStockPriceChangeAmount = 000;
		}
		if (XX == 80) {
			wsprintf(S, TEXT("日本のオリンピック選手が国民栄誉賞"), XX);
			wsprintf(Sb, TEXT("\n\n\n\n\n\nイベント番号；%d"), XX);
			companyAStockPriceChangeAmount = 1000;
			companyBStockPriceChangeAmount = 1000;
			companyCStockPriceChangeAmount = 1000;
			companyDStockPriceChangeAmount = 1000;
			companyEStockPriceChangeAmount = 1000;
		}
		if (XX == 81) {
			wsprintf(S, TEXT("人工知能の開発をする企業への補助金制度が決定"), XX);
			wsprintf(Sb, TEXT("\n\n\n\n\n\nイベント番号；%d"), XX);
			companyAStockPriceChangeAmount = 4000;
			companyBStockPriceChangeAmount = 000;
			companyCStockPriceChangeAmount = 000;
			companyDStockPriceChangeAmount = 1000;
			companyEStockPriceChangeAmount = 1000;
		}
		if (XX == 82) {
			wsprintf(S, TEXT("アメリカ政府が海外からの輸入を大幅に制限"), XX);
			wsprintf(Sb, TEXT("\n\n\n\n\n\nイベント番号；%d"), XX);
			companyAStockPriceChangeAmount = -7000;
			companyBStockPriceChangeAmount = -1000;
			companyCStockPriceChangeAmount = -2000;
			companyDStockPriceChangeAmount = -1000;
			companyEStockPriceChangeAmount = 000;
		}
		if (XX == 83) {
			wsprintf(S, TEXT("ガソリンが大幅に値上げ"), XX);
			wsprintf(Sb, TEXT("\n\n\n\n\n\nイベント番号；%d"), XX);
			companyAStockPriceChangeAmount = -3000;
			companyBStockPriceChangeAmount = -2000;
			companyCStockPriceChangeAmount = -2000;
			companyDStockPriceChangeAmount = -3000;
			companyEStockPriceChangeAmount = -3000;
		}
		if (XX == 84) {
			wsprintf(S, TEXT("東京に乗り物博物館がオープン"), XX);
			wsprintf(Sb, TEXT("\n\n\n\n\n\nイベント番号；%d"), XX);
			companyAStockPriceChangeAmount = 1000;
			companyBStockPriceChangeAmount = 000;
			companyCStockPriceChangeAmount = 000;
			companyDStockPriceChangeAmount = 000;
			companyEStockPriceChangeAmount = 1000;
		}
		if (XX == 85) {
			wsprintf(S, TEXT("東京でピカソ展が開催"), XX);
			wsprintf(Sb, TEXT("\n\n\n\n\n\nイベント番号；%d"), XX);
			companyAStockPriceChangeAmount = 000;
			companyBStockPriceChangeAmount = 000;
			companyCStockPriceChangeAmount = 000;
			companyDStockPriceChangeAmount = 1000;
			companyEStockPriceChangeAmount = 2000;
		}
		if (XX == 86) {
			wsprintf(S, TEXT("安全上の問題からドローンの使用が禁止に"), XX);
			wsprintf(Sb, TEXT("\n\n\n\n\n\nイベント番号；%d"), XX);
			companyAStockPriceChangeAmount = -1000;
			companyBStockPriceChangeAmount = 000;
			companyCStockPriceChangeAmount = 000;
			companyDStockPriceChangeAmount = -1000;
			companyEStockPriceChangeAmount = 000;
		}
		if (XX == 87) {
			wsprintf(S, TEXT("大雪で成田空港の飛行機が1日中欠航"), XX);
			wsprintf(Sb, TEXT("\n\n\n\n\n\nイベント番号；%d"), XX);
			companyAStockPriceChangeAmount = 000;
			companyBStockPriceChangeAmount = 000;
			companyCStockPriceChangeAmount = 000;
			companyDStockPriceChangeAmount = 000;
			companyEStockPriceChangeAmount = -4000;
		}
		if (XX == 88) {
			wsprintf(S, TEXT("箱根で火山が噴火"), XX);
			wsprintf(Sb, TEXT("\n\n\n\n\n\nイベント番号；%d"), XX);
			companyAStockPriceChangeAmount = 000;
			companyBStockPriceChangeAmount = 000;
			companyCStockPriceChangeAmount = 000;
			companyDStockPriceChangeAmount = 000;
			companyEStockPriceChangeAmount = -4000;
		}
		if (XX == 89) {
			wsprintf(S, TEXT("韓国の大統領が竹島に上陸"), XX);
			wsprintf(Sb, TEXT("\n\n\n\n\n\nイベント番号；%d"), XX);
			companyAStockPriceChangeAmount = -1000;
			companyBStockPriceChangeAmount = -1000;
			companyCStockPriceChangeAmount = 000;
			companyDStockPriceChangeAmount = 000;
			companyEStockPriceChangeAmount = -3000;
		}
		if (XX == 90) {
			wsprintf(S, TEXT("北海道新幹線が開通"), XX);
			wsprintf(Sb, TEXT("\n\n\n\n\n\nイベント番号；%d"), XX);
			companyAStockPriceChangeAmount = 000;
			companyBStockPriceChangeAmount = 000;
			companyCStockPriceChangeAmount = 000;
			companyDStockPriceChangeAmount = 000;
			companyEStockPriceChangeAmount = 3000;
		}
		if (XX == 91) {
			wsprintf(S, TEXT("マグロが例年の2倍発生"), XX);
			wsprintf(Sb, TEXT("\n\n\n\n\n\nイベント番号；%d"), XX);
			companyAStockPriceChangeAmount = 000;
			companyBStockPriceChangeAmount = 000;
			companyCStockPriceChangeAmount = 000;
			companyDStockPriceChangeAmount = 1000;
			companyEStockPriceChangeAmount = 2000;
		}
		if (XX == 92) {
			wsprintf(S, TEXT("フィリピンで軍事クーデターが発生"), XX);
			wsprintf(Sb, TEXT("\n\n\n\n\n\nイベント番号；%d"), XX);
			companyAStockPriceChangeAmount = -1000;
			companyBStockPriceChangeAmount = -2000;
			companyCStockPriceChangeAmount = -1000;
			companyDStockPriceChangeAmount = -1000;
			companyEStockPriceChangeAmount = -1000;
		}
		if (XX == 93) {
			wsprintf(S, TEXT("養殖によるウナギの大量生産が始まる"), XX);
			wsprintf(Sb, TEXT("\n\n\n\n\n\nイベント番号；%d"), XX);
			companyAStockPriceChangeAmount = 000;
			companyBStockPriceChangeAmount = 000;
			companyCStockPriceChangeAmount = 000;
			companyDStockPriceChangeAmount = 1000;
			companyEStockPriceChangeAmount = 2000;
		}
		if (XX == 94) {
			wsprintf(S, TEXT("名古屋港が2倍に広がる"), XX);
			wsprintf(Sb, TEXT("\n\n\n\n\n\nイベント番号；%d"), XX);
			companyAStockPriceChangeAmount = 6000;
			companyBStockPriceChangeAmount = 000;
			companyCStockPriceChangeAmount = 000;
			companyDStockPriceChangeAmount = 000;
			companyEStockPriceChangeAmount = 000;
		}
		if (XX == 95) {
			wsprintf(S, TEXT("明日はバレンタインデーだ"), XX);
			wsprintf(Sb, TEXT("\n\n\n\n\n\nイベント番号；%d"), XX);
			companyAStockPriceChangeAmount = 000;
			companyBStockPriceChangeAmount = 000;
			companyCStockPriceChangeAmount = 3000;
			companyDStockPriceChangeAmount = 2000;
			companyEStockPriceChangeAmount = 000;
		}
		if (XX == 96) {
			wsprintf(S, TEXT("牛乳の価格が下がる"), XX);
			wsprintf(Sb, TEXT("\n\n\n\n\n\nイベント番号；%d"), XX);
			companyAStockPriceChangeAmount = 000;
			companyBStockPriceChangeAmount = 000;
			companyCStockPriceChangeAmount = 1000;
			companyDStockPriceChangeAmount = 1000;
			companyEStockPriceChangeAmount = 000;
		}
		if (XX == 97) {
			wsprintf(S, TEXT("牛乳の価格が上がる"), XX);
			wsprintf(Sb, TEXT("\n\n\n\n\n\nイベント番号；%d"), XX);
			companyAStockPriceChangeAmount = 000;
			companyBStockPriceChangeAmount = 000;
			companyCStockPriceChangeAmount = -1000;
			companyDStockPriceChangeAmount = -1000;
			companyEStockPriceChangeAmount = 000;
		}
		if (XX == 98) {
			wsprintf(S, TEXT("本州と九州を結ぶ新しい橋が完成"), XX);
			wsprintf(Sb, TEXT("\n\n\n\n\n\nイベント番号；%d"), XX);
			companyAStockPriceChangeAmount = 000;
			companyBStockPriceChangeAmount = 000;
			companyCStockPriceChangeAmount = 1000;
			companyDStockPriceChangeAmount = 1000;
			companyEStockPriceChangeAmount = 3000;
		}
		NE = XX;

		if (XX == 1000) {
			wsprintf(S, TEXT("最後の株価を表示しています。"), XX);
			wsprintf(Sb, TEXT("\n\n\n\n\n\nイベント番号；%d"), XX);
			//KillTimer(hWnd,ID_MYTIMER2);
		}

	}



	return;
}

void EData(int XX) {
	if (EV < his + 3) {
		companyAStockPriceChangeAmount = (rand() % 7 - 3) * 700;
		companyBStockPriceChangeAmount = (rand() % 7 - 3) * 700;
		companyCStockPriceChangeAmount = (rand() % 7 - 3) * 700;
		companyDStockPriceChangeAmount = (rand() % 7 - 3) * 700;
		companyEStockPriceChangeAmount = (rand() % 7 - 3) * 700;


	}
	else {
		companyAStockPriceChangeAmount = 0;
		companyBStockPriceChangeAmount = 0;
		companyDStockPriceChangeAmount = 0;
		if (XX == 0) {
			wsprintf(S, TEXT("スーパーハイブリッド車が開発された！"), XX);
			wsprintf(Sb, TEXT("\n\n\n\n\n\nイベント番号；%d"), XX);
			companyAStockPriceChangeAmount = 2000;
		}
		if (XX == 1) {
			wsprintf(S, TEXT("汗をかくと冷たくなる「スーパークールシャツ」が開発された！"), XX);
			wsprintf(Sb, TEXT("\n\n\n\n\n\nイベント番号；%d"), XX);
			companyDStockPriceChangeAmount = 2000;

		}
		if (XX == 2) {
			wsprintf(S, TEXT("すべての高層ビルに震度7に耐える耐震工事を義務付ける法律ができた！"), XX);
			wsprintf(Sb, TEXT("\n\n\n\n\n\nイベント番号；%d"), XX);
			companyBStockPriceChangeAmount = 2000;
		}
		if (XX == 3) {
			wsprintf(S, TEXT("東南海大地震が起きて名古屋中心の沿岸部が壊滅的な被害を受けた！"), XX);
			wsprintf(Sb, TEXT("\n\n\n\n\n\nイベント番号；%d"), XX);
			companyAStockPriceChangeAmount = -4000;
			companyBStockPriceChangeAmount = 1000;
			companyDStockPriceChangeAmount = 1000;
		}
		if (XX == 4) {
			wsprintf(S, TEXT("太陽光で動くソーラー自動車が発売された！"), XX);
			wsprintf(Sb, TEXT("\n\n\n\n\n\nイベント番号；%d"), XX);
			companyAStockPriceChangeAmount = 2000;
		}
		if (XX == 5) {
			wsprintf(S, TEXT("世界的に景気が良くなった！"), XX);
			wsprintf(Sb, TEXT("\n\n\n\n\n\nイベント番号；%d"), XX);
			companyAStockPriceChangeAmount = 3000;
			companyBStockPriceChangeAmount = 3000;
			companyDStockPriceChangeAmount = -1000;
		}
		if (XX == 6) {
			wsprintf(S, TEXT("円安になった！"), XX);
			wsprintf(Sb, TEXT("\n\n\n\n\n\nイベント番号；%d"), XX);
			companyAStockPriceChangeAmount = 3000;
			companyDStockPriceChangeAmount = -2000;
		}
		if (XX == 7) {
			wsprintf(S, TEXT("中国と日本の関係が悪化し、一切の貿易が停止した！"), XX);
			wsprintf(Sb, TEXT("\n\n\n\n\n\nイベント番号；%d"), XX);
			companyAStockPriceChangeAmount = -1000;
			companyBStockPriceChangeAmount = -2000;
			companyDStockPriceChangeAmount = -8000;
		}
		if (XX == 8) {
			wsprintf(S, TEXT("所得税が上がった！"), XX);
			wsprintf(Sb, TEXT("\n\n\n\n\n\nイベント番号；%d"), XX);
			companyAStockPriceChangeAmount = -1000;
			companyBStockPriceChangeAmount = -1000;
		}
		if (XX == 9) {
			wsprintf(S, TEXT("急激な円高になった！"), XX);
			wsprintf(Sb, TEXT("\n\n\n\n\n\nイベント番号；%d"), XX);
			companyAStockPriceChangeAmount = -3000;
			companyBStockPriceChangeAmount = 0;
			companyDStockPriceChangeAmount = 2000;
		}
		if (XX == 1000) {
			wsprintf(S, TEXT("最後の株価を表示しています。"), XX);
			wsprintf(Sb, TEXT("\n\n\n\n\n\nイベント番号；%d"), XX);
			KillTimer(hWnd, ID_MYTIMER2);
		}
	}
	return;
}

void Ftime(int companyAStockPriceChangeAmount, HWND hWnd, RECT clientRectangle, HDC hdc) {

	HPEN hPen, hPen2, hPen3;
	HBRUSH hBrush, hBrushW, hBrushT;
	RECT rc0, rc1, rc2, rc3, rc4, rc6;
	TCHAR szPRL[5][10];
	int o, i, q;

	hPen = CreatePen(PS_SOLID, 0, RGB(0, 0, 0));
	hBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
	hBrushW = (HBRUSH)GetStockObject(WHITE_BRUSH);
	hBrushT = (HBRUSH)GetStockObject(DC_BRUSH);
	hPen3 = (HPEN)GetStockObject(NULL_PEN);
	hPen2 = CreatePen(PS_SOLID, 4, RGB(0, 0, 0));
	int Tr = 0, Tg = 0, Tb = 0;

	if (companyAStockPriceChangeAmount != 6) {
		SelectObject(hdc, hBrush);

		if (companyAStockPriceChangeAmount == 1) {
			rc0.left = 0;
			rc0.top = clientRectangle.bottom / 16;
			rc0.right = clientRectangle.right / number;
			rc0.bottom = clientRectangle.bottom / 9;
			rc1.left = rc0.right;
			rc1.top = rc0.top;
			rc1.right = clientRectangle.right / number * 2;
			rc1.bottom = rc0.bottom;
			rc2.top = 2;
			rc2.left = 2;
			rc2.bottom = rc0.top - clientRectangle.bottom / 80;
			rc2.right = rc1.right;
			rc3.top = rc0.bottom + clientRectangle.bottom / 40;
			rc3.left = rc0.left + clientRectangle.right / 40;
			rc3.bottom = clientRectangle.bottom / 2 - clientRectangle.bottom / 40;
			rc3.right = rc1.right - clientRectangle.right / 40;
			wsprintf((LPWSTR)szPRL[2], TEXT("トヨタ自動車"));
			Tr = 204;
		}
		if (companyAStockPriceChangeAmount == 2) {

			rc0.left = clientRectangle.right * 2 / 3;
			rc0.top = clientRectangle.bottom / 16;
			rc0.right = clientRectangle.right * 2 / 3 + clientRectangle.right / number;
			rc0.bottom = clientRectangle.bottom / 9;
			rc1.left = clientRectangle.right * 2 / 3 + clientRectangle.right / number;
			rc1.top = clientRectangle.bottom / 16;
			rc1.right = clientRectangle.right * 2 / 3 + clientRectangle.right / number * 2;
			rc1.bottom = rc0.bottom;
			rc2.top = 2;
			rc2.left = 2 + clientRectangle.right * 2 / 3;
			rc2.bottom = rc0.top - clientRectangle.bottom / 80;
			rc2.right = rc1.right;
			rc3.top = rc0.bottom + clientRectangle.bottom / 40;
			rc3.left = rc0.left + clientRectangle.right / 40;
			rc3.bottom = clientRectangle.bottom / 2 - clientRectangle.bottom / 40;
			rc3.right = rc1.right - clientRectangle.right / 40;
			wsprintf((LPWSTR)szPRL[2], TEXT("ユニクロ"));
			if (DQ == 1) { wsprintf((LPWSTR)szPRL[2], TEXT("鹿島建設")); }
			Tr = 255;
			Tg = 255;
		}
		if (companyAStockPriceChangeAmount == 3) {
			rc0.left = 0;
			rc0.top = clientRectangle.bottom / 16 + clientRectangle.bottom / 2;
			rc0.right = clientRectangle.right / number;
			rc0.bottom = clientRectangle.bottom / 9 + clientRectangle.bottom / 2;
			rc1.left = rc0.right;
			rc1.top = clientRectangle.bottom / 16 + clientRectangle.bottom / 2;
			rc1.right = clientRectangle.right / number * 2;
			rc1.bottom = clientRectangle.bottom / 9 + clientRectangle.bottom / 2;
			rc2.top = 2 + clientRectangle.bottom / 2;
			rc2.left = 2;
			rc2.bottom = rc0.top - clientRectangle.bottom / 80;
			rc2.right = rc1.right;
			rc3.top = rc0.bottom + clientRectangle.bottom / 40;
			rc3.left = rc0.left + clientRectangle.right / 40;
			rc3.bottom = clientRectangle.bottom / 2 - clientRectangle.bottom / 40 + clientRectangle.bottom / 2;
			rc3.right = rc1.right - clientRectangle.right / 40;
			wsprintf((LPWSTR)szPRL[2], TEXT("森永製菓"));

			Tr = 102;
			Tg = 255;
			Tb = 255;
		}
		if (companyAStockPriceChangeAmount == 4) {
			rc0.left = 0 + clientRectangle.right / 3;
			rc0.top = clientRectangle.bottom / 16 + clientRectangle.bottom / 2;
			rc0.right = clientRectangle.right / number + clientRectangle.right / 3;
			rc0.bottom = clientRectangle.bottom / 9 + clientRectangle.bottom / 2;
			rc1.left = rc0.right;
			rc1.top = rc0.top;
			rc1.right = clientRectangle.right / number * 2 + clientRectangle.right / 3;
			rc1.bottom = rc0.bottom;
			rc2.top = 2 + clientRectangle.bottom / 2;
			rc2.left = 2 + clientRectangle.right / 3;
			rc2.bottom = rc0.top - clientRectangle.bottom / 80;
			rc2.right = rc1.right;
			rc3.top = rc0.bottom + clientRectangle.bottom / 40;
			rc3.left = rc0.left + clientRectangle.right / 40;
			rc3.bottom = clientRectangle.bottom / 2 - clientRectangle.bottom / 40 + clientRectangle.bottom / 2;
			rc3.right = rc1.right - clientRectangle.right / 40;
			wsprintf((LPWSTR)szPRL[2], TEXT("イトーヨーカドー"));
			if (DQ == 1) { wsprintf((LPWSTR)szPRL[2], TEXT("ユニクロ")); }
			Tr = 51;
			Tg = 204;
		}
		if (companyAStockPriceChangeAmount == 5) {
			rc0.left = clientRectangle.right * 2 / 3;
			rc0.top = clientRectangle.bottom / 16 + clientRectangle.bottom / 2;
			rc0.right = clientRectangle.right * 2 / 3 + clientRectangle.right / number;
			rc0.bottom = clientRectangle.bottom / 9 + clientRectangle.bottom / 2;
			rc1.left = clientRectangle.right * 2 / 3 + clientRectangle.right / number;
			rc1.top = rc0.top;
			rc1.right = clientRectangle.right * 2 / 3 + clientRectangle.right / number * 2;
			rc1.bottom = rc0.bottom;
			rc2.top = 2 + clientRectangle.bottom / 2;
			rc2.left = 2 + clientRectangle.right * 2 / 3;
			rc2.bottom = rc0.top - clientRectangle.bottom / 80;
			rc2.right = rc1.right;
			rc3.top = rc0.bottom + clientRectangle.bottom / 40;
			rc3.left = rc0.left + clientRectangle.right / 40;
			rc3.bottom = clientRectangle.bottom / 2 - clientRectangle.bottom / 40 + clientRectangle.bottom / 2;
			rc3.right = rc1.right - clientRectangle.right / 40;
			wsprintf((LPWSTR)szPRL[2], TEXT("ＪＴＢ"));
			Tr = 211;
			Tg = 211;
			Tb = 211;
		}
		SetDCBrushColor(hdc, RGB(Tr, Tg, Tb));

		rc2.left += clientRectangle.right / 15;
		rc2.right -= clientRectangle.right / 15;
		rc2.top += clientRectangle.bottom / 200;
		rc2.top += clientRectangle.bottom / 200;
		rc2.bottom += clientRectangle.bottom / 200;
		wsprintf((LPWSTR)szPRL[0], TEXT("株価；%d"), stockPrices[companyAStockPriceChangeAmount - 1][his - 1]);
		wsprintf((LPWSTR)szPRL[3], szPR[companyAStockPriceChangeAmount * 2 + 13]);
		wsprintf((LPWSTR)szPRL[4], szPR[companyAStockPriceChangeAmount * 2 + 14]);

		/*初期の枠を作るスクリプト開始*/

		for (o = 0; o < his; o++) {
			stockPriceChartGraphPoint[companyAStockPriceChangeAmount - 1][o].x = (rc3.right - rc3.left) / (his) * (o + 1) + rc3.left - clientRectangle.right / 300;
		}

		SelectObject(hdc, hPen);
		for (i = 0; i < 2; i++) {
			for (q = 0; q < number / 2; q++) {
				Rectangle(hdc, clientRectangle.right * q * 2 / number, clientRectangle.bottom * i / 2, clientRectangle.right * (q + 1) * 2 / number, clientRectangle.bottom * (i + 1) / 2);
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
		rc41.top = rc40.top + clientRectangle.bottom / 100;

		SelectObject(hdc, hBrushT);

		RoundRect(hdc, rc2.left, rc2.top - clientRectangle.bottom / 200, rc2.right, rc2.bottom, (clientRectangle.right + clientRectangle.bottom) / 50, (clientRectangle.right + clientRectangle.bottom) / 50);

		SelectObject(hdc, hPen);

		SelectObject(hdc, hFont6);

		SelectObject(hdc, hBrush);

		SetTextColor(hdc, RGB(0, 0, 0));

		DrawText(hdc, szPRL[2], -1, &rc2, DT_CENTER);


		SelectObject(hdc, hBrushW);

		SelectObject(hdc, hPen);

		Rectangle(hdc, rc3.left, rc3.top, rc3.right, rc3.bottom);

		SetTextColor(hdc, RGB(0, 0, 255));
		DrawText(hdc, szPRL[0], -1, &rc41, DT_CENTER);

		/*数字処理end*/

		SelectObject(hdc, hPen2);

		Polyline(hdc, stockPriceChartGraphPoint[companyAStockPriceChangeAmount - 1], HH);
		SelectObject(hdc, hFont3);
		SetTextColor(hdc, RGB(255, 102, 51));
		TextOut(hdc, rc3.right, rc3.top, szPRL[3], lstrlen(szPRL[3]));
		SetTextColor(hdc, RGB(102, 102, 255));
		TextOut(hdc, rc3.right, rc3.bottom - 20, szPRL[4], lstrlen(szPRL[4]));

	}
	if (companyAStockPriceChangeAmount == 6) {
		rc0.left = 0;
		rc0.top = clientRectangle.bottom / 16;
		rc0.right = clientRectangle.right / number;
		rc0.bottom = clientRectangle.bottom / 9;
		rc1.left = rc0.right;
		rc1.top = rc0.top;
		rc1.right = clientRectangle.right / number * 2;
		rc1.bottom = rc0.bottom;
		rc2.top = 2;
		rc2.left = 2;
		rc2.bottom = rc0.top - clientRectangle.bottom / 80;
		rc2.right = rc1.right;
		rc3.top = rc0.bottom + clientRectangle.bottom / 40;
		rc3.left = rc0.left + clientRectangle.right / 80;
		rc3.bottom = clientRectangle.bottom / 2 - clientRectangle.bottom / 40;
		rc3.right = rc1.right - clientRectangle.right / 80;
		rc4.left = rc1.right + clientRectangle.right / 20;
		rc4.top = clientRectangle.bottom / 60;
		rc4.bottom = rc0.top;
		rc4.right = clientRectangle.right / number * 2 * 2 - clientRectangle.right / 100;
		/*時計処理start*/
		TCHAR sur[10];
		SelectObject(hdc, hFont2);
		wsprintf(sur, TEXT("%3d"), currentRemainingTime / 1000);

		SetTextColor(hdc, RGB(0, 51, 0));
		TextOut(hdc, rc1.right + clientRectangle.right / 50, rc4.top + rc4.top * 3 / 4, sur, lstrlen(sur));
		SelectObject(hdc, hPen);
		SelectObject(hdc, hBrushW);
		Rectangle(hdc, rc4.left, rc4.top, rc4.right, rc4.bottom);
		hBrush = CreateSolidBrush(RGB(204, 255, 102));
		SelectObject(hdc, hBrush);

		Rectangle(hdc, rc4.left, rc4.top, rc4.right, rc4.bottom);

		SelectObject(hdc, hBrushW);

		Rectangle(hdc, rc4.right - (limitedTime - currentRemainingTime) * (rc4.right - rc4.left) / limitedTime, rc4.top, rc4.right, rc4.bottom);

		/*時計処理end*/
		/*ニュース処理start*/
		SelectObject(hdc, hFont1);
		SetTextColor(hdc, RGB(153, 102, 0));
		TextOut(hdc, rc1.right + clientRectangle.right / 50, rc4.top + rc4.top * 3, TEXT("～ニュース～"), lstrlen(TEXT("～ニュース～")));

		rc6.top = rc0.bottom + clientRectangle.bottom / 40;
		rc6.left = rc0.left + clientRectangle.right / 160 + clientRectangle.right / 3;
		rc6.bottom = clientRectangle.bottom / 2 - clientRectangle.bottom / 10;
		rc6.right = rc1.right - clientRectangle.right / 32 + clientRectangle.right / 3;

		SelectObject(hdc, hFont6);
		DrawText(hdc, S, lstrlen(S), &rc6, DT_CENTER | DT_WORDBREAK);

		SelectObject(hdc, hFont3);
		DrawText(hdc, Sb, lstrlen(Sb), &rc6, DT_CENTER | DT_WORDBREAK);

		rc23.left = clientRectangle.right / 2 + clientRectangle.right / 10;
		rc23.right = clientRectangle.right * 2 / 3 - clientRectangle.right / 30;
		rc23.top = clientRectangle.bottom * 2 / 7;
		rc23.bottom = clientRectangle.bottom / 2 - clientRectangle.bottom / 10;

		rc24.left = clientRectangle.right / 3;
		rc24.right = clientRectangle.right * 3 / 5 - clientRectangle.right / 10;
		rc24.top = clientRectangle.bottom * 2 / 7;
		rc24.bottom = clientRectangle.bottom / 2;

		rc25.left = clientRectangle.right / 2 + clientRectangle.right / 20;
		rc25.right = clientRectangle.right * 2 / 3 + clientRectangle.right / 50;
		rc25.top = clientRectangle.bottom / 2 - clientRectangle.bottom / 10;
		rc25.bottom = clientRectangle.bottom / 2;

		if (DQ != 1) {

			SelectObject(hdc, hBrushW);
			SelectObject(hdc, hPen3);

			Rectangle(hdc, rc23.left, rc23.top, rc23.right, rc23.bottom);

			Rectangle(hdc, rc24.left, rc24.top, rc24.right, rc24.bottom);

			SelectObject(hdc, hPen);
			Ellipse(hdc, rc23.left + (rc23.right - rc23.left) / 2 - ((rc23.bottom - clientRectangle.bottom / 23) - (rc23.top - clientRectangle.bottom / 100)) / 2 - clientRectangle.bottom / 100, rc23.top - clientRectangle.bottom / 100 - clientRectangle.bottom / 100, (rc23.right - rc23.left) / 2 + ((rc23.bottom - clientRectangle.bottom / 23) - (rc23.top - clientRectangle.bottom / 100)) / 2 + rc23.left + clientRectangle.bottom / 100, rc23.bottom - clientRectangle.bottom / 23 + clientRectangle.bottom / 100);

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
	if (companyAStockPriceChangeAmount != 6) {
		if (AS[companyAStockPriceChangeAmount - 1] > 0) {
			SelectObject(hdc, hBrushw);
			Rectangle(hdc, rc0.left, rc0.top, rc1.right, rc3.bottom);
			SelectObject(hdc, hFont3);
			wsprintf((LPWSTR)CON, TEXT("\nこの会社は倒産しました。\n株のカードをボードに戻してください。"));
			DrawText(hdc, CON, -1, &rc3, DT_CENTER);
		}
	}

	DeleteObject(hBrush);
	DeleteObject(hBrushW);
	DeleteObject(hPen);
	DeleteObject(hPen2);
	return;
}
