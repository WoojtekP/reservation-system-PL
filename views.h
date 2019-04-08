#ifndef VIEWS_H_INCLUDED
#define VIEWS_H_INCLUDED

#define ROWS 20

#include "common.h"

typedef struct list {
    char text[50];
    struct list *next;
} List;

GtkWidget *window; //main window of the program
bool isChangePossible;

char* getFilePath(); //open file manager and read path for new file
void changeView (GtkWidget *wid, GtkGrid *grid); //remove current grid and add new one to the window
void showDialog(char *text); //show a prompt with given text
void createDevicesComboBoxList(GtkWidget *window, GtkWidget *combo);
void createBrandComboBoxList(GtkWidget *window, Combo_box_pair *combo);
void createModelComboBoxList(GtkWidget *window, Combo_box_pair *combo);
void createDeviceReservationsList(GtkWidget *window, Device_reservations *properties);
void createPersonReservationList(GtkWidget *window, Person_reservations *properties);
void createWeekReservationList(GtkWidget *window, Week_reservations *properties);
void generateWeekList(GtkWidget *combo); //generate combo box with all weekend in current semester
void attachReservations(GtkWidget *window, Person_reservations *properties); //show reservations for given person
void generateWeekReservationList(GtkWidget *window, GtkWidget *combo); //generate latex with reservations list
void add(List **l, char *s);
bool isEmpty(List *l);
void pop(List **l);

#endif // VIEWS_H_INCLUDED
