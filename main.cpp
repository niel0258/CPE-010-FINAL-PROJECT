#include "hospital_header.h"
#include "interface.h"

void buildRooms(Hospital& h);
void buildCorridors(Hospital& h);
void createHospital(Hospital &h);

int main() {
    Hospital hospital;       // the map, room information and restricted flags
    runInterface(hospital);
    return 0;
}

void buildRooms(Hospital& h) {
    // ==========================================
    // 1. GROUND FLOOR
    // ==========================================
    h.addRoom(MAIN_ENTRANCE, "Main entrance", 1, false);
    h.addRoom(LOBBY, "Lobby", 1, false);
    h.addRoom(REGISTRATION, "Registration", 1, false);
    h.addRoom(CASHIER, "Cashier", 1, false);
    h.addRoom(PHARMACY, "Pharmacy", 1, false);
    h.addRoom(ELEVATOR_G, "Elevator G", 1, false);
    h.addRoom(EMERGENCY, "Emergency", 1, false);
    h.addRoom(LABORATORY, "Laboratory", 1, false);
    h.addRoom(XRAY, "X-ray", 1, false);
    h.addRoom(STAIRS_G, "Stairs G", 1, false);

    // ==========================================
    // 2. SECOND FLOOR
    // ==========================================
    h.addRoom(OUTPATIENT, "Outpatient", 2, false);
    h.addRoom(PEDIATRICS, "Pediatrics", 2, false);
    h.addRoom(OBGYN, "OB-GYN", 2, false);
    h.addRoom(WARD_A, "Ward A", 2, false);
    h.addRoom(WARD_B, "Ward B", 2, false);
    h.addRoom(ELEVATOR_2F, "Elevator 2F", 2, false);
    h.addRoom(NURSE_2F, "Nurse station 2F", 2, true); // restricted
    h.addRoom(HALLWAY_2F, "Hallway 2F", 2, false);
    h.addRoom(CAFETERIA, "Cafeteria", 2, false);
    h.addRoom(STAIRS_2F, "Stairs 2F", 2, false);

    // ==========================================
    // 3. THIRD FLOOR
    // ==========================================
    h.addRoom(WARD_C, "Ward C", 3, false);
    h.addRoom(PRIVATE_ROOMS, "Private rooms", 3, false);
    h.addRoom(ICU, "ICU", 3, true);               // restricted
    h.addRoom(OPERATING_ROOM, "Operating room", 3, true); // restricted
    h.addRoom(RECOVERY_ROOM, "Recovery room", 3, true);   // restricted
    h.addRoom(ELEVATOR_3F, "Elevator 3F", 3, false);
    h.addRoom(NURSE_3F, "Nurse station 3F", 3, true);     // restricted
    h.addRoom(HALLWAY_3F, "Hallway 3F", 3, false);
    h.addRoom(LOUNGE, "Lounge", 3, false);
    h.addRoom(STAIRS_3F, "Stairs 3F", 3, false);
}

// Corridors, weighted by walking distance in meters.
void buildCorridors(Hospital& h) {
        // ---- Ground floor ----
        // top corridor
        h.addCorridor(MAIN_ENTRANCE, LOBBY, 10);
        h.addCorridor(LOBBY, REGISTRATION, 12);
        h.addCorridor(REGISTRATION, CASHIER, 8);
        h.addCorridor(CASHIER, PHARMACY, 10);
        // bottom corridor
        h.addCorridor(ELEVATOR_G, EMERGENCY, 15);
        h.addCorridor(EMERGENCY, LABORATORY, 20);
        h.addCorridor(LABORATORY, XRAY, 12);
        h.addCorridor(XRAY, STAIRS_G, 15);
        // links between the two corridors
        h.addCorridor(MAIN_ENTRANCE, ELEVATOR_G, 15);
        h.addCorridor(LOBBY, EMERGENCY, 20);
        h.addCorridor(REGISTRATION, LABORATORY, 14);

        // ---- 2nd floor ----
        h.addCorridor(OUTPATIENT, PEDIATRICS, 10);
        h.addCorridor(PEDIATRICS, OBGYN, 10);
        h.addCorridor(OBGYN, WARD_A, 12);
        h.addCorridor(WARD_A, WARD_B, 8);
        h.addCorridor(ELEVATOR_2F, NURSE_2F, 10);
        h.addCorridor(NURSE_2F, HALLWAY_2F, 8);
        h.addCorridor(HALLWAY_2F, CAFETERIA, 12);
        h.addCorridor(CAFETERIA, STAIRS_2F, 10);
        h.addCorridor(OUTPATIENT, ELEVATOR_2F, 12);
        h.addCorridor(OBGYN, HALLWAY_2F, 14);
        h.addCorridor(WARD_B, STAIRS_2F, 12);

        // ---- 3rd floor ----
        h.addCorridor(WARD_C, PRIVATE_ROOMS, 8);
        h.addCorridor(PRIVATE_ROOMS, ICU, 12);
        h.addCorridor(ICU, OPERATING_ROOM, 10);
        h.addCorridor(OPERATING_ROOM, RECOVERY_ROOM, 8);
        h.addCorridor(ELEVATOR_3F, NURSE_3F, 10);
        h.addCorridor(NURSE_3F, HALLWAY_3F, 8);
        h.addCorridor(HALLWAY_3F, LOUNGE, 10);
        h.addCorridor(LOUNGE, STAIRS_3F, 12);
        h.addCorridor(WARD_C, ELEVATOR_3F, 12);
        h.addCorridor(ICU, HALLWAY_3F, 15);
        h.addCorridor(RECOVERY_ROOM, STAIRS_3F, 14);

        // ---- Between floors ----
        h.addCorridor(ELEVATOR_G, ELEVATOR_2F, 8);
        h.addCorridor(ELEVATOR_2F, ELEVATOR_3F, 8);
        h.addCorridor(STAIRS_G, STAIRS_2F, 12);
        h.addCorridor(STAIRS_2F, STAIRS_3F, 12);
}

void createHospital(Hospital& h){
    buildRooms(h);
    buildCorridors(h);
}
