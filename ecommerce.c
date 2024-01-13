/**********************************************************************
    * Title: ecommerce (source)
    * Author: Lakshmi Sai Srivatsa
    * Version: 1.0
    * Last Modified: 13-01-2024
**********************************************************************/

#include "ecommerce.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

Item *items;

void addItem() {

    // Display available items from items.txt
    viewItems();

    int orderId = rand() % 1000 + 1;  // Generate a random positive order number between 1 and 1000
    int itemId, quantity;
    int orderItemCount = 0;
    OrderItem* orderItems = NULL;

    while (1) {
        printf("Enter the item ID you want to purchase (0 to finish, -1 to cancel order): ");
        scanf("%d", &itemId);

        // Check if the user wants to finish or cancel the order
        if (itemId == 0) {
            // Finish and place the order
            if (orderItemCount > 0) {
                char buyerName[50];
                char orderDate[15];

                printf("Enter buyer name: ");
                scanf("%s", buyerName);

                // Get the current system date
                time_t t = time(NULL);
                struct tm tm_info = *localtime(&t);
                strftime(orderDate, sizeof(orderDate), "%Y-%m-%d", &tm_info);

                printf("Enter order date (YYYY-MM-DD): ");
                scanf("%s", orderDate);

                // Place the order
                placeOrder(orderId, buyerName, orderDate, orderItems, orderItemCount);

                // Free memory allocated for order items
                free(orderItems);
                orderItemCount = 0;
            } else {
                printf("No items in the order. Order not placed.\n");
            }

            // Reset orderId to -1 to indicate no order in progress
            orderId = -1;
            break;
        } else if (itemId == -1) {
            // Cancel the current order
            printf("Order canceled.\n");
            if (orderItems != NULL) {
                free(orderItems);
            }
            orderItemCount = 0;

            // Reset orderId to -1 to indicate no order in progress
            orderId = -1;
            break;
        }

        // Check if the item ID is valid, -- valid if price != 0.0
        char itemName[50];
        double price = getItemDetails(itemId,itemName);
        if (price == 0.0) {
            printf("Invalid item ID. Please try again.\n");
            continue;
        }

        // Generate a new order ID if starting a new order
        if (orderId == -1) {
            orderId = rand() % 1000 + 1;  // Generate a new random positive order number
            printf("New Order ID: %d\n", orderId);
        }

        printf("Enter the quantity: ");
        scanf("%d", &quantity);

        // Allocate memory for a new OrderItem
        orderItems = realloc(orderItems, (orderItemCount + 1) * sizeof(OrderItem));

        if (orderItems == NULL) {
            printf("Memory allocation error. Unable to add item to order.\n");
            exit(EXIT_FAILURE);
        }

        // Add the new item to the order
        orderItems[orderItemCount].itemId = itemId;
        orderItems[orderItemCount].quantity = quantity;
        ++orderItemCount;
    }
}


void viewItems() {
    FILE* file = fopen("items.txt", "r");
    if (file == NULL) {
        printf("Error opening file for reading items.\n");
        return;
    }

    printf("%-10s%-15s%-25s%-10s\n", "ID", "Item", "Company", "Price");
    printf("----------------------------------------------------------\n");
    
    int itemId;
    char itemName[50];
    char companyName[50];
    double price;

    while (fscanf(file, "%d %[^\t] %[^\t] %lf\n", &itemId, itemName, companyName, &price) == 4) {
        // %[^\t] is used to read strings with spaces until a tab character is encountered
        printf("%-10d%-15s%-25s%-10.2f\n", itemId, itemName, companyName, price);
    }

    fclose(file);
}



void placeOrder(int orderId, const char* buyerName, const char* orderDate, const OrderItem items[], int itemCount) {

    FILE* file = fopen(FILENAME_ORDERS, "a");
    if (file == NULL) {
        printf("Error opening file for appending orders.\n");
        return;
    }

    fprintf(file, "%d\t%s\t%s\n", orderId, buyerName, orderDate);

    for (int i = 0; i < itemCount; ++i) {
        fprintf(file, "%d\t%d\n", items[i].itemId, items[i].quantity);
    }

    fprintf(file, "#\n");
    fclose(file);

    printf("Order placed successfully!\n");
}


void viewOrders() {
    FILE* file = fopen(FILENAME_ORDERS, "r");
    if (file == NULL) {
        printf("Error opening file for reading orders.\n");
        return;
    }

    printf("Orders:\n");

    // Display order summary (ID, Buyer, Date)
    while (1) {
        int orderId;
        char buyerName[50];
        char orderDate[15];

        // Read order information
        if (fscanf(file, "%d\t%s\t%s\n", &orderId, buyerName, orderDate) != 3) {
            break; // End of file
        }

        // Display order summary
        printf("Order ID: %d, Buyer: %s, Date: %s\n", orderId, buyerName, orderDate);

        // Skip item details until a line with '#' is encountered, '#' is my seperator
        char buffer[256];
        while (fgets(buffer, sizeof(buffer), file) != NULL) {
            if (buffer[0] == '#') {
                break; // End of order details
            }
        }
    }

    fclose(file);

    // Allow the user to choose an order ID
    int chosenOrderId;
    printf("\nEnter the Order ID you want to view details for (enter 0 to go back): ");
    scanf("%d", &chosenOrderId);

    if (chosenOrderId != 0) {
        // Reopen the file to read order details
        file = fopen(FILENAME_ORDERS, "r");
        if (file == NULL) {
            printf("Error opening file for reading orders.\n");
            return;
        }

        // Display detailed order information for the chosen order ID
        while (1) {
            int orderId;
            char buyerName[50];
            char orderDate[15];

            // Read order information
            if (fscanf(file, "%d\t%s\t%s", &orderId, buyerName, orderDate) != 3) {
                break; // End of file
            }


            // Check if the current order ID matches the chosen order ID
            if (orderId == chosenOrderId) {
                printf("\nOrder ID: %d, Buyer: %s, Date: %s\n", orderId, buyerName, orderDate);

                // Display item details for the current order
                printf("%-10s%-15s%-15s%-10s%-15s\n", "Item ID", "Item Name", "Item Price", "Quantity", "Total Price");
                printf("----------------------------------------------------------\n");

                double totalPrice = 0.0;

                int flag = 0;
                while (1) {
                    int itemId, quantity;
                    int scanned = fscanf(file, "%d\t%d\n", &itemId, &quantity);

                    // printf("%d %d\n", itemId, quantity); just a line to debug, kept it for later checking

                    if (scanned == EOF) {
                        printf("End of Order\n");
                        break; // End of order
                    } else if (scanned != 2) {
                        if(flag == 0)
                            printf("Error reading item details. Expected 2 values, got %d\n", scanned);
                        break;
                    }

                    // Retrieve item details
                    char itemName[50];
                    double itemPrice = getItemDetails(itemId, itemName);
                    // Check if item details were retrieved successfully
                    if (itemPrice < 0.0) {
                        printf("Error retrieving item details for ID: %d\n", itemId);
                        break;
                    }

                    // Calculate total price for the item
                    double totalItemPrice = itemPrice * quantity;
                    totalPrice += totalItemPrice;

                    // Display item details
                    printf("%-10d%-15s%-15.2f%-10d%-15.2f\n", itemId, itemName, itemPrice, quantity, totalItemPrice);
                    flag = 1;
                }


                // Display total order price
                printf("----------------------------------------------------------\n");
                printf("Grand Total Order Price: %.2f\n", totalPrice);

                break; // Stop reading orders after finding the chosen order ID
            }

            // Skip item details until a line with '#' is encountered
            char buffer[256];
            while (fgets(buffer, sizeof(buffer), file) != NULL) {
                if (buffer[0] == '#') {
                    break; // End of order details
                }
            }
        }

        fclose(file);
    }
}



void generateBills() {
    FILE* file = fopen(FILENAME_ORDERS, "r");
    if (file == NULL) {
        printf("Error opening file for reading orders.\n");
        return;
    }

    printf("Available Orders:\n");

    // Display order summary (ID, Buyer, Date)
    while (1) {
        int orderId;
        char buyerName[50];
        char orderDate[15];

        // Read order information
        if (fscanf(file, "%d\t%s\t%s\n", &orderId, buyerName, orderDate) != 3) {
            break; // End of file
        }

        // Display order summary
        printf("Order ID: %d, Buyer: %s, Date: %s\n", orderId, buyerName, orderDate);

        // Skip item details until a line with '#' is encountered
        char buffer[256];
        while (fgets(buffer, sizeof(buffer), file) != NULL) {
            if (buffer[0] == '#') {
                break; // End of order details
            }
        }
    }

    fclose(file);

    // Allow the user to choose an order ID
    int chosenOrderId;
    printf("\nEnter the Order ID for which you want to generate a bill (enter 0 to go back): ");
    scanf("%d", &chosenOrderId);

    if (chosenOrderId != 0) {
        // Reopen the file to read order details
        file = fopen(FILENAME_ORDERS, "r");
        if (file == NULL) {
            printf("Error opening file for reading orders.\n");
            return;
        }

        // Display detailed order information for the chosen order ID
        while (1) {
            int orderId;
            char buyerName[50];
            char orderDate[15];

            // Read order information
            if (fscanf(file, "%d\t%s\t%s", &orderId, buyerName, orderDate) != 3) {
                break; // End of file
            }

            // Check if the current order ID matches the chosen order ID
            if (orderId == chosenOrderId) {
                printf("\nGenerating Bill for Order ID: %d\n", orderId);
                printf("Buyer: %s, Date: %s\n", buyerName, orderDate);

                // Open a file to write the bill
                char billFileName[50];
                sprintf(billFileName, "bill_%d.txt", orderId);
                FILE* billFile = fopen(billFileName, "w");
                if (billFile == NULL) {
                    printf("Error opening file for writing the bill.\n");
                    return;
                }

                // Display item details and write to the bill file
                fprintf(billFile, "Order ID: %d\nBuyer: %s\nDate: %s\n", orderId, buyerName, orderDate);
                fprintf(billFile, "Items:\n");

                // Display table header
                printf("%-10s%-20s%-15s%-10s%-15s\n", "Item ID", "Item Name", "Item Price", "Quantity", "Total Price");
                fprintf(billFile, "%-10s%-20s%-15s%-10s%-15s\n", "Item ID", "Item Name", "Item Price", "Quantity", "Total Price");
                
                // Display separator line
                printf("----------------------------------------------------------\n");
                fprintf(billFile, "----------------------------------------------------------\n");

                double totalItemPrice = 0.0;

                while (1) {
                    int itemId, quantity;
                    if (fscanf(file, "%d\t%d", &itemId, &quantity) != 2) {
                        break; // End of order
                    }

                    // Retrieve item details
                    char itemName[50];
                    double itemPrice = getItemDetails(itemId, itemName);
                    double itemTotalPrice = itemPrice * quantity;

                    // Display and write to the bill file
                    printf("%-10d%-20s%-15.2f%-10d%-15.2f\n", itemId, itemName, itemPrice, quantity, itemTotalPrice);
                    fprintf(billFile, "%-10d%-20s%-15.2f%-10d%-15.2f\n", itemId, itemName, itemPrice, quantity, itemTotalPrice);

                    totalItemPrice += itemTotalPrice;
                }

                // Display separator line
                printf("----------------------------------------------------------\n");
                fprintf(billFile, "----------------------------------------------------------\n");

                // Display total item price, GST, and grand total
                printf("%-45s%-10.2f\n", "Grand Total Order Price:", totalItemPrice);
                fprintf(billFile, "%-45s%-10.2f\n", "Grand Total Order Price:", totalItemPrice);

                double gstAmount = totalItemPrice * GST_RATE;
                double grandTotal = totalItemPrice + gstAmount;

                printf("%-45s%-10.2f\n", "GST (18%%):", gstAmount);
                fprintf(billFile, "%-45s%-10.2f\n", "GST (18%%):", gstAmount);

                printf("%-45s%-10.2f\n", "Grand Total:", grandTotal);
                fprintf(billFile, "%-45s%-10.2f\n", "Grand Total:", grandTotal);

                // Close the bill file
                fclose(billFile);

                printf("Bill generated successfully! Check %s for the bill.\n", billFileName);

                break; // Stop reading orders after finding the chosen order ID
            }


            char buffer[256];
            while (fgets(buffer, sizeof(buffer), file) != NULL) {
                if (buffer[0] == '#') {
                    break; // End of order details
                }
            }

        }

        fclose(file);
    }
}

// Function to get item details (name and price) by item ID
double getItemDetails(int itemId, char itemName[50]) {
    FILE* file = fopen(FILENAME_ITEMS, "r");
    if (file == NULL) {
        printf("Error opening file for reading items.\n");
        return 0.0;
    }

    double price = 0.0;
    int id;
    char company[50];

    while (fscanf(file, "%d %[^\t] %[^\t] %lf", &id, itemName, company, &price) == 4) {
        if (id == itemId) {
            fclose(file);
            return price;
        }
    }

    fclose(file);
    return 0.0; // Item not found
}