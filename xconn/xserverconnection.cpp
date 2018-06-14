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

#include "xserverconnection.h"
#include <X11/XKBlib.h>
#include <X11/extensions/XTest.h>
#include <X11/extensions/XKB.h>
#include <stdint.h>
#include <stdlib.h>
#include <iostream>

XServerConnection::XServerConnection()
{
	m_pDisplay = 0;
}

XServerConnection::~XServerConnection()
{
	close();
}

bool XServerConnection::open(const std::string &displayName)
{
	if (m_pDisplay != 0)
	{
		setErrorString("A display has already been opened");
		return false;
	}
	
	Display *pDisplay = XOpenDisplay(displayName.c_str());

	if (pDisplay == 0)
	{
		setErrorString("Unable to open display");
		return false;
	}

	int eventBase, errorBase, majorNumber, minorNumber;
	
	if (XTestQueryExtension(pDisplay, &eventBase, &errorBase, &majorNumber, &minorNumber) == False)
	{
		setErrorString("XTEST extension is not supported");
		XCloseDisplay(pDisplay);
		return false;
	}

	if (XTestGrabControl(pDisplay, True) == 0) // TODO: Is this really ok?
	{
		setErrorString("Unable to control server grabbing");
		XCloseDisplay(pDisplay);
		return false;
	}

	m_pDisplay = pDisplay;

	return true;
}

bool XServerConnection::close()
{
	if (m_pDisplay == 0)
	{
		setErrorString("No display has been opened yet");
		return false;
	}

	XCloseDisplay(m_pDisplay);
	m_pDisplay = 0;

	return true;
}

#define XSERVERCONNECTION_KEYCODE_NUMLOCK 	77
#define XSERVERCONNECTION_KEYCODE_CAPSLOCK	66
#define XSERVERCONNECTION_KEYCODE_SCROLLLOCK	78

void XServerConnection::setNumLock(bool f)
{
	if (isNumLockSet() != f)
	{
		XTestFakeKeyEvent(m_pDisplay, XSERVERCONNECTION_KEYCODE_NUMLOCK, True, CurrentTime);
		XFlush(m_pDisplay); // TODO: is this one necessary?
		XTestFakeKeyEvent(m_pDisplay, XSERVERCONNECTION_KEYCODE_NUMLOCK, False, CurrentTime);
		XFlush(m_pDisplay);
	}
}

void XServerConnection::setCapsLock(bool f)
{
	if (isCapsLockSet() != f)
	{
		XTestFakeKeyEvent(m_pDisplay, XSERVERCONNECTION_KEYCODE_CAPSLOCK, True, CurrentTime);
		XFlush(m_pDisplay); // TODO: is this one necessary?
		XTestFakeKeyEvent(m_pDisplay, XSERVERCONNECTION_KEYCODE_CAPSLOCK, False, CurrentTime);
		XFlush(m_pDisplay);
	}
}

bool XServerConnection::isNumLockSet()
{
	XkbStateRec state;
	uint8_t modifierState;

	XkbGetState(m_pDisplay, XkbUseCoreKbd, &state);
	modifierState = XkbModLocks(&state);

	if (modifierState & 0x10)
		return true;
	return false;
}

bool XServerConnection::isCapsLockSet()
{
	XkbStateRec state;
	uint8_t modifierState;

	XkbGetState(m_pDisplay, XkbUseCoreKbd, &state);
	modifierState = XkbModLocks(&state);

	if (modifierState & 0x02)
		return true;
	return false;
}

void XServerConnection::sendMouseButtonEvent(int buttonNumber, bool pressed)
{
	Bool isPressed = (pressed)?True:False;

	XTestFakeButtonEvent(m_pDisplay, buttonNumber, isPressed, CurrentTime);
	XFlush(m_pDisplay);
}

void XServerConnection::sendRelativeMotionEvent(int x, int y)
{
	XTestFakeRelativeMotionEvent(m_pDisplay, x, y, CurrentTime);
	XFlush(m_pDisplay);
}

void XServerConnection::sendScrollUpEvent(int amount)
{
	for (int i = 0 ; i < amount ; i++)
	{
		XTestFakeButtonEvent(m_pDisplay, 4, True, CurrentTime);
		XTestFakeButtonEvent(m_pDisplay, 4, False, CurrentTime);
	}
	XFlush(m_pDisplay);
}

void XServerConnection::sendScrollDownEvent(int amount)
{
	for (int i = 0 ; i < amount ; i++)
	{
		XTestFakeButtonEvent(m_pDisplay, 5, True, CurrentTime);
		XTestFakeButtonEvent(m_pDisplay, 5, False, CurrentTime);
	}
	XFlush(m_pDisplay);
}

void XServerConnection::sendKeyEvent(int keyCode, bool pressed)
{
	Bool isPressed = (pressed)?True:False;
	
	XTestFakeKeyEvent(m_pDisplay, keyCode, isPressed, CurrentTime);
	XFlush(m_pDisplay);
}

void XServerConnection::getMouseAccelleration(double *pAccel, int *pThreshold)
{
	int accNumerator, accDenominator;
	int accThreshold;

	XGetPointerControl(m_pDisplay, &accNumerator, &accDenominator, &accThreshold);

	*pAccel = ((double)accNumerator)/((double)accDenominator);
	*pThreshold = accThreshold;
}

int XServerConnection::errorHandler(Display *pDisplay, XErrorEvent *pEvent)
{
	std::cerr << "Caught (but ignored) Xlib error:" << std::endl
		  << "  Error code:    " << pEvent->error_code << std::endl
	          << "  Request code:  " << pEvent->request_code << std::endl
		  << "  Minor code:    " << pEvent->minor_code << std::endl
		  << "  Serial number: " << pEvent->serial << std::endl
		  << std::endl;

	return 0;
}

void XServerConnection::installXHandlers()
{
	XSetErrorHandler(errorHandler);
}

