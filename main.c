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
#include "functions.h"


int main(int argc, char ** argv){
    int connected = 0;
    int path;
    int ans = 0;
    int algo;
    char username[30];
    struct Subject **subjects = malloc(sizeof(struct Subject*));
    struct Subject *subject;
    int numSubjects = 0;
    char *options[2];

    // Create and initialize the first Subject struct
    options[0] = "Should I leave my wife?";
    options[1] = "Should I stay single for the rest of my life?";
    addSubject(&subjects, &numSubjects, "Relationship advice", 2, options);

// Create and initialize the second Subject struct
    options[0] = "Should I invest in stocks?";
    options[1] = "Should I save my money in a bank?";
    addSubject(&subjects, &numSubjects, "Financial advice", 2, options);

    // Print the Subject structs
    for (int i = 0; i < numSubjects; i++)
        printSubject(subjects[i]);

    freeSubjects(subjects, numSubjects);
    return 0;


    MYSQL *mysql;
    mysql = mysql_init(NULL);
    mysql_options(mysql, MYSQL_READ_DEFAULT_GROUP, "option");

    if (connectDatabase(mysql))
        return 1;

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
