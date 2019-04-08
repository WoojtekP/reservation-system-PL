#include "views.h"

char* getFilePath(char *name)
{
    GtkWidget *dialog;
    GtkFileChooser *chooser;
    GtkFileChooserAction action = GTK_FILE_CHOOSER_ACTION_SAVE;
    gint res;
    dialog = gtk_file_chooser_dialog_new ("Save File", NULL, action, ("_Cancel"), GTK_RESPONSE_CANCEL, ("_Save"), GTK_RESPONSE_ACCEPT, NULL);
    chooser = GTK_FILE_CHOOSER (dialog);
    gtk_file_chooser_set_do_overwrite_confirmation (chooser, TRUE);
    gtk_file_chooser_set_current_name(chooser, name);
    res = gtk_dialog_run (GTK_DIALOG (dialog));
    char *filename = NULL;
    if (res == GTK_RESPONSE_ACCEPT){
        filename = gtk_file_chooser_get_filename (chooser);
    }
    gtk_widget_destroy (dialog);
    return filename;
}

void changeView (GtkWidget *wid, GtkGrid *grid)
{
    UNUSED(wid);
    GtkWidget *child = gtk_container_get_focus_child(GTK_CONTAINER(window));
    gtk_widget_hide(GTK_WIDGET(child));
    g_object_ref(GTK_WIDGET(child));
    gtk_container_remove(GTK_CONTAINER(window), GTK_WIDGET(child));
    gtk_container_add(GTK_CONTAINER(window), GTK_WIDGET(grid));
    gtk_container_set_focus_child(GTK_CONTAINER(window), GTK_WIDGET(grid));
    gtk_widget_show_all (window);
}

void showDialog(char *text)
{
    GtkWidget *dialog = gtk_message_dialog_new (GTK_WINDOW (window), GTK_DIALOG_MODAL, GTK_MESSAGE_INFO, GTK_BUTTONS_CLOSE, "%s", text);
    gtk_window_set_position (GTK_WINDOW (dialog), GTK_WIN_POS_CENTER);
    gtk_window_set_default_size(GTK_WINDOW(dialog), 400, 300);
    gtk_dialog_run (GTK_DIALOG (dialog));
    gtk_widget_destroy (dialog);
}

void createDevicesComboBoxList(GtkWidget *window, GtkWidget *combo)
{
    UNUSED(window);
    FILE *database = openDatabase("devices");
    if (database == NULL)
        return;
    gtk_combo_box_text_remove_all(GTK_COMBO_BOX_TEXT(combo));
    Device readDevice;
    char text[MAX_BRAND_LEN + MAX_MODEL_LEN + MAX_DESCRIPTION_LEN + 20];
    char *type;
    fseek(database, sizeof(Device), SEEK_SET);
    for (int i = 0; i < PRIME; i++){
        if (fread(&readDevice, sizeof(Device), 1, database) != 1){
            return;
        }
        if (readDevice.id){
            switch(readDevice.type){
                case 'l': type = "laptop"; break;
                case 'r': type = "rzutnik"; break;
                case 'p': type = "prezenter"; break;
                case 'm': type = "mikrofon"; break;
                default: type = '\0';
            }
            sprintf(text, "%d: %s %s %s", readDevice.id, type, readDevice.brand, readDevice.model);
            if (readDevice.description[0] != '\0'){
                sprintf(text, "%s\n\t(%s)", text,readDevice.description);
            }
            gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combo), text);
        }
    }
    closeDatabase(database);
}

void createBrandComboBoxList(GtkWidget *window, Combo_box_pair *combo)
{
    //first - brand (create list), second - model (erase)
    isChangePossible = false;
    gtk_combo_box_text_remove_all(GTK_COMBO_BOX_TEXT(combo->first));
    gtk_combo_box_text_remove_all(GTK_COMBO_BOX_TEXT(combo->second));
    char *text = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(window));
    if (text == NULL)
        return;
    char type = text[0];
    FILE *database = openDatabase("devices");
    if (database == NULL){
        showDialog("Błąd. Wystąpił problem z bazą danych");
        return;
    }
    Device readDevice;
    List *l = NULL;
    for (int i = 0; i < PRIME; i++){
        if (fread(&readDevice, sizeof(Device), 1, database) != 1)
            break;
        if (readDevice.type == type)
            add(&l, readDevice.brand);
    }
    while (!isEmpty(l)){
        gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combo->first), l->text);
        pop(&l);
    }
    closeDatabase(database);
    isChangePossible = true;
}

void createModelComboBoxList(GtkWidget *window, Combo_box_pair *combo)
{
    //first - type of device, second - model (create list)
    if (!isChangePossible)
        return;
    gtk_combo_box_text_remove_all(GTK_COMBO_BOX_TEXT(combo->second));
    char *text = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(combo->first));
    char type = text[0];
    char *brand = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(window));
    FILE *database = openDatabase("devices");
    if (database == NULL){
        showDialog("Błąd. Wystąpił problem z bazą danych");
        return;
    }
    Device readDevice;
    for (int i = 0; i < PRIME; i++){
        if (fread(&readDevice, sizeof(Device), 1, database) != 1)
            break;
        if (readDevice.type == type && strcmp(readDevice.brand, brand) == 0)
            gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combo->second), readDevice.model);
    }
    closeDatabase(database);
}

void createDeviceReservationsList(GtkWidget *window, Device_reservations *properties)
{
    UNUSED(window);
    char *typeText, *brand, *model, type;
    typeText = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(properties->type));
    brand = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(properties->brand));
    model = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(properties->model));
    if (brand == NULL || model == NULL || typeText == NULL){
        showDialog("Nie wybrano urządzenia.");
        return;
    }
    type = typeText[0];
    int id = selectDatabaseDeviceId(type, brand, model);
    FILE *database = openDatabase("reservations");
    if (database == NULL){
        showDialog("Błąd. Wystąpił problem z bazą danych");
        return;
    }
    if (*properties->newGrid != NULL)
        gtk_widget_destroy(GTK_WIDGET(*properties->newGrid));
    *properties->newGrid = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(*properties->newGrid), 5);
    gtk_grid_set_row_homogeneous(GTK_GRID(*properties->newGrid), FALSE);
    gtk_grid_set_column_spacing(GTK_GRID(*properties->newGrid), 20);
    gtk_grid_set_column_homogeneous(GTK_GRID(*properties->newGrid), TRUE);
    GtkWidget *label;
    int n = 0, rows = 18;
    Reservation readReservation;
    char text[150];
    for (int i = 0; i < RESERVATIONS_PER_DAY * TERM_DAYS; ++i){
        if (fread(&readReservation, sizeof(Reservation), 1, database) != 1)
            break;
        if (readReservation.idDevice == id){
            if (readReservation.wholeSession){
                sprintf(text, "cały semestr : %s %s", readReservation.fname, readReservation.lname);
                label = gtk_label_new(text);
                gtk_grid_attach(GTK_GRID(*properties->newGrid), GTK_WIDGET(label), n / rows, n % rows, 1, 1);
                break;
            }
            sprintf(text, "%2d/%02d/%4d %2d:%02d - %2d/%02d/%4d %2d:%02d : %s %s",
                readReservation.start.day, readReservation.start.month, readReservation.start.year,
                readReservation.start.hours, readReservation.start.minutes,
                readReservation.finish.day, readReservation.finish.month, readReservation.finish.year,
                readReservation.finish.hours, readReservation.finish.minutes,
                readReservation.fname, readReservation.lname);
            label = gtk_label_new(text);
            gtk_grid_attach(GTK_GRID(*properties->newGrid), label, n / rows, n % rows, 1, 1);
            n++;
        }
    }
    closeDatabase(database);
    gtk_grid_attach(GTK_GRID(properties->grid), GTK_WIDGET(*properties->newGrid), 0, 7, 6, 1);
    changeView(NULL, GTK_GRID(properties->grid));
}

void createPersonReservationList(GtkWidget *window, Person_reservations *properties)
{
    UNUSED(window);
    const char
        *fname = gtk_entry_get_text(GTK_ENTRY(properties->fname)),
        *lname = gtk_entry_get_text(GTK_ENTRY(properties->lname));
    if (fname[0] == '\0' || lname[0] == '\0'){
        return;
    }
    FILE *database = openDatabase("reservations");
    if (database == NULL){
        showDialog("Błąd. Wystąpił problem z bazą danych");
        return;
    }
    if (*properties->newGrid != NULL)
        gtk_widget_destroy(GTK_WIDGET(*properties->newGrid));
    *properties->newGrid = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(*properties->newGrid), 5);
    gtk_grid_set_row_homogeneous(GTK_GRID(*properties->newGrid), FALSE);
    gtk_grid_set_column_spacing(GTK_GRID(*properties->newGrid), 20);
    gtk_grid_set_column_homogeneous(GTK_GRID(*properties->newGrid), TRUE);
    GtkWidget *label;
    int n = 0, rows = 18;
    Reservation readReservation;
    Device writeDevice;
    char text[150];
    for (int i = 0; i < RESERVATIONS_PER_DAY * TERM_DAYS; ++i){
        if (fread(&readReservation, sizeof(Reservation), 1, database) != 1)
            break;
        if (strcmp(readReservation.fname, fname) == 0 && strcmp(readReservation.lname, lname) == 0){
            writeDevice = selectDatabaseDeviceFromId(readReservation.idDevice);
            if (readReservation.wholeSession){
                sprintf(text, "cały semestr : %s %s", writeDevice.brand, writeDevice.model);
            }
            else{
                sprintf(text, "%2d/%02d/%4d %2d:%02d - %2d/%02d/%4d %2d:%02d : %s %s",
                    readReservation.start.day, readReservation.start.month, readReservation.start.year,
                    readReservation.start.hours, readReservation.start.minutes,
                    readReservation.finish.day, readReservation.finish.month, readReservation.finish.year,
                    readReservation.finish.hours, readReservation.finish.minutes,
                    writeDevice.brand, writeDevice.model);
            }
            label = gtk_label_new(text);
            gtk_grid_attach(GTK_GRID(*properties->newGrid), GTK_WIDGET(label), n / rows, n % rows, 1, 1);
            n++;
        }
    }
    closeDatabase(database);
    gtk_grid_attach(GTK_GRID(properties->grid), GTK_WIDGET(*properties->newGrid), 0, 7, 6, 1);
    changeView(NULL, GTK_GRID(properties->grid));
}

void createWeekReservationList(GtkWidget *window, Week_reservations *properties)
{
    UNUSED(window);
    char *week = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(properties->combo));
    if (week == NULL){
        showDialog("Błąd. Nie wybrano żadnej daty.");
        return;
    }
    Time_my start, end;
    start.day = atoi(week);
    start.month = atoi(week + 3);
    start.year = atoi(week + 6);
    end.day = atoi(week + 13);
    end.month = atoi(week + 16);
    end.year = atoi(week + 19);
    FILE *database = openDatabase("reservations");
    if (database == NULL){
        showDialog("Bład otwarcia bazy danych.");
        return;
    }
    if (*properties->newGrid != NULL)
        gtk_widget_destroy(GTK_WIDGET(*properties->newGrid));
    *properties->newGrid = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(*properties->newGrid), 5);
    gtk_grid_set_row_homogeneous(GTK_GRID(*properties->newGrid), FALSE);
    gtk_grid_set_column_spacing(GTK_GRID(*properties->newGrid), 20);
    gtk_grid_set_column_homogeneous(GTK_GRID(*properties->newGrid), TRUE);
    GtkWidget *label;
    int n = 0, rows = 18;
    Reservation readReservation;
    Device writeDevice;
    char text[150];
    int finish = generateReservationId(end, false) + RESERVATIONS_PER_DAY;
    for (int i = 0; i < finish; ++i){
        if (fread(&readReservation, sizeof(Reservation), 1, database) != 1)
            break;
        if (readReservation.id && (readReservation.wholeSession ||
            (isGreaterThan(readReservation.finish, start) && isGreaterThan(end, readReservation.finish)) ||
            (isGreaterThan(readReservation.start, start) && isGreaterThan(end, readReservation.start)))){
                writeDevice = selectDatabaseDeviceFromId(readReservation.idDevice);
                if (readReservation.wholeSession){
                    sprintf(text, "%s %s cały semestr : %s %s", readReservation.fname, readReservation.lname, writeDevice.brand, writeDevice.model);
                }
                else{
                    sprintf(text, "%s %s %2d/%02d/%4d %2d:%02d - %2d/%02d/%4d %2d:%02d : %s %s",
                        readReservation.fname, readReservation.lname,
                        readReservation.start.day, readReservation.start.month, readReservation.start.year,
                        readReservation.start.hours, readReservation.start.minutes,
                        readReservation.finish.day, readReservation.finish.month, readReservation.finish.year,
                        readReservation.finish.hours, readReservation.finish.minutes,
                        writeDevice.brand, writeDevice.model);
                }
                label = gtk_label_new(text);
                gtk_grid_attach(GTK_GRID(*properties->newGrid), GTK_WIDGET(label), n / rows, n % rows, 1, 1);
                n++;
        }
    }
    closeDatabase(database);
    gtk_grid_attach(GTK_GRID(properties->grid), GTK_WIDGET(*properties->newGrid), 0, 7, 6, 1);
    changeView(NULL, GTK_GRID(properties->grid));
}

void generateWeekList(GtkWidget *combo)
{
    Time_my start = firstSemesterDay, iter;
    int d = getWeekDay(start.year, start.month, start.day);
    iter = start;
    char text[25];
    if (d > 4) //d > 4 means weekend -> go to the beginning of next week
        addDays(&iter, 7 - d);
    else{
        addDays(&iter, 6 - d);
        sprintf(text, "%02d/%02d/%d - %02d/%02d/%4d", start.day, start.month, start.year, iter.day, iter.month, iter.year);
        gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combo), text);
    }
    start = iter;
    addDays(&start, 1);
    addDays(&iter, 7);
    while(isGreaterThan(lastSemesterDay, iter)){
        sprintf(text, "%02d/%02d/%d - %02d/%02d/%4d", start.day, start.month, start.year, iter.day, iter.month, iter.year);
        gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combo), text);
        addDays(&start, 7);
        addDays(&iter, 7);
    }
    d = getYearDay(lastSemesterDay.year, lastSemesterDay.month, lastSemesterDay.day) - getYearDay(start.year, start.month, start.day);
    iter = start;
    addDays(&iter, d);
    sprintf(text, "%02d/%02d/%d - %02d/%02d/%4d", start.day, start.month, start.year, iter.day, iter.month, iter.year);
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combo), text);
}

void attachReservations(GtkWidget *window, Person_reservations *properties)
{
    UNUSED(window);
    const char
        *fname = gtk_entry_get_text(GTK_ENTRY(properties->fname)),
        *lname = gtk_entry_get_text(GTK_ENTRY(properties->lname));
    if (fname == NULL || lname == NULL) {
        showDialog("Uzupełnij brakujące dane.");
        return;
    }
    FILE *database = openDatabase("reservations");
    if (database == NULL) {
        showDialog("Błąd otwarcia bazy danych.");
        return;
    }
    gtk_combo_box_text_remove_all(GTK_COMBO_BOX_TEXT(properties->grid));
    char text[80];
    Device writeDevice;
    Reservation readReservation;
    for (int i = 0; i < TERM_DAYS * RESERVATIONS_PER_DAY; ++i) {
        fread(&readReservation, sizeof(Reservation), 1, database);
        if (strcmp(readReservation.lname, lname) == 0 && strcmp(readReservation.fname, fname) == 0) {
            writeDevice = selectDatabaseDeviceFromId(readReservation.idDevice);
            if (readReservation.wholeSession)
                sprintf(text, "%d: %s %s cały semestr", readReservation.id, writeDevice.brand, writeDevice.model);
            else
                sprintf(text, "%d: %s %s %02d/%02d/%4d - %02d/%02d/%4d",
                    readReservation.id, writeDevice.brand, writeDevice.model,
                    readReservation.start.day, readReservation.start.month, readReservation.start.year,
                    readReservation.finish.day, readReservation.finish.month, readReservation.finish.year);
            gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(properties->grid), text);
        }
    }
    closeDatabase(database);
}

void generateWeekReservationList(GtkWidget *window, GtkWidget *combo)
{
    UNUSED(window);
    char *week = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(combo));
    if (week == NULL){
        showDialog("Błąd. Nie wybrano żadnej daty.");
        return;
    }
    static Time_my start;
    start = getTimeStruct(atoi(week+6), atoi(week+3), atoi(week));
    static Time_my end;
    end = getTimeStruct(atoi(week+19), atoi(week+16), atoi(week+13));
    char filename[30];
    sprintf(filename, "rezerwacje%02d_%02d_%d-%02d_%02d_%d.tex", start.day, start.month, start.year, end.day, end.month, end.year);
    char *filePath = getFilePath(filename);
    if (filePath == NULL)
        return;
    if (strcmp(filePath + strlen(filePath) - 4, ".tex") != 0){
        showDialog("Niepoprawny format. Zapisz w plik w formacie *.tex");
        return;
    }
    FILE *database = openDatabase("reservations"), *result = fopen(filePath, "w");
    if (database == NULL){
        showDialog("Błąd otwarcia bazy danych.");
        return;
    }
    if (result == NULL){
        showDialog("Błąd. Nie udało sie uwtorzyć nowego pliku.");
        return;
    }
    fprintf(result,
"\\documentclass[11pt]{article}\n\
\\usepackage{geometry}\n\
\\usepackage[T1]{fontenc}\n\
\\usepackage[polish]{babel}\n\
\\usepackage[utf8]{inputenc}\n\
\\usepackage{lmodern}\n\
\\selectlanguage{polish}\n\
\\newgeometry{tmargin=2cm, bmargin=2cm, lmargin=1cm, rmargin=1cm}\n \
 \n\
\\begin{document}\n\
    \\begin{center}\n\
        \\huge \\underline{Rozkład rezerwacji sprzętu elektronicznego w II UWr}\n\
        \\vspace{0.5cm} \n\
        \n\
        \\large w tygodniu:\\\\ \n\
        %02d/%02d/%d - %02d/%02d/%d\n\
    \\end{center}\n\
    \n\
    \\noindent\\rule[0.5cm]{\\textwidth}{1pt}\n\
    \n", start.day, start.month, start.year, end.day, end.month, end.year);
    bool registration = false;
    char text[150];
    Device writeDevice;
    Reservation readReservation;
    int limit = generateReservationId(start, false);
    for (int i = 0; i < limit; ++i){
        fread(&readReservation, sizeof(Reservation), 1, database);
        if (readReservation.id){
            if (!registration){
                fprintf(result, "\t\\large \\underline{\\textbf{Rezerwacje rozpoczęte przez bieżącym tygodniem:}}\n\
    \\begin{itemize}\n\
		\\normalsize\n");
                registration = true;
            }
            writeDevice = selectDatabaseDeviceFromId(readReservation.idDevice);
            if (readReservation.wholeSession){
                sprintf(text, "%s %s cały semestr %s %s",
                        readReservation.fname, readReservation.lname, writeDevice.brand, writeDevice.model);
                fprintf(result, "\t\t\\item %s\n", text);
            }
            else{
                if (isGreaterThan(readReservation.finish, start)){
                    sprintf(text, "%s %s %02d/%02d/%4d g.%2d:%02d - %02d/%02d/%4d g.%2d:%02d %s %s",
                            readReservation.fname, readReservation.lname,
                            readReservation.start.day, readReservation.start.month, readReservation.start.year,
                            readReservation.start.hours, readReservation.start.minutes,
                            readReservation.finish.day, readReservation.finish.month, readReservation.finish.year,
                            readReservation.finish.hours, readReservation.finish.minutes,
                            writeDevice.brand, writeDevice.model);
                    fprintf(result, "\t\t\\item %s\n", text);
                }
            }
        }
    }
    if (registration)
        fprintf(result, "\t\\end{itemize}\n\n");
    fseek(database, limit*sizeof(Reservation), SEEK_SET);
    Time_my copy = start;
    while(isGreaterThan(end, copy)){
        registration = false;
        for (int i = 0; i < RESERVATIONS_PER_DAY; ++i){
            if (fread(&readReservation, sizeof(Reservation), 1, database) != 1)
                continue;
            if (readReservation.id){
                if (!registration){
                    fprintf(result, "\t\\large \\underline{\\textbf{");
                    switch(getWeekDay(copy.year, copy.month, copy.day)){
                        case 0: fprintf(result, "Poniedziałek"); break;
                        case 1: fprintf(result, "Wtorek"); break;
                        case 2: fprintf(result, "Środa"); break;
                        case 3: fprintf(result, "Czwartek"); break;
                        case 4: fprintf(result, "Piątek"); break;
                        case 5: fprintf(result, "Sobota"); break;
                        case 6: fprintf(result, "Niedziela"); break;
                    }
                    fprintf(result, " (%02d.%02d.%d):}}\n", copy.day, copy.month, copy.year);
                    fprintf(result, "\t\\begin{itemize}\n\t\t\\normalsize\n");
                    registration = true;
                }
                writeDevice = selectDatabaseDeviceFromId(readReservation.idDevice);
                sprintf(text, "%s %s g.%2d:%02d do %2d/%02d/%d g.%2d:%02d %s %s",
                        readReservation.fname, readReservation.lname,
                        readReservation.start.hours, readReservation.start.minutes,
                        readReservation.finish.day, readReservation.finish.month, readReservation.finish.year,
                        readReservation.finish.hours, readReservation.finish.minutes,
                        writeDevice.brand, writeDevice.model);
                fprintf(result, "\t\t\\item %s\n", text);
            }
        }
        if (registration)
            fprintf(result, "\t\\end{itemize}\n\n");
        addDays(&copy, 1);
    }
    closeDatabase(database);
    fprintf(result, "\n\\end{document}");
    fclose(result);
}

void add(List **l, char *s)
{
    List *elem = malloc(sizeof(List));
    sprintf(elem->text, "%s", s);
    elem->next = NULL;
    if (*l == NULL){
        *l = elem;
        return;
    }
    List *p = *l;
    while(p->next != NULL){
        if (strcmp(p->text, s) == 0){
            free(elem);
            return;
        }
        p = p->next;
    }
    if (strcmp(p->text, s) == 0){
        free(elem);
        return;
    }
    p->next = elem;
}

bool isEmpty(List *l)
{
    return (l == NULL);
}

void pop(List **l)
{
    List *pom = (*l)->next;
    free(*l);
    *l = pom;
}
