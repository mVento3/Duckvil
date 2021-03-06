#pragma once

#include "Editor/Widget.h"

#include "Memory/FreeList.h"

#include "RuntimeReflection/RuntimeReflection.h"

#include "Logger/Logger.h"

#include "Editor/Widgets/ConsoleWidget.generated.h"

namespace Duckvil { namespace Editor {

    class ConsoleWidget : public Widget
    {
        DUCKVIL_GENERATED_BODY
    private:
        Memory::FreeList m_heap;

        Memory::Vector<__logger_channel_log_info> m_aLogs;

        bool m_bScrollBottom;

    public:
        ConsoleWidget(const Memory::FreeList& _heap);
        ~ConsoleWidget();

        void InitEditor(void* _pImguiContext);
        void OnDraw();

        void OnEvent(const __logger_channel_log_info& _event);
    };

}}