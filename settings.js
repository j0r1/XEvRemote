const thresKey = "xevremote-threshold";
const accelKey = "xevremote-accel";
const scrollKey = "xevremote-scrolldiv";
const dxyKey = "xevremote-dxyscale";

function loadSettings()
{
    let t = 10;
    let a = 4;
    let sd = 5;
    let sc = 1;

    if (thresKey in localStorage)
        t = parseFloat(localStorage[thresKey]);
    if (accelKey in localStorage)
        a = parseFloat(localStorage[accelKey]);
    if (scrollKey in localStorage)
        sd = parseFloat(localStorage[scrollKey]);
    if (dxyKey in localStorage)
        sc = parseFloat(localStorage[dxyKey]);

    a = Math.round(a);
    if (t < 0)
        t = 0;
    if (a <= 0)
        a = 1;
    if (sd < 0)
        sd = 1;
    if (sc < 0)
        sc = 1;

    return [t, a, sd, sc];
}

function onSettingsChanged()
{
    localStorage[thresKey] = document.getElementById("idthreshold").value;
    localStorage[accelKey] = document.getElementById("idaccel").value;
    localStorage[scrollKey] = document.getElementById("idscroll").value;
    localStorage[dxyKey] = document.getElementById("idmove").value;
}

function settingsMain()
{
    let [t, a, sd, sc ] = loadSettings(); // Get stored values or defaults
    localStorage[thresKey] = t;
    localStorage[accelKey] = a;
    localStorage[scrollKey] = sd;
    localStorage[dxyKey] = sc;

    document.getElementById("idthreshold").value = t;
    document.getElementById("idaccel").value = a;
    document.getElementById("idscroll").value = sd;
    document.getElementById("idmove").value = sc;
}
