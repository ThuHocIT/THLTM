#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <time.h>
#include <pthread.h>
#include "../include/UserController.h"
#include "../include/BookingController.h"
#include "../../model/Booked.h"
#include "../../model/Flight.h"
#include "../include/BookedController.h"
#include "../include/SearchController.h"
#include "../include/CompareController.h"

#define LOG_FILE "data/server_log.txt"
#define USER "data/accounts.txt"
#define FLIGHT_FILE "data/flights.txt"
#define BOOKED "data/booked_flights.txt"

#define PORT 8080
#define BUFFER_SIZE 4096

pthread_mutex_t log_mutex = PTHREAD_MUTEX_INITIALIZER;  // Mutex để bảo vệ ghi log

char* getCurrentDate() {
    static char dateStr[11];  // Dự trữ bộ nhớ cho chuỗi ngày (yyyy-mm-dd)
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    snprintf(dateStr, sizeof(dateStr), "%04d-%02d-%02d", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday);
    return dateStr;
}

// Hàm ghi log
void log_message(const char *message) {
    pthread_mutex_lock(&log_mutex);  // Khóa mutex để đảm bảo không có luồng nào khác ghi log cùng lúc

    FILE *log_file = fopen(LOG_FILE, "a");
    if (log_file == NULL) {
        perror("Could not open log file");
        pthread_mutex_unlock(&log_mutex);
        return;
    }
    fprintf(log_file, "%s\n", message);
    fclose(log_file);

    pthread_mutex_unlock(&log_mutex);  // Mở khóa mutex
}

// Hàm xử lý client
void* handle_client(void* arg) {
    int new_socket = *(int*)arg;  // Socket của client
    char buffer[BUFFER_SIZE] = {0};
    User currentUser;
    Flight flight;
    int ticketCount;

    // Xử lý yêu cầu từ client
    while (1) {
        memset(buffer, 0, BUFFER_SIZE);
        int bytes_received = recv(new_socket, buffer, BUFFER_SIZE - 1, 0);

        if (bytes_received > 0) {
            buffer[bytes_received] = '\0';  // Đảm bảo buffer kết thúc bằng '\0'
            printf("Received: %s\n", buffer);
            log_message("Received from client: ");
            log_message(buffer);
        } else if (bytes_received == 0) {
            printf("Client disconnected.\n");
            break;
        } else {
            perror("recv error");
            break;
        }

        // Xử lý đăng ký
        if (strncmp(buffer, "register", 8) == 0) {
            User user;
            sscanf(buffer + 9, "%s %s %s", user.name, user.email, user.password);

            char response[BUFFER_SIZE];
            memset(response, 0, BUFFER_SIZE);

            if (registerUser(USER, &user) == 0) {
                snprintf(response, BUFFER_SIZE, "Register success");
            } else {
                snprintf(response, BUFFER_SIZE, "Register failed");
            }
            send(new_socket, response, strlen(response), 0);  // Gửi phản hồi
            log_message("Sent to client: ");
            log_message(response);
        }

        // Xử lý đăng nhập
        else if (strncmp(buffer, "login", 5) == 0) {
            char name[50], password[50];
            sscanf(buffer + 6, "%s %s", name, password);

            char response[BUFFER_SIZE];
            memset(response, 0, BUFFER_SIZE);

            if (loginUser(USER, name, password) == 0) {
                User loggedInUser;
                if(getUser(USER, name, &loggedInUser) == 0) {
                    currentUser = loggedInUser;
                }
                snprintf(response, BUFFER_SIZE, "LOGIN_SUCCESS %s", name);
            } else {
                snprintf(response, BUFFER_SIZE, "Login failed");
            }
            send(new_socket, response, strlen(response), 0);  // Gửi phản hồi
            log_message("Sent to client: ");
            log_message(response);
        }
        //xu ly tim kiem  
            else if (strncmp(buffer, "search", 6) == 0) {
                char departure[50], destination[50], departureDate[11], returnDate[11];
                int numOfPassengers;
                char seatClass[20];

                // Giải mã yêu cầu từ client
                sscanf(buffer + 7, "%s %s %s %s %d %s", 
                    departure, destination, departureDate, returnDate, &numOfPassengers, seatClass);

                // Mảng lưu trữ kết quả tìm kiếm
                Flight results[MAX_FLIGHTS];
                int result_count = 0;
                int returnFound = 0; // Biến kiểm tra có chuyến bay chiều ngược lại hay không

                // Gọi hàm searchFlights để tìm kiếm
                if (searchFlights(FLIGHT_FILE, departure, destination, departureDate, returnDate,
                                numOfPassengers, seatClass, results, &result_count) == 0) {
                    // Tạo phản hồi chứa danh sách chuyến bay
                    char response[BUFFER_SIZE];
                    memset(response, 0, sizeof(response));

                    for (int i = 0; i < result_count; i++) {
                        char flightInfo[512];
                        snprintf(flightInfo, sizeof(flightInfo), 
                                "Code: %s, Airline: %s, From: %s, To: %s, Date: %s, Time: %s, Seats: %d, Class: %s, Price: %f\n",
                                results[i].flightCode, results[i].airline, results[i].departure, 
                                results[i].destination, results[i].departureDate, results[i].departureTime, 
                                results[i].remainingSeats, results[i].seatClass, results[i].price);
                        strncat(response, flightInfo, sizeof(response) - strlen(response) - 1);
                    }

                    // Gửi danh sách chuyến bay về client
                            send(new_socket, response, strlen(response), 0);
                        } else {
                            // Không tìm thấy chuyến bay nào
                            char response[BUFFER_SIZE] = "No matching flights found.";
                            send(new_socket, response, strlen(response), 0);
                        }
                        // Ghi log
                        log_message("Sent to client: ");
                        log_message(buffer);
                }

        // Xử lý so sánh chuyến bay
            else if (strncmp(buffer, "compare", 7) == 0) {
                char response[BUFFER_SIZE];
                memset(response, 0, sizeof(response));

                // Call compareFlights with the received request
                int result = compareFlights(buffer + 8, response); // Skip "compare "

                // Send back the response
                if (result == 0) {
                    send(new_socket, response, strlen(response), 0);
                } else {
                    char errorResponse[] = "No matching flights found for comparison.";
                    send(new_socket, errorResponse, strlen(errorResponse), 0);
                }
                send(new_socket, response, strlen(response), 0);  // Gửi phản hồi
                log_message("Sent to client: ");
                log_message(response);
            }


        // Xử lý đặt vé
        else if (strncmp(buffer, "book", 4) == 0) {
            char flightCode[10];
            sscanf(buffer + 5, "%s %d", flightCode, &ticketCount);
            printf("Ticket Count received in booking request: %d\n", ticketCount);
            if (bookFlight(USER, FLIGHT_FILE, &currentUser, flightCode, ticketCount) == 0) {
                // Lấy thông tin chuyến bay sau khi đặt vé thành công
                if (getFlightByCode(FLIGHT_FILE, flightCode, &flight) == 0) {
                    Booked bookedFlight;
                    bookedFlight.user = currentUser;
                    strcpy(bookedFlight.bookingDate, getCurrentDate());  // Lấy ngày đặt vé hiện tại hoặc từ client
                    // printf("Ngày hiện tại được lưu là: %s\n", bookedFlight.bookingDate);
                    bookedFlight.flight = flight;  // Lấy thông tin chuyến bay từ flight đã tìm thấy
                    bookedFlight.numberOfSeats = ticketCount;
                    bookedFlight.totalPrice = ticketCount * flight.price;
                    
//                     printf("User: %s, Email: %s\n", bookedFlight.user.name, bookedFlight.user.email);
//                     printf("Flight price: %.2f\n", flight.price);

// printf("Flight Code: %s, Departure: %s, Destination: %s\n",
//        bookedFlight.flight.flightCode, bookedFlight.flight.departure, bookedFlight.flight.destination);
// printf("Booking Date: %s\n", bookedFlight.bookingDate);
// printf("Number of Seats: %d, Total Price: %.2f\n",
//        bookedFlight.numberOfSeats, bookedFlight.totalPrice);

                    // Lưu thông tin đặt vé vào file
                    if (saveBookedFlight(BOOKED, &bookedFlight) != 0) {
                        char response[BUFFER_SIZE] = "Booking failed while saving.";
                        send(new_socket, response, strlen(response), 0);
                        continue;
                    }

                    char response[BUFFER_SIZE] = "Booking successful.";
                    send(new_socket, response, strlen(response), 0);
                } else {
                    char response[BUFFER_SIZE] = "Failed to retrieve flight information.";
                    send(new_socket, response, strlen(response), 0);
                }
            } else {
                char response[BUFFER_SIZE] = "Booking failed.";
                send(new_socket, response, strlen(response), 0);
            }
        }
        else if (strncmp(buffer, "See Ticket", 10) == 0) {
            printf("Processing 'See Ticket' request from client.\n");
            printf("Current user: [%s]\n", currentUser.name);
            if (strlen(currentUser.name) == 0) {
            char response[BUFFER_SIZE] = "User not logged in. Please log in first.";
            send(new_socket, response, strlen(response), 0);
            continue;
            }

            // Gọi BookedController để xử lý
            handleSeeBookedFlights(new_socket, BOOKED, currentUser.name); // currentUser.email là email của người dùng hiện tại
        }
        else if (strncmp(buffer, "Cancel", 6) == 0) {
            char flightCode[20];
            int cancelCount;
    
            // Phân tích thông điệp từ client
            if (sscanf(buffer + 7, "%s %d", flightCode, &cancelCount) != 2) {
                char response[BUFFER_SIZE] = "Invalid Cancel command format. Usage: Cancel <flightCode> <number>";
                send(new_socket, response, strlen(response), 0);
                continue;
            }

            // Kiểm tra xem người dùng đã đăng nhập hay chưa
            if (strlen(currentUser.name) == 0) {
                char response[BUFFER_SIZE] = "User not logged in. Please log in first.";
                send(new_socket, response, strlen(response), 0);
                continue;
            }

            printf("Processing Cancel for flight %s with %d seats by user %s.\n", flightCode, cancelCount, currentUser.name);

            // Gọi hàm xử lý hủy vé từ Controller
            handleCancelBooked(new_socket, BOOKED, FLIGHT_FILE, USER, currentUser.name, flightCode, cancelCount);
        }

        


        // Ngắt kết nối nếu nhận "exit"
        else if (strncmp(buffer, "exit", 4) == 0) {
            printf("Client requested disconnection.\n");
            break;
        } else {
            char response[BUFFER_SIZE] = "Invalid command";
            send(new_socket, response, strlen(response), 0);
        }
    }

    // Đóng kết nối với client
    close(new_socket);
    return NULL;
}

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    pthread_t tid;  // Để lưu thread id

    // Tạo socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }

    // Thiết lập địa chỉ server
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Gắn socket với địa chỉ và cổng
    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    // Lắng nghe kết nối
    if (listen(server_fd, 3) < 0) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d...\n", PORT);

    // Vòng lặp chính của server
    while (1) {
        // Chấp nhận kết nối từ client
        if ((new_socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen)) < 0) {
            perror("Accept failed");
            exit(EXIT_FAILURE);
        }

        printf("Client connected.\n");

        // Tạo một luồng mới để xử lý kết nối client
        if (pthread_create(&tid, NULL, handle_client, (void*)&new_socket) != 0) {
            perror("Thread creation failed");
            continue;
        }

        // Tách luồng để server có thể tiếp tục chấp nhận kết nối từ các client khác
        pthread_detach(tid);
    }

    return 0;
} 