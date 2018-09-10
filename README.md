# MX3
Light saber version 3 based on mbed-os and c++

# How to use

## prepare
need install mbed-cli first.

## install
``` shell
mbed deploy
# set toolchain u used
mbed config toolchain gcc_arm
```

## export to IDE
``` shell
# check supported ide
mbed export -S
# export specific ide like:qtcreator
mbed export -i qtcreator
```