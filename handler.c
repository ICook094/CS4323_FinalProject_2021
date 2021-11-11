/*Sagar Sawant 
*Group G
*A11524117
*sagar.sawant@okstate.edu
*Description: This is the handler code for server and client which 
*handles the communication between the two
**/

#include "global.h"

void * handler_cli(void * h){

        struct cli_info * info_c = (struct cli_info *)h;
        char msg[1024];
        int soc_conn = info_c->conn_start;
        int m = info_c->m;
        int recv_sz = 0;
        int port = info_c->port;
        char * IPaddr = info_c->IPaddr;


        memset(msg, '\0', 1024);
		sprintf(msg, "\n1. Seller\n2. Buyer\n3. Exit the program\n\nEnter your choice ");
		send(soc_conn, msg, strlen(msg), 0);
        memset(msg, '\0', 1024);


        sleep(1);
        while(1){

                write(1, msg, strlen(msg));
				
				memset(msg, '\0', 1024);
		recv_sz = recv(soc_conn, msg, 1024, 0);
		msg[recv_sz] = '\0';
		
//Seller page open
		if(msg[0] == '1'){
			//Client NAME
			memset(msg, '\0', 1024);
			sprintf(msg, "\n\n\n***SELLER PAGE***\n\nPLEASE ENTER\nFull Name: ");
			send(soc_conn, msg, strlen(msg), 0);
			memset(msg, '\0', 1024);
			recv_sz = recv(soc_conn, msg, 1024, 0);
			msg[recv_sz] = '\0';
			char *name_cli = (char *)malloc(75);
			strcpy(name_cli, msg);
			memset(msg, '\0', 1024);
			
			//Client Seller ID
			memset(msg, '\0', 1024);
			sprintf(msg, "\nSeller ID: ");
			send(soc_conn, msg, strlen(msg), 0);
			memset(msg, '\0', 1024);
			recv_sz = recv(soc_conn, msg, 1024, 0);
			msg[recv_sz] = '\0';
			char *ID_cli = (char *)malloc(12);
			strcpy(ID_cli, msg);
			memset(msg, '\0', 1024);

			//Client Phone number
			memset(msg, '\0', 1024);
			sprintf(msg, "\n10-digit Phone Number: ");
			send(soc_conn, msg, strlen(msg), 0);
			memset(msg, '\0', 1024);
			recv_sz = recv(soc_conn, msg, 1024, 0);
			msg[recv_sz] = '\0';
			char *phone_cli = (char *)malloc(12);
			strcpy(phone_cli, msg);
			memset(msg, '\0', 1024);

			//Client Address
			memset(msg, '\0', 1024);
			sprintf(msg, "\nContact address: ");
			send(soc_conn, msg, strlen(msg), 0);
			memset(msg, '\0', 1024);
			recv_sz = recv(soc_conn, msg, 1024, 0);
			msg[recv_sz] = '\0';
			char *addr_cli = (char *)malloc(75);
			strcpy(addr_cli, msg);
			memset(msg, '\0', 1024);
		}

				

     }

        close(soc_conn);
        info_c->verify[m] = -1;
        info_c->verify = NULL;
        free(info_c);
        pthread_exit(NULL);

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

        if ( (fd_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0 ) {
                perror("socket creation failed");
                exit(EXIT_FAILURE);
        }

        memset(&addr_s, 0, sizeof(addr_s));
        memset(&addr_c, 0, sizeof(addr_c));

        addr_s.sin_family = AF_INET;
        addr_s.sin_addr.s_addr = INADDR_ANY;
        addr_s.sin_port = htons(port); 

       
        if ( bind(fd_socket, (const struct sockaddr *)&addr_s, sizeof(addr_s)) < 0 )
        {
                perror("bind failed");
                exit(EXIT_FAILURE);
        }

        listen(fd_socket, 10);

        int m = 0;

	printf("\nServer with port %d started\n", port);

        while(1){
				
				 if(count_th(verify, num_th) < num_th){

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

