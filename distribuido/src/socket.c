
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#include <pthread.h>

#include "cJSON.h"
#include "gpio.h"
#include "dht22.h"
#include "json.h"

// #define IP "127.0.0.1"
#define IP "192.168.0.53"
#define PORTA 10207
#define MAX_SIZE 5000
pthread_t SOCKET_GET, SOCKET_SEND;

char *mensagem;
int clienteSocket;
int temperature = 0, humidity = 0;

void get_request()
{
    int data_type = 0, gpio_pin;
    recv(clienteSocket, data_type, sizeof(int), MSG_WAITALL);
    switch (data_type)
    {
    case 0:
        // Get JSON
        recv(clienteSocket, mensagem, MAX_SIZE, 0);
        printf("%s\n", mensagem);
        break;
    case 1:
        // Toggle output/input
        recv(clienteSocket, gpio_pin, sizeof(int), 0);
        read_gpio(gpio_pin) ? turn_off(gpio_pin) : turn_on(gpio_pin);
        break;
    }
    sleep(1);
}

void send_data()
{
    if (strlen(mensagem) > 0)
    {
        Server *server_config = malloc(sizeof(Server) + 1);
        parse_json_string(mensagem, server_config);
        read_dht_data(&temperature, &humidity, 0);
        final_json(&mensagem, server_config, temperature, humidity, -1);
        send(clienteSocket, mensagem, MAX_SIZE, 0);
    }
    sleep(1);
}

void main_socket()
{
    struct sockaddr_in servidorAddr;
    unsigned short servidorPorta;
    char *IP_Servidor;
    mensagem = calloc(sizeof(char), 5000);
    IP_Servidor = inet_addr(IP);
    servidorPorta = PORTA;

    if ((clienteSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
    {
        perror("Socket");
        exit(0);
    }
    servidorAddr.sin_family = AF_INET;
    servidorAddr.sin_addr.s_addr = IP_Servidor;
    servidorAddr.sin_port = htons(servidorPorta);

    if (connect(clienteSocket, (struct sockaddr *)&servidorAddr,
                sizeof(servidorAddr)) < 0)
    {
        perror("Connect");
        exit(0);
    }

    while (1)
    {
        pthread_create(&SOCKET_GET, NULL, get_request, NULL);
        pthread_create(&SOCKET_SEND, NULL, send_data, NULL);
        pthread_join(SOCKET_GET, NULL);
        pthread_join(SOCKET_SEND, NULL);
    }
}
