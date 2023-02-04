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

char *removestr(char *address, int lineNo, int pos, int size, char bOrF)
{
    if (!file_exists(address))
    {
        return "ERROR! File has not been found!";
    }
    FILE *file;
    file = fopen(address, "r");
    char *res = (char *)malloc(256);
    res[0] = 0;
    int currLine = 1;
    int bufferLength = 255;
    char buffer[bufferLength];
    while (fgets(buffer, bufferLength, file))
    {
        if (currLine != lineNo)
        {
            strcat(res, buffer);
        }
        else
        {
            int idx = 0;
            while (buffer[idx] != '\0')
            {
                char c[2] = {buffer[idx], '\0'};
                if (idx < pos - 1 || idx > pos - 2 + size)
                {
                    strcat(res, c);
                }
                idx++;
            }
        }
        currLine++;
    }

    fclose(file);
    file = fopen(address, "w");
    fputs(res, file);
    fclose(file);
    return "Characters successfully removed!";
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

char* pre_find(char* str, char* file_name, int bychar, int byword, int at_inp, int all, int count)
{
    if (!file_exists(file_name))
    {
        return "EROR: file not found!";
    }
    if (at_inp != 0 && all == 1)
    {
        return "EROR: this action is unavailable!";
    }
    int at = 1;
    at += at_inp - 1;
    if (bychar == 1 &&  byword == 1)
    {
        return "EROR: this action is unavailable!";
    }

    int word = 1;
    for (int i = 1; str[i - 1] != '\0'; i++)
    {
        if (str[i - 1] == ' ')
            word++;
    }
    int max;
    int out_location[100];
    char words[word][100];
    char *token = strtok(str, " ");

    for (int idx = 0; token != NULL; idx++)
    {
        strcpy(words[idx], token);
        token = strtok(NULL, " ");
    }

    for (int idx = 0; idx < word; idx++)
    {
        int location[100];
        for (int num = 1;; num++) {
            location[num - 1] = find(words[idx], file_name, 1, num, 0);
            if (0 == idx)
                out_location[num - 1] = location[num - 1];
            if( location[num - 1] == -1)
                break;
            location[num - 1] -=idx;
        }

//        for (int x = 1; location[x - 1] != -1; x++) {
//            printf("%i ", location[x - 1]);
//        }
//        printf("\n");

        int temp[100];
        int z = 0;
        for (int i = 0; out_location[i] != -1; i++)
        {
            for (int j = 0; location[j] != -1; j++)
            {
                if (out_location[i] == location[j])
                {
                    temp[z] = out_location[i];
                    z++;
                }
            }
        }
        temp[z] = -1;

        for (int i = 0;; i++)
        {
            out_location[i] = temp[i];
            if (temp[i] == -1){
                if (0 == i)
                    max = 0;
                break;
            }
            max = i + 1;
        }
    }

//    for (int x = 1; out_location[x - 1] != -1; x++) {
//        printf("%i ", out_location[x - 1]);
//    }

    char ex[1024];
    ex[0] = '\0';
    if (count == 1)
    {
        char temp[100];
        sprintf(temp, "count = %i --> ", max);
        strcat(ex, temp);
    }

    if (byword == 1)
    {
        if (all == 1)
        {
            for (int x = 1; out_location[x - 1] != -1; x++) {
                char temp[100];
                sprintf(temp, "%i ", out_location[x - 1]);
                strcat(ex, temp);
            }
        }
        else if(at <= max)
        {
            char temp[100];
            sprintf(temp, "%i ", out_location[at - 1]);
            strcat(ex, temp);
        }
        else
        {
            strcat(ex, "-1");
        }
    }

    else if (bychar == 1)
    {
        if (all == 1)
        {
            for (int x = 1; out_location[x - 1] != -1; x++) {
                FILE *fptr = fopen(file_name, "r");
                int pos;
                int word_pos = 1;
                char c_ex = 'a';
                for (pos = 1; word_pos <= out_location[x - 1] - 1; pos++){
                    char c = fgetc(fptr);
                    if (c_ex != ' ' && c_ex != '\n' && c == ' ')
                        word_pos++;

                    if (c_ex != ' ' && c_ex != '\n' && c == '\n')
                        word_pos++;
                    c_ex = c;
                }
                fclose(fptr);

                char temp[100];
                sprintf(temp, "%i ", pos);
                strcat(ex, temp);
            }
        }
        else if(at <= max)
        {
            FILE *fptr = fopen(file_name, "r");
            int pos;
            int word_pos = 1;
            char c_ex = 'a';
            for (pos = 1; word_pos <= out_location[at - 1] - 1; pos++){
                char c = fgetc(fptr);
                if (c_ex != ' ' && c_ex != '\n' && c == ' ')
                    word_pos++;

                if (c_ex != ' ' && c_ex != '\n' && c == '\n')
                    word_pos++;
                c_ex = c;
            }
            fclose(fptr);

            char temp[100];
            sprintf(temp, "%i ", pos);
            strcat(ex, temp);
        }
        else
        {
            strcat(ex, "-1");
        }
    }

    char* ext = ex;
    return ext;


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
        int size = strlen(str1);

        FILE *fptr;
        fptr = fopen(file_name,"r");
        char *save = (char*) malloc(sizeof(char));
        for(int i = 0; save[i] != -1; i++){
            save = (char*) realloc(save, (i+1) * sizeof(char));
            fscanf(fptr, "%c", &save[i]);
        }
        fclose(fptr);
        char cpy_str1[size];
        strcpy(cpy_str1, str1);

        char out_of_find[1024];
        strcpy(out_of_find, pre_find(cpy_str1, file_name, 1, 0, at, 0, 0));

        int  max;
        sscanf(out_of_find, "%i", &max);
        //printf("%i ", max);
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
        removestr(file_name, line, point, size, 'f');
        insert(file_name, str2, line, point - 1);

        return "SUCCESS: sentence replaced!";
    }
    else if(all == 1)
    {
        int num = 0;
        while(1)
        {
            char cpy_str1[1024];
            strcpy (cpy_str1, str1);

            if( strcmp("EROR: could not find the sentence!", replace(cpy_str1, str2, file_name, 1, 0)) == 0)
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

    char b[100] = "halgnojnml";
    //printf(pre_find(b,"text1.txt",0, 1, 1, 0, 1));

    char h[100] = "hal to che";
    printf("\n%s",replace(h,"KKK","text1.txt",0,1));
//     char* a = replace("|srtm;kmb o[ |", "&&&", "text1.txt",  0, 1);
//    printf(a);

}