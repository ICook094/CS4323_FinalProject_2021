#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

void writeNoInput(int soc_conn, char writeThis[1024]);

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