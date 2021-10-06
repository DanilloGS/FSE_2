#include <stdio.h>
#include "central_server.h";
#include "home.h";

int main()
{
    int file_id = print_intro();
    central_socket(file_id);
    return 0;
}
