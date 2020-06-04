#include<windows.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<MMsystem.h>
#include"_GuitarEffector.h"
#include"resource.h"
#include"ResEffectDialog.h"

/* プロトタイプ */
LRESULT CALLBACK WndProc( HWND, UINT, WPARAM, LPARAM );
BOOL WndMenu( HWND, UINT, WPARAM, LPARAM );
BOOL CALLBACK DistDlgProc( HWND hDlg, UINT msg, WPARAM wp, LPARAM lp );
BOOL CALLBACK OverDriveDlgProc( HWND hDlg, UINT msg, WPARAM wp, LPARAM lp );
BOOL CALLBACK DelayDlgProc( HWND hDlg, UINT msg, WPARAM wp, LPARAM lp );
BOOL CALLBACK ColasDlgProc( HWND hDlg, UINT msg, WPARAM wp, LPARAM lp );
BOOL CALLBACK ReverbDlgProc( HWND hDlg, UINT msg, WPARAM wp, LPARAM lp );
BOOL CALLBACK VibrateDlgProc( HWND hDlg, UINT msg, WPARAM wp, LPARAM lp );
BOOL CALLBACK CompDlgProc( HWND hDlg, UINT msg, WPARAM wp, LPARAM lp );

#include"_GetWavInfoHead.cpp"
#include"_GetWaveData.cpp"
#include"_SetWaveFile.cpp"
#include"File.cpp"
#include"_Common.cpp"
#include"_ShowWave.cpp"

#include"_Effector.cpp"

/***********/
/* WinMain */
/***********************************************************************/
int WINAPI WinMain( HINSTANCE hCurInst, HINSTANCE hPrevInst, LPSTR lpsCmdLine, int nCmdShow ){
	
	MSG msg;
	BOOL bRet;
	WNDCLASSEX wc;
	HWND hWnd;
	
	hInst = hCurInst;
	
	/* WindowClass定義 */
	wc.cbSize = sizeof( WNDCLASSEX );
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WndProc;		//プロシージャ名
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hCurInst;		//インスタンス
	wc.hIcon = (HICON)LoadImage(NULL,
		   MAKEINTRESOURCE(IDI_APPLICATION),
		   IMAGE_ICON,
		   0, 0,
		   LR_DEFAULTSIZE | LR_SHARED );
	wc.hCursor = (HCURSOR)LoadImage( NULL,
		      MAKEINTRESOURCE(IDC_ARROW),
		      IMAGE_CURSOR,
		      0, 0,
		      LR_DEFAULTSIZE | LR_SHARED );
	wc.hbrBackground = (HBRUSH)GetStockObject( WHITE_BRUSH );
	wc.lpszMenuName = "MYMENU";			//メニュー名
	wc.lpszClassName = (LPCSTR)szClassName;
	wc.hIconSm = (HICON)LoadImage( NULL,
		     MAKEINTRESOURCE(IDI_APPLICATION),
		     IMAGE_ICON,
		     0, 0,
		     LR_DEFAULTSIZE | LR_SHARED );
		     
	if( RegisterClassEx( &wc ) == 0 ) return FALSE;
	
	/* Window作成 */
	hWnd = CreateWindow( szClassName,
			     TITLE,
			     WS_OVERLAPPEDWINDOW,	//ウィンドゥの種類
			     CW_USEDEFAULT,		//x座標
			     CW_USEDEFAULT,		//y座標
			     WINSIZE_X,			//幅
			     WINSIZE_Y,			//高さ
			     NULL,			//親ウィンドゥのハンドル
			     NULL,			//メニューハンドル
			     hCurInst,			//インスタンスハンドル
			     NULL );
	if( !hWnd ) return FALSE;
	
	/* Window表示 */
	ShowWindow( hWnd, nCmdShow );
	UpdateWindow( hWnd );
	while( ( bRet = GetMessage( &msg, NULL, 0, 0  ) ) != 0 ){
		if( bRet == -1 ){
			break;
		} else {
			TranslateMessage( &msg );
			DispatchMessage( &msg );
		}
	}
	
	return (int)msg.wParam;

}

/****************/
/* プロシージャ */
/***********************************************************************/
LRESULT CALLBACK WndProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam ){
	
	switch( msg ){
		
		case WM_CREATE:
			_SetWinCenter( hWnd ); 
			break;
		
		case WM_PAINT:
			HDC hdc, hdcm;
			PAINTSTRUCT ps;
			HBITMAP hBmpDC;
				
			hdc = BeginPaint( hWnd, &ps );
			hdcm = CreateCompatibleDC( hdc );
			
			_StartShowWave( hdc );
			
			/* メモリデバイスコンテキスト */
			hBmpDC = CreateCompatibleBitmap( hdc, WINSIZE_X, WINSIZE_Y );
			SelectObject( hdcm, hBmpDC );
			
			if( flagWaveShow == true ){
				_BackPaint( hdc );
				_WavePaint( hdc, spl, times );
			} else {
				_BackPaint( hdc );
				_DrawLine( hdcm, 0, WINSIZE_Y/2, WINSIZE_X, WINSIZE_Y/2, 128, 128, 128 );
			}
			_SetDCm( hdcm );
			
			/* 表示 */
			BitBlt( hdc, 0, 0, WINSIZE_X, WINSIZE_Y,
				  hdcm, 0, 0, SRCCOPY );
			
			
			DeleteDC( hdcm );
			DeleteObject( hBmpDC );
			EndPaint( hWnd, &ps );
			
			_EndShowWave();
			
			
			break;
			
		case WM_CHAR:
			if( wParam == 54 ){
				times+=500;
				InvalidateRect(hWnd, NULL, TRUE);
			} else if( wParam == 52 ){
				times-=500;
				if( times < 0 ) times = 1;
				InvalidateRect(hWnd, NULL, TRUE);
			} else if( wParam == 50 ){
				spl-=1;
				if( spl < 1 ) spl = 1;
				InvalidateRect(hWnd, NULL, TRUE);
			} else if( wParam == 56 ){
				spl+=1;
				InvalidateRect(hWnd, NULL, TRUE);
			} else {
				return (DefWindowProc(hWnd, msg, wParam, lParam));
			}
			wsprintf( szTitle, "%s sample=%d time=%d", TITLE, spl, times );
			SetWindowText( hWnd, szTitle );
			break;
		
		/*case WM_MOUSEMOVE:
			char szBuf[64];
			POINTS p;
			
			p = MAKEPOINTS(lParam);
			wsprintf( szBuf, "x: %d, y: %d", p.x, p.y );
			SetWindowText( hWnd, szBuf );
			break;
		*/
		case WM_COMMAND:
			WndMenu( hWnd, msg, wParam, lParam );
			break;
		
		case WM_CLOSE:
			if( (MessageBox(hWnd, "Exit?", "Exit", MB_YESNO | MB_ICONQUESTION)) == IDYES )
				DestroyWindow( hWnd );
			break;
		
		case WM_DESTROY:
			PostQuitMessage( 0 );
			break;
		
		default:
			return ( DefWindowProc( hWnd, msg, wParam, lParam ) );
	}

	return 0;

}

/************/
/* メニュー */
/***********************************************************************/
BOOL WndMenu( HWND hWnd, UINT msg, WPARAM wp, LPARAM lp ){

	OPENFILENAME ofn;
	hWindow = hWnd;
	
	switch( LOWORD(wp) ){
		
		case IDM_OPEN:
			ofn = _FileOpenDialog(hWnd);
			if( ErrorFlag == false ) {
				_FileOpen( hWnd, ofn );
				InvalidateRect(hWnd, NULL, FALSE);
			} ErrorFlag = false;
			flagWaveEff = false;
			break;
		
		case IDM_UNDO:
			memcpy( dEffDataL, dOldEffDataL, 4*tagWaveinfo.DataNum );
			InvalidateRect(hWnd, NULL, TRUE);
			_SetWaveFile( "temp.wav", dEffDataL, dWaveDataR, &tagWaveinfo );
			break;
			
		case IDM_RELOAD:
			memcpy( dEffDataL, dWaveDataL, 4*tagWaveinfo.DataNum );
			InvalidateRect(hWnd, NULL, TRUE);
			_SetWaveFile( "temp.wav", dEffDataL, dWaveDataR, &tagWaveinfo );
			break;
					
		case IDM_ORIGNPLAY:
			if( flagWaveShow == true ){
				sndPlaySound( NULL, SND_ASYNC );
				sndPlaySound( szFileName, SND_ASYNC );
			}
			break;
			
		case IDM_EFFPLAY:
			if( flagWaveEff == true ){
				sndPlaySound( NULL, SND_ASYNC );
				sndPlaySound( szEffFileName, SND_ASYNC );
			}
			break;
			
		case IDM_STOP:
			sndPlaySound( NULL, SND_ASYNC );
			break;
		
		case IDM_END:
			SendMessage( hWnd, WM_CLOSE, 0, 0 );
			break;
		
		case IDM_HELP:
			MessageBox( hWnd, szHELP, "About", MB_OK );
			break;
			
		case IDM_DELAY_1:
			DialogBox( hInst, "DLG_DELAY", hWindow, (DLGPROC)DelayDlgProc );
			break;
			
		case IDM_DIST_1:
		      DialogBox( hInst, "DLG_DIST", hWindow, (DLGPROC)DistDlgProc );
			break;
			
		case IDM_OD:
			DialogBox( hInst, "DLG_OD", hWindow, (DLGPROC)OverDriveDlgProc );
			break;
			
		case IDM_COLAS_1:
			DialogBox( hInst, "DLG_COLAS", hWindow, (DLGPROC)ColasDlgProc );
			break;
			
		case IDM_REVERB_1:
			DialogBox( hInst, "DLG_REVERB", hWindow, (DLGPROC)ReverbDlgProc );
			break;
		
		case IDM_VIBRATE:
			DialogBox( hInst, "DLG_VIB", hWindow, (DLGPROC)VibrateDlgProc );
			break;
			
		case IDM_COMP:
			DialogBox( hInst, "DLG_COMP", hWindow, (DLGPROC)CompDlgProc );
			break;
				
		case IDM_ROBO:
			_EffRobo( &tagWaveinfo, dEffDataL, dOldEffDataL );
			InvalidateRect(hWnd, NULL, TRUE);
			flagWaveEff=true;
			break;
		
		default:
			return ( DefWindowProc( hWnd, msg, wp, lp ) );	
	}
}

/**************************/
/* ダイアログプロシージャ */
/***********************************************************************/
BOOL CALLBACK DistDlgProc( HWND hDlg, UINT msg, WPARAM wp, LPARAM lp ){
	
	int level, gain;
	
	switch( msg ){
		case WM_COMMAND:
		switch( LOWORD(wp) ){
			case DIST_OK:
				level = GetDlgItemInt( hDlg ,DIST_LEVEL, NULL, FALSE );
				gain = GetDlgItemInt( hDlg ,DIST_GAIN, NULL, FALSE );
				{
				char str[30];
				wsprintf( str, "level: %d, gain: %d", level, gain );
				MessageBox( NULL, str, "check", MB_OK );
				}
				_EffDist( &tagWaveinfo, dEffDataL, dOldEffDataL, level, gain );
				InvalidateRect(hWindow, NULL, TRUE);
				flagWaveEff=true;
				EndDialog( hDlg, DIST_OK );
				return TRUE;
			case DIST_CANCEL:
				EndDialog( hDlg, DIST_CANCEL );
				return FALSE;
		}
		return FALSE;
	}
	return FALSE;
}

BOOL CALLBACK OverDriveDlgProc( HWND hDlg, UINT msg, WPARAM wp, LPARAM lp ){
	
	int level, gain;
	
	switch( msg ){
		case WM_COMMAND:
		switch( LOWORD(wp) ){
			case OD_OK:
				level = GetDlgItemInt( hDlg ,OD_LEVEL, NULL, FALSE );
				gain = GetDlgItemInt( hDlg ,OD_GAIN, NULL, FALSE );
				{
				char str[30];
				wsprintf( str, "level: %d, gain: %d", level, gain );
				MessageBox( NULL, str, "check", MB_OK );
				}
				_EffOverDrive( &tagWaveinfo, dEffDataL, dOldEffDataL, level, gain );
				InvalidateRect(hWindow, NULL, TRUE);
				flagWaveEff=true;
				EndDialog( hDlg, OD_OK );
				return TRUE;
			case OD_CANCEL:
				EndDialog( hDlg, OD_CANCEL );
				return FALSE;
		}
		return FALSE;
	}
	return FALSE;
}


BOOL CALLBACK DelayDlgProc( HWND hDlg, UINT msg, WPARAM wp, LPARAM lp ){
	
	int times;
	int feedback;
	
	switch( msg ){
		case WM_COMMAND:
		switch( LOWORD(wp) ){
			case DELAY_OK:
				times = GetDlgItemInt( hDlg ,DELAY_TIME, NULL, FALSE );
				feedback = GetDlgItemInt( hDlg ,DELAY_FEEDBACK, NULL, FALSE );
				{
				char str[30];
				wsprintf( str, "time: %d, feedback: %d", times, feedback );
				MessageBox( NULL, str, "check", MB_OK );
				}
				_EffDelay( &tagWaveinfo, dEffDataL, dOldEffDataL, times, feedback );
				InvalidateRect(hWindow, NULL, TRUE);
				flagWaveEff=true;
				EndDialog( hDlg, DELAY_OK );
				return TRUE;
			case DELAY_CANCEL:
				EndDialog( hDlg, DELAY_CANCEL );
				return FALSE;
		}
		return FALSE;
	}
	return FALSE;
}

BOOL CALLBACK ColasDlgProc( HWND hDlg, UINT msg, WPARAM wp, LPARAM lp ){
	
	int times;
	
	switch( msg ){
		case WM_COMMAND:
		switch( LOWORD(wp) ){
			case COLAS_OK:
				times = GetDlgItemInt( hDlg ,COLAS_TIME, NULL, FALSE );
				{
				char str[30];
				wsprintf( str, "time: %d", times );
				MessageBox( NULL, str, "check", MB_OK );
				}
				_EffColas( &tagWaveinfo, dEffDataL, dOldEffDataL, times );
				InvalidateRect(hWindow, NULL, TRUE);
				flagWaveEff=true;
				EndDialog( hDlg, COLAS_OK );
				return TRUE;
			case COLAS_CANCEL:
				EndDialog( hDlg, COLAS_CANCEL );
				return FALSE;
		}
		return FALSE;
	}
	return FALSE;
}

BOOL CALLBACK ReverbDlgProc( HWND hDlg, UINT msg, WPARAM wp, LPARAM lp ){
	
	int times;
	int mix;
	
	switch( msg ){
		case WM_COMMAND:
		switch( LOWORD(wp) ){
			case REVERB_OK:
				times = GetDlgItemInt( hDlg ,REVERB_TIME, NULL, FALSE );
				mix = GetDlgItemInt( hDlg ,REVERB_MIX, NULL, FALSE );
				{
				char str[30];
				wsprintf( str, "time: %d, mix: %d", times, mix );
				MessageBox( NULL, str, "check", MB_OK );
				}
				_EffReverb( &tagWaveinfo, dEffDataL, dOldEffDataL, times, mix );
				InvalidateRect(hWindow, NULL, TRUE);
				flagWaveEff=true;
				EndDialog( hDlg, REVERB_OK );
				return TRUE;
			case REVERB_CANCEL:
				EndDialog( hDlg, REVERB_CANCEL );
				return FALSE;
		}
		return FALSE;
	}
	return FALSE;
}

BOOL CALLBACK VibrateDlgProc( HWND hDlg, UINT msg, WPARAM wp, LPARAM lp ){
	
	int times;
	
	switch( msg ){
		case WM_COMMAND:
		switch( LOWORD(wp) ){
			case VIB_OK:
				times = GetDlgItemInt( hDlg ,VIB_TIME, NULL, FALSE );
				{
				char str[30];
				wsprintf( str, "time: %d", times );
				MessageBox( NULL, str, "check", MB_OK );
				}
				_EffVibrate( &tagWaveinfo, dEffDataL, dOldEffDataL, times );
				InvalidateRect(hWindow, NULL, TRUE);
				flagWaveEff=true;
				EndDialog( hDlg, VIB_OK );
				return TRUE;
			case VIB_CANCEL:
				EndDialog( hDlg, VIB_CANCEL );
				return FALSE;
		}
		return FALSE;
	}
	return FALSE;
}

BOOL CALLBACK CompDlgProc( HWND hDlg, UINT msg, WPARAM wp, LPARAM lp ){
	
	int times;
	
	switch( msg ){
		case WM_COMMAND:
		switch( LOWORD(wp) ){
			case COMP_OK:
				times = GetDlgItemInt( hDlg ,COMP_LEVEL, NULL, FALSE );
				{
				char str[30];
				wsprintf( str, "level: %d", times );
				MessageBox( NULL, str, "check", MB_OK );
				}
				_EffComp( &tagWaveinfo, dEffDataL, dOldEffDataL, times );
				InvalidateRect(hWindow, NULL, TRUE);
				flagWaveEff=true;
				EndDialog( hDlg, COMP_OK );
				return TRUE;
			case COMP_CANCEL:
				EndDialog( hDlg, COMP_CANCEL );
				return FALSE;
		}
		return FALSE;
	}
	return FALSE;
}