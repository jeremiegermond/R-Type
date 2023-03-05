#pragma once

#include "network.hpp"
#include "uiElements.hpp"

class Room {
    public:
        Room(int port): server(port) {

            _port = port;
            _thread = std::thread([this](){
                running = true;
                server.start();
                running = false;
            });
        };
        ~Room() {
            if (running) {
                _thread.join();
            }
        };
        bool isRunning() {
            return running;
        };

        int get_port() {
            return _port;
        }

        void displayLog() {
            server.update_log();
        };

        void displayOverlay() {
            interface *overlay = server.getOverlay();
            overlay->update();
            overlay->draw();
        };

        void displayPlayer() {
            for (auto &client : server.getClients()) {
                auto player = client.second;
                auto position = player.getPosition();
                DrawText("p", (position.x + 10) * 40, (-position.y + 5) * 40, 20, GREEN);
            }
        };

        void displayEnemies() {
            for (auto &enemy : server.getEnemies()) {
                auto position = enemy.getPosition();
                DrawText("x", (position.x + 10) * 40, (-position.y + 5) * 40, 20, RED);
            }
        };

        void displayBullets() {
            for (auto &bullet : server.getBullets()) {
                auto position = bullet.getPosition();
                DrawText("o", (position.x + 10) * 40, (-position.y + 5) * 40, 20, YELLOW);
            }
        };

        bool operator==(int port) {
            return _port == port;
        }

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