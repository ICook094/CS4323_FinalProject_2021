#ifndef DATABASE_H
#define DATABASE_H

// Database file name macros
#define FILE_TYPE   ".txt"
#define SELLERDB    "sellerInformation" FILE_TYPE
#define CUSTOMERDB  "customerInformation" FILE_TYPE
#define PRODUCTDB   "productInformation" FILE_TYPE
#define BILLINGDB   "billingInformation" FILE_TYPE
#define ORDERDB     "customerOrder" FILE_TYPE

#define MAX_ENTRIES 500

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
    char description[500];
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
    int profuctID;
    int numPurchased;
    char deliveryAddress[100]; // the customers address
    double totalPrice;

} Order;

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

void addSeller(Seller, SellerTable*);
int loadSellers(SellerTable*);
int saveSellers(SellerTable);

void addCustomer(Customer, CustomerTable*);
int loadCustomers(CustomerTable*);
int saveCustomers(CustomerTable);

void addProduct(Product, ProductTable*);
int loadProducts(ProductTable*);
int saveProducts(ProductTable);

void addBilling(BillingInfo, BillingTable*);
int loadBillings(BillingTable*);
int saveBillings(BillingTable);

void addOrder(Order, OrderTable*);
int loadOrders(OrderTable*);
int saveOrders(OrderTable);

#endif