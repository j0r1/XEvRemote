XEvRemote
---------

This is a small set of tools to e.g. use your smartphone as a mouse/keyboard
for a GNU/Linux based PC (running X windows, with XTest extension enabled).
I use it to use my smartphone to remote control a Raspberry Pi that's connected
to my TV.

 - `xconn`: contains a [Cython](http://cython.org/) based [Python](https://www.python.org/)
   module using which keyboard and mouse events can be injected into
   an X windows display, using the XTest extension. This is basically
   some Python bindings for a
   file from [xevdevserver](http://research.edm.uhasselt.be/jori/page/Misc/XevdevServer.html)

 - `uinput`: similar to the `xconn` module, but instead of using XTest,
   keyboard and mouse events are injected into the system using
   `/dev/uinput`, which in turn creates a `/dev/input/eventX` device.

 - `processmodmap.py`: helper script which reads [xmodmap](https://www.x.org/archive/X11R6.8.1/doc/xmodmap.1.html)
   output to get keyboard event codes. Redirect the output to a file
   called `keymap.js`

 - `index.html` and `main.js`: a webpage that can be loaded on your smartphone.
   I typically just use [simplehttpserver](https://www.npmjs.com/package/simplehttpserver)
   as a small webserver, but the webserver system used does not matter. The
   port on which this server listens for connections is not important either.

 - `server.py`: a small websocket server that should run on the same host
   as the webserver. It will listen for connections on port 8081, and the
   `main.js` file has this port number for the websocket server hardcoded.
   Either takes an X11 display, e.g. `:0`, as argument, or `/dev/uinput`,
   depending on the input method used.

### Using /dev/uinput

On my Slackware system using X11, using the /dev/uinput method works without
extra modifications needed. On my Raspberry Pi that runs systemd and Wayland,
the virtual input device that's added was not recognized immediately because
of two issues:

 - from the systemd-logind source code, it appears that a `"seat"` tag needs
   to be present
 - the tag was looked for in a file like `/run/udev/data/+input:input6`, which
   wasn't created by `udev`. Instead only a `c13:69` (13 and 69 are major and
   minor device numbers) existed in this directory (for which the tag could be
   successfully added).

To resolve both of these issues, I added the following udev rule:

    ACTION=="add", SUBSYSTEM=="input", KERNEL=="event*", ATTRS{name}=="XEvRemote virtual keyboard and mouse", TAG+="seat", RUN+="/bin/sh -c '/usr/bin/ln -sf /run/udev/data/c%M:%m /run/udev/data/+input:`/usr/bin/echo %p | /usr/bin/cut -f 5 -d /` > /tmp/virtual_kb_mouse_udev.log '"

This rule gets the right `inputX` name and creates a symlink to the existing device. 
There's got to be a cleaner way to resolve this, but for now this seems to work.

