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

## debug with openocd and arm-none-eabi-gdb
``` shell
## Shell 1 run:
openocd -f ./openocd_jlink.cfg

## Shell 2 run:
arm-none-eabi-gdb-py xxx.elf -x gdb_load.sh

## download elf to board(in shell 2)
load
```

# note
## Debug Issues
- When debug should enable stm32 sepicific function.
    ``` c
    HAL_DBGMCU_EnableDBGSleepMode();
    HAL_DBGMCU_EnableDBGStandbyMode();
    HAL_DBGMCU_EnableDBGStopMode();
    ```


