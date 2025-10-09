# so_long

A small 2D game built using the **MiniLibX** graphics library.  
Developed at **Hive Helsinki** as part of the 42 curriculum to learn event-driven programming, graphics rendering, and memory management in C.

---

## 🧩 Overview

`so_long` is a simple tile-based game where the player collects all collectibles on the map and reaches the exit.  
The game reads a `.ber` map file, validates it, and renders it using MiniLibX.

---

## 🎮 Gameplay

- **Goal:** Collect all items (`C`) and reach the exit (`E`)  
- **Map elements:**
  - `1` → Wall  
  - `0` → Empty space  
  - `C` → Collectible  
  - `E` → Exit  
  - `P` → Player start position  
- **Controls:**
  - `W` / `↑` → Move up  
  - `A` / `←` → Move left  
  - `S` / `↓` → Move down  
  - `D` / `→` → Move right  
  - `ESC` → Quit game

---

## 🧠 Learning Goals

- Parse and validate game maps from files  
- Manage 2D arrays and pathfinding logic  
- Use **MiniLibX** for basic rendering  
- Handle player movement through event hooks  
- Ensure no memory leaks and proper cleanup  

---

## ⚙️ Compilation

```bash
make
