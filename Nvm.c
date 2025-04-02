#include <Nvm.h>

static bool NVM_IsDataEmpty(uint8_t* data, uint32_t dataSize);
static uint32_t NVM_GetAddressByPageId(NVM_PageId pageId);
static void NVM_LoadData(void);
void NVM_Init(void)
{
	NVM_LoadData();
}
void NVM_Deinit(void)
{
}
void NVM_Loop(void)
{
	// Do nothing
}

bool NVM_WriteDataToNvm(NVM_BlockId blockId, void* data)
{
	NVM_PageId pageId;
	uint32_t pageAddress;
	uint8_t pageData[NVM_PAGE_SIZE];
	uint16_t blockByteOffset;
	uint16_t blockByteLength;
	void* ramDataPtr;

	// Get prop
	pageId = NVM_blockDescriptorTable[blockId].pageId;
	blockByteOffset = NVM_blockDescriptorTable[blockId].byteOffset;
	blockByteLength = NVM_blockDescriptorTable[blockId].byteLength;
	ramDataPtr = NVM_blockDescriptorTable[blockId].ramData;

	// Update RAM data
	memcpy(ramDataPtr, data, blockByteLength);

	// Get Page Address
	pageAddress = NVM_GetAddressByPageId(pageId);

	// Read Page data from NVM
	NVM_Read(pageAddress, pageData, NVM_PAGE_SIZE);

	// Check if current data difference with nvm data
	if(memcmp(data, &pageData[blockByteOffset], blockByteLength) == 0)
	{
		// No difference -> Nothing to do
	}
	else
	{
		// Update data to page data
		memcpy(&pageData[blockByteOffset], data, blockByteLength);
		// Erase NVM data first
		NVM_Erase(pageAddress, NVM_PAGE_SIZE);
		// Write data to NVM
		NVM_Write(pageAddress, pageData, NVM_PAGE_SIZE);
	}
}

bool NVM_ReadDataFromRam(NVM_BlockId blockId, void* data)
{
	uint16_t blockByteOffset = NVM_blockDescriptorTable[blockId].byteOffset;
	uint16_t blockByteLength = NVM_blockDescriptorTable[blockId].byteLength;
	void* blockRamDataPtr = NVM_blockDescriptorTable[blockId].ramData;

	// Copy data from ROM
	memcpy(data, blockRamDataPtr, blockByteLength);
}
bool NVM_ReadDataFromRom(NVM_BlockId blockId, void* data)
{
	uint16_t blockByteOffset = NVM_blockDescriptorTable[blockId].byteOffset;
	uint16_t blockByteLength = NVM_blockDescriptorTable[blockId].byteLength;
	void* blockRomDataPtr = NVM_blockDescriptorTable[blockId].romData;

	// Copy data from ROM
	memcpy(data, blockRomDataPtr, blockByteLength);
}
bool NVM_ReadDataFromNvm(NVM_BlockId blockId, void* data)
{
	uint8_t pageData[NVM_PAGE_SIZE];
	uint32_t pageAddress;

	NVM_PageId pageId = NVM_blockDescriptorTable[blockId].pageId;
	uint16_t blockByteOffset = NVM_blockDescriptorTable[blockId].byteOffset;
	uint16_t blockByteLength = NVM_blockDescriptorTable[blockId].byteLength;

	// Get Page Address
	pageAddress = NVM_GetAddressByPageId(pageId);

	// Read Page data from NVM
	NVM_Read(pageAddress, pageData, NVM_PAGE_SIZE);

	// Copy data from page
	memcpy(data, &pageData[blockByteOffset], blockByteLength);

	return true;
}
bool NVM_IsAllBlockCompleted(void)
{
	return true;
}

static uint32_t NVM_GetAddressByPageId(NVM_PageId pageId)
{
	return NVM_ADDRESS_BASE + pageId * NVM_PAGE_SIZE;
}
static void NVM_LoadData(void)
{
	uint8_t pageData[NVM_PAGE_SIZE];
	uint32_t pageAddress;
	uint16_t blockByteOffset;
	uint16_t blockByteLength;
	void* blockRamDataPtr;
	void* blockRomDataPtr;
	bool blockReadAll;

	// Loop for all page
	for(NVM_PageId pageId = 0; pageId < NVM_NO_PAGE; pageId++)
	{
		pageAddress = NVM_GetAddressByPageId(pageId);

		// Read page data from NVM
		NVM_Read(pageAddress, pageData, NVM_PAGE_SIZE);

		for(NVM_BlockId blockId = 0; blockId < NVM_pageDescriptorTable[pageId].noBlock; blockId++)
		{
			blockByteOffset = NVM_blockDescriptorTable[blockId].byteOffset;
			blockByteLength = NVM_blockDescriptorTable[blockId].byteLength;
			blockRamDataPtr = NVM_blockDescriptorTable[blockId].ramData;
			blockRomDataPtr = NVM_blockDescriptorTable[blockId].romData;
			blockReadAll = NVM_blockDescriptorTable[blockId].readAll;

			// Check if NVM Read all is 1
			if(blockReadAll)
			{
				// Check if Data in Nvm is empty
				if(NVM_IsDataEmpty(&pageData[blockByteOffset], blockByteLength))
				{
					// Load data from ROM
					memcpy(blockRamDataPtr, blockRomDataPtr, blockByteLength);
				}
				else
				{
					// Load data from NVM
					memcpy(blockRamDataPtr, &pageData[blockByteOffset], blockByteLength);
				}
			}
			else
			{
				// Do nothing if it's set as Read all
			}
		}
	}
}

static bool NVM_IsDataEmpty(uint8_t* data, uint32_t dataSize)
{
	for(uint32_t i = 0; i < dataSize; i++)
	{
		if(data[i] != 0xFF)
		{
			return false;
		}
	}
	return true;
}
