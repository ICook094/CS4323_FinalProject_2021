#ifndef DATAFUNC_H
#define DATAFUNC_H

#include "database.h"

BillingTable * tableOfBillings;
CustomerTable * tableOfCustomers;
OrderTable * tableOfOrders;
ProductTable * tableOfProducts;
SellerTable * tableOfSellers;
Seller sellerInfo;
Customer customerInfo;

void startupStructures();

//i think that is how pointers work
void saveStructuresToFiles();

int checkSellerExists(char name[]);

int checkCustomerExists(char name[]);

//purchase a product option 2 in showBuyerMenu
void newOrder(int productID, int quantity);


//return a product option 3 in showBuyerMenu
//delete order and billing struct
//return products to availability
void returnOrder(int orderID);

void addProduct(int soc_conn);

void removeProduct(int productID);

void updateProductQuantity(int productID, int quantity);

void updateProductPrice(int productID, int newPrice);

//TODO
//replace case one in showSellerMenu() maybe make global seller struct
void updateSellerInformation(int soc_conn);

//TODO
//replace case one in showBuyerMenu() maybe make global buyer struct
void updateCustomerInformation(int soc_conn);

//create local order for said buyer and pass that into the function.
//option 5 showBuyerMenu
void viewOrdersAsCustomer(char CustomerAddress[], int soc_conn);

//option 6 showBuyerMenu
void viewBillingInfo(int customerID, int soc_conn);

void viewProductsAvailable();

void viewProductsForSeller(int sellerID, int soc_conn);

void viewOrdersForProducts(int productID, int soc_conn);

#endif