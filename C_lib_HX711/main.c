#include "HX711_RVV_asm.h"
#include "ADC_to_WAV_RCC.h"

#include <stdio.h>
#include <unistd.h>
#include <pthread.h>


void *hx711_task(void *arg) {
    while (1) {
        printf("HX711: Reading data...\n");
        usleep(8333);  // 120 Гц
    }
    return NULL;
}



void *adc_task(void *arg) {
    while (1) {
        printf("ADC: Recording audio...\n");
        usleep(10);  // ~100 кГц
    }
    return NULL;
}




int main(void){
    // initSensors(GPIO_PIN18_CLK, GPIO_PIN15_DT1, GPIO_PIN16_DT2, GPIO_PIN17_DT3);

    // SensorsData  data = readSensors(GPIO_PIN18_CLK, GPIO_PIN15_DT1, GPIO_PIN16_DT2, GPIO_PIN17_DT3);
    // printf("%d\n", data.sens_1);
    // printf("%d\n", data.sens_2);
    // printf("%d\n", data.sens_3);
    // printf("\n");
    pthread_t hx711_thread, adc_thread;

    pthread_create(&hx711_thread, NULL, hx711_task, NULL);
    pthread_create(&adc_thread, NULL, adc_task, NULL);

    pthread_join(hx711_thread, NULL);
    pthread_join(adc_thread, NULL);
    
    return 0;
}