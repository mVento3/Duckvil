#pragma once

#include <mutex>
#include <functional>
#include <queue>
#include <stack>

#include "Memory/Vector.h"
#include "Memory/FreeList.h"

#include "RuntimeReflection/RuntimeReflection.h"

namespace Duckvil { namespace Event {

    enum class mode
    {
        immediate = 1 << 0, // Call function right after broadcast
        buffered = 1 << 1, // Buffer the message for later use
        mixed = immediate | buffered
    };

    struct IChannel
    {

    };

    struct event_lookup
    {
        size_t m_ullMessageTypeID;
        mode m_mode;
        IChannel* m_pChannel;
    };

    template <typename Message, mode Blocking>
    class Channel
    {
    public:
        Channel() = delete;
        ~Channel() = delete;
    };

}}