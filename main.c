#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <stdbool.h>
#include <stdlib.h>

bool file_exists(char *filename)
{
    struct stat buffer;
    return (stat(filename, &buffer) == 0);
}
char* insert(char *file_name, char *inp_str, int line_no, int pos)
{
    if (!file_exists(file_name))
    {
        char* out = "EROR: file not found!";
        return out;
    }

    FILE *fptr;
    fptr = fopen(file_name, "r");

    char str[1024];
    str[0] = '\0';
    for (int i = 0; inp_str[i] != '\0'; i++)
    {

        while (inp_str[i] == '\\')
        {

            if (inp_str[i + 1] == 'n')
            {
                strcat(str, "\n\0");
                i += 2;
            }

            else if (inp_str[i + 1] == '\\')
            {
                strcat(str, "\\\0");
                i += 2;
            }

            else if (inp_str[i + 1] == '\"')
            {
                strcat(str, "\"\0");
                i += 2;
            }
        }

        if (inp_str[i] == '\0')
        {
            break;
        }
        char c[2];
        c[0] = inp_str[i];
        c[1] = '\0';
        strcat(str, c);
    }

    char file[4096] ;
    file[0] = '\0';
    for (int i = 1; i < line_no; i++)
    {
        char line[1024];
        if (fgets(line, 1024, fptr) == NULL)
        {
            strcat(file, "\n");
        }
        else
        {
            strcat(file, line);
        }
    }

    char line[1024];
    if (fgets(line, 1024, fptr) == NULL)
    {
        for (int i = 1; i <= pos ; ++i) {
            strcat(file, " ");
        }

        strcat(file, str);
        fclose(fptr);

        fptr = fopen(file_name, "w");
        fputs(file, fptr);
        fclose(fptr);

        char* out = "SUCCESS: sentence inserted!";
        return out;
    }

    int lengh = strlen(line) - 1;
    if(lengh < pos)
    {
        strncat(file, line, lengh);
        for (int i = 1; i <= pos - lengh ; ++i) {
            strcat(file, " ");
        }
        strcat(file, str);
        strcat(file, "\n");

    }

    else {
        strncat(file, line, pos);
        strcat(file, str);
        strcat(file, line + pos);
    }

    for (int i = 1;; i++)
    {
        char aftr_line[1024];
        if (fgets(aftr_line, 1024, fptr) == NULL)
        {
            break;
        }
        strcat(file, aftr_line);
    }

    fclose(fptr);

    fptr = fopen(file_name, "w");
    fputs(file, fptr);
    fclose(fptr);

    char* out = "SUCCESS: sentence inserted!";
    return out;
}

char* removestr(char *file_name, int line_no, int pos, int size, char bnf)
{
    if (!file_exists(file_name))
    {
        char* out ="EROR: file not found!";
        return out;
    }

    FILE *fptr;
    fptr = fopen(file_name, "r");
    char* str_bfr = (char*) malloc(sizeof(char));
    char* str_afr = (char*) malloc(sizeof(char));
    int max_bfr = 0;
    while (1)
    {
        fscanf(fptr, "%c", &str_bfr[max_bfr]);
        if (line_no == 1 && pos == 1)
        {
            break;
        }
        if (line_no == 1)
        {
            pos--;
        }
        if (str_bfr[max_bfr] == '\n')
        {
            line_no--;
        }
        max_bfr++;
        str_bfr = (char*) realloc(str_bfr, (max_bfr+1) * sizeof(char));

    }

    int max_afr = 0;
    while (1)
    {
        fscanf(fptr, "%c", &str_afr[max_afr]);
        if (str_afr[max_afr] == '\0')
        {
            break;
        }
        max_afr++;
        str_afr = (char*) realloc(str_afr, (max_afr+1) * sizeof(char));
    }
    fclose(fptr);
    //for string cheking
    //printf("%s&&&%s",str_bfr, str_afr);

    fptr = fopen(file_name, "w");

    int forward = 0;
    int backward = max_bfr;

    if (bnf == 'f')
    {
        forward += (size - 1);
    }
    else
    {
        backward -= (size - 1);
    }

    for (int idx = 0; idx < backward; idx++)
    {
        fprintf(fptr, "%c", str_bfr[idx]);
    }
    free(str_bfr);

    for (int idx = forward ; idx < max_afr; idx++)
    {
        fprintf(fptr, "%c", str_afr[idx]);
    }
    fprintf(fptr, "\0");
    free(str_afr);
    fclose(fptr);
    char* out = "SUCCESS: sentence removed!";
    return out;
}

char* compare(char* file1, char* file2){

    if (!file_exists(file1))
    {
        return "EROR: file1 not found!";
    }
    if (!file_exists(file2))
    {
        return "EROR: file2 not found!";
    }

    FILE *f1ptr;
    f1ptr = fopen(file1,"r");

    FILE *f2ptr;
    f2ptr = fopen(file2,"r");

    int line = 0;
    int strt_line;
    int end_line;

    char save1[4096];
    char save2[4096];
    char out[4096];

    while(1){
        line++;

        if(fgets(save1, sizeof(save1), f1ptr) == NULL){

            strt_line = line;
            end_line = line - 1;

            while(1){

                if(fgets(save2, sizeof(save2), f2ptr) == NULL){
                    if (out[0] != '\0')
                        printf(">>>>>>>>>>>> #%i - #%i >>>>>>>>>>>>\n%s", strt_line, end_line, out);
                    break;
                }
                strcat(out,save2);
                end_line++;

            }

            break;
        }

        if(fgets(save2, sizeof(save2), f2ptr) == NULL){

            strt_line = line;
            end_line = line - 1;

            while(1){

                strcat(out,save1);
                end_line++;

                if(fgets(save1, sizeof(save1), f1ptr) == NULL){
                    printf("<<<<<<<<<<<< #%i - #%i <<<<<<<<<<<<\n%s", strt_line, end_line, out);
                    break;
                }

            }

            break;
        }

        if (strcmp(save1, save2) != 0){
            printf("============ #%i ============\n%s%s", line, save1, save2);
        }

    }

    fclose(f1ptr);
    fclose(f2ptr);

    return "SUCCESS: two file compered!";
}
// The main function that checks if two given strings
// match. The first string may contain wildcard characters
bool match(char* first, char* second)
{
    // If we reach at the end of both strings, we are done
    if (*first == '\0' && *second == '\0')
        return true;

    // Make sure to eliminate consecutive '*'
    if (*first == '*') {
        while (*(first + 1) == '*')
            first++;
    }

    // Make sure that the characters after '*' are present
    // in second string. This function assumes that the
    // first string will not contain two consecutive '*'
    if (*first == '*' && *(first + 1) != '\0'
        && *second == '\0')
        return false;

    // If the first string contains '?', or current
    // characters of both strings match
    if (*first == *second)
        return match(first + 1, second + 1);

    // If there is *, then there are two possibilities
    // a) We consider current character of second string
    // b) We ignore current character of second string.
    if (*first == '*')
        return match(first + 1, second)
               || match(first, second + 1);
    return false;
}

int find_byword(char* inp,char* check, int at){

    int lengh = 1;

    char without_newline[4096];

    char* line = strtok(inp,"\n");

    while (line != NULL) {

        strcat(without_newline, line);
        strcat(without_newline," ");
        line = strtok(NULL, "\n");
    }

    char *token = strtok(without_newline, " ");
    while (token != NULL) {

        if (match(check, token) == 1) {
            at--;
            if (at == 0)
                return lengh;
        }
        lengh += 1;
        token = strtok(NULL, " ");

    }
    return -1;

}

int find_bychar(char* inp, char* check, int at){

    int lengh = 1;

    char without_newline[4096];
    char* line = strtok(inp,"\n");

    while (line != NULL) {

        strcat(without_newline, line);
        strcat(without_newline," ");
        line = strtok(NULL, "\n");
    }

    char *token = strtok(without_newline, " ");
    while (token != NULL) {

        if (match(check, token) == 1) {
            at--;
            if (at == 0)
                return lengh;
        }
        lengh += (strlen(token) + 1);
        token = strtok(NULL, " ");

    }
    return -1;

}

int find_byword_without_star(char* inp,char* check, int at){

    int lengh = 1;

    char without_newline[4096];

    char* line = strtok(inp,"\n");

    while (line != NULL) {

        strcat(without_newline, line);
        strcat(without_newline," ");
        line = strtok(NULL, "\n");
    }

    char *token = strtok(without_newline, " ");
    while (token != NULL) {

        if (strcmp(check, token) == 0) {
            at--;
            if (at == 0)
                return lengh;
        }
        lengh += 1;
        token = strtok(NULL, " ");

    }
    return -1;

}

int find_bychar_without_star(char* inp,char* check,int at){

    int lengh = 1;

    char without_newline[4096];

    char* line = strtok(inp,"\n");

    while (line != NULL) {

        strcat(without_newline, line);
        strcat(without_newline," ");
        line = strtok(NULL, "\n");
    }

    char *token = strtok(without_newline, " ");
    while (token != NULL) {
        printf("%s ",token);
        if (strcmp(check, token) == 0) {
            at--;
            if (at == 0)
                return lengh;
        }
        lengh += (strlen(token) + 1);;
        token = strtok(NULL, " ");

    }
    return -1;

}

int find(char* str, char* file_name, int byword, int at, int all){

     if (!file_exists(file_name))
     {
         printf("EROR: file not found!");
         return 0;
     }

     if (at != 0 && all == 1)
     {
         printf("EROR: this action is unavailable!");
         return -2;
     }

     if (at != 0)
     {
         FILE *fptr;
         fptr = fopen(file_name, "r");

         char *save = (char *) malloc(sizeof(char));

         for (int i = 0; save[i] != -1; i++) {
             save = (char *) realloc(save, (i + 1) * sizeof(char));
             fscanf(fptr, "%c", &save[i]);
         }
         fclose(fptr);

         int max = strlen(str);
         if (str[max - 1] == '*' && str[max - 2] == '\\') {

             char star_edit[0];
             strncpy(star_edit, str, max - 2);
             strcat(star_edit, "*");

             if (byword == 1) {
                 return find_byword_without_star(save, star_edit, at);
             } else {
                 return find_bychar_without_star(save, star_edit, at);
             }
         } else if (str[1] == '*' && str[0] == '\\') {
             char save[0];
             char star_edit[] = "*";
             strncpy(save, str + 2, max);
             strcat(star_edit, save);

             if (byword == 1) {
                 return find_byword_without_star(save, star_edit, at);
             } else {
                 return find_bychar_without_star(save, star_edit, at);
             }
         }

         if (byword == 1) {
             return find_byword(save, str, at);
         } else {
             return find_bychar(save, str, at);
         }
     }

     if (all == 1)
     {
         int num = 1;
         while(1)
         {
             int out = find(str, file_name, byword, num, 0);
             if (out == -1)
                 break;
             printf("%i,", out);
             num++;
         }
         return -2;
     }

}

char* replace(char* str1, char* str2, char* file_name, int at, int all)
{
    if (!file_exists(file_name))
    {
        return "EROR: file1 not found!";
    }
    if(at != 0 && all ==1)
    {
        return "EROR: this action is unavailable!";
    }
    else if(at != 0)
    {
        FILE *fptr;
        fptr = fopen(file_name,"r");
        char *save = (char*) malloc(sizeof(char));
        for(int i = 0; save[i] != -1; i++){
            save = (char*) realloc(save, (i+1) * sizeof(char));
            fscanf(fptr, "%c", &save[i]);
        }
        fclose(fptr);

        int  max = find(str1, file_name, 0, at, 0);
        if (max == -1)
            return "EROR: could not find the sentence!";
        int point = 0;
        int line = 1;
        for(int idx = 0; idx < max; idx++)
        {
            point++;
            if (save[idx] == '\n')
            {
                line++;
                point = 0;
            }
        }

        int size = (int) strlen(str1);
        removestr(file_name, line, point, size, 'f');
        insert(file_name, str2, line, point - size + 1);

        return "SUCCESS: sentence replaced!";
    }
    else if(all == 1)
    {
        int num = 0;
        while(1)
        {
//            char out[35];
//            strcpy (out, replace(file_name, str1, str2, num, 0))
            if( strcmp("EROR: could not find the sentence!", replace(str1, str2, file_name, 1, 0)) == 0)
                break;
            num = 1;
        }

        if (num == 0)
            return "EROR: could not find the sentence!";
        else
            return "SUCCESS: all sentences replaced!";
    }
}

//char* createfile(char* file_name){
//
//    if (file_exists(file_name)){
//        return "EROR: this file already exists!";
//    }
//    else{
//        FILE* fptr;
//        fptr = fopen(file_name,"w");
//        fclose(fptr);
//        return "SUCCESS: file created!";
//    }
//}



// int find(char* str, char* file_name){

//     if (!file_exists(file_name))
//     {
//         printf("EROR: file not found!");
//         return;
//     }

//     FILE *fptr;
//     fptr = fopen(file_name,"r");

//     char *save = (char*) malloc(sizeof(char));

//     for(int i = 0; save[i] != -1; i++){
//         save = (char*) realloc(save, (i+1) * sizeof(char));
//         fscanf(fptr, "%c", &save[i]);
//     }

//     fclose(fptr);

//     printf("%s\n%s", save, str);

//     int save_len = strlen(save);
//     if (strstr(save,str) == 0){
//         free(save);
//         return -1;
//     }

//     int out_len = strlen(strstr(save, str)) ;
//     free(save);

//     return save_len + 1 - out_len;


// }
int main()
{
    int a = find("hl","text1.txt",0,1,0);

    printf("\n%i",a);
//     char* a = replace("|srtm;kmb o[ |", "&&&", "text1.txt",  0, 1);
//    printf(a);
 //   removestr("text1.txt", 2, 8, 2, 'f');

}