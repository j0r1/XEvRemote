from libcpp cimport bool as cbool
from libcpp.string cimport string as stdstring

cdef extern from "xserverconnection.h":

    cdef cppclass XServerConnection:
        XServerConnection()

        stdstring getErrorString()

        cbool open(const stdstring &displayName)
        cbool close()

        void setNumLock(cbool f)
        void setCapsLock(cbool f)
        cbool isNumLockSet()
        cbool isCapsLockSet()

        void sendMouseButtonEvent(int buttonNumber, cbool pressed)
        void sendRelativeMotionEvent(int x, int y)
        void sendScrollUpEvent(int amount)
        void sendScrollDownEvent(int amount)
        void sendKeyEvent(int keyCode, cbool pressed)

        void getMouseAccelleration(double *pAccel, int *pThreshold)

        @staticmethod
        void installXHandlers()

