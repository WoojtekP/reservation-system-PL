#ifndef STRUCTURES_H_INCLUDED
#define STRUCTURES_H_INCLUDED

#define DB_DEVICES "db_devices.dat"
#define DB_RESERVATIONS "db_reservations.dat"
#define PRIME 27011

#define MAX_BRAND_LEN 10
#define MAX_MODEL_LEN 15
#define MAX_DESCRIPTION_LEN 50
#define MAX_FNAME_LEN 10
#define MAX_LNAME_LEN 15

#define TERM_DAYS 150
#define RESERVATIONS_PER_DAY 20

#define START_DAY 12
#define START_MONTH 2
#define START_YEAR 2018
#define END_DAY 15
#define END_MONTH 6
#define END_YEAR 2018

typedef struct time_my {
    int year;
    int month;
    int day;
    int weekDay;
    int hours;
    int minutes;
} Time_my;

typedef struct device {
    int id;
    char type; //l - laptop(notebook), r - rzutnik(projector), p - prezenter(presentation remote), m - mikrofon(microphone)
    char brand[MAX_BRAND_LEN+1];
    char model[MAX_MODEL_LEN+1];
    char description[MAX_DESCRIPTION_LEN+1];
} Device;

typedef struct reservation {
    int id;
    int idDevice;
    char fname[MAX_FNAME_LEN+1];
    char lname [MAX_LNAME_LEN+1];
    bool wholeSession; //true - reservation involves whole semester
    Time_my start;
    Time_my finish;
} Reservation;

typedef struct device_properties {
    GtkWidget *type;
    GtkWidget *brand;
    GtkWidget *model;
    GtkWidget *description;
} Device_properties;

typedef struct reservation_properties {
    GtkWidget *type;
    GtkWidget *brand;
    GtkWidget *model;
    GtkWidget *startDate;
    GtkWidget *endDate;
    GtkWidget *startTime;
    GtkWidget *endTime;
    GtkWidget *fname;
    GtkWidget *lname;
    GtkWidget *wholeSession;
} Reservation_properties;

typedef struct combo_box_pair {
    GtkWidget *first;
    GtkWidget *second;
} Combo_box_pair;

typedef struct device_availability {
    GtkWidget *device;
    GtkWidget *startDate;
    GtkWidget *endDate;
    GtkWidget *startTime;
    GtkWidget *endTime;
} Device_availability;

typedef struct device_reservations {
    GtkWidget *type;
    GtkWidget *brand;
    GtkWidget *model;
    GtkWidget *grid;
    GtkWidget **newGrid;
} Device_reservations;

typedef struct person_reservations {
    GtkWidget *fname;
    GtkWidget *lname;
    GtkWidget *grid;
    GtkWidget **newGrid;
} Person_reservations;

typedef struct week_reservations {
    GtkWidget *combo;
    GtkWidget *grid;
    GtkWidget **newGrid;
} Week_reservations;
#endif // STRUCTURES_H_INCLUDED
