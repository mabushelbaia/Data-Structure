/*
 * Mohammad Abu-Shelbaia
 * 1200198
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define True 1

struct AVLNode;
typedef struct AVLNode *AVL_T;
typedef struct data { // Data for each node.
    char course_code[100], course_name[150], department[150], topics[500];
    int hours;
} Data;
struct AVLNode { // Tree Nodes
    Data data;
    AVL_T Left;
    AVL_T Right;
    int Height;
};

/*                          AVL METHODS                     */
void printInOrder(AVL_T t);
int getHeight(AVL_T t);
int getBalance(AVL_T tree);
AVL_T FindMin(AVL_T T);
AVL_T singleRotateLeft(AVL_T x);
AVL_T singleRotateRight(AVL_T y);
AVL_T doubleRotateLeft(AVL_T K3);
AVL_T doubleRotateRight(AVL_T K3);
AVL_T insert(AVL_T, Data data);

/*                          DEV METHODS                     */
void StoreFile(AVL_T t, FILE *file);
void lexicographic(AVL_T t);
void printDepartment(AVL_T t, char *department);
void editCourse(AVL_T t, char *course_code);
void insertCourse(AVL_T tree);
void printTopics(AVL_T t, char *course_code);
AVL_T LoadFile();
AVL_T deleteNode(AVL_T t, char *course_code);
AVL_T deleteDepartment(AVL_T t, char *department);
AVL_T deleteChar(AVL_T t, char ch);


int main() {
    char *menu = "1. Read the file courses.txt file and create the tree.\n"
                 "2. Insert a new course from the user with all its associated data.\n"
                 "3. Find a course and support updating of its information.\n"
                 "4. List courses in lexicographic order with their associated\n"
                 "information (credit hours, IDs, and topics).\n"
                 "5. List all topics associated with a given course.\n"
                 "6. List all courses in lexicographic order that belong to the same\n"
                 "department.\n"
                 "7. Delete a course.\n"
                 "8. Delete all courses that start with a specific letter.\n"
                 "9. Delete all courses belong to a given department.\n"
                 "10. Save all words in file offered_courses.txt\n";
    AVL_T tree = LoadFile();
    char temp[300], ch;
    while (True) {
        printf("%s", menu);
        printf("Enter a number between 1 - 10 or 0 to exit.\n");
        int option = 0;
        scanf("%d", &option);
        if (option == 0)
            break;
        switch (option) {
            case 1:
                tree = LoadFile();
                break;
            case 2:
                insertCourse(tree);
                break;
            case 3: {
                printf("Enter course code: ");
                scanf(" %[^\n]", temp);
                editCourse(tree, temp);
                break;
            }
            case 4:
                lexicographic(tree);
                break;
            case 5: {
                printf("Enter course code: ");
                scanf(" %[^\n]", temp);
                printTopics(tree, temp);
                break;
            }
            case 6: {
                printf("Enter course department: ");
                scanf(" %[^\n]", temp);
                printDepartment(tree, temp);
                break;
            }
            case 7: {
                printf("Enter course code: ");
                scanf(" %[^\n]", temp);
                deleteNode(tree, temp);
                break;
            }
            case 8: {
                printf("Enter character: ");
                scanf(" %c", &ch);
                deleteChar(tree, ch);
                break;
            }
            case 9: {
                printf("Enter department: ");
                scanf(" %[^\n]", temp);
                deleteDepartment(tree, temp);
                break;
            }
            case 10: {
                FILE *file = fopen("offered_courses.txt", "w");
                StoreFile(tree, file);
                break;
            }
            default:
                printf("Choose a valid number!\n");
        }

    }
}

AVL_T LoadFile() { // Loading Data from file into a tree.
    AVL_T tree = NULL;
    FILE *F = fopen("courses.txt", "r");
    char temp[1000];
    while (fgets(temp, 1000, F)) {
        Data data;
        strcpy(data.course_name, strtok(temp, ":"));
        data.hours = atoi(strtok(NULL, "#"));
        strcpy(data.course_code, strtok(NULL, "#"));
        strcpy(data.department, strtok(NULL, "/"));
        strcpy(data.topics, strtok(NULL, "\n"));
        tree = insert(tree, data);
    }
    return tree;
}

void StoreFile(AVL_T t, FILE *f) { // Storing data back into a file
    if (t != NULL) {
        StoreFile(t->Left, f);
        fprintf(f, "%s:%d#%s#%s/%s\n", t->data.course_name, t->data.hours, t->data.course_code, t->data.department,
                t->data.topics);
        StoreFile(t->Right, f);
    }
}

int getHeight(AVL_T t) {
    if (t == NULL)
        return -1;
    else
        return t->Height;
}

int MAX(int x, int y) {
    if (x > y)
        return x;
    else
        return y;
}

int getBalance(AVL_T tree) { // Left(Height) - Right(Height)
    return tree ? getHeight(tree->Left) - getHeight(tree->Right) : 0;
}

AVL_T FindMin(AVL_T T) { // Finding minimum node by going to max left.
    if (T != NULL)
        while (T->Left != NULL)
            T = T->Left;

    return T;
}

AVL_T singleRotateLeft(AVL_T x) { // I'm just putting a comment here.
    AVL_T y = x->Right;
    AVL_T T2 = y->Left;

    y->Left = x;
    x->Right = T2;
    // Just for the gray color.
    x->Height = 1 + MAX(getHeight(x->Left), getHeight(x->Right));
    y->Height = 1 + MAX(getHeight(x), getHeight(y->Right));

    return y;
}

AVL_T singleRotateRight(AVL_T y) { // Pre-Build Function
    AVL_T x = y->Left;
    AVL_T T2 = x->Right;

    x->Right = y;
    y->Left = T2;
    // Just a gray comment
    y->Height = 1 + MAX(getHeight(y->Right), getHeight(y->Left));
    x->Height = 1 + MAX(getHeight(y), getHeight(x->Left));

    return x;
}

AVL_T doubleRotateLeft(AVL_T K3) { // Pre-Built
    K3->Left = singleRotateLeft(K3->Left);
    return singleRotateRight(K3);
}

AVL_T doubleRotateRight(AVL_T K3) { // Pre-Build
    K3->Right = singleRotateRight(K3->Right);
    return singleRotateLeft(K3);
}

AVL_T insert(AVL_T t, Data data) { // Pre-Built
    if (t == NULL) {
        t = malloc(sizeof(struct AVLNode));
        t->data = data; //new edit here to ease the data transfer
        t->Height = 0;
        t->Left = t->Right = NULL;
    } else if (strcmp(data.course_code, t->data.course_code) < 0) { // Look to the left
        t->Left = insert(t->Left, data);
        if (getHeight(t->Left) - getHeight(t->Right) >= 2) { // Look to the right
            if (strcmp(data.course_code, t->Left->data.course_code) < 0)
                t = singleRotateRight(t);
            else
                t = doubleRotateLeft(t);
        }
    } else if (strcmp(data.course_code, t->data.course_code) > 0) { // Insert and balance
        t->Right = insert(t->Right, data);
        if (getHeight(t->Right) - getHeight(t->Left) >= 2) {
            if (strcmp(data.course_code, t->Right->data.course_code) > 0)
                t = singleRotateLeft(t);
            else
                t = doubleRotateRight(t);
        }
    } else {// Course already Exists
        printf("Course %s Exists\n", data.course_code);
    }

    t->Height = 1 + MAX(getHeight(t->Right), getHeight(t->Left));

    return t;
}

void printInOrder(AVL_T t) { // In-Order print
    if (t != NULL) {
        printInOrder(t->Left);
        printf("%s ", t->data.course_code);
        printInOrder(t->Right);
    }
}

void lexicographic(AVL_T t) { // In=Order print with data print
    if (t) {
        lexicographic(t->Left);
        printf("%d\t%s\t%s\t%s\t%s\n", t->data.hours, t->data.course_code, t->data.course_name, t->data.department,
               t->data.topics);
        lexicographic(t->Right);
    }
}

void printTopics(AVL_T t, char *course_code) {
    if (t == NULL) {
        printf("Course not listed\n");
        return;
    }
    if (strcmp(course_code, t->data.course_code) > 0) {
        printTopics(t->Right, course_code);
    } else if (strcmp(course_code, t->data.course_code) < 0) {
        printTopics(t->Left, course_code);
    } else {
        printf("%s\t%s", course_code, t->data.topics); // find the node
    }
}

void printDepartment(AVL_T t, char *department) { // in-order print for courses with the same department
    if (t) {
        printDepartment(t->Left, department);
        if (!strcmp(department, t->data.department))
            printf("%d\t%s\t%s\t%s\t%s\n", t->data.hours, t->data.course_code, t->data.course_name, t->data.department,
                   t->data.topics);
        printDepartment(t->Right, department);
    }
}

void editCourse(AVL_T t, char *course_code) {
    if (t == NULL) {
        printf("Course not listed\n");
        return;
    }
    if (strcmp(course_code, t->data.course_code) > 0) {
        editCourse(t->Right, course_code);
    } else if (strcmp(course_code, t->data.course_code) < 0) {
        editCourse(t->Left, course_code);
    } else { // Found the node
        printf("\n---------------------------------\n");
        printf("%d\t%s\t%s\t%s\t%s\n", t->data.hours, t->data.course_code, t->data.course_name, t->data.department,
               t->data.topics);
        char temp[300];
        printf("Change Course Name (new data or no): ");
        scanf(" %[^\n]", temp);
        !strcmp(temp, "no") ? NULL : strcpy(t->data.course_name, temp);
        printf("Change topics: (N or new data): ");
        scanf(" %[^\n]", temp);
        !strcmp(temp, "no") ? NULL : strcpy(t->data.topics, temp);
        printf("\n---------------------------------\n"); // insert and copy data
    }
}

void insertCourse(AVL_T tree) {
    Data data; // read data
    printf("Enter CourseCode: ");
    scanf(" %[^\n]", data.course_code);
    printf("Enter name: ");
    scanf(" %[^\n]", data.course_name);
    printf("Enter Department: ");
    scanf(" %[^\n]", data.department);
    printf("Enter topics: ");
    scanf(" %[^\n]", data.topics);
    int counter = 0;
    for (int i = 0; i < strlen(data.course_code); i++) { // get hours
        if (isdigit(data.course_code[i])) {
            counter++;
            if (counter == 2) {
                data.hours = data.course_code[i] - '0';
                break;
            }
        }
    }
    insert(tree, data); // insert into the tree
}

AVL_T deleteNode(AVL_T t, char *course_code) {
    if (t == NULL) {  // No node is found
        printf("Node not found\n");
        return t;
    }
    if (strcmp(course_code, t->data.course_code) < 0) // Look on the left side
        t->Left = deleteNode(t->Left, course_code);
    else if (strcmp(course_code, t->data.course_code) > 0) // look on the right side
        t->Right = deleteNode(t->Right, course_code);
    else { // Found the Node
        if ((t->Left == NULL) || (t->Right == NULL)) { // No children or 1 children case
            AVL_T temp = t->Left ? t->Left : t->Right; // Assign the temp to the children if exists

            if (temp == NULL) { // if there is no child
                temp = t;  // save it in a variable, so we could free it later
                t = NULL;
            } else {
                *t = *temp; // 1 children case, copy the child into the temp node.
            }
            free(temp); // delete the node
        } else { // 2 children case
            AVL_T temp = FindMin(t->Right); // Finding min element from the right side.
            t->data = temp->data; // copy the data from the minimum to the node we want to delete
            t->Right = deleteNode(t->Right, t->data.course_code); // delete the node we copied
        }
    }
    if (t == NULL) // if there is 1 node no need to balance
        return t;

    t->Height = 1 + MAX(getHeight(t->Left), getHeight(t->Right)); // get the balance from bottom to top
    int balance = getBalance(t); // Height(Left) - Height(Right)

    if (balance > 1 && getBalance(t->Left) >= 0) // left sub is larger by more than the right sub by 2
        return singleRotateRight(t);

    if (balance > 1 && getBalance(t->Left) < 0) {     // Left Right Case
        t->Left = singleRotateLeft(t->Left); // fix the left
        return singleRotateRight(t); // fix the right
    }

    if (balance < -1 && getBalance(t->Right) <= 0) // right sub is larger by more than the left sub by 2
        return singleRotateLeft(t);

    if (balance < -1 && getBalance(t->Right) > 0) {
        t->Right = singleRotateRight(t->Right); // fix the right
        return singleRotateLeft(t); // fix the left
    }

    return t; // return the tree

}

AVL_T deleteDepartment(AVL_T t, char *department) { // recursive call to delete all courses with given department
    if (t) {
        t->Left = deleteDepartment(t->Left, department);
        t->Right = deleteDepartment(t->Right, department);
        if (!strcmp(department, t->data.department))
            t = deleteNode(t, t->data.course_code);
    }
    return t;
}

AVL_T deleteChar(AVL_T t, char ch) { // recursive call to delete all courses whose code are starting with a given character
    if (t) {
        t->Left = deleteChar(t->Left, ch);
        t->Right = deleteChar(t->Right, ch);
        if (ch == t->data.course_code[0])
            t = deleteNode(t, t->data.course_code);
    }
    return t;
}