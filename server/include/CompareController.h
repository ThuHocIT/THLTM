#ifndef COMPARE_CONTROLLER_H
#define COMPARE_CONTROLLER_H

#include "../../model/Flight.h"

#define MAX_FLIGHTS 100

int compareByPrice(const void* a, const void* b);
int compareByDuration(const void* a, const void* b);

int compareFlights(const char* request, char* response);

#endif // COMPARE_FLIGHT_CONTROLLER_H
