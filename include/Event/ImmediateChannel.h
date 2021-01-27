#pragma once

#include "Event/Channel.h"

namespace Duckvil { namespace Event {

    struct reflected_event
    {
        RuntimeReflection::__duckvil_resource_function_t m_functionHandle;
        RuntimeReflection::__duckvil_resource_type_t m_typeHandle;
        void* m_pObject;
    };

    // Invoked immediately when broadcasted
    template <typename Message>
    class Channel<Message, mode::immediate> : public IChannel
    {
    public:
        typedef void (*Callback)(const Message& _message);

        struct imember_event
        {
            virtual void Invoke(const Message& _message) = 0;
        };

        template <typename Type>
        struct member_event : public imember_event
        {
            typedef void (Type::*Callback)(const Message&);

            member_event(Type* _pObject, const Callback& _fn) :
                m_pObject(_pObject),
                m_fn(_fn)
            {

            }

            Type* m_pObject;
            Callback m_fn;

            void Invoke(const Message& _message) override
            {
                (m_pObject->*(m_fn))(_message);
            }
        };

    private:
        Memory::Vector<reflected_event> m_aRefelctedEvents;
        std::vector<Callback> m_aCallbackEvents;
        std::vector<imember_event*> m_aMemberEvents;

        RuntimeReflection::__data* m_pReflectionData;

    public:
        Channel()
        {

        }

        Channel(const Memory::FreeList& _heap, RuntimeReflection::__data* _pReflectionData) :
            m_pReflectionData(_pReflectionData)
        {
            _heap.Allocate(m_aRefelctedEvents, 1);
        }

        ~Channel()
        {

        }

        void Add(void* _pHandler, RuntimeReflection::__duckvil_resource_type_t _typeHandle)
        {
            reflected_event _event = {};

            _event.m_functionHandle = RuntimeReflection::get_function_handle<const Message&>(m_pReflectionData, _typeHandle, "OnEvent");
            _event.m_typeHandle = _typeHandle;
            _event.m_pObject = _pHandler;

            if(m_aRefelctedEvents.Full())
            {
                m_aRefelctedEvents.Resize(m_aRefelctedEvents.Size() * 2);
            }

            m_aRefelctedEvents.Allocate(_event);
        }

        template <typename Handler>
        void Add(Handler* _pHandler)
        {
            RuntimeReflection::__duckvil_resource_type_t _typeHandle = RuntimeReflection::get_type<Handler>(m_pReflectionData);

            Add(_pHandler, _typeHandle);
        }

        void Add(const Callback& _fn)
        {
            m_aCallbackEvents.push_back(_fn);
        }

        template <typename Handler>
        void Add(Handler* _pHandler, void (Handler::*_fn)(const Message&))
        {
            m_aMemberEvents.push_back(new member_event<Handler>(_pHandler, _fn));
        }

        template <typename Handler>
        void Remove(Handler* _pHandler)
        {
            for(uint32_t i = 0; i < m_aRefelctedEvents.Size(); i++)
            {
                reflected_event& _event = m_aRefelctedEvents[i];

                if(_event.m_pObject == _pHandler)
                {
                    m_aRefelctedEvents.Erase(i);
                }
            }
        }

        void Remove(const Callback& _fn)
        {
            for(uint32_t i = 0; i < m_aCallbackEvents.size(); i++)
            {
                void (*_fnI)(const Message&) = m_aCallbackEvents[i];

                if(_fnI == _fn)
                {
                    m_aCallbackEvents.erase(m_aCallbackEvents.begin() + i);
                }
            }
        }

        void Broadcast(const Message& _message)
        {
            for(reflected_event& _event : m_aRefelctedEvents)
            {
                RuntimeReflection::invoke_member<const Message&>(m_pReflectionData, _event.m_typeHandle, _event.m_functionHandle, _event.m_pObject, _message);
            }

            for(Callback _fn : m_aCallbackEvents)
            {
                _fn(_message);
            }

            for(imember_event* _event : m_aMemberEvents)
            {
                _event->Invoke(_message);
            }
        }

        Channel& operator+=(const Callback& _fn)
        {
            Add(_fn);

            return *this;
        }

        template <typename Handler>
        Channel& operator+=(Handler* _pHandler)
        {
            Add(_pHandler);

            return *this;
        }

        Channel& operator-=(const Callback& _fn)
        {
            Remove(_fn);

            return *this;
        }

        template <typename Handler>
        Channel& operator-=(Handler* _pHandler)
        {
            Remove(_pHandler);

            return *this;
        }

        void operator()(const Message& _message)
        {
            Broadcast(_message);
        }
    };

}}