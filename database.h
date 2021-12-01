/*
Author: Cole
Group G
cole.hutson@okstate.edu
*/

#ifndef DATABASE_H
#define DATABASE_H

// Database file name macros
#define FILE_TYPE   ".txt"
#define SELLERDB    "sellerInformation" FILE_TYPE
#define CUSTOMERDB  "customerInformation" FILE_TYPE
#define PRODUCTDB   "productInformation" FILE_TYPE
#define BILLINGDB   "billingInformation" FILE_TYPE
#define ORDERDB     "customerOrder" FILE_TYPE

// the maximum entries allowed per table
#define MAX_ENTRIES 500

#include <pthread.h>

extern pthread_mutex_t lockBillingTable;
extern pthread_mutex_t lockCustomerTable;
extern pthread_mutex_t lockOrderTable;
extern pthread_mutex_t lockProductTable;
extern pthread_mutex_t lockSellerTable;

// database data definitions
typedef struct sellerInformation 
{
    int sellerID; //primary key
    char name[50];
    char phNumber[15]; // needs to be in format XXX-XXX-XXXX
    char address[100];
} Seller;

typedef struct customerInformation
{
    int customerID; //primary key
    char name[50];
    char phNumber[15]; // needs to be in format XXX-XXX-XXXX
    char address[100];
} Customer;

typedef struct productInformation
{
    int productID; //primary key
    char description[300];
    int sellerID; //foreign key to seller database
    int numAvailable;
    double price;
} Product;

typedef struct billingInformation
{
    int orderID; // foreign key to order database
    int customerID; // foreign key to customer database
    char billingAddress[100]; 
    double orderPrice; // total price of the entire order
} BillingInfo;

typedef struct customerOrder
{
    int orderID;
    int productID;
    int numPurchased;
    char deliveryAddress[100]; // the customers address
    double totalPrice;

} Order;

//table structs
typedef struct sellerTable 
{
    Seller * entries;
    int count;
} SellerTable;

typedef struct customerTable 
{
    Customer * entries;
    int count;
} CustomerTable;

typedef struct productTable 
{
    Product * entries;
    int count;
} ProductTable;

typedef struct billingTable 
{
    BillingInfo * entries;
    int count;
} BillingTable;

typedef struct orderTable
{
    Order * entries;
    int count;
} OrderTable;

//functions stubs for use elsewhere
SellerTable * initSellers();
void addSellerToTable(Seller, SellerTable*);
int loadSellers(SellerTable*);
int saveSellers(SellerTable);
Seller getSeller(int, SellerTable*);

CustomerTable * initCustomers();
void addCustomerToTable(Customer, CustomerTable*);
int loadCustomers(CustomerTable*);
int saveCustomers(CustomerTable);

ProductTable * initProducts();
void addProductToTable(Product, ProductTable*);
int loadProducts(ProductTable*);
int saveProducts(ProductTable);

BillingTable * initBillings();
void addBillingToTable(BillingInfo, BillingTable*);
int loadBillings(BillingTable*);
int saveBillings(BillingTable);

OrderTable * initOrders();
void addOrderToTable(Order, OrderTable*);
int loadOrders(OrderTable*);
int saveOrders(OrderTable);

#endif