# darmstadt plugin for KWin

allows darmstadt to retrieve cursor position when using KWin Wayland.

based on [kwin-effect-spotlight](https://github.com/jinliu/kwin-effect-spotlight).

# note

this is an internal use plugin. I won't provide much detail on how to use it, but it should be pretty simple:

1. mkdir build
2. cd build
3. cmake ..
4. make
5. sudo make install

enable the effect in System Settings. this will create a socket in `/tmp/darmcursor` that darmstadt can use to retrieve cursor position.
