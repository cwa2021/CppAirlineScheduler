#include <functional>
#include <cstdlib>

#include "aircraft_collection.h"
#include "aircraft.h"
#include "crew.h"
#include "crew_collection.h"
#include "flight.h"
#include "flight_collection.h"
#include "helper.h"
#include "time_span.h"

using namespace std;
using namespace std::placeholders;


int main();
void mainMenu();

void manageAircraft(Aircraft&);
void manageAircraft();
void searchAircraft();

void manageCrew(Crew&);
void manageCrew();
void searchCrew();

void manageFlights();
void manageFlights(Flight&);
void searchFlights();

void quit();

