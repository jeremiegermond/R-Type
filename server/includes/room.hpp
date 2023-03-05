#pragma once

#include "network.hpp"
#include "uiElements.hpp"
/**
 * @brief Class used to create rooms in the server
 * 
 */
class Room {
    public:
    /**
     * @brief Construct a new Room object
     * 
     * @param port 
     */
        Room(int port): server(port) {
            _port = port;
            _thread = std::thread([this](){
                running = true;
                server.start();
            });
            _thread.detach();
        };
        /**
         * @brief Destroy the Room object
         * 
         */
        ~Room() {
            if (running) {
                server.stop();
                running = false;
            }
        };
        /**
         * @brief check if a room is running or not
         * 
         * @return true 
         * @return false 
         */
        bool isRunning() {
            if (!server.is_running())
                running = false;
            return running;
        };
        /**
         * @brief stop the room
         * 
         */
        void stop() {
            if (running) {
                _thread.join();
                running = false;
            }
            delete this;
        };
        /**
         * @brief Get the port object
         * 
         * @return int 
         */
        int get_port() {
            return _port;
        }
        /**
         * @brief display logs
         * 
         */
        void displayLog() {
            server.update_log();
        };
        /**
         * @brief display the overlay
         * 
         */
        void displayOverlay() {
            interface *overlay = server.getOverlay();
            overlay->update();
            overlay->draw();
        };
        /**
         * @brief display the players
         * 
         */
        void displayPlayer() {
            for (auto &client : server.getClients()) {
                auto player = client.second;
                auto position = player.getPosition();
                DrawText("p", (position.x + 10) * 40, (-position.y + 5) * 40, 20, GREEN);
            }
        };
        /**
         * @brief display enemies
         * 
         */
        void displayEnemies() {
            for (auto &enemy : server.getEnemies()) {
                auto position = enemy.getPosition();
                DrawText("x", (position.x + 10) * 40, (-position.y + 5) * 40, 20, RED);
            }
        };
        /**
         * @brief display the bullet according to the server
         * 
         */
        void displayBullets() {
            for (auto &bullet : server.getBullets()) {
                auto position = bullet.getPosition();
                DrawText("o", (position.x + 10) * 40, (-position.y + 5) * 40, 20, YELLOW);
            }
        };
        /**
         * @brief override == operator
         * 
         * @param port 
         * @return true 
         * @return false 
         */
        bool operator==(int port) {
            return _port == port;
        }
        /**
         * @brief Get the player count object
         * 
         * @return int 
         */
        int get_player_count() {
            return server.getClients().size();
        }
    private:
        int _port;
        bool running;
        std::thread _thread;
        UdpServer server;
           // Font font;//to moove
};