# CS4323_FinaProject_2021

Project Description:
    This project implements an online store where users can buy and sell products. The server allows multiple users to connect at a time. Sellers can change what products they sell, what the prices for their products are, and how many of each product are available to purchase. Buyers can place orders for products, return products they have ordered, and view orders they have placed. Information for each buyer, seller, product, order, and billing are stored and accessed by the data server. 

Project Status:
    For the most part this project is in complete and working order give or take a few program ending bugs. 
    The project was worked on and completed by all members of the team.

How to Compile:

    Server:
        gcc serv1.c servermenu.c database.c databaseFunctions.c -o server -lpthread

    Client:
        gcc client.c -o client

How to Run Program:

    Server:
        ./server [starting port number] [number of servers] [number of threads]

    (EX: "./server 2000 3 3" starts up three servers at ports 2000 2001 and 2002)

    Client:
        ./client

WARNING when entering information into the program avoid the use of a space

Known Bugs:
    - A segmentation fault may happen after several restarts of the program, we were not able to identify the cause of this issue
    - The program will sometimes randomly issue a segmentation fault for seemlingly no reason this may be because of improper memory management

Copyright:
    This code is the legal property of the childhood gaurdians of Cole Hutson, Isabelle Cook, Scott Schwager, and Sagar Sawanta (the authors). By looking at this copyright you are in violation of the copyright, and as such you agree to call the authors' gaurdians everyday in the case that one or more of the authors suffer a fatal or dihibilitating injury. Failure to follow these procedures may result in legal action and will resullt in a fine of no less than $0.50.

    
