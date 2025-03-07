#ifndef __NVM_H
#define __NVM_H

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#include <Nvm_Cfg.h>

typedef uint16_t NVM_BlockId;

typedef struct
{
	uint16_t pageId;
	uint16_t byteOffset;
	uint16_t byteLength;
	void* romData;
	void* ramData;
	bool readAll;
	bool writeAll;
} NVM_BlockDescriptor;

void NVM_Init(void);
void NVM_Deinit(void);
void NVM_Loop(void);

bool NVM_WriteData(NVM_BlockId blockId, void* data);
bool NVM_ReadData(NVM_BlockId blockId, void* data);
bool NVM_IsAllBlockCompleted(void);

#endif // __NVM_H