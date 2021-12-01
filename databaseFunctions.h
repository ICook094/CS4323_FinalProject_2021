#ifndef DATAFUNC_H
#define DATAFUNC_H

#include "database.h"

//Global definition of structures that make up the database shared among all clients
extern BillingTable * tableOfBillings;
extern CustomerTable * tableOfCustomers;
extern OrderTable * tableOfOrders;
extern ProductTable * tableOfProducts;
extern SellerTable * tableOfSellers;
extern Seller sellerInfo;
extern Customer customerInfo;


//function stubs from databaseFunction.c
void startupStructures();
void saveStructuresToFiles();
int checkSellerExists(char name[]);
int checkCustomerExists(char name[]);
void newOrder(int productID, int quantity);
void returnOrder(int orderID);
void addProduct(int soc_conn);
void removeProduct(int productID);
void updateProductQuantity(int productID, int quantity);
void updateProductPrice(int productID, int newPrice);
void updateSellerInformation(int soc_conn);
void updateCustomerInformation(int soc_conn);
void viewOrdersAsCustomer(char CustomerAddress[], int soc_conn);
void viewBillingInfo(int customerID, int soc_conn);
void viewProductsAvailable();
void viewProductsForSeller(int soc_conn);
void viewOrdersForProducts(int productID, int soc_conn);

#endif