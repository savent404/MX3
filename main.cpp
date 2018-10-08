#include "ff.h"
#include "mbed.h"

#include "dac_init.h"
#include "sdio_init.h"
#include "iparam_port.h"

static FATFS fatfs_sd;
static FRESULT res;


int main(void)
{
    HAL_DBGMCU_EnableDBGSleepMode();
    HAL_DBGMCU_EnableDBGStandbyMode();
    HAL_DBGMCU_EnableDBGStopMode();

    __SDIO_Init();
    res = f_mount(&fatfs_sd, "0:/", 1);

    pParam *pP = new pParam(0);
    pAudio *pA = new pAudio(pP);
    while (1);

    return 0;
}
