# STM32F746ZG Hardware Verification System
A client-server testing framework for STM32F746ZG peripheral validation.

## Project Overview
This project implements a hardware verification system designed for manufacturing quality assurance of STM32F746ZG microcontroller cards. The system validates the correctness of critical peripherals through automated testing via UDP communication.

### Architecture
The system follows a client-server architecture:
- **Server (PC Testing Program)**: Linux-based C program that orchestrates tests and logs results
- **Client (UUT Testing Program)**: STM32F746ZG firmware that executes peripheral tests
- **Communication**: Custom UDP protocol over Ethernet (LWIP stack)

### Tested Peripherals

| Peripheral | Test Method | Key Features |
|------------|-------------|--------------|
| **UART** | Loopback via UART4 <-> UART5 | Partial DMA mode, CRC validation |
| **SPI** | Loopback communication (slave <-> master) | DMA mode, CRC validation |
| **I2C** | Loopback communication (slave <-> master) | DMA mode, CRC validation |
| **ADC** | Known voltage comparison | 12-bit resolution, predefined reference values |
| **Timer** | Pulse counting/timing | DMA mode, precision measurement |

## System Requirements

### Hardware
- STM32F746ZG development board
- Ethernet connection between PC and STM32
- Connectors for UART, SPI, I2C testing
- Stable voltage reference for ADC testing

### Software
- **PC Side**: Linux system with GCC and Make
- **STM32 Side**: STM32CubeIDE or compatible ARM toolchain
- Network configuration: PC and STM32 on same subnet

## Project Structure
Key:
- regular file
- \[Directory\]

```
HW_Verification/
├── [FILES_FOR_PC]                     # PC server files
│   ├── [debug_tools]
│   │   └── dummy_server.c             # Mock server to test main.c in abscence of a UUT
│   ├── main.c                         # Program's main file
│   ├── Makefile                       # Compile with GNU make
│   ├── README.md                      # Readme file for th PC code
│   ├── tests_db.c                     # SQLite database source file
│   ├── tests_db.h                     # SQLite database header file
│   └── usage_example.sh               # Usage examples for PC server
├── README.md                          # <--- This file
└── [STM32CUBE_project]                # STM32 Project files, automatically generated unchanged files skipped
    ├── [Core]
    │   ├── [Src]
    │   │   ├── main.c                 # STM32 main file
    ├── HW_Verification.ioc            # IOC File with the MX configuration
    └── [UUT]                          # All project files for the UUT are here
        ├── [Inc]
        │   ├── adc_test.h             # ADC test header file
        │   ├── hw_verif_crc.h         # CRC header file
        │   ├── hw_verif.h             # HW verification main header file
        │   ├── hw_verif_udp.h         # UDP communication header file
        │   ├── i2c_test.h             # I2C test header file
        │   ├── spi_test.h             # SPI test header file
        │   ├── test_consts.h          # Shared definitions between different tests
        │   ├── tests.h                # Generic testing functions header file
        │   ├── timer_test.h           # TIM test header file
        │   └── uart_test.h            # UART test header file
        └── [Src]
            ├── adc_test.c             # ADC test source file
            ├── hw_verif.c             # HW verification main source file
            ├── hw_verif_crc.c         # CRC source file
            ├── hw_verif_udp.c         # UDP communication source file
            ├── i2c_test.c             # I2C test source file
            ├── spi_test.c             # SPI test source file
            ├── tests.c                # Generic testing functions source file
            ├── timer_test.c           # TIM test source file
            └── uart_test.c            # UART test source file


```

