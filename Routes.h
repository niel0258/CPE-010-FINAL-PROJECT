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
#include "graph.h"

// ============================================================
// Sorting and searching helpers
// ============================================================

// Sorts a[low..high] in place (inclusive bounds). The last element is the pivot.
// `cmp(x, y)` returns true when x should come before y.
template <typename T, typename Compare = std::less<T>>
void QuickSort(std::vector<T>& a, int low, int high, Compare cmp = Compare()) {
    if (low >= high) return;

    T pivot = a[high];
    int boundary = low - 1;  // last index of the "smaller than pivot" section

    for (int j = low; j < high; ++j) {
        if (cmp(a[j], pivot)) {
            std::swap(a[++boundary], a[j]);
        }
    }
    int pivotIndex = boundary + 1;
    std::swap(a[pivotIndex], a[high]);  // put the pivot in its final place

    QuickSort(a, low, pivotIndex - 1, cmp);
    QuickSort(a, pivotIndex + 1, high, cmp);
}

// Returns the index of `target` in the sorted vector `a`, or -1 if not found.
template <typename T, typename Compare = std::less<T>>
int binarySearch(const std::vector<T>& a, const T& target, Compare cmp = Compare()) {
    int low = 0;
    int high = static_cast<int>(a.size()) - 1;

    while (low <= high) {
        int mid = low + (high - low) / 2;
        if (cmp(a[mid], target))      low = mid + 1;
        else if (cmp(target, a[mid])) high = mid - 1;
        else                          return mid;
    }
    return -1;
}

// ============================================================
// Route type
// ============================================================

struct Route {
    std::vector<int> stops;  // vertices from start to destination
    int distance = 0;        // total length in meters
};

// ============================================================
// Blocked vertices (restricted rooms)
// ============================================================
// A BlockedCheck answers "may a route NOT pass through this vertex?".
// An empty BlockedCheck means nothing is blocked.
using BlockedCheck = std::function<bool(int)>;

inline bool vertexIsBlocked(const BlockedCheck& blocked, int vertex) {
    return blocked && blocked(vertex);
}

// ============================================================
// DFS: find every simple route (no repeated vertex) from `current` to `target`
// ============================================================
// `path` and `distance` track the route built so far (start with an empty path
// and distance 0). Finished routes are added to `out`.
// The same `path` vector is shared by every recursive call: that is what lets
// the "already on the path" check prevent loops, and what makes backtracking work.
inline void DFSRoutes(const Graph& g, int current, int target,
                      std::vector<int>& path, int distance,
                      std::vector<Route>& out,
                      const BlockedCheck& blocked = BlockedCheck()) {
    path.push_back(current);

    if (current == target) {
        out.push_back(Route{path, distance});
    } else {
        const auto& adj = g.getAdjList();
        auto it = adj.find(current);
        if (it != adj.end()) {
            for (const auto& [next, weight] : it->second) {
                bool alreadyOnPath = std::find(path.begin(), path.end(), next) != path.end();
                if (alreadyOnPath) continue;                    // would create a loop
                if (vertexIsBlocked(blocked, next)) continue;   // restricted room
                DFSRoutes(g, next, target, path, distance + weight, out, blocked);
            }
        }
    }

    path.pop_back();  // backtrack
}

// ============================================================
// Dijkstra: shortest route from `src` to `dst`
// ============================================================
// Returns an empty Route (no stops) when `dst` cannot be reached.
inline Route shortestRoute(const Graph& g, int src, int dst,
                           const BlockedCheck& blocked = BlockedCheck()) {
    const auto& adj = g.getAdjList();

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
            if (vertexIsBlocked(blocked, v)) continue;   // restricted room

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

// ============================================================
// Printing
// ============================================================

// Lock so output from several threads doesn't get mixed together.
inline std::mutex printMutex;

// Example result: "0 -> 1 -> 2 (22 m)"
inline std::string routeToString(const Route& r) {
    std::string s;
    for (size_t i = 0; i < r.stops.size(); ++i) {
        if (i > 0) s += " -> ";
        s += std::to_string(r.stops[i]);
    }
    return s + " (" + std::to_string(r.distance) + " m)";
}

// Finds and prints all routes from `from` to `to`, shortest first,
// followed by the Dijkstra result. `sortedVertices` must be sorted.
// Safe to call from several threads at once.
inline void findRoutes(const Graph& g, const std::vector<int>& sortedVertices,int from, int to,const BlockedCheck& blocked = BlockedCheck()) {
    std::string out = std::to_string(from) + " to " + std::to_string(to) + ":\n";

    bool bothExist = binarySearch(sortedVertices, from) != -1 &&
                     binarySearch(sortedVertices, to) != -1;

    if (!bothExist) {
        out += "  place does not exist\n";
    } else {
        std::vector<Route> routes;
        std::vector<int> path;
        DFSRoutes(g, from, to, path, 0, routes, blocked);

        QuickSort(routes, 0, static_cast<int>(routes.size()) - 1,
                  [](const Route& a, const Route& b) { return a.distance < b.distance; });

        for (size_t i = 0; i < routes.size(); ++i) {
            out += "  Route " + std::to_string(i + 1) + ": " + routeToString(routes[i]) + "\n";
        }
        out += "  Best: " + routeToString(shortestRoute(g, from, to, blocked)) + "\n";
    }

    std::lock_guard<std::mutex> lock(printMutex);  // one thread prints at a time
    std::cout << out;
}

#endif