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



cJSON *loadJSON() {
    FILE *fp;
    // Read the config.json file into a buffer
    fp = fopen("../config.json", "rt");
    if (fp == NULL) {
        printf("\nError opening config.json");
        return 1;
    }

    fseek(fp, 0, SEEK_END);
    long size = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    char *buffer = malloc(size + 1);
    fread(buffer, 1, size, fp);
    fclose(fp);

    // Parse the JSON data using cJSON
    cJSON *root = cJSON_Parse(buffer);
    if (root == NULL) {
        const char *error_ptr = cJSON_GetErrorPtr();
        printf("Error parsing JSON: %s\n", error_ptr);
        return 1;
    }
    return root;
}
struct SubjectData initSubjects() {
    cJSON *json = loadJSON();
    struct SubjectData data;
    data.subjects = NULL;
    data.numSubjects = 0;

    cJSON *subjectsJSON = cJSON_GetObjectItemCaseSensitive(json, "subjects");
    int numSubjectsJSON = cJSON_GetArraySize(subjectsJSON);

    printf("\nThere is %d subjects", numSubjectsJSON);

    // Iterate through the subjects array and extract the data
    for (int i = 0; i < numSubjectsJSON; i++) {
        cJSON *subjectJSON = cJSON_GetArrayItem(subjectsJSON, i);
        cJSON *nameJSON = cJSON_GetObjectItemCaseSensitive(subjectJSON, "name");
        cJSON *numOptionsJSON = cJSON_GetObjectItemCaseSensitive(subjectJSON, "numOptions");
        cJSON *optionsJSON = cJSON_GetObjectItemCaseSensitive(subjectJSON, "options");
        // Allocate memory for the Subject struct and its options array
        struct Subject *subject = malloc(sizeof(struct Subject) + sizeof(char *) * numOptionsJSON->valueint);
        subject->name = strdup(nameJSON->valuestring);
        subject->numOptions = numOptionsJSON->valueint;

        printf("\nSubject NAME : %s", subject->name);
        printf("\nNumber of options : %d", subject->numOptions);

        // Iterate through the options array and extract the data
        for (int j = 0; j < subject->numOptions; j++) {
            cJSON *optionJSON = cJSON_GetArrayItem(optionsJSON, j);
            subject->options[j] = strdup(optionJSON->valuestring);
            printf("\nSubject options[%d], %s", j, subject->options[j]);
        }
        // Add the Subject struct to the subjects array
        addSubject(&data, subject->name, subject->numOptions, subject->options);
    }

    return data;
}
    int main(int argc, char ** argv){
    FILE * fp;
    int connected;
    int i = 0;
    //int path;
    int ans = 0;
    int ansSubj = 0;
    int ansOptions = 0;

    //int algo;
    char username[30];
    struct Subject *subject;
    char *options[2];
    int err;
    int inputCheck;
    struct SubjectData subjectsData = initSubjects();
    struct Subject** subjects = subjectsData.subjects;
    int numSubjects = subjectsData.numSubjects;

// Now you can access the subjects array and numSubjects variable using subjectsData.subjects and subjectsData.numSubjects respectively

        for (int i = 0; i < subjectsData.numSubjects; i++) {
            printf("Subject name: %s\n", subjectsData.subjects[i]->name);
            printf("Number of options: %d\n", subjectsData.subjects[i]->numOptions);
            for (int j = 0; j < subjectsData.subjects[i]->numOptions; j++) {
                printf("Option %d: %s\n", j+1, subjectsData.subjects[i]->options[j]);
            }
        }

    printf("\nsubjects object ->");
for (int i = 0; i < numSubjects; i++)
      printSubject(subjects[i]);
    printf("<-\n");

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
    coolPrint("\nHi,how do u want to be called ?", atoi(argv[1]));
    inputString(username, 30);
    printf("\nLogging in...");
    login(username, mysql);
}

while(ans != -1) {
    SubjectList:
    for (i = 0; i < numSubjects; i++)
    printf("\n%d ... Need %s", i+1, subjects[i]->name);
    //printf("\n\n%s", subject->name);
        printf("\n-1 ... Quit\n");

    do{
        printf("\nAnswer : ");
        inputCheck = scanf("%d", &ansSubj);
        if (ansSubj == -1)
            return 1;
        if(inputCheck != 1){
            printf("Invalid input, please enter a number between 1 and %d: ", numSubjects);
            scanf("%*[^\n]"); // Used to clear false input. Avoid infinite loop.
        }
    }while (ansSubj < 1 || numSubjects < ansSubj || inputCheck != 1);
            printf("\nCan you be more precise?");
            for (i = 0; i < subjects[ansSubj-1]->numOptions; i++) //
                printf("\n%d ... %s", i+1, subjects[ansSubj-1]->options[i]);
            printf("\n-1 ... Quit\n");

            do{
                printf("\nAnswer : ");
                inputCheck = scanf("%d", &ansOptions);
                if (ansOptions == -1)
                    goto SubjectList;
                if(inputCheck != 1){
                    printf("Invalid input, please enter a number between 1 and %d: ", subjects[ansSubj-1]->numOptions);
                    scanf("%*[^\n]"); // Used to clear false input. Avoid infinite loop.
                }
                if (ansOptions != -1){
                    err = funcUsed();
                    if (err == -1){
                        return 1;
                    }
                }
            }while (ansSubj < 1 || numSubjects < ansSubj || inputCheck != 1);

}
    freeSubjects(subjects, numSubjects);
    return 0;
}

        /*

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
*/
