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
    initBillings();
    initCustomers();
    initOrders();
    initProducts();
    initSellers();
    

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

void viewOrdersForMyProducts(){

}

void viewProductsAvailable(){
    //print out all available products and their quantity and price
}

void viewMyProducts(){
    //for all products if sellerID matches the print out
}
