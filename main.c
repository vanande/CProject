/*
Objectif:

Demo du projet de C

Date:15/11/2021     Auteur:Vanande


*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <winsock.h>
#include <MYSQL/mysql.h>

void getLastTweet(char name[]);
void getAstrological(char astrological[30]);
int connectDatabase(MYSQL *mysql);
void disconnectDatabase(MYSQL * mysql);
int funcUsed();

int connectDatabase(MYSQL *mysql){
    if (mysql_real_connect(mysql, "localhost", "root", "", NULL, 3307, NULL, 0)) {
        printf("MySQL client version: %s\n", mysql_get_client_info());
        mysql_query(mysql, "CREATE DATABASE IF NOT EXISTS cProject");
        mysql_query(mysql, "USE cProject");
        mysql_query(mysql, "DROP TABLE IF EXISTS user");
        mysql_query(mysql, "CREATE TABLE IF NOT EXISTS user(id INT PRIMARY KEY AUTO_INCREMENT, name VARCHAR(30), last_path INT)");
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

int main(int argc, char ** argv){


    int ans = 0;
    int algo;

    MYSQL *mysql;
    mysql = mysql_init(NULL);
    mysql_options(mysql, MYSQL_READ_DEFAULT_GROUP, "option");

    connectDatabase(mysql);



    printf("\nHi, what do you want ?");

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
                printf("\n2 ... Should I start a new life ?");
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
