
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
#include "message_formater.h"

#define IP "192.168.0.53"
#define PORTA 10207
#define MAX_SIZE 5000
pthread_t SOCKET_GET, SOCKET_SEND;

char *message, *data;
int clientSocket;
int temperature = 0, humidity = 0, total_people = 0, toggle_value = 0;

void get_request()
{
    int i;
    char num[5];
    recv(clientSocket, data, MAX_SIZE, 0);
    for (i = 0; data[i] != '\0'; i++)
        if (i > 3)
            break;
    if (i < 3)
    {
        printf("Recebi porta: %d", atoi(num));
        toggle_value = 1;
        strcpy(num, data);
        int value = read_gpio(atoi(num));
        if (value == 0)
        {
            printf("Ligando %d\n", atoi(num));
            turn_on(atoi(num));
        }
        else
        {
            printf("Desligando%d\n", atoi(num));
            turn_off(atoi(num));
        }
    }
    else
    {
        toggle_value = 0;
        strcpy(message, data);
    }
    sleep(1);
}

void send_data()
{
    if (strlen(message) > 0 && !toggle_value)
    {
        char *final = malloc(MAX_SIZE);
        Server *server_config = malloc(sizeof(Server));
        parse_json_string(message, server_config);
        read_dht_data(&temperature, &humidity, 0);
        final_json(&final, server_config, temperature, humidity, &total_people);
        send(clientSocket, final, MAX_SIZE, 0);
        free(final);
        free(server_config);
    }
    sleep(1);
}

void main_socket()
{
    struct sockaddr_in serverAddr;
    unsigned short serverPort;
    char *IP_Server;
    message = calloc(MAX_SIZE, sizeof(char));
    data = calloc(MAX_SIZE, sizeof(char));
    IP_Server = inet_addr(IP);
    serverPort = PORTA;

    if ((clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
    {
        perror("Socket");
        exit(0);
    }
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = IP_Server;
    serverAddr.sin_port = htons(serverPort);

    while (connect(clientSocket, (struct sockaddr *)&serverAddr,
                sizeof(serverAddr)) < 0);

    while (1)
    {
        pthread_create(&SOCKET_GET, NULL, get_request, NULL);
        pthread_join(SOCKET_GET, NULL);
        pthread_create(&SOCKET_SEND, NULL, send_data, NULL);
        pthread_join(SOCKET_SEND, NULL);
    }
}
