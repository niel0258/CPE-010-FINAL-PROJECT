#ifndef INTERFACE_H
#define INTERFACE_H

#include "hospital_header.h"
#include <iostream>
#include <string>
#include <vector>
#include <cctype>

namespace interface {

    struct DepartmentInformation {
        int id = -1;
        std::string name;
        std::string floor;
        bool restricted = false;
    };

    struct RouteInformation {
        std::vector<std::string> destinations; // Location
        std::vector<std::string> floors; // Label of destination floor
        std::vector<int> distances; // In Meters
        int totalDistance = 0; // In Meters
    };

    const size_t MAX_INPUT_SHOWN = 15;

}
