# Rover Arm Hardware

Custom motor control PCB for the Space Concordia Mars rover robotic arm, designed in KiCad.

## Overview

The board drives five independent DC motors on the rover arm, receives commands over CAN from the main rover controller, and provides regulated power rails for the MCU and peripherals.

## Specifications

- **MCU:** STM32G030C8T6
- **CAN controller/transceiver:** MCP25625 (SPI interface)
- **Motor outputs:** 5 independent channels with PWM speed control and direction signals
- **Power input:** 16V DC
- **Regulation:** Buck converter  + LDO (MCU/logic supply)

## Contents

- `Schematics/` — KiCad schematic files
- `PCB/` — KiCad PCB layout files
- `Gerbers/` — Fabrication output files
- `BOM/` — Bill of materials

## Tools

Designed in KiCad. Open the project file in KiCad to view schematics and layout.
