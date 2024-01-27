#include "stdio.h"
#include "vbe.h"
#include "x86.h"

bool VBE_GetControllerInfo(VbeInfoBlock* info) {
    int ret = x86_Video_GetVbeInfo(info);
    printf("VBE_GetControllerInfo: %x\n", ret);
    return ret == 0x004F;
}