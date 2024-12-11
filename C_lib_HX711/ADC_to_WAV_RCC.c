#include <ADC_to_WAV_RCC.h>



// Глобальный флаг остановки
volatile unsigned char stop_flag = 0;



void writeWAVHeader(FILE *file, uint32_t sampleRate, uint32_t dataSize){
    WAVHeader header = {
        .chunkID = "RIFF",
        .chunkSize = 36 + dataSize,
        .format = "WAVE",
        .subchunk1ID = "fmt ",
        .subchunk1Size = 16,
        .audioFormat = 1,
        .numChannels = 1,
        .sampleRate = sampleRate,
        .byteRate = sampleRate * BITS_PER_SAMPLE / 8,
        .blockAlign = BITS_PER_SAMPLE / 8,
        .bitsPerSample = BITS_PER_SAMPLE,
        .subchunk2ID = "data",
        .subchunk2Size = dataSize
    };
    fwrite(&header, sizeof(WAVHeader), 1, file);
    return;
}


int SPI_Init(){
    int spi_fd = open(SPI_DEVICE, O_RDWR);
    if (spi_fd < 0) {
        perror("Failed to open SPI device");
        return 0;
    }
    
    uint8_t mode = 0;
    ioctl(spi_fd, SPI_IOC_WR_MODE, &mode);
    ioctl(spi_fd, SPI_IOC_WR_BITS_PER_WORD, &(uint8_t){12});
    ioctl(spi_fd, SPI_IOC_WR_MAX_SPEED_HZ, &(uint32_t){SPI_SPEED});
    return spi_fd;
}


char WAV_File_Start(int spi_fd){
    uint32_t samplesPerFile = SAMPLE_RATE * FILE_DURATION_SECONDS;  // Количество сэмплов на 10 минут
    uint32_t dataSizePerFile = samplesPerFile * BITS_PER_SAMPLE / 8;

    uint16_t buffer;
    uint32_t sampleCount = 0;
    int fileIndex = 0;


    while(!stop_flag){
        // Открываем новый файл
        char filename[256];
        snprintf(filename, sizeof(filename), "output_%03d.wav", fileIndex++);
        FILE *wavFile = fopen(filename, "wb");
        if (!wavFile) {
            perror("Failed to open WAV file");
            return -1;
        }
        
        // Пишем заголовок
        writeWAVHeader(wavFile, SAMPLE_RATE, dataSizePerFile);
        for (uint32_t i = 0; i < samplesPerFile; i++) {
            if (stop_flag) break;

            if (read(spi_fd, &buffer, sizeof(buffer)) != sizeof(buffer)) {
                perror("SPI read error");
                fclose(wavFile);
                close(spi_fd);
                return -1;
            }

            // Пишем данные в файл
            fwrite(&buffer, sizeof(buffer), 1, wavFile);

            // Усыпляем для точного выполнения частоты выборки
            usleep(10);  // Подстройка для 100 кГц
        }

        // Закрываем файл
        fclose(wavFile);

        // Если stop_flag был выставлен, выходим
        if (stop_flag) break;

    }
    close(spi_fd);
    return 0;
}