//Written by: Scott Schwager
//sschwag@okstate.edu
//Group G

#include <stdio.h>

void buyerOrSeller() {
	
	//This function determines the role of each user
	//Then it calls getUserInfo() with the role as an argument
	//Then it calls the function to show the menu for the user (currently only showSellerMenu(), will add showBuyerMenu() later)
	//This function will be called from the server after a client is accepted
	
	//will need to change printf to write or send
	//will need to change scanf to read or recv
	//signal to client that server expects input or assume that client will provide write after every time it reads?
	//separate funtion to updateUserInfo or reuse getUserInfo?
	
	int option;
	
	printf("Please choose your role:\n1. Buyer\n2. Seller\n");
	scanf("%d", &option);
	
	if (option == 1) {
		printf("Buyer\n");
		getUserInfo("Buyer");
	} else {
		printf("Seller\n");
		getUserInfo("Seller");
		showSellerMenu();
	}
}

void getUserInfo(char userRole[6]) {
	
	//This function gets the user information
	//arguments: userRole (either "Buyer" or "Seller")
	//The user role determines whether the information is stored in sellerInfo table or customerInfo table
	//The information that is gathered is the same regardless of the user's role
	//This function is called by buyerOrSeller()
	
	char userId[50];
	char userName[50];
	char userPhone[12];
	char userAddress[50];
	
	printf("ID:\n");
	scanf("%s", &userId);
	printf("Name:\n");
	scanf("%s", &userName);
	printf("Phone XXX-XXX-XXXX:\n");
	scanf("%s", &userPhone);
	printf("Address:\n");
	scanf("%s", &userAddress);
	
	//Likely will put this info into a struct of some sort
	//This information is then entered into the sellerInformation or customerInformation database depending on the userRole (Buyer or Seller)
}

void showSellerMenu() {
	
	//This function shows the available options to sellers and then calls the functions for those options
	//This function is called by buyerOrSeller()
	
	//Likely will loop this menu so a user can do multiple actions consecutively
	
	int option;
	
	printf("\nWhat to you want to do?\n");
	printf("1. Update User Information\n");
	printf("2. Add a new product\n");
	printf("3. Remove an existing product\n");
	printf("4. Update the quantity of a product\n");
	printf("5. Update the price of a product\n");
	printf("6. View available products\n");
	printf("7. View orders for your products\n");
	
	scanf("%d", &option);
	switch(option) {
		case 1:
			break;
		case 2:
			printf("Please enter product information:\n");
			break;
		case 3:
			break;
	}
}


int main() {
	//This is the driver function for testing purposes.
	//This code will be put into the server code, to be ran after the server accepts a client.
	buyerOrSeller();
	return 0;
}
