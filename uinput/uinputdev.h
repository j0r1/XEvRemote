#pragma once

#include "errorbase.h"

class UInputDev : public errut::ErrorBase
{
public:
	UInputDev();
	~UInputDev();
	bool init(const std::string &fileName);
	void sendMouseButtonEvent(int nr, bool pressed);
	void sendRelativeMotionEvent(int x, int y);
	void sendScrollUpEvent(int amount);
	void sendScrollDownEvent(int amount);
	void sendKeyEvent(int keyCode, bool pressed);
};
