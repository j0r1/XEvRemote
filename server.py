from xconn.xconn import XConn
from uinput.uinput import UInput
import websockets
import asyncio
import json
import sys

if sys.argv[1].startswith("/dev"):
    xc = UInput(sys.argv[1])
else:
    xc = XConn(sys.argv[1])

async def serverMain(webSocket):
    print("Connection from", webSocket)
    while True:
        s = await webSocket.recv()
        if s == "PING":
            await webSocket.send("PONG")
            continue

        cmd = json.loads(s)
        if cmd["type"] == "moverel":
            xc.sendRelativeMotionEvent(cmd["dx"], cmd["dy"])
        elif cmd["type"] == "click":
            xc.sendMouseButtonEvent(cmd["button"], True)
            xc.sendMouseButtonEvent(cmd["button"], False)
        elif cmd["type"] == "scroll":
            n = abs(cmd["num"])
            if cmd["num"] > 0:
                xc.sendScrollUpEvent(n)
            else:
                xc.sendScrollDownEvent(n)
        elif cmd["type"] == "key":
            c = cmd["keycode"]
            p = cmd["pressed"]
            xc.sendKeyEvent(c, p)
        else:
            print("Unknown command", cmd)

async def main():
    startServer = await websockets.serve(serverMain, port=8081)
    print("Server running on port 8081")
    await asyncio.Future()

asyncio.run(main())

