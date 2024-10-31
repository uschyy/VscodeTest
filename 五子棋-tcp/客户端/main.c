#include <stdio.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <linux/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include "input.h"
#include "lcd.h"
#include "app.h"
#include "client_main.h"
#include "start.h"
#include "macro.h"
int main(int argc, char **argv)
{   
    lcd_init();
    game_init_with_start(argv);
    lcd_uninit();

    return 0;
}
