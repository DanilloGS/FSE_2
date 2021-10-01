#include "json.h";
#include "gpio.h";

void parse_json_string(char *json_string, Server *server)
{
    cJSON *json = cJSON_Parse(json_string);
    cJSON *porta, *ip, *outputs, *output, *inputs, *input, *nome;

    strcpy(server->ip, cJSON_GetObjectItemCaseSensitive(json, "ip")->valuestring);
    strcpy(server->nome, cJSON_GetObjectItemCaseSensitive(json, "nome")->valuestring);
    server->porta = cJSON_GetObjectItemCaseSensitive(json, "porta")->valueint;
    server->outputs = cJSON_GetObjectItemCaseSensitive(json, "outputs");
    server->inputs = cJSON_GetObjectItemCaseSensitive(json, "inputs");
}

cJSON *final_json(Server *server, double temperature, double humidity, int total_people)
{
    cJSON *gpio_object = cJSON_CreateObject();
    int count = 2;
    for (int i = 0; i < count; i++)
    {
        char *gpio_type = i ? "output" : "input";
        cJSON *gpio = i ? server->outputs : server->inputs;

        cJSON *gpio_array = cJSON_CreateArray();
        int len = cJSON_GetArraySize(gpio);
        for (int j = 0; j < len; j++)
        {
            cJSON *item = cJSON_GetArrayItem(gpio, j);
            char *type = cJSON_GetObjectItemCaseSensitive(item, "type")->valuestring;
            if (strcmp(type, "contagem") != 0)
            {
                char *tag = cJSON_GetObjectItemCaseSensitive(item, "tag")->valuestring;
                int value = read_gpio(cJSON_GetObjectItemCaseSensitive(item, "gpio")->valueint);
                cJSON *array_item = cJSON_CreateObject();
                cJSON_AddStringToObject(array_item, "type", type);
                cJSON_AddStringToObject(array_item, "tags", tag);
                cJSON_AddNumberToObject(array_item, "value", value);
                cJSON_AddItemToArray(gpio_array, array_item);
            }
        }
        cJSON_AddItemToObject(gpio_object, gpio_type, gpio_array);
    }
    cJSON_AddNumberToObject(gpio_object, "temperature", temperature);
    cJSON_AddNumberToObject(gpio_object, "humidity", humidity);
    cJSON_AddNumberToObject(gpio_object, "total_people", total_people);
    return gpio_object;
}