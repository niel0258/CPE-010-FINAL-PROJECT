#ifndef HOSPITAL_HEADER_H
#define HOSPITAL_HEADER_H

#include "graph.h"
#include <iostream>
#include <map>
#include <string>

class RoomInfo{
    public:
        std::string roomName;
        int floorNum;
        bool restrictedFlag;
        RoomInfo(std::string Room_Name,int Floor_Number,bool isRestrict){
            roomName =Room_Name;
            floorNum = Floor_Number;
            restrictedFlag = isRestrict;
        };
};

class Hospital{
    private:
        Graph g;

        enum Location {
            // Ground floor
            MAIN_ENTRANCE = 0, LOBBY, REGISTRATION, CASHIER, PHARMACY,
            ELEVATOR_G, EMERGENCY, LABORATORY, XRAY, STAIRS_G,
            // 2nd floor
            OUTPATIENT, PEDIATRICS, OBGYN, WARD_A, WARD_B,
            ELEVATOR_2F, NURSE_2F, HALLWAY_2F, CAFETERIA, STAIRS_2F,
            // 3rd floor
            WARD_C, PRIVATE_ROOMS, ICU, OPERATING_ROOM, RECOVERY_ROOM,
            ELEVATOR_3F, NURSE_3F, HALLWAY_3F, LOUNGE, STAIRS_3F,
            LOCATION_COUNT // = 30
        };

        const std::map<int, RoomInfo> hash_map = {
            // Ground floor (Floor 1)
            {MAIN_ENTRANCE, RoomInfo("Main entrance", 1, false)},
            {LOBBY, RoomInfo("Lobby", 1, false)},
            {REGISTRATION, RoomInfo("Registration", 1, false)},
            {CASHIER, RoomInfo("Cashier", 1, false)},
            {PHARMACY, RoomInfo("Pharmacy", 1, false)},
            {ELEVATOR_G, RoomInfo("Elevator G", 1, false)},
            {EMERGENCY, RoomInfo("Emergency", 1, false)},
            {LABORATORY, RoomInfo("Laboratory", 1, false)},
            {XRAY, RoomInfo("X-ray", 1, false)},
            {STAIRS_G, RoomInfo("Stairs G", 1, false)},
            
            // 2nd floor (Floor 2)
            {OUTPATIENT, RoomInfo("Outpatient", 2, false)},
            {PEDIATRICS, RoomInfo("Pediatrics", 2, false)},
            {OBGYN, RoomInfo("OB-GYN", 2, false)},
            {WARD_A, RoomInfo("Ward A", 2, false)},
            {WARD_B, RoomInfo("Ward B", 2, false)},
            {ELEVATOR_2F, RoomInfo("Elevator 2F", 2, false)},
            {NURSE_2F, RoomInfo("Nurse station 2F", 2, true)},
            {HALLWAY_2F, RoomInfo("Hallway 2F", 2, false)},
            {CAFETERIA, RoomInfo("Cafeteria", 2, false)},
            {STAIRS_2F, RoomInfo("Stairs 2F", 2, false)},
            
            // 3rd floor (Floor 3)
            {WARD_C, RoomInfo("Ward C", 3, false)},
            {PRIVATE_ROOMS, RoomInfo("Private rooms", 3, false)},
            {ICU, RoomInfo("ICU", 3, true)},
            {OPERATING_ROOM, RoomInfo("Operating room", 3, true)},
            {RECOVERY_ROOM, RoomInfo("Recovery room", 3, true)},
            {ELEVATOR_3F, RoomInfo("Elevator 3F", 3, false)},
            {NURSE_3F, RoomInfo("Nurse station 3F", 3, true)},
            {HALLWAY_3F, RoomInfo("Hallway 3F", 3, false)},
            {LOUNGE, RoomInfo("Lounge", 3, false)},
            {STAIRS_3F, RoomInfo("Stairs 3F", 3, false)}
        };
 
        const std::string locationName[LOCATION_COUNT] = {
            "Main entrance", 
            "Lobby", 
            "Registration", 
            "Cashier", 
            "Pharmacy",
            "Elevator G", 
            "Emergency", 
            "Laboratory", 
            "X-ray", 
            "Stairs G",
            "Outpatient", 
            "Pediatrics", 
            "OB-GYN", 
            "Ward A", 
            "Ward B",
            "Elevator 2F", 
            "Nurse station 2F", 
            "Hallway 2F", 
            "Cafeteria", 
            "Stairs 2F",
            "Ward C", 
            "Private rooms", 
            "ICU", 
            "Operating room", 
            "Recovery room",
            "Elevator 3F", 
            "Nurse station 3F", 
            "Hallway 3F", 
            "Lounge", 
            "Stairs 3F"
        };
   
        public:
        Hospital();
        // Restricted areas: never a destination, never part of a route
        bool isRestricted(int id);
        const Graph& getGraph() const;
};

inline Hospital::Hospital(){
// ---- Ground floor ----
// top corridor
    g.add_edge(MAIN_ENTRANCE, LOBBY, 10);
    g.add_edge(LOBBY, REGISTRATION, 12);
    g.add_edge(REGISTRATION, CASHIER, 8);
    g.add_edge(CASHIER, PHARMACY, 10);
    // bottom corridor
    g.add_edge(ELEVATOR_G, EMERGENCY, 15);
    g.add_edge(EMERGENCY, LABORATORY, 20);
    g.add_edge(LABORATORY, XRAY, 12);
    g.add_edge(XRAY, STAIRS_G, 15);
    // links between the two corridors
    g.add_edge(MAIN_ENTRANCE, ELEVATOR_G, 15);
    g.add_edge(LOBBY, EMERGENCY, 20);
    g.add_edge(REGISTRATION, LABORATORY, 14);

    // ---- 2nd floor ----
    g.add_edge(OUTPATIENT, PEDIATRICS, 10);
    g.add_edge(PEDIATRICS, OBGYN, 10);
    g.add_edge(OBGYN, WARD_A, 12);
    g.add_edge(WARD_A, WARD_B, 8);
    g.add_edge(ELEVATOR_2F, NURSE_2F, 10);
    g.add_edge(NURSE_2F, HALLWAY_2F, 8);
    g.add_edge(HALLWAY_2F, CAFETERIA, 12);
    g.add_edge(CAFETERIA, STAIRS_2F, 10);
    g.add_edge(OUTPATIENT, ELEVATOR_2F, 12);
    g.add_edge(OBGYN, HALLWAY_2F, 14);
    g.add_edge(WARD_B, STAIRS_2F, 12);

    // ---- 3rd floor ----
    g.add_edge(WARD_C, PRIVATE_ROOMS, 8);
    g.add_edge(PRIVATE_ROOMS, ICU, 12);
    g.add_edge(ICU, OPERATING_ROOM, 10);
    g.add_edge(OPERATING_ROOM, RECOVERY_ROOM, 8);
    g.add_edge(ELEVATOR_3F, NURSE_3F, 10);
    g.add_edge(NURSE_3F, HALLWAY_3F, 8);
    g.add_edge(HALLWAY_3F, LOUNGE, 10);
    g.add_edge(LOUNGE, STAIRS_3F, 12);
    g.add_edge(WARD_C, ELEVATOR_3F, 12);
    g.add_edge(ICU, HALLWAY_3F, 15);
    g.add_edge(RECOVERY_ROOM, STAIRS_3F, 14);

    // ---- Between floors ----
    g.add_edge(ELEVATOR_G, ELEVATOR_2F, 8);
    g.add_edge(ELEVATOR_2F, ELEVATOR_3F, 8);
    g.add_edge(STAIRS_G, STAIRS_2F, 12);
    g.add_edge(STAIRS_2F, STAIRS_3F, 12);
}

inline bool Hospital::isRestricted(int id){
    return id == ICU || id == OPERATING_ROOM || id == RECOVERY_ROOM || id == LOUNGE;
}

const inline Graph& Hospital::getGraph() const{
    return g;
}


#endif