#include "igspch.h"

namespace IGEngine
{

	StackAllocator::StackAllocator(const size_t StackSize) :
		m_TopMarker(0),
		m_StackSize(StackSize)
	{
		// Allocate the memory for the stack and track the current stack address
		m_pStackBottom = malloc(StackSize);
		m_BottomAddress = reinterpret_cast<uint64_t>(m_pStackBottom);
	}

	StackAllocator::~StackAllocator()
	{
		// Free the stacks memory
		free(m_pStackBottom);
		m_pStackBottom = nullptr;
	}

	void* StackAllocator::Allocate(const size_t Size, const uint8_t Alignment)
	{
		// This will enforce power of 2 alignments
		uint64_t alignedAddr = (m_BottomAddress + Alignment - 1) & ~(Alignment - 1);
		uint64_t newTop = m_TopMarker + Size;

		if (newTop <= m_StackSize)
		{
			m_TopMarker = newTop;
			return reinterpret_cast<void*>(alignedAddr);
		}
		else
		{
			return nullptr;
		}
	}

	PoolAllocator::PoolAllocator(size_t chunksPerBlock) : m_ChunksPerBlock(chunksPerBlock)
	{
	}

	Chunk* PoolAllocator::AllocateBlock(size_t ChunkSize)
	{
		//cout << "\nAllocating block (" << mChunksPerBlock << " chunks):\n\n";

		size_t blockSize = m_ChunksPerBlock * ChunkSize;

		// The first chunk of the new block.
		Chunk* blockBegin = reinterpret_cast<Chunk*>(malloc(blockSize));

		// Once the block is allocated, we need to chain all
		// the chunks in this block:

		Chunk* chunk = blockBegin;

		for (int i = 0; i < m_ChunksPerBlock - 1; ++i) 
		{
			chunk->m_pNext = reinterpret_cast<Chunk*>(reinterpret_cast<char*>(chunk) + ChunkSize);
			chunk = chunk->m_pNext;
		}

		chunk->m_pNext = nullptr;

		return blockBegin;
	}

	/**
	 * Returns the first free chunk in the block.
	 *
	 * If there are no chunks left in the block,
	 * allocates a new block.
	 */
	void* PoolAllocator::Allocate(size_t Size) {

		// No chunks left in the current block, or no any block
		// exists yet. Allocate a new one, passing the chunk size:

		if (m_pAlloc == nullptr) 
		{
			m_pAlloc = AllocateBlock(Size);
		}

		// The return value is the current position of
		// the allocation pointer:

		Chunk* freeChunk = m_pAlloc;

		// Advance (bump) the allocation pointer to the next chunk.
		//
		// When no chunks left, the `mAlloc` will be set to `nullptr`, and
		// this will cause allocation of a new block on the next request:

		m_pAlloc = m_pAlloc->m_pNext;

		return freeChunk;
	}

	/**
	 * Puts the chunk into the front of the chunks list.
	 */
	void PoolAllocator::Deallocate(void* pChunk, size_t Size) 
	{

		// The freed chunk's next pointer points to the
		// current allocation pointer:
		reinterpret_cast<Chunk*>(pChunk)->m_pNext = m_pAlloc;

		// And the allocation pointer is moved backwards, and
		// is set to the returned (now free) chunk:
		m_pAlloc = reinterpret_cast<Chunk*>(pChunk);
	}
};