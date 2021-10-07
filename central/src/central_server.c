#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#include <pthread.h>
#include <signal.h>

#include "cJSON.h"
#include "home.h"
#include "file.h"

#define IP "192.168.0.53"
#define PORTA 10207
#define MAX_SIZE 5000
#define true 1
#define false 0

pthread_t SOCKET_GET, SOCKET_SEND, MENU_ID;
int socketCliente, file_id;
char *file, *json_string, toggle_gpio_value = false;

void get_data()
{
  char *data = malloc(MAX_SIZE);
  int buffer_size = recv(socketCliente, data, 5000, 0);
  if (buffer_size)
  {
    strcpy(json_string, data);
  }
  free(data);
  sleep(1);
}

void send_data()
{
  int value = 0;
  int file_size = strlen(file);
  if (file_size > 0)
  {
    if (!toggle_gpio_value)
      send(socketCliente, file, file_size, 0);
    else
    {
      char *pin[5];
      printf("\nQual pino você deseja mudar o valor: ");
      scanf("%s", pin);
      send(socketCliente, pin, sizeof(char) * 5, 0);
      toggle_gpio_value = false;
    }
  }
  sleep(1);
}

void handle_signal(int signal)
{
  printf("\nAguarde...\n");
  if (signal == SIGQUIT)
  {
    toggle_gpio_value = true;
  }
  if (signal == SIGTSTP)
  {
    file_id = file_id == 1 ? 2 : 1;
    strcpy(file, read_file(file_id));
  }
}

void central_socket(int id)
{
  signal(SIGQUIT, handle_signal);
  signal(SIGTSTP, handle_signal);
  int servidorSocket;
  int option = 1;
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

  file = malloc(MAX_SIZE);
  json_string = malloc(MAX_SIZE);
  strcpy(file, read_file(file_id));

  clienteLength = sizeof(clienteAddr);
  if ((socketCliente = accept(servidorSocket,
                              (struct sockaddr *)&clienteAddr,
                              &clienteLength)) < 0)
    perror("Accept");
  while (1)
  {
    pthread_create(&MENU_ID, NULL, print_menu, json_string);
    pthread_join(MENU_ID, NULL);
    pthread_create(&SOCKET_GET, NULL, get_data, NULL);
    pthread_create(&SOCKET_SEND, NULL, send_data, NULL);
    pthread_join(SOCKET_GET, NULL);
    pthread_join(SOCKET_SEND, NULL);
    sleep(1);
  }
  close(servidorSocket);
}
