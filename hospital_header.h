#ifndef HOSPITAL_HEADER_H
#define HOSPITAL_HEADER_H

#include <algorithm>
#include <cctype>
#include <string>
#include <unordered_map>
#include "graph.h"

typedef enum {
    // Ground floor (floor 1)
    MAIN_ENTRANCE = 0, LOBBY, REGISTRATION, CASHIER, PHARMACY,
    ELEVATOR_G, EMERGENCY, LABORATORY, XRAY, STAIRS_G,
    // 2nd floor
    OUTPATIENT, PEDIATRICS, OBGYN, WARD_A, WARD_B,
    ELEVATOR_2F, NURSE_2F, HALLWAY_2F, CAFETERIA, STAIRS_2F,
    // 3rd floor
    WARD_C, PRIVATE_ROOMS, ICU, OPERATING_ROOM, RECOVERY_ROOM,
    ELEVATOR_3F, NURSE_3F, HALLWAY_3F, LOUNGE, STAIRS_3F,

    LOCATION_COUNT // = 30
} Location;

// Lower-case copy of a string. Used so "PHARMACY" and "pharmacy" find the same room.
inline std::string toLower(std::string text) {
    std::transform(text.begin(), text.end(), text.begin(),
                   [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
    return text;
}


class RoomInfo {
public:
    std::string roomName;
    int floorNum; // 1, 2 or 3
    bool restrictedFlag; // true = cannot be a destination or part of a route

    RoomInfo(const std::string& name, int floor, bool restricted)
        : roomName(name), floorNum(floor), restrictedFlag(restricted) {}
};

class Hospital {
private:
    Graph graph; // rooms = vertices, corridors = weighted edges
    std::unordered_map<int, RoomInfo> rooms; // room id -> room information
    std::unordered_map<std::string, int> idByName; // lower-case name -> room id

public:
    void addRoom(Location id, const std::string& name, int floor, bool restricted);
    void addCorridor(int start, int destination, int weight);
    int locationCount() const { return LOCATION_COUNT; }
    // Where every search starts.
    int entranceId() const { return MAIN_ENTRANCE; }
    const Graph& getGraph() const { return graph; }
    int findIdByName(const std::string& name) const;
    std::string nameOf(int id) const;
    int floorNumber(int id) const;
    std::string floorName(int id) const;
    bool isRestricted(int id) const;
    int distanceBetween(int from, int to) const;
};

inline void Hospital::addRoom(Location id, const std::string& name, int floor, bool restricted) {
    rooms.emplace(id, RoomInfo(name, floor, restricted));
    idByName[toLower(name)] = id;
}

inline void Hospital::addCorridor(int start, int destination, int distance){
    graph.add_edge(start, destination, distance);
}

inline int Hospital::findIdByName(const std::string& name) const {
    auto found = idByName.find(toLower(name));
    if (found == idByName.end()) return -1;
    return found->second;
}

inline std::string Hospital::nameOf(int id) const {
    auto found = rooms.find(id);
    if (found == rooms.end()) return "Unknown";
    return found->second.roomName;
}

inline int Hospital::floorNumber(int id) const {
    auto found = rooms.find(id);
    if (found == rooms.end()) return 0;
    return found->second.floorNum;
}

inline std::string Hospital::floorName(int id) const {
    switch (floorNumber(id)) {
        case 1: return "Ground floor";
        case 2: return "2nd floor";
        case 3: return "3rd floor";
        default: return "";
    }
}

inline bool Hospital::isRestricted(int id) const {
    auto found = rooms.find(id);
    if (found == rooms.end()) return true; // unknown ids are treated as off-limits
    return found->second.restrictedFlag;
}

inline int Hospital::distanceBetween(int from, int to) const {
    const auto& adj = graph.getAdjList();
    auto neighbors = adj.find(from);
    if (neighbors == adj.end()) return 0;

    for (const auto& [neighbor, meters] : neighbors->second) {
        if (neighbor == to) return meters;
    }
    return 0;
}

#endif