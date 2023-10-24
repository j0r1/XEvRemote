from libcpp cimport bool as cbool
from libcpp.string cimport string as stdstring

cdef extern from "uinputdev.h":

    cdef cppclass UInputDev:

        stdstring getErrorString()

        UInputDev()
        cbool init(const stdstring &fileName)
        void sendMouseButtonEvent(int nr, cbool pressed)
        void sendRelativeMotionEvent(int x, int y)
        void sendScrollUpEvent(int amount)
        void sendScrollDownEvent(int amount)
        void sendKeyEvent(int keyCode, cbool pressed)

