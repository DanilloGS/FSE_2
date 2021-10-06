
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

char *mensagem, *data;
int clienteSocket;
int temperature = 0, humidity = 0, toggle_value = 0;

void get_request()
{
    int i;
    char num[5];
    recv(clienteSocket, data, MAX_SIZE, 0);
    perror("Teste1");
    for (i = 0; data[i] != '\0'; i++)
        if (i > 3)
            break;
    printf("%s\n", data);
    if (i < 3)
    {
        toggle_value = 1;
        strcpy(num, data);
        int value = read_gpio(atoi(num));
        printf("%s\n", num);
        if(value == 0){
            printf("Ligou\n");
            turn_on(atoi(num));
        } else {
            printf("Desligou\n");
            turn_off(atoi(num));
        }
    }
    else
    {
        toggle_value = 0;
        strcpy(mensagem, data);
    }
    sleep(1);
}

void send_data()
{
    if (strlen(mensagem) > 0 && !toggle_value)
    {
        char *final = malloc(MAX_SIZE);
        perror("Teste 1");
        Server *server_config = malloc(sizeof(Server) + 1);
        perror("Teste 2");
        parse_json_string(mensagem, server_config);
        perror("Teste 3");
        read_dht_data(&temperature, &humidity, 0);
        perror("Teste 4");
        final_json(&final, server_config, temperature, humidity, -1);
        perror("Teste 5");
        send(clienteSocket, final, MAX_SIZE, 0);
        perror("Teste 6");
        free(final);
    }
    sleep(1);
}

void main_socket()
{
    struct sockaddr_in servidorAddr;
    unsigned short servidorPorta;
    char *IP_Servidor;
    mensagem = calloc(MAX_SIZE, sizeof(char));
    data = calloc(MAX_SIZE, sizeof(char));
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
    // get_request();
    while (1)
    {
        pthread_create(&SOCKET_GET, NULL, get_request, NULL);
        pthread_join(SOCKET_GET, NULL);
        pthread_create(&SOCKET_SEND, NULL, send_data, NULL);
        pthread_join(SOCKET_SEND, NULL);
    }
}
