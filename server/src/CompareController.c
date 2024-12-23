#include "../include/CompareController.h"
#include "../../model/Flight.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_FLIGHTS 100

int compareByPrice(const void* a, const void* b) {
    Flight* flightA = (Flight*)a;
    Flight* flightB = (Flight*)b;
    return (flightA->price > flightB->price) - (flightA->price < flightB->price);
}

int compareByDuration(const void* a, const void* b) {
    Flight* flightA = (Flight*)a;
    Flight* flightB = (Flight*)b;

    int hoursA, minutesA, hoursB, minutesB;
    sscanf(flightA->departureTime, "%d:%d", &hoursA, &minutesA);
    sscanf(flightA->arrivalTime, "%d:%d", &hoursB, &minutesB);
    int durationA = (hoursB * 60 + minutesB) - (hoursA * 60 + minutesA);

    sscanf(flightB->departureTime, "%d:%d", &hoursA, &minutesA);
    sscanf(flightB->arrivalTime, "%d:%d", &hoursB, &minutesB);
    int durationB = (hoursB * 60 + minutesB) - (hoursA * 60 + minutesA);

    return (durationA > durationB) - (durationA < durationB);
}

int compareFlights(const char* request, char* response) {
    char departure[50], destination[50], departureDate[11], returnDate[11], seatClass[20];
    int numOfPassengers;

    // Phân tích chuỗi yêu cầu từ client
    sscanf(request, "%s %s %s %s %d %s", departure, destination, departureDate, returnDate, &numOfPassengers, seatClass);

    Flight results[MAX_FLIGHTS];
    int resultCount = 0;

    // Gọi hàm tìm kiếm chuyến bay
    if (searchFlights("data/flights.txt", departure, destination, departureDate, returnDate, numOfPassengers, seatClass, results, &resultCount) < 0) {
        strcpy(response, "No matching flights found for comparison.\n");
        return -1;
    }

    // Sắp xếp và ghi kết quả theo giá
    strcat(response, "Sorted by price:\n");
    qsort(results, resultCount, sizeof(Flight), compareByPrice);
    for (int i = 0; i < resultCount; i++) {
    char flightInfo[512];
    snprintf(flightInfo, sizeof(flightInfo), 
             "%s %s %s -> %s %s %s %s %.2f\n", 
             results[i].flightCode, results[i].airline, 
             results[i].departure, results[i].destination, 
             results[i].departureDate, results[i].departureTime, 
             results[i].seatClass, results[i].price);
    strcat(response, flightInfo);
    }


    strcat(response, "\nSorted by duration:\n");
    qsort(results, resultCount, sizeof(Flight), compareByDuration);
    for (int i = 0; i < resultCount; i++) {
    char flightInfo[512];
    int hoursDep, minutesDep, hoursArr, minutesArr;
    sscanf(results[i].departureTime, "%d:%d", &hoursDep, &minutesDep);
    sscanf(results[i].arrivalTime, "%d:%d", &hoursArr, &minutesArr);
    int durationMinutes = (hoursArr * 60 + minutesArr) - (hoursDep * 60 + minutesDep);
    int durationHours = durationMinutes / 60;
    int durationRemainingMinutes = durationMinutes % 60;

    snprintf(flightInfo, sizeof(flightInfo), 
             "%s %s %s -> %s %s %s %s    %dh %dm\n", 
             results[i].flightCode, results[i].airline, 
             results[i].departure, results[i].destination, 
             results[i].departureDate, results[i].departureTime, 
             results[i].seatClass, durationHours, durationRemainingMinutes);
    strcat(response, flightInfo);
    }


    return 0;
}
