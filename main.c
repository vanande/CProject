/*
Objectif:

Demo du projet de C

Date:15/11/2021     Auteur:Vanande


*/
#include "functions.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <winsock.h>
#include <MYSQL/mysql.h>
#include "cJSON.h"

int main(int argc, char ** argv) {
    FILE *fp;
    int connected;
    int i = 0;
    int ans = 0;
    int ansSubj = 0;
    int ansOptions = 0;
    int path;
    MYSQL *mysql;
    mysql = mysql_init(NULL);
    mysql_options(mysql, MYSQL_READ_DEFAULT_GROUP, "option");
    char username[30] = {0};
    struct Subject *subject;
    char *options[2];
    int err;
    int inputCheck;
    struct SubjectData subjectsData = initSubjects();
    struct Subject **subjects = subjectsData.subjects;
    int numSubjects = subjectsData.numSubjects;

/*
    for (int i = 0; i < numSubjects; i++) {
        printSubject(subjects[i]);
    }

    // Check if data is here
    for (int i = 0; i < subjectsData.numSubjects; i++) {
        printf("Subject name: %s\n", subjectsData.subjects[i]->name);
        printf("Number of options: %d\n", subjectsData.subjects[i]->numOptions);
        for (int j = 0; j < subjectsData.subjects[i]->numOptions; j++) {
            printf("Option %d: %s\n", j+1, subjectsData.subjects[i]->options[j]);
        }
    }

    // Check if data is in subjects
    printf("\nsubjects object ->");
    printf("<-\n");
    */

    connected = connectDatabase(mysql);
    if (!connected) {
        i++;
        do {
            i++;
            coolPrint("\n1 ... Try again", atoi(argv[1]));
            coolPrint("\n2 ... Go offline", atoi(argv[1]));
            coolPrint("\n-1 ... Leave", atoi(argv[1]));
            scanf("%d", &ans);
            switch (ans) {
                case 1:
                    printf("\nAttempt number %d\n", i);
                    mysql = mysql_init(NULL);
                    connected = connectDatabase(mysql);
                    if (!connected) {
                        coolPrint("\nTry again later...", atoi(argv[1]));
                    }
                    break;

                case 2:
                    coolPrint("\nData aren't saved in offline mode", atoi(argv[1]));
                    break;
                case -1:
                    printf("\nBye !");
                    return 1;
                default:
                    printf("\nNot an answer so leaving...");
                    return 1;
            }
        } while (ans == 1);
    }

    if (connected) {
        printf("\nHi,how do u want to be called ?");
        inputString(username, 30);
        printf("\nLogging in...");
        login(username, mysql);
    }

    while (ans != -1) {
        SubjectList:
        for (i = 0; i < numSubjects; i++)
            printf("\n%d ... Need %s", i + 1, subjects[i]->name);
        //printf("\n\n%s", subject->name);
        printf("\n-1 ... Quit\n");

        do {
            printf("\nAnswer : ");
            inputCheck = scanf("%d", &ansSubj);
            if (ansSubj == 007)
                goto secretRoom;
            if (ansSubj == -1)
                return 1;
            if (inputCheck != 1) {
                printf("Invalid input, please enter a number between 1 and %d: ", numSubjects);
                scanf("%*[^\n]"); // Used to clear false input. Avoid infinite loop.
            }
        } while (ansSubj < 1 || numSubjects < ansSubj || inputCheck != 1);
        printf("\nCan you be more precise?");
        for (i = 0; i < subjects[ansSubj - 1]->numOptions; i++) //
            printf("\n%d ... %s", i + 1, subjects[ansSubj - 1]->options[i]);
        printf("\n-1 ... Quit\n");

        do {
            printf("\nAnswer : ");
            inputCheck = scanf("%d", &ansOptions);
            if (ansOptions == -1)
                goto SubjectList;
            if (inputCheck != 1) {
                printf("Invalid input, please enter a number between 1 and %d: ",
                       subjects[ansSubj - 1]->numOptions);
                scanf("%*[^\n]"); // Used to clear false input. Avoid infinite loop.
            }
            if (ansOptions != -1) {
                path = ansSubj;
                path *= 10;
                path += ansOptions;
                err = funcUsed(username, path);
                if (err == -1) {
                    return 1;
                }
            }
        } while (ansSubj < 1 || numSubjects < ansSubj || inputCheck != 1);
    }

    end:
    coolPrint("\nBye!", atoi(argv[1]));
    freeSubjects(subjects, numSubjects);
    disconnectDatabase(mysql);
    return 0;


    secretRoom:
    srand(time(NULL));
    int secret_action;
    char stmt_select[100];
    MYSQL * result;
    int num_fields;
    MYSQL_ROW row;
    int reverse_path;
    int subject_options;
    char options_string[100];
    char subject_string[100];
    char greetings_string[100];
    int intel_choice;
    cJSON * json_delivery = loadJSON();
    char * string_delivery;
    char new_subject_name[30];
    int new_num_options;
    char **new_options;
    char subject_to_delete[30];
    cJSON *root;
    cJSON *subjects_array;
    char * new_json_string;
    FILE * config_file;
    char *voice_strings[] = {
            "with a worried voice",
            "with a defeated voice",
            "with a determined voice",
            "with a curious voice",
            "with a disappointed voice",
            "with a reflective voice",
            "with an optimistic voice",
            "with a curious voice",
            "with a serious voice",
            "with a hopeful voice"
    };


    // Enter the room
    if (rand() % 2 == 0) {
        sprintf(greetings_string, "Greetings, Master %s. It is an honor to meet you.", username);
        coolPrint(greetings_string, atoi(argv[1]));
    }else
        printf("\nGood morning, Master %s. It is a pleasure to make your acquaintance.", username);

    // Doing stuff
    while(secret_action != -1) {
        printf("\n1... Show my slaves");
        printf("\n2... What are their trouble ?");
        printf("\n3... Let's talk about intel");
        printf("\n-1... Leave");
        do {
            printf("\nAnswer : ");
            inputCheck = scanf("%d", &secret_action);
            if (secret_action == -1)
                goto LeaveTheRoom;;
            if (inputCheck != 1) {
                printf("\nPlease master enter a valid number.");
                scanf("%*[^\n]"); // Used to clear false input. Avoid infinite loop.
            }
        } while (secret_action < 1 || secret_action > 3 || inputCheck != 1);


        switch (secret_action) {
            case 1:
                printf("\nHere is the database master!\n");
                strcpy(stmt_select,"SELECT * FROM user");

                if (mysql_query(mysql, stmt_select)) {
                    finish_with_error(mysql);
                }
                result = mysql_store_result(mysql);
                if (result == NULL) {
                    finish_with_error(mysql);
                }
                num_fields = mysql_num_fields(result);
                while ((row = mysql_fetch_row(result))) {
                    for (int i = 0; i < num_fields; i++) {
                        printf("%s\t", row[i]);
                    }
                    printf("\n");
                }
                mysql_free_result(result);

                break;
            case 2:
                coolPrint("\nLooking for their biggest trouble", atoi(argv[1]));
                strcpy(stmt_select,"SELECT path_code FROM path ORDER BY number_of_times DESC LIMIT 1;");

                if (mysql_query(mysql, stmt_select)) {
                    finish_with_error(mysql);
                }
                result = mysql_store_result(mysql);
                if (result == NULL) {
                    finish_with_error(mysql);
                }
                if (mysql_num_rows(result) > 0) {
                    row = mysql_fetch_row(result);
                    reverse_path = atoi(row[0]);
                    if ( reverse_path%10 == 1){
                        coolPrint("\nIt seems that they're looking at the sky to get their answers instead of asking you.", atoi(argv[1]));
                    } else {
                        coolPrint("\nIt seems that they're getting their answers from Twitter, instead of asking you.",atoi(argv[1]));
                    }
                    reverse_path /= 10;
                    subject_options = reverse_path % 10;
                    reverse_path /= 10;

                    sprintf(options_string,"\nThey are asking theirself : \"%s\" %s", subjects[reverse_path%10]->options[subject_options], voice_strings[rand()%10]);
                    coolPrint(options_string, atoi(argv[1]));

                    sprintf(subject_string,"\nThey probably need %s", subjects[reverse_path%10]->name);
                    coolPrint(subject_string, atoi(argv[1]));
                }
                mysql_free_result(result);
                break;
            case 3:
                printf("\nThat's some serious business...");
                printf("\n1 ... Show me intel");
                printf("\n2 ... Add some");
                printf("\n3 ... Delete that part");
                printf("\n-1 ... Enough, I've got headache");
                do {
                    printf("\nAnswer : ");
                    inputCheck = scanf("%d", &intel_choice);
                    if (intel_choice == -1)
                        goto LeaveTheRoom;;
                    if (inputCheck != 1) {
                        printf("\nPlease master enter a valid number.");
                        scanf("%*[^\n]"); // Used to clear false input. Avoid infinite loop.
                    }
                } while (intel_choice < 1 || intel_choice > 4 || inputCheck != 1);
                switch (intel_choice) {
                    case 1:
                        string_delivery = cJSON_Print(json_delivery);
                        printf("\nHere is what you asked for : \n%s", string_delivery);
                        break;
                    case 2:
                        printf("\nType the name of the new subject\n");
                        scanf("%s", new_subject_name);
                        printf("\nType the number of options\n");
                        inputCheck = scanf("%d", &new_num_options);
                        if (inputCheck != 1){
                            printf("\nDon't you know what a number is ?");
                            goto LeaveTheRoom;
                        }
                        if (new_num_options < 0 || new_num_options > 5){
                            printf("\nDon't want to work anymore");
                            goto end;
                        }
                        new_options = malloc(new_num_options * sizeof(char *));
                        for (i = 0; i < new_num_options; i++){
                            new_options[i] = malloc(150 * sizeof(char));
                            if (new_options[i] == NULL){
                                printf("\nError in the malloc");
                                return 1;
                            }

                            printf("\nType options %d\n", i);
                            scanf("%s", new_options[i]);
                        }
                        string_delivery = cJSON_Print(json_delivery);
                        root = cJSON_Parse(string_delivery);
                        subjects_array = cJSON_GetObjectItemCaseSensitive(root, "subjects");

                        cJSON *new_subject = cJSON_CreateObject();
                        cJSON_AddStringToObject(new_subject, "name", new_subject_name);
                        cJSON_AddNumberToObject(new_subject, "numOptions", new_num_options);
                        cJSON *options_array = cJSON_CreateArray();
                        for (int i = 0; i < new_num_options; i++) {
                            cJSON_AddItemToArray(options_array, cJSON_CreateString(new_options[i]));
                        }
                        cJSON_AddItemToObject(new_subject, "options", options_array);
                        cJSON_AddItemToArray(subjects_array, new_subject);

                        //new_json_string = malloc(3000 * sizeof(char ));
                        new_json_string = cJSON_Print(root);
                        printf("Here is the new JSON %s", new_json_string);

                        config_file = fopen("../config.json", "w");
                        fprintf(config_file, "%s", new_json_string);
                        fclose(config_file);
                        break;
                    case 3:
                        printf("\nType the name of the subject to delete\n");
                        scanf("%s", subject_to_delete);

                        string_delivery = cJSON_Print(json_delivery);
                        root = cJSON_Parse(string_delivery);
                        subjects_array = cJSON_GetObjectItemCaseSensitive(root, "subjects");

                        for (i = 0; i < cJSON_GetArraySize(subjects_array); i++) {
                            cJSON *subject_to_delete_JSON = cJSON_GetArrayItem(subjects_array, i);
                            if (strcmp(cJSON_GetObjectItemCaseSensitive(subject_to_delete_JSON, "name")->valuestring, subject_to_delete) == 0) {
                                cJSON_DeleteItemFromArray(subjects_array, i);
                                break;
                            }
                        }

                        //new_json_string = malloc(3000 * sizeof(char ));
                        new_json_string = cJSON_Print(root);
                        printf("Here is the new JSON with deleted subject %s", new_json_string);

                        config_file = fopen("../config.json", "w");
                        fprintf(config_file, "%s", new_json_string);
                        fclose(config_file);


                        break;
                }
                break;

            case -1:
                goto LeaveTheRoom;
            default:
                printf("\nPlease master, don't test me");
                break;
        }
    }
    // Leave the room
    LeaveTheRoom:
    switch (rand() % 3) {
        case 0:
            printf("\nFarewell, Master. It was an honor to have met you.");
            break;
        case 1:
            printf("\nGoodbye, Master. Thank you for your time and guidance.");
            break;
        case 2:
            printf("\nThank you for your time, Master. It was an honor to have met you.");
            break;
    }


    goto end;
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
