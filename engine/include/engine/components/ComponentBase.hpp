/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** ComponentBase.hpp by thibb1
*/

#pragma once

#include "engine/graphics/Raylib.hpp"
#include "engine/messages/MessageManager.hpp"

namespace Engine {
class ComponentBase {
  public:
    /**
     * @brief Base class for all components
     */
    ComponentBase();

    virtual ~ComponentBase();

    /**
     * @brief Initialize the component
     */
    void Init();

    /**
     * @brief Update the component
     */
    void Update();

    /**
     * @brief Destroy the component
     */
    void Destroy();

    /**
     * @brief Set the component current state
     * @param active
     */
    void SetActive(bool active);

    /**
     * @brief Get the component current state
     * @return bool
     */
    [[nodiscard]] bool IsActive() const;

    /**
     * @brief Send a message to the MessageManager
     * @tparam T
     * @param message
     * @details This function is used to send a message to the MessageManager.
     *         It will only send the message if the component is active.
     */
    template <typename T>
    void SendMessage(const T &message) {
        if (IsActive())
            _messageManager.SendMessage(message);
    }

  protected:
    bool _active;
    bool _initialized;
    MessageManager _messageManager;

    virtual void DoInit() {}
    virtual void DoUpdate() {}
    virtual void DoDestroy() {}

    /**
     * @brief Register all message handlers for the component
     */
    virtual void RegisterMessageHandlers() {}

    /**
     * @brief Register a message handler for the component
     * @tparam C
     * @tparam T
     * @param handler
     * @details This function is used to register a message handler for the component.
     *        It is used to register a message handler for a specific message type.
     */
    template <typename C, typename T>
    void RegisterMessageHandler(void (C::*handler)(T &)) {
        _messageManager.RegisterMessageHandler<C, T>(handler);
    }
    // template<typename C, typename T>
    // void RegisterMessageHandler(void (C::*handler)(const T&)) {
    //     _messageManager.RegisterMessageHandler<C, T>(handler);
    // }
};
} // namespace Engine
