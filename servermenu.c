//Written by: Scott Schwager
//A20151349
//sschwag@okstate.edu
//Group G

//#include "global.h"
#include "databaseFunctions.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

void writeNoInput(int soc_conn, char writeThis[1024]);
void getUserInfo(char userRole[6], int soc_conn);
void showSellerMenu(int soc_conn);
void showBuyerMenu(int soc_conn);


void buyerOrSeller(int soc_conn) {
	
	//This function determines the role of each user
	//Then it calls getUserInfo() with the role as an argument
	//Then it calls the function to show the proper menu (buyer menu or seller menu) for the user 
	//This function will be called from the server after a client is accepted
	
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
		getUserInfo("Buyer ", soc_conn);
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
	char userName[50];
	char userPhone[50];
	char userAddress[50];

	//Get userName
	bzero(msg, sizeof(msg));
	strcat(msg, "Name:\n");
	write(soc_conn, msg, sizeof(msg));
	write(soc_conn, "input", sizeof("input"));	//Ask for input from client
	bzero(msg, sizeof(msg));
	read(soc_conn, msg, sizeof(msg));
	strcpy(userName, msg);

	if(strcmp(userRole, "Seller") == 0){
		//Does user already exist as a seller:
		if (checkSellerExists(userName)){
			writeNoInput(soc_conn, "Your name was found in the database.\n");
			return;
		}

	} else { //Customer
		//Does user already exist as a custmer:
		if (checkCustomerExists(userName)){
			writeNoInput(soc_conn, "Your name was found in the database.\n");
			return;
		}
	}
	
	//Get userPhone
	bzero(msg, sizeof(msg));
	strcat(msg, "Phone XXX-XXX-XXXX:\n");
	write(soc_conn, msg, sizeof(msg));
	write(soc_conn, "input", sizeof("input")); //Ask for input from client
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

	if(userRole == "Buyer"){
		printf("save address");
		strcpy(customerInfo.name, userName);
		strcpy(customerInfo.phNumber, userPhone);
		strcpy(customerInfo.address, userAddress);

		addCustomerToTable(customerInfo, tableOfCustomers);
	} else { //Seller
		strcpy(sellerInfo.name, userName);
		strcpy(sellerInfo.phNumber, userPhone);
		strcpy(sellerInfo.address, userAddress);

		addSellerToTable(sellerInfo, tableOfSellers);
	}

	return;
}

void showSellerMenu(int soc_conn) {
	
	//This function shows the available options to sellers and then implements the functions for those options
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
		printf("%s", msg);
		switch(atoi(msg)) {
			case 1:
				updateSellerInformation(soc_conn);
				break;
			case 2:
				addProduct(soc_conn);
				break;
			case 3:
				writeNoInput(soc_conn, "Product ID of product to be removed:\n");
				write(soc_conn, "input", sizeof("input"));
				bzero(msg, sizeof(msg));
				read(soc_conn, msg, sizeof(msg));
				removeProduct(atoi(msg));
				break;
			case 4:
				writeNoInput(soc_conn, "Product ID of product to be updated quantity:\n");
				write(soc_conn, "input", sizeof("input"));
				bzero(msg, sizeof(msg));
				read(soc_conn, msg, sizeof(msg));
				int updateQuantityProductID = atoi(msg);
				writeNoInput(soc_conn, "Updated quantity available:\n");
				write(soc_conn, "input", sizeof("input"));
				bzero(msg, sizeof(msg));
				read(soc_conn, "input", sizeof("input"));
				int newQuantity = atoi(msg);
				printf("%d\n", newQuantity);
				updateProductQuantity(updateQuantityProductID, newQuantity);
				break;
			case 5:
				writeNoInput(soc_conn, "Product ID of product to be updated price:\n");
				write(soc_conn, "input", sizeof("input"));
				bzero(msg, sizeof(msg));
				read(soc_conn, msg, sizeof(msg));
				int updatePriceProductID = atoi(msg);
				writeNoInput(soc_conn, "Updated price:\n");
				write(soc_conn, "input", sizeof("input"));
				bzero(msg, sizeof(msg));
				read(soc_conn, msg, sizeof(msg));
				int newPrice = atoi(msg);
				printf("%d\n", newPrice);
				updateProductPrice(updatePriceProductID, newPrice);
				break;
			case 6:
				writeNoInput(soc_conn, "Please enter your Seller ID to view the products you offer:\n");
				write(soc_conn, "input", sizeof("input"));
				bzero(msg, sizeof(msg));
				read(soc_conn, msg, sizeof(msg));
				writeNoInput(soc_conn, "These are the products that you offer:\n");
				viewProductsForSeller(atoi(msg), soc_conn);
				break;
			case 7:
				writeNoInput(soc_conn, "Please enter the Product ID to view that product's orders:\n");
				write(soc_conn, "input", sizeof("input"));
				bzero(msg, sizeof(msg));
				read(soc_conn, msg, sizeof(msg));
				writeNoInput(soc_conn, "These are the orders for your products:\n");
				viewOrdersForProducts(atoi(msg), soc_conn);
				break;
			case 8:
				saveStructuresToFiles();
				write(soc_conn, "exit", sizeof("exit"));
				done = 1;
				break;
		}
	}
	return;
}

void writeNoInput(int soc_conn, char writeThis[1024]) {
	
	//This function writes a string to the client
	//It is called by many different functions to cut down on the number of lines of code
	
	char msg[1024];
	bzero(msg, sizeof(msg));
	strcat(msg, writeThis);
	write(soc_conn, msg, sizeof(msg));
	return;
}

void showBuyerMenu(int soc_conn) {
	
	//This function shows the available options to buyers and then implements the functions for those options
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
				saveStructuresToFiles();
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