#include "uinputdev.h"
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <linux/input.h>
#include <linux/uinput.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <string>
#include <iostream>

using namespace std;

string getErr()
{
	return string(strerror(errno));
}

UInputDev::UInputDev() : m_fd(-1)
{
}

UInputDev::~UInputDev()
{
	close();
}

bool UInputDev::init(const std::string &fileName)
{
	if (m_fd >= 0)
	{
		setErrorString("A device is already open");
		return false;
	}

	int fkbd = open(fileName.c_str(), O_RDWR|O_NONBLOCK);

	auto errExit = [this, fkbd](const string &err) -> bool
	{
		if (fkbd >= 0)
			::close(fkbd);

		setErrorString(err);
		return false;
	};

	if (fkbd < 0)
		return errExit("Can't open uinput device " + fileName + ": " + getErr());

	struct uinput_user_dev device;

	memset(&device, 0, sizeof(device));
	strcpy(device.name, "XEvRemote virtual keyboard and mouse");
	device.id.bustype = BUS_USB;
	device.id.vendor = 1;
	device.id.product = 1;
	device.id.version = 1;

	if (write(fkbd, &device, sizeof(struct uinput_user_dev)) < 0)
		return errExit("Can't register new device: " + getErr());

	if (ioctl(fkbd, UI_SET_EVBIT, EV_KEY) < 0)
		return errExit("Can't set EV_KEY: " + getErr());

	for (unsigned short k = 1; k < KEY_MAX ; k++)
	{
		if (ioctl(fkbd, UI_SET_KEYBIT, k) < 0)
			return errExit("Can't set UI_SET_KEYBIT " + to_string(k) + ": " + getErr());
	}

	if (ioctl(fkbd, UI_SET_EVBIT, EV_REL) < 0)
		return errExit("Can't set EV_REL: " + getErr());

	if (ioctl(fkbd, UI_SET_RELBIT, REL_X) < 0)
		return errExit("Can't set REL_X: " + getErr());

	if (ioctl(fkbd, UI_SET_RELBIT, REL_Y) < 0)
		return errExit("Can't set REL_Y: " + getErr());

	if (ioctl(fkbd, UI_SET_RELBIT, REL_WHEEL) < 0)
		return errExit("Can't set REL_WHEEL: " + getErr());

	if (ioctl(fkbd, UI_DEV_CREATE) < 0)
		return errExit("Can't create device: " + getErr());

	m_fd = fkbd;

	return true;
}

bool UInputDev::close()
{
	if (m_fd < 0)
	{
		setErrorString("Not open");
		return false;
	}

	if (ioctl(m_fd, UI_DEV_DESTROY) < 0)
	{
		setErrorString("Unable to destroy device" + getErr());
		return false;
	}
	return true;
}

void UInputDev::sendMouseButtonEvent(int nr, bool pressed)
{
	int btn = 0;
	if (nr == 1)
		btn = BTN_LEFT;
	else if (nr == 2)
		btn = BTN_MIDDLE;
	else if (nr == 3)
		btn = BTN_RIGHT;
	else
	{
		cerr << "Ignoring invalid button number " << nr << endl;
		return;
	}

	if (!writeEvent(EV_KEY, btn, (pressed)?1:0))
		return;
	if (!writeEvent(EV_SYN))
		return;
	cerr << "Wrote button press for nr " << nr << " pressed: " << ((pressed)?"true":"false") << endl;
}

void UInputDev::sendRelativeMotionEvent(int x, int y)
{
	if (!writeEvent(EV_REL, REL_X, x))
		return;
	if (!writeEvent(EV_REL, REL_Y, y))
		return;
	if (!writeEvent(EV_SYN))
		return;
	cerr << "Relative motion by " << x << "," << y << endl;
}

void UInputDev::sendScrollUpEvent(int amount)
{
	if (!writeEvent(EV_REL, REL_WHEEL, amount))
		return;
	if (!writeEvent(EV_SYN))
		return;
	cerr << "Scrolled up by " << amount << endl;
}

void UInputDev::sendScrollDownEvent(int amount)
{
	if (!writeEvent(EV_REL, REL_WHEEL, -amount))
		return;
	if (!writeEvent(EV_SYN))
		return;
	cerr << "Scrolled down by " << amount << endl;
}

bool UInputDev::writeEvent(int type, int code, int value)
{
	struct input_event event;

	memset(&event, 0, sizeof(event));
	event.type = type;
	event.code = code;
	event.value = value;
	if (write(m_fd, &event, sizeof(event)) != sizeof(event))
	{
		cerr << "Error writing event (" << type << "," << value << "," << code << "): " << getErr() << endl;
		return false;
	}
	return true;
}

void UInputDev::sendKeyEvent(int keyCode, bool pressed)
{
	if (!writeEvent(EV_KEY, keyCode, (pressed)?1:0))
		return;

	if (!writeEvent(EV_SYN))
		return;

	cerr << "Wrote keyCode " << keyCode << " pressed: " << ((pressed)?"true":"false") << endl;
}

