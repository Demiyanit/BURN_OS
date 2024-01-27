#include <stdint.h>
#include "stdio.h"
#include "x86.h"
#include "disk.h"
#include "fat.h"
#include "memdefs.h"
#include "memory.h"
#include "vbe.h"

uint8_t* KernelLoadBuffer = (uint8_t*)MEMORY_LOAD_KERNEL;
uint8_t* Kernel = (uint8_t*)MEMORY_KERNEL_ADDR;

typedef void (*KernelStart)();

void __attribute__((cdecl)) start(uint16_t bootDrive)
{
    clrscr();

    DISK disk;
    if (!DISK_Initialize(&disk, bootDrive))
    {
        printf("Disk init error\r\n");
        goto end;
    }

    if (!FAT_Initialize(&disk))
    {
        printf("FAT init error\r\n");
        goto end;
    }

    // load kernel
    FAT_File* fd = FAT_Open(&disk, "/kernel.bin");
    uint32_t read;
    uint8_t* kernelBuffer = Kernel;
    while ((read = FAT_Read(&disk, fd, MEMORY_LOAD_SIZE, KernelLoadBuffer)))
    {
        memcpy(kernelBuffer, KernelLoadBuffer, read);
        kernelBuffer += read;
    }
    FAT_Close(fd);

    // Initialize graphics
    VbeInfoBlock* info = (VbeInfoBlock*)MEMORY_VESA_INFO;
    VbeModeInfo* modeInfo = (VbeModeInfo*)MEMORY_MODE_INFO;
    if(VBE_GetControllerInfo(info)) {
        uint16_t* mode = (uint16_t*)(info->VideoModePtr);
        for(int i = 0; mode[i] != 0xFFFF; i++) {
            if(!VBE_GetModeInfo(mode[i], modeInfo)) {
                printf("Cant get mode Info %x :(\n", mode[i]);
                continue; 
            }
            
            printf("Found mode %x: %dx%d, bpp:%d, fb:0x%x\n", mode[i], modeInfo->width, modeInfo->height, modeInfo->bpp, modeInfo->framebuffer);

        }
    } else {
        printf("No VBE extensions :(\n");
    }

    // execute kernel
    KernelStart kernelStart = (KernelStart)Kernel;
    //kernelStart();

end:
    for (;;);
}
