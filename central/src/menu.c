#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cJSON.h"

void print_intro()
{
    system("clear");
    printf(
        "\t██████  ██████   ██████       ██ ███████ ████████  ██████      ██████\n"
        "\t██   ██ ██   ██ ██    ██      ██ ██         ██    ██    ██          ██\n"
        "\t██████  ██████  ██    ██      ██ █████      ██    ██    ██      █████  \n"
        "\t██      ██   ██ ██    ██ ██   ██ ██         ██    ██    ██     ██      \n"
        "\t██      ██   ██  ██████   █████  ███████    ██     ██████      ███████ \n");
}

void print_menu(cJSON *json)
{
    system("clear");
    int temp = cJSON_GetObjectItemCaseSensitive(json, "temperature")->valueint;
    int hum = cJSON_GetObjectItemCaseSensitive(json, "humidity")->valueint;
    cJSON *outputs = cJSON_GetObjectItemCaseSensitive(json, "output");
    cJSON *inputs = cJSON_GetObjectItemCaseSensitive(json, "input");
    cJSON *pin = NULL;
    printf("Temperatura: %d⁰C\t Úmidade: %d%%\n\n", temp, hum);
    printf("Output\nPino\tTag => Valor\n");
    cJSON_ArrayForEach(pin, outputs)
    {
        int gpio = cJSON_GetObjectItemCaseSensitive(pin, "gpio")->valueint;
        char *tag = cJSON_GetObjectItemCaseSensitive(pin, "tags")->valuestring;
        int value = cJSON_GetObjectItemCaseSensitive(pin, "value")->valueint;
        printf("%d\t%s => %d\n", gpio, tag, value);
    }
    printf("\nInput\nPino\tTag => Valor\n");
    cJSON_ArrayForEach(pin, inputs)
    {
        int gpio = cJSON_GetObjectItemCaseSensitive(pin, "gpio")->valueint;
        char *tag = cJSON_GetObjectItemCaseSensitive(pin, "tags")->valuestring;
        int value = cJSON_GetObjectItemCaseSensitive(pin, "value")->valueint;
        printf("%d\t%s => %d\n", gpio, tag, value);
    }
}