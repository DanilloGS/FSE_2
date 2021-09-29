#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <wiringPi.h>
#include "cJSON.h"

typedef struct Server {
  int porta;
  char ip[15], nome[15];
  cJSON *outputs, *inputs;
} Server;

Server *server_config;

char *read_file(char *file_path) {
  char *fp = file_path ? file_path : "../config/configuracao_andar_terreo.json";
  char *buffer = 0;
  long length;
  FILE *f = fopen(fp, "r");

  if (f) {
    fseek(f, 0, SEEK_END);
    length = ftell(f);
    fseek(f, 0, SEEK_SET);
    buffer = malloc(length);
    if (buffer) {
      fread(buffer, 1, length, f);
    }
    fclose(f);
    return buffer;
  } else {
    perror("fopen");
    return "Error";
  }
}

void create_json(char *json_string, Server *server_config) {
  cJSON *json = cJSON_Parse(json_string);
  cJSON *porta, *ip, *outputs, *output, *inputs, *input, *nome;

  strcpy(server_config->ip, cJSON_GetObjectItemCaseSensitive(json, "ip")->valuestring);
  strcpy(server_config->nome, cJSON_GetObjectItemCaseSensitive(json, "nome")->valuestring);
  server_config->porta = cJSON_GetObjectItemCaseSensitive(json, "porta")->valueint;
  server_config->outputs = cJSON_GetObjectItemCaseSensitive(json, "outputs");
  server_config->inputs = cJSON_GetObjectItemCaseSensitive(json, "inputs");
}

int main(int argc, char const *argv[]) {
  wiringPiSetup();
  read_gpio(atoi(argv[1]));
  // struct Server server_config;
  // int total_pins;
  // char *json_string = read_file(argv[1]);
  // server_config = create_json(json_string);

  // create_gpio_reference(server_config.outputs);
  // create_gpio_reference(server_config.inputs);

}
