#include "Load.h"
#include "resource.h"
#include <windows.h>
#include <tchar.h>

HBITMAP LoadBK(HBITMAP hbit1, HINSTANCE g_hinst)
{
	hbit1 = (HBITMAP)LoadBitmap(g_hinst, MAKEINTRESOURCE(IDB_BITMAP1));
	return hbit1;
}