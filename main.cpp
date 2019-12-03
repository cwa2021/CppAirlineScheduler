/*  Connor Applegate
    ConnorApplegate@my.unt.edu
    This program is an extensive flight scheduling tool 
*/
#include "main.h"

const string WELCOME_MESSAGE = "Welcome to the Mean Green Airlines scheduling system";

const string MAIN_MENU_PROMPT = "Please indicate what you would like to do.";
const map<string, function<void()>>  MAIN_MENU {
  { "Manage Aircraft", bind<void()>(&manageAircraft) },
  { "Manage Crew",     bind<void()>(&manageCrew)     },
  { "Manage Flights",  bind<void()>(&manageFlights)  },
  { "Quit",            quit                          },
};

const string AIRCRAFT_MENU_PROMPT = "What operation would you like to perform on the collection of aircraft?";
const map<string, function<void()>>  AIRCRAFT_MENU {
  { "Add Aircraft",       bind(mem_fn<void(), AircraftCollection>(&AircraftCollection::add), ref(aircraftCollection)) },
  { "Search Aircraft",    searchAircraft                                                                              },
  { "Print All Aircraft", bind(mem_fn(&AircraftCollection::print), ref(aircraftCollection))                           },
  { "Back",               mainMenu                                                                                    },
};

const string EDIT_AIRCRAFT_MENU_PROMPT = "What operation would you like to perform on this aircraft?";
const map<string, function<void(Aircraft&)>>  EDIT_AIRCRAFT_MENU {
  { "Edit",                      mem_fn(&Aircraft::interactiveEdit)                                                                          },
  { "Delete",                    bind(mem_fn<bool(Aircraft&), AircraftCollection>(&AircraftCollection::remove), ref(aircraftCollection), _1) },
  { "Print",                     mem_fn(&Aircraft::print)                                                                                    },
  { "Print Assignment Schedule", bind(mem_fn<void(Aircraft&), FlightCollection>(&FlightCollection::print_all), ref(flightCollection), _1)    },
  { "Back",                      bind<void(Aircraft&)>(&manageAircraft, _1)                                                                  },
};

const string CREW_MENU_PROMPT = "What operation would you like to perform on the collection of crew members?";
const map<string, function<void()>>  CREW_MENU {
  { "Add Crew",       bind(mem_fn<bool(), CrewCollection>(&CrewCollection::add), ref(crewCollection)) },
  { "Search Crew",    searchCrew                                                                      },
  { "Print All Crew", bind(mem_fn(&CrewCollection::print), ref(crewCollection))                       },
  { "Back",           mainMenu                                                                        },
};

const string EDIT_CREW_MENU_PROMPT = "What operation would you like to perform on this crew?";
const map<string, function<void(Crew&)>>  EDIT_CREW_MENU {
  { "Edit",                      mem_fn(&Crew::interactiveEdit)                                                                           },
  { "Delete",                    bind(mem_fn<bool(const Crew&), CrewCollection>(&CrewCollection::remove), ref(crewCollection), _1)        },
  { "Print",                     mem_fn<void() const, Crew>(&Crew::print)                                                                 },
  { "Print Assignment Schedule", bind(mem_fn<void(Crew&), FlightCollection>(&FlightCollection::print_all), ref(flightCollection), _1)     },
  { "Back",                      bind<void(Crew&)>(&manageCrew, _1)                                                                       },
};

const string FLIGHT_MENU_PROMPT = "What operation would you like to perform on the collection of flights?";
const map<string, function<void()>>  FLIGHT_MENU {
  { "Add Flight",                         bind(mem_fn<bool(), FlightCollection>(&FlightCollection::add), ref(flightCollection))                                                 },
  { "Search Flights",                     searchFlights                                                                                                                         },
  { "Print Active Flights",               bind(mem_fn<void(Flight::Status&), FlightCollection>(&FlightCollection::print_all), ref(flightCollection), Flight::Status::ACTIVE)    },
  { "Print Completed Flights",            bind(mem_fn<void(Flight::Status&), FlightCollection>(&FlightCollection::print_all), ref(flightCollection), Flight::Status::COMPLETE)  },
  { "Print Cancelled Flights",            bind(mem_fn<void(Flight::Status&), FlightCollection>(&FlightCollection::print_all), ref(flightCollection), Flight::Status::CANCELLED) },
  { "Print All Flights",                  bind(mem_fn(&FlightCollection::print), ref(flightCollection))                                                                         },
  { "Mark Finished Flights Completed",    bind(mem_fn(&FlightCollection::updateCompletedFlights), ref(flightCollection))                                                        },
  { "Remove Cancelled/Completed Flights", bind(mem_fn(&FlightCollection::removeCancelledAndCompleted), ref(flightCollection))                                                   },
  { "Back",                               mainMenu                                                                                                                              },
};

const string EDIT_FLIGHT_MENU_PROMPT = "What operation would you like to perform on this flight?";
const map<string, function<void(Flight&)>>  EDIT_FLIGHT_MENU {
  { "Edit",     mem_fn(&Flight::interactiveEdit)                                                                    },
  { "Delete",   bind(mem_fn<bool(Flight&), FlightCollection>(&FlightCollection::remove), ref(flightCollection), _1) },
  { "Print",    mem_fn(&Flight::print)                                                                              },
  { "Back",     bind<void(Flight&)>(&manageFlights, _1)                                                             },
};


int main() {
  aircraftCollection.load();
  crewCollection.load();
  flightCollection.load();
  cout << WELCOME_MESSAGE << endl;
  while (true) mainMenu();
}

void mainMenu() {
    const function<void()> &choice = Helper::promptMenu(MAIN_MENU, MAIN_MENU_PROMPT);
    choice();
}

void manageAircraft(Aircraft&) {
  manageAircraft();
}

void manageAircraft() {
    const function<void()> &choice = Helper::promptMenu(AIRCRAFT_MENU, AIRCRAFT_MENU_PROMPT);
    choice();
}

void searchAircraft() {
  string tailNum = Helper::promptValue<string>("Enter the tail number of the aircraft you would like to look up");
  auto aircraftIt = aircraftCollection.find(tailNum);
  if (aircraftCollection.isEnd(aircraftIt)) {
    cout << "Aircraft not found." << endl;
    return;
  }
  Aircraft &aircraft = *aircraftIt;

  cout << "Aircraft with tail number " << tailNum << " found." << endl;
  function<void(Aircraft&)> aircraftOperation = Helper::promptMenu(EDIT_AIRCRAFT_MENU, EDIT_AIRCRAFT_MENU_PROMPT);
  aircraftOperation(aircraft);
}

void manageCrew() { 
  const function<void()> &choice = Helper::promptMenu(CREW_MENU, CREW_MENU_PROMPT);
  choice();
}

void manageCrew(Crew &) {
  manageCrew();
}

void searchCrew() { 
  int id = Helper::promptValue<int>("Enter the ID of the employee you would like to look up");
  auto crewIt = crewCollection.find(id);
  if (crewCollection.isEnd(crewIt)) {
    cout << "Crew member not found." << endl;
    return;
  }
  Crew &crew = **crewIt;

  cout << "Crew member with ID # " << id << " found." << endl;
  function<void(Crew&)> crewOperation = Helper::promptMenu(EDIT_CREW_MENU, EDIT_CREW_MENU_PROMPT);
  crewOperation(crew);
}

void manageFlights() {
  const function<void()> &choice = Helper::promptMenu(FLIGHT_MENU, FLIGHT_MENU_PROMPT);
  choice();
}

void manageFlights(Flight&) {
  manageFlights();
}

void searchFlights() { 
  int flightNum = Helper::promptValue<int>("Enter the number you would like to look up.");
  auto flightIt = flightCollection.find(flightNum);
  if (flightCollection.isEnd(flightIt)) {
    cout << "Flight not found." << endl;
    return;
  }
  Flight &flight = *flightIt;

  cout << "Flight with number " << flightNum << " found." << endl;
  function<void(Flight&)> flightOperation = Helper::promptMenu(EDIT_FLIGHT_MENU, EDIT_FLIGHT_MENU_PROMPT);
  flightOperation(flight);
}

void quit() {
  aircraftCollection.save();
  crewCollection.save();
  flightCollection.save();
  exit(0);
}
