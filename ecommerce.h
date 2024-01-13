/**********************************************************************
    * Title: ecommerce (header)
    * Author: Lakshmi Sai Srivatsa
    * Version: 1.0
    * Last Modified: 12-01-2024
**********************************************************************/

#ifndef ECOMMERCE_H
#define ECOMMERCE_H

#define GST_RATE 0.18
#define FILENAME_ITEMS "items.txt"
#define FILENAME_ORDERS "orders.txt"

typedef struct {
    int itemId;
    char itemName[50];
    char companyName[50];
    double price;
} Item;

typedef struct {
    int itemId;
    int quantity;
} OrderItem;

typedef struct {
    int orderId;
    char buyerName[50];
    char orderDate[15];
    OrderItem items[50]; // Assuming a maximum of 50 items per order
    int itemCount;
} Order;

extern Item *items;

void addItem();
void viewItems();
void placeOrder(int orderId, const char* buyerName, const char* orderDate, const OrderItem items[], int itemCount);
void viewOrders();
void generateBills();
double getItemDetails(int itemId, char itemName[50]);

#endif  // ECOMMERCE_H
