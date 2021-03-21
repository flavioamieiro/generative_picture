This is the software for the Generative Picture. It drives a waveshare
7.5 inch black and white display using an ESP32. The [description of
the hardware for this project is also
available](https://github.com/flavioamieiro/generative_picture_hardware).


Requirements
------------

You need the [esp32 arduino
core](https://github.com/espressif/arduino-esp32#installation-instructions)
and [GxEPD2](https://github.com/ZinggJM/GxEPD2) installed.

You will also need to patch GxEPD2 to run partial updates after the
ESP32 wakes up from deep sleep without any artifacts. To do so, first
copy the patch (which is under the [patches](patches/) directory) to
your arduino libraries directory. The libraries are inside the
sketchbook, which you can find by opening "File > Preferences" in the
IDE and checking the "Sketchbook Location" field (on linux, by
default, this is `~/Arduino/`, so your libraries are in
`~/Arduino/libraries`).

After copying the patch to that directory (the `libraries/` inside your
sketchbook), move there and run:

```
patch -p1 < 0001-Creates-a-method-to-allow-the-user-to-set-the-curren.patch
```

Restart your arduino IDE and you should be able to just upload the
code.

License
-------

This project is free software, distributed under the terms of the GNU
General Public License as published by the Free Software Foundation,
either version 3 of the License, or (at your option) any later
version. See the `LICENSE` file for more information.
