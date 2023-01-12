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
int i = 0;
int path;
int ans = 0;
int ansSubj = 0;
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
//for (int i = 0; i < numSubjects; i++)
  //  printSubject(subjects[i]);


MYSQL *mysql;
mysql = mysql_init(NULL);
mysql_options(mysql, MYSQL_READ_DEFAULT_GROUP, "option");

connected = connectDatabase(mysql);
if (!connected) {
    i++;
    do{
        i++;
        printf("\n1 ... Try again");
        printf("\n2 ... Go offline");
        printf("\n-1 ... Leave");
        scanf("%d",&ans);
        switch (ans) {
            case 1:
                printf("\nAttempt number %d\n", i);
                mysql = mysql_init(NULL);
                connected = connectDatabase(mysql);
                if (!connected){
                    printf("\nTry again later...");
                }
                break;

            case 2:
                printf("\nData aren't saved in offline mode");
                break;
            case -1:
                printf("Bye !");
                return 1;
        }
    }while (ans == 1);
}

if(connected){
    printf("\nHi,how do u want to be called ?");
    inputString(username, 30);
    printf("\nLogging in...");
    login(username, mysql);
}

while(ans != -1) {
    for (i = 0; i < numSubjects; i++)
    printf("\n%d ... Need %s", i+1, subjects[i]->name);
    //printf("\n\n%s", subject->name);
        printf("\n-1 ... Quit\n");
        scanf("%d", &ansSubj);

    switch (ansSubj) {
        case 1: // Need relationship advice
            printf("\nCan you be more precise");
            for (i = 0; i < subjects[ansSubj]->numOptions; i++) //
                printf("\n%d ... %s", i, subjects[ansSubj-1]->options[i]);

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
        case 2: // Need financial advice
            printf("\nCan you be more precise");
            for (i = 0; i < subjects[ansSubj]->numOptions; i++) //
                printf("\n%d ... %s", i, subjects[ansSubj-1]->options[i]);
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

freeSubjects(subjects, numSubjects);
return 0;
}
