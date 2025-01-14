#pragma once

#include <atomic>
#include <memory>

// template<typename T>
// using SmartPointer = std::shared_ptr<T>;
// 
// template <typename T>
// class UniquePtr 
// {
// public:
// 	// Constructor
// 	UniquePtr(T* ptr = nullptr) : m_ptr(ptr) {}
// 
// 	// Destructor
// 	~UniquePtr() 
// 	{
// 		delete m_ptr;
// 	}
// 
// 	// Dereference operator
// 	T& operator*() const 
// 	{
// 		return *m_ptr;
// 	}
// 
// 	// Member access operator
// 	T* operator->() const 
// 	{
// 		return m_ptr;
// 	}
// 
// 	// Get the raw pointer
// 	T* get() const 
// 	{
// 		return m_ptr;
// 	}
// 
// private:
// 	T* m_ptr;
// 
// 	// Disallow copy constructor and assignment operator
// 	UniquePtr(const UniquePtr&) = delete;
// 	UniquePtr& operator=(const UniquePtr&) = delete;
// };

namespace IGEngine
{
	class RefCounted
	{
	protected:
		virtual ~RefCounted() = default;

	public:
		RefCounted() : m_count(0), m_weak_count(0) {}
		RefCounted(const RefCounted&) {}
		RefCounted& operator=(const RefCounted&)
		{
			return *this;
		}

		int RefCount() const
		{
			return m_count.load();
		}

		void IncrementRefCount() noexcept
		{
			++(m_count);
		}

		bool DecrementRefCount() noexcept
		{
			bool cleanup = false;
			{
				auto result = --(m_count);
				if (result == 0)
				{
					if (m_weak_count == 0)
					{
						cleanup = true;
					}
				}
			}
			return cleanup;
		}

		void IncrementWeakRefCount() noexcept
		{
			++(m_weak_count);
		}

		bool DecrementWeakRefCount() noexcept
		{
			bool cleanup = false;
			auto result = --(m_weak_count);
			if (result == 0 && m_count == 0)
			{
				cleanup = true;
			}
			return cleanup;
		}

		int StrongRefCount()
		{
			return m_count.load();
		}

		int WeakRefCount()
		{
			return m_weak_count.load();
		}

	private:
		std::atomic<int> m_count;
		std::atomic<int> m_weak_count;
	};

	template <typename T>
	class SmartPtr;

// 	template <class T>
// 	class WeakPtr;

	template <typename T>
	using Ref = SmartPtr<T>;

// 	template <typename T>
// 	using WeakRef = WeakPtr<T>;
// 
// 	template <typename T>
// 	using Scope = std::unique_ptr<T>;

	// Intrusive smart pointer class
	template <typename T>
	class SmartPtr : public RefCounted
	{
	public:
		SmartPtr() noexcept
		{
			IncrementRefCount();
		}
		SmartPtr(std::nullptr_t) noexcept {}
		SmartPtr(T* ptr) noexcept(noexcept(IncrementRefCount())) : m_ptr(ptr)
		{
			IncrementRefCount();
		}

		SmartPtr(const SmartPtr& other) noexcept(noexcept(IncrementRefCount())) : m_ptr(other.m_ptr)
		{
			IncrementRefCount();
		}

		SmartPtr(SmartPtr&& other) noexcept : m_ptr(other.detach()) {}

		template <class U, typename = std::enable_if_t<std::convertible_to<U*, T*>>>
		SmartPtr(const SmartPtr<U>& other) noexcept(noexcept(IncrementRefCount())) : m_ptr(other.get())
		{
			IncrementRefCount();
		}

		template <class U, typename = std::enable_if_t<std::convertible_to<U*, T*>>>
		SmartPtr(SmartPtr<U>&& other) noexcept : m_ptr(other.detach())
		{
		}

// 		SmartPtr(const WeakPtr<T>& other) noexcept
// 		{
// 			SmartPtr ptr = other.lock();
// 			ptr.swap(*this);
// 		}

		SmartPtr& operator=(const SmartPtr& other) noexcept
		{
			if (this != &other)
			{
				if (DecrementRefCount())
				{
					delete m_ptr;
				}
				m_ptr = other.m_ptr;
				IncrementRefCount();
			}
			return *this;
		}

		SmartPtr& operator=(SmartPtr&& other) noexcept
		{
			if (this != &other)
			{
				if (DecrementRefCount())
				{
					delete m_ptr;
				}
				m_ptr = other.detach();
			}
			return *this;
		}

		template <class U, typename = std::enable_if_t<std::convertible_to<U*, T*>>>
		SmartPtr& operator=(const SmartPtr<U>& other) noexcept
		{
			SmartPtr<T> p(other);
			p.swap(*this);
			return *this;
		}

		template <class U, typename = std::enable_if_t<std::convertible_to<U*, T*>>>
		SmartPtr& operator=(SmartPtr<U>&& other) noexcept
		{
			if (DecrementRefCount())
			{
				delete m_ptr;
			}
			m_ptr = other.detach();
			return *this;
		}

		SmartPtr& operator=(T* raw_ptr) noexcept
		{
			if (m_ptr != raw_ptr)
			{
				if (DecrementRefCount())
				{
					delete m_ptr;
				}
				m_ptr = raw_ptr;
				IncrementRefCount();
			}
			return *this;
		}

		void reset(T* newPtr = nullptr) noexcept
		{
			if (m_ptr != newPtr)
			{
				if (DecrementRefCount())
				{
					delete m_ptr;
				}
				m_ptr = newPtr;
				IncrementRefCount();
			}
		}

		void swap(SmartPtr& other) noexcept
		{
			T* temp = m_ptr;
			m_ptr = other.m_ptr;
			other.m_ptr = temp;
		}

		void swapValue(SmartPtr& other) noexcept
		{
			T temp = *m_ptr;
			*m_ptr = *(other.m_ptr);
			*(other.m_ptr) = temp;
		}

		T* detach() noexcept
		{
			T* current = m_ptr;
			m_ptr = nullptr;
			return current;
		}

// 		void attach(T* ptr) noexcept(noexcept(SmartPtr<T>(ptr)))
// 		{
// 			SmartPtr<T> p(ptr);
// 			DecrementRefCount(p.m_ptr); // reset the count back to original since SmartPtr will increment it
// 			p.swap(*this);
// 		}

		T* get() const noexcept
		{
			return m_ptr;
		}

		T* operator->() const noexcept
		{
			return m_ptr;
		}

		T& operator*() const noexcept
		{
			return *m_ptr;
		}

		bool operator!() const noexcept
		{
			return !m_ptr;
		}

		explicit operator bool() const noexcept
		{
			return m_ptr != nullptr;
		}

		bool operator==(std::nullptr_t) const noexcept
		{
			return m_ptr == nullptr;
		}

		bool operator!=(std::nullptr_t) const noexcept
		{
			return m_ptr != nullptr;
		}

		bool operator==(const T* raw_ptr) const noexcept
		{
			return m_ptr == raw_ptr;
		}

		bool operator!=(const T* raw_ptr) const noexcept
		{
			return m_ptr != raw_ptr;
		}

		bool operator==(const SmartPtr& other) const noexcept
		{
			return m_ptr == other.m_ptr;
		}

		bool operator!=(const SmartPtr& other) const noexcept
		{
			return m_ptr != other.m_ptr;
		}

		bool operator<(const SmartPtr& other) const noexcept
		{
			return m_ptr < other.m_ptr;
		}

		bool operator>(const SmartPtr& other) const noexcept
		{
			return m_ptr > other.m_ptr;
		}

		bool operator<=(const SmartPtr& other) const noexcept
		{
			return m_ptr <= other.m_ptr;
		}

		bool operator>=(const SmartPtr& other) const noexcept
		{
			return m_ptr >= other.m_ptr;
		}

// 		WeakPtr<T> Weak() const noexcept
// 		{
// 			IncrementWeakRefCount();
// 			return weak_ptr;
// 		}

		long count() const noexcept
		{
			return StrongRefCount();
		}

		virtual ~SmartPtr()
		{
			if (DecrementRefCount())
			{
				delete [] m_ptr;
			}
		}

	private:
		T* m_ptr = nullptr;
	};

	template <typename T>
	void swap(SmartPtr<T>& lhs, SmartPtr<T>& rhs) noexcept
	{
		lhs.swap(rhs);
	}

	template <typename T>
	void swapValue(SmartPtr<T>& lhs, SmartPtr<T>& rhs) noexcept
	{
		lhs.swapValue(rhs);
	}

// 	template <typename T, typename... Args>
// 	SmartPtr<T> make_smartptr(Args&&... args)
// 	{
// 		return SmartPtr<T>(new T(std::forward<Args>(args)...));
// 	}

// 	template <class T>
// 	class WeakPtr
// 	{
// 		friend class SmartPtr<T>;
// 
// 	public:
// 		WeakPtr() noexcept = default;
// 		WeakPtr(std::nullptr_t) noexcept {}
// 		WeakPtr(const WeakPtr& other) noexcept(noexcept(T::IncrementWeakRefCount(m_ptr))) : m_ptr(other.m_ptr)
// 		{
// 			T::IncrementWeakRefCount(m_ptr);
// 		}
// 		WeakPtr(const SmartPtr<T>& other) noexcept
// 		{
// 			WeakPtr<T> weak_ptr = other.Weak();
// 			weak_ptr.swap(*this);
// 		}
// 		~WeakPtr()
// 		{
// 			T::DecrementWeakRefCount(m_ptr);
// 		}
// 
// 		template <class U, typename = std::enable_if_t<std::convertible_to<U*, T*>>>
// 		WeakPtr(const SmartPtr<U>& other) noexcept
// 		{
// 			WeakPtr<T> weak_ptr = other.Weak();
// 			weak_ptr.swap(*this);
// 		}
// 
// 		WeakPtr(WeakPtr&& other) noexcept
// 		{
// 			std::swap(m_ptr, other.m_ptr);
// 		}
// 
// 		template <class U, typename = std::enable_if_t<std::convertible_to<U*, T*>>>
// 		WeakPtr(WeakPtr<U>&& other) noexcept
// 		{
// 			std::swap(m_ptr, other.m_ptr);
// 		}
// 
// 		WeakPtr& operator=(const WeakPtr& other) noexcept
// 		{
// 			if (this != &other)
// 			{
// 				T::DecrementWeakRefCount(m_ptr);
// 				m_ptr = other.m_ptr;
// 				T::IncrementWeakRefCount(m_ptr);
// 			}
// 			return *this;
// 		}
// 
// 		template <class U, typename = std::enable_if_t<std::convertible_to<U*, T*>>>
// 		WeakPtr& operator=(const WeakPtr<U>& other) noexcept
// 		{
// 			if (this != &other)
// 			{
// 				T::DecrementWeakRefCount(m_ptr);
// 				m_ptr = other.m_ptr;
// 				T::IncrementWeakRefCount(m_ptr);
// 			}
// 			return *this;
// 		}
// 
// 		template <class U, typename = std::enable_if_t<std::convertible_to<U*, T*>>>
// 		WeakPtr& operator=(const SmartPtr<U>& other) noexcept
// 		{
// 			WeakPtr weak_ptr = other.Weak();
// 			weak_ptr.swap(*this);
// 			return *this;
// 		}
// 
// 		WeakPtr& operator=(WeakPtr&& other) noexcept
// 		{
// 			other.swap(*this);
// 			return *this;
// 		}
// 
// 		template <class U, typename = std::enable_if_t<std::convertible_to<U*, T*>>>
// 		WeakPtr& operator=(WeakPtr<U>&& other) noexcept
// 		{
// 			other.swap(*this);
// 			return *this;
// 		}
// 
// 		void reset() noexcept
// 		{
// 			WeakPtr().swap(*this);
// 		}
// 
// 		void swap(WeakPtr& other) noexcept
// 		{
// 			std::swap(m_ptr, other.m_ptr);
// 		}
// 
// 		bool expired() const noexcept
// 		{
// 			return lock() == nullptr;
// 		}
// 
// 		SmartPtr<T> lock() const noexcept
// 		{
// 			return (T::StrongRefCount(m_ptr) > 0) ? SmartPtr<T>(m_ptr) : SmartPtr<T>();
// 		}
// 
// 	private:
// 		T* m_ptr = nullptr;
// 	};

} // namespace IGEngine