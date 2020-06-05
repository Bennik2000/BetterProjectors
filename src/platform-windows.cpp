/*
Better Projectors
Copyright (C) 2020 Bennik2000 bennik.ko@gmail.com

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License along
with this program. If not, see <https://www.gnu.org/licenses/>
*/

#include "platform.hpp"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

bool IsAlwaysOnTop(QWidget *window)
{
	DWORD exStyle = GetWindowLong((HWND)window->winId(), GWL_EXSTYLE);
	return (exStyle & WS_EX_TOPMOST) != 0;
}

void SetAlwaysOnTop(QWidget *window, bool enable)
{
	HWND hwnd = (HWND)window->winId();
	SetWindowPos(hwnd, enable ? HWND_TOPMOST : HWND_NOTOPMOST, 0, 0, 0, 0,
		     SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
}
