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
#include <curl/curl.h>

void getLastTweet(char name[]);
void getAstrological(char astrological[30]);
int connectDatabase(MYSQL *mysql);
void disconnectDatabase(MYSQL * mysql);
int funcUsed();
int login(char name[30], MYSQL *mysql);
void inputString(char* string, int size);
void finish_with_error(MYSQL *con);
int createUser(char * name, MYSQL *mysql);



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

    printf("\nHi,how do u want to be called ?");
    inputString(username, 30);
    printf("\nLogging in...");
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
