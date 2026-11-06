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

    void addRoom(Location id, const std::string& name, int floor, bool restricted);
    void buildRooms();
    void buildCorridors();

public:
    Hospital() {
        buildRooms();
        buildCorridors();
    }
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

void inline Hospital::addRoom(Location id, const std::string& name, int floor, bool restricted) {
        rooms.emplace(id, RoomInfo(name, floor, restricted));
        idByName[toLower(name)] = id;
    }

void inline Hospital::buildRooms() {
        // ---- Ground floor ----
        addRoom(MAIN_ENTRANCE, "Main entrance", 1, false);
        addRoom(LOBBY, "Lobby", 1, false);
        addRoom(REGISTRATION, "Registration", 1, false);
        addRoom(CASHIER, "Cashier", 1, false);
        addRoom(PHARMACY, "Pharmacy", 1, false);
        addRoom(ELEVATOR_G, "Elevator G", 1, false);
        addRoom(EMERGENCY, "Emergency", 1, false);
        addRoom(LABORATORY, "Laboratory", 1, false);
        addRoom(XRAY, "X-ray", 1, false);
        addRoom(STAIRS_G, "Stairs G", 1, false);

        // ---- 2nd floor ----
        addRoom(OUTPATIENT, "Outpatient", 2, false);
        addRoom(PEDIATRICS, "Pediatrics", 2, false);
        addRoom(OBGYN, "OB-GYN", 2, false);
        addRoom(WARD_A, "Ward A", 2, false);
        addRoom(WARD_B, "Ward B", 2, false);
        addRoom(ELEVATOR_2F, "Elevator 2F", 2, false);
        addRoom(NURSE_2F, "Nurse station 2F", 2, true); // restricted
        addRoom(HALLWAY_2F, "Hallway 2F", 2, false);
        addRoom(CAFETERIA, "Cafeteria", 2, false);
        addRoom(STAIRS_2F, "Stairs 2F", 2, false);

        // ---- 3rd floor ----
        addRoom(WARD_C, "Ward C", 3, false);
        addRoom(PRIVATE_ROOMS, "Private rooms", 3, false);
        addRoom(ICU, "ICU", 3, true); // restricted
        addRoom(OPERATING_ROOM, "Operating room", 3, true); // restricted
        addRoom(RECOVERY_ROOM, "Recovery room", 3, true); // restricted
        addRoom(ELEVATOR_3F, "Elevator 3F", 3, false);
        addRoom(NURSE_3F, "Nurse station 3F", 3, true); // restricted
        addRoom(HALLWAY_3F, "Hallway 3F", 3, false);
        addRoom(LOUNGE, "Lounge", 3, false);
        addRoom(STAIRS_3F, "Stairs 3F", 3, false);
    }

    // Corridors, weighted by walking distance in meters.
void inline Hospital::buildCorridors() {
        // ---- Ground floor ----
        // top corridor
        graph.add_edge(MAIN_ENTRANCE, LOBBY, 10);
        graph.add_edge(LOBBY, REGISTRATION, 12);
        graph.add_edge(REGISTRATION, CASHIER, 8);
        graph.add_edge(CASHIER, PHARMACY, 10);
        // bottom corridor
        graph.add_edge(ELEVATOR_G, EMERGENCY, 15);
        graph.add_edge(EMERGENCY, LABORATORY, 20);
        graph.add_edge(LABORATORY, XRAY, 12);
        graph.add_edge(XRAY, STAIRS_G, 15);
        // links between the two corridors
        graph.add_edge(MAIN_ENTRANCE, ELEVATOR_G, 15);
        graph.add_edge(LOBBY, EMERGENCY, 20);
        graph.add_edge(REGISTRATION, LABORATORY, 14);

        // ---- 2nd floor ----
        graph.add_edge(OUTPATIENT, PEDIATRICS, 10);
        graph.add_edge(PEDIATRICS, OBGYN, 10);
        graph.add_edge(OBGYN, WARD_A, 12);
        graph.add_edge(WARD_A, WARD_B, 8);
        graph.add_edge(ELEVATOR_2F, NURSE_2F, 10);
        graph.add_edge(NURSE_2F, HALLWAY_2F, 8);
        graph.add_edge(HALLWAY_2F, CAFETERIA, 12);
        graph.add_edge(CAFETERIA, STAIRS_2F, 10);
        graph.add_edge(OUTPATIENT, ELEVATOR_2F, 12);
        graph.add_edge(OBGYN, HALLWAY_2F, 14);
        graph.add_edge(WARD_B, STAIRS_2F, 12);

        // ---- 3rd floor ----
        graph.add_edge(WARD_C, PRIVATE_ROOMS, 8);
        graph.add_edge(PRIVATE_ROOMS, ICU, 12);
        graph.add_edge(ICU, OPERATING_ROOM, 10);
        graph.add_edge(OPERATING_ROOM, RECOVERY_ROOM, 8);
        graph.add_edge(ELEVATOR_3F, NURSE_3F, 10);
        graph.add_edge(NURSE_3F, HALLWAY_3F, 8);
        graph.add_edge(HALLWAY_3F, LOUNGE, 10);
        graph.add_edge(LOUNGE, STAIRS_3F, 12);
        graph.add_edge(WARD_C, ELEVATOR_3F, 12);
        graph.add_edge(ICU, HALLWAY_3F, 15);
        graph.add_edge(RECOVERY_ROOM, STAIRS_3F, 14);

        // ---- Between floors ----
        graph.add_edge(ELEVATOR_G, ELEVATOR_2F, 8);
        graph.add_edge(ELEVATOR_2F, ELEVATOR_3F, 8);
        graph.add_edge(STAIRS_G, STAIRS_2F, 12);
        graph.add_edge(STAIRS_2F, STAIRS_3F, 12);
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

std::string Hospital::floorName(int id) const {
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