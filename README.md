# SPI Test Application

This repository contains a simple C application to demonstrate SPI communication using the `spidev` driver in a Raspberry Pi environment.

## Prerequisites

1. **Raspberry Pi with SPI enabled**: Ensure that SPI is enabled on your Raspberry Pi. This can be done using `raspi-config`.
2. **`spidev` driver**: Ensure the `spidev` module is loaded. This can be checked with `lsmod | grep spidev`.
3. **GCC**: Make sure GCC is installed on your Raspberry Pi.

## File Structure

- `spi_test.c`: The main C application that performs SPI communication.

## How to Compile and Run

1. **Compile the Application**:
    ```sh
    gcc -o spi_test spi_test.c
    ```

2. **Run the Application**:
    ```sh
    sudo ./spi_test
    ```

## Code Explanation

Here's a line-by-line breakdown of the `spi_test.c` file:

### Include Headers

- `#include <stdio.h>`, `#include <stdlib.h>`, `#include <stdint.h>`, `#include <string.h>`: 

    Standard C libraries for input/output, standard utilities, fixed-width integer types, and string handling.
- `#include <fcntl.h>`, `#include <unistd.h>`, `#include <sys/ioctl.h>`: 

    Headers for file control options, UNIX standard functions, and input/output control.
- `#include <linux/spi/spidev.h>`: 

    Header for SPI communication using the `spidev` driver.

### Define SPI Device

- `#define SPI_DEVICE "/dev/spidev0.0"`: 
    
    This macro defines the path to the SPI device file. This may need to be changed depending on your SPI device configuration.

### Main Function

- `int fd = open(SPI_DEVICE, O_RDWR);`: 
    
    Opens the SPI device file for reading and writing. If it fails, an error message is printed, and the program exits.

### Configure SPI Parameters

- `uint8_t mode = 0;`, `uint8_t bits = 8;`, `uint32_t speed = 50000;`, `uint16_t delay = 0;`: 
    
    Defines the SPI parameters.
- `if (ioctl(fd, SPI_IOC_WR_MODE, &mode) < 0 || ioctl(fd, SPI_IOC_RD_MODE, &mode) < 0) { ... }`: 

    Sets the SPI mode using `ioctl` system calls. Error handling is performed for each call.
- `if (ioctl(fd, SPI_IOC_WR_BITS_PER_WORD, &bits) < 0 || ioctl(fd, SPI_IOC_RD_BITS_PER_WORD, &bits) < 0) { ... }`: 

    Sets the bits per word for SPI communication.
- `if (ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed) < 0 || ioctl(fd, SPI_IOC_RD_MAX_SPEED_HZ, &speed) < 0) { ... }`: 

    Sets the maximum speed for SPI communication.

### Prepare Data Buffers

- `char *message = "Hi, from SPI";`, `size_t message_len = strlen(message) + 1;`: 

    Prepares the message to be sent and calculates its length.
- `uint8_t tx[message_len];`, `uint8_t rx[message_len];`:

     Declares transmission (`tx`) and reception (`rx`) buffers.
- `memset(tx, 0, sizeof(tx));`, `memset(rx, 0, sizeof(rx));`:

     Initializes the buffers to zero.
- `strncpy((char *)tx, message, sizeof(tx) - 1);`: 

    Copies the message to the `tx` buffer.

### SPI Transfer Structure

- `struct spi_ioc_transfer tr = { ... };`: 

    Defines the SPI transfer structure with designated initializers:
  - `.tx_buf = (unsigned long)tx,`
  - `.rx_buf = (unsigned long)rx,`
  - `.len = message_len,`
  - `.delay_usecs = delay,`
  - `.speed_hz = speed,`
  - `.bits_per_word = bits,`

### Perform SPI Transfer

- `if (ioctl(fd, SPI_IOC_MESSAGE(1), &tr) < 0) { ... }`: 

    Sends the SPI transfer message and checks for errors.

### Output Results

- `printf("Sent: %s\n", tx);`, `printf("Received: %s\n", rx);`: 

    Prints the sent and received messages.

### Clean Up

- `close(fd);`: Closes the SPI device file.
- `return EXIT_SUCCESS;`: Exits the program with a success status.
