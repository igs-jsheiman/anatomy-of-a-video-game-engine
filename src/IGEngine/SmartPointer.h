#pragma once

typedef std::shared_ptr<char[]> StrSmartPtr;

template <typename T>
class UniquePtr 
{
public:
	// Constructor
	UniquePtr(T* ptr = nullptr) : m_ptr(ptr) {}

	// Destructor
	~UniquePtr() 
	{
		delete m_ptr;
	}

	// Dereference operator
	T& operator*() const 
	{
		return *m_ptr;
	}

	// Member access operator
	T* operator->() const 
	{
		return m_ptr;
	}

	// Get the raw pointer
	T* get() const 
	{
		return m_ptr;
	}

private:
	T* m_ptr;

	// Disallow copy constructor and assignment operator
	UniquePtr(const UniquePtr&) = delete;
	UniquePtr& operator=(const UniquePtr&) = delete;
};