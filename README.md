# OpenDAQ
University Of Illinois DAQ firmware

Please see Documentation in /docs/html/index.html of each version.

Overview

This firmware is designed to be easily edited to add, remove, and change the order of inputs.
All the standard input blocks are in the file inputs.c
With minimal programming experience, other sensors with different protocols may be added as well, by creating a new input block and adding it to inputs.c
Adding and ordering channels Steps:

    Install the compiler, libraries and editor.
    Edit inputs.c, placing input code blocks in order.
    Compile to a .hex file.
    Upload this hex file to the DAQ
