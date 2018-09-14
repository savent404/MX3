#include "ff.h"
#include "mbed.h"

#include "sdio_init.h"

static FATFS fatfs_sd;
static FRESULT res;

static void fatfs_test(void);

int main(void)
{
    HAL_DBGMCU_EnableDBGSleepMode();
    HAL_DBGMCU_EnableDBGStandbyMode();
    HAL_DBGMCU_EnableDBGStopMode();

    __SDIO_Init();
    res = f_mount(&fatfs_sd, "0:/", 1);

    while (1);

    return 0;
}

void fatfs_test(void)
{
    FRESULT res;
    FATFS_DIR dir;
    FILINFO fno;

    res = f_opendir(&dir, "0:/");
    for (;;) {
        res = f_readdir(&dir, &fno);
        if (res != FR_OK || fno.fname[0] == 0)
            break;
    }
}
