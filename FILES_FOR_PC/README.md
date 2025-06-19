# Linux CLI Program for STM32F756ZG Verification
A Linux-based testing program for STM32F756ZG peripheral validation via UDP communication.

## Content
1. [Overview](#overview)
2. [Compilation](#compilation)
3. [Network Configuration](#network-configuration)
4. [Usage](#usage)
5. [Usage Examples](#usage-examples)
6. [SQLite3 Database](#sqlite3-database)

## Overview
This program sends test commands to an STM32F756ZG microcontroller over UDP and logs the results to a local database. It supports testing of multiple peripherals including UART, SPI, I2C, ADC, and Timer modules.

### File Structure
Key:
- regular file
- \[Directory\]
```
[FILES_FOR_PC]                     # PC server files
├── [debug_tools]
│   └── dummy_server.c             # Mock server to test main.c in abscence of a UUT
├── main.c                         # Program's main file
├── Makefile                       # Compile with GNU make
├── README.md                      # <--- This file
├── tests_db.c                     # SQLite database source file
├── tests_db.h                     # SQLite database header file
└── usage_example.sh               # Usage examples
```

## Compilation
### Before compilation
1. If you're using a different IP address, change `UUT_ADDR` in `main.c`
2. If you want the database to be saved locally in the current working directory instead of the user's home directory, add to `tests_db.h` the line `#define LOCAL_DB_PATH 1`. The database will be saved wherever the program was run from, so make sure you `cd` into the correct directory before running the program.

### Prerequisites
- GCC compiler
- Make utility
- Standard C libraries
- POSIX-compatible system (Linux/Unix)

### Build Instructions
```
# Compile the program
make

# Clean build artifacts
make clean
```

## Network Configuration
### Communicating with the STM32 Board
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

### Using Dummy Server
The `debug_tools` directory contains a dummy server to test the PC software in the abscence of a STM32 board. The dummy server listens to incoming requests, then return success to any incoming test request. Since I developed the PC software before the STM32 board software, I had to test my software without the board. I kept this dummy server in case someone would want to run this project without a working board.

To use the dummy server:
1. Change `UUT_ADDR` in `main.c` to `127.0.0.1` and compile it with the above instructions.
2. Compile `dummy_server.c` with
   ```
   gcc dummy_server.c -o dummy_server
   ```
3. run `./dummy_server`
4. run `./hw_tester` from a different terminal with any option you want.

## Usage
### Command Syntax
```
./hw_tester [OPTIONS]
./hw_tester [COMMAND]
```

### Test Options

| Flag | Description | Message Support |
|------|-------------|-----------------|
| `-u [msg]` | Run UART test | Optional custom message |
| `-s [msg]` | Run SPI test | Optional custom message |
| `-i [msg]` | Run I2C test | Optional custom message |
| `-a` | Run ADC test | No message |
| `-t` | Run Timer test | No message |
| `--all [msg]` | Run all tests | Message applies to UART/SPI/I2C only |

### Additional Options

| Flag | Description |
|------|-------------|
| `-n <int>` | Set number of test iterations (0-255) |
| `-h, --help` | Show help message |

### Data Retrieval Commands

| Command | Description |
|---------|-------------|
| `get <id1> <id2> ...` | Print test data by test ID(s) |
| `export` | Print all test data in CSV format |

### Rules and Constraints
When using testing options (i.e. not when using `get` or `export` commands):
1. **Required Flags**: At least one test flag (`-u`, `-s`, `-i`, `-a`, `-t`, or `--all`) must be specified
2. **No Repetition**: Each flag can only appear once
3. **Message Rules**: 
   - Flags `a` and `t` cannot accept messages
   - Stacked flags with `a` or `t` cannot be followed by a message
   - Communication flags (`u`, `s`, `i`) can share a message when stacked
4. **Iteration Limit**: Number of iterations must be between 0-255

When using `get` command, at least one test ID must be given.

`export` command must show up alone.

### Exit codes
On successful run, the program returns 0. If an error occured, the program returns one of the following codes:

| Error Name   | Code    | Description                       |
| ------------ | ------- | --------------------------------- |
| ARGS_ERROR | 1 | Error parsing command line arguments |
| UDP_ERROR | 2 | UDP communication error |
| SQLITE_ERROR | 3 | SQLite3 database error |


## Usage Examples
### Basic Testing
```
# Test UART with default message
./hw_tester -u

# Test SPI with custom message
./hw_tester -s "Custom SPI test"

# Test multiple peripherals
./hw_tester -u -s -i

# Test ADC and Timer (no messages)
./hw_tester -a -t
```

### Stacked Flags
```
# Test UART, SPI, and I2C with shared message
./hw_tester -usi "Shared test message"

# Test ADC and Timer together
./hw_tester -at

# Mixed stack with separate ADC test
./hw_tester -si "Communication test" -a
```

### Iterations and All Tests
```
# Run all tests with 10 iterations each
./hw_tester --all -n 10

# Run UART test 5 times with custom message
./hw_tester -u "Iteration test" -n 5

# Run all tests with shared message for communication peripherals
./hw_tester --all "Universal test message"
```

### Data Retrieval Commands
```
# Print specific test result
./hw_tester get 123

# Print multiple test results
./hw_tester get 12 34 56

# Export all results to CSV file
./hw_tester export > tests.csv
```
## SQLite3 Database
This program logs each test's metadata and result to a local SQLite3 database for tracking and review purposes.

### Schema
| Column       | Type    | Description                       |
| ------------ | ------- | --------------------------------- |
| `test_id` | INTEGER | Auto-incremented primary key |
| `timestamp` | TEXT | ISO 8601-formatted time of test |
| `duration` | REAL| Test duration in seconds |
| `result` | INTEGER | Result code (1=PASS, 0=FAIL) |

### Storage Location
Unless `LOCAL_DB_PATH` is defined, the database will be stored in:
```
~/HW_tester/records.db
```
### Data retrieval
See [**Usage**](#usage) and [**Usage Examples**](#usage-examples) sections above.

The raw database format and the exported CSV data are compatible to use for further view and data analysis with several tools like SQLite3 CLI, Excel, python's SQLite3 module and more.
