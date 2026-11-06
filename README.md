<div align="center">
  <!-- Repository Badges -->
  <img src="https://img.shields.io/badge/Course-CPE%20010-blue?style=flat-square" alt="Course Name">
  <img src="https://img.shields.io/badge/Section-CPE21S2-orange?style=flat-square" alt="Term">
   <img src="https://img.shields.io/badge/Group-6-success?style=flat-square" alt="Status">

  <br />
  <br />
  
# CPE-010-FINAL-PROJECT
Emulation of route finding.

## Overview
This project simulates a route-finding system. It prompts the user for a location, verifies its existence using a hash map, and then calculates routes based on user preference:
* **All Paths:** Traverses the graph using Depth-First Search (DFS) and sorts the paths using Quick Sort.
* **Optimal Path:** Calculates and displays the most efficient route using Dijkstra's Algorithm.

## Features
* **Building Lookup:** Stores campus buildings in a Hash Map for fast searches by name.
* **Campus Mapping:** Represents the campus layout as a weighted graph, where nodes are buildings (e.g., Study Hall, Anniversary Hall, PEC1) and edges are the walking distances between them.
* **Optimal Route:** Uses **Dijkstra's Algorithm** and a priority queue to calculate the single shortest path to the destination.
* **Alternative Routes:** Uses **Depth-First Search (DFS)** to find all possible routes, which are then sorted by total distance using the **Quick Sort** algorithm.
* *Restricted Areas:* Safely routes around restricted or blocked locations (e.g., "Building 7 ??") by checking the restricted flag before traversing.

## Project Files
* `main.cpp` - Main program execution.
* `graph.h` - Graph data structure and traversal logic.
* `hospital_header.h` - Hospital-specific definitions or data.
* `interface.h` - User interface and console outputs.
* `routes.h` - Pathfinding algorithms and route management.

## Team Composition
* Condino, Niel Vincent
* Esguerra, Paula
* Estorco, Clint Hoven
* Libo-on, Gian Carlo
* Lopez, Andrei Dion
* Mendoza, Nathaniel
