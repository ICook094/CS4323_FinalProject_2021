#include "database.h"

BillingTable * tableOfBillings;
CustomerTable * tableOfCustomers;
OrderTable * tableOfOrders;
ProductTable * tableOfProducts;
SellerTable * tableOfSellers;

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

void newBilling(){
    BillingInfo newBill;

    addBillingToTable(newBill, tableOfBillings);
}

void newCustomer(){
    Customer newCustomer;


    addCustomerToTable(newCustomer, tableOfCustomers);
}

void newOrder(){
    Order newOrder;

    addOrderToTable(newOrder, tableOfOrders);
}

void newProduct(){
    Product newProduct;

    addProductToTable(newProduct, tableOfProducts);
}

void newSeller(){
    Seller newSeller;

    addSellerToTable(newSeller, tableOfSellers);
}

void removeProduct(int productID){
    int count = tableOfProducts->count;
    for(int i = 0; i < count; i++){
        if (tableOfProducts->entries->productID == productID){
            //entries may need to be an array
            for (int j = i; j < count; j++){
                tableOfProducts->entries[i] = tableOfProducts->entries[i+1];
            }
        }
    }
    tableOfProducts->count--;
}

void updateProductQuantity(){

}

void updateProductPrice(){

}

void viewOrder(){

}

void viewBillingInfo(){

}

void viewProductsAvailable(){
    //print out all available products and their quantity and price
}

void viewMyProducts(){
    //for all products if sellerID matches the print out
}
