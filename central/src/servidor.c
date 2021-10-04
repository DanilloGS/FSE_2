// C program to demonstrate
// socket programming in finding ip address
// and port number of connected client
// on Server Side
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#include <pthread.h>

#include "cJSON.h"

#define IP "192.168.0.53"
#define PORTA 10007
#define MAX_SIZE 5000

pthread_t SOCKET_GET, SOCKET_SEND;
int socketCliente;
char *file, *json_string;

char *read_file(int file_id)
{
  char *fp = file_id == 2 ? "./config/configuracao_andar_1.json" : "./config/configuracao_andar_terreo.json";
  long length;
  FILE *file = fopen(fp, "r");
  if (file)
  {
    fseek(file, 0, SEEK_END);
    length = ftell(file);
    fseek(file, 0, SEEK_SET);
    char *buffer = malloc(length + 1);
    if (buffer)
    {
      fread(buffer, 1, length, file);
    }
    fclose(file);
    return buffer;
  }
  else
  {
    exit(0);
  }
}

void send_data()
{
  int value = 0;
  int tamanhoRecebido = strlen(file);
  if (tamanhoRecebido > 0)
  {
    send(socketCliente, value, sizeof(int), 0);
    send(socketCliente, file, tamanhoRecebido, 0);
  }
  sleep(1);
}

void get_data()
{
  char *data = malloc(MAX_SIZE);
  int total_bytes = recv(socketCliente, data, 5000, 0);
  if (total_bytes)
  {
    strcpy(json_string, data);
  }
  printf("%s\n", json_string);
  free(data);
  sleep(1);
}

int main(int argc, char const *argv[])
{
  int servidorSocket;
  int option = 1, floor_id;
  struct sockaddr_in servidorAddr;
  struct sockaddr_in clienteAddr;
  unsigned int clienteLength;

  if ((servidorSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
    perror("Socket");

  memset(&servidorAddr, 0, sizeof(servidorAddr));
  servidorAddr.sin_family = AF_INET;
  servidorAddr.sin_addr.s_addr = inet_addr(IP);
  servidorAddr.sin_port = htons(PORTA);

  setsockopt(servidorSocket, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option));
  if (bind(servidorSocket, (struct sockaddr *)&servidorAddr, sizeof(servidorAddr)) < 0)
    perror("Bind");

  if (listen(servidorSocket, 10) < 0)
    perror("Listen");

  // while (1)
  // {
  //   printf("Qual andar você deseja monitorar:\n\t1)Terreo\n\t2)Primeiro\n");
  //   scanf("%d", &floor_id);
  //   if (floor_id < 1 || floor_id > 2)
  //     printf("ID inválido\n");
  //   else
  //     break;
  // }
  file = malloc(MAX_SIZE);
  json_string = malloc(MAX_SIZE);
  strcpy(file, read_file(atoi(argv[1])));
  // char *file = read_file(floor_id);

  clienteLength = sizeof(clienteAddr);
  if ((socketCliente = accept(servidorSocket,
                              (struct sockaddr *)&clienteAddr,
                              &clienteLength)) < 0)
    perror("Accept");
  while (1)
  {
    pthread_create(&SOCKET_GET, NULL, get_data, NULL);
    pthread_create(&SOCKET_SEND, NULL, send_data, NULL);
    pthread_join(SOCKET_GET, NULL);
    pthread_join(SOCKET_SEND, NULL);
    sleep(1);
  }
  close(servidorSocket);
}
