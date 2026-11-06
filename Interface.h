#ifndef INTERFACE_H
#define INTERFACE_H

#include "hospital_header.h"  
#include <cctype>
#include <iostream>
#include <string>
#include <vector>

struct DepartmentInfo {
    int         id = -1;          // location id used by the graph
    std::string name;
    std::string floorLabel;       // e.g. "2nd floor"
    bool        restricted = false;
};

struct Route {
    std::vector<std::string> destination;
    std::vector<std::string> floors;
    std::vector<int>         distances; // IN METERS
    int                      totalDistance = 0; // IN METERS
};

const size_t MAX_ROUTES_SHOWN = 10;

/*  =============================================== INPUT  =============================================== */

/*  =============================================== OUTPUT  =============================================== */

void printLine(char c = '=', int n = 56) { std::cout << std::string(n, c) << "\n"; }

void printFrontDisplay() {
    printLine();
    std::cout << "                      Welcome to Hospital\n"
              << "        This is a Directional Guidance System\n";
    printLine();
    std::cout << "  Find your path to any hospital location.\n"
    printLine();
}

inline void printHelp() {
    std::cout << "\n=== HOW TO USE THE SYSTEM===\n"
              << "  Use the Up/Down arrow keys to move, Enter to select, Esc to go back.\n"
              << "  Look a Location  : Type location NAME (example: Pharmacy).\n"
              << "  Browse Locations : Scroll through every location and pick one.\n"
              << "  Then choose the fastest / shortest route for that location or all routes and follow the steps.\n"
              << "  NOTE : Restricted areas cannot be selected and are never part of a route.\n";
}

void printDepartmentInfo(const DepartmentInfo& d) {
    std::cout << "\nLOCATION FOUND\n";
    printLine('-');
    std::cout << "  Name  : " << d.name << "\n";
    if (!d.code.empty()) std::cout << "  Code  : " << d.code << "\n";
    std::cout << "  Floor : " << (d.floorLabel.empty() ? "(not specified)" : d.floorLabel) << "\n";
    printLine('-');
}

void printNotFound(const std::string& input) {
    std::cout << "\n  [!] Location name is not found: \"" << input << "\"\n"
              << "      Please check the spelling and try again.\n";
}

void printRestricted(const DepartmentInfo& d) {
    std::cout << "\n  [X] RESTRICTED AREA\n"
              << "      " << d.name << " is restricted and cannot be selected as a destination.\n"
              << "      Please ask the front desk staff for assistance.\n";
}

void printNoRoute(const std::string& destName) {
    std::cout << "\n  [!] No available route to " << destName << " was found.\n"
              << "      Please ask the front desk staff for assistance.\n";
}

/*  =============================================== OUTPUT ROUTE STEPS  =============================================== */

void printRouteSteps(const Route& r) {
    if (r.destination.empty()) return;
    int step = 1;
    std::cout << "  " << step++ << ". Start at the " << r.destination[0] << ".\n";
    for (size_t i = 0; i + 1 < r.destination.size(); ++i) {
        std::cout << "  " << step++ << ". Walk " << r.legMeters[i] << " meters from "
                  << r.destination[i] << " to " << r.destination[i + 1];
        if (i + 1 < r.floors.size() && !r.floors[i].empty() && r.floors[i] != r.floors[i + 1])
            std::cout << " (" << r.floors[i + 1] << ")";
        std::cout << ".\n";
    }
    std::cout << "  " << step << ". You have arrived at " << r.destination.back() << ".\n";
}

/* ================================== SHORTEST ROUTE ================================== */
void printShortestRoute(const std::string& start, const DepartmentInfo& dest, const Route& r) {
    std::cout << "\nHOSPITAL NAVIGATION RESULT - SHORTEST ROUTE\n";
    printLine();
    std::cout << "Starting Point : " << start << "\n"
              << "Destination    : " << dest.name << " (" << dest.floorLabel << ")\n\n"
              << "Recommended Route:\n";
    printRouteSteps(r);
    std::cout << "\nTotal Distance : " << r.totalDistance << " meters\n";
    printLine();
}

/* ================================== ALT ROUTE ==================================*/

/* ================================== Search Done ================================== */
void printSearchComplete() { 
    std::cout << "\n  Search complete.\n"; 
}

/* ================================== Bye Message ================================== */
void printGoodbye() { 
    std::cout << "\nThank you for using our System. Goodbye!\n"; 
}
