#include "common.h"

int main (int argc, char *argv[])
{
    bool errorDialog = false;
    firstSemesterDay = getTimeStruct(START_YEAR, START_MONTH, START_DAY);
    lastSemesterDay = getTimeStruct(END_YEAR, END_MONTH, END_DAY);
    if (argc == 7){
        Time_my start, end;
        start.day = atoi(argv[1]);
        start.month = atoi(argv[2]);
        start.year = atoi(argv[3]);
        end.day = atoi(argv[4]);
        end.month = atoi(argv[5]);
        end.year = atoi(argv[6]);
        if (verifyTimeInput(start, end)){
            firstSemesterDay = start;
            lastSemesterDay = end;
        }
        else
            errorDialog = true;
    }

    gtk_init(&argc, &argv);
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    gtk_window_set_title(GTK_WINDOW(window), "System rezerwacji");
    gtk_window_set_default_size(GTK_WINDOW(window), 800, 600);
    gtk_window_set_position(GTK_WINDOW(window),GTK_WIN_POS_CENTER);
    gtk_container_set_border_width(GTK_CONTAINER(window), 20);

    GtkWidget *gridMenu, *gridCheck, *gridWeek, *gridDelete, *gridDeviceReservations, *gridPersonReservations, *gridReservationsDeviceList, *gridReservationsPersonList, *gridReservationsWeek, *gridReservation, *gridAddDevice;
    GtkWidget *label;
    GtkWidget *button, *separator, *buttonWholeSession, *calendarFrom, *calendarTo, *calendarFromCheck, *calendarToCheck;
    GtkWidget *entryBrandRes, *entryModelRes, *entryFnameRes, *entryLnameRes, *timeFrom, *timeTo, *timeFromCheck, *timeToCheck, *entryFnameDevices, *entryLnameDevices, *entryBrand, *entryModel, *entryDescription, *entryFnameDelete, *entryLnameDelete;
    GtkWidget *comboTypeRes, *comboType, *comboDevicesList, *comboDeviceType, *comboDeviceBrand, *comboDeviceModel, *comboBoxWeek, *comboReservations;
    char text[26];

    //MAIN MENU
    gridMenu = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(gridMenu), 25);
    gtk_grid_set_row_homogeneous(GTK_GRID(gridMenu), TRUE);
    gtk_grid_set_column_spacing(GTK_GRID(gridMenu), 25);
    gtk_grid_set_column_homogeneous(GTK_GRID(gridMenu), TRUE);

    //ADD RESERVATION
    gridReservation = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(gridReservation), 5);
    gtk_grid_set_row_homogeneous(GTK_GRID(gridReservation), TRUE);
    gtk_grid_set_column_spacing(GTK_GRID(gridReservation), 20);
    gtk_grid_set_column_homogeneous(GTK_GRID(gridReservation), TRUE);

    //CHECK AVAILABILITY
    gridCheck = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(gridCheck), 20);
    gtk_grid_set_row_homogeneous(GTK_GRID(gridCheck), TRUE);
    gtk_grid_set_column_spacing(GTK_GRID(gridCheck), 10);
    gtk_grid_set_column_homogeneous(GTK_GRID(gridCheck), TRUE);

    //ADD DEVICE
    gridAddDevice = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(gridAddDevice), 25);
    gtk_grid_set_row_homogeneous(GTK_GRID(gridAddDevice), TRUE);
    gtk_grid_set_column_spacing(GTK_GRID(gridAddDevice), 25);
    gtk_grid_set_column_homogeneous(GTK_GRID(gridAddDevice), TRUE);

    //CHECK RESERVATIONS (DEVICE)
    gridDeviceReservations = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(gridDeviceReservations), 5);
    gtk_grid_set_row_homogeneous(GTK_GRID(gridDeviceReservations), FALSE);
    gtk_grid_set_column_spacing(GTK_GRID(gridDeviceReservations), 10);
    gtk_grid_set_column_homogeneous(GTK_GRID(gridDeviceReservations), TRUE);

    //CHECK RESERVATIONS (PERSON)
    gridPersonReservations = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(gridPersonReservations), 5);
    gtk_grid_set_row_homogeneous(GTK_GRID(gridPersonReservations), FALSE);
    gtk_grid_set_column_spacing(GTK_GRID(gridPersonReservations), 25);
    gtk_grid_set_column_homogeneous(GTK_GRID(gridPersonReservations), TRUE);

    //GENERATE RESERVATIONS LIST
    gridWeek = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(gridWeek), 5);
    gtk_grid_set_row_homogeneous(GTK_GRID(gridWeek), FALSE);
    gtk_grid_set_column_spacing(GTK_GRID(gridWeek), 25);
    gtk_grid_set_column_homogeneous(GTK_GRID(gridWeek), TRUE);

    //REMOVE RESERVATION
    gridDelete = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(gridDelete), 5);
    gtk_grid_set_row_homogeneous(GTK_GRID(gridDelete), FALSE);
    gtk_grid_set_column_spacing(GTK_GRID(gridDelete), 25);
    gtk_grid_set_column_homogeneous(GTK_GRID(gridDelete), TRUE);

    //GRID - MAIN MENU
    if (firstSemesterDay.year != lastSemesterDay.year)
        sprintf(text, "%s %d/%d", "SEMESTR ZIMOWY", firstSemesterDay.year, lastSemesterDay.year);
    else
        sprintf(text, "%s %d", "SEMESTR LETNI", firstSemesterDay.year);
    label = gtk_label_new(text);
    gtk_grid_attach(GTK_GRID(gridMenu), GTK_WIDGET(label), 0, 0, 2, 1);
    label = gtk_label_new_with_mnemonic("MENU GŁÓWNE");
    gtk_grid_attach(GTK_GRID(gridMenu), GTK_WIDGET(label), 0, 1, 2, 1);
    button = gtk_button_new_with_label("REZERWUJ");
        //book -> gridReservation
        g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(changeView), gridReservation);

    gtk_grid_attach(GTK_GRID(gridMenu), button, 0, 2, 1, 2);
    button = gtk_button_new_with_label("SPRAWDŹ DOSTĘPNOŚĆ");
    gtk_grid_attach(GTK_GRID(gridMenu), button, 1, 2, 1, 2);
        //checking availability -> gridCheck
            comboDevicesList = gtk_combo_box_text_new();
        g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(createDevicesComboBoxList), comboDevicesList);
        g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(changeView), gridCheck);

    button = gtk_button_new_with_label("USUŃ REZERWACJĘ");
    gtk_grid_attach(GTK_GRID(gridMenu), button, 0, 4, 1, 2);
        g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(changeView), gridDelete);
    button = gtk_button_new_with_label("POKAŻ REZERWACJE URZĄRZENIA");
    gtk_grid_attach(GTK_GRID(gridMenu), button, 1, 4, 1, 2);
        g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(changeView), gridDeviceReservations);
    button = gtk_button_new_with_label("DODAJ/USUŃ URZĄDZENIE");
    gtk_grid_attach(GTK_GRID(gridMenu), button, 0, 6, 1, 2);
        g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(changeView), gridAddDevice);
    button = gtk_button_new_with_label("POKAŻ REZERWACJE OSOBY");
    gtk_grid_attach(GTK_GRID(gridMenu), button, 1, 6, 1, 2);
        g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(changeView), gridPersonReservations);
    button = gtk_button_new_with_label("DRUKUJ ROZKŁAD TYGODNIA");
    gtk_grid_attach(GTK_GRID(gridMenu), button, 0, 8, 1, 2);
        g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(changeView), gridWeek);

    //GRID RESERVATION - book device
    label = gtk_label_new("REZERWACJA URZĄDZENIA");
    gtk_grid_attach(GTK_GRID(gridReservation), GTK_WIDGET(label), 0, 0, 8, 2);
    label = gtk_label_new("typ urządzenia:");
    gtk_grid_attach(GTK_GRID(gridReservation), GTK_WIDGET(label), 0, 2, 4, 2);
    label = gtk_label_new("marka:");
    gtk_grid_attach(GTK_GRID(gridReservation), GTK_WIDGET(label), 0, 4, 4, 2);
    label = gtk_label_new("model:");
    gtk_grid_attach(GTK_GRID(gridReservation), GTK_WIDGET(label), 0, 6, 4, 2);
    sprintf(text, "imię (do %d znaków):", MAX_FNAME_LEN);
    label = gtk_label_new(text);
    gtk_grid_attach(GTK_GRID(gridReservation), GTK_WIDGET(label), 0, 8, 4, 2);
    sprintf(text, "nazwisko (do %d znaków):", MAX_LNAME_LEN);
    label = gtk_label_new(text);
    gtk_grid_attach(GTK_GRID(gridReservation), GTK_WIDGET(label), 0, 10, 4, 2);

    comboTypeRes = gtk_combo_box_text_new();
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(comboTypeRes), "laptop" );
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(comboTypeRes), "rzutnik" );
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(comboTypeRes), "prezenter" );
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(comboTypeRes), "mikrofon" );
    gtk_grid_attach(GTK_GRID(gridReservation), GTK_WIDGET(comboTypeRes), 4, 2, 4, 2);
    entryBrandRes = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(gridReservation), GTK_WIDGET(entryBrandRes), 4, 4, 4, 2);
    entryModelRes = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(gridReservation), GTK_WIDGET(entryModelRes), 4, 6, 4, 2);
    entryFnameRes = gtk_entry_new();
    gtk_entry_set_max_length(GTK_ENTRY(entryFnameRes), MAX_FNAME_LEN);
    gtk_grid_attach(GTK_GRID(gridReservation), GTK_WIDGET(entryFnameRes), 4, 8, 4, 2);
    entryLnameRes = gtk_entry_new();
    gtk_entry_set_max_length(GTK_ENTRY(entryLnameRes), MAX_LNAME_LEN);
    gtk_grid_attach(GTK_GRID(gridReservation), GTK_WIDGET(entryLnameRes), 4, 10, 4, 2);

    buttonWholeSession = gtk_check_button_new_with_label("cały semestr");
    gtk_grid_attach(GTK_GRID(gridReservation), GTK_WIDGET(buttonWholeSession), 6, 14, 2, 2);

    label = gtk_label_new("od");
    gtk_grid_attach(GTK_GRID(gridReservation), GTK_WIDGET(label), 0, 14, 1, 2);
    label = gtk_label_new("do");
    gtk_grid_attach(GTK_GRID(gridReservation), GTK_WIDGET(label), 4, 14, 1, 2);

    calendarFrom = gtk_calendar_new();
    gtk_grid_attach(GTK_GRID(gridReservation), GTK_WIDGET(calendarFrom), 0, 16, 4, 11);
    calendarTo = gtk_calendar_new();
    gtk_grid_attach(GTK_GRID(gridReservation), GTK_WIDGET(calendarTo), 4, 16, 4, 11);

    label = gtk_label_new("godzina (hh:mm):");
    gtk_grid_attach(GTK_GRID(gridReservation), GTK_WIDGET(label), 0, 27, 2, 2);

    timeFrom = gtk_entry_new();
    gtk_entry_set_text(GTK_ENTRY(timeFrom), "00:00");
    gtk_entry_set_max_length(GTK_ENTRY(timeFrom), 5);
    gtk_grid_attach(GTK_GRID(gridReservation), GTK_WIDGET(timeFrom), 2, 27, 2, 2);

    label = gtk_label_new("godzina (hh:mm):");
    gtk_grid_attach(GTK_GRID(gridReservation), GTK_WIDGET(label), 4, 27, 2, 2);

    timeTo = gtk_entry_new();
    gtk_entry_set_text(GTK_ENTRY(timeTo), "00:00");
    gtk_entry_set_max_length(GTK_ENTRY(timeTo), 5);
    gtk_grid_attach(GTK_GRID(gridReservation), GTK_WIDGET(timeTo), 6, 27, 2, 2);

    button = gtk_button_new_with_label("ANULUJ");
        //cancel -> gridMenu (main menu)
        g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(changeView), gridMenu);

    gtk_grid_attach(GTK_GRID(gridReservation), button, 0, 29, 4, 4);
    button = gtk_button_new_with_label("ZATWIERDŹ");
    gtk_grid_attach(GTK_GRID(gridReservation), button, 4, 29, 4, 4);

    Reservation_properties *addReservationData = malloc(sizeof(Reservation_properties));
    addReservationData->brand = entryBrandRes;
    addReservationData->type = comboTypeRes;
    addReservationData->model = entryModelRes;
    addReservationData->startDate = calendarFrom;
    addReservationData->endDate = calendarTo;
    addReservationData->startTime = timeFrom;
    addReservationData->endTime = timeTo;
    addReservationData->fname = entryFnameRes;
    addReservationData->lname = entryLnameRes;
    addReservationData->wholeSession = buttonWholeSession;

        g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(verifyReservationInsertion), addReservationData);

    //GRID DELETE - delete reservation
    label = gtk_label_new("USUWANIE REZERWACJI");
    gtk_grid_attach(GTK_GRID(gridDelete), GTK_WIDGET(label), 0, 0, 6, 2);
    label = gtk_label_new("imię:");
    gtk_widget_set_size_request(GTK_WIDGET(label), 0, 40);
    gtk_grid_attach(GTK_GRID(gridDelete), GTK_WIDGET(label), 0, 2, 2, 2);
    label = gtk_label_new("nazwisko:");
    gtk_widget_set_size_request(GTK_WIDGET(label), 0, 40);
    gtk_grid_attach(GTK_GRID(gridDelete), GTK_WIDGET(label), 0, 4, 2, 2);
    entryFnameDelete = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(gridDelete), GTK_WIDGET(entryFnameDelete), 2, 2, 2, 2);
    entryLnameDelete = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(gridDelete), GTK_WIDGET(entryLnameDelete), 2, 4, 2, 2);
    button = gtk_button_new_with_label("WRÓĆ");
    gtk_grid_attach(GTK_GRID(gridDelete), button, 4, 2, 2, 2);
        g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(changeView), gridMenu);
    button = gtk_button_new_with_label("POKAŻ REZERWACJE");
    gtk_grid_attach(GTK_GRID(gridDelete), button, 4, 4, 2, 2);
        comboReservations = gtk_combo_box_text_new();
    Person_reservations *readReservations = malloc(sizeof(Person_reservations));
    readReservations->fname = entryFnameDelete;
    readReservations->lname = entryLnameDelete;
    readReservations->grid = comboReservations;
        g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(attachReservations), readReservations);
    separator = gtk_separator_new(GTK_ORIENTATION_HORIZONTAL);
    gtk_grid_attach(GTK_GRID(gridDelete), separator, 0, 6, 6, 1);
    gtk_widget_set_size_request(comboReservations, 0, 40);
    gtk_grid_attach(GTK_GRID(GTK_GRID(gridDelete)), comboReservations, 1, 7, 4, 2);
    button = gtk_button_new_with_label("USUŃ");
    gtk_widget_set_size_request(button, 0, 40);
    gtk_grid_attach(GTK_GRID(GTK_GRID(gridDelete)), button, 1, 9, 4, 2);
        g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(deleteDatabaseReservation), comboReservations);
        g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(attachReservations), readReservations);

    //GRID CHECK - checking device for availability
    label = gtk_label_new("SPRAWDŹ DOSTĘPNOŚĆ URZĄDZENIA");
    gtk_grid_attach(GTK_GRID(gridCheck), GTK_WIDGET(label), 0, 0, 8, 1);
    label = gtk_label_new("wybierz\nurządzenie:");
    gtk_grid_attach(GTK_GRID(gridCheck), GTK_WIDGET(label), 0, 1, 1, 2);
    gtk_grid_attach(GTK_GRID(gridCheck), GTK_WIDGET(comboDevicesList), 1, 1, 6, 2);

    label = gtk_label_new("od");
    gtk_grid_attach(GTK_GRID(gridCheck), GTK_WIDGET(label), 0, 3, 1, 1);
    label = gtk_label_new("do");
    gtk_grid_attach(GTK_GRID(gridCheck), GTK_WIDGET(label), 4, 3, 1, 1);

    calendarFromCheck = gtk_calendar_new();
    gtk_grid_attach(GTK_GRID(gridCheck), GTK_WIDGET(calendarFromCheck), 0, 4, 4, 4);
    calendarToCheck = gtk_calendar_new();
    gtk_grid_attach(GTK_GRID(gridCheck), GTK_WIDGET(calendarToCheck), 4, 4, 4, 4);

    label = gtk_label_new("godzina (hh:mm):");
    gtk_grid_attach(GTK_GRID(gridCheck), GTK_WIDGET(label), 0, 8, 2, 2);

    timeFromCheck = gtk_entry_new();
    gtk_entry_set_text(GTK_ENTRY(timeFromCheck), "00:00");
    gtk_entry_set_max_length(GTK_ENTRY(timeFromCheck), 5);
    gtk_grid_attach(GTK_GRID(gridCheck), GTK_WIDGET(timeFromCheck), 2, 8, 2, 2);

    label = gtk_label_new("godzina (hh:mm):");
    gtk_grid_attach(GTK_GRID(gridCheck), GTK_WIDGET(label), 4, 8, 2, 2);

    timeToCheck = gtk_entry_new();
    gtk_entry_set_text(GTK_ENTRY(timeToCheck), "00:00");
    gtk_entry_set_max_length(GTK_ENTRY(timeToCheck), 5);
    gtk_grid_attach(GTK_GRID(gridCheck), GTK_WIDGET(timeToCheck), 6, 8, 2, 2);

    button = gtk_button_new_with_label("WRÓĆ");
        g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(changeView), gridMenu);
    gtk_grid_attach(GTK_GRID(gridCheck), GTK_WIDGET(button), 0, 10, 4, 2);

    button = gtk_button_new_with_label("SPRAWDŹ");
    gtk_grid_attach(GTK_GRID(GTK_GRID(gridCheck)), GTK_WIDGET(button), 4, 10, 4, 2);
    Device_availability *checkAvailability = malloc(sizeof(Device_availability));
    checkAvailability->device = comboDevicesList;
    checkAvailability->startDate = calendarFromCheck;
    checkAvailability->endDate = calendarToCheck;
    checkAvailability->startTime = timeFromCheck;
    checkAvailability->endTime = timeToCheck;
        g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(verifyDeviceAvailability), checkAvailability);

    //GRID ADD DEVICE - add/remove device
    label = gtk_label_new("DODAWANIE/USUWANIE NOWEGO URZĄDZENIA");
    gtk_grid_attach(GTK_GRID(gridAddDevice), GTK_WIDGET(label), 0, 0, 6, 1);
    label = gtk_label_new("typ urządzenia:");
    gtk_grid_attach(GTK_GRID(gridAddDevice), GTK_WIDGET(label), 0, 1, 3, 1);
    sprintf(text, "marka (do %d znaków):", MAX_BRAND_LEN);
    label = gtk_label_new(text);
    gtk_grid_attach(GTK_GRID(gridAddDevice), GTK_WIDGET(label), 0, 2, 3, 1);
    sprintf(text, "model (do %d znaków):", MAX_MODEL_LEN);
    label = gtk_label_new(text);
    gtk_grid_attach(GTK_GRID(gridAddDevice), GTK_WIDGET(label), 0, 3, 3, 1);
    sprintf(text, "opis (do %d znaków)", MAX_DESCRIPTION_LEN);
    label = gtk_label_new(text);
    gtk_grid_attach(GTK_GRID(gridAddDevice), GTK_WIDGET(label), 0, 4, 3, 1);

    comboType = gtk_combo_box_text_new();
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(comboType), "laptop");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(comboType), "rzutnik");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(comboType), "prezenter");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(comboType), "mikrofon");
    gtk_grid_attach(GTK_GRID(GTK_GRID(gridAddDevice)), GTK_WIDGET(comboType), 3, 1, 3, 1);

    entryBrand = gtk_entry_new();
    gtk_entry_set_max_length(GTK_ENTRY(entryBrand), MAX_BRAND_LEN);
    gtk_grid_attach(GTK_GRID(GTK_GRID(gridAddDevice)), GTK_WIDGET(entryBrand), 3, 2, 3, 1);

    entryModel = gtk_entry_new();
    gtk_entry_set_max_length(GTK_ENTRY(entryModel), MAX_MODEL_LEN);
    gtk_grid_attach(GTK_GRID(gridAddDevice), GTK_WIDGET(entryModel), 3, 3, 3, 1);

    entryDescription = gtk_entry_new();
    gtk_entry_set_max_length(GTK_ENTRY(entryDescription), MAX_DESCRIPTION_LEN);
    gtk_grid_attach(GTK_GRID(gridAddDevice), GTK_WIDGET(entryDescription), 3, 4, 3, 1);

    button = gtk_button_new_with_label("WRÓĆ");
        g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(changeView), gridMenu);

    gtk_grid_attach(GTK_GRID(gridAddDevice), GTK_WIDGET(button), 0, 5, 2, 1);
    button = gtk_button_new_with_label("DODAJ");
    gtk_grid_attach(GTK_GRID(gridAddDevice), GTK_WIDGET(button), 2, 5, 2, 1);

    Device_properties *addDeviceData = malloc(sizeof(Device_properties));
    addDeviceData->type = comboType;
    addDeviceData->brand = entryBrand;
    addDeviceData->model = entryModel;
    addDeviceData->description = entryDescription;
        g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(verifyDeviceInsertion), addDeviceData);

    button = gtk_button_new_with_label("USUŃ");
    gtk_grid_attach(GTK_GRID(gridAddDevice), GTK_WIDGET(button), 4, 5, 2, 1);
        g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(verifyDeviceDelete), addDeviceData);

    //GRID DEVICE RESERVATION - list of booked devices
    label = gtk_label_new("rodzaj");
    gtk_grid_attach(GTK_GRID(gridDeviceReservations), GTK_WIDGET(label), 0, 0, 2, 2);
    label = gtk_label_new("marka");
    gtk_grid_attach(GTK_GRID(gridDeviceReservations), GTK_WIDGET(label), 0, 2, 2, 2);
    label = gtk_label_new("model");
    gtk_grid_attach(GTK_GRID(gridDeviceReservations), GTK_WIDGET(label), 0, 4, 2, 2);
    comboDeviceType = gtk_combo_box_text_new();
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(comboDeviceType), "laptop");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(comboDeviceType), "rzutnik");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(comboDeviceType), "prezenter");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(comboDeviceType), "mikrofon");
    gtk_grid_attach(GTK_GRID(gridDeviceReservations), GTK_WIDGET(comboDeviceType), 2, 0, 2, 2);
    comboDeviceBrand = gtk_combo_box_text_new();
    gtk_grid_attach(GTK_GRID(gridDeviceReservations), GTK_WIDGET(comboDeviceBrand), 2, 2, 2, 2);
    comboDeviceModel = gtk_combo_box_text_new();
    gtk_grid_attach(GTK_GRID(gridDeviceReservations), GTK_WIDGET(comboDeviceModel), 2, 4, 2, 2);
    button = gtk_button_new_with_label("SPRAWDŹ");
    gtk_grid_attach(GTK_GRID(gridDeviceReservations), GTK_WIDGET(button), 4, 0, 2, 3);
    separator = gtk_separator_new(GTK_ORIENTATION_HORIZONTAL);
    gtk_grid_attach(GTK_GRID(gridDeviceReservations), GTK_WIDGET(separator), 0, 6, 6, 1);
    gtk_widget_set_size_request(GTK_WIDGET(comboDeviceType), 0, 30);
    gtk_widget_set_size_request(GTK_WIDGET(comboDeviceBrand), 0, 30);
    gtk_widget_set_size_request(GTK_WIDGET(comboDeviceModel), 0, 30);

    isChangePossible = true;
    Device_reservations *deviceListRes = malloc(sizeof(Device_reservations));
    deviceListRes->type = comboDeviceType;
    deviceListRes->brand = comboDeviceBrand;
    deviceListRes->model = comboDeviceModel;
    deviceListRes->grid = gridDeviceReservations;
    gridReservationsDeviceList = NULL;
    deviceListRes->newGrid = &gridReservationsDeviceList;

    Combo_box_pair *comboBoxesBrand = malloc(sizeof(Combo_box_pair));
    comboBoxesBrand->first = comboDeviceBrand;
    comboBoxesBrand->second = comboDeviceModel;

    Combo_box_pair *comboBoxesModel = malloc(sizeof(Combo_box_pair));
    comboBoxesModel->first = comboDeviceType;
    comboBoxesModel->second = comboDeviceModel;

        g_signal_connect(G_OBJECT(comboDeviceType), "changed", G_CALLBACK(createBrandComboBoxList), comboBoxesBrand);
        g_signal_connect(G_OBJECT(comboDeviceBrand), "changed", G_CALLBACK(createModelComboBoxList), comboBoxesModel);
        g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(createDeviceReservationsList), deviceListRes);

    button = gtk_button_new_with_label("WRÓĆ");
    gtk_grid_attach(GTK_GRID(gridDeviceReservations), GTK_WIDGET(button), 4, 3, 2, 3);
        g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(changeView), gridMenu);

    //GRID PERSON RESERVATIONS
    label = gtk_label_new("imię:");
    gtk_grid_attach(GTK_GRID(gridPersonReservations), GTK_WIDGET(label), 0, 0, 2, 3);
    label = gtk_label_new("nazwisko");
    gtk_grid_attach(GTK_GRID(gridPersonReservations), GTK_WIDGET(label), 0, 3, 2, 3);
    entryFnameDevices = gtk_entry_new();
    gtk_entry_set_max_length(GTK_ENTRY(entryFnameDevices), MAX_FNAME_LEN);
    gtk_grid_attach(GTK_GRID(gridPersonReservations), GTK_WIDGET(entryFnameDevices), 2, 0, 2, 3);
    entryLnameDevices = gtk_entry_new();
    gtk_entry_set_max_length(GTK_ENTRY(entryLnameDevices), MAX_FNAME_LEN);
    gtk_grid_attach(GTK_GRID(gridPersonReservations), GTK_WIDGET(entryLnameDevices), 2, 3, 2, 3);
    button = gtk_button_new_with_label("SPRAWDŹ");
    gtk_grid_attach(GTK_GRID(gridPersonReservations), GTK_WIDGET(button), 4, 0, 2, 3);
    separator = gtk_separator_new(GTK_ORIENTATION_HORIZONTAL);
    gtk_grid_attach(GTK_GRID(gridPersonReservations), GTK_WIDGET(separator), 0, 6, 6, 1);

    Person_reservations *entryPair = malloc(sizeof(Person_reservations));
    entryPair->fname = entryFnameDevices;
    entryPair->lname = entryLnameDevices;
    entryPair->grid = gridPersonReservations;
    gridReservationsPersonList = NULL;
    entryPair->newGrid = &gridReservationsPersonList;
        g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(createPersonReservationList), entryPair);

    button = gtk_button_new_with_label("WRÓĆ");
    gtk_grid_attach(GTK_GRID(gridPersonReservations), GTK_WIDGET(button), 4, 3, 2, 3);
        g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(changeView), gridMenu);

    //GRID WEEK - list of reservations in given week
    label = gtk_label_new("wybierz tydzień z listy:");
    gtk_grid_attach(GTK_GRID(gridWeek), GTK_WIDGET(label), 0, 0, 2, 6);
    comboBoxWeek = gtk_combo_box_text_new();
    generateWeekList(comboBoxWeek);
    gtk_grid_attach(GTK_GRID(gridWeek), GTK_WIDGET(comboBoxWeek), 2, 2, 2, 2);
    button = gtk_button_new_with_label("WRÓĆ");
    gtk_grid_attach(GTK_GRID(gridWeek), GTK_WIDGET(button), 4, 0, 2, 2);
        g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(changeView), gridMenu);

    gridReservationsWeek = NULL;
    Week_reservations *weekResList = malloc(sizeof(Week_reservations));
    weekResList->combo = comboBoxWeek;
    weekResList->grid = gridWeek;
    weekResList->newGrid = &gridReservationsWeek;

    button = gtk_button_new_with_label("WYŚWIETL");
    gtk_grid_attach(GTK_GRID(gridWeek), GTK_WIDGET(button), 4, 2, 2, 2);
        g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(createWeekReservationList), weekResList);
    button = gtk_button_new_with_label("GENERUJ");
    gtk_grid_attach(GTK_GRID(gridWeek), GTK_WIDGET(button), 4, 4, 2, 2);
        g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(generateWeekReservationList), comboBoxWeek);
    separator = gtk_separator_new(GTK_ORIENTATION_HORIZONTAL);
    gtk_grid_attach(GTK_GRID(gridWeek), GTK_WIDGET(separator), 0, 6, 6, 1);

    gtk_container_add(GTK_CONTAINER(window), gridMenu);
    if (errorDialog)
        showDialog("Niestety nie udało się poprawnie wczytać parametrów kompilacji programu\nDomyślny semestr: lato 2018.");
    gtk_widget_show_all (window);
    gtk_main ();
	return 0;
}
