#include <stdio.h>

#include "database.h"
#include "database.c"

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

//TODO
//might be done in main function to have access to that new struct without returning it
void newSeller(){
    Seller newSeller;

    addSellerToTable(newSeller, tableOfSellers);
}

//TODO
//might be done in main function to have access to that new struct without returning it
void newCustomer(){
    Customer newCustomer;


    addCustomerToTable(newCustomer, tableOfCustomers);
}

//TODO
//purchase a product option 2 in showBuyerMenu
void newOrder(){
    Order newOrder;

    addOrderToTable(newOrder, tableOfOrders);
}
//not mentioned Yet part of newORDER
void newBilling(){
    BillingInfo newBill;

    addBillingToTable(newBill, tableOfBillings);
}


//return a product option 3 in showBuyerMenu
//delete order and billing struct
//return products to availability
void returnOrder(Order orderToReturn){
    //return products purshased to products available
    int productCount = tableOfProducts->count;
    for (int i = 0; i < productCount; i++){
        if (tableOfProducts->entries[i].productID == orderToReturn.productID){
            tableOfProducts->entries[i].numAvailable = tableOfProducts->entries[i].numAvailable + orderToReturn.numPurchased;
            break;
        }
    }

    //delete order from orderTable
    int orderCount = tableOfOrders->count;
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

//TODO
void addNewProduct(){
    Product newProduct;

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
void updateSellerInformation(){
    printf("What information would you like to update?\n");
    printf("\t1. Name\n\t2. Address\n\t3. Phone Number\n> ");
    
    char token[15];
    fgets(token, 15, stdin);

    if(token[0] == '1')
    {
        printf("What would you like to change it to?");
        bzero(token, 15);
        
    }
    if(token[0] == '2')
    {
        
    }
    if(token[0] == '3')
    {
        
    }
    else
    {
        printf("Invalid Input. Try Again.\n");
    }
}

//TODO
//replace case one in showBuyerMenu() maybe make global buyer struct
void updateCustomerInformation(){

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
            strcat(msg, "\nOrderID: ");
            strcat(msg, tableOfOrders->entries[i].orderID);
            strcat(msg, "\nProductID: ");
            strcat(msg, tableOfOrders->entries[i].productID);
            strcat(msg, "\nAmount of Product Purchased: ");
            strcat(msg, tableOfOrders->entries[i].numPurchased);
            strcat(msg, "\nDelivery Address: ");
            strcat(msg, tableOfOrders->entries[i].deliveryAddress);
            strcat(msg, "\nTotal Price: ");

            //change double type to char [] type
            int sizeOfPrice = sizeof(tableOfOrders->entries[i].totalPrice);
            char arr[sizeOfPrice];
            memcpy(arr, &tableOfOrders->entries[i].totalPrice, sizeOfPrice);

            strcat(msg, arr);
            strcat(msg, "\n\n");

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
            strcat(msg, "\nOrderID: ");
            strcat(msg, tableOfBillings->entries[i].orderID);
            strcat(msg, "\nProductID: ");
            strcat(msg, tableOfBillings->entries[i].customerID);
            strcat(msg, "\nBilling Address: ");
            strcat(msg, tableOfBillings->entries[i].billingAddress);
            strcat(msg, "\nOrder Price: ");


            //change double type to char [] type
            int sizeOfPrice = sizeof(tableOfBillings->entries[i].orderPrice);
            char arr[sizeOfPrice];
            memcpy(arr, &tableOfBillings->entries[i].orderPrice, sizeOfPrice);

            strcat(msg, arr);
            strcat(msg, "\n\n");

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

void viewProductsForSeller(int sellerID){

    //for all products if sellerID matches then print out
    for(int i = 0; i < tableOfProducts->count; i++)
    {
        if(tableOfProducts->entries[i].sellerID == sellerID) // check to see if the seller with selleID listed this product
        {
            Product product = tableOfProducts->entries[i]; // get product information

            printf("Product ID: %d\nQuantity Available: %d\nPrice: %f\n\nDescription: %s",
                product.productID,
                product.numAvailable,
                product.price,
                product.description
                );
        }
    }
}

void viewOrdersForProducts(int productID)
{
    //for all products if sellerID matches then print out
    for(int i = 0; i < tableOfOrders->count; i++)
    {
        if(tableOfOrders->entries[i].productID == productID)
        {
            Order order = tableOfOrders->entries[i]; // get product with billing information

            printf("Order ID: %d\nQuantity Purchased: %d\nTotal Order Price: %f\n\nDelivery Address: %s",
                order.orderID,
                order.numPurchased,
                order.totalPrice,
                order.deliveryAddress
                );
        }
    }
}
