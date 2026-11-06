#ifndef INTERFACE_H
#define INTERFACE_H

#include <algorithm>
#include <cctype>
#include <cstdlib>
#include <chrono>
#include <cstdio>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <thread> 
#include <vector>

#include "hospital_header.h"   // Hospital: graph, hash maps, names, floors, restricted flags
#include "routes.h"            // Route, QuickSort, DFSRoutes, shortestRoute

#ifdef _WIN32
    #ifndef NOMINMAX
        #define NOMINMAX
    #endif
    #include <conio.h>
    #include <io.h>
    #include <windows.h>
#else
    #include <termios.h>
    #include <unistd.h>
#endif

// How this file connects to the other two:
//   - A location is just its id (the Location enum in hospital_header.h).
//     Name, floor and restricted flag come from the Hospital hash maps.
//   - A route is a `Route` from routes.h (ids + distance), turned into text only when printed.
//   - Routes are found with DFSRoutes / shortestRoute / QuickSort from routes.h.

/*  ==============================================  TIMER  =============================================== */

struct Timer {
    std::chrono::steady_clock::time_point begin;

    void start() { begin = std::chrono::steady_clock::now(); }

    double elapsedMs() const {
        return std::chrono::duration<double, std::milli>(
                   std::chrono::steady_clock::now() - begin).count();
    }
};

const size_t MAX_ROUTES_SHOWN = 50;

/*  =============================================== INPUT  =============================================== */

inline std::string trim(const std::string& text) {
    size_t first = 0;
    while (first < text.size() && std::isspace(static_cast<unsigned char>(text[first]))) {
        ++first;
    }
    size_t last = text.size();
    while (last > first && std::isspace(static_cast<unsigned char>(text[last - 1]))) {
        --last;
    }
    return text.substr(first, last - first);
}

// True once the keyboard input has ended (Ctrl+D on Linux/Mac, Ctrl+Z on Windows).
inline bool inputClosed() {
    return std::cin.eof() || std::cin.fail();
}

// Reads one line. Returns false when the input has ended.
inline bool readLine(std::string& line) {
    return static_cast<bool>(std::getline(std::cin, line));
}

// Arrow-key menus need a real terminal. When the input comes from a file or pipe,
// the menus fall back to numbered choices.
inline bool isInteractiveTerminal() {
#ifdef _WIN32
    return _isatty(_fileno(stdin)) != 0;
#else
    return isatty(STDIN_FILENO) != 0;
#endif
}

// Lets the menu redraw itself with ANSI escape codes (Windows 10+ needs this switched on).
inline void enableAnsiOutput() {
#ifdef _WIN32
    HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD mode = 0;
    if (GetConsoleMode(out, &mode)) {
        SetConsoleMode(out, mode | 0x0004);   // ENABLE_VIRTUAL_TERMINAL_PROCESSING
    }
#endif
}

// Clears screen 
inline void clearScreen() {
    if (!isInteractiveTerminal()) return;
    system("clear");
}

// Pauses for a set amount of time
inline void wait(double seconds) {
    if (!isInteractiveTerminal()) return;
    std::this_thread::sleep_for(std::chrono::duration<double>(seconds));
}

enum class Key { Up, Down, Enter, Escape, Other, Closed };

// Waits for ONE key press (no Enter needed) and tells which key it was.
inline Key readKey() {
#ifdef _WIN32
    int c = _getch();
    if (c == 13) return Key::Enter;
    if (c == 27) return Key::Escape;
    if (c == 0 || c == 224) {              // arrow keys send two codes
        int code = _getch();
        if (code == 72) return Key::Up;
        if (code == 80) return Key::Down;
    }
    return Key::Other;
#else
    // Switch the terminal to "raw" mode so a key press arrives immediately and is not echoed.
    termios original;
    tcgetattr(STDIN_FILENO, &original);
    termios raw = original;
    raw.c_lflag &= ~(ICANON | ECHO);
    raw.c_cc[VMIN]  = 1;
    raw.c_cc[VTIME] = 0;
    tcsetattr(STDIN_FILENO, TCSANOW, &raw);

    Key key = Key::Other;
    unsigned char c = 0;

    if (read(STDIN_FILENO, &c, 1) <= 0) {
        key = Key::Closed;
    } else if (c == '\n' || c == '\r') {
        key = Key::Enter;
    } else if (c == 27) {
        // An arrow key sends 3 bytes: ESC [ A (up) or ESC [ B (down).
        // A single ESC with nothing after it is the Escape key.
        raw.c_cc[VMIN]  = 0;
        raw.c_cc[VTIME] = 1;               // wait up to 0.1 s for the rest
        tcsetattr(STDIN_FILENO, TCSANOW, &raw);

        unsigned char rest[2] = {0, 0};
        bool gotSequence = read(STDIN_FILENO, &rest[0], 1) == 1 &&
                           read(STDIN_FILENO, &rest[1], 1) == 1;
        if (!gotSequence) {
            key = Key::Escape;
        } else if (rest[0] == '[' || rest[0] == 'O') {
            if (rest[1] == 'A') key = Key::Up;
            if (rest[1] == 'B') key = Key::Down;
        }
    }

    tcsetattr(STDIN_FILENO, TCSANOW, &original);   // always restore the terminal
    return key;
#endif
}

inline void waitForAnyKey() {
    std::cout << "\n  Press any key to continue...";
    std::cout.flush();
    if (isInteractiveTerminal()) {
        readKey();
    } else {
        std::string ignored;
        readLine(ignored);
    }
    std::cout << "\n";
}

inline bool askYesNo(const std::string& question) {
    while (true) {
        std::cout << question << " (y/n): ";
        std::string answer;
        if (!readLine(answer)) return false;

        answer = toLower(trim(answer));
        if (answer == "y" || answer == "yes") return true;
        if (answer == "n" || answer == "no")  return false;
        std::cout << "  Please type y or n.\n";
    }
}

// Draws the visible rows of the menu. Returns how many lines it printed,
// so the caller can move the cursor back up and draw again.
inline int drawMenu(const std::vector<std::string>& items, int selected, int top, int rows) {
    for (int i = top; i < top + rows; ++i) {
        std::cout << "\r\033[2K" << (i == selected ? "  > " : "    ") << items[i] << "\n";
    }
    std::cout << "\r\033[2K  [" << (selected + 1) << "/" << items.size()
              << "]  Up/Down = Move, Enter = select, Esc = back\n";
    return rows + 1;
}

// Shows the items and lets the user pick one with the Up/Down arrow keys.
// Returns the chosen index, or -1 when the user pressed Esc (or the input ended).
inline int arrowMenu(const std::string& title, const std::vector<std::string>& items) {
    std::cout << title << "\n";
    if (items.empty()) return -1;

    const int totalItems = static_cast<int>(items.size());
    const int visibleRows = std::min(10, totalItems);   // long lists scroll

    int selected = 0;   // highlighted item
    int top = 0;   // first item currently on screen
    int linesDrawn = 0;

    while (true) {
        if (linesDrawn > 0) {
            std::cout << "\033[" << linesDrawn << "A";   // move cursor up to redraw
        }
        linesDrawn = drawMenu(items, selected, top, visibleRows);
        std::cout.flush();

        Key key = readKey();
        if (key == Key::Enter)  return selected;
        if (key == Key::Escape) return -1;
        if (key == Key::Closed) {
            std::cin.setstate(std::ios::eofbit | std::ios::failbit);   // so inputClosed() is true
            return -1;
        }

        if (key == Key::Up) {
            selected = (selected - 1 + totalItems) % totalItems;
        }
        if (key == Key::Down){ 
            selected = (selected + 1) % totalItems;
        }

        // keep the highlighted item inside the visible window
        if (selected < top){
            top = selected;
        }
        if (selected >= top + visibleRows) { 
            top = selected - visibleRows + 1;
        }
    }
}

/*  =============================================== OUTPUT  =============================================== */

const std::string TITLE = "MEDICOMPASS";

inline void printLine(char c = '=', int n = 77) {
    std::cout << std::string(n, c) << "\n";
}

inline void printFrontDisplay() {
    printLine();
    std::cout << "\t\t\t   " << TITLE << "\n" << "\t\tHospital Directional Guidance System\n";
    printLine();
    std::cout << "\t\tFind your way to any hospital location.\n";
    printLine();
}

inline void printHelp() {
    std::cout << "\n======================= HOW TO USE THE SYSTEM ===========================\n\n"
              << "  Use the Up/Down arrow keys to move, Enter to select, Esc to go back.\n"
              << "  Find Location    : Type the location NAME (example: Pharmacy). Type 'back' to return.\n"
              << "  Browse Locations : Scroll through every location and pick one.\n"
              << "  Then choose the shortest route to that location, or all routes, and follow the steps.\n"
              << "  NOTE : Restricted areas cannot be selected and are never part of a route.\n";
}

inline void printDepartmentInfo(const Hospital& hospital, int id) {
    std::string floor = hospital.floorName(id);

    std::cout << "\nLOCATION FOUND\n";
    printLine('-');
    std::cout << "  Name  : " << hospital.nameOf(id) << "\n";
    std::cout << "  Floor : " << (floor.empty() ? "(not specified)" : floor) << "\n";
    printLine('-');
}

inline void printNotFound(const std::string& input) {
    std::cout << "\n  [!] Location name is not found: \"" << input << "\"\n"
              << "      Please check the spelling and try again.\n";
}

inline void printRestricted(const Hospital& hospital, int id) {
    std::cout << "\n  [X] RESTRICTED AREA\n"
              << "      " << hospital.nameOf(id) << " is restricted and cannot be selected as a destination.\n"
              << "      Please ask the front desk staff for assistance.\n";
}

inline void printAlreadyHere(const Hospital& hospital, int id) {
    std::cout << "\n  You are already at the " << hospital.nameOf(id) << ".\n";
}

inline void printNoRoute(const Hospital& hospital, int destId) {
    std::cout << "\n  [!] No available route to " << hospital.nameOf(destId) << " was found.\n"
              << "      Please ask the front desk staff for assistance.\n";
}

/*  =============================================== OUTPUT ROUTE STEPS  =============================================== */

// Turns a Route (ids) into numbered walking steps (names and meters).
inline void printRouteSteps(const Hospital& hospital, const Route& route) {
    if (route.stops.empty()) return;

    int step = 1;
    std::cout << "  " << step++ << ". Start at the " << hospital.nameOf(route.stops[0]) << ".\n";

    for (size_t i = 0; i + 1 < route.stops.size(); ++i) {
        int from = route.stops[i];
        int to   = route.stops[i + 1];

        std::cout << "  " << step++ << ". Walk " << hospital.distanceBetween(from, to)
                  << " meters from " << hospital.nameOf(from) << " to " << hospital.nameOf(to);

        bool changesFloor = hospital.floorNumber(from) != hospital.floorNumber(to);
        if (changesFloor) {
            std::cout << " (" << hospital.floorName(to) << ")";
        }
        std::cout << ".\n";
    }

    std::cout << "  " << step << ". You have arrived at " << hospital.nameOf(route.stops.back()) << ".\n";
}

/* ================================== SHORTEST ROUTE ================================== */
inline void printShortestRoute(const Hospital& hospital, int destId, const Route& route) {
    std::cout << "\nHOSPITAL NAVIGATION RESULT - SHORTEST ROUTE\n";
    printLine();
    std::cout << "Starting Point : " << hospital.nameOf(hospital.entranceId()) << "\n"
              << "Destination    : " << hospital.nameOf(destId)
              << " (" << hospital.floorName(destId) << ")\n\n"
              << "Recommended Route:\n";
    printRouteSteps(hospital, route);
    std::cout << "\nTotal Distance : " << route.distance << " meters\n";
    printLine();
}

/* ================================== ALL ROUTES ================================== */
inline void printAlternativeRoutes(const Hospital& hospital, int destId,
                                   const std::vector<Route>& routes) {
    size_t shown = std::min(routes.size(), MAX_ROUTES_SHOWN);

    std::cout << "\nHOSPITAL NAVIGATION RESULT - ALL AVAILABLE ROUTES\n";
    printLine();
    std::cout << "Starting Point : " << hospital.nameOf(hospital.entranceId()) << "\n"
              << "Destination    : " << hospital.nameOf(destId)
              << " (" << hospital.floorName(destId) << ")\n"
              << "Routes found   : " << routes.size() << " (shortest first)\n";
    if (shown < routes.size()) {
        std::cout << "Showing the " << shown << " shortest routes.\n";
    }

    for (size_t i = 0; i < shown; ++i) {
        printLine('-');
        std::cout << "ROUTE " << (i + 1) << (i == 0 ? "  (Shortest)" : "")
                  << " - " << routes[i].distance << " meters\n";
        printRouteSteps(hospital, routes[i]);
    }
    printLine();
}

/* ================================== Search Done ================================== */
inline void printSearchComplete(double searchMs) {
    std::ostringstream time;
    time << std::fixed << std::setprecision(3) << searchMs;
    std::cout << "\n  Search complete (" << time.str() << " ms).\n";
}

/* ================================== Bye Message ================================== */
inline void printGoodbye() {
    std::cout << "\nThank you for using our System. Goodbye!\n";
}

/*  =============================================== FINDING ROUTES  =============================================== */

// Choose the route type, search with the routes.h functions, and print the result.
// Returns true when directions were shown, false when the user went back or no route exists.
inline bool showDirections(const Hospital& hospital, int destId) {
    int choice = arrowMenu("\nWhich route do you want?",
                           {"Shortest route", "All possible routes"});
    if (choice < 0) return false;   // Esc = back
    
    clearScreen();

    // Restricted rooms are blocked, so no route can pass through them.
    auto isRestrictedRoom = [&hospital](int id) { return hospital.isRestricted(id); };
    int start = hospital.entranceId();

    Timer timer;
    timer.start();

    if (choice == 0) {
        Route best = shortestRoute(hospital, start, destId);
        double searchMs = timer.elapsedMs();

        if (best.stops.empty()) {
            printNoRoute(hospital, destId);
            return false;
        }
        printShortestRoute(hospital, destId, best);
        printSearchComplete(searchMs);
    } else {
        std::vector<Route> routes;
        std::vector<int> path;
        DFSRoutes(hospital, start, destId, path, 0, routes);
        QuickSort(routes, 0, static_cast<int>(routes.size()) - 1);
        double searchMs = timer.elapsedMs();

        if (routes.empty()) {
            printNoRoute(hospital, destId);
            return false;
        }
        printAlternativeRoutes(hospital, destId, routes);
        printSearchComplete(searchMs);
    }
    return true;
}

/* ================================== Typing a Destination ================================== */

inline int askForDestination(const Hospital& hospital) {
    std::cout << "\n============ FIND LOCATION ======================\n";

    while (true) {
        clearScreen();
        std::cout << "  Type a location name (example: Pharmacy), or 'back' to return: ";
        std::string input;
        if (!readLine(input)) return -1;

        input = trim(input);
        if (input.empty()) continue;
        if (toLower(input) == "back") return -1;

        int id = hospital.findIdByName(input);
        if (id < 0) {
            printNotFound(input);
            wait(2);
            continue;
        }

        printDepartmentInfo(hospital, id);

        if (hospital.isRestricted(id)) {
            printRestricted(hospital, id);
            wait(3);
            continue;
        }
        if (id == hospital.entranceId()) {
            printAlreadyHere(hospital, id);
            wait(2);
            continue;
        }
        return id;
    }
}

/* ================================== Find Location Menu ================================== */
inline bool runFindLocation(const Hospital& hospital) {
    clearScreen();
    int destId = askForDestination(hospital);
    if (destId < 0) return false;
    return showDirections(hospital, destId);
}

/* ================================== Browsing a Location Menu ================================== */

inline std::vector<std::string> browseItems(const Hospital& hospital) {
    std::vector<std::string> items;
    for (int id = 0; id < hospital.locationCount(); ++id) {
        std::string floor = hospital.floorName(id);
        items.push_back(floor + " | " + hospital.nameOf(id) +
                        (hospital.isRestricted(id) ? "  [Restricted]" : ""));
    }
    return items;
}

inline bool runBrowseLocations(const Hospital& hospital) {
    clearScreen();
    std::cout << "\n================== BROWSE LOCATIONS ======================\n";
    int id = arrowMenu("Select a location:", browseItems(hospital));
    if (id < 0) return false;

    if (hospital.isRestricted(id)) {
        printRestricted(hospital, id);
        wait(2);
        return false;
    }
    if (id == hospital.entranceId()) {
        printAlreadyHere(hospital, id);
        wait(2);
        return false;
    }
    return showDirections(hospital, id);
}

/* ================================== Main Program Loop ================================== */
inline void runInterface(const Hospital& hospital) {
    clearScreen();
    enableAnsiOutput();
    printFrontDisplay();
    waitForAnyKey();

    bool running = true;
    while (running && !inputClosed()) {
        clearScreen();
        int choice = arrowMenu("\n============================ " + TITLE + " =================================\n\n----- Choose an Option ----",
                               {"Find Location", "Browse Locations", "How to use the system", "Exit"});

        switch (choice) {
            case 0:
                while (runFindLocation(hospital) && !inputClosed() &&
                       askYesNo("\nSearch for another location?")) { }
                std::cout << "\n  Returning to the main menu...\n";
                wait(2);
                break;

            case 1:
                while (runBrowseLocations(hospital) && !inputClosed() &&
                       askYesNo("\nBrowse another location?")) { }
                std::cout << "\n  Returning to the main menu...\n";
                wait(2);
                break;

            case 2:
                clearScreen();
                printHelp();
                waitForAnyKey();
                break;

            case 3:
                running = false;
                clearScreen();
                printGoodbye();
                wait(2);
                break;

            default:
                break;
        }
    }
}

#endif
