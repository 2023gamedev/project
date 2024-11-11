#pragma once

#include "Common.h"
#include <string>
#include <vector>
#include <mutex>

class Room {
private:
    int room_id;
    std::string room_name;
    int max_players;
    int current_players;

public:
    Room();
    ~Room();

    bool JoinRoom();
    void LeaveRoom();

    int GetRoomID();
    int GetMaxPlayers();
    int GetCurrentPlayers();
};
