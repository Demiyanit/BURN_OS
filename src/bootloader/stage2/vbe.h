#pragma once

#include <stdint.h>
#include <stdbool.h>

typedef struct VbeInfoBlock {
    char VbeSignature[4];
    uint16_t VbeVersion;
    uint16_t OemStringPtr[2];
    uint8_t Capabilities[4];
    uint16_t VideoModePtr[2];
    uint16_t TotalMemory;
    uint8_t _Reserved[236 + 256];   
} __attribute__((packed)) VbeInfoBlock;

bool VBE_GetControllerInfo(VbeInfoBlock* info);