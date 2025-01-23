#pragma once

namespace IGEngine
{
	class StackAllocator
	{
		public:

			// Can only create memory through "New" function
			StackAllocator() = delete;
			explicit StackAllocator(uint64_t StackSize);
			~StackAllocator();

			void* Allocate(uint64_t Size, uint8_t Alignment);

			// Creates a new object on the stack allocator
			template<typename T, typename... args>
			T* New(args...);

			template<typename T>
			T* NewArray(uint64_t ArrayLength, uint8_t Alignment);

			inline void Free(uint64_t Marker = 0)
			{
				// By default, set the top marker back to 0 which clears the whole stack allocator to it's bottom.
				// Alternatively, clear only a portion of the stack by setting the top marker to a specific location
				uiTopMarker = Marker;
			}

		private:

			void* pStack;
			uint64_t uiStackSize;
			uint64_t uiTopMarker;
			uint64_t uiBottomMarker;
			uint64_t uiCurrStackAddr;
	};

	template<typename T, typename... args>
	T* StackAllocator::New(args... argList)
	{
		const uint8_t Alignment = 16;
		void* pMem = Allocate(sizeof(T), Alignment);
		return new (pMem) T(argList...);
	}

	template<typename T>
	T* StackAllocator::NewArray(uint64_t ArrayLength, uint8_t Alignment)
	{
		void* pAlloc = Allocate(sizeof(T) * ArrayLength, Alignment);
		uint8_t* pAllocAddress = static_cast<uint8_t*>(pAlloc);

		for (uint32_t i = 0; i < ArrayLength; ++i)
		{
			new (pAllocAddress + i * sizeof(T)) T;
		}

		return static_cast<T*>(pAlloc);
	}


	class MemoryManager : public Singleton<MemoryManager>
	{
	public:

		void InitializeAllocators()
		{
		}

	};

};

