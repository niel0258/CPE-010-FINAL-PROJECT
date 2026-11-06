#ifndef INTERFACE_H
#define INTERFACE_H

#include "hospital_header.h"  
#include <cctype>
#include <iostream>
#include <string>
#include <vector>
#include <chrono>


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

struct Timer {
    std::chrono::steady_clock::time_point begin;
    void start() { begin = std::chrono::steady_clock::now(); }
    double elapsedMs() const {
        return std::chrono::duration<double, std::milli>(std::chrono::steady_clock::now() - begin).count();
    }
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
void printAlternativeRoutes(const std::string& start, const DepartmentInfo& dest,
                                   const std::vector<Route>& routes) {
    size_t shown = routes.size() < MAX_ROUTES_SHOWN ? routes.size() : MAX_ROUTES_SHOWN;
    std::cout << "\nHOSPITAL NAVIGATION RESULT - ALL AVAILABLE ROUTES\n";
    printLine();
    std::cout << "Starting Point : " << start << "\n"
              << "Destination    : " << dest.name << " (" << dest.floorLabel << ")\n"
              << "Routes found   : " << routes.size() << " (shortest first)\n";
    if (shown < routes.size())
        std::cout << "Showing the " << shown << " shortest routes.\n";
    for (size_t i = 0; i < shown; ++i) {
        printLine('-');
        std::cout << "ROUTE " << (i + 1) << (i == 0 ? "  (Shortest)" : "")
                  << " - " << routes[i].totalDistance << " meters\n";
        printRouteSteps(routes[i]);
    }
    printLine();
}

/* ================================== Search Done ================================== */
void printSearchComplete() { 
    std::cout << "\n  Search complete.\n"; 
}

/* ================================== Bye Message ================================== */
void printGoodbye() { 
    std::cout << "\nThank you for using our System. Goodbye!\n"; 
}

class HospitalBackend {
    const Hospital& h;

    Route buildRoute(const std::vector<int>& path) const {
        Route r;
        for (size_t i = 0; i < path.size(); ++i) {
            r.destination.push_back(h.nameOf(path[i]));
            r.floors.push_back(h.floorName(path[i]));
            if (i + 1 < path.size()) {
                int w = h.getGraph().weight(path[i], path[i + 1]);
                r.distances.push_back(w);
                r.totalDistance += w;
            }
        }
        return r;
    }
    bool touchesRestricted(const std::vector<int>& path) const {   // Checker
        for (int id : path) if (h.isRestricted(id)) return true;
        return false;
    }
public:
    explicit HospitalBackend(const Hospital& hospital) : h(hospital) {}

    int locationCount() const { return h.locationCount(); }

    void getLocation(int id, DepartmentInfo& out) const {
        out.id = id;
        out.name = h.nameOf(id);
        out.floorLabel = h.floorName(id);
        out.restricted = h.isRestricted(id);
    }

    bool findDepartment(const std::string& input, DepartmentInfo& out) {
        std::string key = toLower(trim(input));
        for (int id = 0; id < h.locationCount(); ++id) {
            if (toLower(h.nameOf(id)) == key) { getLocation(id, out); return true; }
        }
        return false;
    }

    std::string startingPointName() { return h.nameOf(h.entranceId()); }

    bool shortestRoute(int destId, Route& out) {
        std::vector<int> path; int total = 0;
        auto blocked = [this](int id) { return h.isRestricted(id); };
        if (!h.getGraph().shortestPath(h.entranceId(), destId, blocked, path, total)) return false;
        if (touchesRestricted(path)) return false;
        out = buildRoute(path);
        return true;
    }

    std::vector<Route> alternativeRoutes(int destId) {
        auto blocked = [this](int id) { return h.isRestricted(id); };
        std::vector<Route> routes;
        for (const auto& p : h.getGraph().allPaths(h.entranceId(), destId, blocked))
            if (!touchesRestricted(p)) routes.push_back(buildRoute(p));
        if (routes.size() > 1)                                   // Quick Sort from graph.h
            QuickSort(routes, 0, (int)routes.size() - 1,
                      [](const Route& a, const Route& b) { return a.totalDistance < b.totalDistance; });
        return routes;
    }
};

/* ================================== Typing a Destination ================================== */

/* ================================== Finding and Browsing a Destination ================================== */

/* ================================== Typing a Destination Menu  ================================== */
bool runFindLocation(HospitalBackend& backend) {
    DepartmentInfo dest;
    if (!askForDestination(backend, dest)) return false;
    return showDirections(backend, dest);
}

// Build the browse list: one row per location, with its floor.
std::vector<std::string> browseItems(HospitalBackend& backend) {
    std::vector<std::string> items;
    for (int id = 0; id < backend.locationCount(); ++id) {
        DepartmentInfo d;
        backend.getLocation(id, d);
        std::string floor = d.floorLabel;
        while (floor.size() < 12) floor += ' ';              // align the columns
        items.push_back(floor + " | " + d.name + (d.restricted ? "  [Restricted]" : ""));
    }
    return items;
}

/* ================================== Browsing a Location Menu ================================== */
bool runBrowseLocations(HospitalBackend& backend) {
    std::cout << "\n=== BROWSE LOCATIONS ===\n";
    int id = arrowMenu("Select a location:", browseItems(backend));
    if (id < 0) return false;                                // Esc = back

    DepartmentInfo dest;
    backend.getLocation(id, dest);
    if (dest.restricted) { printRestricted(dest); return false; }
    if (dest.name == backend.startingPointName()) {
        std::cout << "\n  You are already at the " << dest.name << ".\n";
        return false;
    }
    return showDirections(backend, dest);
}

/* ================================== Main Program Loop ================================== */
void runInterface(HospitalBackend& backend) {
    printFrontDisplay();
    waitForAnyKey();
    bool running = true;
    while (running && !inputClosed()) {
        int choice = arrowMenu("\n=== MAIN MENU ===",
                               {"Find Location", "Browse Locations", "How to use MediGuide", "Exit"});

        switch (choice) {
            case 0:                                          // Find Location
                while (runFindLocation(backend) && !inputClosed() &&
                       askYesNo("\nSearch for another location?")) { }
                std::cout << "\n  Returning to the main menu...\n";
                break;
            case 1:                                          // Browse Locations
                while (runBrowseLocations(backend) && !inputClosed() &&
                       askYesNo("\nBrowse another location?")) { }
                std::cout << "\n  Returning to the main menu...\n";
                break;
            case 2:                                          // How to use or Guide to the System
                printHelp();
                break;
            case 3:                                          // Exit
                running = false;
                break;
            default:                                         // Esc on the main menu: just show it again
                break;
        }
    }
    printGoodbye();
}

#endif
