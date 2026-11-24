# Tower Siege

## Overview

**Tower Siege** is a tower defense game developed in C for the MINIX environment as the final project of the Computer Laboratory (LCOM) course at the Faculty of Engineering of the University of Porto (FEUP).

The game challenges players to defend their territory against successive waves of enemies by strategically placing towers and directly controlling a hero. Players must manage their resources, defeat enemies, and survive all ten waves to achieve victory.

The project integrates five hardware devices — Timer, Keyboard, Mouse, Video Card, and RTC — through the device drivers developed throughout the semester.

## Features

- **Three difficulty levels:** Easy, Medium, and Hard, each featuring a different enemy path and gameplay settings.
- **Wave-based gameplay:** Ten successive waves with increasing numbers of enemies and shorter spawn intervals.
- **Tower defense system:** Two tower types, Archer and Mage, with different costs, damage values, and attack ranges.
- **Tower management:** Place, upgrade, and sell towers to adapt your strategy throughout the game.
- **Hero control:** Move the hero freely around the map and attack enemies directly.
- **Enemy system:** Different enemy types, including Goblins and Orcs, with distinct characteristics.
- **Resource management:** Earn gold by defeating enemies and use it to build and upgrade towers.
- **Lives and respawning:** Protect your lives while managing the hero's health and respawn time.
- **Scoring system:** Earn points by defeating enemies and receive bonus points for unspent gold at the end of the game.
- **High-score leaderboard:** Save the top ten scores under a chosen username, together with the date and time of the game.
- **Game interface:** Main menu, difficulty selection, instructions, pause menu, and game-over screens.

## Controls

### Hero

| Key | Action |
|---|---|
| W | Move up |
| A | Move left |
| S | Move down |
| D | Move right |
| Space | Attack enemies |

### Mouse

| Action | Function |
|---|---|
| Move mouse | Move the in-game cursor |
| Left click | Select a tower spot or interact with tower management options |

### Menus

| Key | Action |
|---|---|
| Up / W | Navigate up |
| Down / S | Navigate down |
| Enter | Confirm selection |
| ESC | Pause the game or return to the previous menu |

## Technologies and Hardware

The project was developed in **C**, targeting the MINIX environment, and integrates the following hardware devices:

| Device | Function |
|---|---|
| Timer (i8254) | Controls the game loop, frame rate, animations, and gameplay timing. |
| Keyboard (i8042) | Handles player movement, attacks, and menu navigation. |
| Mouse (PS/2) | Controls the in-game cursor and tower interactions. |
| Video Card (VBE) | Renders the game in graphics mode using XPM sprites and double buffering. |
| RTC | Retrieves real-world date and time information for match timestamps and high scores. |

The application is built using the static libraries developed throughout the LCOM laboratory assignments:

- `librtc.a`
- `libtimer.a`
- `libutils.a`
- `libkbd.a`
- `libmouse.a`
- `libvideo.a`

## Project Structure

```text
.
├── lab1/                  # RTC driver and library
├── lab2/                  # Timer and utility libraries
├── lab3/                  # Keyboard driver and library
├── lab4/                  # Mouse driver and library
├── lab5/                  # Video graphics library
│
├── proj/
│   ├── src/
│   │   ├── assets/        # Game sprites and graphical assets
│   │   ├── enemy/         # Enemy entities and wave management
│   │   ├── font/          # Font rendering and name entry
│   │   ├── game/          # Main gameplay logic
│   │   ├── hero/          # Hero movement and attack system
│   │   ├── hud/           # In-game heads-up display
│   │   ├── map/           # Game map and path management
│   │   ├── menu/          # Menus, difficulty, pause and high scores
│   │   ├── player/        # Player resources, lives and score
│   │   ├── sprite/        # Sprite rendering and animations
│   │   ├── tower/         # Tower and projectile systems
│   │   ├── main.c         # Application entry point and main loop
│   │   └── Makefile       # Build configuration
│   │
│   ├── docs/              # Generated Doxygen documentation
│   └── highscores.bin     # High-score data
│
└── README.md
```
