#include <stdio.h>
#include "../include/MenuView.h"
#include "../include/BookingView.h" // Để sử dụng bookFlight
#include "../include/BookedView.h"
#include "../include/CompareView.h"
#include "../include/SearchView.h"
#define BOOKED "data/booked_flights.txt"

void displayMenu() {
    printf("\n=====================\n");
    printf("       MENU\n");
    printf("=====================\n");
    printf("1. Find Flights\n");
    printf("2. Booking Flights\n");
    printf("3. Flight Booked\n");
    printf("4. Compare Flights\n");
    printf("5. Exit\n");
    printf("=====================\n");
}

int getUserChoice() {
    int choice;
    printf("Please enter your choice: ");
    while (scanf("%d", &choice) != 1 || choice < 1 || choice > 5) {
        printf("Invalid input. Please enter a number between 1 and 4: ");
        while (getchar() != '\n'); // Clear invalid input
    }
    return choice;
}

void handleMenuSelection(int choice, int sock) {
    switch (choice) {
        case 1:
            searchFlight(sock);
            break;
        case 2:
            bookFlight(sock); // Gọi hàm bookFlight từ BookingView
            break;
        case 3:
            bookedMenu(sock);
            break;
        case 4: 
            compareFlight(sock); 
            break;   
        case 5:
            printf("Exiting program. Goodbye!\n");
            break;
        default:
            printf("Invalid choice.\n");
            break;
    }
}
