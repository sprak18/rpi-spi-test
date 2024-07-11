#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/spi/spidev.h>

#define SPI_DEVICE "/dev/spidev0.0"

int main() {
    int fd = open(SPI_DEVICE, O_RDWR);
    if (fd < 0) {
        perror("Failed to open SPI device");
        return EXIT_FAILURE;
    }

    uint8_t mode = 0;
    uint8_t bits = 8;
    uint32_t speed = 50000;
    uint16_t delay = 0;

    if (ioctl(fd, SPI_IOC_WR_MODE, &mode) < 0 || ioctl(fd, SPI_IOC_RD_MODE, &mode) < 0) {
        perror("Failed to set SPI mode");
        close(fd);
        return EXIT_FAILURE;
    }

    if (ioctl(fd, SPI_IOC_WR_BITS_PER_WORD, &bits) < 0 || ioctl(fd, SPI_IOC_RD_BITS_PER_WORD, &bits) < 0) {
        perror("Failed to set bits per word");
        close(fd);
        return EXIT_FAILURE;
    }

    if (ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed) < 0 || ioctl(fd, SPI_IOC_RD_MAX_SPEED_HZ, &speed) < 0) {
        perror("Failed to set max speed");
        close(fd);
        return EXIT_FAILURE;
    }

    char *message = "Hi, from SPI";
    size_t message_len = strlen(message) + 1; // +1 to include null terminator
    uint8_t tx[message_len];
    uint8_t rx[message_len];

    memset(tx, 0, sizeof(tx));
    memset(rx, 0, sizeof(rx));
    strncpy((char *)tx, message, sizeof(tx) - 1);

    struct spi_ioc_transfer tr = {
        .tx_buf = (unsigned long)tx,
        .rx_buf = (unsigned long)rx,
        .len = message_len,
        .delay_usecs = delay,
        .speed_hz = speed,
        .bits_per_word = bits,
    };

    if (ioctl(fd, SPI_IOC_MESSAGE(1), &tr) < 0) {
        perror("Failed to transfer SPI message");
        close(fd);
        return EXIT_FAILURE;
    }

    printf("Sent: %s\n", tx);
    printf("Received: %s\n", rx);

    close(fd);
    return EXIT_SUCCESS;
}
