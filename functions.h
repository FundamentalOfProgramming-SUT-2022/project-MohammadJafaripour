char *rootFolder = "C:/Users/Public/Documents/root/";
#define revStr(a, b) (!strcmp(a, b))

int strIdx(char *haystack, char *needle)
{
    return strstr(haystack, needle) - haystack;
}

char *breackerOfStr(const char *forward, int begin, int end)
{
    long long int sizeOf = end - begin;
    char *max = (char *)malloc(sizeof(char) * (sizeOf + 1));
    for (int i = begin; i < end; i++)
    {
        *max = *(forward + i);
        max++;
    }
    *max = '\0';
    return max - sizeOf;
}

char *parsePath(char *forward)
{
    if (forward[1] != ':' && (forward[2] != '/' || forward[2] != '\\'))
    {
        char address[512];
        strcpy(address, rootFolder);
        strcat(address, forward);
        strcpy(forward, address);
    }
}
void CreateFolder(const char *path)
{
if (!CreateDirectory(path, NULL))
    return;
}

bool fileExist(char *filename)
{
    struct stat buffer;
    return (stat(filename, &buffer) == 0);
}

char *delQutation(char *inpOfStr)
{
if ('"' == inpOfStr[0])
    strcpy(inpOfStr, breackerOfStr(inpOfStr, 1, strlen(inpOfStr) - 1));
return inpOfStr;
}

char *createFile(char* locationOfFile)
{
CreateFolder(locationOfFile);
locationOfFile = parsePath(locationOfFile);
FILE *filePtr=fopen(locationOfFile, "w");

if (filePtr == NULL)
    return "can not create file.\n";
else
{
    fputs("\0", filePtr);
    return "Your file created.\n";
}
}
char *grep(char **files, char *strPat, int modSwitch)
{
    char *pVoid = malloc(4096);pVoid[0] = 0;
    bool putOut = 0,count = 0, list = 0;


    if (modSwitch == 0)
        putOut = 1;

    else if (modSwitch == 1)
        count = 1;

    else if (modSwitch == 2)
        list = 1;

    int amount = 0;

    for (int z = 0; 0 != files[z]; z++)
    {

        bool valid = fileExist(files[z]);
        if (1 == valid)
        {
            FILE *nowCarr=fopen(files[z], "a+");

            char *khat = malloc(256);
            khat[0] = 0;
            while (fgets(khat, 50, nowCarr))
            {

                if (strstr(khat, delQutation(strPat)) != NULL)
                {
                    if (putOut)
                    {
                        strcat(pVoid, files[z]);
                        strcat(pVoid, " : ");

                        strcat(pVoid, khat);
                        if (khat[strlen(khat) - 1] != '\n')
                        {
                            strcat(pVoid, "\n");
                        }
                    }
                    else if (list)
                    {
                        strcat(pVoid, files[z]);
                        strcat(pVoid, "\n");
                        break;
                    }
                    amount++;
                }
            }
        }
        else
        {
            strcat(pVoid, "Error! File \0");
            strcat(pVoid, *(files + z));
            strcat(pVoid, " has not been found!\n");
            strcat(pVoid, "\n");
        }
    }
    if (count)
    {
        char *caterPilar = malloc(256);
        sprintf(caterPilar, "%i", amount);
        strcat(pVoid, caterPilar);
        strcat(pVoid, "\n");
    }
    return pVoid;
}

char *insertstr(char *fileName, char *putStrInPlace, int lnum, int pos)
{
    lnum--;
    pos--;
    if (!fileExist(fileName))
        return "The File could not be found!";

    FILE *fptr = fopen(fileName, "r");

    char currArr[2048] = "\0";
    char A='\0';
    char B='\\';
    char C='n';
    char D='\"';
    long long int idxNum = 0;
    while (putStrInPlace[idxNum] != A)
    {

        while (putStrInPlace[idxNum] == B)
        {

            if (putStrInPlace[idxNum + 1] == C)
            {
                strcat(currArr, "\n\0");
            }
            else if (putStrInPlace[idxNum + 1] == D)
            {
                strcat(currArr, "\"\0");
            }

            else if (putStrInPlace[idxNum + 1] == B)
            {
                strcat(currArr, "\\\0");
            }



            idxNum += 2;
        }

        if (putStrInPlace[idxNum] == A)
        {
            break;
        }
        char Ch[2] = {putStrInPlace[idxNum], A};
        strcat(currArr, Ch);
        idxNum++;
    }

    char toBeStr[4096];
    toBeStr[0] = '\0';
    long long int situation = 0;
    while(situation < lnum)
    {
        char lNum[1024];
        if (fgets(lNum, 1024, fptr) == NULL)
        {
            strcat(toBeStr, "\n");
        }
        else if (fgets(lNum, 1024, fptr) != NULL)
        {
            strcat(toBeStr, lNum);
        }
        situation++;
    }

    char oneLiner[4096];
    if (fgets(oneLiner, 4096, fptr) == NULL)
    {
        long int X2;
        while ( X2 <= pos)
        {

            strcat(toBeStr, " ");
            X2++;
        }

        strcat(toBeStr, currArr);
        fclose(fptr);

        fptr = fopen(fileName, "w");
        fputs(toBeStr, fptr);
        fclose(fptr);

        return "The str was inserted";
    }

    int len = strlen(oneLiner) - 1;
    if (len < pos)
    {
        strncat(toBeStr, oneLiner, len);
        for (int i = 1; i <= pos - len; i++)
        {
            strcat(toBeStr, " ");
        }
        strcat(toBeStr, currArr);
        strcat(toBeStr, "\n");
    }

    else if (len>=pos)
    {
        strncat(toBeStr, oneLiner, pos);
        strcat(toBeStr, currArr);
        strcat(toBeStr, oneLiner + pos);
    }


    char restOfText[2048];
    while(fgets(restOfText, 2048, fptr) != NULL){

        strcat(toBeStr, restOfText);
    }

    fclose(fptr);

    fptr = fopen(fileName, "w");
    fputs(toBeStr, fptr);
    fclose(fptr);

    return "The string was inserted!";
}

char *removestr(char *inpStr, int lNum, int chr, int size, char BnF)
{
    if (!fileExist(inpStr))
    {
        return "str not found!";
    }
    FILE *file;
    file = fopen(inpStr, "r");
    char *Array = (char *)malloc(512);
    Array[0] = 0;
    long  int Line = 1,Harf=0;
    char A='\0';

    char OneLine[512];
    while (fgets(OneLine, 511, file)!=NULL)
    {
        if (Line != lNum)
        {
            strcat(Array, OneLine);
        }
        else
        {

            while (OneLine[Harf] != A)
            {
                char Ch[2] = {OneLine[Harf], '\0'};
                if (Harf < chr - 1 || Harf > chr - 2 + size)
                {
                    strcat(Array, Ch);
                }
                Harf++;
            }
        }
        Line++;
    }

    fclose(file);
    file = fopen(inpStr, "w");
    fputs(Array, file);
    fclose(file);
    return "Chars removed!";
}

char *cat(char *fileName)
{
    if (!fileExist(fileName))
        return "File does not exist";

    FILE *fptr = fopen(fileName, "r");

    if (fptr == NULL)
    {
        return ("File cannot be opened");
    }
    char *scan = malloc(256);
    char *outOfFile = malloc(256);
    outOfFile[0] = 0;
    while (0 != fgets(scan, 256, fptr))
    {
        strcat(outOfFile, scan);
    }
    fclose(fptr);
    return outOfFile;
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

int adv_find(char* str, char* file_name, int byword, int at, int all){

    if (!fileExist(file_name))
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
            int out = adv_find(str, file_name, byword, num, 0);
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
    if (!fileExist(file_name))
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
            location[num - 1] = adv_find(words[idx], file_name, 1, num, 0);
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
char* pre_replace(char* str1, char* str2, char* file_name, int at, int all)
{
    if (!fileExist(file_name))
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
            return "EROR: could not adv_find the sentence!";
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
        insertstr(file_name, str2, line, point - 1);

        return "SUCCESS: sentence replaced!";
    }
    else if(all == 1)
    {
        int num = 0;
        while(1)
        {
            char cpy_str1[1024];
            strcpy (cpy_str1, str1);

            if(strcmp("EROR: could not adv_find the sentence!", pre_replace(cpy_str1, str2, file_name, 1, 0)) == 0)
                break;
            num = 1;
        }

        if (num == 0)
            return "EROR: could not adv_find the sentence!";
        else
            return "SUCCESS: all sentences replaced!";
    }
}

/////
char *autoIndent(char *address)
{
    char *readData = cat(address);
    long int i = 0;
    int currLine = 1;
    int currPos = 1;
    int opened = 0;
    while (readData[i] != '\0')
    {
        if (readData[i] == '{')
        {
            insertstr(address, "\n", currLine, currPos);
            opened++;
        }
        if (readData[i] == '}')
        {
            insertstr(address, "\n", currLine, currPos);
            opened--;
        }
        if (readData[i] == '\n')
        {
            currPos = 1;
            currLine++;
        }
        currPos++;
        i++;
    }
    return "SUCCESS";
}
///////

char *copystr(char *address, int lnum, int chr, int size, char bOrF)
{
    FILE *file = fopen(address, "r");
    char line[256];
    if (!fileExist(address))
        return "File does not exist!";
    char *res = malloc(256);
    res[0] = 0;
    int currLine = 1;
    while (fgets(line, 256, file))
    {
        if (lnum == currLine)
        {
            if (bOrF == 'f')
            {
                for (int i = chr - 1; i < size + chr - 1; i++)
                {
                    char c[2] = {line[i]};
                    strcat(res, c);
                }
            }
            else
            {
                for (int i = chr - size; i < chr - 1; i++)
                {
                    char c[2] = {line[i]};
                    strcat(res, c);
                }
            }
        }
        currLine++;
    }
    const char *output = res;
    const size_t len = strlen(output) + 1;
    HGLOBAL hMem = GlobalAlloc(GMEM_MOVEABLE, len);
    memcpy(GlobalLock(hMem), output, len);
    GlobalUnlock(hMem);
    OpenClipboard(0);
    EmptyClipboard();
    SetClipboardData(CF_TEXT, hMem);
    CloseClipboard();
    return "Text copied to clipboard!";
}
char *cutstr(char *address, int lNum, int pos, int size, char bOrF)
{
    if (!fileExist(address))
        return "File does not exist!";
    copystr(address, lNum, pos, size, bOrF);
    removestr(address, lNum, pos, size, bOrF);
    return "Text cut to clipboard!";
}

char* compare(char* file1, char* file2){

    if (!fileExist(file1))
    {
        return "EROR: file1 not found!";
    }
    if (!fileExist(file2))
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

char *pastestr(char *inpStr, int lnum, int chr)
{
    if (!fileExist(inpStr))
        return "File does not exist!";
    char *strExit = malloc(32);
    strExit[0] = 0;
    OpenClipboard(0);
    strExit = GetClipboardData(CF_TEXT);CloseClipboard();

    insertstr(inpStr, strExit, lnum, chr);
    return "pasted!";
}

char *find(char *address, char *str, int byWord, int at, int count, int all)
{

    if (byWord)
    {
        FILE *file = fopen(address, "r");
        char *line = calloc(256,8);
        long long int Tool = 0,counter=0;
        int List[2048] = {0};

        while (fgets(line, 256, file))
        {
            char *MMA = strtok(line, " ");
            while (MMA != NULL)
            {
                if (!strcmp(MMA, str))
                {
                    List[counter] = Tool + 1;
                    counter++;
                }
                Tool++;
                MMA = strtok(NULL, " ");
            }
        }
        if (count)
        {
            char *output = calloc(256, 1);
            sprintf(output, "%d", counter);
            return output;
        }
        if (counter <= 0)
        {
            return "None";
        }
        else if (at)
        {

            if (all)
            {
                char *output = calloc(128 * 16, 1);
                char *X = calloc(128, 1);
                sprintf(X, "%d", List[0]);
                strcat(output, X);
                int i=0;
                while ( i < counter)
                {
                    sprintf(X, ", %d", List[i]);
                    strcat(output, X);
                    i++;
                }
                return output;
            }
            else if (counter < at)
            {
                return "At more than Possible!";
            }
            char *output = calloc(128, 1);
            sprintf(output, "%i", List[at - 1]);
            return output;
        }
        else
        {
            char *output = calloc(128, 1);
            sprintf(output, "%d", List[0]);
            return output;
        }
    }
    if (!byWord)
    {
        char *data = cat(address);
        int strSize = strlen(str);
        int;
        int List[1024] = {0};

        long int firstFound = 0, Tool = 0 , counter = 0;
        if (!strIdx(data, str))
        {
            firstFound = 1;
            counter++;
            List[0] = 0;
        }
        char *MMA = strtok(data, str);
        while (MMA != NULL)
        {
            if (!((MMA[0] == '\n' || MMA[0] == ' ') && strlen(MMA) == 1))
            {

                Tool += strlen(MMA);
                List[counter++] = Tool + 1;
                Tool += strSize;
            }
            MMA = strtok(NULL, str);
        }
        // counter--;
        if (firstFound && counter != 1)
        { counter--;}
        if (counter > 1)
        {counter--;}
        if (count)
        {
            char *output = calloc(128, 1);
            sprintf(output, "%d", counter);
            return output;
        }
        else if (counter <= 0)
        {
            return "None!";
        }
        else if (at)
        {
            if (counter < at)
            {
                return "At exeeded!";
            }
            char *output = calloc(128, 1);
            sprintf(output, "%d", List[at - 1] + 1);
            return output;
        }
        else if (all)
        {
            char *output = calloc(256 * 16, 1);
            char *X = calloc(256, 1);
            sprintf(X, "%d", List[0] + 1);
            strcat(output, X);
            int i=1;
            while( i < counter)
            {
                sprintf(X, ", %d", List[i] + 1);
                strcat(output, X);
                i++;
            }
            return output;
        }
        else
        {
            char *output = calloc(256, 1);
            sprintf(output, "%d", List[0] + 1);
            return output;
        }
    }
}
char *replace(char *address, char *str1, char *str2, int at, int all)
{
    long int size1 = strlen(str1);
    if (at)
    {
        int charPos = atoi(find(address, str1, 0, at, 0, 0));
        charPos--;
        printf("%i", charPos);
        char *data = calloc(4096, 1);
        strcpy(data, cat(address));
        long int line = 1, pos = 1, cntr = 0, i = 0;
        while (cntr < charPos)
        {
            pos++;
            if (data[i] == '\n')
            {
                line++;
                pos = 1;
                cntr -= 2;
            }
            cntr++;
            i++;

        }
        removestr(address, line, pos, size1, 'f');
        insertstr(address, str2, line, pos);
        if (!all)
            return "Replaced";
    }
    if (!!strcmp(find(address, str1, 0, 0, 0, 0), "No results!"))
    {
        replace(address, str1, str2, 0, 1);
    }
    return "String has been replaced!";
}