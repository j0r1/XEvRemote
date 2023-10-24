from cpython.version cimport PY_MAJOR_VERSION
from cpython.array cimport array

cdef B(s):
    return bytes(s, 'UTF-8')

cdef S(b):
    return b.decode(encoding='UTF-8')

cdef class UInput:

    def __cinit__(self):
        pass

    def __init__(self, devFileName):
        raise Exception("TODO")

    def sendMouseButtonEvent(self, int buttonNumber, pressed):
        pass

    def sendRelativeMotionEvent(self, int x, int y):
        pass

    def sendScrollUpEvent(self, int amount):
        pass

    def sendScrollDownEvent(self, int amount):
        pass

    def sendKeyEvent(self, int keyCode, pressed):
        pass


