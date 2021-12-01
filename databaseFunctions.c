/**
 * Group G
 * Isabell Cook
 * icook@okstate.edu
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "unistd.h"

#include "database.h"
#include "databaseFunctions.h"

void writeNoInput(int soc_conn, char writeThis[1024]);

BillingTable * tableOfBillings;
CustomerTable * tableOfCustomers;
OrderTable * tableOfOrders;
ProductTable * tableOfProducts;
SellerTable * tableOfSellers;
Seller sellerInfo;
Customer customerInfo;

pthread_mutex_t lockBillingTable;
pthread_mutex_t lockCustomerTable;
pthread_mutex_t lockOrderTable;
pthread_mutex_t lockProductTable;
pthread_mutex_t lockSellerTable;

/**
 * Argument: NULL
 * Return: void
 * Description: 
 *  This function calls all of the functions needed to start up the "database".
 *  Table structures defined globaly are initialized then the contents stored in the files
 *  are loaded into the corresponding structures
 */
//Does not need Mutex Locks
void startupStructures(){
    //initialize structures that hold all the information
    tableOfBillings = initBillings();
    tableOfCustomers = initCustomers();
    tableOfOrders = initOrders();
    tableOfProducts = initProducts();
    tableOfSellers = initSellers();
    
    //Load information from each text file
    loadBillings(tableOfBillings);
    loadCustomers(tableOfCustomers);
    loadOrders(tableOfOrders);
    loadProducts(tableOfProducts);
    loadSellers(tableOfSellers);

}

/**
 * Argument: NULL
 * Return: void
 * Description: 
 *  This function calls the necessary methods in order to save all of the global
 *  structs that make up the database into their respective text files.
 */
void saveStructuresToFiles(){
    saveBillings(*tableOfBillings);
    saveCustomers(*tableOfCustomers);
    saveOrders(*tableOfOrders);
    saveProducts(*tableOfProducts);
    saveSellers(*tableOfSellers);
}

/**
 * Argument: char []
 * Return: int
 * Description: 
 *  This function takes in the name of a seller and checks to see if the name
 *  already exists in the database.
 */
int checkSellerExists(char name[]){

    pthread_mutex_lock(&lockSellerTable);

    int count = tableOfSellers->count;
    //for each entry in the table compare the sellers name and the name given
    for (int i = 0; i < count; i++){
        if (strcmp(tableOfSellers->entries[i].name, name) == 0){
            sellerInfo = tableOfSellers->entries[i];
            return 1;
        }
    }
    
    pthread_mutex_unlock(&lockSellerTable);
    return 0;
}

/**
 * Argument: char []
 * Return: int
 * Description: 
 *  This function takes in the name of a customer and checks to see if the name
 *  already exists in the database.
 */
int checkCustomerExists(char name[]){

    pthread_mutex_lock(&lockCustomerTable);

    int count = tableOfCustomers->count;
    //for each entry in the table compare the customers name and the name given
    for (int i = 0; i < count; i++){
        if (strcmp(tableOfCustomers->entries[i].name, name) == 0){
            customerInfo = tableOfCustomers->entries[i];
            return 1;
        }
    }
    
    pthread_mutex_unlock(&lockCustomerTable);
    return 0;
}

/**
 * Argument: int, int
 * Return: void
 * Description: 
 *  This function is for option 2 in showBuyerMenu.
 *  When a customer makes a purchase, a new order and bill is created,
 *  and the amount of product bought is removed from the database.
 */
void newOrder(int productID, int quantity){
    pthread_mutex_lock(&lockProductTable);
    pthread_mutex_lock(&lockOrderTable);

    //create order
    Order newOrder;
    newOrder.productID = productID;
    newOrder.numPurchased = quantity;
    strcpy(newOrder.deliveryAddress, customerInfo.address);

    //find the product and remove the number bought from the products database
    
    Product product = tableOfProducts->entries[productID];
    product.numAvailable -= quantity;
    tableOfProducts->entries[productID].numAvailable = product.numAvailable;
    

    //find the total price
    newOrder.totalPrice = quantity * product.price;
    
    //add order to the database
    addOrderToTable(newOrder, tableOfOrders);

    //create a new bill
    BillingInfo newBill;
    newBill.customerID = customerInfo.customerID;
    strcpy(newBill.billingAddress, customerInfo.address);
    
    newBill.orderID = tableOfOrders->count - 1;

    newBill.orderPrice = newOrder.totalPrice;

    pthread_mutex_unlock(&lockOrderTable);
    pthread_mutex_unlock(&lockProductTable);
 

    //add order to the bill.
    addBillingToTable(newBill, tableOfBillings);
}

/**
 * Argument: int
 * Return: void
 * Description: 
 *  This function is for option 3 in showBuyerMenu.
 *  When a customer returns a product, the order and bill associated with
 *  that purchase is removed, and the amount of product being returned is
 *  added back to the products database.
 */
void returnOrder(int orderID){
    pthread_mutex_lock(&lockBillingTable);
    pthread_mutex_lock(&lockOrderTable);
    pthread_mutex_lock(&lockProductTable);

    sleep(10);

    //Find order in database.
 
    Order orderToReturn;
    int orderCount = tableOfOrders->count;
    for (int i = 0; i < orderCount; i++){
        if(tableOfOrders->entries[i].orderID == orderID){
            orderToReturn = tableOfOrders->entries[i];
            break;
        }
    }



    //return products purshased to products available
    int productCount = tableOfProducts->count;
    for (int i = 0; i < productCount; i++){
        if (tableOfProducts->entries[i].productID == orderToReturn.productID){
            tableOfProducts->entries[i].numAvailable = tableOfProducts->entries[i].numAvailable + orderToReturn.numPurchased;
            break;
        }
    }
    //delete order from orderTable
    for (int i = 0; i < orderCount; i++){
        if(tableOfOrders->entries[i].orderID == orderToReturn.orderID){
            for (int j = i; j < orderCount; j++){
                tableOfOrders->entries[i] = tableOfOrders->entries[i+1];
            }
            break;
        }
    }

    
    //delete bill correspondint to order from billingTable
    int billingCount = tableOfBillings->count;
    for (int i = 0; i < billingCount; i++){
        if (tableOfBillings->entries[i].orderID == orderToReturn.orderID){
            for (int j = i; j < billingCount; j++){
                tableOfBillings->entries[i] = tableOfBillings->entries[i+1];
            }
            break;
        }
    }
    pthread_mutex_unlock(&lockBillingTable);
    pthread_mutex_unlock(&lockProductTable);
    pthread_mutex_unlock(&lockOrderTable);
}

/**
 * Argument: int
 * Return: void
 * Description: 
 *  Adds new product from sellers into the databse structures
 */
void addProduct(int soc_conn){
    Product newProduct;

    char msg[1024];
	writeNoInput(soc_conn, "Enter a description of the product:\n");	//Using writeNoInput here to prevent buffer issue
    //ask for user input from the client
    write(soc_conn, "input", sizeof("input"));
    bzero(msg, sizeof(msg));
    read(soc_conn, msg, sizeof(msg));
    strcpy(newProduct.description, msg);

    //ask client for number of product available
	writeNoInput(soc_conn, "Enter the quantity of the product that is available:\n");	//Using writeNoInput here to prevent buffer issue
    //ask for user input from the client
    write(soc_conn, "input", sizeof("input"));
    bzero(msg, sizeof(msg));
    read(soc_conn, msg, sizeof(msg));
    //char to int
    int numAvailable = atoi(msg);
    newProduct.numAvailable = numAvailable;
    
    //enter price
	writeNoInput(soc_conn, "Enter the price of the product:\n");	//Using writeNoInput here to prevent buffer issue
    //ask user for input from the client
    write(soc_conn, "input", sizeof("input"));
    bzero(msg, sizeof(msg));
    read(soc_conn, msg, sizeof(msg));
    double price = atof(msg);
    newProduct.price = price;

    //assign the sellerID
    newProduct.sellerID = sellerInfo.sellerID;

    //add product to database
    addProductToTable(newProduct, tableOfProducts);
}

/**
 * Argument: int, int
 * Return: void
 * Description: 
 *  Removing a product from the database/from being available to sell
 *  by the seller. The product is found in the database and is removed
 */
void removeProduct(int productID){
    pthread_mutex_lock(&lockProductTable);

    int count = tableOfProducts->count;
    //find the product entry that matched the product ID given
    for(int i = 0; i < count; i++){
        if (tableOfProducts->entries[i].productID == productID){
            //move all products behind the product being removed up on in the array.
            for (int j = i; j < count; j++){
                tableOfProducts->entries[i] = tableOfProducts->entries[i+1];
                break;
            }
        }

    }
    //decrease count of database
    tableOfProducts->count--;

    pthread_mutex_unlock(&lockProductTable);
}

/**
 * Argument: int, int
 * Return: void
 * Description: 
 *  This is the function that changed the quantity the seller wants to sell of 
 *  a product.
 */
void updateProductQuantity(int productID, int quantity){
    pthread_mutex_lock(&lockProductTable);
    
    int count = tableOfProducts->count;
    //find product corresponding to given ID
    for (int i = 0; i < count; i++){
        if (tableOfProducts->entries[i].productID == productID){
            //change the quantity
            tableOfProducts->entries[i].numAvailable = quantity;
            break;
        }
    }
    pthread_mutex_unlock(&lockProductTable);
}

/**
 * Argument: int, int
 * Return: void
 * Description: 
 *  This function changes the price that a seller wants to sell their product at.
 */
void updateProductPrice(int productID, int newPrice){
    pthread_mutex_lock(&lockProductTable);

    int count = tableOfProducts->count;
    //find product matching the given ID
    for (int i = 0; i < count; i++){
        if(tableOfProducts->entries[i].productID == productID){
            //change the price of said product
            tableOfProducts->entries[i].price = newPrice;
            break;
        }
    }

    pthread_mutex_unlock(&lockProductTable);
}

/**
 * Argument: int
 * Return: void
 * Description: 
 *  This function allows the seller to change either their name, phone number, or address.
 *  Sellers are not allowed to change their ID because that corresponds to their place as an
 *  entry in the database
 */
void updateSellerInformation(int soc_conn){
    char msg[1024];
    strcpy(msg, "What information would you like to update?\n");
    strcat(msg, "\t1. Name\n\t2. Address\n\t3. Phone Number\n");
    write(soc_conn, msg, sizeof(msg));
    bzero(msg, sizeof(msg));
    
    write(soc_conn, "input", 5);
    char token[1024];
    bzero(token, sizeof(token));
    read(soc_conn, token, sizeof(token));

    if(strcmp(token, "1") == 0)
    {
		writeNoInput(soc_conn, "What would you like to change it to?\n");
        
        write(soc_conn, "input", sizeof("input"));
        bzero(token, sizeof(token));
        read(soc_conn, token, sizeof(token));

        bzero(sellerInfo.name, sizeof(sellerInfo.name));
        strcpy(sellerInfo.name, token);

		writeNoInput(soc_conn, "Information Updated\n");
    }
    else if(strcmp(token, "2") == 0)
    {
		writeNoInput(soc_conn, "What would you like to change it to?\n");
        
        write(soc_conn, "input", sizeof("input"));
        bzero(token, sizeof(token));
        read(soc_conn, token, sizeof(token));

        bzero(sellerInfo.address, sizeof(sellerInfo.address));
        strcpy(sellerInfo.address, token);

        writeNoInput(soc_conn, "Information Updated\n");
    }
    else if(token[0] == '3')
    {
		writeNoInput(soc_conn, "What would you like to change it to?\n");
        
        write(soc_conn, "input", sizeof("input"));
        bzero(token, sizeof(token));
        read(soc_conn, token, sizeof(token));

        bzero(sellerInfo.phNumber, sizeof(sellerInfo.phNumber));
        strcpy(sellerInfo.phNumber, token);

        writeNoInput(soc_conn, "Information Updated\n");
    }
    else
    {
        printf("Invalid Input. Try Again.\n");
    }

    //Send local seller information to database
    pthread_mutex_lock(&lockSellerTable);
    tableOfSellers->entries[sellerInfo.sellerID] = sellerInfo;
    pthread_mutex_unlock(&lockSellerTable);

	return;
}

/**
 * Argument: int
 * Return: void
 * Description: 
 *  This function allows the customer to change either their name, phone number, or address.
 *  Customers are not allowed to change their ID because that corresponds to their place as an
 *  entry in the database
 */
void updateCustomerInformation(int soc_conn){
	
	char msg[1024];
    strcpy(msg, "What information would you like to update?\n");
    strcat(msg, "\t1. Name\n\t2. Address\n\t3. Phone Number\n");
    write(soc_conn, msg, sizeof(msg));
    bzero(msg, sizeof(msg));
    
    write(soc_conn, "input", 5);
    char token[1024];
    bzero(token, sizeof(token));
    read(soc_conn, token, sizeof(token));

    if(strcmp(token, "1") == 0)
    {
		writeNoInput(soc_conn, "What would you like to change it to?\n");
        
        write(soc_conn, "input", sizeof("input"));
        bzero(token, sizeof(token));
        read(soc_conn, token, sizeof(token));

        bzero(customerInfo.name, sizeof(customerInfo.name));
        strcpy(customerInfo.name, token);

		writeNoInput(soc_conn, "Information Updated\n");
    }
    else if(strcmp(token, "2") == 0)
    {
		writeNoInput(soc_conn, "What would you like to change it to?\n");
        
        write(soc_conn, "input", sizeof("input"));
        bzero(token, sizeof(token));
        read(soc_conn, token, sizeof(token));

        bzero(customerInfo.address, sizeof(customerInfo.address));
        strcpy(customerInfo.address, token);

        writeNoInput(soc_conn, "Information Updated\n");
    }
    else if(token[0] == '3')
    {
		writeNoInput(soc_conn, "What would you like to change it to?\n");
        
        write(soc_conn, "input", sizeof("input"));
        bzero(token, sizeof(token));
        read(soc_conn, token, sizeof(token));

        bzero(customerInfo.phNumber, sizeof(customerInfo.phNumber));
        strcpy(customerInfo.phNumber, token);

        writeNoInput(soc_conn, "Information Updated\n");
    }
    else
    {
        printf("Invalid Input. Try Again.\n");
    }

    //Send local seller information to database
    pthread_mutex_lock(&lockCustomerTable);
    tableOfCustomers->entries[customerInfo.customerID] = customerInfo;
    pthread_mutex_unlock(&lockCustomerTable);

	return;
}

/**
 * Argument: char [], int
 * Return: void
 * Description: 
 *  This function allows customers to view all products that they have purchased. 
 */
void viewOrdersAsCustomer(char CustomerAddress[], int soc_conn){
    pthread_mutex_lock(&lockOrderTable);

    int count = tableOfOrders->count;
    for (int i; i < count; i++){
        if(tableOfOrders->entries[i].deliveryAddress == CustomerAddress){
            //print out order
            char msg[1024];
            bzero(msg, sizeof(msg));

            sprintf(msg, "\nOrderID: %d \nProductID: %d \nAmount of Product Purchased: %d \nDeliverAddress: %s \nTotal Price %f\n\n",
                tableOfOrders->entries[i].orderID, 
                tableOfOrders->entries[i].productID,
                tableOfOrders->entries[i].numPurchased,
                tableOfOrders->entries[i].deliveryAddress,
                tableOfOrders->entries[i].totalPrice
                );

            write(soc_conn, msg, sizeof(msg));
        }
    }

    pthread_mutex_unlock(&lockOrderTable);
}

/**
 * Argument: int, int
 * Return: void
 * Description: 
 *  Option 6 in showBuyerMenu
 *  This function allows customers to view their billing information
 */
void viewBillingInfo(int customerID, int soc_conn){
    pthread_mutex_lock(&lockBillingTable);
    int count = tableOfBillings->count;
    for (int i; i < count; i++){
        if(tableOfBillings->entries[i].customerID == customerID){
            //print out order
            char msg[1024];
            bzero(msg, sizeof(msg));

            sprintf(msg, "\nOrderID: %d \nProductID: %d \nBilling Adress: %s \nOrder Price %f\n\n",
                tableOfBillings->entries[i].orderID,
                tableOfBillings->entries[i].customerID,
                tableOfBillings->entries[i].billingAddress,
                tableOfBillings->entries[i].orderPrice
                );

            write(soc_conn, msg, sizeof(msg));
        }
    }
    pthread_mutex_unlock(&lockBillingTable);
}

/**
 * Argument: int
 * Return: void
 * Description: 
 *  This is where all products available for sell are sent to the client's terminal to be printed.
 */
void viewProductsAvailable(int soc_conn){
    pthread_mutex_lock(&lockProductTable);
    //print out all available products and their quantity and price
    for(int i = 0; i < tableOfProducts->count; i++)
    {
        Product product = tableOfProducts->entries[i];
		char msg[1024];
        sprintf(msg, "Product ID: %d\nQuantity Available: %d\nPrice: %f\nDescription: %s\n",
                product.productID,
                product.numAvailable,
                product.price,
                product.description
                );
			
			writeNoInput(soc_conn, msg);
    }

    pthread_mutex_unlock(&lockProductTable);
	return;
}

/**
 * Argument: int
 * Return: void
 * Description: 
 *  This allows sellers to see what products they have up for sell
 */
void viewProductsForSeller(int soc_conn){
    int sellerID = sellerInfo.sellerID;

    pthread_mutex_lock(&lockProductTable);
    //for all products if sellerID matches then print out
    for(int i = 0; i < tableOfProducts->count; i++)
    {
        if(tableOfProducts->entries[i].sellerID == sellerID) // check to see if the seller with selleID listed this product
        {
            Product product = tableOfProducts->entries[i]; // get product information

            char msg[500];
            sprintf(msg, "Product ID: %d\nQuantity Available: %d\nPrice: %f\nDescription: %s\n",
                product.productID,
                product.numAvailable,
                product.price,
                product.description
                );

            writeNoInput(soc_conn, msg);
        }
    }
    pthread_mutex_unlock(&lockProductTable);
	return;
}

/**
 * Argument: int, int
 * Return: void
 * Description: 
 *  This allows the seller to see how many orders they have for their products.
 */
void viewOrdersForProducts(int productID, int soc_conn)
{   
    pthread_mutex_lock(&lockOrderTable);
    
    //for all products if sellerID matches then print out
    for(int i = 0; i < tableOfOrders->count; i++)
    {
        if(tableOfOrders->entries[i].productID == productID)
        {
            Order order = tableOfOrders->entries[i]; // get product with billing information
            
            char msg[500];
            sprintf(msg, "Order ID: %d\nQuantity Purchased: %d\nTotal Order Price: %f\nDelivery Address: %s\n",
                order.orderID,
                order.numPurchased,
                order.totalPrice,
                order.deliveryAddress
                );
            
            writeNoInput(soc_conn, msg);
        }
    }

    pthread_mutex_unlock(&lockOrderTable);
}