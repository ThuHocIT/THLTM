#ifndef BOOKED_CONTROLLER_H
#define BOOKED_CONTROLLER_H

void handleSeeBookedFlights(int client_socket, const char* booked_file, const char* user_name);

void handleCancelBooked(int client_socket, const char* booked_file, const char* flight_file, const char* user_file, const char* user_name, const char* flightCode, const int cancelNumber);
#endif // BOOKED_CONTROLLER_H
