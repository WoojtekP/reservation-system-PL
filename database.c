#include "database.h"

int hash(char *nazwa)
{
    char *p;
    unsigned long g, h = 0;
    for (p = nazwa; *p != '\0'; ++p){
        h = (h << 4) + *p;
        if ((g = (h & 0xF0000000)) != 0)
            h = h ^ (g | (g >> 24));
    }
    return h % PRIME + 1;
}

FILE *openDatabase(char *name)
{
    char *dbName;
    int size;
    int variant;
    if (strcmp(name, "devices") == 0){
        dbName = DB_DEVICES;
        size = sizeof(Device);
        variant = 1;
    }
    else if (strcmp(name, "reservations") == 0){
        dbName = DB_RESERVATIONS;
        size = sizeof(Reservation);
        variant = 2;
    }
    else
        return NULL;
    FILE *connection;
    if ((connection = fopen(dbName,"r+b")) == NULL) {
        connection = fopen(dbName,"w+b");
        if (variant == 1){ //database "urzadzenia"
            Device creator;
            creator.id = 0;
            for (int i = 0; i <= PRIME; ++i)
                fwrite(&creator, size, 1, connection);
        }
        else if (variant == 2){ // database "rezerwacje"
            Reservation creator;
            creator.id = 0;
            for (int i = 0; i <= TERM_DAYS * RESERVATIONS_PER_DAY; ++i)
                fwrite(&creator, size, 1, connection);
        }
        fclose(connection);
    }
    return fopen(dbName,"r+b");
}

void closeDatabase(FILE *f)
{
    fclose(f);
}

void verifyDeviceInsertion(GtkWidget *wid, Device_properties *properties)
{
    UNUSED(wid);
    const char *type, *brand, *model, *description;
    type = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(properties->type));
    brand = gtk_entry_get_text(GTK_ENTRY(properties->brand));
    model = gtk_entry_get_text(GTK_ENTRY(properties->model));
    description = gtk_entry_get_text(GTK_ENTRY(properties->description));
    if (type[0] == '\0' || brand[0] == '\0' || model[0] == '\0'){
        showDialog("Błąd. Uzupełnij brakujące dane.");
        return;
    }
    if (selectDatabaseDeviceId(type[0], brand, model)){
        showDialog("Urządzenie juz istnieje w bazie");
        return;
    }
    if (insertDatbaseDevice(type[0], brand, model, description)){
        showDialog("Pomyślnie dodano urządzenie do bazy.");
        return;
    }
    showDialog("Nie udało się dodać urządzenia poprawnie - błąd bazy danych.");
}

void verifyDeviceDelete(GtkWidget *wid, Device_properties *properties)
{
    UNUSED(wid);
    const char *type, *brand, *model;
    type = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(properties->type));
    brand = gtk_entry_get_text(GTK_ENTRY(properties->brand));
    model = gtk_entry_get_text(GTK_ENTRY(properties->model));
    if (type == NULL || brand[0] == '\0' || model[0] == '\0'){
        showDialog("Błąd. Uzupełnij brakujące dane.");
        return;
    }
    int id = selectDatabaseDeviceId(type[0], brand, model);
    if (id == 0){
        showDialog("Nie znaleziono urządzenia w bazie");
        return;
    }
    if (deleteDatbaseDevice(id)){
        showDialog("Pomyślnie usunięto urządzenie z bazy.");
        return;
    }
    showDialog("Nie udało się usunąć urządzenia - błąd bazy danych.");
}

void verifyReservationInsertion(GtkWidget *wid, Reservation_properties *properties)
{
    UNUSED(wid);
    const char *type, *brand, *model, *fname, *lname;
    type = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(properties->type) );
    brand = gtk_entry_get_text(GTK_ENTRY(properties->brand));
    model = gtk_entry_get_text(GTK_ENTRY(properties->model));
    fname = gtk_entry_get_text(GTK_ENTRY(properties->fname));
    lname = gtk_entry_get_text(GTK_ENTRY(properties->lname));
    if (type == NULL || brand[0] == '\0' || model[0] == '\0' || fname[0] == '\0' || lname[0] == '\0'){
        showDialog("Błąd. Uzupełnij brakujące dane.");
        return;
    }
    int id = selectDatabaseDeviceId(type[0], brand, model);
    if (id == 0){
        showDialog("Błąd. Nie odnaleziono urządzenia w bazie.");
        return;
    }
    Time_my start, end;
    bool session = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(properties->wholeSession));
    if (!session){
        if ( !isTimeCorrect(&start, &end, properties->startDate, properties->endDate, properties->startTime, properties->endTime) ){
            showDialog("Błąd. Niepoprawna data lub godzina");
            return;
        }
    }
    if (!isDeviceAvailable(id, start, end, session)){
        showDialog("Niestety to urządzenie jest zajęte w podanym terminie.");
        return;
    }
    if (!insertDatabaseReservation(fname, lname, id, session, start, end)){
        showDialog("Nie udało się zapisać rezerwacji - błąd bazy danych.");
        return;
    }
    showDialog("Zarezerwowano urządzenie pomyślnie.");
}

void verifyDeviceAvailability(GtkWidget *wid, Device_availability *properties)
{
    UNUSED(wid);
    int id = atoi(gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(properties->device)));
    if (id == 0){
        showDialog("Nie wybrano urządzenia!");
        return;
    }
    Time_my start, end;
    if(!isTimeCorrect(&start, &end, properties->startDate, properties->endDate, properties->startTime, properties->endTime)){
        showDialog("Błąd. Niepoprawna data lub godzina.");
        return;
    }
    if(isDeviceAvailable(id, start, end, false)){
        showDialog("Urządzenie jest dostępne w podanym terminie.");
        return;
    }
    showDialog("Niestety to urządzenie jest zajęte w podanym terminie.");
}

bool insertDatbaseDevice(const char type, const char *brand, const char *model, const char *description)
{
    FILE *database = openDatabase("devices");
    if (database == NULL)
        return false;
    Device newDevice, readDevice;
    newDevice.type = type;
    sprintf(newDevice.brand, "%s", brand);
    sprintf(newDevice.model, "%s", model);
    sprintf(newDevice.description, "%s", description);
    int id, i;
    char code[MAX_BRAND_LEN + MAX_MODEL_LEN + 2];
    sprintf(code, "%c%s%s", type, brand, model);
    i = id = hash(code);
    int insert;
    fseek(database, id * sizeof(Device), SEEK_SET);
    do {
        newDevice.id = id;
        if (fread(&readDevice, sizeof(Device), 1, database) != 1)
            break;
        if (readDevice.id == 0){
            fseek(database, -1 * sizeof(Device), SEEK_CUR);
            insert = fwrite(&newDevice, sizeof(Device), 1, database);
            closeDatabase(database);
            return (insert == 1);
        }
        id = (id % PRIME) + 1;
        if (id == 1)
            fseek(database, sizeof(Device), SEEK_SET);
    } while(id != i);
    closeDatabase(database);
    return false;
}

bool deleteDatbaseDevice(int id)
{
    FILE
        *databaseDev = openDatabase("devices"),
        *databaseRes = openDatabase("reservations");
    if (databaseDev == NULL || databaseRes == NULL){
        closeDatabase(databaseDev);
        closeDatabase(databaseRes);
        return false;
    }
    Device deleteDevice;
    deleteDevice.id = 0;
    fseek(databaseDev, id * sizeof(Device), 1);
    if (fwrite(&deleteDevice, sizeof(Device), 1, databaseDev) != 1){
        closeDatabase(databaseDev);
        closeDatabase(databaseRes);
        return false;
    }
    Reservation deleteReservation, readReservation;
    deleteReservation.id = deleteReservation.idDevice = 0;
    for (int i = 0; i < TERM_DAYS * RESERVATIONS_PER_DAY; ++i){
        fread(&readReservation, sizeof(Reservation), 1, databaseRes);
        if (readReservation.idDevice == id){
            fseek(databaseRes, -1 * sizeof(Reservation), 1);
            fwrite(&deleteReservation, sizeof(Reservation), 1, databaseRes);
        }
    }
    closeDatabase(databaseDev);
    closeDatabase(databaseRes);
    return true;
}

void deleteDatabaseReservation(GtkWidget *wid, GtkWidget *combo)
{
    UNUSED(wid);
    char *text = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(combo));
    if (text == NULL){
        showDialog("Nie wybrano rezerwacji.");
        return;
    }
    int id = atoi(text);
    FILE *database = openDatabase("reservations");
    if (database == NULL){
        showDialog("Błąd otwarcia bazy danych - operacja nie mogła zostać zrealizowana.");
        return;
    }
    fseek(database, id * sizeof(Reservation), SEEK_SET);
    Reservation readReservation;
    if(fread(&readReservation, sizeof(Reservation), 1, database) != 1){
        showDialog("Bład odczytu bazy danych.");
        closeDatabase(database);
        return;
    }
    if(readReservation.id == id){
        fseek(database, -1 * sizeof(Reservation), SEEK_CUR);
        readReservation.id = readReservation.idDevice = 0;
        readReservation.fname[0] = '\0';
        readReservation.lname[0] = '\0';
        int delete = fwrite(&readReservation, sizeof(Reservation), 1, database);
        if (delete == 1)
            showDialog("Usunięto rezerwację pomyślnie.");
        else
            showDialog("Błąd. Nie udało się usunąć rezerwacji.");
    }
    closeDatabase(database);
}

bool insertDatabaseReservation(const char *fname, const char *lname, int id, bool all, Time_my from, Time_my to)
{
    FILE *database = openDatabase("reservations");
    if (database == NULL)
        return false;
    Reservation newReservation, readReservation;
    sprintf(newReservation.fname, "%s", fname);
    sprintf(newReservation.lname, "%s", lname);
    newReservation.idDevice = id;
    newReservation.start = from;
    newReservation.finish = to;
    newReservation.wholeSession = all;
    int idRes = generateReservationId(newReservation.start, newReservation.wholeSession);
    fseek(database, idRes * sizeof(Reservation), SEEK_SET);
    int insert;
    for (int i = 0; i < RESERVATIONS_PER_DAY; i++) {
        if (fread(&readReservation, sizeof(Reservation), 1, database) != 1)
            break;
        if (readReservation.id == 0){
            fseek(database, -1 * sizeof(Device), SEEK_CUR);
            newReservation.id = idRes;
            insert = fwrite(&newReservation, sizeof(Reservation), 1, database);
            closeDatabase(database);
            return (insert == 1);
        }
        idRes = (idRes % (RESERVATIONS_PER_DAY * TERM_DAYS)) + 1;
        if (idRes == 1)
            fseek(database, sizeof(Reservation), SEEK_SET);
    }
    closeDatabase(database);
    return false;
}

bool isDeviceAvailable(int id, Time_my from, Time_my to, bool session)
{
    FILE *database = openDatabase("reservations");
    if (database == NULL)
        return false;
    Reservation readReservation;
    fseek(database, sizeof(Reservation), SEEK_SET);
    //booking for whole semester
    for (int i = 0; i < RESERVATIONS_PER_DAY; ++i){
        fread(&readReservation, sizeof(Reservation), 1, database);
        if (readReservation.idDevice == id){
            closeDatabase(database);
            return false;
        }
    }
    if (session){   //whole semester
        for (int i = RESERVATIONS_PER_DAY; i <= TERM_DAYS * RESERVATIONS_PER_DAY; ++i){
            fread(&readReservation, sizeof(Reservation), 1, database);
            if (readReservation.idDevice == id){
                closeDatabase(database);
                return false;
            }
        }
        closeDatabase(database);
        return true;
    }
    for (int i = 0; i <= id + RESERVATIONS_PER_DAY; ++i){
        fread(&readReservation, sizeof(Reservation), 1, database);
        if (readReservation.idDevice == id)
            if ( (isGreaterThan(readReservation.start, from) && isGreaterThan(to, readReservation.start))
                || (isGreaterThan(readReservation.finish, from) && isGreaterThan(to, readReservation.finish)) ){
                closeDatabase(database);
                return false;
            }
    }
    closeDatabase(database);
    return true;
}

int selectDatabaseDeviceId(const char type, const char *brand, const char *model)
{
    FILE *database = openDatabase("devices");
    if (database == NULL)
        return 0;
    char code[MAX_BRAND_LEN + MAX_MODEL_LEN + 2];
    sprintf(code, "%c%s%s", type, brand, model);
    Device readDevice;
    int id, i;
    id = i = hash(code);
    fseek(database, i * sizeof(Device), SEEK_SET);
    do {
        if (fread(&readDevice, sizeof(Device), 1, database) != 1){
            readDevice.id = 0;
            break;
        }
        if (readDevice.type == type &&
            strcmp(readDevice.brand, brand) == 0 &&
            strcmp(readDevice.model, model) == 0){
            break;
        }
        id = (id % PRIME) + 1;
        if (id == 1)
            fseek(database, sizeof(Device), SEEK_SET);
    } while(id != i && readDevice.id);
    closeDatabase(database);
    return readDevice.id;
}

Device selectDatabaseDeviceFromId(int id)
{
    Device readDevice;
    readDevice.id = 0;
    FILE *database = openDatabase("devices");
    if (database == NULL)
        return readDevice;
    fseek(database, id * sizeof(Device), SEEK_SET);
    fread(&readDevice, sizeof(Device), 1, database);
    return readDevice;
}

int generateReservationId(Time_my date, bool all)
{
    if (all)
        return 1;
    int semesterStart = getYearDay(firstSemesterDay.year, firstSemesterDay.month, firstSemesterDay.day);
    int reservationDay = getYearDay(date.year, date.month, date.day);
    int day = reservationDay - semesterStart + 1;
    if (day <= 0)
        day = getYearDay(firstSemesterDay.year, 12, 31) - semesterStart + 1 + reservationDay;
    int id = 1 + day * RESERVATIONS_PER_DAY;
    return id;
}
