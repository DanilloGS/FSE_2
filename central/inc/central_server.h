#ifndef SERVIDOR_H_
#define SERVIDOR_H_

void send_data(int socketCliente, char *file);
void get_data(int socketCliente);
void central_socket(int id);

#endif /* SERVIDOR_H_ */
