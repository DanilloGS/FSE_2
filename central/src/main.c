#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include "servidor.h";
#include "menu.h";

int main(int argc, char const *argv[])
{
    // print_intro();
    // int file_id;
    // while (1)
    // {
    //   printf("Qual andar você deseja monitorar:\n\t1)Terreo\n\t2)Primeiro\n");
    //   scanf("%d", &file_id);
    //   if (file_id < 1 || file_id > 2)
    //     printf("ID inválido\n");
    //   else
    //     break;
    // }
    main_socket(atoi(argv[1]));
    return 0;
}
