#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cJSON.h"

int print_intro()
{
    system("clear");
    printf(
        "\t██████  ██████   ██████       ██ ███████ ████████  ██████      ██████\n"
        "\t██   ██ ██   ██ ██    ██      ██ ██         ██    ██    ██          ██\n"
        "\t██████  ██████  ██    ██      ██ █████      ██    ██    ██      █████  \n"
        "\t██      ██   ██ ██    ██ ██   ██ ██         ██    ██    ██     ██      \n"
        "\t██      ██   ██  ██████   █████  ███████    ██     ██████      ███████ \n");
    int file_id;
    while (1)
    {
        printf("\n\nQual andar você deseja monitorar primeiro:\n\t1)Terreo\n\t2)Primeiro\n\n");
        scanf("%d", &file_id);
        if (file_id < 1 || file_id > 2)
        {
            printf("\nID inválido\n");
        }
        else
            return file_id;
    }
}

void print_menu(char *file)
{
    if (strlen(file) > 0)
    {
        cJSON *json = cJSON_Parse(file);
        system("clear");
        int temp = cJSON_GetObjectItemCaseSensitive(json, "temperature")->valueint;
        int hum = cJSON_GetObjectItemCaseSensitive(json, "humidity")->valueint;
        int total_people = cJSON_GetObjectItemCaseSensitive(json, "total_people")->valueint;
        int *nome = cJSON_GetObjectItemCaseSensitive(json, "nome")->valuestring;

        cJSON *outputs = cJSON_GetObjectItemCaseSensitive(json, "output");
        cJSON *inputs = cJSON_GetObjectItemCaseSensitive(json, "input");
        cJSON *pin = NULL;
        printf("Monitorando: %s\n", nome);
        printf("Temperatura: %d⁰C\t Úmidade: %d%%\t Pessoas no Prédio: %d\n", temp, hum, total_people);
        printf("\n\nOutput:\n\n       Pino\t\tValor\t\t\tTag\n");
        cJSON_ArrayForEach(pin, outputs)
        {
            int gpio = cJSON_GetObjectItemCaseSensitive(pin, "gpio")->valueint;
            char *tag = cJSON_GetObjectItemCaseSensitive(pin, "tags")->valuestring;
            int value = cJSON_GetObjectItemCaseSensitive(pin, "value")->valueint;
            char *status = value ? "ON " : "OFF";
            printf("|\t%02d\t|\t%s\t|\t%s\n", gpio, status, tag);
        }
        printf("\n\nInput:\n\n       Pino\t\tValor\t\t\tTag\n");
        cJSON_ArrayForEach(pin, inputs)
        {
            int gpio = cJSON_GetObjectItemCaseSensitive(pin, "gpio")->valueint;
            char *tag = cJSON_GetObjectItemCaseSensitive(pin, "tags")->valuestring;
            int value = cJSON_GetObjectItemCaseSensitive(pin, "value")->valueint;
            char *status = value ? "ON " : "OFF";
            printf("|\t%02d\t|\t%s\t|\t%s\n", gpio, status, tag);
        }
    }
}