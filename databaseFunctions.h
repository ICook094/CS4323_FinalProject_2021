#ifndef DATAFUNC_H
#define DATAFUNC_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "unistd.h"
#include "database.h"

BillingTable * tableOfBillings;
CustomerTable * tableOfCustomers;
OrderTable * tableOfOrders;
ProductTable * tableOfProducts;
SellerTable * tableOfSellers;
Seller sellerInfo;
Customer customerInfo;

void startupStructures(){
    //initialize structures that hold all the information
    tableOfBillings = initBillings();
    tableOfCustomers = initCustomers();
    tableOfOrders = initOrders();
    tableOfProducts = initProducts();
    tableOfSellers = initSellers();
    

    loadBillings(tableOfBillings);
    loadCustomers(tableOfCustomers);
    loadOrders(tableOfOrders);
    loadProducts(tableOfProducts);
    loadSellers(tableOfSellers);

}

//i think that is how pointers work
void saveStructuresToFiles(){
    saveBillings(*tableOfBillings);
    saveCustomers(*tableOfCustomers);
    saveOrders(*tableOfOrders);
    saveProducts(*tableOfProducts);
    saveSellers(*tableOfSellers);
}

int checkSellerExists(char name[]){
    int count = tableOfSellers->count;

    for (int i = 0; i < count; i++){
        if (tableOfSellers->entries[i].name == name){
            sellerInfo = tableOfSellers->entries[i];
            return 1;
        }
    }
    return 0;
}

int checkCustomerExists(char name[]){
    int count = tableOfCustomers->count;

    for (int i = 0; i < count; i++){
        if (tableOfCustomers->entries[i].name == name){
            customerInfo = tableOfCustomers->entries[i];
            return 1;
        }
    }
    return 0;
}

//purchase a product option 2 in showBuyerMenu
void newOrder(int productID, int quantity){
    Order newOrder;
    newOrder.productID = productID;
    newOrder.numPurchased = quantity;
    strcpy(newOrder.deliveryAddress, customerInfo.address);

    Product * product = &(tableOfProducts->entries[productID]);
    product->numAvailable -= quantity;

    newOrder.totalPrice = quantity * product->price;
    
    addOrderToTable(newOrder, tableOfOrders);

    BillingInfo newBill;
    newBill.customerID = customerInfo.customerID;
    strcpy(newBill.billingAddress, customerInfo.address);
    newBill.orderID = tableOfOrders->count - 1;
    newBill.orderPrice = newOrder.totalPrice;

    addBillingToTable(newBill, tableOfBillings);
}


//return a product option 3 in showBuyerMenu
//delete order and billing struct
//return products to availability
void returnOrder(int orderID){
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

}

void addProduct(int soc_conn){
    Product newProduct;

    char msg[1024];
    bzero(msg, sizeof(msg));
    //ask client for a description of product
    strcat(msg, "\nEnter a description of the product:\n");
    write(soc_conn, msg, sizeof(msg));
    //ask for user input from the client
    write(soc_conn, "input", sizeof("input"));
    bzero(msg, sizeof(msg));
    read(soc_conn, msg, sizeof(msg));
    strcpy(newProduct.description, msg);

    //ask client for number of product available
    bzero(msg,sizeof(msg));
    strcat(msg, "\nEnter the quantity of the product that is available:\n");
    write(soc_conn, msg, sizeof(msg));
    //ask for user input from the client
    write(soc_conn, "input", sizeof("input"));
    bzero(msg, sizeof(msg));
    read(soc_conn, msg, sizeof(msg));
    //char to int
    int numAvailable = atoi(msg);
    newProduct.numAvailable = numAvailable;
    
    //enter price
    bzero(msg, sizeof(msg));
    strcat(msg, "\nEnter the price of the product:\n");
    write(soc_conn, msg, sizeof(msg));
    //ask user for input from the client
    write(soc_conn, "input", sizeof("input"));
    bzero(msg, sizeof(msg));
    read(soc_conn, msg, sizeof(msg));
    double price = atof(msg);
    newProduct.price = price;

    //assign the sellerID
    newProduct.sellerID = sellerInfo.sellerID;

    addProductToTable(newProduct, tableOfProducts);
}



void removeProduct(int productID){
    int count = tableOfProducts->count;
    for(int i = 0; i < count; i++){
        if (tableOfProducts->entries[i].productID == productID){
            //entries may need to be an array
            for (int j = i; j < count; j++){
                tableOfProducts->entries[i] = tableOfProducts->entries[i+1];
                break;
            }
        }

    }
    tableOfProducts->count--;
}

void updateProductQuantity(int productID, int quantity){
    int count = tableOfProducts->count;
    for (int i = 0; i < count; i++){
        if (tableOfProducts->entries[i].productID == productID){
            tableOfProducts->entries[i].numAvailable = quantity;
            break;
        }
    }
}

void updateProductPrice(int productID, int newPrice){
    int count = tableOfProducts->count;
    for (int i = 0; i < count; i++){
        if(tableOfProducts->entries[i].productID == productID){
            tableOfProducts->entries[i].price = newPrice;
            break;
        }
    }
}

//TODO
//replace case one in showSellerMenu() maybe make global seller struct
void updateSellerInformation(int soc_conn){
    char msg[250];
    strcpy(msg, "What information would you like to update?\n");
    strcat(msg, "\t1. Name\n\t2. Address\n\t3. Phone Number\n> ");
    write(soc_conn, msg, sizeof(msg));
    bzero(msg, sizeof(msg));
    
    write(soc_conn, "input", 5);

    char token[50];
    bzero(token, sizeof(token));
    read(soc_conn, token, sizeof(token));

    if(token[0] == '1')
    {
        strcpy(msg, "What would you like to change it to?\n> ");
        write(soc_conn, msg, sizeof(msg));
        
        write(soc_conn, "input", 5);

        bzero(token, sizeof(token));
        read(soc_conn, token, sizeof(token));

        bzero(sellerInfo.name, sizeof(sellerInfo.name));
        strcpy(sellerInfo.name, token);

        bzero(msg, sizeof(msg));
        strcpy(msg, "Information Updated\n");                
        write(soc_conn, msg, sizeof(msg));

    }
    if(token[0] == '2')
    {
        strcpy(msg, "What would you like to change it to?\n> ");
        write(soc_conn, msg, sizeof(msg));
        
        write(soc_conn, "input", 5);

        bzero(token, sizeof(token));
        read(soc_conn, token, sizeof(token));

        bzero(sellerInfo.address, sizeof(sellerInfo.address));
        strcpy(sellerInfo.address, token);

        bzero(msg, sizeof(msg));
        strcpy(msg, "Information Updated\n");                
        write(soc_conn, msg, sizeof(msg));
    }
    if(token[0] == '3')
    {
        strcpy(msg, "What would you like to change it to?\n> ");
        write(soc_conn, msg, sizeof(msg));
        
        write(soc_conn, "input", 5);

        bzero(token, sizeof(token));
        read(soc_conn, token, sizeof(token));

        bzero(sellerInfo.phNumber, sizeof(sellerInfo.phNumber));
        strcpy(sellerInfo.phNumber, token);

        bzero(msg, sizeof(msg));
        strcpy(msg, "Information Updated\n");                
        write(soc_conn, msg, sizeof(msg));
    }
    else
    {
        printf("Invalid Input. Try Again.\n");
    }
}

//TODO
//replace case one in showBuyerMenu() maybe make global buyer struct
void updateCustomerInformation(int soc_conn){
    char msg[250];
    strcpy(msg, "What information would you like to update?\n");
    strcat(msg, "\t1. Name\n\t2. Address\n\t3. Phone Number\n> ");
    write(soc_conn, msg, sizeof(msg));
    bzero(msg, sizeof(msg));
    
    write(soc_conn, "input", 5);

    char token[50];
    bzero(token, sizeof(token));
    read(soc_conn, token, sizeof(token));

    if(token[0] == '1')
    {
        strcpy(msg, "What would you like to change it to?\n> ");
        write(soc_conn, msg, sizeof(msg));
        
        write(soc_conn, "input", 5);

        bzero(token, sizeof(token));
        read(soc_conn, token, sizeof(token));

        bzero(customerInfo.name, sizeof(customerInfo.name));
        strcpy(customerInfo.name, token);

        bzero(msg, sizeof(msg));
        strcpy(msg, "Information Updated\n");                
        write(soc_conn, msg, sizeof(msg));

    }
    if(token[0] == '2')
    {
        strcpy(msg, "What would you like to change it to?\n> ");
        write(soc_conn, msg, sizeof(msg));
        
        write(soc_conn, "input", 5);

        bzero(token, sizeof(token));
        read(soc_conn, token, sizeof(token));

        bzero(customerInfo.address, sizeof(customerInfo.address));
        strcpy(customerInfo.address, token);

        bzero(msg, sizeof(msg));
        strcpy(msg, "Information Updated\n");                
        write(soc_conn, msg, sizeof(msg));
    }
    if(token[0] == '3')
    {
        strcpy(msg, "What would you like to change it to?\n> ");
        write(soc_conn, msg, sizeof(msg));
        
        write(soc_conn, "input", 5);

        bzero(token, sizeof(token));
        read(soc_conn, token, sizeof(token));

        bzero(customerInfo.phNumber, sizeof(customerInfo.phNumber));
        strcpy(customerInfo.phNumber, token);

        bzero(msg, sizeof(msg));
        strcpy(msg, "Information Updated\n");                
        write(soc_conn, msg, sizeof(msg));
    }
    else
    {
        printf("Invalid Input. Try Again.\n");
    }
}

//create local order for said buyer and pass that into the function.
//option 5 showBuyerMenu
void viewOrdersAsCustomer(char CustomerAddress[], int soc_conn){
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
}

//option 6 showBuyerMenu
void viewBillingInfo(int customerID, int soc_conn){
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
}

void viewProductsAvailable(){
    //print out all available products and their quantity and price
    for(int i = 0; i < tableOfProducts->count; i++)
    {
        Product product = tableOfProducts->entries[i];
        printf("Product ID: %d\nQuantity Available: %d\nPrice: %f\n\nDescription: %s",
                product.productID,
                product.numAvailable,
                product.price,
                product.description
                );
    }
}

void viewProductsForSeller(int sellerID, int soc_conn){
    //for all products if sellerID matches then print out
    for(int i = 0; i < tableOfProducts->count; i++)
    {
        if(tableOfProducts->entries[i].sellerID == sellerID) // check to see if the seller with selleID listed this product
        {
            Product product = tableOfProducts->entries[i]; // get product information

            char msg[500];
            sprintf(msg, "Product ID: %d\nQuantity Available: %d\nPrice: %f\n\nDescription: %s",
                product.productID,
                product.numAvailable,
                product.price,
                product.description
                );

            write(soc_conn, msg, sizeof(msg));
        }
    }
}

void viewOrdersForProducts(int productID, int soc_conn)
{
    //for all products if sellerID matches then print out
    for(int i = 0; i < tableOfOrders->count; i++)
    {
        if(tableOfOrders->entries[i].productID == productID)
        {
            Order order = tableOfOrders->entries[i]; // get product with billing information
            
            char msg[500];
            printf(msg, "Order ID: %d\nQuantity Purchased: %d\nTotal Order Price: %f\n\nDelivery Address: %s",
                order.orderID,
                order.numPurchased,
                order.totalPrice,
                order.deliveryAddress
                );
            
            write(soc_conn, msg, sizeof(msg));
        }
    }
}

#endif