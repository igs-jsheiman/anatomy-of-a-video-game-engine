#include "igpch.h"
#include "IGSMemory.h"

namespace IGEngine
{

	StackAllocator::StackAllocator(const uint64_t StackSize) :
		uiTopMarker(0),
		uiStackSize(StackSize)
	{
		// Allocate the memory for the stack and track the current stack address
		pStack = malloc(StackSize);
		uiCurrStackAddr = reinterpret_cast<uint64_t>(pStack);
	}

	StackAllocator::~StackAllocator()
	{
		// Free the stacks memory
		free(pStack);
	}

	void* StackAllocator::Allocate(const uint64_t Size, const uint8_t Alignment)
	{
		uint64_t alignedAddr = (uiCurrStackAddr + Alignment - 1) & ~(Alignment - 1);

		if (alignedAddr + Size <= uiStackSize)
		{
			void* pcurrStack = &pStack + alignedAddr;
			uiCurrStackAddr = alignedAddr + Size;
			return pcurrStack;
		}
		else
		{
			return nullptr;
		}
	}
};