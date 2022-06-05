//#include "ControCANx64.h"
#include "stdio.h"
#include "ControlCAN.h"


int main(int argc, char* argv[])
{

    printf("Hello\n");

    int nDeviceType = 21; /* USBCAN-2E-U  */
    int nDeviceInd = 0; /* 索引号0 */
    int nReserved = 0;
    DWORD dwRel;
    dwRel = VCI_OpenDevice(nDeviceType, nDeviceInd, nReserved);
    if (dwRel != STATUS_OK)
    {
        printf("打开设备失败!\n");
        return -1;
    }

    return 0;
}
