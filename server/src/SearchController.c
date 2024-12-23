#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../include/SearchController.h"
#include "../../model/User.h"
#include "../../model/Flight.h"

#define USER_FILE "data/accounts.txt"
#define MAX_FLIGHTS 100


//ham tim kiem chuyen bay cua nguoi dung
int searchFlights(const char* file_path, const char* departure, const char* destination,
                  const char* departureDate, const char* returnDate, int numOfPassengers,
                  const char* seatClass, Flight* results, int* result_count) {
    FILE* file = fopen(file_path, "r");
    if (!file) {
        perror("Could not open flights file");
        return -1;
    }

    Flight temp;
    *result_count = 0; // Khởi tạo số lượng chuyến bay phù hợp
    int returnFound = 0; // Biến kiểm tra có chuyến bay chiều ngược lại hay không

    // Đọc từng chuyến bay trong file
    while (fscanf(file, "%s %s %s %s %s %s %s %d %s %f", temp.flightCode, temp.airline,
                  temp.departure, temp.destination, temp.departureDate, temp.departureTime,
                  temp.arrivalTime, &temp.remainingSeats, temp.seatClass, &temp.price) != EOF) {
        // Kiểm tra điều kiện chuyến bay chiều đi
        if (strcmp(temp.departure, departure) == 0 &&
            strcmp(temp.destination, destination) == 0 &&
            strcmp(temp.departureDate, departureDate) == 0 &&
            temp.remainingSeats >= numOfPassengers &&
            strcmp(temp.seatClass, seatClass) == 0) {
            results[*result_count] = temp; // Lưu chuyến bay chiều đi
            (*result_count)++;
        }

        // Nếu có ngày về, kiểm tra chuyến bay chiều ngược lại
        if (strcmp(returnDate, "NA") != 0 && 
            strcmp(temp.departure, destination) == 0 &&
            strcmp(temp.destination, departure) == 0 &&
            strcmp(temp.departureDate, returnDate) == 0 &&
            temp.remainingSeats >= numOfPassengers &&
            strcmp(temp.seatClass, seatClass) == 0) {
            results[*result_count] = temp; // Lưu chuyến bay chiều về
            (*result_count)++;
            returnFound = 1;
        }

        // Giới hạn số lượng chuyến bay phù hợp lưu vào kết quả
        if (*result_count >= MAX_FLIGHTS) {
            break;
        }
    }

    fclose(file);

    if (*result_count > 0) {
        // Nếu có ngày về nhưng không tìm thấy chuyến bay chiều ngược lại
        if (strcmp(returnDate, "NA") != 0 && !returnFound) {
            printf("No return flights found for the specified return date.\n");
        }
        return 0; // Tìm thấy ít nhất một chuyến bay
    }

    return -1; // Không tìm thấy chuyến bay nào
}

