#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include "../include/SearchView.h"

#define BUFFER_SIZE 4096

void searchFlight(int sock) {
    char departure[50];
    char destination[50];
    char departureDate[11]; // dd/mm/yyyy
    char returnDate[11]; // dd/mm/yyyy
    int numofPassenger;
    char seatClass[20];
    char buffer[BUFFER_SIZE];
    
    while (getchar() != '\n');

    // Nhập thông tin từ người dùng
    printf("Enter departure location: ");
    if (fgets(departure, sizeof(departure), stdin) != NULL) {
        departure[strcspn(departure, "\n")] = '\0';
    }

    printf("Enter destination location: ");
    if (fgets(destination, sizeof(destination), stdin) != NULL) {
        destination[strcspn(destination, "\n")] = '\0';
    }

    printf("Enter departure date (dd/mm/yyyy): ");
    if (fgets(departureDate, sizeof(departureDate), stdin) != NULL) {
        departureDate[strcspn(departureDate, "\n")] = '\0';
    }

    int c, d;
    while ((c = getchar()) != '\n' && c != EOF);

    printf("Enter return date (dd/mm/yyyy) or press Enter for one-way flight: ");
    if (fgets(returnDate, sizeof(returnDate), stdin) != NULL) {
        returnDate[strcspn(returnDate, "\n")] = '\0';
        // Kiểm tra nếu người dùng chỉ nhấn Enter
        if (returnDate[0] == '\0' || strcmp(returnDate, "") == 0) {
            strcpy(returnDate, "NA");
        }
    } else {
        strcpy(returnDate, "NA");
    }
    while ((d = getchar()) != '\n' && d != EOF);

    // Nhập số lượng hành khách
    char numBuffer[10];
    printf("Enter number of passengers: ");
    if (fgets(numBuffer, sizeof(numBuffer), stdin) != NULL) {
        numofPassenger = atoi(numBuffer);
    } else {
        numofPassenger = 1; // Giá trị mặc định
    }
   
    printf("Enter seat class (Economy/Business): ");
    if (fgets(seatClass, sizeof(seatClass), stdin) != NULL) {
        seatClass[strcspn(seatClass, "\n")] = '\0';
    }

    // Tạo yêu cầu gửi đến server
    snprintf(buffer, BUFFER_SIZE, "search %s %s %s %s %d %s",
             departure, destination, departureDate, returnDate, numofPassenger, seatClass);

    // Gửi dữ liệu tới server
    send(sock, buffer, strlen(buffer), 0);

    // Nhận kết quả từ server
    memset(buffer, 0, BUFFER_SIZE);
    int bytes_received = recv(sock, buffer, BUFFER_SIZE - 1, 0);
    if (bytes_received > 0) {
        buffer[bytes_received] = '\0';
        displaySearchResults(buffer);
    } else if (bytes_received == 0) {
        printf("Server closed the connection.\n");
    } else {
        printf("Error receiving data from server.\n");
    }
}

void displaySearchResults(const char* response) {
    printf("\nSearch results:\n%s\n", response);
}