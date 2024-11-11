#include "Room.h"

Room::Room()
{
}

Room::~Room()
{
}

bool Room::JoinRoom() {
    if (current_players < max_players) {
        current_players++;
        return true;
    }
    return false;
}

void Room::LeaveRoom() {
    if (current_players > 0) {
        current_players--;
    }
}