/*
 * Mohammad Abu-Shelbaia
 * 1200198
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

/* DEFINITIONS & GLOBAL VARIABLES */
typedef struct data *Course;
#define STRING_SIZE 500
#define Deleted (Course)(0xFFFFFFFFFAAAA07)
typedef char String[STRING_SIZE];
int TABLE_LOADS = 0;
int TABLE_LOADS_DOUBLE = 0;
int TABLE_SIZE = 0;
int TABLE_SIZE_DOUBLE = 0;
int OLD_SIZE = 0;
int OLD_SIZE_DOUBLE = 0;
int QuadraticCollisions = 0;
int DoubleCollisions = 0;
Course *QuadraticTable = NULL;
Course *DoubleTable = NULL;
FILE *data_in;
#define True 1
#define False 0

/* NODE */
struct data {
    String course_code, course_name, department, topics;
    int hours;
};


/* PROTOTYPES */
void loadFile();
void init_tables();
void printTables();
void printInfo();
int hashFunction(const String in, int table_size);
unsigned int hashFunction_2(const String in, int table_size);
int isPrime(int n);
int nextPrime(int n);
float loadFactor();
Course QuadraticDelete(const String name, const String code);
Course QuadraticFind(const String name, const String code);
int QuadraticInsert(Course table[], Course course);
int DoubleInsert(Course table[], Course course);
Course DoubleDelete(const String name, const String code);
Course DoubleFind(const String name, const String code);
void reHash_DOUBLE();
void reHash();
void StoreFile();
void printFunctions();
void insertRecord();
void searchRecords();
void deleteRecord();
void printCourse(Course pData);

int main() {
    TABLE_SIZE = TABLE_SIZE_DOUBLE = 13; // INITIAL SIZE
    QuadraticTable = (Course *) malloc((sizeof(Course)) * TABLE_SIZE);
    DoubleTable = (Course *) malloc((sizeof(Course)) * TABLE_SIZE);
    init_tables();
    loadFile();
    char * menu = "1. Print hashed tables (including empty spots).\n"
                  "2. Print out table size and the load factor.\n"
                  "3. Print out the used hash functions.\n"
                  "4. Insert a new record to hash table (insertion will be done on both hash tables).\n"
                  "5. Search for a specific word (specify which table to search in).\n"
                  "6. Delete a specific record (from both tables).\n"
                  "7. Compare between the two methods in terms of number of collisions occurred.\n"
                  "8. Save hash table back to a file named saved_courses.txt\n"
                  "0. Quit\n\n";
    while(True) {
        printf("%s",menu);
        int input = 0;
        scanf("%d", &input); // NOLINT(cert-err34-c)
        if (input == 0) break;
        switch (input) {
            case 1: printTables();break;
            case 2: printInfo();break;
            case 3: printFunctions();break;
            case 4: insertRecord(); break;
            case 5: searchRecords(); break;
            case 6: deleteRecord();break;
            case 7: printInfo(); break;
            case 8: StoreFile(); break;
            default: printf("Enter a valid option\n");break;

        }
    }

}

void deleteRecord() {
    String course_code, course_name; // reading data
    printf("Enter CourseCode: ");
    scanf(" %[^\n]", course_code);
    printf("Enter name: ");
    scanf(" %[^\n]", course_name);
    if (QuadraticDelete(course_name, course_code) != NULL) { // deleting from first=table
        printf("DELETED SUCCESSFULLY FROM QUAD\n");
    } else {
        printf("NODE NOT FOUND IN QUAD\n");
    }
    if (DoubleDelete(course_name, course_code) != NULL) {// deleting from second=table
        printf("DELETED SUCCESSFULLY FROM DOUBLE\n\n");
    } else {
        printf("NODE NOT FOUND IN DOUBLE\n\n");
    }
}

void searchRecords() {
    String input;
    printf("Enter which table to search from DOUBLE,QUADRATIC\n"); // reading data
    scanf(" %[^\n]", input);
    String course_code, course_name;
    printf("Enter CourseCode: ");
    scanf(" %[^\n]", course_code);
    printf("Enter name: ");
    scanf(" %[^\n]", course_name);
    printf("\n");
    printCourse(!strcasecmp(input, "double")?DoubleFind(course_name, course_code): QuadraticFind(course_name, course_code)); // ternary operator
    printf("\n");
}

void insertRecord() {
    Course data = malloc(sizeof(struct data)); // reading data
    printf("Enter CourseCode: ");
    scanf(" %[^\n]", data->course_code);
    printf("Enter name: ");
    scanf(" %[^\n]", data->course_name);
    printf("Enter Department: ");
    scanf(" %[^\n]", data->department);
    printf("Enter topics: ");
    scanf(" %[^\n]", data->topics);
    int counter = 0;
    for (int i = 0; i < strlen(data->course_code); i++) { // get hours
        if (isdigit(data->course_code[i])) {
            counter++;
            if (counter == 2) {
                data->hours = data->course_code[i] - '0';
                break;
            }
        }
    }
    QuadraticCollisions += QuadraticInsert(QuadraticTable, data); // inserting
    DoubleCollisions += DoubleInsert(DoubleTable, data);
}

void printFunctions() {
    printf("Summation[i](0 -> n - 1)  [String[i] * ((i * 26 )+ 1) % TABLE_SIZE], String[i] -> ASCII and n -> Length\n");
    printf("\t\t Used For Quadratic and second hashing for Double\n");
    printf("Summation[i](0 -> n - 1)  [String[ n - i -1 ] * 32^i % TABLE_SIZE], String[i] -> ASCII and n -> Length\n");
    printf("\t\t Used as first hashing for Double\n\n");
}

void printCourse(Course pData) {
    if (pData == NULL) return;
    printf("%s\t%s\t%s\t%s\t%d\n", pData->course_code, pData->course_name, pData->department, pData->topics,
           pData->hours);
}

void reHash() {
    TABLE_LOADS = 0; // Resting entries size
    OLD_SIZE = TABLE_SIZE; // saving old size
    TABLE_SIZE = nextPrime(OLD_SIZE * 2); // setting new size
    Course *tempTable = (Course *) malloc((sizeof(Course)) * TABLE_SIZE); // creating an array with bigger size
    for (int i = 0; i < TABLE_SIZE; ++i) { //initializing table
        tempTable[i] = NULL;
    }
    for (int i = 0; i < OLD_SIZE; ++i) {
        if (QuadraticTable[i] != NULL && QuadraticTable[i] != Deleted) {
            QuadraticCollisions += QuadraticInsert(tempTable, QuadraticTable[i]); //inserting nodes
        }
    }
    free(QuadraticTable); // freeing the old table
    QuadraticTable = tempTable; //assigning the new table to the bigger table
}
void reHash_DOUBLE() { // same as above just with different variables
    TABLE_LOADS_DOUBLE = 0;
    OLD_SIZE_DOUBLE = TABLE_SIZE_DOUBLE;
    TABLE_SIZE_DOUBLE = nextPrime(OLD_SIZE_DOUBLE * 2);
    Course *temp = (Course *) malloc((sizeof(Course)) * TABLE_SIZE_DOUBLE);
    for (int i = 0; i < TABLE_SIZE_DOUBLE; ++i) {
        temp[i] = NULL;
    }
    for (int i = 0; i < OLD_SIZE_DOUBLE; ++i) {
        if (DoubleTable[i] != NULL && DoubleTable[i] != Deleted) {
            DoubleCollisions += DoubleInsert(temp, DoubleTable[i]);
        }
    }
    free(DoubleTable);
    DoubleTable = temp;
}


float loadFactor() { // self explanatory
    return (float) TABLE_LOADS / TABLE_SIZE;
}
float loadFactor_DOUBLE() {
    return (float) TABLE_LOADS_DOUBLE / TABLE_SIZE_DOUBLE;
}
void init_tables() { // initializing tables
    for (int i = 0; i < TABLE_SIZE; i++) {
        QuadraticTable[i] = NULL;
    }
    for (int i = 0; i < TABLE_SIZE_DOUBLE; ++i) {
        DoubleTable[i] = NULL;
    }
}

void printTables() { // just printing method
    printf("\n\n\n%s\t\t\t\t\t\t\t%10s\n", "QUADRATIC", "DOUBLE");
    int i;
    for (i = 0; i < TABLE_SIZE && i < TABLE_SIZE_DOUBLE; ++i) {
        if (QuadraticTable[i] == NULL) printf("%d\t=>\t%25s", i, "---");
        else if (QuadraticTable[i] == Deleted) printf("%d\t=>\t%25s", i, "(DELETED)");
        else printf("%d\t=>\t%25s", i, QuadraticTable[i]->course_name);
        printf("\t\t\t");
        if (DoubleTable[i] == NULL) printf("%d\t=>\t%25s\n", i, "---");
        else if (DoubleTable[i] == Deleted) printf("%d\t=>\t%25s\n", i, "(DELETED)");
        else printf("%d\t=>\t%25s\n", i, DoubleTable[i]->course_name);
    }

    for (; i < TABLE_SIZE; i++){
        if (QuadraticTable[i] == NULL) printf("%d\t=>\t%25s", i, "---");
        else if (QuadraticTable[i] == Deleted) printf("%d\t=>\t%25s", i, "(DELETED)");
        else printf("%d\t=>\t%25s\n", i, QuadraticTable[i]->course_name);
    }
    for(;i<TABLE_SIZE_DOUBLE;i++){
        printf("\t\t\t\t");
        if (DoubleTable[i] == NULL) printf("%d\t=>\t%25s\n", i, "---");
        else if (DoubleTable[i] == Deleted) printf("%d\t=>\t%25s\n", i, "(DELETED)");
        else printf("%d\t=>\t%25s\n", i, DoubleTable[i]->course_name);
    }
    printf("SIZE: %d\tLOAD FACTOR: %f\t\t\tSIZE: %d\tLOAD FACTOR: %f\n",TABLE_SIZE, loadFactor(), TABLE_SIZE_DOUBLE, loadFactor_DOUBLE());
    printf("COLLISIONS: %d\t\t\t\t\t\tCOLLISIONS: %d\n\n\n",QuadraticCollisions, DoubleCollisions);

}
void printInfo() { // printing method
    printf("SIZE: %d\tLOAD FACTOR: %f\t\t\tSIZE: %d\tLOAD FACTOR: %f\n",TABLE_SIZE, loadFactor(), TABLE_SIZE_DOUBLE, loadFactor_DOUBLE());
    printf("COLLISIONS: %d\t\t\t\t\t\tCOLLISIONS: %d\n\n\n",QuadraticCollisions, DoubleCollisions);
}
int hashFunction(const String in, int table_size) { // first hash Summation[i](0 -> n - 1)  [String[i] * ((i * 26 )+ 1) % TABLE_SIZE]
    int sum = 0, i = 0;
    while (in[i] != '\0') {
        sum += in[i] * (i * 26 + 1) % table_size;
        i++;
    }
    return (sum % table_size);
}

unsigned int hashFunction_2(const String in, int table_size) { // second hash Summation[i](0 -> n - 1)  [String[ n - i -1 ] * 32^i % TABLE_SIZE], String[i]
    unsigned int sum = 0;
    int i = 0;
    while (in[i] != '\0') {
        sum = (sum << 5) + in[i];
        i++;
    }
    if (sum < 0) sum += table_size;
    return (sum % table_size);
}


int QuadraticInsert(Course table[], Course course) {
    if (loadFactor() > 0.7) reHash();
    int QuadCollisionCounter = 0;
    if (course == NULL) return False; //ok
    int index = hashFunction(course->course_name, TABLE_SIZE);
    for (int i = 0; i < TABLE_SIZE; i++) {
        int reIndex = (i * i + index) % TABLE_SIZE; // quadratic eq

        if (table[reIndex] == NULL) { //finding an empty cell
            table[reIndex] = course;
            TABLE_LOADS += 1;
            return QuadCollisionCounter;
        }
        QuadCollisionCounter++;
    }
    printf("Couldn't insert %s\t%s at QuadTable\n",course->course_code, course->course_name); // just in-case
    return QuadCollisionCounter;
}

int DoubleInsert(Course table[], Course course) { // same as above
    if (loadFactor_DOUBLE() > 0.7) reHash_DOUBLE();
    int DoubleCollisionCounter = 0;
    if (course == NULL) return False;
    int index_x = hashFunction(course->course_name, TABLE_SIZE);
    int index_y = hashFunction_2(course->course_name, TABLE_SIZE);
    for (int i = 0; i < TABLE_SIZE; i++) {
        int reIndex = (index_x + i * index_y) % TABLE_SIZE;

        if (table[reIndex] == NULL) {
            table[reIndex] = course;
            TABLE_LOADS_DOUBLE += 1;
            return DoubleCollisionCounter;
        }
        DoubleCollisionCounter++;
    }
    printf("Couldn't insert %s\t%s at DoubleTable\n",course->course_code, course->course_name);
    return DoubleCollisionCounter;
}

Course DoubleFind(const String name, const String code) { //same as insert, but we go until we find a node with the same name
    int index_x = hashFunction(name, TABLE_SIZE);
    int index_y = hashFunction_2(name, TABLE_SIZE);
    for (int i = 0; i < TABLE_SIZE; ++i) {
        int reIndex = (index_x + i * index_y) % TABLE_SIZE;
        if (DoubleTable[reIndex] == NULL) return NULL;
        if (DoubleTable[reIndex] == Deleted) continue;
        if (!strcmp(code, DoubleTable[reIndex]->course_code)) {
            return DoubleTable[reIndex];
        }
    }
    return NULL;
}

Course DoubleDelete(const String name, const String code) { // finding the node when we find it we set it to an address which indicates that its deleted
    int index_x = hashFunction(name, TABLE_SIZE);
    int index_y = hashFunction_2(name, TABLE_SIZE);
    for (int i = 0; i < TABLE_SIZE; ++i) {
        int reIndex = (index_x + i * index_y) % TABLE_SIZE;
        if (DoubleTable[reIndex] == NULL) return NULL;
        if (DoubleTable[reIndex] == Deleted) continue;
        if (!strcmp(code, DoubleTable[reIndex]->course_code)) {
            Course temp = DoubleTable[reIndex];
            DoubleTable[reIndex] = Deleted;
            TABLE_LOADS_DOUBLE -= 1;
            return temp;
        }

    }
    return NULL;
}

Course QuadraticFind(const String name, const String code) { // same as the other
    int index = hashFunction(name, TABLE_SIZE);
    for (int i = 0; i < TABLE_SIZE; ++i) {
        int reIndex = (index + i * i) % TABLE_SIZE;
        if (QuadraticTable[reIndex] == NULL) return NULL;
        if (QuadraticTable[reIndex] == Deleted) continue;
        if (!strcmp(code, QuadraticTable[reIndex]->course_code)) {
            return QuadraticTable[reIndex];
        }
    }
    return NULL;
}


Course QuadraticDelete(const String name, const String code) { //same as the other
    int index = hashFunction(name, TABLE_SIZE);
    for (int i = 0; i < TABLE_SIZE; ++i) {
        int reIndex = (index + i * i) % TABLE_SIZE;
        if (QuadraticTable[reIndex] == NULL) return NULL;
        if (QuadraticTable[reIndex] == Deleted) continue;
        if (!strcmp(code, QuadraticTable[reIndex]->course_code)) {
            Course temp = QuadraticTable[reIndex];
            QuadraticTable[reIndex] = Deleted;
            TABLE_LOADS--;
            return temp;
        }

    }
    return NULL;
}

int isPrime(int n) { // self-explanatory
    if (n <= 1) return False;
    if (n <= 3) return True;

    for (int j = 2; j < n; j++) {
        if (n % j == 0) return False;
    }
    return True;
}

int nextPrime(int n) {
    if (n <= 1) return 2;
    int prime = n;
    while (!isPrime(prime)) {
        prime++;
    }
    return prime;
}



void loadFile() { //loading file
    data_in = fopen("offered_courses.txt", "r");
    String temp;
    while (fgets(temp, 500, data_in)) {
        Course course = malloc(sizeof(struct data));
        strcpy(course->course_name, strtok(temp, ":"));
        course->hours = atoi(strtok(NULL, "#")); // NOLINT(cert-err34-c)
        strcpy(course->course_code, strtok(NULL, "#"));
        strcpy(course->department, strtok(NULL, "/"));
        strcpy(course->topics, strtok(NULL, "\n"));
        QuadraticCollisions += QuadraticInsert(QuadraticTable, course); //inserting node
        DoubleCollisions += DoubleInsert(DoubleTable, course); //inserting node
    }
    fclose(data_in);
}
void StoreFile() { // Storing data back into a file
    FILE *output_file = fopen("saved_courses.txt", "w");
    for(int i=0;i< TABLE_SIZE_DOUBLE;i++) {
        if(DoubleTable[i] != NULL && DoubleTable[i] != Deleted) // inserting non-null nodes
            fprintf(output_file, "%s:%d#%s#%s/%s\n", DoubleTable[i]->course_name, DoubleTable[i]->hours, DoubleTable[i]->course_code, DoubleTable[i]->department,DoubleTable[i]->topics);
    }
}