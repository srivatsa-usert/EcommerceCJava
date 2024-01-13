/**********************************************************************
    * Title: Main (C)
    * Author: Lakshmi Sai Srivatsa
    * Version: 1.0
    * Last Modified: 13-01-2024
**********************************************************************/
#include "ecommerce.h"
#include <stdio.h>
#include <stdlib.h>

int main() {
    int choice;


    do {
        printf("\n1. Add Item\n");
        printf("2. View Items\n");
        printf("3. View Orders\n");
        printf("4. Generate Bills\n");
        printf("0. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                addItem();
                break;
            case 2:
                viewItems();
                break;
            case 3:
                viewOrders();
                break;
            case 4:
                generateBills();
                break;
            case 0:
                printf("Exiting the program. Goodbye!\n");
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 0);

    return 0;
}
