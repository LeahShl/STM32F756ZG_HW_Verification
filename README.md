# STM32F746ZG Hardware Verification System
A client-server testing framework for STM32F746ZG peripheral verification.

## Content
1. [Project Overview](#project-overview)
2. [System Requirements](#system-requirements)
3. [Project Structure](#project-structure)
4. [Usage](#usage)
5. [Communication Protocol](#communication-protocol)
6. [Testing Methodology](#testing-methodology)

## Project Overview
This project implements a hardware verification system designed for manufacturing quality assurance of STM32F746ZG microcontroller cards. The system validates the correctness of critical peripherals through automated testing via UDP communication.

### Architecture

![Project's Block Diagram](https://github.com/LeahShl/STM32F756ZG_HW_Verification/blob/main/block_diagram.png "Project's Block Diagram")

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
- PC with Ethernet port
- Ethernet cable to connect PC and STM32
- Connectors for UART, SPI, I2C, ADC testing

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

## Usage

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
#### Ethernet
**Setup**: Connect the STM32 board directly to PC with an Ethernet cable.
#### UART
UART GPIO Configuration:
| Peripheral | Type | GPIO Port |
|------------|-----|-----------|
| UART4 | TX | PC10 |
| UART4 | RX | PC11 |
| UART5 | TX | PC12 |
| UART5 | RX | PD2 |

**Setup**: Connect PC10 (UART4_TX) to PD2 (UART5_RX) and PC11 (UART4_RX) to PD12 (UART5_TX).

#### I2C
I2C1 is set as master and I2C2 is set as slave.
I2C GPIO Configuration:
| Peripheral | Type | GPIO Port |
|------------|-----|-----------|
| I2C1 | SCL | PB8 |
| I2C1 | SDA | PB9 |
| I2C2 | SCL | PB10 |
| I2C2 | SDA | PB11 |

**Setup**: Connect PB8 (I2C1_SCL) to PB10 (I2C2_SCL) and PB9 (I2C1_SDA) to PB11 (I2C2_SDA).

#### SPI
SPI1 is set as master and SPI4 is set as slave. Hardware NSS is used.
SPI GPIO configuration:
| Peripheral | Type | GPIO Port |
|------------|-----|-----------|
| SPI1 | NSS | PA4 |
| SPI1 | SCK | PA5 |
| SPI1 | MISO | PA6 |
| SPI1 | MOSI | PB5 |
| SPI4 | NSS | PE4 |
| SPI4 | SCK | PE2 |
| SPI4 | MISO | PE5 |
| SPI4 | MOSI | PE6 |

**Setup**: Connect PA4 (SPI1_NSS) to PE4 (SPI4_NSS), PA5 (SPI1_SCK) to PE2 (SPI4_SCK), PA6 (SPI1_MISO) to PE5 (SPI4_MISO) and PB5 (SPI1_MOSI) to PE6 (SPI4_MOSI).

#### ADC
ADC GPIO configuration:
| Peripheral | Type | GPIO Port |
|------------|-----|-----------|
| ADC1 | IN0 | PA0 |

**Setup**: Connect PA0 (ADC1/0) to 3V3 voltage.

#### Timer
No hardware setup needed.

### 5. Run Tests
For more examples consult `FILES_FOR_PC/usage_example.sh` file and the [PC program's README](https://github.com/LeahShl/STM32F756ZG_HW_Verification/blob/main/FILES_FOR_PC/README.md ).

1. Run tests
   ```
   ./hw_verif --all                          # Run all tests
   ./hw_verif --all -n 10                    # Run all test with specified number of iterations
   ./hw_verif -usi "bitpatternfoobar"        # Run specific tests with custom message (ascii bit pattern)
   ./hw_verif -uit                           # Run specific tests without custom message
   ```
2. Print specific test(s) result(s) by test ID
   ```
   ./hw_verif get 123                        # Print single test
   ./hw_verif get 12 34 56                   # Print multiple tests
   ```
3. Export all test data to CSV (default prints to stdout)
   ```
   ./hw_verif export > data.csv
   ```
4. Display help
   ```
   ./hw_verif --help
   ```
## Communication Protocol

### Command Format (PC -> STM32)
```
┌─────────────┬─────────────┬───────────┬─────────────┬─────────────────┐
│  Test ID    │ Peripheral  │ Iterations│  Bit Length │   Bit Pattern   │
│  (4 bytes)  │  (1 byte)   │ (1 byte)  │  (1 byte)   │  (0-255 bytes)  │
└─────────────┴─────────────┴───────────┴─────────────┴─────────────────┘
```

### Response Format (STM32 -> PC)
```
┌─────────────┬─────────────┐
│  Test ID    │   Result    │
│  (4 bytes)  │  (1 byte)   │
│             │ 0x01=PASS   │
│             │ 0xFF=FAIL   │
└─────────────┴─────────────┘
```

## Testing Methodology
### Communication Peripherals (UART, SPI, I2C) - CRC Verified Loopback Test
1. A bit pattern is sent from one peripheral to another.
2. The bit pattern is sent back to the first peripheral.
3. The incoming bit pattern is compared to the original via CRC.
4. The peripheral passes the test if all iterations passed.

### ADC Test
1. A single test passes if ADC sample is within a specified tolerance.
2. The peripheral passes the test if all iterations passed.

### Timer Test
1. TIM2 (advanced timer) is set to count up.
2. TIM6 (basic timer) is set to restart every 10ms.
3. TIM2 count is sampled every time TIM6 restarts, until N_SAMPLES(=10) are retrieved.
4. A single test passes if all samples are spaced 54,000 counts from each other.
5. The peripheral passes the test if all iterations passed.
