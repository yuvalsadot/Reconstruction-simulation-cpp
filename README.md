# Reconstruction Simulation in C++ 🏙️

A full object-oriented simulation of settlement reconstruction plans in SPLand, designed with memory safety and modular architecture using C++.

## 📚 Project Overview

This project simulates the reconstruction of various settlements (cities, villages, metropolises) through plans that build new facilities (schools, parks, factories) over time.  
The simulation handles different building strategies (Balanced, Economy, Environment) and scores settlements according to their facilities.

Key highlights:
- Full application of the **Rule of 5** for memory management.
- Global **Backup and Restore** functionality.
- Command-driven architecture (build, select, simulate, print, backup, restore).

## 🚀 Technologies Used
- C++14
- Object-Oriented Programming
- Dynamic Memory Management
- Design Patterns (Backup/Restore)
- Valgrind-tested memory safety

## 🛠️ How to Build and Run

1. Navigate to the project directory.
2. Build using the Makefile:
   ```bash
   make
   ```
3. Run the simulation by specifying a configuration file:
   ```bash
   ./simulation config1.json
   ```

# 🧠 Key Features
- Settlement Types: Cities, Villages, Metropolises
- Facilities: Parks, Schools, Factories
- Strategies for construction: Balanced, Environment-first, Economy-first
- Full Copy/Move Constructors and Assignment Operators
- Global Backup/Restore of simulation state
- Command-line input parsing

# ✨ Skills Demonstrated
- Advanced Object-Oriented Programming
- Dynamic Memory Management in C++
- Command Pattern and Action Processing
- Serialization and State Management
- Software Architecture and Design Principles
