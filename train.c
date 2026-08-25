#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* =========================================================
   CONSTANTS
   ========================================================= */

#define TRAIN_FILE "trains.dat"
#define PASSENGER_FILE "passengers.dat"
#define RESERVATION_FILE "reservations.dat"
#define ADMIN_FILE "admin.dat"


/* =========================================================
   STRUCTURES
   ========================================================= */

typedef struct
{
    int trainNo;

    char trainName[50];
    char source[30];
    char destination[30];

    char departure[10];
    char arrival[10];

    int totalSeats;
    int availableSeats;

    float sleeperFare;
    float acFare;

} Train;
typedef struct 
{
    int passengerId;
    char name[50];
    int age;
    char gender;
    char phone[15];
} Passenger;

typedef struct
{
    int pnr;
    int trainNo;
    int passengerId;
    int seatNo;

    char journeyDate[15];
    char classType[20];

    float fare;

    char status[15];

} Reservation;

typedef struct 
{
    char username[30];
    char password[30];
} Admin;




/* =========================================================
   FUNCTION PROTOTYPES
   ========================================================= */

void mainMenu(void);

void passengerMenu(void);
void adminMenu(void);

void searchTrain(void);
void bookTicket(void);
void checkPNR(void);
void cancelTicket(void);

void addTrain(void);
void viewTrains(void);
void updateTrain(void);
void deleteTrain(void);
void viewReservations(void);

void clearScreen(void);
void pauseScreen(void);

int trainExists(int trainNo);
void searchTrainMenu(void);
void searchByTrainNumber(void);
void searchByRoute(void);
void displayTrain(Train train);
int stringEqualIgnoreCase(const char *a, const char *b);
void passengerManagementMenu(void);

void addPassenger(void);
void viewPassengers(void);
void searchPassenger(void);
void updatePassenger(void);
void deletePassenger(void);

int generatePassengerId(void);
int passengerExists(int passengerId);
void bookTicket(void);

int generatePNR(void);

int getNextSeatNumber(int trainNo, char journeyDate[]);
float calculateFare(Train train, char classType[]);
int updateAvailableSeats(int trainNo, int change);

void checkPNR(void);
void displayTicket(Reservation reservation);
void viewReservations(void);
void searchReservationByPassenger(void);

int findTrainByNumber(int trainNo, Train *train);
int findPassengerById(int passengerId, Passenger *passenger);
void cancelTicket(void);
int updateReservationStatus(int pnr, const char newStatus[]);
void adminLogin(void);

int checkAdminLogin(const char username[],
                    const char password[]);

void createDefaultAdmin(void);
int readInt(const char *prompt, int min, int max);
float readFloat(const char *prompt, float min, float max);

void readString(const char *prompt, char str[], int size);

char readGender(void);

int isValidPhone(const char phone[]);
int isValidDate(const char date[]);

void readDate(const char *prompt, char date[]);
void readPhone(char phone[]);
void systemSummary(void);

/* =========================================================
   MAIN
   ========================================================= */

int main(void)
{
    createDefaultAdmin();
    mainMenu();

    return 0;
}


/* =========================================================
   UTILITY FUNCTIONS
   ========================================================= */

void clearScreen(void)
{
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}


void pauseScreen(void)
{
    printf("\nPress ENTER to continue...");
    getchar();
    getchar();
}


/* =========================================================
   MAIN MENU
   ========================================================= */

void mainMenu(void)
{
    int choice;

    while (1)
    {
        clearScreen();

        printf("\n");
        printf("=============================================\n");
        printf("       RAILWAY RESERVATION SYSTEM\n");
        printf("=============================================\n");

        printf("\n1. Passenger Section");
        printf("\n2. Admin Section");
        printf("\n3. Exit");

        printf("\n\nEnter your choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
            case 1:
                passengerMenu();
                break;

            case 2:
                adminLogin();
                break;

            case 3:
                printf("\nThank you for using the system!\n");
                exit(0);

            default:
                printf("\nInvalid choice!");
                pauseScreen();
        }
    }
}


/* =========================================================
   PASSENGER MENU
   ========================================================= */

void passengerMenu(void)
{
    int choice;

    while (1)
    {
        clearScreen();

        printf("\n");
        printf("=============================================\n");
        printf("            PASSENGER SECTION\n");
        printf("=============================================\n");

        printf("\n1. Search Train");
        printf("\n2. Passenger Management");
        printf("\n3. Book Ticket");
        printf("\n4. PNR Enquiry");
        printf("\n5. My Reservations");
        printf("\n6. Cancel Ticket");
        printf("\n7. Back to Main Menu");

        printf("\n\nEnter your choice: ");
        scanf("%d", &choice);


        switch (choice)
        {
            case 1:
                searchTrain();
                break;

            case 2:
                passengerManagementMenu();
                break;

            case 3:
                bookTicket();
                break;

            case 4:
                checkPNR();
                break;

            case 5:
                searchReservationByPassenger();
                break;

            case 6:
                cancelTicket();
                break;

            case 7:
                return;

            default:
                printf("\nInvalid choice!");

                pauseScreen();
        }
    }
}
/* =========================================================
   ADMIN MENU
   ========================================================= */

void adminMenu(void)
{
    int choice;

    while (1)
    {
        clearScreen();

        printf("\n");
        printf("=============================================\n");
        printf("              ADMIN DASHBOARD\n");
        printf("=============================================\n");

        printf("\n1. Add Train");
        printf("\n2. View Trains");
        printf("\n3. Update Train");
        printf("\n4. Delete Train");
        printf("\n5. View Reservations");
        printf("\n6. View Passengers");
        printf("\n7. System Summary ");
        printf("\n8. Logout");


        printf("\n\nEnter your choice: ");
        scanf("%d", &choice);


        switch (choice)
        {
            case 1:
                addTrain();
                break;


            case 2:
                viewTrains();
                break;


            case 3:
                updateTrain();
                break;


            case 4:
                deleteTrain();
                break;


            case 5:
                viewReservations();
                break;


            case 6:
                viewPassengers();
                break;

            case 7:
                systemSummary();
                break;


            case 8:
                printf("\nLogging out...");

                pauseScreen();

                return;


            default:
                printf("\nInvalid choice!");

                pauseScreen();
        }
    }
}

/* =========================================================
   CHECK WHETHER TRAIN EXISTS
   ========================================================= */

int trainExists(int trainNo)
{
    FILE *file;
    Train train;

    file = fopen(TRAIN_FILE, "rb");

    if (file == NULL)
    {
        return 0;
    }

    while (fread(&train, sizeof(Train), 1, file) == 1)
    {
        if (train.trainNo == trainNo)
        {
            fclose(file);
            return 1;
        }
    }

    fclose(file);

    return 0;
}


/* =========================================================
   ADD TRAIN
   ========================================================= */

void addTrain(void)
{
    FILE *file;
    Train train;

    clearScreen();

    printf("\n");
    printf("=============================================\n");
    printf("                 ADD TRAIN\n");
    printf("=============================================\n");

    printf("\nEnter Train Number: ");
    scanf("%d", &train.trainNo);

    /* Check duplicate train number */

    if (trainExists(train.trainNo))
    {
        printf("\nTrain number already exists!");

        pauseScreen();
        return;
    }

    printf("Enter Train Name: ");
    scanf(" %[^\n]", train.trainName);

    printf("Enter Source: ");
    scanf(" %[^\n]", train.source);

    printf("Enter Destination: ");
    scanf(" %[^\n]", train.destination);

    printf("Enter Departure Time: ");
    scanf("%s", train.departure);

    printf("Enter Arrival Time: ");
    scanf("%s", train.arrival);

    printf("Enter Total Seats: ");
    scanf("%d", &train.totalSeats);

    /*
       Initially all seats are available.
    */

    train.availableSeats = train.totalSeats;

    printf("Enter Sleeper Fare: ");
    scanf("%f", &train.sleeperFare);

    printf("Enter AC Fare: ");
    scanf("%f", &train.acFare);


    /* Open file */

    file = fopen(TRAIN_FILE, "ab");

    if (file == NULL)
    {
        printf("\nError opening train database!");

        pauseScreen();
        return;
    }


    /* Write structure to file */

    fwrite(&train, sizeof(Train), 1, file);

    fclose(file);

    printf("\n=============================================\n");
    printf("Train added successfully!\n");
    printf("=============================================\n");

    pauseScreen();
}


/* =========================================================
   VIEW ALL TRAINS
   ========================================================= */

void viewTrains(void)
{
    FILE *file;
    Train train;

    int count = 0;

    clearScreen();

    printf("\n");
    printf("================================================================================\n");
    printf("                              ALL TRAINS\n");
    printf("================================================================================\n");

    file = fopen(TRAIN_FILE, "rb");

    if (file == NULL)
    {
        printf("\nNo train records found.");

        pauseScreen();
        return;
    }

    printf("\n%-8s %-20s %-15s %-15s %-10s %-10s\n",
           "Number",
           "Train Name",
           "Source",
           "Destination",
           "Seats",
           "Available");

    printf("--------------------------------------------------------------------------------\n");

    while (fread(&train, sizeof(Train), 1, file) == 1)
    {
        printf("%-8d %-20s %-15s %-15s %-10d %-10d\n",
               train.trainNo,
               train.trainName,
               train.source,
               train.destination,
               train.totalSeats,
               train.availableSeats);

        count++;
    }

    fclose(file);

    if (count == 0)
    {
        printf("\nNo trains available.");
    }

    printf("\n================================================================================\n");
    printf("Total trains: %d\n", count);

    pauseScreen();
}

/* =========================================================
   SEARCH TRAIN MENU
   ========================================================= */

void searchTrain(void)
{
    searchTrainMenu();
}


/* =========================================================
   SEARCH TRAIN MENU
   ========================================================= */

void searchTrainMenu(void)
{
    int choice;

    while (1)
    {
        clearScreen();

        printf("\n");
        printf("=============================================\n");
        printf("                SEARCH TRAIN\n");
        printf("=============================================\n");

        printf("\n1. Search by Train Number");
        printf("\n2. Search by Source & Destination");
        printf("\n3. View All Trains");
        printf("\n4. Back");

        printf("\n\nEnter your choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
            case 1:
                searchByTrainNumber();
                break;

            case 2:
                searchByRoute();
                break;

            case 3:
                viewTrains();
                break;

            case 4:
                return;

            default:
                printf("\nInvalid choice!");
                pauseScreen();
        }
    }
}


/* =========================================================
   SEARCH BY TRAIN NUMBER
   ========================================================= */

void searchByTrainNumber(void)
{
    FILE *file;
    Train train;

    int trainNo;
    int found = 0;

    clearScreen();

    printf("\n");
    printf("=============================================\n");
    printf("          SEARCH BY TRAIN NUMBER\n");
    printf("=============================================\n");

    printf("\nEnter Train Number: ");
    scanf("%d", &trainNo);

    file = fopen(TRAIN_FILE, "rb");

    if (file == NULL)
    {
        printf("\nNo train database found.");
        pauseScreen();
        return;
    }

    while (fread(&train, sizeof(Train), 1, file) == 1)
    {
        if (train.trainNo == trainNo)
        {
            found = 1;

            displayTrain(train);

            break;
        }
    }

    fclose(file);

    if (!found)
    {
        printf("\nTrain %d was not found.", trainNo);
    }

    pauseScreen();
}


/* =========================================================
   SEARCH BY SOURCE & DESTINATION
   ========================================================= */

void searchByRoute(void)
{
    FILE *file;
    Train train;

    char source[30];
    char destination[30];

    int found = 0;

    clearScreen();

    printf("\n");
    printf("=============================================\n");
    printf("        SEARCH BY SOURCE & DESTINATION\n");
    printf("=============================================\n");

    printf("\nEnter Source: ");
    scanf(" %[^\n]", source);

    printf("Enter Destination: ");
    scanf(" %[^\n]", destination);


    file = fopen(TRAIN_FILE, "rb");

    if (file == NULL)
    {
        printf("\nNo train database found.");
        pauseScreen();
        return;
    }


    printf("\n");
    printf("================================================================================\n");
    printf("%-8s %-20s %-15s %-15s %-10s %-10s\n",
           "Number",
           "Train Name",
           "Source",
           "Destination",
           "Departure",
           "Available");

    printf("================================================================================\n");


    while (fread(&train, sizeof(Train), 1, file) == 1)
    {
        if (stringEqualIgnoreCase(train.source, source) &&
            stringEqualIgnoreCase(train.destination, destination))
        {
            printf("%-8d %-20s %-15s %-15s %-10s %-10d\n",
                   train.trainNo,
                   train.trainName,
                   train.source,
                   train.destination,
                   train.departure,
                   train.availableSeats);

            found = 1;
        }
    }


    fclose(file);


    if (!found)
    {
        printf("\nNo trains found for this route.");
    }

    printf("\n================================================================================\n");

    pauseScreen();
}


/* =========================================================
   DISPLAY COMPLETE TRAIN INFORMATION
   ========================================================= */

void displayTrain(Train train)
{
    printf("\n");
    printf("---------------------------------------------\n");

    printf("Train Number     : %d\n", train.trainNo);
    printf("Train Name       : %s\n", train.trainName);

    printf("Source           : %s\n", train.source);
    printf("Destination      : %s\n", train.destination);

    printf("Departure        : %s\n", train.departure);
    printf("Arrival          : %s\n", train.arrival);

    printf("Total Seats      : %d\n", train.totalSeats);
    printf("Available Seats  : %d\n", train.availableSeats);

    printf("Sleeper Fare     : %.2f\n", train.sleeperFare);
    printf("AC Fare          : %.2f\n", train.acFare);

    printf("---------------------------------------------\n");
}


/* =========================================================
   CASE-INSENSITIVE STRING COMPARISON
   ========================================================= */

int stringEqualIgnoreCase(const char *a, const char *b)
{
    while (*a && *b)
    {
        char charA = *a;
        char charB = *b;

        /* Convert uppercase to lowercase */

        if (charA >= 'A' && charA <= 'Z')
        {
            charA = charA + ('a' - 'A');
        }

        if (charB >= 'A' && charB <= 'Z')
        {
            charB = charB + ('a' - 'A');
        }

        if (charA != charB)
        {
            return 0;
        }

        a++;
        b++;
    }

    return *a == *b;
}


/* =========================================================
   UPDATE TRAIN
   ========================================================= */

void updateTrain(void)
{
    FILE *file;
    Train train;

    int trainNo;
    int found = 0;

    clearScreen();

    printf("\n");
    printf("=============================================\n");
    printf("                UPDATE TRAIN\n");
    printf("=============================================\n");

    printf("\nEnter Train Number: ");
    scanf("%d", &trainNo);


    file = fopen(TRAIN_FILE, "rb+");

    if (file == NULL)
    {
        printf("\nNo train database found.");

        pauseScreen();
        return;
    }


    while (fread(&train, sizeof(Train), 1, file) == 1)
    {
        if (train.trainNo == trainNo)
        {
            found = 1;

            printf("\nCurrent Train Name: %s", train.trainName);

            printf("\n\nEnter New Train Name: ");
            scanf(" %[^\n]", train.trainName);

            printf("Enter New Source: ");
            scanf(" %[^\n]", train.source);

            printf("Enter New Destination: ");
            scanf(" %[^\n]", train.destination);

            printf("Enter New Departure Time: ");
            scanf("%s", train.departure);

            printf("Enter New Arrival Time: ");
            scanf("%s", train.arrival);

            printf("Enter New Total Seats: ");
            scanf("%d", &train.totalSeats);

            /*
               For now we reset available seats.
               Later reservation logic will handle this properly.
            */

            train.availableSeats = train.totalSeats;

            printf("Enter New Sleeper Fare: ");
            scanf("%f", &train.sleeperFare);

            printf("Enter New AC Fare: ");
            scanf("%f", &train.acFare);


            /*
               Move file pointer back by one structure
               so the modified record can overwrite it.
            */

            fseek(file, -sizeof(Train), SEEK_CUR);

            fwrite(&train, sizeof(Train), 1, file);

            printf("\nTrain updated successfully!");

            break;
        }
    }


    fclose(file);


    if (!found)
    {
        printf("\nTrain not found!");
    }

    pauseScreen();
}


/* =========================================================
   DELETE TRAIN
   ========================================================= */

void deleteTrain(void)
{
    FILE *file;
    FILE *tempFile;

    Train train;

    int trainNo;
    int found = 0;


    clearScreen();

    printf("\n");
    printf("=============================================\n");
    printf("                DELETE TRAIN\n");
    printf("=============================================\n");

    printf("\nEnter Train Number: ");
    scanf("%d", &trainNo);


    file = fopen(TRAIN_FILE, "rb");

    if (file == NULL)
    {
        printf("\nNo train database found.");

        pauseScreen();
        return;
    }


    tempFile = fopen("temp.dat", "wb");

    if (tempFile == NULL)
    {
        fclose(file);

        printf("\nError creating temporary file!");

        pauseScreen();
        return;
    }


    /*
       Copy every train except the one
       we want to delete.
    */

    while (fread(&train, sizeof(Train), 1, file) == 1)
    {
        if (train.trainNo == trainNo)
        {
            found = 1;
            continue;
        }

        fwrite(&train, sizeof(Train), 1, tempFile);
    }


    fclose(file);
    fclose(tempFile);


    /*
       Replace original file with temporary file.
    */

    remove(TRAIN_FILE);
    rename("temp.dat", TRAIN_FILE);


    if (found)
    {
        printf("\nTrain deleted successfully!");
    }
    else
    {
        printf("\nTrain not found!");
    }

    pauseScreen();
}
/* =========================================================
   PASSENGER MANAGEMENT MENU
   ========================================================= */

void passengerManagementMenu(void)
{
    int choice;

    while (1)
    {
        clearScreen();

        printf("\n");
        printf("=============================================\n");
        printf("          PASSENGER MANAGEMENT\n");
        printf("=============================================\n");

        printf("\n1. Add Passenger");
        printf("\n2. View All Passengers");
        printf("\n3. Search Passenger");
        printf("\n4. Update Passenger");
        printf("\n5. Delete Passenger");
        printf("\n6. Back");

        printf("\n\nEnter your choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
            case 1:
                addPassenger();
                break;

            case 2:
                viewPassengers();
                break;

            case 3:
                searchPassenger();
                break;

            case 4:
                updatePassenger();
                break;

            case 5:
                deletePassenger();
                break;

            case 6:
                return;

            default:
                printf("\nInvalid choice!");
                pauseScreen();
        }
    }
}

/* =========================================================
   ADD PASSENGER
   ========================================================= */

void addPassenger(void)
{
    FILE *file;

    Passenger passenger;


    clearScreen();

    printf("\n");
    printf("=============================================\n");
    printf("              ADD PASSENGER\n");
    printf("=============================================\n");


    passenger.passengerId =
        generatePassengerId();


    printf("\nPassenger ID: %d\n",
           passenger.passengerId);


    readString("Enter Passenger Name: ",
               passenger.name,
               sizeof(passenger.name));


    passenger.age =
        readInt("Enter Age: ", 1, 120);


    passenger.gender =
        readGender();


    readPhone(passenger.phone);


    file = fopen(PASSENGER_FILE, "ab");

    if (file == NULL)
    {
        printf("\nError opening passenger database!");

        pauseScreen();

        return;
    }


    fwrite(&passenger,
           sizeof(Passenger),
           1,
           file);


    fclose(file);


    printf("\n=============================================\n");
    printf("Passenger added successfully!\n");
    printf("Passenger ID: %d\n",
           passenger.passengerId);
    printf("=============================================\n");


    pauseScreen();
}

/* =========================================================
   GENERATE PASSENGER ID
   ========================================================= */

int generatePassengerId(void)
{
    FILE *file;

    Passenger passenger;

    int maxId = 1000;


    file = fopen(PASSENGER_FILE, "rb");

    if (file == NULL)
    {
        return 1001;
    }


    while (fread(&passenger, sizeof(Passenger), 1, file) == 1)
    {
        if (passenger.passengerId > maxId)
        {
            maxId = passenger.passengerId;
        }
    }


    fclose(file);

    return maxId + 1;
}

/* =========================================================
   VIEW ALL PASSENGERS
   ========================================================= */

void viewPassengers(void)
{
    FILE *file;

    Passenger passenger;

    int count = 0;


    clearScreen();

    printf("\n");
    printf("====================================================================\n");
    printf("                        PASSENGER LIST\n");
    printf("====================================================================\n");


    file = fopen(PASSENGER_FILE, "rb");

    if (file == NULL)
    {
        printf("\nNo passenger records found.");

        pauseScreen();
        return;
    }


    printf("\n%-8s %-25s %-8s %-10s %-15s\n",
           "ID",
           "Name",
           "Age",
           "Gender",
           "Phone");

    printf("--------------------------------------------------------------------\n");


    while (fread(&passenger,
                 sizeof(Passenger),
                 1,
                 file) == 1)
    {
        printf("%-8d %-25s %-8d %-10c %-15s\n",
               passenger.passengerId,
               passenger.name,
               passenger.age,
               passenger.gender,
               passenger.phone);

        count++;
    }


    fclose(file);


    printf("\n--------------------------------------------------------------------\n");
    printf("Total passengers: %d\n", count);

    pauseScreen();
}

/* =========================================================
   SEARCH PASSENGER
   ========================================================= */

void searchPassenger(void)
{
    FILE *file;

    Passenger passenger;

    int passengerId;
    int found = 0;


    clearScreen();

    printf("\n");
    printf("=============================================\n");
    printf("            SEARCH PASSENGER\n");
    printf("=============================================\n");


    printf("\nEnter Passenger ID: ");
    scanf("%d", &passengerId);


    file = fopen(PASSENGER_FILE, "rb");

    if (file == NULL)
    {
        printf("\nPassenger database not found.");

        pauseScreen();
        return;
    }


    while (fread(&passenger,
                 sizeof(Passenger),
                 1,
                 file) == 1)
    {
        if (passenger.passengerId == passengerId)
        {
            found = 1;

            printf("\n---------------------------------------------\n");

            printf("Passenger ID : %d\n",
                   passenger.passengerId);

            printf("Name         : %s\n",
                   passenger.name);

            printf("Age          : %d\n",
                   passenger.age);

            printf("Gender       : %c\n",
                   passenger.gender);

            printf("Phone        : %s\n",
                   passenger.phone);

            printf("---------------------------------------------\n");

            break;
        }
    }


    fclose(file);


    if (!found)
    {
        printf("\nPassenger not found!");
    }


    pauseScreen();
}

/* =========================================================
   CHECK PASSENGER EXISTENCE
   ========================================================= */

int passengerExists(int passengerId)
{
    FILE *file;

    Passenger passenger;


    file = fopen(PASSENGER_FILE, "rb");

    if (file == NULL)
    {
        return 0;
    }


    while (fread(&passenger,
                 sizeof(Passenger),
                 1,
                 file) == 1)
    {
        if (passenger.passengerId == passengerId)
        {
            fclose(file);

            return 1;
        }
    }


    fclose(file);

    return 0;
}

/* =========================================================
   UPDATE PASSENGER
   ========================================================= */

void updatePassenger(void)
{
    FILE *file;

    Passenger passenger;

    int passengerId;
    int found = 0;


    clearScreen();

    printf("\n");
    printf("=============================================\n");
    printf("             UPDATE PASSENGER\n");
    printf("=============================================\n");


    printf("\nEnter Passenger ID: ");
    scanf("%d", &passengerId);


    file = fopen(PASSENGER_FILE, "rb+");

    if (file == NULL)
    {
        printf("\nPassenger database not found.");

        pauseScreen();
        return;
    }


    while (fread(&passenger,
                 sizeof(Passenger),
                 1,
                 file) == 1)
    {
        if (passenger.passengerId == passengerId)
        {
            found = 1;


            printf("\nCurrent Name: %s\n",
                   passenger.name);


            printf("\nEnter New Name: ");
            scanf(" %[^\n]", passenger.name);


            printf("Enter New Age: ");
            scanf("%d", &passenger.age);


            printf("Enter New Gender (M/F/O): ");
            scanf(" %c", &passenger.gender);


            printf("Enter New Phone: ");
            scanf("%s", passenger.phone);


            /*
               Move back one structure
            */

            fseek(file,
                  -sizeof(Passenger),
                  SEEK_CUR);


            /*
               Overwrite old record
            */

            fwrite(&passenger,
                   sizeof(Passenger),
                   1,
                   file);


            printf("\nPassenger updated successfully!");

            break;
        }
    }


    fclose(file);


    if (!found)
    {
        printf("\nPassenger not found!");
    }


    pauseScreen();
}

/* =========================================================
   DELETE PASSENGER
   ========================================================= */

void deletePassenger(void)
{
    FILE *file;
    FILE *tempFile;

    Passenger passenger;

    int passengerId;
    int found = 0;


    clearScreen();

    printf("\n");
    printf("=============================================\n");
    printf("             DELETE PASSENGER\n");
    printf("=============================================\n");


    printf("\nEnter Passenger ID: ");
    scanf("%d", &passengerId);


    file = fopen(PASSENGER_FILE, "rb");

    if (file == NULL)
    {
        printf("\nPassenger database not found.");

        pauseScreen();
        return;
    }


    tempFile = fopen("passenger_temp.dat", "wb");

    if (tempFile == NULL)
    {
        fclose(file);

        printf("\nError creating temporary file!");

        pauseScreen();
        return;
    }


    while (fread(&passenger,
                 sizeof(Passenger),
                 1,
                 file) == 1)
    {
        if (passenger.passengerId == passengerId)
        {
            found = 1;

            continue;
        }


        fwrite(&passenger,
                sizeof(Passenger),
                1,
                tempFile);
    }


    fclose(file);
    fclose(tempFile);


    remove(PASSENGER_FILE);

    rename("passenger_temp.dat",
           PASSENGER_FILE);


    if (found)
    {
        printf("\nPassenger deleted successfully!");
    }
    else
    {
        printf("\nPassenger not found!");
    }


    pauseScreen();
}

/* =========================================================
   GENERATE PNR
   ========================================================= */

int generatePNR(void)
{
    FILE *file;

    Reservation reservation;

    int maxPNR = 100000000;


    file = fopen(RESERVATION_FILE, "rb");

    if (file == NULL)
    {
        return 100000001;
    }


    while (fread(&reservation,
                 sizeof(Reservation),
                 1,
                 file) == 1)
    {
        if (reservation.pnr > maxPNR)
        {
            maxPNR = reservation.pnr;
        }
    }


    fclose(file);

    return maxPNR + 1;
}

/* =========================================================
   GET NEXT AVAILABLE SEAT
   ========================================================= */

int getNextSeatNumber(int trainNo, char journeyDate[])
{
    FILE *file;

    Reservation reservation;

    int seat = 1;

    int used;


    file = fopen(RESERVATION_FILE, "rb");

    if (file == NULL)
    {
        return 1;
    }


    while (1)
    {
        used = 0;

        rewind(file);


        while (fread(&reservation,
                     sizeof(Reservation),
                     1,
                     file) == 1)
        {
            if (reservation.trainNo == trainNo &&
                reservation.seatNo == seat &&
                strcmp(reservation.journeyDate,
                       journeyDate) == 0 &&
                strcmp(reservation.status,
                       "CONFIRMED") == 0)
            {
                used = 1;

                break;
            }
        }


        if (!used)
        {
            fclose(file);

            return seat;
        }


        seat++;
    }
}

/* =========================================================
   CALCULATE FARE
   ========================================================= */

float calculateFare(Train train, char classType[])
{
    if (strcmp(classType, "Sleeper") == 0)
    {
        return train.sleeperFare;
    }

    if (strcmp(classType, "AC") == 0)
    {
        return train.acFare;
    }

    return 0;
}

/* =========================================================
   UPDATE AVAILABLE SEATS
   ========================================================= */

int updateAvailableSeats(int trainNo, int change)
{
    FILE *file;

    Train train;

    int updated = 0;


    file = fopen(TRAIN_FILE, "rb+");

    if (file == NULL)
    {
        return 0;
    }


    while (fread(&train,
                 sizeof(Train),
                 1,
                 file) == 1)
    {
        if (train.trainNo == trainNo)
        {
            /*
               Check limits
            */

            if (train.availableSeats + change < 0)
            {
                fclose(file);

                return 0;
            }


            if (train.availableSeats + change >
                train.totalSeats)
            {
                fclose(file);

                return 0;
            }


            train.availableSeats += change;


            /*
               Move back one structure
            */

            fseek(file,
                  -sizeof(Train),
                  SEEK_CUR);


            fwrite(&train,
                   sizeof(Train),
                   1,
                   file);


            updated = 1;

            break;
        }
    }


    fclose(file);

    return updated;
}

/* =========================================================
   FIND TRAIN BY TRAIN NUMBER
   ========================================================= */

int findTrainByNumber(int trainNo, Train *train)
{
    FILE *file;

    file = fopen(TRAIN_FILE, "rb");

    if (file == NULL)
    {
        return 0;
    }

    while (fread(train, sizeof(Train), 1, file) == 1)
    {
        if (train->trainNo == trainNo)
        {
            fclose(file);

            return 1;
        }
    }

    fclose(file);

    return 0;
}

/* =========================================================
   FIND PASSENGER BY ID
   ========================================================= */

int findPassengerById(int passengerId,
                      Passenger *passenger)
{
    FILE *file;

    file = fopen(PASSENGER_FILE, "rb");

    if (file == NULL)
    {
        return 0;
    }

    while (fread(passenger,
                 sizeof(Passenger),
                 1,
                 file) == 1)
    {
        if (passenger->passengerId == passengerId)
        {
            fclose(file);

            return 1;
        }
    }

    fclose(file);

    return 0;
}

/* =========================================================
   DISPLAY COMPLETE TICKET
   ========================================================= */

void displayTicket(Reservation reservation)
{
    Train train;
    Passenger passenger;


    printf("\n");
    printf("============================================================\n");
    printf("                 RAILWAY E-TICKET\n");
    printf("============================================================\n");


    printf("\nPNR        : %d",
           reservation.pnr);

    printf("\nStatus     : %s",
           reservation.status);


    if (findPassengerById(reservation.passengerId,
                          &passenger))
    {
        printf("\n\nPASSENGER");
        printf("\n---------------------------------------------");

        printf("\nID         : %d",
               passenger.passengerId);

        printf("\nName       : %s",
               passenger.name);

        printf("\nAge        : %d",
               passenger.age);

        printf("\nGender     : %c",
               passenger.gender);

        printf("\nPhone      : %s",
               passenger.phone);
    }


    if (findTrainByNumber(reservation.trainNo,
                          &train))
    {
        printf("\n\nTRAIN");
        printf("\n---------------------------------------------");

        printf("\nNumber     : %d",
               train.trainNo);

        printf("\nName       : %s",
               train.trainName);

        printf("\nRoute      : %s -> %s",
               train.source,
               train.destination);

        printf("\nDeparture  : %s",
               train.departure);

        printf("\nArrival    : %s",
               train.arrival);
    }


    printf("\n\nJOURNEY");
    printf("\n---------------------------------------------");

    printf("\nDate       : %s",
           reservation.journeyDate);

    printf("\nClass      : %s",
           reservation.classType);

    printf("\nSeat       : %d",
           reservation.seatNo);

    printf("\nFare       : %.2f",
           reservation.fare);


    printf("\n\n============================================================\n");
    printf("             Thank you for travelling!\n");
    printf("============================================================\n");
}
/* =========================================================
   SEARCH RESERVATIONS BY PASSENGER
   ========================================================= */

void searchReservationByPassenger(void)
{
    FILE *file;

    Reservation reservation;

    int passengerId;

    int found = 0;


    clearScreen();

    printf("\n");
    printf("=============================================\n");
    printf("       PASSENGER RESERVATION HISTORY\n");
    printf("=============================================\n");


    printf("\nEnter Passenger ID: ");
    scanf("%d", &passengerId);


    file = fopen(RESERVATION_FILE, "rb");

    if (file == NULL)
    {
        printf("\nNo reservation database found.");

        pauseScreen();

        return;
    }


    while (fread(&reservation,
                 sizeof(Reservation),
                 1,
                 file) == 1)
    {
        if (reservation.passengerId == passengerId)
        {
            found = 1;

            printf("\n---------------------------------------------");

            printf("\nPNR          : %d",
                   reservation.pnr);

            printf("\nTrain Number : %d",
                   reservation.trainNo);

            printf("\nSeat Number   : %d",
                   reservation.seatNo);

            printf("\nJourney Date  : %s",
                   reservation.journeyDate);

            printf("\nClass         : %s",
                   reservation.classType);

            printf("\nFare          : %.2f",
                   reservation.fare);

            printf("\nStatus        : %s",
                   reservation.status);

            printf("\n---------------------------------------------\n");
        }
    }


    fclose(file);


    if (!found)
    {
        printf("\nNo reservations found for Passenger ID %d.",
               passengerId);
    }


    pauseScreen();
}

/* =========================================================
   UPDATE RESERVATION STATUS
   ========================================================= */

int updateReservationStatus(int pnr, const char newStatus[])
{
    FILE *file;

    Reservation reservation;

    int updated = 0;


    file = fopen(RESERVATION_FILE, "rb+");

    if (file == NULL)
    {
        return 0;
    }


    while (fread(&reservation,
                 sizeof(Reservation),
                 1,
                 file) == 1)
    {
        if (reservation.pnr == pnr)
        {
            strcpy(reservation.status, newStatus);


            /*
               Move file pointer back to the beginning
               of the current reservation.
            */

            fseek(file,
                  -sizeof(Reservation),
                  SEEK_CUR);


            fwrite(&reservation,
                   sizeof(Reservation),
                   1,
                   file);


            updated = 1;

            break;
        }
    }


    fclose(file);

    return updated;
}

/* =========================================================
   CREATE DEFAULT ADMIN
   ========================================================= */

void createDefaultAdmin(void)
{
    FILE *file;

    Admin admin;


    /*
       Check whether admin database already exists.
    */

    file = fopen(ADMIN_FILE, "rb");

    if (file != NULL)
    {
        fclose(file);

        return;
    }


    /*
       Create default admin.
    */

    strcpy(admin.username, "admin");

    strcpy(admin.password, "admin123");


    file = fopen(ADMIN_FILE, "wb");

    if (file == NULL)
    {
        return;
    }


    fwrite(&admin,
            sizeof(Admin),
            1,
            file);


    fclose(file);
}

/* =========================================================
   CHECK ADMIN LOGIN
   ========================================================= */

int checkAdminLogin(const char username[],
                    const char password[])
{
    FILE *file;

    Admin admin;


    file = fopen(ADMIN_FILE, "rb");

    if (file == NULL)
    {
        return 0;
    }


    while (fread(&admin,
                 sizeof(Admin),
                 1,
                 file) == 1)
    {
        if (strcmp(admin.username, username) == 0 &&
            strcmp(admin.password, password) == 0)
        {
            fclose(file);

            return 1;
        }
    }


    fclose(file);

    return 0;
}

/* =========================================================
   ADMIN LOGIN
   ========================================================= */

void adminLogin(void)
{
    char username[30];
    char password[30];

    int attempts = 3;


    while (attempts > 0)
    {
        clearScreen();

        printf("\n");
        printf("=============================================\n");
        printf("                 ADMIN LOGIN\n");
        printf("=============================================\n");


        printf("\nUsername: ");
        scanf("%29s", username);


        printf("Password: ");
        scanf("%29s", password);


        if (checkAdminLogin(username,
                             password))
        {
            printf("\nLogin successful!");

            pauseScreen();


            /*
               Open Admin Dashboard
            */

            adminMenu();

            return;
        }


        attempts--;


        printf("\nIncorrect username or password!");

        printf("\nAttempts remaining: %d",
               attempts);


        if (attempts > 0)
        {
            pauseScreen();
        }
    }


    printf("\n\nToo many failed attempts.");

    printf("\nReturning to main menu...");

    pauseScreen();
}

/* =========================================================
   SAFE INTEGER INPUT
   ========================================================= */

int readInt(const char *prompt, int min, int max)
{
    char input[100];

    int value;

    char extra;


    while (1)
    {
        printf("%s", prompt);

        if (fgets(input, sizeof(input), stdin) != NULL)
        {
            /*
               %d reads an integer.
               %c checks whether extra characters exist.
            */

            if (sscanf(input, "%d %c", &value, &extra) == 1)
            {
                if (value >= min && value <= max)
                {
                    return value;
                }
            }
        }

        printf("Invalid input! Enter a value between %d and %d.\n",
               min,
               max);
    }
}

/* =========================================================
   SAFE FLOAT INPUT
   ========================================================= */

float readFloat(const char *prompt,
                float min,
                float max)
{
    char input[100];

    float value;

    char extra;


    while (1)
    {
        printf("%s", prompt);

        if (fgets(input, sizeof(input), stdin) != NULL)
        {
            if (sscanf(input, "%f %c", &value, &extra) == 1)
            {
                if (value >= min && value <= max)
                {
                    return value;
                }
            }
        }

        printf("Invalid input! Enter a value between %.2f and %.2f.\n",
               min,
               max);
    }
}

/* =========================================================
   SAFE STRING INPUT
   ========================================================= */

void readString(const char *prompt,
                char str[],
                int size)
{
    while (1)
    {
        printf("%s", prompt);

        if (fgets(str, size, stdin) != NULL)
        {
            /*
               Remove newline
            */

            str[strcspn(str, "\n")] = '\0';


            /*
               Don't accept empty strings
            */

            if (strlen(str) > 0)
            {
                return;
            }
        }

        printf("Input cannot be empty!\n");
    }
}

/* =========================================================
   READ GENDER
   ========================================================= */

char readGender(void)
{
    char input[20];

    char gender;


    while (1)
    {
        printf("Enter Gender (M/F/O): ");

        if (fgets(input, sizeof(input), stdin) != NULL)
        {
            if (sscanf(input, " %c", &gender) == 1)
            {
                if (gender >= 'a' &&
                    gender <= 'z')
                {
                    gender = gender - 'a' + 'A';
                }

                if (gender == 'M' ||
                    gender == 'F' ||
                    gender == 'O')
                {
                    return gender;
                }
            }
        }

        printf("Invalid gender! Use M, F or O.\n");
    }
}

/* =========================================================
   VALIDATE PHONE NUMBER
   ========================================================= */

int isValidPhone(const char phone[])
{
    int i;

    int length = strlen(phone);


    /*
       Indian-style 10 digit number
    */

    if (length != 10)
    {
        return 0;
    }


    for (i = 0; i < length; i++)
    {
        if (phone[i] < '0' ||
            phone[i] > '9')
        {
            return 0;
        }
    }


    return 1;
}

/* =========================================================
   READ PHONE NUMBER
   ========================================================= */

void readPhone(char phone[])
{
    while (1)
    {
        readString("Enter Phone Number: ",
                   phone,
                   15);

        if (isValidPhone(phone))
        {
            return;
        }

        printf("Invalid phone number!\n");
        printf("Enter exactly 10 digits.\n");
    }
}

/* =========================================================
   VALIDATE DATE
   ========================================================= */

int isValidDate(const char date[])
{
    int day;
    int month;
    int year;

    char extra;


    /*
       Check format
    */

    if (sscanf(date,
               "%d-%d-%d %c",
               &day,
               &month,
               &year,
               &extra) != 3)
    {
        return 0;
    }


    /*
       Basic range validation
    */

    if (year < 2026 ||
        year > 2100)
    {
        return 0;
    }


    if (month < 1 ||
        month > 12)
    {
        return 0;
    }


    if (day < 1 ||
        day > 31)
    {
        return 0;
    }


    /*
       Months having 30 days
    */

    if ((month == 4 ||
         month == 6 ||
         month == 9 ||
         month == 11) &&
        day > 30)
    {
        return 0;
    }


    /*
       February
    */

    if (month == 2)
    {
        int leap =
            (year % 400 == 0) ||
            (year % 4 == 0 &&
             year % 100 != 0);

        if (day > (leap ? 29 : 28))
        {
            return 0;
        }
    }


    return 1;
}

/* =========================================================
   READ DATE
   ========================================================= */

void readDate(const char *prompt,
              char date[])
{
    while (1)
    {
        readString(prompt,
                   date,
                   15);

        if (isValidDate(date))
        {
            return;
        }

        printf("Invalid date!\n");
        printf("Use DD-MM-YYYY format.\n");
    }
}

void systemSummary(void)
{
    FILE *file;

    Train train;
    Passenger passenger;
    Reservation reservation;

    int trains = 0;
    int passengers = 0;
    int reservations = 0;
    int confirmed = 0;
    int cancelled = 0;


    file = fopen(TRAIN_FILE, "rb");

    if (file != NULL)
    {
        while (fread(&train,
                     sizeof(Train),
                     1,
                     file) == 1)
        {
            trains++;
        }

        fclose(file);
    }


    file = fopen(PASSENGER_FILE, "rb");

    if (file != NULL)
    {
        while (fread(&passenger,
                     sizeof(Passenger),
                     1,
                     file) == 1)
        {
            passengers++;
        }

        fclose(file);
    }


    file = fopen(RESERVATION_FILE, "rb");

    if (file != NULL)
    {
        while (fread(&reservation,
                     sizeof(Reservation),
                     1,
                     file) == 1)
        {
            reservations++;

            if (strcmp(reservation.status,
                       "CONFIRMED") == 0)
            {
                confirmed++;
            }

            if (strcmp(reservation.status,
                       "CANCELLED") == 0)
            {
                cancelled++;
            }
        }

        fclose(file);
    }


    clearScreen();

    printf("\n");
    printf("=============================================\n");
    printf("             SYSTEM SUMMARY\n");
    printf("=============================================\n");

    printf("\nTotal Trains        : %d", trains);
    printf("\nTotal Passengers    : %d", passengers);
    printf("\nTotal Reservations  : %d", reservations);
    printf("\nConfirmed Tickets   : %d", confirmed);
    printf("\nCancelled Tickets   : %d", cancelled);

    printf("\n=============================================\n");

    pauseScreen();
}

/* =========================================================
   FUTURE FUNCTIONS
   ========================================================= */

/* =========================================================
   BOOK TICKET
   ========================================================= */

void bookTicket(void)
{
    FILE *file;

    Reservation reservation;
    Train train;

    int passengerId;
    int trainNo;

    char classChoice;
    char classType[20];

    int seatNo;
    float fare;


    clearScreen();

    printf("\n");
    printf("=============================================\n");
    printf("                 BOOK TICKET\n");
    printf("=============================================\n");


    /* Passenger */

    passengerId =
        readInt("Enter Passenger ID: ",
                1001,
                999999999);


    if (!passengerExists(passengerId))
    {
        printf("\nPassenger does not exist!");

        printf("\nPlease register the passenger first.");

        pauseScreen();

        return;
    }


    /* Train */

    trainNo =
        readInt("Enter Train Number: ",
                1,
                99999);


    if (!findTrainByNumber(trainNo, &train))
    {
        printf("\nTrain not found!");

        pauseScreen();

        return;
    }


    /* Seats */

    if (train.availableSeats <= 0)
    {
        printf("\nNo seats available!");

        pauseScreen();

        return;
    }


    printf("\n---------------------------------------------");
    printf("\nTrain Number    : %d", train.trainNo);
    printf("\nTrain Name      : %s", train.trainName);
    printf("\nRoute           : %s -> %s",
           train.source,
           train.destination);
    printf("\nAvailable Seats : %d",
           train.availableSeats);
    printf("\n---------------------------------------------");


    /* Date */

    readDate("\nEnter Journey Date (DD-MM-YYYY): ",
             reservation.journeyDate);


    /* Class */

    while (1)
    {
        printf("\n\nSelect Class:");
        printf("\nS. Sleeper");
        printf("\nA. AC");

        printf("\n\nEnter choice: ");

        scanf(" %c", &classChoice);

        if (classChoice == 'S' ||
            classChoice == 's')
        {
            strcpy(classType, "Sleeper");
            break;
        }

        if (classChoice == 'A' ||
            classChoice == 'a')
        {
            strcpy(classType, "AC");
            break;
        }

        printf("\nInvalid class!");
    }


    /* Fare */

    fare = calculateFare(train, classType);


    if (fare <= 0)
    {
        printf("\nUnable to calculate fare!");

        pauseScreen();

        return;
    }


    /* Seat */

    seatNo =
        getNextSeatNumber(trainNo,
                          reservation.journeyDate);


    if (seatNo > train.totalSeats)
    {
        printf("\nNo seat available for this journey.");

        pauseScreen();

        return;
    }


    /* Confirmation */

    printf("\n");
    printf("---------------------------------------------");
    printf("\nSeat Number : %d", seatNo);
    printf("\nClass       : %s", classType);
    printf("\nFare        : %.2f", fare);
    printf("\n---------------------------------------------");


    char confirm;

    printf("\nConfirm booking? (Y/N): ");
    scanf(" %c", &confirm);


    if (confirm != 'Y' &&
        confirm != 'y')
    {
        printf("\nBooking cancelled.");

        pauseScreen();

        return;
    }


    /* Prepare reservation */

    reservation.pnr =
        generatePNR();

    reservation.trainNo =
        trainNo;

    reservation.passengerId =
        passengerId;

    reservation.seatNo =
        seatNo;

    strcpy(reservation.classType,
           classType);

    reservation.fare =
        fare;

    strcpy(reservation.status,
           "CONFIRMED");


    /*
       Update train seats FIRST.
    */

    if (!updateAvailableSeats(trainNo, -1))
    {
        printf("\nUnable to reserve the seat.");

        pauseScreen();

        return;
    }


    /*
       Save reservation.
    */

    file = fopen(RESERVATION_FILE, "ab");

    if (file == NULL)
    {
        /*
           Roll back the seat.
        */

        updateAvailableSeats(trainNo, 1);

        printf("\nUnable to save reservation.");

        pauseScreen();

        return;
    }


    if (fwrite(&reservation,
               sizeof(Reservation),
               1,
               file) != 1)
    {
        fclose(file);

        /*
           Roll back seat.
        */

        updateAvailableSeats(trainNo, 1);

        printf("\nBooking failed.");

        pauseScreen();

        return;
    }


    fclose(file);


    /* Success */

    printf("\n");
    printf("=============================================\n");
    printf("       TICKET BOOKED SUCCESSFULLY\n");
    printf("=============================================\n");

    printf("\nPNR          : %d",
           reservation.pnr);

    printf("\nPassenger ID : %d",
           reservation.passengerId);

    printf("\nTrain        : %d",
           reservation.trainNo);

    printf("\nDate         : %s",
           reservation.journeyDate);

    printf("\nClass        : %s",
           reservation.classType);

    printf("\nSeat         : %d",
           reservation.seatNo);

    printf("\nFare         : %.2f",
           reservation.fare);

    printf("\nStatus       : CONFIRMED");

    printf("\n=============================================\n");

    pauseScreen();
}
/* =========================================================
   PNR ENQUIRY
   ========================================================= */

void checkPNR(void)
{
    FILE *file;

    Reservation reservation;

    int pnr;
    int found = 0;


    clearScreen();

    printf("\n");
    printf("=============================================\n");
    printf("                PNR ENQUIRY\n");
    printf("=============================================\n");


    printf("\nEnter PNR Number: ");
    scanf("%d", &pnr);


    file = fopen(RESERVATION_FILE, "rb");

    if (file == NULL)
    {
        printf("\nNo reservation records found.");

        pauseScreen();

        return;
    }


    while (fread(&reservation,
                 sizeof(Reservation),
                 1,
                 file) == 1)
    {
        if (reservation.pnr == pnr)
        {
            found = 1;

            displayTicket(reservation);

            break;
        }
    }


    fclose(file);


    if (!found)
    {
        printf("\nPNR %d not found!", pnr);
    }


    pauseScreen();
}


/* =========================================================
   CANCEL TICKET
   ========================================================= */

void cancelTicket(void)
{
    FILE *file;

    Reservation reservation;

    Train train;

    int pnr;

    int found = 0;


    clearScreen();


    printf("\n");
    printf("=============================================\n");
    printf("               CANCEL TICKET\n");
    printf("=============================================\n");


    printf("\nEnter PNR Number: ");
    scanf("%d", &pnr);


    /* =====================================================
       OPEN RESERVATION DATABASE
       ===================================================== */

    file = fopen(RESERVATION_FILE, "rb");

    if (file == NULL)
    {
        printf("\nNo reservation records found.");

        pauseScreen();

        return;
    }


    /* =====================================================
       SEARCH PNR
       ===================================================== */

    while (fread(&reservation,
                 sizeof(Reservation),
                 1,
                 file) == 1)
    {
        if (reservation.pnr == pnr)
        {
            found = 1;

            break;
        }
    }


    fclose(file);


    /* =====================================================
       PNR NOT FOUND
       ===================================================== */

    if (!found)
    {
        printf("\nPNR %d not found!", pnr);

        pauseScreen();

        return;
    }


    /* =====================================================
       CHECK CURRENT STATUS
       ===================================================== */

    if (strcmp(reservation.status, "CANCELLED") == 0)
    {
        printf("\nThis ticket has already been cancelled.");

        pauseScreen();

        return;
    }


    /* =====================================================
       DISPLAY TICKET
       ===================================================== */

    printf("\nTicket found!");

    displayTicket(reservation);


    /* =====================================================
       CONFIRM CANCELLATION
       ===================================================== */

    char confirm;

    printf("\nAre you sure you want to cancel this ticket? (Y/N): ");
    scanf(" %c", &confirm);


    if (confirm != 'Y' && confirm != 'y')
    {
        printf("\nCancellation aborted.");

        pauseScreen();

        return;
    }


    /* =====================================================
       FIND TRAIN
       ===================================================== */

    if (!findTrainByNumber(reservation.trainNo,
                           &train))
    {
        printf("\nTrain information could not be found.");

        pauseScreen();

        return;
    }


    /* =====================================================
       UPDATE RESERVATION
       ===================================================== */

    if (!updateReservationStatus(pnr,
                                 "CANCELLED"))
    {
        printf("\nUnable to cancel reservation.");

        pauseScreen();

        return;
    }


    /* =====================================================
       RESTORE SEAT
       ===================================================== */

    if (!updateAvailableSeats(reservation.trainNo, 1))
    {
        printf("\nWarning: Ticket cancelled, but seat count");
        printf(" could not be restored.");

        pauseScreen();

        return;
    }


    /* =====================================================
       SUCCESS
       ===================================================== */

    printf("\n");
    printf("=============================================\n");
    printf("          TICKET CANCELLED SUCCESSFULLY\n");
    printf("=============================================\n");

    printf("\nPNR          : %d",
           reservation.pnr);

    printf("\nTrain Number : %d",
           reservation.trainNo);

    printf("\nSeat Number  : %d",
           reservation.seatNo);

    printf("\nStatus       : CANCELLED");

    printf("\n=============================================\n");


    pauseScreen();
}

/* =========================================================
   VIEW ALL RESERVATIONS
   ========================================================= */

void viewReservations(void)
{
    FILE *file;

    Reservation reservation;

    int count = 0;


    clearScreen();

    printf("\n");
    printf("====================================================================================\n");
    printf("                              ALL RESERVATIONS\n");
    printf("====================================================================================\n");


    file = fopen(RESERVATION_FILE, "rb");

    if (file == NULL)
    {
        printf("\nNo reservation records found.");

        pauseScreen();

        return;
    }


    printf("\n%-12s %-10s %-12s %-8s %-15s %-12s %-12s\n",
           "PNR",
           "Train",
           "Passenger",
           "Seat",
           "Date",
           "Class",
           "Status");

    printf("------------------------------------------------------------------------------------\n");


    while (fread(&reservation,
                 sizeof(Reservation),
                 1,
                 file) == 1)
    {
        printf("%-12d %-10d %-12d %-8d %-15s %-12s %-12s\n",
               reservation.pnr,
               reservation.trainNo,
               reservation.passengerId,
               reservation.seatNo,
               reservation.journeyDate,
               reservation.classType,
               reservation.status);

        count++;
    }


    fclose(file);


    printf("\n------------------------------------------------------------------------------------\n");

    printf("Total reservations: %d\n",
           count);


    pauseScreen();
}