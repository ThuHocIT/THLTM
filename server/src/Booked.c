#include <stdio.h>
#include <string.h>

#include "../../model/Booked.h"

int saveBookedFlight(const char* file_path, Booked* bookedFlight) {
    FILE *file = fopen(file_path, "a"); // Mở file ở chế độ append (thêm vào cuối file)
    if (file == NULL) {
        perror("Error opening booked flights file");
        return -1; // Lỗi khi mở file
    }

    // printf("Debug: Writing to file: %s %s %s %s %s %s %d %.2f\n", 
    //    bookedFlight->user.name, bookedFlight->user.email, 
    //    bookedFlight->flight.flightCode, bookedFlight->flight.departure, 
    //    bookedFlight->flight.destination, bookedFlight->bookingDate, 
    //    bookedFlight->numberOfSeats, bookedFlight->totalPrice);


    // Lưu thông tin chuyến bay và người dùng vào file
    fprintf(file, "%s ", bookedFlight->user.name);
    fprintf(file, "%s ", bookedFlight->user.email);
    fprintf(file, "%s ", bookedFlight->flight.flightCode);
    fprintf(file, "%s ", bookedFlight->flight.departure);
    fprintf(file, "%s ", bookedFlight->flight.destination);
    fprintf(file, "%s ", bookedFlight->bookingDate);
    fprintf(file, "%d ", bookedFlight->numberOfSeats);
    fprintf(file, "%.2f\n", bookedFlight->totalPrice);

    fclose(file); // Đóng file
    return 0; // Thành công
}

int findBookedFlight(const char* file_path, const char* username, const char* flightCode, Booked* foundBooked) {
    FILE* file = fopen(file_path, "r");
    if (!file) {
        perror("Could not open booked file");
        return -1;
    }

    char line[1024];
    while (fgets(line, sizeof(line), file)) {
        Booked tempBooked;
        if (sscanf(line, "%s %s %s %s %s %s %d %f", 
                   tempBooked.user.name,
                   tempBooked.user.email,
                   tempBooked.flight.flightCode,
                   tempBooked.flight.departure,
                   tempBooked.flight.destination,
                   tempBooked.bookingDate,
                   &tempBooked.numberOfSeats,
                   &tempBooked.totalPrice) == 8) {
            // Kiểm tra khớp username và flightCode
            if (strcmp(tempBooked.user.name, username) == 0 && strcmp(tempBooked.flight.flightCode, flightCode) == 0) {
                *foundBooked = tempBooked; // Sao chép dữ liệu
                fclose(file);
                return 0; // Tìm thấy
            }
        }
    }

    fclose(file);
    return -1; // Không tìm thấy
}