#include "uinputdev.h"

UInputDev::UInputDev()
{
}

UInputDev::~UInputDev()
{
}

bool UInputDev::init(const std::string &fileName)
{
    return true;
}

void UInputDev::sendMouseButtonEvent(int nr, bool pressed)
{
}

void UInputDev::sendRelativeMotionEvent(int x, int y)
{
}

void UInputDev::sendScrollUpEvent(int amount)
{
}

void UInputDev::sendScrollDownEvent(int amount)
{
}

void UInputDev::sendKeyEvent(int keyCode, bool pressed)
{
}

