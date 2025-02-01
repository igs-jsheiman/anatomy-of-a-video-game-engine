#pragma once

namespace IGEngine
{
	////////////////////////////////////////////////////////////////////////////
	//////////////////// STACK ALLOCATOR ///////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////

	class StackAllocator
	{
		public:

			// Can only create memory through "New" function
			StackAllocator() = delete;
			explicit StackAllocator(size_t StackSize);
			~StackAllocator();

			void* Allocate(size_t Size, uint8_t Alignment);

			// Creates a new object on the stack allocator
			template<typename T, typename... args>
			T* New(args...);

			template<typename T>
			T* NewArray(size_t ArrayLength, uint8_t Alignment);

			inline void Deallocate(uint64_t Marker = 0)
			{
				// By default, set the top marker back to 0 which clears the whole stack allocator to it's bottom.
				// Alternatively, clear only a portion of the stack by setting the top marker to a specific location
				m_TopMarker = Marker;
			}

		private:

			size_t		m_StackSize;
			uint64_t	m_TopMarker;
			uint64_t	m_BottomAddress;
			void*		m_pStackBottom;
	};

	template<typename T, typename... args>
	T* StackAllocator::New(args... argList)
	{
		const uint8_t Alignment = 16;
		void* pMem = Allocate(sizeof(T), Alignment);
		return new (pMem) T(argList...);
	}

	template<typename T>
	T* StackAllocator::NewArray(size_t ArrayLength, uint8_t Alignment)
	{
		void* pAlloc = Allocate(sizeof(T) * ArrayLength, Alignment);
		uint8_t* pAllocAddress = static_cast<uint8_t*>(pAlloc);

		for (uint32_t i = 0; i < ArrayLength; ++i)
		{
			new (pAllocAddress + i * sizeof(T)) T;
		}

		return static_cast<T*>(pAlloc);
	}

	////////////////////////////////////////////////////////////////////////////
	//////////////////// POOL ALLOCATOR ////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////

	struct Chunk
	{
		Chunk* m_pNext;
	};

	class PoolAllocator 
	{
	public:
		PoolAllocator(size_t ChunksPerBlock);

		void* Allocate(size_t Size);
		void Deallocate(void* pChunk, size_t Size);

	private:

		//Number of chunks per larger block.
		size_t m_ChunksPerBlock;

		 //Allocation pointer.
		Chunk* m_pAlloc = nullptr;

		 //Allocates a larger block (pool) for chunks.
		Chunk* AllocateBlock(size_t ChunkSize);
	};

	////////////////////////////////////////////////////////////////////////////
	//////////////////// OBJECT DATA ///////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////

	/**
	* The `Object` structure uses custom allocator,
	* overloading `new`, and `delete` operators.
	*/
	struct Object 
	{
		// Object data, 16 bytes:
		uint64_t Data[2];

		// Declare out custom allocator for the `Object` structure:
		static PoolAllocator allocator;

		static void* operator new(size_t Size) 
		{
			// Overload new operator to allocate the memory from our pool
			return allocator.Allocate(Size);
		}

		static void operator delete(void* ptr, size_t Size) 
		{
			// Overload delete operator to deallocate the memory from our pool
			return allocator.Deallocate(ptr, Size);
		}
	};

	////////////////////////////////////////////////////////////////////////////
	//////////////////// MEMORY MANAGER ////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////

	class MemoryManager : public Singleton<MemoryManager>
	{
	public:

		MemoryManager()
		{
			InitializeAllocators();
		}

		~MemoryManager()
		{
			delete m_pSingleFrameAlloc;
		}

		void InitializeAllocators()
		{
			m_pSingleFrameAlloc = new StackAllocator(1024);
		}

		template <typename T>
		T* SingleFrameAllocate(uint64_t Size, uint8_t Alignment)
		{
			return reinterpret_cast<T*>(m_pSingleFrameAlloc->Allocate(Size, Alignment));
		}

		void ClearStackAllocator()
		{
			m_pSingleFrameAlloc->Deallocate();
		}

	private:
		
		StackAllocator* m_pSingleFrameAlloc;
	};

	static MemoryManager g_MemoryManager;

};

