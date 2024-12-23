#ifndef FLIGHT_H
#define FLIGHT_H

typedef struct {
    char flightCode[10];
    char airline[50];
    char departure[50];
    char destination[50];
    char departureDate[11];
    char departureTime[6];
    char arrivalTime[6];
    int remainingSeats;
    char seatClass[20];
    float price;
} Flight;

int getFlightByCode(const char* file_path, const char* flight_code, Flight* flight);
int updateFlightSeats(const char* file_path, Flight* flight);

#endif // FLIGHT_H
