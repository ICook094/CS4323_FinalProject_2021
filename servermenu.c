//Written by: Scott Schwager
//A20151349
//sschwag@okstate.edu
//Group G

//#include "global.h"
#include <stdio.h>
#include <string.h>

void buyerOrSeller(int soc_conn) {
	
	//This function determines the role of each user
	//Then it calls getUserInfo() with the role as an argument
	//Then it calls the function to show the menu for the user (currently only showSellerMenu(), will add showBuyerMenu() later)
	//This function will be called from the server after a client is accepted
	
	//will need to change printf to write or send
	//will need to change scanf to read or recv
	//signal to client that server expects input or assume that client will provide write after every time it reads?
	//separate funtion to updateUserInfo or reuse getUserInfo?
	
	char msg[1024];
	int option;
	
	bzero(msg, sizeof(msg));
	strcat(msg, "Please choose your role:\n1. Buyer\n2. Seller\n");
	write(soc_conn, msg, sizeof(msg));
	write(soc_conn, "input", sizeof("input"));	//Ask for input from client
	bzero(msg, sizeof(msg));
	read(soc_conn, msg, sizeof(msg));
	option = atoi(msg);
	
	if (option == 1) {
		printf("Buyer\n");
		getUserInfo("Buyer", soc_conn);
		showBuyerMenu(soc_conn);
	} else {
		printf("Seller\n");
		getUserInfo("Seller", soc_conn);
		showSellerMenu(soc_conn);
	}
	return;
}

void getUserInfo(char userRole[6], int soc_conn) {
	
	//This function gets the user information
	//arguments: userRole (either "Buyer" or "Seller")
	//The user role determines whether the information is stored in sellerInfo table or customerInfo table
	//The information that is gathered is the same regardless of the user's role
	//This function is called by buyerOrSeller()
	
	char msg[1024];
	char userId[50];
	char userName[50];
	char userPhone[12];
	char userAddress[50];
	
	//Get userId
	bzero(msg, sizeof(msg));
	strcat(msg, "ID:\n");
	write(soc_conn, msg, sizeof(msg));
	write(soc_conn, "input", sizeof("input"));	//Ask for input from client
	bzero(msg, sizeof(msg));
	read(soc_conn, msg, sizeof(msg));
	strcpy(userId, msg);
	
	//Get userName
	bzero(msg, sizeof(msg));
	strcat(msg, "Name:\n");
	write(soc_conn, msg, sizeof(msg));
	write(soc_conn, "input", sizeof("input"));	//Ask for input from client
	bzero(msg, sizeof(msg));
	read(soc_conn, msg, sizeof(msg));
	strcpy(userName, msg);
	
	//Get userPhone
	bzero(msg, sizeof(msg));
	strcat(msg, "Phone XXX-XXX-XXXX:\n");
	write(soc_conn, msg, sizeof(msg));
	write(soc_conn, "input", sizeof("input"));	//Ask for input from client
	bzero(msg, sizeof(msg));
	read(soc_conn, msg, sizeof(msg));
	strcpy(userPhone, msg);
	
	//Get userAddress
	bzero(msg, sizeof(msg));
	strcat(msg, "Address:\n");
	write(soc_conn, msg, sizeof(msg));
	write(soc_conn, "input", sizeof("input"));	//Ask for input from client
	bzero(msg, sizeof(msg));
	read(soc_conn, msg, sizeof(msg));
	strcpy(userAddress, msg);
	
	return;
	
	/*if (strcmp(userRole, "Buyer") == 0) {
		newCustomer();
	} else {
		newSeller();
	}*/
	
	//Likely will put this info into a struct of some sort
	//This information is then entered into the sellerInformation or customerInformation database depending on the userRole (Buyer or Seller)
}

void showSellerMenu(int soc_conn) {
	
	//This function shows the available options to sellers and then calls the functions for those options
	//This function is called by buyerOrSeller()
	
	int option;
	char msg[1024];
	int done = 0;
	
	while (done != 1) {
		bzero(msg, sizeof(msg));
		strcat(msg, "\nWhat to you want to do?\n");
		strcat(msg, "1. Update User Information\n");
		strcat(msg, "2. Add a new product\n");
		strcat(msg, "3. Remove an existing product\n");
		strcat(msg, "4. Update the quantity of a product\n");
		strcat(msg, "5. Update the price of a product\n");
		strcat(msg, "6. View my products for sale\n");
		strcat(msg, "7. View orders for my products\n");
		strcat(msg, "8. Exit the program\n");
		
		write(soc_conn, msg, sizeof(msg));
		write(soc_conn, "input", sizeof("input"));	//Ask for input from client
		bzero(msg, sizeof(msg));
		read(soc_conn, msg, sizeof(msg));
		switch(atoi(msg)) {
			case 1:
				bzero(msg, sizeof(msg));
				strcat(msg, "Please Enter Updated User Information:\n");
				write(soc_conn, msg, sizeof(msg));
				write(soc_conn, "input", sizeof("input"));
				bzero(msg, sizeof(msg));
				read(soc_conn, msg, sizeof(msg));
				printf("%s\n", msg);
				break;
			case 2:
				addNewProduct(soc_conn);
				
				break;
			case 3:
				bzero(msg, sizeof(msg));
				strcat(msg, "Product ID of product to be removed:\n");
				write(soc_conn, msg, sizeof(msg));
				write(soc_conn, "input", sizeof("input"));
				bzero(msg, sizeof(msg));
				read(soc_conn, msg, sizeof(msg));
				printf("%s\n", msg);
				break;
			case 4:
				bzero(msg, sizeof(msg));
				strcat(msg, "Product ID of product to be updated quantity:\n");
				write(soc_conn, msg, sizeof(msg));
				write(soc_conn, "input", sizeof("input"));
				bzero(msg, sizeof(msg));
				read(soc_conn, msg, sizeof(msg));
				printf("%s\n", msg);
				break;
			case 5:
				bzero(msg, sizeof(msg));
				strcat(msg, "Product ID of product to be updated price:\n");
				write(soc_conn, msg, sizeof(msg));
				write(soc_conn, "input", sizeof("input"));
				bzero(msg, sizeof(msg));
				read(soc_conn, msg, sizeof(msg));
				printf("%s\n", msg);
				break;
			case 6:
				bzero(msg, sizeof(msg));
				strcat(msg, "These are the products that you offer:\n");
				write(soc_conn, msg, sizeof(msg));
				break;
			case 7:
				bzero(msg, sizeof(msg));
				strcat(msg, "These are the orders for your products:\n");
				write(soc_conn, msg, sizeof(msg));
				break;
			case 8:
				write(soc_conn, "exit", sizeof("exit"));
				done = 1;
				break;
		}
	}
	return;
}

void writeNoInput(int soc_conn, char writeThis[1024]) {
	char msg[1024];
	bzero(msg, sizeof(msg));
	strcat(msg, writeThis);
	write(soc_conn, msg, sizeof(msg));
	return;
}
	

void addNewProduct(int soc_conn) {
	char msg[1024];
	int productID;
	char description[500];
	int sellerID;
	int numAvailable;
	double price;
	
	writeNoInput(soc_conn, "Please enter product information:\n");
	
	//Get productID
	writeNoInput(soc_conn, "Product ID:\n");
	write(soc_conn, "input", sizeof("input"));
	bzero(msg, sizeof(msg));
	read(soc_conn, msg, sizeof(msg));
	productID = atoi(msg);
	
	//Get description
	writeNoInput(soc_conn, "Description:\n");
	write(soc_conn, "input", sizeof("input"));
	bzero(msg, sizeof(msg));
	read(soc_conn, msg, sizeof(msg));
	strcpy(description, msg);
	
	//Get sellerID
	writeNoInput(soc_conn, "SellerID:\n");
	write(soc_conn, "input", sizeof("input"));
	bzero(msg, sizeof(msg));
	read(soc_conn, msg, sizeof(msg));
	sellerID = atoi(msg);	//Need to check if this sellerID is a valid foreign key?
	
	//Get numAvailable
	writeNoInput(soc_conn, "Quantity Available:\n");
	write(soc_conn, "input", sizeof("input"));
	bzero(msg, sizeof(msg));
	read(soc_conn, msg, sizeof(msg));
	numAvailable = atoi(msg);
	
	//Get Price
	writeNoInput(soc_conn, "Price:\n");
	write(soc_conn, "input", sizeof("input"));
	bzero(msg, sizeof(msg));
	read(soc_conn, msg, sizeof(msg));
	price = atof(msg);
	
	//newProduct()
	return;
}

void showBuyerMenu(int soc_conn) {
	
	//This function shows the available options to buyers and then calls the functions for those options
	//This function is called by buyerOrSeller()
	
	//Likely will loop this menu so a user can do multiple actions consecutively
	
	int option;
	char msg[1024];
	int done = 0;
	
	while (done != 1) {
		bzero(msg, sizeof(msg));
		strcat(msg, "\nWhat to you want to do?\n");
		strcat(msg, "1. Update User Information\n");
		strcat(msg, "2. Purchase a product\n");
		strcat(msg, "3. Return a product\n");
		strcat(msg, "4. View available products\n");
		strcat(msg, "5. View orders you have placed\n");
		strcat(msg, "6. View your billing information\n");
		strcat(msg, "7. Exit the program\n");
		
		write(soc_conn, msg, sizeof(msg));
		write(soc_conn, "input", sizeof("input"));	//Ask for input from client
		bzero(msg, sizeof(msg));
		read(soc_conn, msg, sizeof(msg));
		switch(atoi(msg)) {
			case 1:
				break;
				printf("Please Enter Updated User Information:\n");
			case 2:
				printf("Product ID of product you want to buy:\n");
				break;
			case 3:
				printf("Product ID of product to be returned:\n");
				break;
			case 4:
				printf("These are the available products to buy with their prices:\n");
				break;
			case 5:
				printf("These are the orders you have placed:\n");
				break;
			case 6:
				printf("This is your billing information:\n");
				break;
			case 7:
				write(soc_conn, "exit", sizeof("exit"));
				done = 1;
				break;
		}
	}
	return;
}

/*int main() {
	//This is the driver function for testing purposes.
	//This code will be put into the server code, to be ran after the server accepts a client.
	buyerOrSeller();
	return 0;
}*/