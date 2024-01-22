#pragma once

#include <stdint.h>
#include <stdbool.h>

typedef struct PICDriver {
    bool (*Probe)();
    void (*Enable)();
    void (*Disable)();
    void (*sendEndOfInterrupt)(int irq);
    
} PICDriver;


void i8259_Configure(uint8_t offsetPic1, uint8_t offsetPic2, bool autoEoi);
void i8259_SendEndOfInterrupt(int irq);
void i8259_Disable();
void i8259_SetMask(uint16_t mask);
void i8259_Mask(int irq);
void i8259_Unmask(int irq);
uint16_t i8259_ReadIRQRequestRegister();
uint16_t i8259_ReadInServiceRegister();
