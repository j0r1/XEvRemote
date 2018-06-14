from cpython.version cimport PY_MAJOR_VERSION
from cpython.array cimport array

cimport xserverconnection

cdef B(s):
    if PY_MAJOR_VERSION < 3:
        return s
    return bytes(s, 'UTF-8')

cdef S(b):
    if PY_MAJOR_VERSION < 3:
        return b
    return b.decode(encoding='UTF-8')

cdef class XConn:

    cdef xserverconnection.XServerConnection *m_pConn

    def __cinit__(self):
        xserverconnection.XServerConnection.installXHandlers()
        self.m_pConn = new xserverconnection.XServerConnection()

    def __dealloc__(self):
        del self.m_pConn

    def __init__(self, displayName):
        if not self.m_pConn.open(B(displayName)):
            raise Exception("Unable to open display {}".format(displayName))

    def setNumLock(self, f):
        self.m_pConn.setNumLock(f)

    def setCapsLock(self, f):
        self.m_pConn.setCapsLock(f)

    def isNumLockSet(self):
        return self.m_pConn.isNumLockSet()

    def isCapsLockSet(self):
        return self.m_pConn.isCapsLockSet()

    def sendMouseButtonEvent(self, int buttonNumber, pressed):
        self.m_pConn.sendMouseButtonEvent(buttonNumber, pressed)

    def sendRelativeMotionEvent(self, int x, int y):
        self.m_pConn.sendRelativeMotionEvent(x, y)

    def sendScrollUpEvent(self, int amount):
        self.m_pConn.sendScrollUpEvent(amount)

    def sendScrollDownEvent(self, int amount):
        self.m_pConn.sendScrollDownEvent(amount)

    def sendKeyEvent(self, int keyCode, pressed):
        self.m_pConn.sendKeyEvent(keyCode, pressed)

    def getMouseAccelleration(self):
        cdef double accel
        cdef int thresh

        self.m_pConn.getMouseAccelleration(&accel, &thresh)
        return (accel, thresh)


