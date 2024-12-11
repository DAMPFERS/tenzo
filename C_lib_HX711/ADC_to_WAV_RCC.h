#ifndef ADC_TO_WAV_FILE
#define ADC_TO_WAV_FILE

#include <stdio.h>
#include <stdint.h>
#include <fcntl.h>
#include <linux/spi/spidev.h>
#include <sys/ioctl.h>
#include <unistd.h>

#define SPI_DEVICE "/dev/spidev0.0"
#define SPI_SPEED 1000000  // 1 MHz
#define SAMPLE_RATE 100000  // 100 kHz
#define BITS_PER_SAMPLE 16
#define FILE_DURATION_SECONDS 600  // 10 минут


typedef struct {
    char chunkID[4];  // "RIFF"
    uint32_t chunkSize;
    char format[4];   // "WAVE"
    char subchunk1ID[4];  // "fmt "
    uint32_t subchunk1Size;
    uint16_t audioFormat;
    uint16_t numChannels;
    uint32_t sampleRate;
    uint32_t byteRate;
    uint16_t blockAlign;
    uint16_t bitsPerSample;
    char subchunk2ID[4];  // "data"
    uint32_t subchunk2Size;
} WAVHeader;


void writeWAVHeader(FILE *file, uint32_t sampleRate, uint32_t dataSize);
int SPI_Init();
char WAV_File_Start(int spi_fd);

#endif
