# CS4323_FinaProject_2021

Compile:
    Server:
        gcc serv1.c servermenu.c database.c databaseFunctions.c -o server -lpthread

    Client:
        gcc client.c -o client

Run Program:
    ./server [starting port number] [number of servers] [number of threads]
        (EX: "./server 2000 3 3" starts up three servers at ports 2000 2001 and 2002)
    ./client

