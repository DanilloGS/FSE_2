#include <stdio.h>
#include <dirent.h>
#include <fcntl.h>
#include <assert.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <bcm2835.h>
#include <unistd.h>

#define MAXTIMINGS 100
#define TEMP_UMI_PIN 21

int dht22(float *temperature, float *humidity)
{
    int bits[250], data[100];
    int bitidx = 0;
    int counter = 0;
    int laststate = 0x1;
    int j = 0;

    // Set GPIO pin to output
    bcm2835_gpio_fsel(TEMP_UMI_PIN, BCM2835_GPIO_FSEL_OUTP);

    bcm2835_gpio_write(TEMP_UMI_PIN, HIGH);
    usleep(500000); // 500 ms
    bcm2835_gpio_write(TEMP_UMI_PIN, LOW);
    usleep(20000);

    bcm2835_gpio_fsel(TEMP_UMI_PIN, BCM2835_GPIO_FSEL_INPT);

    data[0] = data[1] = data[2] = data[3] = data[4] = 0;

    while (bcm2835_gpio_lev(TEMP_UMI_PIN) == 1)
    {
        usleep(1);
    }
    for (int i = 0; i < MAXTIMINGS; i++)
    {
        counter = 0;
        while (bcm2835_gpio_lev(TEMP_UMI_PIN) == laststate)
        {
            counter++;
            if (counter == 1000)
                break;
        }
        laststate = bcm2835_gpio_lev(TEMP_UMI_PIN);
        if (counter == 1000)
            break;
        bits[bitidx++] = counter;

        if ((i > 3) && (i % 2 == 0))
        {
            data[j / 8] <<= 1;
            if (counter > 200)
                data[j / 8] |= 1;
            j++;
        }
    }

    if ((j >= 39) &&
        (data[4] == ((data[0] + data[1] + data[2] + data[3]) & 0xFF)))
    {
        *humidity = data[0] * 256 + data[1];
        *humidity /= 10;

        *temperature = (data[2] & 0x7F) * 256 + data[3];
        *temperature /= 10.0;
        if (data[2] & 0x80)
            *temperature *= -1;
        return 1;
    }
    printf("Erro na leitura da temperatura e humidade\n");
    return 0;
}