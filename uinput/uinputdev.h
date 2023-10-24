#pragma once

#include "errorbase.h"

class UInputDev : public errut::ErrorBase
{
public:
	UInputDev();
	~UInputDev();
	bool init(const std::string &fileName);
	bool close();
	void sendMouseButtonEvent(int nr, bool pressed);
	void sendRelativeMotionEvent(int x, int y);
	void sendScrollUpEvent(int amount);
	void sendScrollDownEvent(int amount);
	void sendKeyEvent(int keyCode, bool pressed);
private:
	bool writeEvent(int type, int code = 0, int value = 0);
	int m_fd;
};
