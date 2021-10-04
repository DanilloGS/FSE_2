
#include <wiringPi.h>

#include "socket.h"


int main(int argc, char const *argv[])
{
    wiringPiSetup();
    main_socket();
}
