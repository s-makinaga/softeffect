/* 関数使う前に呼び出す */
void _StartShowWave( HDC hdc ){
	
	hdcBack = CreateCompatibleDC( hdc );
	hdcWave = CreateCompatibleDC( hdc );
}

/* 関数使った後必ず呼び出す */
void _EndShowWave(){
	
	DeleteDC( hdcBack );
	DeleteDC( hdcWave );
}

/* hdcmに全部描画させとく */
void _SetDCm( HDC hdc ){
	
	/* メモリデバスコンテキストに描画 */
	BitBlt( hdc, 0, 0, WINSIZE_X, WINSIZE_Y,
		hdcBack, 0, 0, SRCCOPY );
	BitBlt( hdc, 0, 0, WINSIZE_X, WINSIZE_Y,
		hdcWave, 1, 1, SRCCOPY );
}


/* 全体のデザイン描画 */
void _BackPaint( HDC hdc ){
	
	static HBITMAP hBmpDC;
	
	hBmpDC = CreateCompatibleBitmap( hdc, WINSIZE_X, WINSIZE_Y );
	SelectObject( hdcBack, hBmpDC );
	
	static HPEN hPen, hOldPen;
	static HBRUSH hBrush, hOldBrush;
	
	hPen = CreatePen( PS_SOLID, 0, RGB( 255, 255, 255 ) );
	hOldPen = (HPEN)SelectObject( hdcBack, hPen );
	hBrush = CreateSolidBrush( RGB( 255, 255, 255 ) );
	hOldBrush = (HBRUSH)SelectObject( hdcBack, hBrush );
	Rectangle( hdcBack, 0, 0, WINSIZE_X, WINSIZE_Y );
	
	_DrawLine( hdcBack, 0, WINSIZE_Y/2, WINSIZE_X, WINSIZE_Y/2, 0, 0, 0 );
	
	DeleteObject( hPen );
	DeleteObject( hBrush );
	DeleteObject( hBmpDC );
	
}

/* 波形の表示 */
/* a = サンプル感覚*/
/* b = 時間 */
void _WavePaint( HDC hdc, int a, int b ){
	
	static HBITMAP hBmpDC;
	int i, j;
	
	hBmpDC = CreateCompatibleBitmap( hdc, WINSIZE_X, WINSIZE_Y );
	SelectObject( hdcWave, hBmpDC );
	
	/* まず背景を白に */
	static HPEN hPen, hOldPen;
	static HBRUSH hBrush, hOldBrush;
	
	hPen = CreatePen( PS_SOLID, 0, RGB( 255, 255, 255 ) );
	hOldPen = (HPEN)SelectObject( hdcBack, hPen );
	hBrush = CreateSolidBrush( RGB( 255, 255, 255 ) );
	hOldBrush = (HBRUSH)SelectObject( hdcBack, hBrush );
	Rectangle( hdcWave, 0, 0, WINSIZE_X, WINSIZE_Y );
	
	/* 中心線 */
	_DrawLine( hdcWave, 0, WINSIZE_Y/2, WINSIZE_X, WINSIZE_Y/2, 128, 128, 128 );

	i=0;j=b;
	while(1){
		int z;
		
		z = dEffDataL[j];
		_DrawLine( hdcWave, i, WINSIZE_Y/2, i, WINSIZE_Y/2+(z-128), 0, 255, 0 );
		j+=a;
		i++;
		if( i > WINSIZE_X ){
			break;
		}
	}
	
	DeleteObject( hPen );
	DeleteObject( hBrush );
	DeleteObject( hBmpDC );
}