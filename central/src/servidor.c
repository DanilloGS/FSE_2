#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

void TrataClienteTCP(int socketCliente)
{
	char buffer[16];
	int tamanhoRecebido;

	if ((tamanhoRecebido = recv(socketCliente, buffer, 16, 0)) < 0)
		printf("Erro no recv()\n");

	while (tamanhoRecebido > 0)
	{
		if (send(socketCliente, buffer, tamanhoRecebido, 0) != tamanhoRecebido)
			printf("Erro no envio - send()\n");

		if ((tamanhoRecebido = recv(socketCliente, buffer, 16, 0)) < 0)
			printf("Erro no recv()\n");
	}
}

char *read_file(int file_path)
{
	char *fp = file_path == 1 ? "./config/configuracao_andar_1.json" : "./config/configuracao_andar_terreo.json";
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

int main(int argc, char *argv[])
{
	int servidorSocket;
	int socketCliente;
	struct sockaddr_in servidorAddr;
	struct sockaddr_in clienteAddr;
	unsigned short servidorPorta;
	unsigned int clienteLength;

	servidorPorta = 10007;

	// Abrir Socket
	if ((servidorSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
		printf("falha no socker do Servidor\n");

	// Montar a estrutura sockaddr_in
	memset(&servidorAddr, 0, sizeof(servidorAddr)); // Zerando a estrutura de dados
	servidorAddr.sin_family = AF_INET;
	servidorAddr.sin_addr.s_addr = htonl("192.168.0.53");
	servidorAddr.sin_port = htons(servidorPorta);

	// Bind
	if (bind(servidorSocket, (struct sockaddr *)&servidorAddr, sizeof(servidorAddr)) < 0)
	{
		perror("Bind");
		exit(0);
	}

	// Listen
	if (listen(servidorSocket, 10) < 0)
		printf("Falha no Listen\n");

	while (1)
	{
		clienteLength = sizeof(clienteAddr);
		if ((socketCliente = accept(servidorSocket,
									(struct sockaddr *)&clienteAddr,
									&clienteLength)) < 0)
			printf("Falha no Accept\n");

		printf("Conexão do Cliente %s\n", inet_ntoa(clienteAddr.sin_addr));

		TrataClienteTCP(socketCliente);
		close(socketCliente);
	}
	close(servidorSocket);
	// char *buffer = read_file(atoi(argv[1]));
	// printf("%s\n", buffer);
}
