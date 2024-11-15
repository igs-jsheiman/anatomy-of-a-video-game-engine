#pragma once

namespace IGEngine
{

	enum class EventType
	{
		None = 0,
		WindowClose,
		WindowResize,
		WindowFocus,
		WindowLostFocus,
		WindowMoved,
		AppTick,
		AppUpdate,
		AppRender,
		KeyPressed,
		KeyReleased,
		MouseButtonPressed,
		MouseButtonReleased,
		MouseMoved,
		MouseScrolled
	};

	enum EventCategory
	{
		None = 0,
		EventCategoryApplication = 1 << 0,
		EventCategoryInput = 1 << 1,
		EventCategoryKeyboard = 1 << 2,
		EventCategoryMouse = 1 << 3,
		EventCategoryMouseButton = 1 << 4
	};

	// Helper macros for defining event types and categories...
#define EVENT_CLASS_TYPE(type)	static EventType GetStaticType() { return EventType::##type; }\
								virtual EventType GetEventType() const override { return GetStaticType(); }\
								virtual const char* GetName() const override { return #type; }

#define EVENT_CLASS_CATEGORY(category) virtual int32_t GetCategoryFlags() const override { return category; }


	class IGENGINE_API Event
	{
		friend class EventDispatcher;

	public:

		virtual EventType GetEventType()	const = 0;
		virtual const char* GetName()		const = 0;
		virtual int32_t GetCategoryFlags()	const = 0;

		inline bool IsInCategory(EventCategory Category)
		{
			return GetCategoryFlags() & Category;
		}

	protected:
		bool EventHandled_ = false;
	};

	class EventDispatcher
	{
		template<typename T>
		using EventFn = std::Function<bool(T&)>;

	public:
		EventDispatcher(Event& Event) : Event_(Event) {}

		template<typename T>
		bool Dispatch(Event<T> Func)
		{
			if (Event_.GetEventType() == T::GetStaticType())
			{
				Event_.EventHandled_ = Func(*(T*)&Event_);
				return true;
			}
		}
	private:
		Event Event_;
	};

	class KeyEvent : public Event
	{
	public:
		inline int GetKeyCode() const { return KeyCode_; }

		EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput)
	protected:
		KeyEvent(int KeyCode) : KeyCode_(KeyCode) {}

		int32_t KeyCode_;
	};

	class IGENGINE_API KeyPressedEvent : public KeyEvent
	{
	public:

		KeyPressedEvent(int32_t KeyCode, int32_t RepeatCount) :
			KeyCode_(KeyCode),
			RepeatCount_(RepeatCount)
		{
		}

		inline int32_t GetRepeatCount() const { return RepeatCount_; }

		static EventType GetStaticType() { return EventType::KeyPressed; }
		virtual EventType GetEventType() const override { return GetStaticType(); }
		virtual const char* GetName() const override { return "KeyPressed"; }

		EVENT_CLASS_TYPE(KeyPressed)
	private:
		int32_t RepeatCount_;
	};

	class IGENGINE_API KeyReleasedEvent : public KeyEvent
	{
	public:
		KeyReleasedEvent(int32_t KeyCode) : KeyCode_(KeyCode) {}

		EVENT_CLASS_TYPE(KeyReleased)
	};


	class IGENGINE_API MouseMoveEvent : public Event
	{
	public:
		MouseMoveEvent(float X, float Y) : MouseX_(X), MouseY_(Y) {}

		inline float GetX() const { return MouseX_; }
		inline float GetY() const { return MouseY_; }

		EVENT_CLASS_TYPE(MouseMoved)
			EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)

	private:
		float MouseX_, MouseY_;
	};

	class IGENGINE_API MouseScrollEvent : public Event
	{
	public:
		MouseScrollEvent(float XOffset, float YOffset) : XOffset_(XOffset), YOffset_(YOffset) {}

		inline float GetXOffset() const { return XOffset_; }
		inline float GetYOffset() const { return YOffset_; }

		EVENT_CLASS_TYPE(MouseScrollEvent)
			EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
	};

	class IGENGINE_API MouseButtonEvent : public Event
	{
	public:

		inline int32_t GetMouseButton() const { return MouseButton_; }

		EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)

	protected:
		MouseButtonEvent(int32_t Button) : MouseButton_(Button) {}

	private:
		int32_t MouseButton_;
	};

	class IGENGINE_API MouseButtonPressedEvent : public MouseButtonEvent
	{
	public:
		MouseButtonPressedEvent(int32_t Button) : MouseButtonEvent(Button) {}

		EVENT_CLASS_TYPE(MouseButtonPressed)
	};

	class IGENGINE_API MouseButtonReleasedEvent : public MouseButtonEvent
	{
	public:
		MouseButtonReleasedEvent(int32_t Button) : MouseButtonEvent(Button) {}

		EVENT_CLASS_TYPE(MouseButtonReleased)
	};

	class WindowResizeEvent : public Event
	{
	public:
		WindowResizeEvent(unsigned int Width, unsigned int Height) : Width_(width), Height_(height) {}

		unsigned int GetWidth() const { return Width_; }
		unsigned int GetHeight() const { return Height_; }

		EVENT_CLASS_TYPE(WindowResize)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	private:
		unsigned int Width_, Height_;
	};

	class WindowCloseEvent : public Event
	{
	public:
		WindowCloseEvent() = default;

		EVENT_CLASS_TYPE(WindowClose)
			EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};

	class AppTickEvent : public Event
	{
	public:
		AppTickEvent() = default;

		EVENT_CLASS_TYPE(AppTick)
			EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};

	class AppUpdateEvent : public Event
	{
	public:
		AppUpdateEvent() = default;

		EVENT_CLASS_TYPE(AppUpdate)
			EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};

	class AppRenderEvent : public Event
	{
	public:
		AppRenderEvent() = default;

		EVENT_CLASS_TYPE(AppRender)
			EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};
}