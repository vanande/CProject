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
//#include "cJSON.h"
#include "functions.h"


int main(int argc, char ** argv){
    FILE * fp;
    int connected;
    int i = 0;
    //int path;
    int ans = 0;
    int ansSubj = 0;
    //int algo;
    char username[30];
    struct Subject **subjects = malloc(sizeof(struct Subject*));
    struct Subject *subject;
    int numSubjects = 0;
    char *options[2];
    int err;

    // Read the config.json file into a buffer
    fp = fopen("config.json", "rt");
    fseek(fp, 0, SEEK_END);
    long size = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    char *buffer = malloc(size + 1);
    fread(buffer, 1, size, fp);
    fclose(fp);

    // Parse the JSON data using cJSON
    cJSON *root = cJSON_Parse(buffer);
    cJSON *subjectsJSON = cJSON_GetObjectItemCaseSensitive(root, "subjects");

    char *string = cJSON_Print(subjectsJSON);
    printf("\nJSON I got %s\n", string);


/*
// Create and initialize the first Subject struct
options[0] = "Should I leave my wife?";
options[1] = "Should I stay single for the rest of my life?";
addSubject(&subjects, &numSubjects, "relationship advice", 2, options);

// Create and initialize the second Subject struct
options[0] = "Should I invest in stocks?";
options[1] = "Should I save my money in a bank?";
addSubject(&subjects, &numSubjects, "financial advice", 2, options);

 */
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
        coolPrint("\n1 ... Try again", argv);
        coolPrint("\n2 ... Go offline", argv);
        coolPrint("\n-1 ... Leave", argv);
        scanf("%d",&ans);
        switch (ans) {
            case 1:
                printf("\nAttempt number %d\n", i);
                mysql = mysql_init(NULL);
                connected = connectDatabase(mysql);
                if (!connected){
                    coolPrint("\nTry again later...", argv);
                }
                break;

            case 2:
                coolPrint("\nData aren't saved in offline mode", argv);
                break;
            case -1:
                printf("\nBye !");
                return 1;
            default:
                printf("\nNot an answer so leaving...");
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
                printf("\n%d ... %s", i+1, subjects[ansSubj-1]->options[i]);

            scanf("%d", &ans);
            switch(ans){
                case 1:
                    err = funcUsed();
                    if (err == -1)
                        return 1;
                    break;
                case 2:
                    err = funcUsed();
                    if (err == -1)
                        return 1;
                    break;
                case 0:
                    sleep(2);
                    printf("\nYou are suspect so leaving");
                    return 1;
                default:
                    printf("\nYou answered %d, try again", ans);
                    ans = 0;
                    break;
            }

            break;
        case 2: // Need financial advice
            printf("\nCan you be more precise");
            for (i = 0; i < subjects[ansSubj-1]->numOptions; i++)
                printf("\n%d ... %s", i+1, subjects[ansSubj-1]->options[i]);
            scanf("%d", &ans);
            switch(ans){
                case 1:
                    err = funcUsed();
                    if (err == -1)
                        return 1;
                    break;
                case 2:
                    err = funcUsed();
                    if (err == -1)
                        return 1;
                    break;
                case 0:
                    sleep(2);
                    printf("\nYou are suspect so leaving");
                    return 1;

                default:
                    printf("\nYou answered %d", ans);
                    ans = 0;
                    break;
            }
            break;
        case -1:
            disconnectDatabase(mysql);
            printf("\nGoodbye!\n");
            break;

        case 0:
            printf("\nYou are suspect so leaving...");
            return 1;

        default:
            printf("\nYou answered %d, try again", ans);
            ans = 0;
            break;

    }
    if (ans != -1){
        sleep(4);
        coolPrint("\n\nNeed anything else ?\n\n", argv);
        sleep(2);
    }
}

freeSubjects(subjects, numSubjects);
return 0;
}
