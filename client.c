#include <stdio.h>
#include <netdb.h>
#include <unistd.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#define MAX 1024
#define SERVER_LEN 255
#define SA struct sockaddr

char name [100];
void comms(int soc_conn)
{
    char msg[MAX];
    int n;
	
	
    for (;;) {
        bzero(msg, sizeof(msg));
        read(soc_conn, msg, sizeof(msg));
        
        if ((strncmp(msg, "exit", 4)) == 0) {
            printf("Client Disconnected....\n");
            break;
        } else if ((strncmp(msg, "input", 5)) == 0) {	//This is when the server asks for input
			bzero(msg, sizeof(msg));
			scanf("%s", msg);
			//printf("%s\n", msg);		//For troubleshooting
			write(soc_conn, msg, sizeof(msg));
			//printf("Client wrote something\n");	//For troubleshooting
		} else {
			printf("%s", msg);
		}
		
		//Below is Sagar's code for getting input but it confused me so I did input a different way (see above) -Scott
		/*bzero(msg, sizeof(msg));
		n = 0;
        while ((msg[n++] = getchar()) != '\n');
		write(soc_conn, msg, sizeof(msg));*/
    }
}

int main(int argc, char * argv[])
{
	printf("Please enter your name: ");
	scanf ("%s", name);
    int soc_conn;
    struct sockaddr_in add_serv, cli;
    char serv_name[SERVER_LEN + 1] = {0};
    int port_num;

    port_num = argc > 2 ? atoi(argv[2]) : 0;
	 if (!port_num) {
        printf("\n\n%s Please enter Server port: ", name);
        scanf("%d", &port_num);
    }

    // socket create and varification
    soc_conn = socket(AF_INET, SOCK_STREAM, 0);
    if (soc_conn == -1) {
        printf("Socket Creation Failed...\n");
        exit(0);
    }
    else
        printf("Socket created..\n");
    bzero(&add_serv, sizeof(add_serv));

    // assign IP, PORT
    add_serv.sin_family = AF_INET;
    add_serv.sin_port = htons(port_num);
    add_serv.sin_addr.s_addr = htonl(INADDR_ANY);


    // connect the client socket to server socket
    if (connect(soc_conn, (SA*)&add_serv, sizeof(add_serv)) != 0) {
        printf("Connection with the Server Failed...\n");
        exit(0);
    }
    else
        printf("Connected to Server..\n");

    // function for chat
    comms(soc_conn);

    // close the socket
    close(soc_conn);
}