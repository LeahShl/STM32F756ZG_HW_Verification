#!/usr/bin/bash
EXECUTABLE="./hw_tester"

$EXECUTABLE --all 

# Below are usage examples.
# Uncomment  any of the following lines to run them

# Individual peripheral tests

# $EXECUTABLE -u "UART test message"    # UART test (Optional custom msg)
# $EXECUTABLE -s "SPI test message"     # SPI test  (Optional custom msg)
# $EXECUTABLE -i "I2C test message"     # I2C test  (Optional custom msg)
# $EXECUTABLE -a                        # ADC test
# $EXECUTABLE -t                        # Timer test

# Combined tests

# $EXECUTABLE -usi "Shared message"     # Multiple peripherals
# $EXECUTABLE --all "Universal test"    # All peripherals

# Set number of iterations

# $EXECUTABLE -u -n 5                   # 5 iterations
# $EXECUTABLE --all -n 20               # 20 iterations each

# Data retrieval

# $EXECUTABLE get 1 5 10               # Print specific test results
# $EXECUTABLE export                   # Print all data in a CSV format
# $EXECUTABLE export > "out.csv"       # You can redirect data to any file
