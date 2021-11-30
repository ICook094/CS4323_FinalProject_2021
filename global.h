#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <pthread.h>

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

void * handler_serv(void * h);

void buyerOrSeller(int soc_conn);