#ifndef __NVM_H
#define __NVM_H

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#include <Nvm_Cfg.h>

void NVM_Init(void);
void NVM_Deinit(void);
void NVM_Loop(void);

bool NVM_WriteDataToNvm(NVM_BlockId blockId, void* data);
bool NVM_ReadDataFromRam(NVM_BlockId blockId, void* data);
bool NVM_ReadDataFromRom(NVM_BlockId blockId, void* data);
bool NVM_ReadDataFromNvm(NVM_BlockId blockId, void* data);

bool NVM_IsAllBlockCompleted(void);

#endif // __NVM_H