#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "database.h"

SellerTable * initSellers()
{
    SellerTable * table = malloc(sizeof(SellerTable));
    table->count = 0;
    table->entries = malloc(sizeof(Seller) * MAX_ENTRIES);
}

void addSeller(Seller toAdd, SellerTable * table)
{
    toAdd.sellerID = ++table->count;
    table->entries[table->count - 1] = toAdd;
}

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

            addSeller(newSeller, table);
        }
    }
    else
    {
        return -1;
    }

    return 0;
}

int saveSellers(SellerTable table)
{
    FILE *sellerFile = fopen(SELLERDB, "w"); // open the file in write mode

    for (int i = 0; i < table.count; i++)
    {
        if (table.entries[i].sellerID == 0)
            break; //there should not be an id with a value of 0 or less

        char entry[500] = "";
        sprintf(entry, "%i,%s,%s,%s\n",
                table.entries[i].sellerID,
                table.entries[i].name,
                table.entries[i].phNumber,
                table.entries[i].address);

        fputs(entry, sellerFile);
    }

    fclose(sellerFile);
    return 0;
}

CustomerTable * initCustomers()
{
    CustomerTable * table = malloc(sizeof(table));
    table->count = 0;
    table->entries = malloc(sizeof(Customer) * MAX_ENTRIES);
}

void addCustomer(Customer toAdd, CustomerTable * table)
{
    toAdd.customerID = ++table->count;
    table->entries[table->count - 1] = toAdd;
}

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

            addCustomer(newCustomer, table);
        }
    }
    else
    {
        return -1;
    }

    return 0;
}

int saveCustomers(CustomerTable table)
{
    FILE * customerFile = fopen(CUSTOMERDB, "w"); // open the file in write mode
    
    for(int i = 0; i < table.count; i++)
    {
        if(table.entries[i].customerID == 0) break; //there should not be an id with a value of 0 or less

        char entry[500] = "";
        sprintf(entry, "%i,%s,%s,%s\n",
            table.entries[i].customerID,
            table.entries[i].name,
            table.entries[i].phNumber,
            table.entries[i].address
        );

        fputs(entry, customerFile);
    }

    fclose(customerFile);
    return 0;
}

ProductTable * initProducts()
{
    ProductTable * table = malloc(sizeof(Product));
    table->count = 0;
    table->entries = malloc(sizeof(Product) * MAX_ENTRIES);
}

void addProduct(Product toAdd, ProductTable * table)
{
    toAdd.productID = ++table->count;
    table->entries[table->count - 1] = toAdd;
}

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

            addProduct(newProduct, table);
        }
    }
    else
    {
        return -1;
    }

    return 0;
}

int saveProducts(ProductTable table)
{
    FILE * productFile = fopen(PRODUCTDB, "w"); // open the file in write mode
    
    for(int i = 0; i < table.count; i++)
    {
        if(table.entries[i].productID == 0) break; //there should not be an id with a value of 0 or less

        char entry[500] = "";
        sprintf(entry, "%i,%s,%s,%s,%d\n",
            table.entries[i].productID,
            table.entries[i].description,
            table.entries[i].sellerID,
            table.entries[i].numAvailable,
            table.entries[i].price
        );

        fputs(entry, productFile);
    }

    fclose(productFile);
    return 0;
}

BillingTable * initBillings()
{
    BillingTable * table = malloc(sizeof(BillingTable));
    table->count = 0;
    table->entries = malloc(sizeof(BillingInfo) * MAX_ENTRIES);
}

void addBilling(BillingInfo toAdd, BillingTable * table)
{
    table->entries[table->count - 1] = toAdd;
}

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

            addBilling(newBilling, table);
        }
    }
    else
    {
        return -1;
    }

    return 0;
}

int saveBillings(BillingTable table)
{
    FILE * billingFile = fopen(BILLINGDB, "w"); // open the file in write mode
    
    for(int i = 0; i < table.count; i++)
    {
        char entry[500] = "";
        sprintf(entry, "%i,%d,%s,%d\n",
            table.entries[i].orderID,
            table.entries[i].customerID,
            table.entries[i].billingAddress,
            table.entries[i].orderPrice
        );

        fputs(entry, billingFile);
    }

    fclose(billingFile);
    return 0;
}

OrderTable * initOrders()
{
    OrderTable * table = malloc(sizeof(OrderTable));
    table->count = 0;
    table->entries = malloc(sizeof(Order) * MAX_ENTRIES);
}

void addOrder(Order toAdd, OrderTable * table)
{
    toAdd.orderID = ++table->count;
    table->entries[table->count - 1] = toAdd;
}

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

            addOrder(newOrder, table);
        }
    }
    else
    {
        return -1;
    }

    return 0;
}

int saveOrders(OrderTable table)
{
    FILE * orderFile = fopen(ORDERDB, "w"); // open the file in write mode
    
    for(int i = 0; i < table.count; i++)
    {
        char entry[500] = "";
        sprintf(entry, "%i,%d,%d,%s,%d\n",
            table.entries[i].orderID,
            table.entries[i].productID,
            table.entries[i].numPurchased,
            table.entries[i].deliveryAddress,
            table.entries[i].totalPrice
        );

        fputs(entry, orderFile);
    }

    fclose(orderFile);
    return 0;
}


