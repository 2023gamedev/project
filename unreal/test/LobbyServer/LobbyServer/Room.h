#pragma once

#include "Common.h"
#include <string>
#include <vector>
#include <mutex>
#include <unordered_map>

#define MAX_PLAYERS 4

struct Join_Player {
    int id;
    string name;
    int character_type;
};

class Room {
private:
    std::unordered_map<int, Join_Player> Join_Players;

    int room_id;
    int current_players;

public:
    Room(int room_id);
    ~Room();

    bool JoinRoom();
    void LeaveRoom();

    int GetRoomID();
    int GetCurrentPlayers();
};
