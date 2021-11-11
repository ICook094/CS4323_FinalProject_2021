/*Sagar Sawant 
*Group G
*A11524117
*sagar.sawant@okstate.edu
*Description: This is a header file which includes all the header files
*that are being used by all the .c files
*
**/

#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <pthread.h>
#define MAX 1024
#define MAXLINE 1024
#define SERVER_LEN 255
#define SA struct sockaddr

struct serv_info{

        int port;
        int num_th;
};

struct cli_info {
        int port;
        char * IPaddr;
        int m;
        int * verify;
        int conn_start;
};

int count_th(int * verify, int length){

        int n = 0;
        for(int i = 0; i < length; i++){
                if(verify[i] != -1)
                        n++;
        }

        return n;
}

void renew(int * verify, int length){

        for(int i = 0; i < length; i++){
                verify[i] = -1;
        }
}

int chk_space(int * verify, int length){

        for(int i = 0; i < length; i++){

                if(verify[i] == -1)
                        return i;
        }

        return -1;
}