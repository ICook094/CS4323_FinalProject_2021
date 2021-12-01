/*
//Group G
//Sagar Sawant
//A11524117
//sagar.sawant@okstate.edu
//Description: Creating a program which allows us to create mutiple servers and connect them to clients
// This lets you input the number of servers to be started and how many clients each server can handle*/


#define MAXLINE 1024
#include "handler.c"
struct serv_info * info_s;



int main(int argc, char *argv[]) {

        //This gives us the number of servers and threads per server that will connect to the clients
		//Each client can is connected with thread to the server 

        if(argc < 4){
                printf("Format is Server.out [starting port] [serv_count] [num_th]\n");
                exit(1);
        }

        int port_1 = atoi(argv[1]);

        int serv_count =  atoi(argv[2]);

        info_s = (struct serv_info *)malloc(sizeof(struct serv_info) * serv_count);

        int num_th = atoi(argv[3]);

        pthread_t * t = (pthread_t *)malloc(sizeof(pthread_t) * serv_count);
 
 for(int i = 0; i < serv_count; i++){

                info_s[i].port = port_1++;

                info_s[i].num_th = num_th;

                pthread_create(&t[i], NULL, handler_serv, (void *)&info_s[i]);
        }

        pthread_exit(NULL);

        free(t);
        free(info_s);

        return 0;
}