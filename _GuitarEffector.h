#define WINSIZE_X	750
#define WINSIZE_Y	430

#define szHELP		"Copyright(c) 05cs21 2006"
#define TITLE		"ÅuGuitarEffectorÅv Ver0.01"

/* WindowClassName */
char szClassName[] = "GuitarEffector";

char szTitle[64];

char szFileName[MAX_PATH];
char szFileTitle[MAX_PATH];
char szEffFileName[] = "temp.wav";
int *dWaveDataL;
int *dWaveDataR;
int *dEffDataL;
int *dOldEffDataL;

HDC	hdcBack;
HDC	hdcWave;

bool	flagWaveShow=false;
bool	ErrorFlag=false;
bool	flagWaveEff=false;

int spl = 1;
int times = 1;

HINSTANCE hInst;
HWND hWindow;