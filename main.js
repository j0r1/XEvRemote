function log(s)
{
    var e = document.getElementById("log");
    var txt = e.innerText;
    e.innerText = txt + "\n" + s;
}

var ws = null;
var connected = false;

function onConnected()
{
    connected = true;

    var tbl = document.getElementById("maintable");
    tbl.style.display = "";

    var d = document.getElementById("connecting");
    d.style.display = "none";

    var el = document.getElementById("touchdiv");
    el.addEventListener("touchstart", handleStart, false);
    el.addEventListener("touchend", handleEnd, false);
    el.addEventListener("touchcancel", handleCancel, false);
    el.addEventListener("touchmove", handleMove, false);

    var b = document.getElementById("sendbutton");
    b.onclick = onSendText;
}

function onClose()
{
    if (!connected)
    {
        var d = document.getElementById("connecting");
        d.innerHTML += "<br>Connection failed!";
        return;
    }

    var el = document.getElementById("touchdiv");
    el.style.background = "red";
    el.style.color = "white";

    el.innerHTML = "<p>CONNECTION LOST!</p><p><button onclick='location.reload(false)'>Reload</button></p>";
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
                ws.send(JSON.stringify({ "type": "click", "button": 1 }));
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

        ws.send(JSON.stringify({ "type": "scroll", "num": 1 }));
        scrollSum -= 1;
    }

    while (scrollSum < 0)
    {
        if (scrollSum > -1)
            break;

        ws.send(JSON.stringify({ "type": "scroll", "num": -1 }));
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
                    ws.send(JSON.stringify({ "type": "moverel", "dx": dxLeft, "dy": dyLeft }));
                else
                    ws.send(JSON.stringify({ "type": "moverel", "dx": dxLeft*accel, "dy": dyLeft*accel }));
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

    log(txt);

    for (var i = 0 ; i < txt.length ; i++)
    {
        var c = txt[i];
        var charName = keyMap["charnames"][c];
        log(charName)
        if (charName)
        {
            for (var j = 0 ; j < mapNames.length ; j++)
            {
                var mapName = mapNames[j];
                var modifier = sendExtra[j];

                var code = keyMap[mapName][charName];
                if (code !== undefined)
                {
                    log("sending " + code + " " + modifier);
                    if (modifier !== null)
                        ws.send(JSON.stringify({ "type": "key", "keycode": modifier, "pressed": true }));
                    
                    ws.send(JSON.stringify({ "type": "key", "keycode": code, "pressed": true }));
                    ws.send(JSON.stringify({ "type": "key", "keycode": code, "pressed": false }));
                    
                    if (modifier !== null)
                        ws.send(JSON.stringify({ "type": "key", "keycode": modifier, "pressed": false }));
                    break;
                }
            }
        }
    }
}

function main()
{
    log("V16");
    try
    {
        var wsUrl = "ws://" + location.hostname + ":8081";
        ws = new WebSocket(wsUrl);
        ws.onopen = onConnected;
        ws.onclose = onClose;
    } 
    catch(e)
    {
        log("Error: " + e);
    }
}
