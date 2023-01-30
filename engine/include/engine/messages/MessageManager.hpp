/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** MessageHandler.hpp by thibb1
*/

#pragma once

#include "engine/utils/HashId.hpp"

#include <string>
#include <unordered_map>

namespace Engine {
/**
 * @brief Base class for message handlers
 * @details This class is the base class for message handlers
 */
class MessageHandlerBase {
  public:
    template <typename C, typename T>
    void Execute(C *component, T &message);
    // template<typename C, typename T>
    // void Execute(C* component, const T& message);
};

/**
 * @brief Message handler
 * @tparam C
 * @tparam T
 * @details This class is used to store message handlers in a map
 */
template <typename C, typename T>
class MessageHandler : public MessageHandlerBase {
  private:
    void (C::*_handler)(T &);

  public:
    explicit MessageHandler(void (C::*handler)(T &)) : _handler(handler) {}
    void HandleMessage(C *component, T &message) { (component->*_handler)(message); }
};

///**
// * @brief Message handler
// * @tparam C
// * @tparam T
// * @details This class is used to store message handlers in a map
// */
// template<typename C, typename T>
// class ConstMessageHandler : public MessageHandlerBase {
// private:
//    void (C::*_handler)(const T&);
// public:
//    explicit ConstMessageHandler(void (C::*handler)(const T&)) : _handler(handler) {}
//    void HandleMessage(C* component, const T& message) {
//        (component->*_handler)(message);
//    }
//};

/**
 * @brief Execute a message handler if it exists
 * @tparam C
 * @tparam T
 * @param component
 * @param message
 * @details This function is used to execute a message handler if it exists
 */
template <typename C, typename T>
void MessageHandlerBase::Execute(C *component, T &message) {
    auto handler = static_cast<MessageHandler<C, T> *>(this);
    if (handler)
        handler->HandleMessage(component, message);
}
// template<typename C, typename T>
// void MessageHandlerBase::Execute(C* component, const T& message) {
//     auto handler = static_cast<MessageHandler<C, T>*>(this);
//     if (handler)
//         handler->HandleMessage(component, message);
// }

/**
 * @brief Message manager
 * @details This class is used to send messages to components
 */
class MessageManager {
  public:
    /**
     * @brief Get the instance of the message manager
     * @return MessageManager&
     * @details This function is used to get the instance of the message manager
     */
    static MessageManager &GetInstance() {
        static MessageManager instance;
        return instance;
    }
    template <typename T>
    void SendMessage(T &message) {
        auto type = HashId::Get<T>();
        auto handler = _messageHandlers.find(type);
        if (handler != _messageHandlers.end()) {
            handler->second->Execute(this, message);
        }
    }
    template <typename C, typename T>
    void RegisterMessageHandler(void (C::*handler)(T &)) {
        auto type = HashId::Get<T>();
        auto messageHandler = new MessageHandler<C, T>(handler);
        _messageHandlers[type] = messageHandler;
    }

  private:
    MessageManager() = default;
    std::unordered_map<size_t, MessageHandlerBase *> _messageHandlers;
};
} // namespace Engine
