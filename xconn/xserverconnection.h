/*

  This file is a part of xevdevserver, a program that reads events using 
  the evdev input event interface of the Linux kernel and sends them to an
  X display using the XTEST extension.
  
  Copyright (C) 2006-2007  Jori Liesenborgs (jori.liesenborgs@gmail.com)

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

*/

#ifndef XSERVERCONNECTION_H

#define XSERVERCONNECTION_H

#include "errorbase.h"
#include <X11/Xlib.h>

class XServerConnection : public errut::ErrorBase
{
public:
	XServerConnection();
	~XServerConnection();

	bool open(const std::string &displayName);
	bool close();

	void setNumLock(bool f);
	void setCapsLock(bool f);
	bool isNumLockSet();
	bool isCapsLockSet();

	void sendMouseButtonEvent(int buttonNumber, bool pressed);
	void sendRelativeMotionEvent(int x, int y);
	void sendScrollUpEvent(int amount);
	void sendScrollDownEvent(int amount);
	void sendKeyEvent(int keyCode, bool pressed);

	void getMouseAccelleration(double *pAccel, int *pThreshold);

	static void installXHandlers();
private:
	static int errorHandler(Display *pDisplay, XErrorEvent *pEvent);

	Display *m_pDisplay;	
};

#endif // XSERVERCONNECTION_H
