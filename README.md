![Banner](https://github.com/project-gullwing/gullware/blob/master/files/banner.png)

# GULLware (Project Gullwing firmware)

This project aims to provide precise remote control of a Dobson-mounted telescope with use of two stepper motors.

## Hardware compatibility
- Compatible with ESP32-based microcontrollers

## Features
- Bluetooth Low Energy command & feedback interface
- Smooth velocity control of the steppers
- Dual-core operation to minimize undesirable lags in step pulses
- Generic low-level interface for the stepper drivers
    - Built-in support for Trinamic TMC2209 drivers
    - Built-in support for Pololu A4988 drivers (legacy)

## When used with TMC2209 drivers
- Single-wire UART driver control using [TMCStepper](https://github.com/teemuatlut/TMCStepper) library
- Silent and precise low RPM operation combined with up to 1000 RPM maximum speed, using on-demand StealthChop / SpreadCycle mode switching
- In total, only 5 pins required for both steppers (ENABLE, UART_RX, UART_TX, ALT_STEP, AZ_STEP)
- Wide range of microstep capabilities

## When used with A4988 drivers
- Simple to use, legacy driver interface
- Up to 750 RPM
- Steppers controlled directly via MC pins (Common: ENABLE, Per driver: STEP, DIR, MS0, MS1, MS2)
- 1x-16x microstep options

## Bluetooth interface
(Will be described, when stable)
