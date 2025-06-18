# STM32F756ZG Tester
A Linux-based testing program for STM32F756ZG peripheral validation via UDP communication.

## Overview
This program sends test commands to an STM32F756ZG microcontroller over UDP and logs the results to a local database. It supports testing of multiple peripherals including UART, SPI, I2C, ADC, and Timer modules.

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
The `debug_tool` directory contains a dummy server to test the PC software in the abscence of a STM32 board. The dummy server listens to incoming requests, then return success to any incoming test request. Since I developed the PC software before the STM32 board software, I had to test my software without the board. I kept this dummy server in case someone would want to run this project without a working board.

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
./main [OPTIONS]
./main [COMMAND]
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
