#pragma once

// Simple base class to inherit from when only a single instance of the class is necessary.
// Disallows constructor/destructor public usage and removed ability to copy/move the single instance.
// e.g. - Class A : public Singleton<A>

template <typename T>
class Singleton
{
	public:

		static T* GetInstance() 
		{
			static T Instance;
			return &Instance;
		}

	protected:
		Singleton() = default;
		~Singleton() = default;

		Singleton(const Singleton&) = delete;
		Singleton& operator=(const Singleton&) = delete;
};