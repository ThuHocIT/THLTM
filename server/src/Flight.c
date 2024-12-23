#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../../model/Flight.h"

// Hàm tìm kiếm chuyến bay theo mã
int getFlightByCode(const char* file_path, const char* flight_code, Flight* flight) {
    FILE* file = fopen(file_path, "r");
    if (!file) {
        perror("Could not open flights file");
        return -1;
    }

    Flight temp;
    while (fscanf(file, "%s %s %s %s %s %s %s %d %s %f", 
                  temp.flightCode, temp.airline, temp.departure, temp.destination, 
                  temp.departureDate, temp.departureTime, temp.arrivalTime,
                  &temp.remainingSeats, temp.seatClass, &temp.price) != EOF) {
        // printf("Debug: Flight Code: %s, Price: %.2f\n", temp.flightCode, temp.price);  // Kiểm tra giá trị price

        if (strcmp(temp.flightCode, flight_code) == 0) {
            *flight = temp; // Sao chép dữ liệu chuyến bay
            fclose(file);
            return 0; // Tìm thấy chuyến bay
        }
    }

    fclose(file);
    return -1; // Không tìm thấy chuyến bay
}


// Hàm cập nhật số ghế còn lại của chuyến bay
int updateFlightSeats(const char* file_path, Flight* updatedFlight) {
    FILE* file = fopen(file_path, "r+");
    if (file == NULL) {
        perror("Could not open flights file");
        return -1;
    }

    Flight flights[100];  // Giả sử bạn có tối đa 100 chuyến bay
    int flightCount = 0;

    // Đọc toàn bộ chuyến bay vào bộ nhớ
    while (fscanf(file, "%s %s %s %s %s %s %s %d %s %f", 
                  flights[flightCount].flightCode, flights[flightCount].airline,
                  flights[flightCount].departure, flights[flightCount].destination, 
                  flights[flightCount].departureDate,
                  flights[flightCount].departureTime, flights[flightCount].arrivalTime,
                  &flights[flightCount].remainingSeats, flights[flightCount].seatClass, 
                  &flights[flightCount].price) == 10) {
        flightCount++;
    }

    // Sửa chuyến bay cần thay đổi
    for (int i = 0; i < flightCount; i++) {
        if (strcmp(flights[i].flightCode, updatedFlight->flightCode) == 0) {
            flights[i] = *updatedFlight;
            break;
        }
    }

    // Ghi lại toàn bộ dữ liệu vào file
    fseek(file, 0, SEEK_SET);  // Quay lại đầu file
    for (int i = 0; i < flightCount; i++) {
        fprintf(file, "%s %s %s %s %s %s %s %d %s %.2f\n",
                flights[i].flightCode, flights[i].airline, flights[i].departure,
                flights[i].destination, flights[i].departureDate,
                flights[i].departureTime, flights[i].arrivalTime,
                flights[i].remainingSeats, flights[i].seatClass, flights[i].price);
    }

    fclose(file);
    return 0;  // Cập nhật thành công
}

