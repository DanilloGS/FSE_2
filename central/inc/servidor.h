
#ifndef SERVIDOR_H_
#define SERVIDOR_H_
char *read_file(int file_path);

void send_data(int socketCliente, char *file);

void get_data(int socketCliente);

int socket(int file_id);
#endif /* SERVIDOR_H_ */
