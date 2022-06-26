#include <stdio.h>
#include <stdlib.h>
#include <string.h>


struct Pass { // Passenger Node
    int id, date;
    char leave_time[20], from[20], to[20];
    struct Pass* next;
};


struct Bus { // Bus Node
    int id, date, price, capacity, size;
    char leave_time[20], from[20], to[20];
    struct Bus* next;
    struct Pass* passenger;
};


typedef struct Bus* BUS;
typedef struct Pass* PASS;

//  Functions
void trim(char *string);
void printBus(BUS list, int id);
void push_bus(BUS head, BUS temp);
void push_back(PASS head, PASS temp);
int load_passes(PASS waiting_list);
int load_busses(BUS head);
void print_pass(PASS pass);
void print_bus(BUS bus);
void print_busses(BUS head);
void assign(BUS head, PASS waiting);
void print_waiting(PASS waiting);
int bus_exists(BUS head, BUS temp);
int pass_exists(PASS head, PASS temp);
int deletePassenger(PASS head, int key);
int deleteBus(BUS head, PASS waiting, int key);
int deletePassenger_Bus(BUS head, int id);
void deleteRecursive(BUS head, int id);
int assigned_count = 0;


int main() {
    BUS head = (BUS) malloc(sizeof (struct Bus));
    head -> next = NULL;
    PASS waiting_list = (PASS) malloc(sizeof (struct Pass));
    waiting_list -> next = NULL;
    printf("1. Load the bus information file\n"
           "2. Load the passenger information file\n"
           "3. Assign passengers and print assignment information of all busses\n"
           "4. Print a specific bus information along with its passengers information (names and IDs)\n"
           "5. Print unmatched passengers\n"
           "6. Add new passenger\n"
           "7. Delete passenger\n"
           "8. Delete bus number\n"
           "9. Exit\n");
    int option;
    scanf("%d",&option);
    int bus_flag = 0, pass_flag = 0;
    int read_bus = 0, read_pass = 0;
    while(option != 9) {
        int id, date;
        char time[20], from[20], to[20];
        switch ( option ) {
            case 1:
                if( read_bus ) {
                    printf("Already Loaded!\n");
                    break;
                }
                bus_flag += load_busses(head);
                if( bus_flag ) printf("\nDONE: loaded %d busses.\n",bus_flag);
                read_bus = 1;
                break;
            case 2:
                if( read_pass ) {
                    printf("Already Loaded!\n");
                    break;
                }
                pass_flag += load_passes(waiting_list);
                if( pass_flag)  printf("\nDONE: loaded %d passengers.\n",pass_flag);
                read_pass = 1;
                break;
            case 3:
                if ( pass_flag && bus_flag ) {
                    assign(head,waiting_list);
                    print_busses(head);
                    printf("Count = %d\n",assigned_count);

                    break;
                }
                printf("Load 1 & 2 first!\n");
                break;
            case 4:
                if ( bus_flag ) {
                    printf("Enter Bus-num: ");
                    int bus_num;
                    scanf(" %d",&bus_num);
                    printf("------------------------------------- BUSSES ----------------------------------------\n");
                    printBus(head, bus_num);
                    break;
                }
                printf("Load 1 first\n");
                break;
            case 5:
                if ( pass_flag ) {
                    print_waiting(waiting_list);
                    break;
                }
                printf("Load 1\n");
                break;
            case 6:
                printf("Enter ID: ");
                scanf(" %d",&id);
                printf("Enter date: ");
                scanf(" %d",&date);
                printf("Enter time: ");
                scanf(" %s",time);
                printf("Enter from: ");
                scanf(" %s",from);
                printf("Enter to: ");
                scanf(" %s",to);
                PASS node = (PASS) malloc(sizeof (struct Pass));
                node -> next = NULL;
                node -> id = id;
                node -> date = date;
                trim(time); trim(from); trim(to);
                strcpy(node -> leave_time, time);
                strcpy(node -> from,from);
                strcpy(node -> to, to);
                if (pass_exists(waiting_list, node)){
                    printf("Passenger already exists!");
                    break;
                }
                push_back(waiting_list,node);
                printf("Passenger added to waiting list!\n");
                pass_flag++;
                break;
            case 7:
                if( assigned_count ) {
                    printf("enter pass-num: ");
                    scanf(" %d", &id);
                    deleteRecursive(head, id);
                    break;
                }
                printf("Load 3 first\n");
                break;
            case 8:
                if ( bus_flag ) {
                    int bus_num;
                    printf("Enter Bus-Num: ");
                    scanf(" %d",&bus_num);
                    assigned_count-= deleteBus(head,waiting_list,bus_num);
                    break;
                }
                break;
            case 9:
                printf("Bye!");
                break;
            case 10:
                print_busses(head);
                break;
            default:
                printf("Choose something :)\n");
                break;
        }
        scanf("%d",&option);
    }
    return 0;
}


int deleteBus(BUS head, PASS waiting, int key) { // returns the number of the removed passengers
    int count = 0;
    BUS p = head -> next;
    if (p == NULL) {
        printf("NULL POINTER\n");
        return 0;
    } else if (p->id == key) {
        while (p->passenger != NULL) {
            count++;
            PASS current = p->passenger;
            PASS next = p->passenger->next;
            p->passenger = next;
            current->next = NULL;
            push_back(waiting,current);
        }
        head->next = p -> next;
        free(p);
        printf("DONE!\n");
        return count;
    }
    BUS  q = p -> next;
    while(q != NULL) {
        if (q->id == key) {
            while (p->passenger != NULL) {
                PASS current = p->passenger;
                PASS next = p->passenger->next;
                p->passenger = next;
                current->next = NULL;
                push_back(waiting,current);
                count++;
            }
            p->next = q->next;
            free(q);
            printf("DONE!\n");
            return count;
        }
        p = p -> next;
        q = p->next;
    }
    printf("Bus not found\n");
}


int load_passes(PASS waiting_list) { // loads the file of passengers
    FILE *file = fopen("passengers.txt", "r");
    char temp[300];
    int n = 0;
    while (fgets(temp, 300, file)) {
        /* Function to tokenize the string */
        char tokens[5][(int) (300 / 5)];
        int k = 0, j = 0;
        for (int i = 0; i <= strlen(temp); i++) {
            if (temp[i] == '#' || temp[i] == '\0') {
                tokens[k][j] = '\0';
                k++;
                j = 0;
            } else {
                tokens[k][j] = temp[i];
                j++;
            }
        }
        for (int i = 0; i < 5; i++) trim(tokens[i]);
        PASS temp_pass = (PASS) malloc(sizeof(struct Pass));
        temp_pass->id = (int) strtol(tokens[0], NULL, 10);
        temp_pass->date = (int) strtol(tokens[1], NULL, 10);
        strcpy(temp_pass->leave_time, tokens[2]);
        strcpy(temp_pass->from, tokens[3]);
        strcpy(temp_pass->to, tokens[4]);
        temp_pass->next = NULL;
        if(pass_exists(waiting_list, temp_pass) == 0) { // checks if pass already exists on the waiting list
            push_back(waiting_list,temp_pass);
            n++;
        }
    }
    return n;
}


int load_busses(BUS head) { // loading busses file
    FILE* file= fopen("busses.txt", "r");
    int n = 0;
    char temp[300];
    while(fgets(temp, 300, file)) {
        /* Function to tokenize the string */
        char tokens[7][(int) (300/7)];
        int k=0,j=0;
        for(int i = 0; i <= strlen(temp); i++) {
            if (temp[i] == '#' || temp[i] == '\0') {
                tokens[k][j] = '\0';
                k++; j = 0;
            }
            else {
                tokens[k][j] = temp[i];
                j++;
            }
        }
        for(int i = 0; i < 7; i++) trim(tokens[i]);
        BUS temp_bus = (BUS) malloc(sizeof (struct Bus));
        temp_bus->id = (int) strtol(tokens[0], NULL, 10);
        temp_bus->date = (int) strtol(tokens[1], NULL, 10);
        strcpy(temp_bus->leave_time, tokens[2]);
        strcpy(temp_bus->from, tokens[3]);
        strcpy(temp_bus->to, tokens[4]);
        temp_bus->price = (int) strtol(tokens[5], NULL, 10);
        temp_bus->capacity = (int) strtol(tokens[6], NULL, 10);
        temp_bus->passenger = NULL;
        temp_bus->next =  NULL;
        temp_bus->size = 0;
        if( (bus_exists(head,temp_bus)) == 0) {
            push_bus(head, temp_bus);
            n++;
        }
        else printf("BUS Exists");
    }
    return n;
}


void assign(BUS head, PASS waiting) { // assigning people in the waiting list to the busses accordingly
    if (head->next == NULL || waiting->next == NULL) {
        printf("Either there is no busses or no passengers!\n");
        return;
    }
    PASS pass_list = waiting->next;
    while(pass_list != NULL) {
        BUS bus_list = head->next;
        while(bus_list != NULL) {
            int date_flag = (bus_list->date == pass_list->date);
            int time_flag = !strcmp(bus_list->leave_time, pass_list->leave_time);
            int from_flag = !strcmp(bus_list->from, pass_list->from);
            int to_flag = !strcmp(bus_list->to, pass_list->to);
            int size_flag = (bus_list->capacity - bus_list->size);
            if ( date_flag && time_flag && from_flag && to_flag && size_flag ) {
                PASS temp_pass = malloc(sizeof (struct Pass));
                memcpy(temp_pass, pass_list, sizeof (struct Pass));
                temp_pass->next = NULL;
                if(bus_list->passenger == NULL) {
                    bus_list->passenger = temp_pass;
                }
                else {
                    temp_pass->next = bus_list->passenger;
                    bus_list->passenger = temp_pass;
                }
                bus_list->size++;
                deletePassenger(waiting, temp_pass->id);
                assigned_count++;
                break;
            }
            bus_list = bus_list->next;
        }
        pass_list = pass_list->next;
    }
}


void print_bus(BUS bus) { // printing method
    printf("|%10d\t|%10d\t|%10s\t|%10s\t|%10s\t|%10d\t|%6d/%d\t|\n", bus->id, bus->date, bus->leave_time, bus->from, bus->to, bus->price, bus->size, bus->capacity);
    printf("|\t\t\t((PASSENGERS: ID)) \t\t\t\t\t\t\t\t\t\t\t\t\t\t|");
    PASS q = bus->passenger;
    int i=0;
    while( q != NULL ) {
        if (i % 3 == 0) {
            if (i > 0) printf("\t\t\t\t\t\t\t\t\t\t\t\t|");
            printf("\n|\t\t\t");
        }
        printf("%d\t", q->id);
        q = q->next;
        i++;
        if ( q == NULL) {
            if (!(i%3)) printf("\t\t\t\t\t\t\t\t\t\t\t\t|");
            if (i%3 == 2) printf("\t\t\t\t\t\t\t\t\t\t\t\t\t\t|");
            if (i%3 == 1) printf("\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t|");
        }
    }
    printf("\n-------------------------------------------------------------------------------------\n");
}


void printBus(BUS list, int id) { // printing method
    BUS p = list->next;
    while ( p != NULL) {
        if (p->id == id) {
            print_bus(p);
            return;
        }
        p=p->next;
    }
    printf("Bus not found!\n");
}


void print_pass(PASS pass) { // printing method
    printf("|%10d\t|%10d\t|%10s\t|%10s\t|%10s\t|\n", pass->id, pass->date, pass->leave_time, pass->from, pass->to);
}


void print_busses(BUS head) { // printing all busses
    printf("------------------------------------- BUSSES ----------------------------------------\n|");
    printf("%10s\t|%10s\t|%10s\t|%10s\t|%10s|%10s\t|%10s\t|\n","ID","DATE","DEPART","FROM","TO","TICKET","CAPACITY");
    printf("-------------------------------------------------------------------------------------\n");
    BUS p = head->next;
    int i = 0;
    while( p != NULL ) {
        print_bus(p);
        p = p->next;
        i++;
    }
    printf("-------------------------------------------------------------------------------------\n");
    printf("%d busses\n",i);
}


void print_waiting(PASS waiting) { // printing people on waiting list
    printf("--------------------------- QUEUE ---------------------------\n|");
    printf("%10s\t|%10s\t|%10s\t|%10s\t|%10s\t|\n","ID","DATE","DEPART","FROM","TO");
    printf("-------------------------------------------------------------\n");
    PASS q = waiting->next;
    int i = 0;
    while( q != NULL) {
        print_pass(q);
        q = q->next;
        i++;
    }
    printf("-------------------------------------------------------------\n");
    printf("%d on Queue\n",i);
}


void push_bus(BUS head, BUS temp) { // function to join busses nodes
    if (head->next == NULL) {
        head->next = temp;
    } else {
        temp->next = head->next;
        head->next = temp;
    }
}


void push_back(PASS head, PASS temp) { // function to join passenger nodes.
    PASS ptr = head;
    while(ptr->next != NULL) {
        ptr = ptr->next;
    }
    ptr->next = temp;
}


int bus_exists(BUS head, BUS temp) { // check if the bus already exists
    if(temp == NULL) {
        printf("NULL POINTER!\n");
        return 0;
    }
    BUS p = head->next;
    while (p != NULL) {
        if ( p->id == temp->id && p->date == temp->date && !strcmp(p->leave_time,temp->leave_time)) {
            return 1;
        }
        p = p->next;
    }
    return 0;
}


int pass_exists(PASS head, PASS temp) { // check if the passenger already exists
    if( temp == NULL) {
        printf("NULL POINTER!\n");
        return -1;
    }
    PASS p = head->next;
    while (p != NULL) {
        if ( p->id == temp->id && p->date == temp->date && !strcmp(p->leave_time,temp->leave_time)) {
            return 1;
        }
        p = p->next;
    }
    return 0;
}

void deleteRecursive(BUS head, int id) { //deleting all passengers with id recursively
    while(deletePassenger_Bus(head,id));
}
int deletePassenger_Bus(BUS head, int id) { // deleting passengers from busses
    BUS curr_bus = head->next;
    while ( curr_bus != NULL ) {
        PASS curr_passenger = curr_bus -> passenger;
        if ( curr_passenger -> id == id ) {
            curr_bus -> passenger = curr_bus-> passenger -> next;
            assigned_count -= 1;
            free(curr_passenger);
            printf("Done!\n");
            return 1;
        }
        if (deletePassenger(curr_passenger, id)) {
            assigned_count -= 1;
            printf("Done!\n");
            return 1;
        }
        else curr_bus = curr_bus -> next;
    }
    printf("Pass not found!\n");
    return 0;
}


int deletePassenger(PASS head, int key) { // deleting passenger from a linked list of passengers
    PASS p = head->next;
    if (p == NULL) {
        return 0;
    } else if (p->id == key) {
        head->next = p->next;
        free(p);
        return 1;
    }
    PASS q = p -> next;
    while(q != NULL) {
        if (q->id == key) {
            p->next = q->next;
            free(q);
            return 1;
        }
        p = p -> next;
        q = p->next;
    }
    return 0;
}


void trim(char *string) { // function to trim leading and trailing white spaces from an input
    int index = 0;
    while(string[index] == ' ' || string[index] == '\t' || string[index] == '\n') index++;
    int i;
    for (i=0;string[i + index] != '\0';i++) string[i] = string[i + index]; //shifting the string  to the left (index) times.
    string[i] = '\0'; // terminating the shift.
    i = 0;
    index = -1;
    while(string[i] != '\0') {
        if(string[i] != ' ' && string[i] != '\t' && string[i] != '\n') index = i; //finding last non-space character
        i++;
    }
    string[index + 1] = '\0'; // ending the string after the lat non-space character
}