/*Sagar Sawant 
*Group G
*A11524117
*sagar.sawant@okstate.edu
*Description: This is a client program that will enable 
*the program to connect with a server for basic TCP connection
*
**/

#include "global.h"

char name [100];
void comms(int soc_conn)
{
    char msg[MAX];
    int n;
	
	
    for (;;) {
        bzero(msg, sizeof(msg));
        n = 0;
        while ((msg[n++] = getchar()) != '\n');
        write(soc_conn, msg, sizeof(msg));
        bzero(msg, sizeof(msg));
        read(soc_conn, msg, sizeof(msg));
        if ((strncmp(msg, "exit", 4)) == 0) {
            printf("Client Disconnected....\n");
            break;
        }
    }
}

int main(int argc, char * argv[])
{
	printf("Client please enter your name: ");
	scanf ("%s", name);
    int soc_conn;
    struct sockaddr_in add_serv, cli;
    char serv_name[SERVER_LEN + 1] = {0};
    int port_num;

    port_num = argc > 2 ? atoi(argv[2]) : 0;
	 if (!port_num) {
        printf("\n\n%s Please enter one of the availble server ports: ", name);
        scanf("%d", &port_num);
    }

    
    soc_conn = socket(AF_INET, SOCK_STREAM, 0);
    if (soc_conn == -1) {
        printf("Socket Creation Failed...\n");
        exit(0);
    }
    else
        printf("Socket created..\n");
    bzero(&add_serv, sizeof(add_serv));
	
    add_serv.sin_family = AF_INET;
    add_serv.sin_port = htons(port_num);
    add_serv.sin_addr.s_addr = htonl(INADDR_ANY);


    if (connect(soc_conn, (SA*)&add_serv, sizeof(add_serv)) != 0) {
        printf("Connection with the Server Failed...\n");
        exit(0);
    }
    else
        printf("Connected to Server..\n");

    comms(soc_conn);
    close(soc_conn);
}
