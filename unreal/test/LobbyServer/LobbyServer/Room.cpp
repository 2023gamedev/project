#include "Room.h"

Room::Room(int id)
{
    current_players = 0;
    room_id = id;
}

Room::~Room()
{
}

bool Room::JoinRoom() {
    if (current_players < MAX_PLAYERS) {
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

int Room::GetRoomID()
{
    return room_id;
}

int Room::GetCurrentPlayers()
{
    return current_players;
}
