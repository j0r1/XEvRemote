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

TODO: update info for /dev/uinput method

Needed to add a udev rule

    ACTION=="add", SUBSYSTEM=="input", KERNEL=="event*", ATTRS{name}=="XEvRemote virtual keyboard and mouse", TAG+="seat", RUN+="/bin/sh -c '/usr/bin/ln -sf /run/udev/data/c%M:%m /run/udev/data/+input:`/usr/bin/echo %p | /usr/bin/cut -f 5 -d /` > /tmp/virtual_kb_mouse_udev.log '"

For control by systemd, the device needed to have the tag 'seat'. The code was also
looking for a file `/run/udev/data/+input:input6`, which didn't exist. Instead, only
a file c13:69 (using major/minor device number) was created. The udev rule gets the
right `inputX` name and creats a symlink to the existing device. There's got to be
a cleaner way to do this, but for now this seems to work.
    
