// win01.cpp

#define stockPriceChartGraphPointNumber 10

#include <Windows.h>
#include <iostream>

#include <time.h>


#define ID_LOADING_TIMER 100
#define ID_MAIN_TIMER 200

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
ATOM InitApp(HINSTANCE);
BOOL InitInstance(HINSTANCE, int);
HFONT MyCreateFont(int, DWORD, LPCTSTR);
void Data(int);
void Ftime(int, RECT, HDC);

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

//それぞれの企業の株価 一次元目で企業を指定、二次元目で期間を指定
static int stockPrices[5][stockPriceChartGraphPointNumber + 1];

//画面に表示する期間の数
const int his = stockPriceChartGraphPointNumber;
static int EV = 0;
TCHAR messageEventTitle[100];
TCHAR messageEventNumber[30];
const static int cellNumber = 6;/*表示する領域の数*/
static POINT stockPriceChartGraphPoint[5][stockPriceChartGraphPointNumber];
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
TCHAR eachCompanyMaxStockPriceInPeriod[10][10];
TCHAR messageWaitForMinute[] = TEXT("\n今からの取引はできません。\n少々お待ちください。");
TCHAR messageNextLimitTime[] = TEXT("\n\n\n\n\n次回の制限時間；%d秒");
TCHAR messageCompanyBankruptcy[] = TEXT("\nこの会社は倒産しました。\n株のカードをボードに戻してください。");
TCHAR messageLoadingPleaseWait[] = TEXT("\n読み込み中です。\n少々お待ちください。");
TCHAR messageLoadProgress[] = TEXT("\n\n\n\n\n%d完了");

const int nD = 98;
bool nLOOP[130];

bool isTrading;
HPEN hPen4, hPen5;
int programLoadRate;

static int AS[5];

static HBITMAP hBmp;
BITMAP bmp_info;
static HDC hdc_mem;
int bmpw, bmph;

//ウィンドウプロシージャ
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp)
{
	RECT clientRectangle;
	RECT rc0, rc1, rc2, rc3;
	RECT rc4;

	int i;
	int stockPriceFraction;
	i = 0;
	int destroyCheckStatus;

	static int maxStockPriceInPeriod, minStockPriceInPeriod;

	static int XX;

	/*数字処理start*/

	GetClientRect(hWnd, &clientRectangle);

	/*数字処理end*/

	switch (msg) {
	case WM_CREATE:
		for (int k = 0; k < nD; k++) {
			nLOOP[k] = false;
		}
		hPen5 = (HPEN)GetStockObject(BLACK_PEN);
		hPen4 = (HPEN)GetStockObject(NULL_PEN);
		hBrushw = (HBRUSH)GetStockObject(WHITE_BRUSH);
		hBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
		limitedTime = 10000;
		currentRemainingTime = limitedTime;
		isTrading = true;
		rc0 = RECT{ 0, clientRectangle.bottom / 16, clientRectangle.right / cellNumber, clientRectangle.bottom / 9 };
		rc1 = RECT{ rc0.right , rc0.top, clientRectangle.right / cellNumber * 2, rc0.bottom };
		rc2 = RECT{ 2, 2, rc0.top - clientRectangle.bottom / 80 , rc1.right };
		rc3 = RECT{ rc0.bottom + clientRectangle.bottom / 40, rc0.left + clientRectangle.right / 160, clientRectangle.bottom / 2 - clientRectangle.bottom / 40, rc3.right = rc1.right - clientRectangle.right / 35 };
		/*それぞれの全期間での株価の初期化*/
		for (int k = 0; k < stockPriceChartGraphPointNumber; k++) {
			stockPrices[0][k] = 15000;
			stockPrices[1][k] = 15000;
			stockPrices[2][k] = 15000;
			stockPrices[3][k] = 15000;
			stockPrices[4][k] = 15000;
		}
		SetTimer(hWnd, ID_LOADING_TIMER, 200, NULL);

		srand((unsigned)time(NULL));

		hFont1 = MyCreateFont(36, SHIFTJIS_CHARSET, TEXT("ＭＳ 明朝"));
		hFont2 = MyCreateFont(15, SHIFTJIS_CHARSET, TEXT("ＭＳ 明朝"));
		hFont3 = MyCreateFont(10, SHIFTJIS_CHARSET, TEXT("ＭＳ 明朝"));
		hFont4 = MyCreateFont(40, SHIFTJIS_CHARSET, TEXT("ＭＳ 明朝"));
		hFont5 = MyCreateFont(50, SHIFTJIS_CHARSET, TEXT("ＭＳ 明朝"));
		hFont6 = MyCreateFont(25, SHIFTJIS_CHARSET, TEXT("ＭＳ 明朝"));

		for (int k = 0; k < his; k++) {
			stockPriceChartGraphPoint[0][k].y = (rc3.bottom - rc3.top) / 2 + rc3.top;
			stockPriceChartGraphPoint[0][k].x = (rc3.right - rc3.left) / (his) * (k + 1) + rc3.left;
			stockPriceChartGraphPoint[1][k].y = (rc3.bottom - rc3.top) / 2 + rc3.top;
			stockPriceChartGraphPoint[1][k].x = (rc3.right - rc3.left) / (his) * (k + 1) + rc3.left + clientRectangle.right * 2 / 3;
			stockPriceChartGraphPoint[2][k].y = (rc3.bottom - rc3.top) / 2 + rc3.top + clientRectangle.bottom / 2;
			stockPriceChartGraphPoint[2][k].x = (rc3.right - rc3.left) / (his) * (k + 1) + rc3.left;
			stockPriceChartGraphPoint[3][k].y = (rc3.bottom - rc3.top) / 2 + rc3.top + clientRectangle.bottom / 2;
			stockPriceChartGraphPoint[3][k].x = (rc3.right - rc3.left) / (his) * (k + 1) + rc3.left + clientRectangle.right / 3;
			stockPriceChartGraphPoint[4][k].y = (rc3.bottom - rc3.top) / 2 + rc3.top + clientRectangle.bottom / 2;
			stockPriceChartGraphPoint[4][k].x = (rc3.right - rc3.left) / (his) * (k + 1) + rc3.left + clientRectangle.right * 2 / 3;
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
		destroyCheckStatus = MessageBox(hWnd,
			TEXT("終了してもよろしいですか"),
			TEXT("確認"),
			MB_YESNO | MB_ICONQUESTION);
		if (destroyCheckStatus == IDYES) {
			DestroyWindow(hWnd);
		}
		break;
	case WM_DESTROY:
		KillTimer(hWnd, ID_LOADING_TIMER);
		KillTimer(hWnd, ID_MAIN_TIMER);
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
		hdc = BeginPaint(hWnd, &ps);

		SelectObject(hdc, hPen4);
		SelectObject(hdc, hBrushw);
		Rectangle(hdc, clientRectangle.left, clientRectangle.top, clientRectangle.right, clientRectangle.bottom);

		if (!(EV < his + 3)) {
			/*
			* 取引中画面の表示
			*/
			if (isTrading) {
				Ftime(1, clientRectangle, hdc);
				Ftime(2, clientRectangle, hdc);
				Ftime(3, clientRectangle, hdc);
				Ftime(5, clientRectangle, hdc);
				Ftime(4, clientRectangle, hdc);
				Ftime(6, clientRectangle, hdc);
			}

			/*
			* 取引準備中画面の表示
			*/
			if (!isTrading) {
				bmph = clientRectangle.bottom / 9;
				bmpw = bmph;
				SelectObject(hdc, hBrushw);
				Rectangle(hdc, clientRectangle.left, clientRectangle.top, clientRectangle.right, clientRectangle.bottom);
				SelectObject(hdc, hFont5);
				wsprintf((LPWSTR)messageWaitForMinute, TEXT("\n今からの取引はできません。\n少々お待ちください。"));
				DrawText(hdc, messageWaitForMinute, -1, &clientRectangle, DT_CENTER);
				wsprintf((LPWSTR)messageNextLimitTime, TEXT("\n\n\n\n\n次回の制限時間；%d秒"), limitedTime / 1000);
				DrawText(hdc, messageNextLimitTime, -1, &clientRectangle, DT_CENTER);

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
			}
		}
		SelectObject(hdc, hBrush);

		/*
		* 読み込み中画面の表示
		*/
		if (EV < his + 3) {
			SelectObject(hdc, hBrushw);

			Rectangle(hdc, clientRectangle.left, clientRectangle.top, clientRectangle.right, clientRectangle.bottom);
			SelectObject(hdc, hPen5);
			SelectObject(hdc, hFont5);
			wsprintf((LPWSTR)messageLoadingPleaseWait, TEXT("\n読み込み中です。\n少々お待ちください。"));
			DrawText(hdc, messageLoadingPleaseWait, -1, &clientRectangle, DT_CENTER);
			wsprintf((LPWSTR)messageLoadProgress, TEXT("\n\n\n\n\n%d％完了"), programLoadRate * 100 / (his + 1 + limitedTime / 1000));
			DrawText(hdc, messageLoadProgress, -1, &clientRectangle, DT_CENTER);
		}
		EndPaint(hWnd, &ps);

		break;
	case WM_TIMER:
		if (EV == his + 3) {
			KillTimer(hWnd, ID_LOADING_TIMER);
			SetTimer(hWnd, ID_MAIN_TIMER, 1000, NULL);
		}
		programLoadRate++;

		if (isTrading) {
			rc0 = RECT{ 0, clientRectangle.bottom / 16, clientRectangle.right / cellNumber, clientRectangle.bottom / 9 };
			rc1 = RECT{ rc0.right, rc0.top, clientRectangle.right / cellNumber * 2, rc0.bottom };
			rc2 = RECT{ 2, 2, rc1.right, rc0.top - clientRectangle.bottom / 80 };
			rc3 = RECT{ rc0.left + clientRectangle.right / 80, rc0.bottom + clientRectangle.bottom / 40, rc1.right - clientRectangle.right / 80, clientRectangle.bottom / 2 - clientRectangle.bottom / 40 };
			rc4 = RECT{ rc1.right + clientRectangle.right / 20, clientRectangle.bottom / 60, clientRectangle.right / cellNumber * 2 * 2 - clientRectangle.right / 100, rc0.top };
			
			if (EV < his + 2) {
				currentRemainingTime -= limitedTime - 1000;
			}

			currentRemainingTime -= 1000;

			RECT rc5 = RECT{ clientRectangle.right / 3 , rc4.top, rc4.right, rc4.bottom };
			
			InvalidateRect(hWnd, &rc5, 0);

			if (EV < his + 3) {
				InvalidateRect(hWnd, NULL, 0);
			}

			if (currentRemainingTime == 0) {

				if (!(EV < his + 2)) {
					isTrading = false;
				}

				EV++;
				
				/*
				* それぞれの企業の最新の株価を15000円に初期化
				*/
				if (EV == 2) {
					stockPrices[0][his] = 15000;
					stockPrices[1][his] = 15000;
					stockPrices[2][his] = 15000;
					stockPrices[3][his] = 15000;
					stockPrices[4][his] = 15000;
				}

				/*
				* 次の株価の増減を選択
				*/
				for (int j = 0;true;) {
					XX = rand() % nD;
					if (nLOOP[XX] == true) {
						j++;
						if (j == 1000000) {
							Data(1000);
							break;
						}
						continue;
					}
					if (!(EV < his + 3)) {
						nLOOP[XX] = true;
						if (NE == 36) {
							Data(34);
							nLOOP[34] = true;
							NE = 0;
						}
						else {
							Data(XX);
						}
					}
					break;
				}

				/*
				* ある期間の情報を一つ古い期間のものとする
				* グラフに表示する期間の最大・最小株価を求める
				* グラフに表示するそれぞれの点のy座標を求める
				*/
				for (int k = 0; k < 5; k++) {
					for (int l = 0; l < his; l++) {
						stockPrices[k][l] = stockPrices[k][l + 1];
					}
					for (int l = 0; l < his; l++) {
						if (l == 0) {
							maxStockPriceInPeriod = stockPrices[k][l];
						}
						else if (stockPrices[k][l] > maxStockPriceInPeriod) {
							maxStockPriceInPeriod = stockPrices[k][l];
						}
					}
					for (int l = 0; l < his; l++) {
						if (l == 0) {
							minStockPriceInPeriod = stockPrices[k][l];
						}
						else if (stockPrices[k][l] < minStockPriceInPeriod) {
							minStockPriceInPeriod = stockPrices[k][l];
						}
					}
					if (maxStockPriceInPeriod != minStockPriceInPeriod) {
						if (k < 2) {
							for (int l = 0; l < his; l++) {
								i = (stockPrices[k][l] - minStockPriceInPeriod) * 100 / (maxStockPriceInPeriod - minStockPriceInPeriod) * ((clientRectangle.bottom / 2 - clientRectangle.bottom / 30) - (rc0.bottom + clientRectangle.bottom / 30));
								stockPriceChartGraphPoint[k][l].y = clientRectangle.bottom / 2 - clientRectangle.bottom / 30 - i / 100;
							}
						}
						else {
							for (int l = 0; l < his; l++) {
								i = (stockPrices[k][l] - minStockPriceInPeriod) * 100 / (maxStockPriceInPeriod - minStockPriceInPeriod) * ((clientRectangle.bottom / 2 - clientRectangle.bottom / 30) - (rc0.bottom + clientRectangle.bottom / 30));
								stockPriceChartGraphPoint[k][l].y = clientRectangle.bottom / 2 - clientRectangle.bottom / 30 - i / 100 + clientRectangle.bottom / 2;
							}
						}
					}
					else {
						if (k < 2) {
							for (int l = 0; l < his; l++) {
								stockPriceChartGraphPoint[k][l].y = (rc3.bottom - rc3.top) / 2 + rc3.top;
							}
						}
						else {
							for (int l = 0; l < his; l++) {
								stockPriceChartGraphPoint[k][l].y = (rc3.bottom - rc3.top) / 2 + rc3.top + clientRectangle.bottom / 2;
							}
						}
					}
					wsprintf(eachCompanyMaxStockPriceInPeriod[k * 2], TEXT("%d"), maxStockPriceInPeriod);
					wsprintf(eachCompanyMaxStockPriceInPeriod[k * 2], TEXT("%d"), minStockPriceInPeriod);

				}

				currentRemainingTime = limitedTime;

				/*
				* それぞれの企業の次の株価を決定
				*/
				stockPrices[0][his] = stockPrices[0][his - 1] + companyAStockPriceChangeAmount;
				stockPrices[1][his] = stockPrices[1][his - 1] + companyBStockPriceChangeAmount;
				stockPrices[2][his] = stockPrices[2][his - 1] + companyCStockPriceChangeAmount;
				stockPrices[3][his] = stockPrices[3][his - 1] + companyDStockPriceChangeAmount;
				stockPrices[4][his] = stockPrices[4][his - 1] + companyEStockPriceChangeAmount;
				
				/*
				* 1000円以下の端数の四捨五入
				*/
				for (int k = 0; k < 5; k++) {
					std::cout << stockPrices[k][stockPriceChartGraphPointNumber] << std::endl;
					stockPriceFraction = stockPrices[k][stockPriceChartGraphPointNumber] % 1000;
					if (stockPriceFraction >= 500) {
						stockPrices[k][stockPriceChartGraphPointNumber] += 1000;
					}
					stockPrices[k][stockPriceChartGraphPointNumber] -= stockPriceFraction;
				}

				if (EV < his + 3) {
					for (int k = 0; k < 5; k++) {
						if (stockPrices[k][stockPriceChartGraphPointNumber] < 12000) {
							stockPrices[k][stockPriceChartGraphPointNumber] = 12000;
						}
					}

					for (int k = 0; k < 5; k++) {
						if (stockPrices[k][stockPriceChartGraphPointNumber] > 18000) {
							stockPrices[k][stockPriceChartGraphPointNumber] = 18000;
						}
					}
				}
				if (EV > stockPriceChartGraphPointNumber - 2) {
					for (int k = 0; k < 5; k++) {
						if (stockPrices[k][his - 1] < 100) {
							AS[k] = 2;
						}
						if (AS[k] > 0) { AS[k]--; }
					}
				}
				int r;
				for (int w = 0; w < 5; w++) {
					if (AS[w] > 0) {
						for (int k = 0; k < stockPriceChartGraphPointNumber + 1; k++) {
							//-2000~2000
							r = rand() % 4001 - 2000;
							stockPrices[w][k] = 15000 + r;
							stockPriceFraction = stockPrices[w][k] % 1000;
							if (stockPriceFraction >= 500) {
								stockPrices[w][k] += 1000;
							}
							stockPrices[w][k] -= stockPriceFraction;
						}
					}
				}
				InvalidateRect(hWnd, NULL, 0);
			}
		}
		break;
	case WM_CHAR:
		if (wp == VK_ESCAPE) {
			if (!isTrading) isTrading = true; InvalidateRect(hWnd, &clientRectangle, 0); break;
			isTrading = false;
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

void Data(const int eventNumber) {
	if (EV < his + 3) {
		companyAStockPriceChangeAmount = (rand() % 7 - 3) * 700;
		companyBStockPriceChangeAmount = (rand() % 7 - 3) * 700;
		companyCStockPriceChangeAmount = (rand() % 7 - 3) * 700;
		companyDStockPriceChangeAmount = (rand() % 7 - 3) * 700;
		companyEStockPriceChangeAmount = (rand() % 7 - 3) * 700;


	}
	else {
		if (eventNumber == 0) {
			wsprintf(messageEventTitle, TEXT("本州と九州を結ぶ新しい橋が完成"));
			companyAStockPriceChangeAmount = 000;
			companyBStockPriceChangeAmount = 000;
			companyCStockPriceChangeAmount = 1000;
			companyDStockPriceChangeAmount = 1000;
			companyEStockPriceChangeAmount = 3000;
		}
		if (eventNumber == 1) {
			wsprintf(messageEventTitle, TEXT("南海トラフ地震が発生"));
			companyAStockPriceChangeAmount = -0;
			companyBStockPriceChangeAmount = -3000;
			companyCStockPriceChangeAmount = -4000;
			companyDStockPriceChangeAmount = -3000;
			companyEStockPriceChangeAmount = -5000;
		}
		if (eventNumber == 2) {
			wsprintf(messageEventTitle, TEXT("タイで大洪水が発生"));
			companyAStockPriceChangeAmount = -4000;
			companyBStockPriceChangeAmount = -1000;
			companyCStockPriceChangeAmount = 000;
			companyDStockPriceChangeAmount = 000;
			companyEStockPriceChangeAmount = -2000;
		}
		if (eventNumber == 3) {
			wsprintf(messageEventTitle, TEXT("日本全国で米が不作"));
			companyAStockPriceChangeAmount = 000;
			companyBStockPriceChangeAmount = 000;
			companyCStockPriceChangeAmount = 000;
			companyDStockPriceChangeAmount = -2000;
			companyEStockPriceChangeAmount = -1000;
		}
		if (eventNumber == 4) {
			wsprintf(messageEventTitle, TEXT("愛知県に巨大台風が上陸"));
			companyAStockPriceChangeAmount = -3000;
			companyBStockPriceChangeAmount = -1000;
			companyCStockPriceChangeAmount = -1000;
			companyDStockPriceChangeAmount = -1000;
			companyEStockPriceChangeAmount = -1000;
		}
		if (eventNumber == 5) {
			wsprintf(messageEventTitle, TEXT("ハワイで大規模な火山噴火"));
			companyAStockPriceChangeAmount = 000;
			companyBStockPriceChangeAmount = 000;
			companyCStockPriceChangeAmount = 000;
			companyDStockPriceChangeAmount = 000;
			companyEStockPriceChangeAmount = -5000;
		}
		if (eventNumber == 6) {
			wsprintf(messageEventTitle, TEXT("九州地方で地震が発生"));
			companyAStockPriceChangeAmount = -2000;
			companyBStockPriceChangeAmount = -1000;
			companyCStockPriceChangeAmount = -1000;
			companyDStockPriceChangeAmount = -1000;
			companyEStockPriceChangeAmount = -2000;
		}
		if (eventNumber == 7) {
			wsprintf(messageEventTitle, TEXT("京都が集中豪雨で大きな被害"));
			companyAStockPriceChangeAmount = -2000;
			companyBStockPriceChangeAmount = 000;
			companyCStockPriceChangeAmount = 000;
			companyDStockPriceChangeAmount = 000;
			companyEStockPriceChangeAmount = -2000;
		}
		if (eventNumber == 8) {
			wsprintf(messageEventTitle, TEXT("鎌倉が世界遺産に"));
			companyAStockPriceChangeAmount = 000;
			companyBStockPriceChangeAmount = 000;
			companyCStockPriceChangeAmount = 000;
			companyDStockPriceChangeAmount = 000;
			companyEStockPriceChangeAmount = 3000;
		}
		if (eventNumber == 9) {
			wsprintf(messageEventTitle, TEXT("富士山が世界遺産に"));
			companyAStockPriceChangeAmount = 000;
			companyBStockPriceChangeAmount = 000;
			companyCStockPriceChangeAmount = 000;
			companyDStockPriceChangeAmount = 000;
			companyEStockPriceChangeAmount = 3000;
		}
		if (eventNumber == 10) {
			wsprintf(messageEventTitle, TEXT("日本を訪れる外国人が3000万人を超える"));
			companyAStockPriceChangeAmount = 000;
			companyBStockPriceChangeAmount = 1000;
			companyCStockPriceChangeAmount = 1000;
			companyDStockPriceChangeAmount = 1000;
			companyEStockPriceChangeAmount = 6000;
		}
		if (eventNumber == 11) {
			wsprintf(messageEventTitle, TEXT("伊勢志摩サミットが開催"));
			companyAStockPriceChangeAmount = 1000;
			companyBStockPriceChangeAmount = 000;
			companyCStockPriceChangeAmount = 000;
			companyDStockPriceChangeAmount = 000;
			companyEStockPriceChangeAmount = 2000;
		}
		if (eventNumber == 12) {
			wsprintf(messageEventTitle, TEXT("フランスでテロ"));
			companyAStockPriceChangeAmount = -2000;
			companyBStockPriceChangeAmount = 000;
			companyCStockPriceChangeAmount = 000;
			companyDStockPriceChangeAmount = -1000;
			companyEStockPriceChangeAmount = -4000;
		}
		if (eventNumber == 13) {
			wsprintf(messageEventTitle, TEXT("電気料金が値上げ"));
			companyAStockPriceChangeAmount = -2000;
			companyBStockPriceChangeAmount = -1000;
			companyCStockPriceChangeAmount = -1000;
			companyDStockPriceChangeAmount = -2000;
			companyEStockPriceChangeAmount = -1000;
		}
		if (eventNumber == 14) {
			wsprintf(messageEventTitle, TEXT("石油の値段が上がる"));
			companyAStockPriceChangeAmount = -1000;
			companyBStockPriceChangeAmount = -1000;
			companyCStockPriceChangeAmount = -1000;
			companyDStockPriceChangeAmount = -1000;
			companyEStockPriceChangeAmount = -3000;
		}
		if (eventNumber == 15) {
			wsprintf(messageEventTitle, TEXT("石油の値段が下がる"));
			companyAStockPriceChangeAmount = 1000;
			companyBStockPriceChangeAmount = 1000;
			companyCStockPriceChangeAmount = 1000;
			companyDStockPriceChangeAmount = 1000;
			companyEStockPriceChangeAmount = 3000;
		}
		if (eventNumber == 16) {
			wsprintf(messageEventTitle, TEXT("鉄の値段が上がる"));
			companyAStockPriceChangeAmount = -2000;
			companyBStockPriceChangeAmount = 000;
			companyCStockPriceChangeAmount = 000;
			companyDStockPriceChangeAmount = 000;
			companyEStockPriceChangeAmount = 000;
		}
		if (eventNumber == 17) {
			wsprintf(messageEventTitle, TEXT("鉄の値段が下がる"));
			companyAStockPriceChangeAmount = 2000;
			companyBStockPriceChangeAmount = 000;
			companyCStockPriceChangeAmount = 000;
			companyDStockPriceChangeAmount = 000;
			companyEStockPriceChangeAmount = 000;
		}
		if (eventNumber == 18) {
			wsprintf(messageEventTitle, TEXT("今年の冬は暖冬になると予想"));
			companyAStockPriceChangeAmount = 000;
			companyBStockPriceChangeAmount = -2000;
			companyCStockPriceChangeAmount = 000;
			companyDStockPriceChangeAmount = 1000;
			companyEStockPriceChangeAmount = -1000;
		}
		if (eventNumber == 19) {
			wsprintf(messageEventTitle, TEXT("今年の夏は過去最高の猛暑になると予想"));
			companyAStockPriceChangeAmount = 000;
			companyBStockPriceChangeAmount = 1000;
			companyCStockPriceChangeAmount = 2000;
			companyDStockPriceChangeAmount = 2000;
			companyEStockPriceChangeAmount = 1000;
		}
		if (eventNumber == 20) {
			wsprintf(messageEventTitle, TEXT("北陸新幹線が開通"));
			companyAStockPriceChangeAmount = 000;
			companyBStockPriceChangeAmount = 000;
			companyCStockPriceChangeAmount = 000;
			companyDStockPriceChangeAmount = 000;
			companyEStockPriceChangeAmount = 3000;
		}
		if (eventNumber == 21) {
			wsprintf(messageEventTitle, TEXT("海外で日本食ブーム"));
			companyAStockPriceChangeAmount = 000;
			companyBStockPriceChangeAmount = 000;
			companyCStockPriceChangeAmount = 1000;
			companyDStockPriceChangeAmount = 1000;
			companyEStockPriceChangeAmount = 2000;
		}
		if (eventNumber == 22) {
			wsprintf(messageEventTitle, TEXT("日本を舞台にしたディズニー映画が大ヒット"));
			companyAStockPriceChangeAmount = 1000;
			companyBStockPriceChangeAmount = 1000;
			companyCStockPriceChangeAmount = 1000;
			companyDStockPriceChangeAmount = 2000;
			companyEStockPriceChangeAmount = 2000;
		}
		if (eventNumber == 23) {
			wsprintf(messageEventTitle, TEXT("ハワイでテロ"));
			companyAStockPriceChangeAmount = -1000;
			companyBStockPriceChangeAmount = -1000;
			companyCStockPriceChangeAmount = -1000;
			companyDStockPriceChangeAmount = -1000;
			companyEStockPriceChangeAmount = -5000;
		}
		if (eventNumber == 24) {
			wsprintf(messageEventTitle, TEXT("ドローンの新技術が開発"));
			companyAStockPriceChangeAmount = 1000;
			companyBStockPriceChangeAmount = 1000;
			companyCStockPriceChangeAmount = 000;
			companyDStockPriceChangeAmount = 1000;
			companyEStockPriceChangeAmount = 000;
		}
		if (eventNumber == 25) {
			wsprintf(messageEventTitle, TEXT("錦織圭がオリンピックで金メダル"));
			companyAStockPriceChangeAmount = 000;
			companyBStockPriceChangeAmount = 5000;
			companyCStockPriceChangeAmount = 000;
			companyDStockPriceChangeAmount = 000;
			companyEStockPriceChangeAmount = 2000;
		}
		if (eventNumber == 26) {
			wsprintf(messageEventTitle, TEXT("中国の軍艦と日本の漁船が衝突"));
			companyAStockPriceChangeAmount = -3000;
			companyBStockPriceChangeAmount = -3000;
			companyCStockPriceChangeAmount = 000;
			companyDStockPriceChangeAmount = -1000;
			companyEStockPriceChangeAmount = -3000;
		}
		if (eventNumber == 27) {
			wsprintf(messageEventTitle, TEXT("日本海で大量の石油が発見"));
			companyAStockPriceChangeAmount = 2000;
			companyBStockPriceChangeAmount = 2000;
			companyCStockPriceChangeAmount = 1000;
			companyDStockPriceChangeAmount = 1000;
			companyEStockPriceChangeAmount = 2000;
		}
		if (eventNumber == 28) {
			wsprintf(messageEventTitle, TEXT("富士山噴火の危険が高まる"));
			companyAStockPriceChangeAmount = 000;
			companyBStockPriceChangeAmount = 000;
			companyCStockPriceChangeAmount = 000;
			companyDStockPriceChangeAmount = 000;
			companyEStockPriceChangeAmount = -3000;
		}
		if (eventNumber == 29) {
			wsprintf(messageEventTitle, TEXT("中国で日本製品が大ブーム"));
			companyAStockPriceChangeAmount = 5000;
			companyBStockPriceChangeAmount = 4000;
			companyCStockPriceChangeAmount = 3000;
			companyDStockPriceChangeAmount = 2000;
			companyEStockPriceChangeAmount = 3000;
		}
		if (eventNumber == 30) {
			wsprintf(messageEventTitle, TEXT("中国人の爆買いが流行"));
			companyAStockPriceChangeAmount = 000;
			companyBStockPriceChangeAmount = 1000;
			companyBStockPriceChangeAmount = -3000;
			companyCStockPriceChangeAmount = 1000;
			companyDStockPriceChangeAmount = 3000;
			companyEStockPriceChangeAmount = 5000;
		}
		if (eventNumber == 31) {
			wsprintf(messageEventTitle, TEXT("首相が靖国神社参拝"));
			companyAStockPriceChangeAmount = -4000;
			companyCStockPriceChangeAmount = -1000;
			companyDStockPriceChangeAmount = -1000;
			companyEStockPriceChangeAmount = -3000;
		}
		if (eventNumber == 32) {
			wsprintf(messageEventTitle, TEXT("「石油が20年後に無くなる」と国連が発表"));
			companyAStockPriceChangeAmount = 1000;
			companyBStockPriceChangeAmount = -2000;
			companyCStockPriceChangeAmount = -2000;
			companyDStockPriceChangeAmount = -3000;
			companyEStockPriceChangeAmount = -5000;
		}
		if (eventNumber == 33) {
			wsprintf(messageEventTitle, TEXT("中国が歴史問題を理由に日中間の往来を制限"));
			companyAStockPriceChangeAmount = -4000;
			companyBStockPriceChangeAmount = -4000;
			companyCStockPriceChangeAmount = -1000;
			companyDStockPriceChangeAmount = -2000;
			companyEStockPriceChangeAmount = -6000;
		}
		if (eventNumber == 34) {
			wsprintf(messageEventTitle, TEXT("日本で中華料理が大ブーム"));
			companyAStockPriceChangeAmount = 1000;
			companyBStockPriceChangeAmount = 1000;
			companyCStockPriceChangeAmount = 000;
			companyDStockPriceChangeAmount = 2000;
			companyEStockPriceChangeAmount = 3000;
		}
		if (eventNumber == 35) {
			wsprintf(messageEventTitle, TEXT("イタリアで大地震が発生"));
			companyAStockPriceChangeAmount = 000;
			companyBStockPriceChangeAmount = 000;
			companyCStockPriceChangeAmount = 000;
			companyDStockPriceChangeAmount = 000;
			companyEStockPriceChangeAmount = -3000;
		}
		if (eventNumber == 36) {
			wsprintf(messageEventTitle, TEXT("消費税が15％に増税"));
			companyAStockPriceChangeAmount = -3000;
			companyBStockPriceChangeAmount = -2000;
			companyCStockPriceChangeAmount = -2000;
			companyDStockPriceChangeAmount = -2000;
			companyEStockPriceChangeAmount = -3000;
		}
		if (eventNumber == 37) {
			wsprintf(messageEventTitle, TEXT("韓国で新型インフルエンザが大流行"));
			companyAStockPriceChangeAmount = -1000;
			companyBStockPriceChangeAmount = -1000;
			companyCStockPriceChangeAmount = 000;
			companyDStockPriceChangeAmount = 1000;
			companyEStockPriceChangeAmount = 2000;
		}
		if (eventNumber == 38) {
			wsprintf(messageEventTitle, TEXT("本州で大規模な停電"));
			companyAStockPriceChangeAmount = -3000;
			companyBStockPriceChangeAmount = -2000;
			companyCStockPriceChangeAmount = -3000;
			companyDStockPriceChangeAmount = -3000;
			companyEStockPriceChangeAmount = -3000;
		}
		if (eventNumber == 39) {
			wsprintf(messageEventTitle, TEXT("サッカーワールドカップが大阪で開催"));
			companyAStockPriceChangeAmount = 000;
			companyBStockPriceChangeAmount = 1000;
			companyCStockPriceChangeAmount = 000;
			companyDStockPriceChangeAmount = 1000;
			companyEStockPriceChangeAmount = 4000;
		}
		if (eventNumber == 40) {
			wsprintf(messageEventTitle, TEXT("広島で土砂災害"));
			companyAStockPriceChangeAmount = -1000;
			companyBStockPriceChangeAmount = -1000;
			companyCStockPriceChangeAmount = -1000;
			companyDStockPriceChangeAmount = -1000;
			companyEStockPriceChangeAmount = -1000;
		}
		if (eventNumber == 41) {
			wsprintf(messageEventTitle, TEXT("エボラ出血熱がヨーロッパで大流行"));
			companyAStockPriceChangeAmount = -5000;
			companyBStockPriceChangeAmount = -2000;
			companyCStockPriceChangeAmount = -2000;
			companyDStockPriceChangeAmount = -2000;
			companyEStockPriceChangeAmount = -7000;
		}
		if (eventNumber == 42) {
			wsprintf(messageEventTitle, TEXT("高速道路料金が無料に"));
			companyAStockPriceChangeAmount = 2000;
			companyBStockPriceChangeAmount = 1000;
			companyCStockPriceChangeAmount = 1000;
			companyDStockPriceChangeAmount = 1000;
			companyEStockPriceChangeAmount = 2000;
		}
		if (eventNumber == 43) {
			wsprintf(messageEventTitle, TEXT("新幹線の事故が相次ぐ"));
			companyAStockPriceChangeAmount = 1000;
			companyBStockPriceChangeAmount = 000;
			companyCStockPriceChangeAmount = 000;
			companyDStockPriceChangeAmount = 000;
			companyEStockPriceChangeAmount = -2000;
		}
		if (eventNumber == 44) {
			wsprintf(messageEventTitle, TEXT("北朝鮮のミサイルが神奈川県沖に落下"));
			companyAStockPriceChangeAmount = -3000;
			companyBStockPriceChangeAmount = -3000;
			companyCStockPriceChangeAmount = -3000;
			companyDStockPriceChangeAmount = -3000;
			companyEStockPriceChangeAmount = -3000;
		}
		if (eventNumber == 45) {
			wsprintf(messageEventTitle, TEXT("上野動物園でパンダの赤ちゃんが生まれる"));
			companyAStockPriceChangeAmount = 1000;
			companyBStockPriceChangeAmount = 1000;
			companyCStockPriceChangeAmount = 000;
			companyDStockPriceChangeAmount = 000;
			companyEStockPriceChangeAmount = 2000;
		}
		if (eventNumber == 46) {
			wsprintf(messageEventTitle, TEXT("富岡製糸場が世界遺産に"));
			companyAStockPriceChangeAmount = 000;
			companyBStockPriceChangeAmount = 1000;
			companyCStockPriceChangeAmount = 000;
			companyDStockPriceChangeAmount = 000;
			companyEStockPriceChangeAmount = 3000;
		}
		if (eventNumber == 47) {
			wsprintf(messageEventTitle, TEXT("リニアモーターカーが開通"));
			companyAStockPriceChangeAmount = 000;
			companyBStockPriceChangeAmount = 000;
			companyCStockPriceChangeAmount = 000;
			companyDStockPriceChangeAmount = 1000;
			companyEStockPriceChangeAmount = 4000;
		}
		if (eventNumber == 48) {
			wsprintf(messageEventTitle, TEXT("成田国際空港の国際線が2倍に増える"));
			companyAStockPriceChangeAmount = 000;
			companyBStockPriceChangeAmount = 000;
			companyCStockPriceChangeAmount = 000;
			companyDStockPriceChangeAmount = 000;
			companyEStockPriceChangeAmount = 5000;
		}
		if (eventNumber == 49) {
			wsprintf(messageEventTitle, TEXT("中東で大規模な反政府デモ"));
			companyAStockPriceChangeAmount = -2000;
			companyBStockPriceChangeAmount = -2000;
			companyCStockPriceChangeAmount = -1000;
			companyDStockPriceChangeAmount = -1000;
			companyEStockPriceChangeAmount = -2000;
		}
		if (eventNumber == 50) {
			wsprintf(messageEventTitle, TEXT("インドの道路が整備される"));
			companyAStockPriceChangeAmount = 2000;
			companyBStockPriceChangeAmount = 2000;
			companyCStockPriceChangeAmount = 000;
			companyDStockPriceChangeAmount = 000;
			companyEStockPriceChangeAmount = 2000;
		}
		if (eventNumber == 51) {
			wsprintf(messageEventTitle, TEXT("アメリカが「30年でCO2排出量を半分にする」と発表"));
			companyAStockPriceChangeAmount = 7000;
			companyBStockPriceChangeAmount = 000;
			companyCStockPriceChangeAmount = 000;
			companyDStockPriceChangeAmount = 000;
			companyEStockPriceChangeAmount = 000;
		}
		if (eventNumber == 52) {
			wsprintf(messageEventTitle, TEXT("野菜の価格が昨年の2倍に"));
			companyAStockPriceChangeAmount = 000;
			companyBStockPriceChangeAmount = 000;
			companyCStockPriceChangeAmount = -1000;
			companyDStockPriceChangeAmount = -2000;
			companyEStockPriceChangeAmount = -1000;
		}
		if (eventNumber == 53) {
			wsprintf(messageEventTitle, TEXT("デング熱の大流行により東南アジアへの渡航が制限"));
			companyAStockPriceChangeAmount = -3000;
			companyBStockPriceChangeAmount = -3000;
			companyCStockPriceChangeAmount = -1000;
			companyDStockPriceChangeAmount = -1000;
			companyEStockPriceChangeAmount = -3000;
		}
		if (eventNumber == 54) {
			wsprintf(messageEventTitle, TEXT("ソマリアの海賊が消滅"));
			companyAStockPriceChangeAmount = 2000;
			companyBStockPriceChangeAmount = 2000;
			companyCStockPriceChangeAmount = 1000;
			companyDStockPriceChangeAmount = 1000;
			companyEStockPriceChangeAmount = 1000;
		}
		if (eventNumber == 55) {
			wsprintf(messageEventTitle, TEXT("日本の人口が増加"));
			companyAStockPriceChangeAmount = 5000;
			companyBStockPriceChangeAmount = 4000;
			companyCStockPriceChangeAmount = 4000;
			companyDStockPriceChangeAmount = 3000;
			companyEStockPriceChangeAmount = 4000;
		}
		if (eventNumber == 56) {
			wsprintf(messageEventTitle, TEXT("札幌オリンピック開催が決定"));
			companyAStockPriceChangeAmount = 4000;
			companyBStockPriceChangeAmount = 3000;
			companyCStockPriceChangeAmount = 2000;
			companyDStockPriceChangeAmount = 2000;
			companyEStockPriceChangeAmount = 4000;
		}
		if (eventNumber == 57) {
			wsprintf(messageEventTitle, TEXT("東京スカイツリーが事故により営業を停止"));
			companyAStockPriceChangeAmount = 000;
			companyBStockPriceChangeAmount = 000;
			companyCStockPriceChangeAmount = 000;
			companyDStockPriceChangeAmount = -1000;
			companyEStockPriceChangeAmount = -2000;
		}
		if (eventNumber == 58) {
			wsprintf(messageEventTitle, TEXT("ガソリン税が廃止"));
			companyAStockPriceChangeAmount = 4000;
			companyBStockPriceChangeAmount = 1000;
			companyCStockPriceChangeAmount = 1000;
			companyDStockPriceChangeAmount = 1000;
			companyEStockPriceChangeAmount = 4000;
		}
		if (eventNumber == 59) {
			wsprintf(messageEventTitle, TEXT("ディズニーランドにリニューアルオープン"));
			companyAStockPriceChangeAmount = 000;
			companyBStockPriceChangeAmount = 1000;
			companyCStockPriceChangeAmount = 0000;
			companyDStockPriceChangeAmount = 1000;
			companyEStockPriceChangeAmount = 3000;
		}
		if (eventNumber == 60) {
			wsprintf(messageEventTitle, TEXT("石油の新しい採掘方法が開発"));
			companyAStockPriceChangeAmount = 3000;
			companyBStockPriceChangeAmount = 1000;
			companyCStockPriceChangeAmount = 1000;
			companyDStockPriceChangeAmount = 1000;
			companyEStockPriceChangeAmount = 3000;
		}
		if (eventNumber == 61) {
			wsprintf(messageEventTitle, TEXT("燃費不正問題でヨーロッパの大手自動車メーカーが倒産"));
			companyAStockPriceChangeAmount = 5000;
			companyBStockPriceChangeAmount = 000;
			companyCStockPriceChangeAmount = 000;
			companyDStockPriceChangeAmount = 000;
			companyEStockPriceChangeAmount = 000;
		}
		if (eventNumber == 62) {
			wsprintf(messageEventTitle, TEXT("アメリカの大統領が広島訪問"));
			companyAStockPriceChangeAmount = 000;
			companyBStockPriceChangeAmount = 000;
			companyCStockPriceChangeAmount = 000;
			companyDStockPriceChangeAmount = 000;
			companyEStockPriceChangeAmount = 2000;
		}
		if (eventNumber == 63) {
			wsprintf(messageEventTitle, TEXT("コーヒー豆の値段が下がる"));
			companyAStockPriceChangeAmount = 000;
			companyBStockPriceChangeAmount = 000;
			companyCStockPriceChangeAmount = 2000;
			companyDStockPriceChangeAmount = 2000;
			companyEStockPriceChangeAmount = 000;
		}
		if (eventNumber == 64) {
			wsprintf(messageEventTitle, TEXT("国連が「排気ガスを40年間で50％削減する」と発表"));
			companyAStockPriceChangeAmount = 6000;
			companyBStockPriceChangeAmount = 000;
			companyCStockPriceChangeAmount = 000;
			companyDStockPriceChangeAmount = 000;
			companyEStockPriceChangeAmount = 000;
		}
		if (eventNumber == 65) {
			wsprintf(messageEventTitle, TEXT("アメリカで忍者ブーム"));
			companyAStockPriceChangeAmount = 000;
			companyBStockPriceChangeAmount = 000;
			companyCStockPriceChangeAmount = 000;
			companyDStockPriceChangeAmount = 000;
			companyEStockPriceChangeAmount = 2000;
		}
		if (eventNumber == 66) {
			wsprintf(messageEventTitle, TEXT("中国で大規模な反日デモ"));
			companyAStockPriceChangeAmount = -2000;
			companyBStockPriceChangeAmount = -3000;
			companyCStockPriceChangeAmount = 000;
			companyDStockPriceChangeAmount = 000;
			companyEStockPriceChangeAmount = -3000;
		}
		if (eventNumber == 67) {
			wsprintf(messageEventTitle, TEXT("ヨーロッパで日本のファッションが注目"));
			companyAStockPriceChangeAmount = 1000;
			companyBStockPriceChangeAmount = 5000;
			companyCStockPriceChangeAmount = 1000;
			companyDStockPriceChangeAmount = 2000;
			companyEStockPriceChangeAmount = 2000;
		}
		if (eventNumber == 68) {
			wsprintf(messageEventTitle, TEXT("新国立競技場が完成"));
			companyAStockPriceChangeAmount = 5000;
			companyBStockPriceChangeAmount = 5000;
			companyCStockPriceChangeAmount = 5000;
			companyDStockPriceChangeAmount = 5000;
			companyEStockPriceChangeAmount = 5000;
		}
		if (eventNumber == 69) {
			wsprintf(messageEventTitle, TEXT("政府が「50年間で日本の車の半分をエコカーにする」と発表"));
			companyAStockPriceChangeAmount = 7000;
			companyBStockPriceChangeAmount = 000;
			companyCStockPriceChangeAmount = 000;
			companyDStockPriceChangeAmount = 000;
			companyEStockPriceChangeAmount = 000;
		}
		if (eventNumber == 70) {
			wsprintf(messageEventTitle, TEXT("東南アジアで低価格の洋服が流行"));
			companyAStockPriceChangeAmount = 000;
			companyBStockPriceChangeAmount = 4000;
			companyCStockPriceChangeAmount = 000;
			companyDStockPriceChangeAmount = 1000;
			companyEStockPriceChangeAmount = 000;
		}
		if (eventNumber == 71) {
			wsprintf(messageEventTitle, TEXT("「チョコレートががん予防になる」という研究結果が発表"));
			companyAStockPriceChangeAmount = 000;
			companyBStockPriceChangeAmount = 000;
			companyCStockPriceChangeAmount = 3000;
			companyDStockPriceChangeAmount = 2000;
			companyEStockPriceChangeAmount = 000;
		}
		if (eventNumber == 72) {
			wsprintf(messageEventTitle, TEXT("アメリカでとんこつラーメンが大ヒット"));
			companyAStockPriceChangeAmount = 000;
			companyBStockPriceChangeAmount = 000;
			companyCStockPriceChangeAmount = 000;
			companyDStockPriceChangeAmount = 2000;
			companyEStockPriceChangeAmount = 2000;
		}
		if (eventNumber == 73) {
			wsprintf(messageEventTitle, TEXT("冷凍食品による食中毒が多発"));
			companyAStockPriceChangeAmount = 000;
			companyBStockPriceChangeAmount = 000;
			companyCStockPriceChangeAmount = 000;
			companyDStockPriceChangeAmount = -3000;
			companyEStockPriceChangeAmount = 000;
		}
		if (eventNumber == 74) {
			wsprintf(messageEventTitle, TEXT("スターウォーズの新作が公開"));
			companyAStockPriceChangeAmount = 000;
			companyBStockPriceChangeAmount = 1000;
			companyCStockPriceChangeAmount = 000;
			companyDStockPriceChangeAmount = 1000;
			companyEStockPriceChangeAmount = 1000;
		}
		if (eventNumber == 75) {
			wsprintf(messageEventTitle, TEXT("大手企業が会員の個人情報を流出"));
			companyAStockPriceChangeAmount = -2000;
			companyBStockPriceChangeAmount = -1000;
			companyCStockPriceChangeAmount = -1000;
			companyDStockPriceChangeAmount = -2000;
			companyEStockPriceChangeAmount = -2000;
		}
		if (eventNumber == 76) {
			wsprintf(messageEventTitle, TEXT("日本の化粧品の新商品がアジアでヒット"));
			companyAStockPriceChangeAmount = 000;
			companyBStockPriceChangeAmount = 000;
			companyCStockPriceChangeAmount = 000;
			companyDStockPriceChangeAmount = 2000;
			companyEStockPriceChangeAmount = 2000;
		}
		if (eventNumber == 77) {
			wsprintf(messageEventTitle, TEXT("サウジアラビアで自爆テロが多発"));
			companyAStockPriceChangeAmount = -1000;
			companyBStockPriceChangeAmount = -2000;
			companyCStockPriceChangeAmount = -1000;
			companyDStockPriceChangeAmount = -1000;
			companyEStockPriceChangeAmount = -2000;
		}
		if (eventNumber == 78) {
			wsprintf(messageEventTitle, TEXT("中国の人件費が上がる"));
			companyAStockPriceChangeAmount = -2000;
			companyBStockPriceChangeAmount = -3000;
			companyCStockPriceChangeAmount = -1000;
			companyDStockPriceChangeAmount = -1000;
			companyEStockPriceChangeAmount = -1000;
		}
		if (eventNumber == 79) {
			wsprintf(messageEventTitle, TEXT("駄菓子メーカーの賞味期限偽装が発覚"));
			companyAStockPriceChangeAmount = 000;
			companyBStockPriceChangeAmount = 000;
			companyCStockPriceChangeAmount = -5000;
			companyDStockPriceChangeAmount = -1000;
			companyEStockPriceChangeAmount = 000;
		}
		if (eventNumber == 80) {
			wsprintf(messageEventTitle, TEXT("日本のオリンピック選手が国民栄誉賞"));
			companyAStockPriceChangeAmount = 1000;
			companyBStockPriceChangeAmount = 1000;
			companyCStockPriceChangeAmount = 1000;
			companyDStockPriceChangeAmount = 1000;
			companyEStockPriceChangeAmount = 1000;
		}
		if (eventNumber == 81) {
			wsprintf(messageEventTitle, TEXT("人工知能の開発をする企業への補助金制度が決定"));
			companyAStockPriceChangeAmount = 4000;
			companyBStockPriceChangeAmount = 000;
			companyCStockPriceChangeAmount = 000;
			companyDStockPriceChangeAmount = 1000;
			companyEStockPriceChangeAmount = 1000;
		}
		if (eventNumber == 82) {
			wsprintf(messageEventTitle, TEXT("アメリカ政府が海外からの輸入を大幅に制限"));
			companyAStockPriceChangeAmount = -7000;
			companyBStockPriceChangeAmount = -1000;
			companyCStockPriceChangeAmount = -2000;
			companyDStockPriceChangeAmount = -1000;
			companyEStockPriceChangeAmount = 000;
		}
		if (eventNumber == 83) {
			wsprintf(messageEventTitle, TEXT("ガソリンが大幅に値上げ"));
			companyAStockPriceChangeAmount = -3000;
			companyBStockPriceChangeAmount = -2000;
			companyCStockPriceChangeAmount = -2000;
			companyDStockPriceChangeAmount = -3000;
			companyEStockPriceChangeAmount = -3000;
		}
		if (eventNumber == 84) {
			wsprintf(messageEventTitle, TEXT("東京に乗り物博物館がオープン"));
			companyAStockPriceChangeAmount = 1000;
			companyBStockPriceChangeAmount = 000;
			companyCStockPriceChangeAmount = 000;
			companyDStockPriceChangeAmount = 000;
			companyEStockPriceChangeAmount = 1000;
		}
		if (eventNumber == 85) {
			wsprintf(messageEventTitle, TEXT("東京でピカソ展が開催"));
			companyAStockPriceChangeAmount = 000;
			companyBStockPriceChangeAmount = 000;
			companyCStockPriceChangeAmount = 000;
			companyDStockPriceChangeAmount = 1000;
			companyEStockPriceChangeAmount = 2000;
		}
		if (eventNumber == 86) {
			wsprintf(messageEventTitle, TEXT("安全上の問題からドローンの使用が禁止に"));
			companyAStockPriceChangeAmount = -1000;
			companyBStockPriceChangeAmount = 000;
			companyCStockPriceChangeAmount = 000;
			companyDStockPriceChangeAmount = -1000;
			companyEStockPriceChangeAmount = 000;
		}
		if (eventNumber == 87) {
			wsprintf(messageEventTitle, TEXT("大雪で成田空港の飛行機が1日中欠航"));
			companyAStockPriceChangeAmount = 000;
			companyBStockPriceChangeAmount = 000;
			companyCStockPriceChangeAmount = 000;
			companyDStockPriceChangeAmount = 000;
			companyEStockPriceChangeAmount = -4000;
		}
		if (eventNumber == 88) {
			wsprintf(messageEventTitle, TEXT("箱根で火山が噴火"));
			companyAStockPriceChangeAmount = 000;
			companyBStockPriceChangeAmount = 000;
			companyCStockPriceChangeAmount = 000;
			companyDStockPriceChangeAmount = 000;
			companyEStockPriceChangeAmount = -4000;
		}
		if (eventNumber == 89) {
			wsprintf(messageEventTitle, TEXT("韓国の大統領が竹島に上陸"));
			companyAStockPriceChangeAmount = -1000;
			companyBStockPriceChangeAmount = -1000;
			companyCStockPriceChangeAmount = 000;
			companyDStockPriceChangeAmount = 000;
			companyEStockPriceChangeAmount = -3000;
		}
		if (eventNumber == 90) {
			wsprintf(messageEventTitle, TEXT("北海道新幹線が開通"));
			companyAStockPriceChangeAmount = 000;
			companyBStockPriceChangeAmount = 000;
			companyCStockPriceChangeAmount = 000;
			companyDStockPriceChangeAmount = 000;
			companyEStockPriceChangeAmount = 3000;
		}
		if (eventNumber == 91) {
			wsprintf(messageEventTitle, TEXT("マグロが例年の2倍発生"));
			companyAStockPriceChangeAmount = 000;
			companyBStockPriceChangeAmount = 000;
			companyCStockPriceChangeAmount = 000;
			companyDStockPriceChangeAmount = 1000;
			companyEStockPriceChangeAmount = 2000;
		}
		if (eventNumber == 92) {
			wsprintf(messageEventTitle, TEXT("フィリピンで軍事クーデターが発生"));
			companyAStockPriceChangeAmount = -1000;
			companyBStockPriceChangeAmount = -2000;
			companyCStockPriceChangeAmount = -1000;
			companyDStockPriceChangeAmount = -1000;
			companyEStockPriceChangeAmount = -1000;
		}
		if (eventNumber == 93) {
			wsprintf(messageEventTitle, TEXT("養殖によるウナギの大量生産が始まる"));
			companyAStockPriceChangeAmount = 000;
			companyBStockPriceChangeAmount = 000;
			companyCStockPriceChangeAmount = 000;
			companyDStockPriceChangeAmount = 1000;
			companyEStockPriceChangeAmount = 2000;
		}
		if (eventNumber == 94) {
			wsprintf(messageEventTitle, TEXT("名古屋港が2倍に広がる"));
			companyAStockPriceChangeAmount = 6000;
			companyBStockPriceChangeAmount = 000;
			companyCStockPriceChangeAmount = 000;
			companyDStockPriceChangeAmount = 000;
			companyEStockPriceChangeAmount = 000;
		}
		if (eventNumber == 95) {
			wsprintf(messageEventTitle, TEXT("明日はバレンタインデーだ"));
			companyAStockPriceChangeAmount = 000;
			companyBStockPriceChangeAmount = 000;
			companyCStockPriceChangeAmount = 3000;
			companyDStockPriceChangeAmount = 2000;
			companyEStockPriceChangeAmount = 000;
		}
		if (eventNumber == 96) {
			wsprintf(messageEventTitle, TEXT("牛乳の価格が下がる"));
			companyAStockPriceChangeAmount = 000;
			companyBStockPriceChangeAmount = 000;
			companyCStockPriceChangeAmount = 1000;
			companyDStockPriceChangeAmount = 1000;
			companyEStockPriceChangeAmount = 000;
		}
		if (eventNumber == 97) {
			wsprintf(messageEventTitle, TEXT("牛乳の価格が上がる"));
			companyAStockPriceChangeAmount = 000;
			companyBStockPriceChangeAmount = 000;
			companyCStockPriceChangeAmount = -1000;
			companyDStockPriceChangeAmount = -1000;
			companyEStockPriceChangeAmount = 000;
		}
		if (eventNumber == 98) {
			wsprintf(messageEventTitle, TEXT("本州と九州を結ぶ新しい橋が完成"));
			companyAStockPriceChangeAmount = 000;
			companyBStockPriceChangeAmount = 000;
			companyCStockPriceChangeAmount = 1000;
			companyDStockPriceChangeAmount = 1000;
			companyEStockPriceChangeAmount = 3000;
		}
		NE = eventNumber;

		if (eventNumber == 1000) {
			wsprintf(messageEventTitle, TEXT("最後の株価を表示しています。"));
		}
		wsprintf(messageEventNumber, TEXT("\n\n\n\n\n\nイベント番号；%d"), eventNumber);
	}



	return;
}

/**
* それぞれの株価の表示を担う関数
* param@ A:企業の番号(1~5), hWnd:ウィンドウハンドル, rc:画面全体のビットマップ長方形, hdc:ウィンドウコンテクスト
*/

void Ftime(const int A, const RECT rc, const HDC hdc) {

	HPEN hPen, hPen2, hPen3;
	HBRUSH hBrush, hBrushW, hBrushT;
	RECT rc0{}, rc1{}, companyNameTitleRectangle{}, companyStockPriceGraphRectangle{}, timerRectangle{}, eventTitleRectangle{};
	TCHAR companyName[10]{};
	TCHAR companyStockPrice[10]{};
	TCHAR maxCompanyStockPrice[10]{};
	TCHAR minCompanyStockPrice[10]{};

	hPen = CreatePen(PS_SOLID, 0, RGB(0, 0, 0));
	hBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
	hBrushW = (HBRUSH)GetStockObject(WHITE_BRUSH);
	hBrushT = (HBRUSH)GetStockObject(DC_BRUSH);
	hPen3 = (HPEN)GetStockObject(NULL_PEN);
	hPen2 = CreatePen(PS_SOLID, 4, RGB(0, 0, 0));
	int companyNameTitleColorRed = 0, companyNameTitleColorGreen = 0, companyNameTitleColorBlue = 0;

	if (A != 6) {
		SelectObject(hdc, hBrush);
		if (A == 1) {
			rc0 = RECT{ 0, rc.bottom / 16, rc.right / cellNumber, rc.bottom / 9 };
			rc1 = RECT{ rc0.right, rc0.top, rc.right / cellNumber * 2, rc0.bottom };
			companyNameTitleRectangle = RECT{ 2, 2, rc1.right, rc0.top - rc.bottom / 80 };
			companyStockPriceGraphRectangle = RECT{ rc0.left + rc.right / 40, rc0.bottom + rc.bottom / 40, rc1.right - rc.right / 40, rc.bottom / 2 - rc.bottom / 40 };
			wsprintf((LPWSTR)companyName, TEXT("トヨタ自動車"));
			companyNameTitleColorRed = 204;
		}
		if (A == 2) {
			rc0 = RECT{ rc.right * 2 / 3, rc.bottom / 16, rc.right * 2 / 3 + rc.right / cellNumber, rc.bottom / 9 };
			rc1 = RECT{ rc.right * 2 / 3 + rc.right / cellNumber, rc.bottom / 16, rc.right * 2 / 3 + rc.right / cellNumber * 2, rc0.bottom };
			companyNameTitleRectangle = RECT{ 2 + rc.right * 2 / 3,	2, rc1.right, rc0.top - rc.bottom / 80 };
			companyStockPriceGraphRectangle = RECT{ rc0.left + rc.right / 40, rc0.bottom + rc.bottom / 40, rc1.right - rc.right / 40, rc.bottom / 2 - rc.bottom / 40 };
			wsprintf((LPWSTR)companyName, TEXT("ユニクロ"));
			companyNameTitleColorRed = 255;
			companyNameTitleColorGreen = 255;
		}
		if (A == 3) {
			rc0 = RECT{ 0, rc.bottom / 16 + rc.bottom / 2, rc.right / cellNumber, rc.bottom / 9 + rc.bottom / 2 };
			rc1 = RECT{ rc0.right, rc.bottom / 16 + rc.bottom / 2, rc.right / cellNumber * 2, rc.bottom / 9 + rc.bottom / 2 };
			companyNameTitleRectangle = RECT{ 2, 2 + rc.bottom / 2, rc1.right, rc0.top - rc.bottom / 80 };
			companyStockPriceGraphRectangle = RECT{ rc0.left + rc.right / 40, rc0.bottom + rc.bottom / 40, rc1.right - rc.right / 40, rc.bottom / 2 - rc.bottom / 40 + rc.bottom / 2 };
			wsprintf((LPWSTR)companyName, TEXT("森永製菓"));
			companyNameTitleColorRed = 102;
			companyNameTitleColorGreen = 255;
			companyNameTitleColorBlue = 255;
		}
		if (A == 4) {
			rc0 = RECT{ 0 + rc.right / 3, rc.bottom / 16 + rc.bottom / 2, rc.right / cellNumber + rc.right / 3, rc.bottom / 9 + rc.bottom / 2 };
			rc1 = RECT{ rc0.right, rc0.top, rc.right / cellNumber * 2 + rc.right / 3, rc0.bottom };
			companyNameTitleRectangle = RECT{ 2 + rc.right / 3, 2 + rc.bottom / 2, rc1.right, rc0.top - rc.bottom / 80 };
			companyStockPriceGraphRectangle = RECT{ rc0.left + rc.right / 40, rc0.bottom + rc.bottom / 40, rc1.right - rc.right / 40, rc.bottom / 2 - rc.bottom / 40 + rc.bottom / 2 };
			wsprintf((LPWSTR)companyName, TEXT("イトーヨーカドー"));
			companyNameTitleColorRed = 51;
			companyNameTitleColorGreen = 204;
		}
		if (A == 5) {
			rc0 = RECT{ rc.right * 2 / 3, rc.bottom / 16 + rc.bottom / 2, rc.right * 2 / 3 + rc.right / cellNumber, rc.bottom / 9 + rc.bottom / 2 };
			rc1 = RECT{ rc.right * 2 / 3 + rc.right / cellNumber, rc0.top, rc.right * 2 / 3 + rc.right / cellNumber * 2, rc0.bottom };
			companyNameTitleRectangle = RECT{ 2 + rc.right * 2 / 3, 2 + rc.bottom / 2, rc1.right, rc0.top - rc.bottom / 80 };
			companyStockPriceGraphRectangle = RECT{ rc0.left + rc.right / 40, rc0.bottom + rc.bottom / 40, rc1.right - rc.right / 40, rc.bottom / 2 - rc.bottom / 40 + rc.bottom / 2 };
			wsprintf((LPWSTR)companyName, TEXT("ＪＴＢ"));
			companyNameTitleColorRed = 211;
			companyNameTitleColorGreen = 211;
			companyNameTitleColorBlue = 211;
		}
		SetDCBrushColor(hdc, RGB(companyNameTitleColorRed, companyNameTitleColorGreen, companyNameTitleColorBlue));

		companyNameTitleRectangle.left += rc.right / 15;
		companyNameTitleRectangle.top += rc.bottom / 100;
		companyNameTitleRectangle.right -= rc.right / 15;
		companyNameTitleRectangle.bottom += rc.bottom / 200;
		
		wsprintf((LPWSTR)companyStockPrice, TEXT("株価：%d"), stockPrices[A - 1][his - 1]);
		wsprintf((LPWSTR)maxCompanyStockPrice, eachCompanyMaxStockPriceInPeriod[A * 2 - 2]);	//maxStockPriceInPeriod
		wsprintf((LPWSTR)minCompanyStockPrice, eachCompanyMaxStockPriceInPeriod[A * 2 - 2]);	//minStockPriceInPeriod

		/*初期の枠を作るスクリプト開始*/

		for (int i = 0; i < his; i++) {
			stockPriceChartGraphPoint[A - 1][i].x = (companyStockPriceGraphRectangle.right - companyStockPriceGraphRectangle.left) / (his) * (i + 1) + companyStockPriceGraphRectangle.left - rc.right / 300;
		}

		SelectObject(hdc, hPen);
		for (int i = 0; i < 2; i++) {
			for (int j = 0; j < cellNumber / 2; j++) {
				Rectangle(hdc, rc.right * j * 2 / cellNumber, rc.bottom * i / 2, rc.right * (j + 1) * 2 / cellNumber, rc.bottom * (i + 1) / 2);
			}
		}

		/*初期の枠を作るスクリプト終了*/
		/*数字処理start*/

		SelectObject(hdc, hBrushW);
		SelectObject(hdc, hPen3);

		RECT rc40{};
		rc40.left = (rc0.left + rc1.left) / 2;
		rc40.top = (rc0.top + rc1.top) / 2;
		rc40.right = (rc0.right + rc1.right) / 2;
		rc40.bottom = (rc0.bottom + rc1.bottom) / 2;

		RECT rc41{};
		rc41.left = rc40.left;
		rc41.top = rc40.top + rc.bottom / 100;
		rc41.right = rc40.right;
		rc41.bottom = rc40.bottom;

		SelectObject(hdc, hBrushT);

		RoundRect(hdc, companyNameTitleRectangle.left, companyNameTitleRectangle.top - rc.bottom / 200, companyNameTitleRectangle.right, companyNameTitleRectangle.bottom, (rc.right + rc.bottom) / 50, (rc.right + rc.bottom) / 50);

		SelectObject(hdc, hPen);

		SelectObject(hdc, hFont6);

		SelectObject(hdc, hBrush);

		SetTextColor(hdc, RGB(0, 0, 0));

		DrawText(hdc, companyName, -1, &companyNameTitleRectangle, DT_CENTER);


		SelectObject(hdc, hBrushW);

		SelectObject(hdc, hPen);

		Rectangle(hdc, companyStockPriceGraphRectangle.left, companyStockPriceGraphRectangle.top, companyStockPriceGraphRectangle.right, companyStockPriceGraphRectangle.bottom);

		SetTextColor(hdc, RGB(0, 0, 255));
		DrawText(hdc, companyStockPrice, -1, &rc41, DT_CENTER);

		/*数字処理end*/

		SelectObject(hdc, hPen2);

		Polyline(hdc, stockPriceChartGraphPoint[A - 1], stockPriceChartGraphPointNumber);
		SelectObject(hdc, hFont3);
		SetTextColor(hdc, RGB(255, 102, 51));
		TextOut(hdc, companyStockPriceGraphRectangle.right, companyStockPriceGraphRectangle.top, maxCompanyStockPrice, lstrlen(maxCompanyStockPrice));
		SetTextColor(hdc, RGB(102, 102, 255));
		TextOut(hdc, companyStockPriceGraphRectangle.right, companyStockPriceGraphRectangle.bottom - 20, minCompanyStockPrice, lstrlen(minCompanyStockPrice));

	}
	if (A == 6) {
		rc0.left = 0;
		rc0.top = rc.bottom / 16;
		rc0.right = rc.right / cellNumber;
		rc0.bottom = rc.bottom / 9;

		rc1.left = rc0.right;
		rc1.top = rc0.top;
		rc1.right = rc.right / cellNumber * 2;
		rc1.bottom = rc0.bottom;

		companyNameTitleRectangle.left = 2;
		companyNameTitleRectangle.top = 2;
		companyNameTitleRectangle.right = rc1.right;
		companyNameTitleRectangle.bottom = rc0.top - rc.bottom / 80;

		companyStockPriceGraphRectangle.left = rc0.left + rc.right / 80;
		companyStockPriceGraphRectangle.top = rc0.bottom + rc.bottom / 40;
		companyStockPriceGraphRectangle.right = rc1.right - rc.right / 80;
		companyStockPriceGraphRectangle.bottom = rc.bottom / 2 - rc.bottom / 40;

		timerRectangle.left = rc1.right + rc.right / 20;
		timerRectangle.top = rc.bottom / 60;
		timerRectangle.right = rc.right / cellNumber * 2 * 2 - rc.right / 100;
		timerRectangle.bottom = rc0.top;
		
		/*時計処理start*/
		TCHAR sur[10];
		SelectObject(hdc, hFont2);
		wsprintf(sur, TEXT("%3d"), currentRemainingTime / 1000);

		SetTextColor(hdc, RGB(0, 51, 0));
		TextOut(hdc, rc1.right + rc.right / 50, timerRectangle.top + timerRectangle.top * 3 / 4, sur, lstrlen(sur));
		SelectObject(hdc, hPen);
		SelectObject(hdc, hBrushW);
		Rectangle(hdc, timerRectangle.left, timerRectangle.top, timerRectangle.right, timerRectangle.bottom);
		hBrush = CreateSolidBrush(RGB(204, 255, 102));
		SelectObject(hdc, hBrush);

		Rectangle(hdc, timerRectangle.left, timerRectangle.top, timerRectangle.right, timerRectangle.bottom);

		SelectObject(hdc, hBrushW);

		Rectangle(hdc, timerRectangle.right - (limitedTime - currentRemainingTime) * (timerRectangle.right - timerRectangle.left) / limitedTime, timerRectangle.top, timerRectangle.right, timerRectangle.bottom);

		/*時計処理end*/
		/*ニュース処理start*/
		SelectObject(hdc, hFont1);
		SetTextColor(hdc, RGB(153, 102, 0));
		TextOut(hdc, rc1.right + rc.right / 50, timerRectangle.top + timerRectangle.top * 3, TEXT("～ニュース～"), lstrlen(TEXT("～ニュース～")));

		eventTitleRectangle.left = rc0.left + rc.right / 160 + rc.right / 3;
		eventTitleRectangle.top = rc0.bottom + rc.bottom / 40;
		eventTitleRectangle.right = rc1.right - rc.right / 32 + rc.right / 3;
		eventTitleRectangle.bottom = rc.bottom / 2 - rc.bottom / 10;

		SelectObject(hdc, hFont6);
		DrawText(hdc, messageEventTitle, lstrlen(messageEventTitle), &eventTitleRectangle, DT_CENTER | DT_WORDBREAK);

		SelectObject(hdc, hFont3);
		DrawText(hdc, messageEventNumber, lstrlen(messageEventNumber), &eventTitleRectangle, DT_CENTER | DT_WORDBREAK);
	}
	if (A != 6) {
		if (AS[A - 1] > 0) {
			SelectObject(hdc, hBrushw);
			Rectangle(hdc, rc0.left, rc0.top, rc1.right, companyStockPriceGraphRectangle.bottom);
			SelectObject(hdc, hFont2);
			wsprintf((LPWSTR)messageCompanyBankruptcy, TEXT("\nこの会社は倒産しました。\n株のカードをボードに戻してください。"));
			DrawText(hdc, messageCompanyBankruptcy, -1, &companyStockPriceGraphRectangle, DT_CENTER);
		}
	}

	DeleteObject(hBrush);
	DeleteObject(hBrushW);
	DeleteObject(hPen);
	DeleteObject(hPen2);
	return;
}
