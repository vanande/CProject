/*
Objectif:

Demo du projet de C

Date:15/11/2021     Auteur:Vanande


*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <winsock.h>
#include <MYSQL/mysql.h>

void getLastTweet(char name[]);
void getAstrological(char astrological[30]);
int connectDatabase(MYSQL *mysql);
void disconnectDatabase(MYSQL * mysql);
int funcUsed();
int login(char name[30], MYSQL *mysql);


int connectDatabase(MYSQL *mysql){
    if (mysql_real_connect(mysql, "localhost", "root", "", NULL, 3307, NULL, 0)) {
        printf("MySQL client version: %s\n", mysql_get_client_info());
        mysql_query(mysql, "CREATE DATABASE IF NOT EXISTS cProject");
        mysql_query(mysql, "USE cProject");
        return 0;
    } else {
        printf("Erreur connexion a la BDD!");
        return 1;
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

int login(char name[30], MYSQL *mysql){
    MYSQL_STMT *stmt;
    unsigned long nameLen = strlen(name);
    MYSQL_BIND bind[1];

    const char *stmt_str = "INSERT INTO user(name) VALUES (?)";
    unsigned long a;

    mysql_query(mysql, "CREATE TABLE IF NOT EXISTS user(id INT PRIMARY KEY AUTO_INCREMENT, name VARCHAR(30), last_path INT)");
    stmt = mysql_stmt_init(mysql);

    // Prepare request
    if (mysql_stmt_prepare(stmt, stmt_str, strlen(stmt_str)))
    {
        fprintf(stderr, " mysql_stmt_prepare(), INSERT failed\n");
        fprintf(stderr, " %s\n", mysql_stmt_error(stmt));
        exit(0);
    }
    fprintf(stdout, " prepare, INSERT successful\n");

    // count param
    /* Get the parameter count from the statement */
    a = mysql_stmt_param_count(stmt);
    fprintf(stdout, " total parameters in INSERT: %d\n", a);

    /* Bind the data of params */
    bind[0].buffer_type = MYSQL_TYPE_STRING;
    bind[0].buffer = (char *)name;
    bind[0].buffer_length = strlen(name);
    bind[0].is_null = 0;
    bind[0].length = &nameLen;

    /* Bind the buffers */
    if (mysql_stmt_bind_param(stmt, bind))
    {
        fprintf(stderr, " mysql_stmt_bind_param() failed\n");
        fprintf(stderr, " %s\n", mysql_stmt_error(stmt));
        exit(0);
    }


    /* Specify the data values for the first row */
    strncpy(stmt_str, "MySQL", 50); /* string */


    /* Execute the INSERT statement - 1*/
    if (mysql_stmt_execute(stmt))
    {
        fprintf(stderr, " mysql_stmt_execute(), 1 failed\n");
        fprintf(stderr, " %s\n", mysql_stmt_error(stmt));
        exit(0);
    }

    /* Get the number of affected rows */
    a= mysql_stmt_affected_rows(stmt);
    fprintf(stdout, " total affected rows(insert 1): %lu\n",
            (unsigned long) a);
    /* Close the statement */
    if (mysql_stmt_close(stmt))
    {
        /* mysql_stmt_close() invalidates stmt, so call          */
        /* mysql_error(mysql) rather than mysql_stmt_error(stmt) */
        fprintf(stderr, " failed while closing the statement\n");
        fprintf(stderr, " %s\n", mysql_error(mysql));
        exit(0);
    }

}

int main(int argc, char ** argv){
    int connected = 0;
    int path;
    int ans = 0;
    int algo;
    char username[30];

    MYSQL *mysql;
    mysql = mysql_init(NULL);
    mysql_options(mysql, MYSQL_READ_DEFAULT_GROUP, "option");

    connectDatabase(mysql);


    printf("\nHi, register first ?");
    printf("\nHow do u want to be called ?");
    fgets(username, 30, stdin);
    login(username, mysql);

    while(ans != -1) {
        printf("\n1 ... Need relationship advice");
        printf("\n2 ... Need financial advice");
        printf("\n3 ... Need family advice");
        printf("\n-1 ... Quit\n");
        scanf("%d", &ans);

        switch (ans) {
            case 1: // Need relationship advice
                printf("\nCan you be more precise");
                printf("\n1 ... Should I leave my partner ?");
                printf("\n2 ... Should I have a new partner ?");
                printf("\n3 ... Should I stay single for the rest of my life ?");
                scanf("%d", &ans);
                switch(ans){
                    case 1:
                        printf("\nWhere to look for your answer?");
                        printf("\n1 ... My astrological");
                        printf("\n2 ... Elon's tweet");
                        scanf("%d", &algo);
                        if (algo == 1)
                            getAstrological("scorpion");
                        if (algo == 2)
                            getLastTweet("elon");
                        break;
                    case 2:
                        funcUsed(&algo);
                        break;
                    case 3:
                        funcUsed(&algo);
                        break;
                    default:
                        printf("\nYou answered %d, try again", ans);
                }

                break;
            case 2: // Need financial advice
                printf("\nCan you be more precise");
                printf("\n1 ... Should I invest in Bitcoin ?");
                printf("\n2 ... Should I become a freelancer ?");
                printf("\n3 ... Other");
                scanf("%d", &ans);
                switch(ans){
                    case 1:
                        funcUsed(&algo);
                        break;
                    case 2:
                        funcUsed(&algo);
                        break;
                    case 3:
                        funcUsed(&algo);
                        break;
                    default:
                        printf("\nYou answered %d, try again", ans);
                }
                break;
            case 3: // Need family advice
                printf("\nCan you be more precise");
                printf("\n1 ... Should I offer a gift to my mom for Christmas ?");
                printf("\n2 ... Should I have kids?");
                printf("\n3 ... Other");
                scanf("%d", &ans);
                switch(ans){
                    case 1:
                        funcUsed(&algo);
                        break;
                    case 2:
                        funcUsed(&algo);
                        break;
                    case 3:
                        funcUsed(&algo);
                        break;
                    default:
                        printf("\nYou answered %d, try again", ans);
                }
                break;
            case -1:
                disconnectDatabase(mysql);
                    printf("\nGoodbye!\n");
                break;

            default:
                printf("\nYou answered %d, try again", ans);

        }
        if (ans != -1){
            sleep(4);
            printf("\n\nNeed anything else ?\n\n");
            sleep(2);
        }
    }

    return 0;
}
