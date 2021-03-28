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


Helper script
--------------

There is a script to help with encoding and decoding images in the
format the firmware wants. To use them, you need to install the
requirements:

`cd scripts/`

`pip install -r requirements.txt`

After that, you can use the script, for example, to decode a binary
dump of the buffer array from the microcontroller:

`./codec decode -i dump.bin -o dumped_image.png`

Or to convert an image to this binary representation:

`./codec encode -i image.png -o image.bin`

You can also output the image as a C array. This is useful to use
directly in the firmware as your base image, for example:

`./codec encode -i image.png --as-c-array`


License
-------

This project is free software, distributed under the terms of the GNU
General Public License as published by the Free Software Foundation,
either version 3 of the License, or (at your option) any later
version. See the `LICENSE` file for more information.
