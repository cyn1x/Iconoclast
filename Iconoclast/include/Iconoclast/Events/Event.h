#pragma once

#include "IconoclastPCH.h"

static_assert(true, "");
#pragma warning(push)
#pragma warning(disable : 4514)

namespace Iconoclast {

    enum class EventType
    {
        None = 0,
        WindowClose,
        WindowResize,
        WindowFocus,
        WindowLostFocus,
        WindowMoved,
        KeyPressed,
        KeyReleased,
        MouseButtonPressed,
        MouseButtonReleased,
        MouseMoved,
        MouseScrolled
    };

    enum EventCategory
    {
        None                     = 0,
        EventCategoryApplication = 1 << 0,
        EventCategoryInput       = 1 << 1,
        EventCategoryKeyboard    = 1 << 2,
        EventCategoryMouse       = 1 << 3,
        EventCategoryMouseButton = 1 << 4
    };

#define EVENT_CLASS_TYPE(type)                                                                     \
    static EventType GetStaticType()                                                               \
    {                                                                                              \
        return EventType::type;                                                                    \
    }                                                                                              \
    virtual EventType GetEventType() const override                                                \
    {                                                                                              \
        return GetStaticType();                                                                    \
    }                                                                                              \
    virtual const char *GetName() const override                                                   \
    {                                                                                              \
        return #type;                                                                              \
    }

#define EVENT_CLASS_CATEGORY(category)                                                             \
    virtual int GetCategoryFlags() const override                                                  \
    {                                                                                              \
        return category;                                                                           \
    }

    class Event
    {
        friend class EventDispatcher;

    public:
        virtual ~Event()
        {
        }

        virtual EventType   GetEventType() const     = 0;
        virtual const char *GetName() const          = 0;
        virtual int         GetCategoryFlags() const = 0;
        virtual std::string ToString() const
        {
            return GetName();
        }

        inline bool IsInCategory(EventCategory category)
        {
            return GetCategoryFlags() & category;
        }

    protected:
#pragma warning(push)
#pragma warning(disable : 4820)
        bool m_Handled = false;
#pragma warning(pop)
    };

    class EventDispatcher
    {
    public:
        EventDispatcher(Event &event) : m_Event(event)
        {
        }

        // Explicitly delete copy constructor and assignment operator
        EventDispatcher(const EventDispatcher &)            = delete;
        EventDispatcher &operator=(const EventDispatcher &) = delete;

        virtual ~EventDispatcher()
        {
        }

        template <typename T> using EventFn = std::function<bool(T &)>;
        template <typename T> bool Dispatch(EventFn<T> func)
        {
            if (m_Event.GetEventType() == T::GetStaticType()) {
                m_Event.m_Handled = func(*(T *)&m_Event);
                return true;
            }
            return false;
        }

    private:
        Event &m_Event;
    };

} // namespace Iconoclast

#pragma warning(pop)
