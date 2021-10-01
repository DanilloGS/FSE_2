#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <wiringPi.h>

#include "cJSON.h"
#include "socket.h"
#include "gpio.h"
#include "json.h"

int main(int argc, char const *argv[])
{
    wiringPiSetup();
    char *json_string = malloc(100000);
    Server *server_config = malloc(sizeof(Server) + 1);
    int output_values[20], input_values[20];
    client_socket(json_string);
    parse_json_string(json_string, server_config);
    cJSON *json_send = final_json(server_config, 30.0, 23.0, 59);
    printf("%s\n", cJSON_Print(json_send));
}
