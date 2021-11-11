#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>

/*
*   Communication Between Server and Data Server
*   Using Shared Memory - mmap
*
*   At the start of each server an access point to the mmap will need to be declared
*       to add to the server
*
*/

struct someInformation{
    int ID;
    char name[50]=0;
};

void main(){

    //create shared memory to hold struct
    int shmid = shmget(1234, sizeof(struct someInformation), IPC_CREAT|0666);

    struct someInformation* pData =(struct someInformation *) shmat(shmid, NULL, 0);

    //attach to process
    pData = shmat(shmid,0,0);

    //create memory for ID
    shmid = shmget(123, sizeof(int), IPC_CREAT|0666);
    //attach to process
    pData->ID = shmat(shmid,0,0);

    //create memory for name
    shmid = shmget(12, sizeof(char[50]), IPC_CREAT|0666);
    //attach to process
    pData->name = shmat(shmid,0,0);
}

/*
    Data to be sent to and from data derver
        UserName
        SellerID
        CustomerID
            SellerName (First and last)
            ContactNumber (XXX-XXX-XXXX)
            Contact Address
        ProductID
            ProductDescription
            SellerID
            ProductQuantity
            ProductPrice (per unit)
        OrderID
            CustomerID
            CustomerBillingAddress
            TotalOrderPrice
        CustomerOrder
            OrderID
            ProductID
            QuantityPurchased
            DeliveryAddress (AKA CustomerAdress)
            TotalPrice
*/

