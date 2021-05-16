#pragma once

#include "Utils/Macro.h"

#include "Memory/FreeList.h"
#include "Memory/Vector.h"
#include "Memory/Queue.h"

#include <mutex>
#include <thread>
#include <future>
#include <type_traits>

#include "Event/ImmediateChannel.h"

namespace Duckvil { namespace Thread {

    typedef void (*TaskCallback)(void*);

    struct task
    {
        TaskCallback m_callback;
        void* m_pData;
    };

    struct pool_data
    {
        uint32_t m_uiThreadsCount;
        Memory::Vector<std::thread*> m_aWorkers;
        Memory::FreeList m_heap;
        std::mutex m_lock;
        std::mutex m_threadPoolLock;
        std::condition_variable m_condition;
        Memory::Queue<task> m_aTasks;
        bool m_bRunning;
        bool m_bTerminate;
        std::atomic<uint32_t> m_uiTaskCount;
    };

    struct pool_ftable
    {
        pool_data* (*m_fnInit)(const Memory::FreeList& _heap);
        void (*m_fnStart)(pool_data*);
        void (*m_fnTerminate)(pool_data*);
        void (*m_fnOrderTask)(pool_data*, TaskCallback _task);
        void (*m_fnOrderDataTask)(pool_data*, TaskCallback _task, void* _pTaskData);
        bool (*m_fnRemainingTasks)(pool_data*);
        uint32_t (*m_fnGetTaskCount)(pool_data*);
    };

    template <typename F, typename... Args>
    auto order_task(pool_ftable* _pThreadPool, pool_data* _pThreadPoolData, F&& f, Args&&... args) -> std::future<typename std::invoke_result<F, Args...>::type>
    {
        using return_type = typename std::invoke_result<F, Args...>::type;

        auto task = std::make_shared<std::packaged_task<return_type()>>(std::bind(std::forward<F>(f), std::forward<Args>(args)...));

        std::future<return_type> res = task->get_future();

        _pThreadPool->m_fnOrderTask(_pThreadPoolData, Utils::lambda([task](){ (*task)(); }));

        return res;
    }

}}

DUCKVIL_EXPORT Duckvil::Thread::pool_ftable* duckvil_thread_pool_init();