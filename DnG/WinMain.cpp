#include "stdafx.h"
#include "Resource.h"
#include <iostream>
#include <string>
#include "GUI.h"

int _stdcall WinMain(HINSTANCE hinstance, HINSTANCE hprev, LPSTR cmd, int cmdshow) {
	GUI G(hinstance, cmdshow);
	G.run();

	return 0;
}