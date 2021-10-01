#ifndef JSON_H_
#define JSON_H_

#include "cJSON.h";

typedef struct Server
{
    int porta;
    char ip[20], nome[20];
    cJSON *outputs, *inputs;
} Server;

void parse_json_string(char *json_string, Server *server);
cJSON *final_json(Server *server, double temperature, double humidity, int total_people);

#endif /* JSON_H_ */