#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "cJSON.h"

typedef struct Server {
  int porta;
  char ip[15], nome[15];
  cJSON *outputs, *inputs;
} Server;

typedef struct Pins {
  int gpio;
  char type[50], tag[20];
} Pins;

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
    return "Error";
    perror("fopen");
  }
}

struct Server create_json(char *json_string) {
  Server server;
  cJSON *json = cJSON_Parse(json_string);
  cJSON *porta, *ip, *outputs, *output, *inputs, *input, *nome;

  strcpy(server.ip, cJSON_GetObjectItemCaseSensitive(json, "ip")->valuestring);
  strcpy(server.nome, cJSON_GetObjectItemCaseSensitive(json, "nome")->valuestring);
  server.porta = cJSON_GetObjectItemCaseSensitive(json, "porta")->valueint;
  server.outputs = cJSON_GetObjectItemCaseSensitive(json, "outputs");
  server.inputs = cJSON_GetObjectItemCaseSensitive(json, "inputs");

  return server;
}

void create_gpio_reference(cJSON *json_pins, Pins *pins) {
  cJSON *json_pin = NULL;
  int i = 0;
  cJSON_ArrayForEach(json_pin, json_pins) {
    strcpy(pins[i].type, cJSON_GetObjectItemCaseSensitive(json_pin, "type")->valuestring);
    strcpy(pins[i].tag, cJSON_GetObjectItemCaseSensitive(json_pin, "tag")->valuestring);
    pins[i].gpio = cJSON_GetObjectItemCaseSensitive(json_pin, "gpio")->valueint;
    i++;
  }
}

int main(int argc, char const *argv[]) {
  struct Server server;
  struct Pins *output_pins, *input_pins;
  int total_pins;
  char *json_string = read_file(argv[1]);
  server = create_json(json_string);

  total_pins = cJSON_GetArraySize(server.outputs);
  output_pins = malloc(sizeof(Pins) * total_pins);
  total_pins = cJSON_GetArraySize(server.inputs);
  input_pins = malloc(sizeof(Pins) * total_pins);

  create_gpio_reference(server.outputs, output_pins);
  create_gpio_reference(server.inputs, input_pins);

}
