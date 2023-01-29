#pragma once
#include "include.hpp"
/**
 * class that wll be used to store the current session data
 * player pos
 * other players
 * client and other client's ids
 * etc
 */

class Player {
    private:
        std::array<float, 2> pos;
        unsigned int id;
        int hp;
    public:
        Player(unsigned int id) {
            this->id = id;
            pos.fill(0);
            hp = 100;
        }
        Player(int id, std::array<float, 2> pos, int hp) {
            this->id = id;
            this->pos = pos;
            this->hp = hp;
        }
        std::array<float, 2> get_pos() {
            return pos;
        }
        int get_hp() {
            return hp;
        }
        int get_id() {
            return id;
        }
        void set_pos(std::array<float, 2> pos) {
            this->pos = pos;
        }
        void set_hp(int hp) {
            this->hp = hp;
        }
};

class Session {
    private:
        Player *player = nullptr;
        std::vector<Player *> players;
    public:
        Session() {
        }
        Session(unsigned int id) {
            player = new Player(id, {0, 0}, 100);
        }
        Session(Player *player) {
            this->player = player;
        }
        unsigned int get_player_id() {
            return player->get_id();
        }
        Player *get_player() {
            return player;
        }
        Player *get_player(unsigned int id) {
            for (int i = 0; i < players.size(); i++) {
                if (players[i]->get_id() == id)
                    return players[i];
            }
            return nullptr;
        }
        std::vector<Player *> get_players() {
            return players;
        }
        void add_player(Player *player) {
            players.push_back(player);
        }
        void remove_player(int id) {
            for (int i = 0; i < players.size(); i++) {
                if (players[i]->get_id() == id) {
                    players.erase(players.begin() + i);
                    break;
                }
            }
        }
        void update_player_pos(int id, std::array<float, 2> pos) {
            for (int i = 0; i < players.size(); i++) {
                if (players[i]->get_id() == id) {
                    players[i]->set_pos(pos);
                    break;
                }
            }
        }
};