<div align="center">

<!-- Course badges -->
<img src="https://img.shields.io/badge/Course-CPE%20010-0A66C2?style=flat-square" alt="Course">
<img src="https://img.shields.io/badge/Section-CPE21S2-F57C00?style=flat-square" alt="Section">
<img src="https://img.shields.io/badge/Group-6-2E7D32?style=flat-square" alt="Group">

<br /><br />

# 🏥 MediGuide
### `> Find the way. Skip the queue.`
**Hospital guidance and department referral, built in C++.**

<br />

<!-- Tech stack row -->
<img src="https://img.shields.io/badge/C++17-00599C?style=for-the-badge&logo=cplusplus&logoColor=white" alt="C++17">
<img src="https://img.shields.io/badge/GIT-F05032?style=for-the-badge&logo=git&logoColor=white" alt="Git">
<img src="https://img.shields.io/badge/GITHUB-181717?style=for-the-badge&logo=github&logoColor=white" alt="GitHub">
<img src="https://img.shields.io/badge/VISUAL%20STUDIO%20CODE-007ACC?style=for-the-badge&logo=visualstudiocode&logoColor=white" alt="VS Code">

<!-- Algorithms row -->
<img src="https://img.shields.io/badge/HASH%20MAP-E91E63?style=for-the-badge" alt="Hash Map">
<img src="https://img.shields.io/badge/GRAPH-00BFA5?style=for-the-badge" alt="Graph">
<img src="https://img.shields.io/badge/DIJKSTRA-2979FF?style=for-the-badge" alt="Dijkstra">
<img src="https://img.shields.io/badge/DFS-FF6D00?style=for-the-badge" alt="DFS">
<img src="https://img.shields.io/badge/QUICK%20SORT-AA00FF?style=for-the-badge" alt="Quick Sort">

<br /><br />

<!-- Quick facts -->
<img src="https://img.shields.io/badge/Departments-12-brightgreen?style=flat-square" alt="Departments">
<img src="https://img.shields.io/badge/Interface-Text%20UI-lightgrey?style=flat-square" alt="Interface">
<img src="https://img.shields.io/badge/Data-In--memory-blue?style=flat-square" alt="Data">

</div>

<br />

<!-- Add a screenshot or GIF of the program running here, e.g.: -->
<!-- <p align="center"><img src="docs/demo.gif" alt="MediGuide demo" width="700"></p> -->

## 📖 Overview
At a hospital front desk, every visitor asks the staff for directions. This causes long queues, only one route is given, and different staff give different answers.

**MediGuide** emulates the *Patient Guidance and Department Referral Process*. The program prompts the user for a department, verifies that it exists using a hash map, and then calculates routes from the front desk based on the user's preference:
* 🧭 **All Paths:** Traverses the graph using Depth-First Search (DFS) and sorts the paths using Quick Sort.
* ⚡ **Optimal Path:** Calculates and displays the most efficient route using Dijkstra's Algorithm.

---

## ✨ Features

| | Feature | How |
| :---: | :--- | :--- |
| 🔎 | **Department Lookup** | Hash Map: fast search by code or name |
| 🗺️ | **Hospital Mapping** | Weighted graph of 12 departments; edges are corridor distances in meters |
| ⚡ | **Optimal Route** | Dijkstra's Algorithm with a priority queue: the single shortest path |
| 🧭 | **Alternative Routes** | DFS finds all possible routes, then Quick Sort orders them by distance |
| ⛔ | **Restricted Areas** | Restricted departments cannot be a destination, and routes never pass through them |
| 🛡️ | **Input Handling** | Unknown departments are reported ("Department code or name is not found") and the user is asked again |

---

## 🧠 Data Structures and Algorithms

| Topic | Used for | Time complexity |
| :--- | :--- | :--- |
| 🟣 Hash map | Department lookup by code or name | O(1) average, O(n) worst |
| 🟢 Weighted graph (adjacency list) | Hospital map | Add edge O(1) |
| 🔵 Dijkstra + priority queue | Shortest route | O((V + E) log V) |
| 🟠 Depth-first search (DFS) | Finding all possible routes | O(V + E) per traversal |
| 🟪 Quick Sort | Sorting routes by distance | O(n log n) average, O(n²) worst |

*V = number of departments, E = number of corridors, n = number of routes.*

---



## 📂 Project Files

| File | Description |
| :--- | :--- |
| `main.cpp` | Main program execution. |
| `graph.h` | Graph data structure and traversal logic. |
| `hospital_header.h` | Hospital-specific definitions or data. |
| `interface.h` | User interface and console outputs. |
| `routes.h` | Pathfinding algorithms and route management. |

---

## ⚠️ Scope and Limitations
* Predefined hospital layout of 12 departments (no adding departments or corridors without editing the code)
* Text user interface only (no GUI or 3D map)
* Data is kept in memory only; closing the program deletes all data
* Single user; no networking, databases, real patient data, billing or appointments

---

## 👥 Team Composition
**CPE21S2 / Group 6**

* **Condino, Niel Vincent**
* **Esguerra, Paula**
* **Estorco, Clint Hoven**
* **Libo-on, Gian Carlo**
* **Lopez, Andrei Dion**
* **Mendoza, Nathaniel**

---

<div align="center">

🎓 **CPE 010 Data Structures and Algorithms**
Technological Institute of the Philippines, Quezon City
Instructor: Engr. Jimlord M. Quejado

</div>
