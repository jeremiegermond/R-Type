#pragma once
#include "include.hpp"

class UdpRequest {
    private:
        std::string data;
        std::string response;
        unsigned int id;
        int state = 0; // -1: not processed,  0: pending, 1: response received, 2: timeout
    public:
        UdpRequest(std::string data, unsigned int id) : data(data), id(id) {}
        std::string get_data() { return data; }
        unsigned int get_id() { return id; }
        int get_state() { return state; }
        std::string get_response() { return response; }
        bool operator==(int num) {
            if (id == num) {
                return true;
            }
            return false;
        }
        void set_response(std::string data) {
            response = data;
            state = 1;
        }
        void set_state(int new_state) {
            state = new_state;
        }
};