#pragma once

typedef struct
{
  DSTATUS (*disk_initialize) (BYTE);                            /*!< Initialize Disk Drive                     */
  DSTATUS (*disk_status)     (BYTE);                            /*!< Get Disk Status                           */
  DRESULT (*disk_read)       (BYTE, BYTE*, DWORD, UINT);        /*!< Read Sector(s)                            */
  DRESULT (*disk_write)      (BYTE, const BYTE*, DWORD, UINT);  /*!< Write Sector(s) when _USE_WRITE = 0       */
  DRESULT (*disk_ioctl)      (BYTE, BYTE, void*);               /*!< I/O control operation when _USE_IOCTL = 1 */

} Diskio_drvTypeDef;
