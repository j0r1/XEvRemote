from xconn.xconn import XConn
import websockets
import asyncio
import json
import sys

xc = XConn(sys.argv[1])

async def serverMain(webSocket, path):
    print("Connection from", webSocket)
    while True:
        cmd = json.loads(await webSocket.recv())
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

startServer = websockets.serve(serverMain, port=8081)

loop = asyncio.get_event_loop()  
loop.run_until_complete(startServer)  
loop.run_forever()
