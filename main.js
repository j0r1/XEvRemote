class Connection
{
    constructor(url)
    {
        this.url = url;
        this._openWS();
    }

    destroy()
    {
        if (!this.ws)
            return;

        this.ws.onclose = null;
        this.ws.onmessage = null;
        this.ws.onopen = null;
        this.ws.close();
        this.ws = null;
        console.log("Websocket command connection destroyed");
    }

    onConnectionStateChanged(connected) { } // Override this!

    _openWS()
    {
        this.ws = new WebSocket(this.url);
        this.ws.onclose = () => { this._onWSClose(); this.onConnectionStateChanged(false); }
        this.ws.onopen = () => { this.onConnectionStateChanged(true); }
        this.ws.onmessage = (msg) => { this.onMessage(msg); }
    }

    _onWSClose()
    {
        this.ws.onclose = null;
        this.ws.onmessage = null;
        this.ws.onopen = null;
        this.ws = null;
        setTimeout(() => { 
            console.log("Trying to re-open websocket");
            this._openWS()
        }, 1000); // Wait a bit and try again
    }

    onMessage(msg) // Override if needed!
    {
        console.log("Received message on WebSocket:");
        console.log(msg);
    }

    sendPing()
    {
        if (this.ws && this.ws.readyState == 1)
            this.ws.send("PING");
    }

    sendCommand(obj)
    {
        if (this.ws && this.ws.readyState == 1)
            this.ws.send(JSON.stringify(obj));
    }
}

let conn = null;

function onConnectionStateChanged(connected)
{
    let tbl = document.getElementById("maintable");
    let d = document.getElementById("connecting");
    if (connected)
    {
        tbl.style.display = "";
        d.style.display = "none";
    }
    else
    {
        tbl.style.display = "none";
        d.style.display = "";
    }
}

function log(s)
{
    var e = document.getElementById("log");
    var txt = e.innerText;
    e.innerText = txt + "\n" + s;
}

var threshold = 10;
var accel = 4;
var dxRemainder = 0;
var dyRemainder = 0;

var activeTouches = { }

function handleStart(e)
{
    var touches = e.changedTouches;
    for (var i = 0 ; i < touches.length ; i++)
    {
        var n = touches[i].identifier;
        activeTouches[n] = { "x": touches[i].pageX, "y": touches[i].pageY, "time": Date.now(), "relx": 0, "rely": 0 };
    }
}

function getNumActiveTouches()
{
    var count = 0;
    for (var n in activeTouches)
        count++;

    return count;
}

function handleEnd(e)
{
    var touches = e.changedTouches;
    var count = getNumActiveTouches();

    if (count == 1 && touches.length == 1)
    {
        var n = touches[0].identifier;
        var at = activeTouches[n];

        if (n in activeTouches)
        {
            var dt = Date.now() - at.time;
            var diff2 = at.relx*at.relx+at.rely*at.rely;
            if (dt < 500 && diff2 < 16)
                conn.sendCommand({ "type": "click", "button": 1 });
        }
    }

    // Generic code
    for (var i = 0 ; i < touches.length ; i++)
    {
        var n = touches[i].identifier;
        delete activeTouches[n];
    }
}

function handleCancel(e)
{
}

var scrollSum = 0;
var scrollDiv = 10;

function processScroll(dyLeft)
{
    scrollSum += dyLeft/5;

    while (scrollSum > 0)
    {
        if (scrollSum < 1)
            break;

        conn.sendCommand({ "type": "scroll", "num": 1 });
        scrollSum -= 1;
    }

    while (scrollSum < 0)
    {
        if (scrollSum > -1)
            break;

        conn.sendCommand({ "type": "scroll", "num": -1 });
        scrollSum += 1;
    }
}

function handleMove(e)
{
    var touches = e.changedTouches;
    var count = getNumActiveTouches();

    for (var i = 0 ; i < touches.length ; i++)
    {
        var t = touches[i];
        var n = t.identifier;
        var x = t.pageX;
        var y = t.pageY;
        var at = activeTouches[n];
        var dx = (x - at.x);
        var dy = (y - at.y);
        var dxLeft = Math.round(dx - at.relx);
        var dyLeft = Math.round(dy - at.rely);

        at.relx += dxLeft;
        at.rely += dyLeft;

        if (count > 1)
        {
            processScroll(dyLeft);
        }
        else
        {
            if (dxLeft != 0 || dyLeft != 0)
            {
                var q = dxLeft*dxLeft+dyLeft*dyLeft;
                if (q <= threshold*threshold)
                    conn.sendCommand({ "type": "moverel", "dx": dxLeft, "dy": dyLeft });
                else
                    conn.sendCommand({ "type": "moverel", "dx": dxLeft*accel, "dy": dyLeft*accel });
            }
        }
    }
}

var mapNames = [ "normalmap", "shiftmap", "altgrmap" ];
var sendExtra = [ null, keyMap["shiftcode"], keyMap["altgrcode"] ];

function onSendText()
{
    var txtElem = document.getElementById("textinput");
    var txt = txtElem.value;
    txtElem.value = "";

    log("Sending text: " + txt);

    for (var i = 0 ; i < txt.length ; i++)
    {
        var c = txt[i];
        var charName = keyMap["charnames"][c];
        log("Char is: " + charName)
        if (charName)
        {
            for (var j = 0 ; j < mapNames.length ; j++)
            {
                var mapName = mapNames[j];
                var modifier = sendExtra[j];

                var code = keyMap[mapName][charName];
                if (code !== undefined)
                {
                    var oldCode = code;
                    if (modifier !== null)
                        modifier -= keyMapOffset;

                    code -= keyMapOffset;
                    log("Converting code " + oldCode + " to " + code);
                    log("sending " + code + " " + modifier);

                    if (modifier !== null)
                        conn.sendCommand({ "type": "key", "keycode": modifier, "pressed": true });
                    
                    conn.sendCommand({ "type": "key", "keycode": code, "pressed": true });
                    conn.sendCommand({ "type": "key", "keycode": code, "pressed": false });
                    
                    if (modifier !== null)
                        conn.sendCommand({ "type": "key", "keycode": modifier, "pressed": false });
                    break;
                }
            }
        }
    }
}

function onKeyButton(keyName)
{
    if (keyName == "RightClick")
    {
        conn.sendCommand({ "type": "click", "button": 3 });
        return;
    }

    var code = keyMap["normalmap"][keyName];
    if (!code)
        return;

    code -= keyMapOffset;

    conn.sendCommand({ "type": "key", "keycode": code, "pressed": true });
    conn.sendCommand({ "type": "key", "keycode": code, "pressed": false });
}

function startConnection()
{
    let wsUrl = "ws://" + location.hostname + ":8081";
    conn = new Connection(wsUrl);
    conn.onConnectionStateChanged = onConnectionStateChanged;
    conn.onMessage = (msg) => { conn.lastHeardTime = performance.now(); console.log("Got message: " + msg.data); }
    conn.lastHeardTime = performance.now();
}

function connectionMonitorTimer()
{
    conn.sendPing();

    if (performance.now() - conn.lastHeardTime > 1000)
    {
        console.log("Restarting connection");
        conn.destroy();
        conn = null;

        startConnection();
    }
}

function main()
{
    log("V20");
    try
    {
        startConnection();
        setInterval(connectionMonitorTimer, 200);

        let el = document.getElementById("touchdiv");
        el.addEventListener("touchstart", handleStart, false);
        el.addEventListener("touchend", handleEnd, false);
        el.addEventListener("touchcancel", handleCancel, false);
        el.addEventListener("touchmove", handleMove, false);

        let b = document.getElementById("sendbutton");
        b.onclick = onSendText;
    } 
    catch(e)
    {
        log("Error: " + e);
    }
}
