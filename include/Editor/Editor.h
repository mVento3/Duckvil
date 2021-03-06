#pragma once

#include "Utils/Macro.h"

#include "Memory/FreeListAllocator.h"
#include "Memory/FreeList.h"

#include "Window/IWindow.h"

#include "Editor/Draw.h"

#include "RuntimeReflection/RuntimeReflection.h"

#include "Event/ImmediatePool.h"

#include "HotReloader/Events/SwapEvent.h"

#include "HotReloader/RuntimeCompilerSystem.h"

#include "Graphics/Renderer/Renderer.h"

namespace Duckvil { namespace Editor {

    struct ImGuiEditorData
    {
#ifdef DUCKVIL_HOT_RELOADING
        Memory::Vector<Editor::HotDraw> m_aHotDraws;
#endif
        Memory::Vector<Editor::Draw> m_aDraws;
        void* _ctx;
        Event::Pool<Event::mode::immediate> m_pEditorEvents;
        Memory::FreeList m_heap;

        Memory::Vector<uint32_t> m_aTextures;

        Graphics::Renderer::renderer_ftable* m_pRenderer;
        Graphics::Renderer::renderer_data* m_pRendererData;

        Event::Pool<Event::mode::buffered>* m_pWindowEventPool;

        // Store viewports, and retrieve them
    };

    struct EditorFTable
    {
        void* (*m_fnInit)(Duckvil::Memory::ftable* _pMemoryInterface, Duckvil::Memory::free_list_allocator* _pAllocator, Window::IWindow* _pWindow, Graphics::Renderer::renderer_ftable* _pRenderer, Graphics::Renderer::renderer_data* _pRendererData);
        void (*m_fnRender)(void* _pData, Window::IWindow* _pWindow);

        void (*m_fnAddHotDraw)(void* _pData, HotDraw _draw);
        void (*m_fnAddDraw)(void* _pData, Draw _draw);
        // void (*m_fnRemoveDraw)(void* _pData, void* _pObject);

        void (*m_fnSetWindowEventPool)(void* _pData, Event::Pool<Event::mode::buffered>*);

        void (*m_fnPostInit)(const Memory::FreeList& _heap, void* _pData, EditorFTable* _pEditor, Event::Pool<Event::mode::immediate>* _pEventPool, HotReloader::RuntimeCompilerSystem* _pHotReloader);
        void (*m_fnHotReloadInit)(ImGuiEditorData* _pData, const HotReloader::SwapEvent& _event);
    };

}}

DUCKVIL_EXPORT Duckvil::Editor::EditorFTable* duckvil_editor_init();