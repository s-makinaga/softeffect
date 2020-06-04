int _DrawLine( HDC hdc, int oldx, int oldy, int newx, int newy, int r, int g, int b ){ 
	HPEN hPen, hOldPen; 
	 
	hPen = CreatePen(PS_SOLID, 1, RGB(r, g, b)); 
	(void *)hOldPen = SelectObject(hdc, hPen); 
	MoveToEx(hdc, oldx, oldy, NULL); 
	LineTo(hdc, newx, newy); 
	SelectObject(hdc, hOldPen); 
	DeleteObject(hPen); 

	return 0; 
}

int _SetWinCenter(HWND hWnd) { 
	
	HWND hDeskWnd; 
	RECT deskrc, rc; 
	int x, y; 
	
	hDeskWnd = GetDesktopWindow(); 
	GetWindowRect(hDeskWnd, (LPRECT)&deskrc); 
	GetWindowRect(hWnd, (LPRECT)&rc); 
	x = (deskrc.right - (rc.right-rc.left)) / 2; 
	y = (deskrc.bottom - (rc.bottom-rc.top)) / 2; 
	SetWindowPos(hWnd, HWND_TOP, x, y, (rc.right-rc.left), (rc.bottom-rc.top),SWP_SHOWWINDOW); 
	
	return 0; 
}
