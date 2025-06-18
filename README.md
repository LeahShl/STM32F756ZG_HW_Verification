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
- Connectors for UART, SPI, I2C, ADC testing
- Stable voltage referenc for ADC testing

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
│   
├── README.md                          # <--- This file
│   
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

## Quick Start

### 1. Build PC Program
1. Open terminal and `cd` into `FILES_FOR_PC`
2. run `make`

### 2. Setup Network Configuration
The STM32 board is configured at the static IP of `10.0.1.100`. Unless you already have a device on your network with this exact IP address, you don't need to change the board's configuration. You do, however, have to make sure your PC is on the same network. Here's how you set up your Linux PC:

1. Identify your ethernet interface name using `ip addr show`. Look for a name like `eth0` or `enp3s0`. replace `<your-eth-interface>` with the name you found for the following commands.
2. add a new IP address to the interface you found (Any IP in `10.0.1.x` range except `10.0.1.100`, I chose `10.0.1.101`):

   ```
   sudo ip addr add 10.0.1.101/24 dev <your-eth-interface>
   ```
3. Set your ether interface up using
   ```
   sudo ip link set <your-eth-interface> up
   ```
4. Verify that the new address is set and the interface is up using:
   ```
   ip addr show <your-eth-interface>
   ```

### 3. Build STM32 Firmware
1. Open STM32CubeIDE
2. Click `File > Open Projects from Filesystem...`
3. Next to `Import source` choose `Directory..`
4. Choose `STM32CUBE_project` from your filesystem
5. Click `Finish`
6. Connect your STM32 board to your PC through the ST-Link USB port.
7. Build the project.

### 4. Setup Hardware

### 5. Run Tests
