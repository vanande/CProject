#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <winsock.h>
#include <MYSQL/mysql.h>
#include <curl/curl.h>

struct Subject {
    char *name;
    int numOptions;
    char *options[];
};


int connectDatabase(MYSQL *mysql);
void disconnectDatabase(MYSQL * mysql);
void getLastTweet(char name[]);
void getAstrological(char astrological[30]);
int funcUsed();
void inputString(char* string, int size);
void finish_with_error(MYSQL *con);
int createUser(char * name, MYSQL *mysql);
int lookForUser(char *name, MYSQL *mysql);
int login(char name[30], MYSQL *mysql);
struct Subject *createSubject(char *name, int numOptions, char **options);
void printSubject(struct Subject *subject);
void addSubject(struct Subject ***subjects, int *numSubjects, char *name, int numOptions, char **options);
void freeSubjects(struct Subject **subjects, int numSubjects);
void coolPrint(char str[], char ** argv);




int connectDatabase(MYSQL *mysql){
    if (mysql_real_connect(mysql, "localhost", "root", "", NULL, 3307, NULL, 0)) {
        printf("MySQL client version: %s\n", mysql_get_client_info());
        mysql_query(mysql, "CREATE DATABASE IF NOT EXISTS cProject");
        mysql_query(mysql, "USE cProject");
        return 1;
    } else {
        printf("Error connecting to database!");
        return 0;
    }
}

void disconnectDatabase(MYSQL * mysql){
    mysql_close(mysql);
}

void getLastTweet(char name[30]){
    printf("\nIn GetLastTweet");

    int a;
    srand(time(NULL));
    a = rand()%2;
    printf("\nGetting %s's last tweet", name);
    sleep(2);
    printf("\nHere is %s's last Tweet: ", name);
    printf("\nGetting the answer to your universal question...");
    sleep(2);
    if (!a)
        printf("\nYou definitely should do it according to %s",name);
    else
        printf("\nYou definitely shouldn't do it according to %s",name);
}

void getAstrological(char astrological[30]){
    printf("\nIn GetAstrological");

    int a;
    srand(time(NULL));
    a = rand()%2;
    printf("\nGetting the great %s opinion", astrological);
    sleep(2);
    printf("\nHere is what %s says ", astrological);
    printf("\nGetting the answer to your universal question...");
    sleep(2);
    if (!a)
        printf("\nYou definitely should do it according to %s",astrological);
    else
        printf("\nYou definitely shouldn't do it according to %s",astrological);
}

int funcUsed(){
    int algo;
    printf("\nWhere to look for your answer?");
    printf("\n1 ... My astrological");
    printf("\n2 ... Elon's tweet");
    scanf("%d", &algo);
    if (algo == 1){
        getAstrological("scorpion");
    }
    if (algo == 2){
        getLastTweet("elon");

    }
}
void inputString(char* string, int size){
    fgets(string, size, stdin);
    if(string[strlen(string)-1] == '\n')
        string[strlen(string)-1] = '\0';
    else
        fflush(stdin);
}


void finish_with_error(MYSQL *con){
    fprintf(stderr, "%s\n", mysql_error(con));
    mysql_close(con);
    exit(1);
}

void coolPrint(char str[], char ** argv){
    int sleepTime;
    for (int i = 0; i < strlen(str); i++){
        if(str[i] == ' '){
            printf("%c",str[i]);
            if(argv[1] != NULL){
                sleepTime = argv[1];
                sleep(sleepTime);
            } else {
                Sleep(500);
            }
        } else {
            printf("%c",str[i]);
            Sleep(70);
        }
    }
}

int createUser(char * name, MYSQL *mysql){
    char stmt_insert[50] = "INSERT INTO user(name) VALUES ('";

    strcat(stmt_insert, name);
    strcat(stmt_insert, "')");

    mysql_query(mysql, stmt_insert);

    if (!mysql_affected_rows(mysql)) {
        finish_with_error(mysql);
        return 1;
    }
    return 0;
}


int lookForUser(char *name, MYSQL *mysql){
    char stmt_select[50] = "SELECT * FROM user WHERE name = '";

    strcat(stmt_select, name);
    strcat(stmt_select, "'");

    if (mysql_query(mysql, stmt_select))
    {
        finish_with_error(mysql);
    }

    MYSQL_RES *result = mysql_store_result(mysql);
    if (result == NULL)
    {
        finish_with_error(mysql);
    }

    // On sait ici s'il existe déjà ou pas
    if(mysql_fetch_row(result)) {
        // Cas user existe
        mysql_free_result(result);
        return 1;
    } else {
        mysql_free_result(result);
        return 0;
    }

}

int login(char *name, MYSQL *mysql){
    if (lookForUser(name, mysql)){
        printf("\nWelcome back %s!", name);
        return 0;
    } else {
        // Crée un user
        createUser(name, mysql);
        printf("\nWelcome %s!", name);
        return 1;
    }

}

// Function to create and initialize a Subject struct
struct Subject *createSubject(char *name, int numOptions, char **options) {
    // Size of the Subject struct plus space for the options array
    size_t size = sizeof(struct Subject) + numOptions * sizeof(char *);
    // Dynamically allocate memory for the Subject struct
    struct Subject *subject = malloc(size);
    if (subject == NULL) {
        // Error handling
        fprintf(stderr, "Error allocating memory\n");
        return NULL;
    }
    // Initialize the name field
    subject->name = name;
    // Initialize the numOptions field
    subject->numOptions = numOptions;
    // Initialize the options array
    for (int i = 0; i < numOptions; i++) {
        subject->options[i] = options[i];
    }
    return subject;
}

// Function to print a Subject struct and its options
void printSubject(struct Subject *subject) {
    printf("\n\n%s", subject->name);
    for (int i = 0; i < subject->numOptions; i++) {
        printf("\n%s", subject->options[i]);
    }
}

// Function to create, initialize, and store a Subject struct in the subjects array
void addSubject(struct Subject ***subjects, int *numSubjects, char *name, int numOptions, char **options) {
    // Create and initialize the Subject struct
    struct Subject *subject = createSubject(name, numOptions, options);
    // Increase the size of the subjects array
    *subjects = realloc(*subjects, (*numSubjects + 1) * sizeof(struct Subject *));
    // Store the Subject struct in the subjects array
    (*subjects)[*numSubjects] = subject;
    // Increment the numSubjects variable
    (*numSubjects)++;
}

// Function to free the memory for each Subject struct in the subjects array
void freeSubjects(struct Subject **subjects, int numSubjects) {
    for (int i = 0; i < numSubjects; i++) {
        free(subjects[i]);
    }
    free(subjects);
}