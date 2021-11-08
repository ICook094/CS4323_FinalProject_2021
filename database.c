#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "database.h"

void addSeller(Seller newSeller, SellerTable * table)
{
    newSeller.sellerID = ++table->count;
    table->sellers[table->count - 1] = newSeller;
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
    FILE * sellerFile = fopen(SELLERDB, "w"); // open the file in write mode
    
    for(int i = 0; i < table.count; i++)
    {
        if(table.sellers[i].sellerID == 0) break; //there should not be an id with a value of 0 or less

        char entry[500] = "";
        sprintf(entry, "%i,%s,%s,%s\n",
            table.sellers[i].sellerID,
            table.sellers[i].name,
            table.sellers[i].phNumber,
            table.sellers[i].address
        );

        fputs(entry, sellerFile);
    }

    fclose(sellerFile);
    return 0;
}

int main(int argc, char const *argv[])
{
    SellerTable sellerTable;
    sellerTable.count = 0;

    sellerTable.sellers = malloc(sizeof(Seller) * MAX_ENTRIES);

    // Seller newSeller;
    // strcpy(newSeller.name, "Cole Hutson");
    // strcpy(newSeller.phNumber, "918-616-3623");
    // strcpy(newSeller.address,"1913 E elm Ave");

    // addSeller(newSeller, &sellerTable);

    // strcpy(newSeller.name, "Jimbob Bopete");
    // strcpy(newSeller.phNumber, "405-576-8672");
    // strcpy(newSeller.address,"520 S GoldMine Rd");

    // addSeller(newSeller, &sellerTable);

    // saveSellers(sellerTable);

    loadSellers(&sellerTable);
    
    for(int i = 0; i < sellerTable.count; i++)
    {
        printf("%s\n", sellerTable.sellers[i].name);
    }
    
    return 0;
}
