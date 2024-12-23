#ifndef SEARCH_CONTROLLER_H
#define SEARCH_CONTROLLER_H

#include "../../model/User.h"
#include "../../model/Flight.h"

#define FLIGHTS_FILE "data/flights.txt"

int searchFlights(const char* file_path, const char* departure, const char* destination,
                  const char* departureDate, const char* returnDate, int numOfPassengers,
                  const char* seatClass, Flight* results, int* result_count);



#endif // SEARCH_CONTROLLER_H