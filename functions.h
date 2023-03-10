#define BEARER_TOKEN "AAAAAAAAAAAAAAAAAAAAAGPvlAEAAAAA1o0fe4FSGXB89i5N%2FT8ZWKKBI%2BU%3DonZOaCBeDMgXahH8aiX2LyGwN4lXmn2SdNZ6eWrmFrBrd2WM4e"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <winsock.h>
#include <MYSQL/mysql.h>
#include <curl/curl.h>
#include "cJSON.h"

struct Subject {
    char *name;
    int numOptions;
    char *options[];
};
struct SubjectData {
    struct Subject** subjects;
    int numSubjects;
};


int connectDatabase(MYSQL *mysql);
void disconnectDatabase(MYSQL * mysql);
void getLastTweet(char name[]);
void getAstrological(char astrological[30]);
int funcUsed(char * username, int path);
void inputString(char* string, int size);
void finish_with_error(MYSQL *con);
int createUser(char * name, MYSQL *mysql);
int lookForUser(char *name, MYSQL *mysql);
int login(char name[30], MYSQL *mysql);
int insertData(MYSQL *mysql);
struct Subject *createSubject(char *name, int numOptions, char **options);
void printSubject(struct Subject *subject);
void addSubject(struct SubjectData *data, char *name, int numOptions, char **options);
void freeSubjects(struct Subject **subjects, int numSubjects);
cJSON *loadJSON();
struct SubjectData initSubjects();
void coolPrint(char str[], int argv);

unsigned int write_callback(char *ptr, unsigned int size, unsigned int number_members, void *userdata);
char* getLastTweetID(char *);
cJSON * sendRequest(char * url);
char* getTweetText(char *);
char* getIdFromName(char *name);

char* getHoroscope(char *astrological);
cJSON* sendPostRequest(char *url, char * postfields);

cJSON *loadJSON();
struct SubjectData initSubjects();




int connectDatabase(MYSQL *mysql){
    if (mysql_real_connect(mysql, "localhost", "root", "", NULL, 3307, NULL, 0)) {
        //printf("MySQL client version: %s\n", mysql_get_client_info());
        if (mysql_query(mysql, "CREATE DATABASE IF NOT EXISTS cProject")){
            finish_with_error(mysql);
        }
        if (mysql_query(mysql, "USE cProject")){
            finish_with_error(mysql);
        }
        if (mysql_query(mysql, "CREATE TABLE IF NOT EXISTS path (path_code VARCHAR(10) PRIMARY KEY, number_of_times INT)")){
            finish_with_error(mysql);
        }
        // Inserting some data
        insertData(mysql);
        return 1;
    } else {
        printf("Error connecting to database!");
        return 0;
    }
}
int insertData(MYSQL *mysql){
    srand(time(NULL));
    char stmt_insert[150];
    int path_code;
    int subject_index;
    struct SubjectData subjectsData = initSubjects();
    struct Subject **subjects = subjectsData.subjects;
    int numSubjects = subjectsData.numSubjects;

    for (int i = 0; i < 100; ++i) {
        path_code = 0;
        path_code = subject_index = rand()%numSubjects;
        path_code *= 10;
        path_code += rand()%subjects[subject_index]->numOptions;
        path_code *= 10;
        path_code += rand()%2;
        sprintf(stmt_insert, "INSERT INTO path (path_code, number_of_times) VALUES ('%d', 1) ON DUPLICATE KEY UPDATE number_of_times = number_of_times + 1", path_code);
        if (mysql_query(mysql, stmt_insert)) {
            finish_with_error(mysql);
        }
    }
}

void disconnectDatabase(MYSQL * mysql){
    mysql_close(mysql);
}

void getLastTweet(char tweet_user[30]){
    int a;
    char tweet_id[30];
    char tweet_text[255];
    srand(time(NULL));
    a = rand()%2;

    // Getting last tweet ID

    if (!getLastTweetID(tweet_user))
        return;

    strcpy(tweet_id,getLastTweetID(tweet_user));
    //printf("Here is user last tweet ID : %s", tweet_id);
    printf("\nGot the tweet!");
    printf("\nReading it...");
    sleep(2);

    // Print the last tweet
    strcpy(tweet_text, getTweetText(tweet_id));
    printf("\nHere is %s last tweet :\n     %s\n",tweet_user, tweet_text);
    printf("\nGetting the answer to your universal question...");
    sleep(2);
    if (!a)
        printf("\nYou definitely should do it according to %s",tweet_user);
    else
        printf("\nYou definitely shouldn't do it according to %s",tweet_user);
}

void getAstrological(char astrological[30]){
    int a;
    char horoscope[1000];
    srand(time(NULL));
    a = rand()%2;
    // Getting horoscope
    //printf("Getting horoscope\n");
    //printf("\n%s", getHoroscope(astrological));
    strcpy(horoscope,getHoroscope(astrological));
    printf("\nGetting the great %s opinion", astrological);
    sleep(2);
    printf("\nHere is what %s says ", astrological);
    printf("\n%s", horoscope);
    printf("\nGetting the answer to your universal question...");
    sleep(2);
    if (!a)
        printf("\nYou definitely should do it according to %s",astrological);
    else
        printf("\nYou definitely shouldn't do it according to %s",astrological);

    printf("\n\n\n\n\n");
}

int funcUsed(char * username, int path){
    int algo;
    int i;
    char astro[30];
    char tweet_user[30] = {0};
    char tweet_example[6][100];
    char signs[12][20];
    int chosenSigns=0;
    int inputCheck;
    MYSQL *mysql;
    mysql = mysql_init(NULL);
    mysql_options(mysql, MYSQL_READ_DEFAULT_GROUP, "option");
    int connected = connectDatabase(mysql);
    char stmt[150];
    char stmt_update[150];



    printf("\nWhere to look for your answer?");
    printf("\n1 ... My astrological");
    printf("\n2 ... From Twitter");
    scanf("%d", &algo);
    switch(algo){
        case 1:
            path *= 10;
            path += 1;
            sprintf(stmt, "INSERT INTO path (path_code, number_of_times) VALUES ('%d', 1) ON DUPLICATE KEY UPDATE number_of_times = number_of_times + 1", path);
            if (mysql_query(mysql, stmt)){
                finish_with_error(mysql);
            }

            sprintf(stmt_update, "UPDATE user SET last_path = '%d' WHERE name = '%s'", path, username);
            if (mysql_query(mysql, stmt_update)){
                finish_with_error(mysql);
            }

            //printf("\nHere is a list of all signs - aries, taurus, gemini, cancer, leo, virgo, libra, scorpio, sagittarius, capricorn, aquarius and pisces.");
            printf("\nWhat is your sign ?\n");
            strcpy(signs[0],"Aries");
            strcpy(signs[1],"Taurus");
            strcpy(signs[2],"Gemini");
            strcpy(signs[3],"Cancer");
            strcpy(signs[4],"Leo");
            strcpy(signs[5],"Virgo");
            strcpy(signs[6],"Libra");
            strcpy(signs[7],"Scorpio");
            strcpy(signs[8],"Sagittarius");
            strcpy(signs[9],"Capricorn");
            strcpy(signs[10],"Aquarius");
            strcpy(signs[11],"Pisces");

                for (i = 0; i < 12; i++) {
                    printf("%d ... ", i + 1);
                    printf(signs[i]);
                    printf("\n");
                }
            do{
                chosenSigns = 0;
                printf("\nAnswer : ");
                inputCheck = scanf("%d", &chosenSigns);
                if(inputCheck != 1)
                {
                    printf("Invalid input, please enter a number between 1 and 12: ");
                    scanf("%*[^\n]"); // Used to clear false input. Avoid infinite loop.
                }
            }while (chosenSigns < 1 || 12 < chosenSigns || inputCheck != 1);
            strcpy(astro,signs[chosenSigns-1]);
                getAstrological(astro);
            sleep(10);
            break;
        case 2:
            path *= 10;
            path += 2;
            sprintf(stmt, "INSERT INTO path (path_code, number_of_times) VALUES ('%d', 1) ON DUPLICATE KEY UPDATE number_of_times = number_of_times + 1", path);
            if (mysql_query(mysql, stmt)){
                finish_with_error(mysql);
            }
            sprintf(stmt_update, "UPDATE user SET last_path = '%d' WHERE name = '%s'", path, username);
            if (mysql_query(mysql, stmt_update)){
                finish_with_error(mysql);
            }

            // Init the tweet_example with raw text
                strcpy(tweet_example[0], "Elon Musk : @elonmusk");
                strcpy(tweet_example[1], "Bill Gates : @BillGates");
                strcpy(tweet_example[2], "Emmanuel Macron : @EmmanuelMacron");
                strcpy(tweet_example[3], "LeBron James : @KingJames");
                strcpy(tweet_example[4], "Barack Obama : @BarackObama");
                strcpy(tweet_example[5], "IDF Mobilites : @IDFmobilites");


                printf("Good choice! What's the account of your prophet? (start with @)");
                printf("\nSome ideas...\n");
                for (i = 0; i < 6; i++){
                    printf("\n");
                    printf(tweet_example[i]);
                }
                printf("\nAnswer : ");
                //printf("%s", tweet_user[i][0]);

                scanf("%s", &tweet_user);
                if (tweet_user[0] == '@'){
                    for (i = 0; i < strlen(tweet_user); i++){
                        tweet_user[i] = tweet_user[i+1];
                    }
                }
                getLastTweet(tweet_user);
            sleep(10);

            break;
        default:
            printf("\nDon't need advice ? Ok");
            return -1;
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

void coolPrint(char str[], OPTIONAL int argv){
    int sleepTime;
    for (int i = 0; i < strlen(str); i++){
        if(str[i] == ' '){
            printf("%c",str[i]);
            if(argv != NULL){
                sleepTime = argv;
                Sleep(sleepTime);
            } else {
                Sleep(200);
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

    if (mysql_query(mysql, stmt_select)){
        finish_with_error(mysql);
    }

    MYSQL_RES *result = mysql_store_result(mysql);
    if (result == NULL){
        finish_with_error(mysql);
    }

    // On sait ici s'il existe d??j?? ou pas
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
        // Cr??e un user
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

void addSubject(struct SubjectData *data, char *name, int numOptions, char **options) {
    // Create and initialize the Subject struct
    struct Subject *subject = createSubject(name, numOptions, options);
    // Increase the size of the subjects array
    data->subjects = realloc(data->subjects, (data->numSubjects + 1) * sizeof(struct Subject));
    // Store the Subject struct in the subjects array
    data->subjects[data->numSubjects] = subject;
    // Increment the numSubjects variable
    // Increment the numSubjects variable
    data->numSubjects++;
}
/*
void addSubject(struct Subject ***subjects, int *numSubjects, char *name, int numOptions, char **options) {
    // Create and initialize the Subject struct
    struct Subject *subject = createSubject(name, numOptions, options);
    // Increase the size of the subjects array
    *subjects = realloc(*subjects, (*numSubjects + 1) * sizeof(struct Subject));
    // Store the Subject struct in the subjects array
    (*subjects)[*numSubjects] = subject;
    // Increment the numSubjects variable
    (*numSubjects)++;
}
*/

// Function to free the memory for each Subject struct in the subjects array
void freeSubjects(struct Subject **subjects, int numSubjects) {
    for (int i = 0; i < numSubjects; i++) {
        free(subjects[i]);
    }
    free(subjects);
}


unsigned int write_callback(char *ptr, unsigned int size, unsigned int number_members, void *userdata) {
    // Append the received data to the string
    char *response = (char *)userdata;
    unsigned int realsize = size * number_members;
    memcpy(response + strlen(response), ptr, realsize);
    response[realsize + strlen(response)] = 0;
    return realsize;
}


// Take url return json
cJSON * sendRequest(char * url){
    CURL *curl;
    char response[256000] = {0};
    CURLcode res;
    cJSON * json = NULL;

    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
        struct curl_slist *headers = NULL;
        headers = curl_slist_append(headers, "Authorization: Bearer " BEARER_TOKEN);
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, response);

        res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        }

        // Print the response JSON
        json = cJSON_Parse(response);
        if (cJSON_IsNull(json))
            printf("\n Error in parsing");
        //char *string = cJSON_Print(json);
        //printf("\nJSON in the func %s", string);

        curl_easy_cleanup(curl);
        return json;
    } else {
        return NULL;
    }
}

//Take name return id
char* getIdFromName(char *name){
    CURL *curl;
    char response[4096];
    CURLcode res;
    cJSON * json = NULL;
    cJSON * data = NULL;
    cJSON * id = NULL;
    int user_id = 0;
    char *url;
    const char endpoint[] = "https://api.twitter.com/2/users/by/username/";

    // Create the URL
    url = malloc((strlen(name) + strlen(endpoint)) * sizeof(char*));
    strcpy(url, "https://api.twitter.com/2/users/by/username/");
    strcat(url,name);
    if (url[strlen(url)-1] == '\n')
        url[strlen(url)-1] = '\0';

    // Send the request
    json = sendRequest(url);
    if (cJSON_IsNull(json))
        printf("\nThe JSON is NULL");
    // Print the response JSON
    //char *string = cJSON_Print(json);
    //printf("\nJSON I got %s\n", string);


    // Get content
    data = cJSON_GetObjectItemCaseSensitive(json, "data");
    if (!cJSON_IsObject(data)){
        printf("Got nothing with that username");
        return 0;
    }

    id = cJSON_GetObjectItemCaseSensitive(data,"id");
    if (!cJSON_IsString(id)){
        printf("Error data.id is not a string");
        return 0;
    }

    user_id = atoi(id->valuestring);

    free(url);
    //  cJSON_Delete(id);
    return cJSON_IsString(id)?id->valuestring:0;
}

char* getLastTweetID(char *user_name){
    cJSON * json;
    cJSON * meta;
    cJSON * last_tweet;
    char *url;
    const char endpoint[] = "https://api.twitter.com/2/tweets/search/recent?query=from:";

    // Create the URL
    url = malloc((strlen(user_name) + strlen(endpoint)) * sizeof(char*));
    strcpy(url, endpoint);
    strcat(url,user_name);
    if (url[strlen(url)-1] == '\n')
        url[strlen(url)-1] = '\0';

    // Send the request
    json = sendRequest(url);
    if (cJSON_IsNull(json))
        printf("\nThe JSON is NULL");
    // Print the response JSON
    //char *string = cJSON_Print(json);
    //printf("\nJSON I got %s\n", string);

    // Get content
    meta = cJSON_GetObjectItemCaseSensitive(json, "meta");
    if (!cJSON_IsObject(meta))
        printf("\nGot nothing with that username");
    last_tweet = cJSON_GetObjectItemCaseSensitive(meta,"newest_id");
    if (!cJSON_IsString(last_tweet)){
        printf("\nThat user either does not exist or hasn't tweet anything for a long time");
    }

    free(url);
    //  cJSON_Delete(id);
    return cJSON_IsString(last_tweet)?last_tweet->valuestring:0;
}

char* getTweetText(char *tweet_id){
    cJSON * json;
    cJSON * data;
    cJSON * text;
    char *url;
    const char endpoint[] = "https://api.twitter.com/2/tweets/";

    // Create the URL
    url = malloc((strlen(tweet_id) + strlen(endpoint)) * sizeof(char*));
    strcpy(url, endpoint);
    strcat(url,tweet_id);
    if (url[strlen(url)-1] == '\n')
        url[strlen(url)-1] = '\0';

    // Send the request
    json = sendRequest(url);
    if (cJSON_IsNull(json))
        printf("\nThe JSON is NULL");
    // Print the response JSON
    //char *string = cJSON_Print(json);
    //printf("\nJSON I got %s\n", string);

    // Get content
    data = cJSON_GetObjectItemCaseSensitive(json, "data");
    text = cJSON_GetObjectItemCaseSensitive(data,"text");

    free(url);
    //  cJSON_Delete(text);
    return text->valuestring;
}


cJSON* sendPostRequest(char *url, char * postfields){
    CURL *curl;
    char response[256000] = {0};
    CURLcode res;
    cJSON * json = NULL;

    curl_global_init(CURL_GLOBAL_ALL);

    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, "sign=aries&day=today");
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, response);

        res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        }

        // Print the response JSON
        json = cJSON_Parse(response);
        if (cJSON_IsNull(json))
            printf("\n Error in parsing");
        //char *string = cJSON_Print(json);
        //printf("\nJSON in the func %s", string);

        curl_easy_cleanup(curl);
        curl_global_cleanup();
        return json;
    }
}

char* getHoroscope(char *astrological){
    char * url;
    char *endpoint = "https://aztro.sameerkumar.website/?";
    char *day = "&day=today";
    char *sign;
    char * postFields;
    cJSON * json;
    cJSON * description;

    // Allocating sign
    sign = malloc((strlen("sign=") + strlen(astrological)+100 ) * sizeof(char));
    if (sign == NULL) {
        return 0;
    }
    strcpy(sign, "sign=");
    strcat(sign,astrological);

    // Allocating postFields
    postFields = malloc ( ( strlen(sign) + strlen(day)+100 ) * sizeof(char) );
    if (postFields == NULL) {
        return 0;
    }
    strcpy(postFields, sign);
    strcat(postFields, day);

    // Setting url
    url = malloc((strlen(endpoint) + strlen(postFields)) * sizeof(char));
    if (url == NULL) {
        return 0;
    }
    strcpy(url,endpoint);
    strcat(url,postFields);

    //printf("Here is url : %s", url);

    //printf("\nURL : %s", url);
    //printf("\npostFields : %s", postFields);

    if (url[strlen(url)-1] == '\n')
        url[strlen(url)-1] = '\0';

    // Sending request
    json = sendPostRequest(url, postFields);
    if(cJSON_IsNull(json)){
        printf("sendPostRequest JSON is NULL");
        return 0;
    }

    // Get content
    description = cJSON_GetObjectItemCaseSensitive(json, "description");
    if (!cJSON_IsString(description))
        printf("Error the description item is not an string");
    return description->valuestring != NULL?description->valuestring:0;
}

struct SubjectData initSubjects() {
    cJSON *json = loadJSON();
    struct SubjectData data;
    data.subjects = NULL;
    data.numSubjects = 0;

    cJSON *subjectsJSON = cJSON_GetObjectItemCaseSensitive(json, "subjects");
    int numSubjectsJSON = cJSON_GetArraySize(subjectsJSON);

    //printf("\nThere is %d subjects", numSubjectsJSON);

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

        //printf("\nSubject NAME : %s", subject->name);
        //printf("\nNumber of options : %d", subject->numOptions);

        // Iterate through the options array and extract the data
        for (int j = 0; j < subject->numOptions; j++) {
            cJSON *optionJSON = cJSON_GetArrayItem(optionsJSON, j);
            subject->options[j] = strdup(optionJSON->valuestring);
            //printf("\nSubject options[%d], %s", j, subject->options[j]);
        }
        // Add the Subject struct to the subjects array
        addSubject(&data, subject->name, subject->numOptions, subject->options);
    }
    //printf("\nJSON loaded successfully\n");
    return data;
}

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
