#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "database.h"

//initializes sellertable dynamically and returns a pointer to the table
//Returns - The newly allocated Table
SellerTable * initSellers()
{
    SellerTable * table = malloc(sizeof(SellerTable));
    table->count = 0;
    table->entries = malloc(sizeof(Seller) * MAX_ENTRIES);
}

// adds a new seller (toAdd) to the table and increases the tables count
// arguments:
// toAdd - the entry that you would like to insert into the table
// table - the table to add the entry into
void addSellerToTable(Seller toAdd, SellerTable * table)
{
    pthread_mutex_lock(&lockSellerTable);
    toAdd.sellerID = table->count++;
    table->entries[table->count - 1] = toAdd;
    pthread_mutex_unlock(&lockSellerTable);
}

//loads all sellers as text from file sellerinforamtion.txt and adds them to the table as a structure
//arguments:
//table - the (perviously allocated) table to load the data into
//returns - a flag value of wether the file exsists or not
int loadSellers(SellerTable * table)
{
    FILE * sellerFile = fopen(SELLERDB, "r"); // open the file in read mode
    if(sellerFile != NULL)
    {
        char entry[500];
        while(fgets(entry, 500, sellerFile))
        {
            char * token;
            Seller newSeller;

            token = strtok(entry, ",");

            token = strtok(NULL, ",");
            strcpy(newSeller.name, token);
            token = strtok(NULL, ",");
            strcpy(newSeller.phNumber, token);
            token = strtok(NULL, ",");
            token[strlen(token) - 1] = '\0';
            strcpy(newSeller.address, token);

            addSellerToTable(newSeller, table);
        }
    }
    else
    {
        return -1;
    }

    fclose(sellerFile);
    return 0;
}

//saves all sellers from the table to the file sellerinformation.txt
// arguments:
// table - the table of values that will be written to values
int saveSellers(SellerTable table)
{
    FILE *sellerFile = fopen(SELLERDB, "w"); // open the file in write mode

    pthread_mutex_lock(&lockSellerTable);
    for (int i = 0; i < table.count; i++)
    {
        if (table.entries[i].sellerID < 0)
            break; //there should not be an id with a value of 0 or less

        char entry[500] = "";
        sprintf(entry, "%i,%s,%s,%s\n",
                table.entries[i].sellerID,
                table.entries[i].name,
                table.entries[i].phNumber,
                table.entries[i].address);

        fputs(entry, sellerFile);
    }
    pthread_mutex_unlock(&lockSellerTable);

    fclose(sellerFile);
    return 0;
}


//initializes customertable dynamically and returns a pointer to the table
//Returns - The newly allocated Table
CustomerTable * initCustomers()
{
    CustomerTable * table = malloc(sizeof(table));
    table->count = 0;
    table->entries = malloc(sizeof(Customer) * MAX_ENTRIES);
}

// adds a new customer (toAdd) to the table and increases the tables count
// arguments:
// toAdd - the entry that you would like to insert into the table
// table - the table to add the entry into
void addCustomerToTable(Customer toAdd, CustomerTable * table)
{
    pthread_mutex_lock(&lockCustomerTable);
    toAdd.customerID = table->count++;
    table->entries[table->count - 1] = toAdd;
    pthread_mutex_unlock(&lockCustomerTable);
}

//loads all customers as text from file customerinformation.txt and adds them to the table as a structure
//arguments:
//table - the (perviously allocated) table to load the data into
//returns - a flag value of wether the file exsists or not
int loadCustomers(CustomerTable * table)
{
    FILE * customerFile = fopen(CUSTOMERDB, "r"); // open the file in read mode
    if(customerFile != NULL)
    {
        char entry[500];
        while(fgets(entry, 500, customerFile))
        {
            char * token;
            Customer newCustomer;

            token = strtok(entry, ",");

            token = strtok(NULL, ",");
            strcpy(newCustomer.name, token);
            token = strtok(NULL, ",");
            strcpy(newCustomer.phNumber, token);
            token = strtok(NULL, ",");
            strcpy(newCustomer.address, token);

            addCustomerToTable(newCustomer, table);
        }
    }
    else
    {
        return -1;
    }
    fclose(customerFile);
    return 0;
}

//saves all customers from the table to the file customerinformation.txt
// arguments:
// table - the table of values that will be written to values
int saveCustomers(CustomerTable table)
{
    FILE * customerFile = fopen(CUSTOMERDB, "w"); // open the file in write mode
    
    pthread_mutex_lock(&lockCustomerTable);
    for(int i = 0; i < table.count; i++)
    {
        if(table.entries[i].customerID < 0) break; //there should not be an id with a value of 0 or less

        char entry[500] = "";
        sprintf(entry, "%i,%s,%s,%s\n",
            table.entries[i].customerID,
            table.entries[i].name,
            table.entries[i].phNumber,
            table.entries[i].address
        );

        fputs(entry, customerFile);
    }
    pthread_mutex_unlock(&lockCustomerTable);

    fclose(customerFile);
    return 0;
}

//initializes producttable dynamically and returns a pointer to the table
//Returns - The newly allocated Table
ProductTable * initProducts()
{
    ProductTable * table = malloc(sizeof(Product));
    table->count = 0;
    table->entries = malloc(sizeof(Product) * MAX_ENTRIES);
}

// adds a new product (toAdd) to the table and increases the tables count
// arguments:
// toAdd - the entry that you would like to insert into the table
// table - the table to add the entry into
void addProductToTable(Product toAdd, ProductTable * table)
{
    pthread_mutex_lock(&lockProductTable);
    toAdd.productID = table->count++;
    table->entries[table->count - 1] = toAdd;
    pthread_mutex_unlock(&lockProductTable);
}

//loads all products as text from file productinformation.txt and adds them to the table as a structure
//arguments:
//table - the (perviously allocated) table to load the data into
//returns - a flag value of wether the file exsists or not
int loadProducts(ProductTable * table)
{
    FILE * productFile = fopen(PRODUCTDB, "r"); // open the file in read mode
    if(productFile != NULL)
    {
        char entry[500];
        while(fgets(entry, 500, productFile))
        {
            char * token;
            Product newProduct;

            token = strtok(entry, ",");

            token = strtok(NULL, ",");
            strcpy(newProduct.description, token);
            token = strtok(NULL, ",");
            newProduct.sellerID = atoi(token);
            token = strtok(NULL, ",");
            newProduct.numAvailable = atoi(token);
            token = strtok(NULL, ",");
            newProduct.price = atof(token);

            addProductToTable(newProduct, table);
        }
    }
    else
    {
        return -1;
    }
    fclose(productFile);
    return 0;
}

//saves all products from the table to the file productinformation.txt
// arguments:
// table - the table of values that will be written to values
int saveProducts(ProductTable table)
{
    FILE * productFile = fopen(PRODUCTDB, "w"); // open the file in write mode
    
    pthread_mutex_lock(&lockProductTable);
    for(int i = 0; i < table.count; i++)
    {
        if(table.entries[i].productID < 0) break; //there should not be an id with a value of 0 or less

        char entry[1000] = "";
        sprintf(entry, "%i,%s,%i,%i,%f\n",
            table.entries[i].productID,
            table.entries[i].description,
            table.entries[i].sellerID,
            table.entries[i].numAvailable,
            table.entries[i].price
        );

        fputs(entry, productFile);
    }
    pthread_mutex_unlock(&lockProductTable);

    fclose(productFile);
    return 0;
}

//initializes billingtable dynamically and returns a pointer to the table
//Returns - The newly allocated Table
BillingTable * initBillings()
{
    BillingTable * table = malloc(sizeof(BillingTable));
    table->count = 0;
    table->entries = malloc(sizeof(BillingInfo) * MAX_ENTRIES);
}

// adds a new Billing (toAdd) to the table and increases the tables count
// arguments:
// toAdd - the entry that you would like to insert into the table
// table - the table to add the entry into
void addBillingToTable(BillingInfo toAdd, BillingTable * table)
{
    pthread_mutex_lock(&lockBillingTable);
    table->count++;
    table->entries[table->count - 1] = toAdd;
    pthread_mutex_unlock(&lockBillingTable);
}

//loads all billings as text from file billingsinformation.txt and adds them to the table as a structure
//arguments:
//table - the (perviously allocated) table to load the data into
//returns - a flag value of wether the file exsists or not
int loadBillings(BillingTable * table)
{
    FILE * billingFile = fopen(BILLINGDB, "r"); // open the file in read mode
    if(billingFile != NULL)
    {
        char entry[500];
        while(fgets(entry, 500, billingFile))
        {
            char * token;
            BillingInfo newBilling;

            token = strtok(entry, ",");

            token = strtok(NULL, ",");
            newBilling.orderID = atoi(token);
            token = strtok(NULL, ",");
            newBilling.customerID = atoi(token);
            token = strtok(NULL, ",");
            strcpy(newBilling.billingAddress, token);
            token = strtok(NULL, ",");
            newBilling.orderPrice = atof(token);

            addBillingToTable(newBilling, table);
        }
    }
    else
    {
        return -1;
    }
    fclose(billingFile);
    return 0;
}

//saves all billings from the table to the file billinginformation.txt
// arguments:
// table - the table of values that will be written to values
int saveBillings(BillingTable table)
{
    FILE * billingFile = fopen(BILLINGDB, "w"); // open the file in write mode
    
    pthread_mutex_lock(&lockBillingTable);
    for(int i = 0; i < table.count; i++)
    {
        if(table.entries[i].orderID < 0) break;

        char entry[500] = "";
        sprintf(entry, "%i,%i,%s,%f\n",
            table.entries[i].orderID,
            table.entries[i].customerID,
            table.entries[i].billingAddress,
            table.entries[i].orderPrice
        );

        fputs(entry, billingFile);
    }
    pthread_mutex_unlock(&lockBillingTable);

    fclose(billingFile);
    return 0;
}

//initializes ordertable dynamically and returns a pointer to the table
//Returns - The newly allocated Table
OrderTable * initOrders()
{
    OrderTable * table = malloc(sizeof(OrderTable));
    table->count = 0;
    table->entries = malloc(sizeof(Order) * MAX_ENTRIES);
}

// adds a new order (toAdd) to the table and increases the tables count
// arguments:
// toAdd - the entry that you would like to insert into the table
// table - the table to add the entry into
void addOrderToTable(Order toAdd, OrderTable * table)
{
    pthread_mutex_lock(&lockOrderTable);
    toAdd.orderID = table->count++;
    table->entries[table->count - 1] = toAdd;
    pthread_mutex_unlock(&lockOrderTable);
}


//loads all orders as text from file orderinformation.txt and adds them to the table as a structure
//arguments:
//table - the (perviously allocated) table to load the data into
//returns - a flag value of wether the file exsists or not
int loadOrders(OrderTable * table)
{
    FILE * orderFile = fopen(ORDERDB, "r"); // open the file in read mode
    if(orderFile != NULL)
    {
        char entry[500];
        while(fgets(entry, 500, orderFile))
        {
            char * token;
            Order newOrder;

            token = strtok(entry, ",");

            token = strtok(NULL, ",");
            newOrder.productID = atoi(token);
            token = strtok(NULL, ",");
            newOrder.numPurchased = atoi(token);
            token = strtok(NULL, ",");
            strcpy(newOrder.deliveryAddress, token);
            token = strtok(NULL, ",");
            newOrder.totalPrice = atof(token);

            addOrderToTable(newOrder, table);
        }
    }
    else
    {
        return -1;
    }
    fclose(orderFile);
    return 0;
}

//saves all orders from the table to the file orderinformation.txt
// arguments:
// table - the table of values that will be written to values
int saveOrders(OrderTable table)
{
    FILE * orderFile = fopen(ORDERDB, "w"); // open the file in write mode
    
    pthread_mutex_lock(&lockOrderTable);
    for(int i = 0; i < table.count; i++)
    {
        char entry[1000] = "";
        sprintf(entry, "%i,%d,%d,%s,%f\n",
            table.entries[i].orderID,
            table.entries[i].productID,
            table.entries[i].numPurchased,
            table.entries[i].deliveryAddress,
            table.entries[i].totalPrice
        );

        fputs(entry, orderFile);
    }
    pthread_mutex_unlock(&lockOrderTable);

    fclose(orderFile);
    return 0;
}


