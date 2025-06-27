# TraSim

This repository contains the source code of a traffic simulator developed in **C++** using the **Qt Framework**. The simulator represents a road environment where vehicles move dynamically, obeying traffic lights, yielding rules, and interacting with other vehicles in the scene.

## 📌 Features

- Automatic vehicle generation with variable intervals;
- Traffic light simulation;
- Yield logic at intersections and roundabouts;
- 2D graphical visualization with vehicle sprites;
- Real-time metrics (average travel time, active vehicles, etc);

## 🚀 How to Run

### Prerequisites

- Qt 5.15+ (Qt 6.x recommended if compatible)
- C++ compiler (MinGW, g++, clang)
- Qt Creator (optional, but recommended)

### Installation

Clone the repository:

```bash
git clone https://github.com/pedroguerreiro19/TraSim
```
Open the project in Qt Creator:

File > Open File or Project...

Select the *.pro file.

Configure the build kit (e.g., Desktop Qt 5.15 MinGW 64-bit).

Build and run (Ctrl+B, then Ctrl+R).

🖥 Screenshot
![image](https://github.com/user-attachments/assets/946fe481-6507-4ced-907d-f675d9c89f7a)


📈 Displayed Metrics
 - Number of active vehicles

 - Number of despawned vehicles

 - Average distance covered per vehicle

 - Average travel time per vehicle

 - Average velocity per vehicle

 - Vehicles currently stopped

 - Percentage of stopped vehicles

 - Average traffic flow

 - Max number of active vehicles

🛠 Technologies Used

C++

Qt Framework (QGraphicsScene, QTimer, QPixmap, etc.)

Blender (to create the map and the vehicles)
 
