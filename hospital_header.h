#ifndef HOSPITAL_HEADER_H
#define HOSPITAL_HEADER_H

#include "graph.h"
#include <iostream>

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
    g.add_edge(MAIN_ENTRANCE, LOBBY,        10);
    g.add_edge(LOBBY,         REGISTRATION, 12);
    g.add_edge(REGISTRATION,  CASHIER,       8);
    g.add_edge(CASHIER,       PHARMACY,     10);
    // bottom corridor
    g.add_edge(ELEVATOR_G,    EMERGENCY,    15);
    g.add_edge(EMERGENCY,     LABORATORY,   20);
    g.add_edge(LABORATORY,    XRAY,         12);
    g.add_edge(XRAY,          STAIRS_G,     15);
    // links between the two corridors
    g.add_edge(MAIN_ENTRANCE, ELEVATOR_G,   15);
    g.add_edge(LOBBY,         EMERGENCY,    20);
    g.add_edge(REGISTRATION,  LABORATORY,   14);
 
    // ---- 2nd floor ----
    g.add_edge(OUTPATIENT,    PEDIATRICS,   10);
    g.add_edge(PEDIATRICS,    OBGYN,        10);
    g.add_edge(OBGYN,         WARD_A,       12);
    g.add_edge(WARD_A,        WARD_B,        8);
    g.add_edge(ELEVATOR_2F,   NURSE_2F,     10);
    g.add_edge(NURSE_2F,      HALLWAY_2F,    8);
    g.add_edge(HALLWAY_2F,    CAFETERIA,    12);
    g.add_edge(CAFETERIA,     STAIRS_2F,    10);
    g.add_edge(OUTPATIENT,    ELEVATOR_2F,  12);
    g.add_edge(OBGYN,         HALLWAY_2F,   14);
    g.add_edge(WARD_B,        STAIRS_2F,    12);
 
    // ---- 3rd floor ----
    g.add_edge(WARD_C,        PRIVATE_ROOMS, 8);
    g.add_edge(PRIVATE_ROOMS, ICU,          12);
    g.add_edge(ICU,           OPERATING_ROOM,10);
    g.add_edge(OPERATING_ROOM,RECOVERY_ROOM, 8);
    g.add_edge(ELEVATOR_3F,   NURSE_3F,     10);
    g.add_edge(NURSE_3F,      HALLWAY_3F,    8);
    g.add_edge(HALLWAY_3F,    LOUNGE,       10);
    g.add_edge(LOUNGE,        STAIRS_3F,    12);
    g.add_edge(WARD_C,        ELEVATOR_3F,  12);
    g.add_edge(ICU,           HALLWAY_3F,   15);
    g.add_edge(RECOVERY_ROOM, STAIRS_3F,    14);
 
    // ---- Between floors ----
    g.add_edge(ELEVATOR_G,    ELEVATOR_2F,   8);
    g.add_edge(ELEVATOR_2F,   ELEVATOR_3F,   8);
    g.add_edge(STAIRS_G,      STAIRS_2F,    12);
    g.add_edge(STAIRS_2F,     STAIRS_3F,    12);
}

inline bool Hospital::isRestricted(int id){
    return id == ICU || id == OPERATING_ROOM || id == RECOVERY_ROOM || id == LOUNGE;
}

const inline Graph& Hospital::getGraph() const{
    return g;
}
#endif