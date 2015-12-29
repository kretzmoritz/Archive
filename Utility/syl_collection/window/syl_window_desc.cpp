#include "syl_window_desc.h"

EXTERN_C IMAGE_DOS_HEADER __ImageBase;
#define HINST_THISCOMPONENT ((HINSTANCE)&__ImageBase)

SylWindowDesc::SylWindowDesc()
	: style(CS_HREDRAW | CS_VREDRAW), hIcon(nullptr), hCursor(LoadCursor(nullptr, IDC_ARROW)), hbrBackground(reinterpret_cast<HBRUSH>(GetStockObject(BLACK_BRUSH))), hIconSm(nullptr), 
	dwExStyle(0), lpWindowName("window"), dwStyle(WS_OVERLAPPEDWINDOW), x(0), y(0), nWidth(640), nHeight(480), hInstance(HINST_THISCOMPONENT), 
	nCmdShow(SW_SHOW)
{

}