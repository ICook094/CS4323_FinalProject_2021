/*
//Group G
//Sagar Sawant 
//A11524117
//sagar.sawant@okstate.edu
//Description: This program handles the server and client communication. 
//It also allows us to access the server menu and start up structures*/

#include "global.h"
#include "databaseFunctions.h"
#include "database.h"
#define MAXLINE 1024
//Server thread
//Each client will get a separate Server thread to communicate

void * handler_cli(void * h){
		//printf("Handling client...\n");
        struct cli_info * info_c = (struct cli_info *)h; 
        char msg[1024];
        int soc_conn = info_c->conn_start;
        int m = info_c->m;
        int recv_sz = 0;
        int port = info_c->port;
        char * IPaddr = info_c->IPaddr;
		
        //Function is in databaseFunction.c
        startupStructures();
		//Sends the execution to servermenu.c to navigate through the menus
		buyerOrSeller(soc_conn);	//This function is in servermenu.c
		//When the user is done in the menus, the execution will return here
		//We need the execution to return here so the code below can close the connection and free up memory
		
		
		printf("started closing things\n");
		close(soc_conn);
        info_c->verify[m] = -1;
        info_c->verify = NULL;
        free(info_c);
        pthread_exit(NULL);
		printf("done closing everything\n");
}

void * handler_serv(void * h){

        struct serv_info * info_s = (struct serv_info *)h;

        int num_th = info_s->num_th;
        int port = info_s->port;

        pthread_t * serv_th = (pthread_t *)malloc(sizeof(pthread_t) * num_th);
		
		int * verify = (int *)malloc(sizeof(int) * num_th);
		renew(verify, num_th);
        int fd_socket;
        char msg[MAXLINE];
        struct sockaddr_in addr_s, addr_c;

        // Socket creation

        if ( (fd_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0 ) {
                perror("socket creation failed");
                exit(EXIT_FAILURE);
        }

        memset(&addr_s, 0, sizeof(addr_s));
        memset(&addr_c, 0, sizeof(addr_c));

        // Server connectivity information
        addr_s.sin_family = AF_INET; // IPv4
        addr_s.sin_addr.s_addr = INADDR_ANY;//IP
        addr_s.sin_port = htons(port); //Port

        // Binding the socket with server info
        if ( bind(fd_socket, (const struct sockaddr *)&addr_s, sizeof(addr_s)) < 0 )
        {
                perror("bind failed");
                exit(EXIT_FAILURE);
        }

        listen(fd_socket, 10);

        int m = 0;

	printf("\nServer with port %d started\n", port);
		/////////////////////////////////////////////////DEADLOCK FIX///////////////////////////////////////////////////////////////////////////
		///////////////////////////////////////////////////////////////////////////////////////////////////
		///////////////////////////////////////////////////////////////////////////////////////////////////
		
		
        while(1){
				// ******* By adding the equal to sign we can have multiple clients join in. It is not limited******///
				//******** Remove the equal to sign to have deadlock where a client can possibly go into starvation****//////
				if(count_th(verify, num_th) <= num_th){      

                        int len = sizeof(addr_c);
                        int fd_1 = accept(fd_socket, (struct sockaddr *)&addr_c, &len);

                        struct cli_info * info_c = (struct cli_info *)malloc(sizeof(struct cli_info));
						m = chk_space(verify, num_th);

                        verify[m] = 1;

                        info_c->port = ntohs(addr_c.sin_port);
                        info_c->IPaddr = inet_ntoa(addr_c.sin_addr);
						info_c->m = m;
						info_c->conn_start = fd_1;
                        info_c->verify = verify;
						
                        pthread_create(&serv_th[m], NULL, handler_cli, (void *)info_c);
				 }
				 

                }
}
