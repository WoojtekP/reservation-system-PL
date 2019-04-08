#ifndef DATABASE_H_INCLUDED
#define DATABASE_H_INCLUDED

#include "common.h"

int hash(char *nazwa);
FILE *openDatabase(char *name); //open database with given name
void closeDatabase(FILE *f); //open database with given name
void verifyDeviceInsertion(GtkWidget *wid, Device_properties *properties); //verify input given during adding device and process them
void verifyDeviceDelete(GtkWidget *wid, Device_properties *properties); //verify input given during deleting device and process them
void verifyReservationInsertion(GtkWidget *wid, Reservation_properties *properties); //verify if reservation may be added into database and show a prompt
void verifyDeviceAvailability(GtkWidget *wid, Device_availability *properties); //verify if device is available in given period
bool insertDatbaseDevice(char type, const char *brand, const char *model, const char *description); //insert info about new device into database
bool deleteDatbaseDevice(int id);
void deleteDatabaseReservation(GtkWidget *wid, GtkWidget *combo);
bool insertDatabaseReservation(const char *fname, const char *lname, int id, bool all, Time_my from, Time_my to);
bool isDeviceAvailable(int id, Time_my form, Time_my to, bool session);
int selectDatabaseDeviceId(char type, const char *brand, const char *model); //find device id in database
int selectDatabaseReservationId(const char *fname, const char *lname, char type, const char *brand, const char *model, Time_my from, Time_my to); //find  reservation id in database
int generateReservationId( Time_my date, bool all);
Device selectDatabaseDeviceFromId(int id);
#endif // DATABASE_H_INCLUDED
