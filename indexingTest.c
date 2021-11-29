#include <stdio.h>
#include <string.h>

#include "database.h"
#include "databaseFunctions.h"
int main(int argc, char const *argv[])
{
    Seller ted;
    strcpy(ted.name, "Ted Danson");
    strcpy(ted.address, "1618 Hollywood");
    strcpy(ted.phNumber, "168-789-1456");

    SellerTable * sellers = initSellers();
    addSellerToTable(ted, sellers);

    printf("Seller ID: %d\n", sellers->entries[0].sellerID);
    return 0;
}
