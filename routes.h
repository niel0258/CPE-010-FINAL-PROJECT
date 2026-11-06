#ifndef ROUTES_H
#define ROUTES_H

#include <algorithm>
#include <functional>
#include <iostream>
#include <map>
#include <mutex>
#include <queue>
#include <string>
#include <utility>
#include <vector>
#include "hospital_header.h"   // Hospital, RoomInfo (and graph.h)

// ============================================================
// Sorting and searching helpers
// ============================================================

// Sorts a[low..high] in place (inclusive bounds). The last element is the pivot.
// `cmp(x, y)` returns true when x should come before y.
template <typename T, typename Compare = std::less<T>>
void QuickSort(std::vector<T>& a, int low, int high) {
    if (low >= high) return;

    T pivot = a[high];
    int boundary = low - 1;  // last index of the "smaller than pivot" section

    for (int j = low; j < high; ++j) {
        if (a[j] < pivot) {
            std::swap(a[++boundary], a[j]);
        }
    }
    int pivotIndex = boundary + 1;
    std::swap(a[pivotIndex], a[high]);  // put the pivot in its final place

    QuickSort(a, low, pivotIndex - 1);
    QuickSort(a, pivotIndex + 1, high);
}

// ============================================================
// Route type
// ============================================================

class Route {
    public:
        std::vector<int> stops;  // vertices from start to destination
        int distance = 0;        // total length in meters

        bool operator < (const Route& other) const{
            return distance < other.distance;
        }

};

// ============================================================
// Restricted rooms
// ============================================================
// A route may not pass through a room whose RoomInfo has restrictedFlag set.
// A vertex id with no RoomInfo at all is treated as restricted too.
inline bool vertexIsBlocked(const Hospital& hospital, int vertex) {
    return hospital.isRestricted(vertex);
}

// ============================================================
// DFS: find every simple route (no repeated vertex) from `current` to `target`
// ============================================================
// `path` and `distance` track the route built so far (start with an empty path
// and distance 0). Finished routes are added to `out`.
// The same `path` vector is shared by every recursive call: that is what lets
// the "already on the path" check prevent loops, and what makes backtracking work.
inline void DFSRoutes(const Hospital& hospital, int current, int target,
                      std::vector<int>& path, int distance,
                      std::vector<Route>& out) {
    path.push_back(current);

    if (current == target) {
        out.push_back(Route{path, distance});
    } else {
        const auto& adj = hospital.getGraph().getAdjList();
        auto it = adj.find(current);
        if (it != adj.end()) {
            for (const auto& [next, weight] : it->second) {
                bool alreadyOnPath = std::find(path.begin(), path.end(), next) != path.end();
                if (alreadyOnPath) continue;                      // would create a loop
                if (vertexIsBlocked(hospital, next)) continue;    // restricted room
                DFSRoutes(hospital, next, target, path, distance + weight, out);
            }
        }
    }

    path.pop_back();  // backtrack
}

// ============================================================
// Dijkstra: shortest route from `src` to `dst`
// ============================================================
// Returns an empty Route (no stops) when `dst` cannot be reached.
inline Route shortestRoute(const Hospital& hospital, int src, int dst) {
    const auto& adj = hospital.getGraph().getAdjList();

    std::map<int, int> dist;  // best known distance from src
    std::map<int, int> prev;  // previous vertex on the best route

    using QueueItem = std::pair<int, int>;  // (distance, vertex)
    std::priority_queue<QueueItem, std::vector<QueueItem>, std::greater<QueueItem>> pq;

    dist[src] = 0;
    pq.push({0, src});

    while (!pq.empty()) {
        auto [d, u] = pq.top();
        pq.pop();

        if (u == dst) break;        // closest remaining vertex is the destination
        if (d > dist[u]) continue;  // outdated queue entry

        auto it = adj.find(u);
        if (it == adj.end()) continue;

        for (const auto& [v, weight] : it->second) {
            if (vertexIsBlocked(hospital, v)) continue;   // restricted room

            int newDist = d + weight;
            if (!dist.count(v) || newDist < dist[v]) {
                dist[v] = newDist;
                prev[v] = u;
                pq.push({newDist, v});
            }
        }
    }

    Route route;
    if (!dist.count(dst)) return route;  // no route exists

    // Walk back from the destination to the start, then flip the order.
    for (int at = dst;; at = prev[at]) {
        route.stops.push_back(at);
        if (at == src) break;
    }
    std::reverse(route.stops.begin(), route.stops.end());
    route.distance = dist[dst];
    return route;
}

#endif