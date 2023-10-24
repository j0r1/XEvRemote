from cpython.version cimport PY_MAJOR_VERSION
from cpython.array cimport array
from libcpp.memory cimport unique_ptr, make_unique
from cython.operator cimport dereference as deref

cimport uinputdev

cdef B(s):
    return bytes(s, 'UTF-8')

cdef S(b):
    return b.decode(encoding='UTF-8')

cdef class UInput:

    cdef unique_ptr[uinputdev.UInputDev] m_pInput

    def __cinit__(self):
        self.m_pInput = make_unique[uinputdev.UInputDev]()

    def __init__(self, devFileName):
        if not deref(self.m_pInput).init(B(devFileName)):
            raise Exception("Unable to open device '{}'".format(devFileName))

    def sendMouseButtonEvent(self, int buttonNumber, pressed):
        deref(self.m_pInput).sendMouseButtonEvent(buttonNumber, pressed)

    def sendRelativeMotionEvent(self, int x, int y):
        deref(self.m_pInput).sendRelativeMotionEvent(x, y)

    def sendScrollUpEvent(self, int amount):
        deref(self.m_pInput).sendScrollUpEvent(amount)

    def sendScrollDownEvent(self, int amount):
        deref(self.m_pInput).sendScrollDownEvent(amount)

    def sendKeyEvent(self, int keyCode, pressed):
        deref(self.m_pInput).sendKeyEvent(keyCode, pressed)


