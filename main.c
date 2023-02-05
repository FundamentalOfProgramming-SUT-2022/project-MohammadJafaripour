#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <conio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdbool.h>
#include <windows.h>

#ifdef WIN32
#include <io.h>
#define F_OK 0
#define access _access
#endif

#ifdef UNIX
#include <unistd.h>
#endif

#define revStr(a, b) (!strcmp(a, b))

#include "functions.h" //-->my header file

char *strAnalyse(char **arrOfStr);
char **strExpand(char *stdInLine);

int main(){
while(1) {
CreateFolder("C:/Users/Public/Documents/root/");
char inp[512];
char* out;

gets(inp);
if (strcmp(inp, "sudo out") == 0)
    break;

char** arr = strExpand(inp);
out = strAnalyse(arr);

puts(out);
}
return 0;
}

char *strAnalyse(char **arrOfStr)
{
    char *baseCmd = arrOfStr[0];
    if (revStr(baseCmd, "createfile"))
    {
        char fileAddress[] = "";
        for (int i = 1; arrOfStr[i] != NULL && arrOfStr[i + 1] != NULL; i += 2)
        {
            char *argKey = arrOfStr[i];
            char *argVal = arrOfStr[i + 1];

            if (argVal[0] == '"')
            {
                strcpy(argVal, breackerOfStr(argVal, 1, strlen(argVal) - 1));
            }

            if (revStr(argKey, "-file"))
            {
                strcpy(fileAddress, argVal);
            }
        }
        return (createFile(fileAddress));
    }
    else if (revStr(baseCmd, "insertstr"))
    {
        // needed
        char *address = malloc(128);
        int lineNo = 0, pos = 0;
        char *str = malloc(128);
        //
        for (int i = 1; arrOfStr[i] != NULL && arrOfStr[i + 1] != NULL; i += 2)
        {
            char *argKey = arrOfStr[i];
            char *argVal = arrOfStr[i + 1];

            argVal = delQutation(argVal);

            if (revStr(argKey, "-file"))
            {
                strcpy(address, argVal);
            }
            else if (revStr(argKey, "-str"))
            {
                strcpy(str, argVal);
            }
            else if (revStr(argKey, "-pos"))
            {
                sscanf(argVal, "%i:%i", &lineNo, &pos);
            }
        }
        return insertstr(parsePath(address), str, lineNo, pos);
    }
    else if (revStr(baseCmd, "cat"))
    {
        char output[256];
        for (int idxOfArr = 1;
        arrOfStr[idxOfArr] != NULL&& arrOfStr[idxOfArr + 1] != NULL;
        idxOfArr += 2)
        {
            char *strNext = arrOfStr[idxOfArr];
            char *strEx = arrOfStr[idxOfArr + 1];
            strEx = delQutation(strEx);
            if (revStr(strNext, "-file"))
                strcpy(output, strEx);
        }
        return cat(parsePath(output));
    }

    else if (revStr(baseCmd, "removestr"))
    {
        long int lNum = 0;
        char address[256];
        int pos = 0;
        long long tol = 0;
        char FnB = 'w';

        int i = 1;
        while ( arrOfStr[i] != NULL && arrOfStr[i + 1] != NULL)
        {
            char *strEx = arrOfStr[i];
            char *strNext = arrOfStr[i + 1];

            strNext = delQutation(strNext);

            if (revStr(strEx, "-file"))
                strcpy(address, strNext);

            else if (revStr(strEx, "-pos"))
                sscanf(strNext, "%i:%i", &lNum, &pos);

            else if (revStr(strEx, "-tol"))
                tol = atoll(strNext);

            i += 2;

        }

        if (revStr(arrOfStr[i], "-b"))
            FnB = 'b';

        else if (revStr(arrOfStr[i], "-f"))
            FnB = 'f';

        return removestr(parsePath(address), lNum, pos, tol, FnB);
    }
    else if (revStr(baseCmd, "copystr"))
    {
        // needed
        char *address = malloc(128);
        int lineNo = 0, pos = 0;
        long long size = 0;
        char fOrB = 'a';

        //
        int i = 1;

        for (i = 1; arrOfStr[i] != NULL && arrOfStr[i + 1] != NULL; i += 2)
        {
            char *argKey = arrOfStr[i];
            char *argVal = arrOfStr[i + 1];

            argVal = delQutation(argVal);

            if (revStr(argKey, "-file"))
            {

                strcpy(address, argVal);
            }

            else if (revStr(argKey, "-pos"))
            {
                sscanf(argVal, "%i:%i", &lineNo, &pos);
            }
            else if (revStr(argKey, "-size"))
            {
                size = atoll(argVal);
            }
        }

        if (revStr(arrOfStr[i], "-b"))
        {
            fOrB = 'b';
        }
        if (revStr(arrOfStr[i], "-f"))
        {
            fOrB = 'f';
        }

        return copystr(parsePath(address), lineNo, pos, size, fOrB);
    }
    else if (revStr(baseCmd, "cutstr"))
    {
        // needed
        char *address = malloc(128);
        int lineNo = 0, pos = 0, size = 0;
        ;
        char fOrB = 'a';
        //
        int i = 1;

        for (i = 1; arrOfStr[i] != NULL && arrOfStr[i + 1] != NULL; i += 2)
        {
            char *argKey = arrOfStr[i];
            char *argVal = arrOfStr[i + 1];

            argVal = delQutation(argVal);

            if (revStr(argKey, "-file"))
            {
                strcpy(address, argVal);
            }
            if (revStr(argKey, "-size"))
            {
                sscanf(argVal, "%i", &size);
            }
            else if (revStr(argKey, "-pos"))
            {
                sscanf(argVal, "%i:%i", &lineNo, &pos);
            }
        }
        if (revStr(arrOfStr[i], "-b"))
        {
            fOrB = 'b';
        }
        if (revStr(arrOfStr[i], "-f"))
        {
            fOrB = 'f';
        }
        return cutstr(parsePath(address), lineNo, pos, size, fOrB);
    }
    else if (revStr(baseCmd, "pastestr"))
    {
        // needed
        char *address = malloc(128);
        int lineNo = 0, pos = 0;
        //
        for (int i = 1; arrOfStr[i] != NULL && arrOfStr[i + 1] != NULL; i += 2)
        {
            char *argKey = arrOfStr[i];
            char *argVal = arrOfStr[i + 1];

            argVal = delQutation(argVal);

            if (revStr(argKey, "-file"))
            {
                strcpy(address, argVal);
            }
            else if (revStr(argKey, "-pos"))
            {
                sscanf(argVal, "%i:%i", &lineNo, &pos);
            }
        }
        return (pastestr(parsePath(address), lineNo, pos));
    }
    else if (revStr(baseCmd, "find"))
    {
        // needed
        char *address = malloc(128);
        char *str = malloc(128);
        int byword = 0, at = 0, count = 0, all = 0;

        //
        for (int i = 1; arrOfStr[i] != NULL && arrOfStr[i + 1] != NULL; i += 2)
        {
            char *argKey = arrOfStr[i];
            char *argVal = arrOfStr[i + 1];

            argVal = delQutation(argVal);

            if (revStr(argKey, "-file"))
            {
                strcpy(address, argVal);
            }
            if (revStr(argKey, "-str"))
            {
                strcpy(str, argVal);
            }
        }
        int i = 5;
        while (arrOfStr[i] != NULL)
        {
            // printf("-{%s}-\n", arrOfStr[i]);
            if (revStr(arrOfStr[i], "-count"))
            {
                count = 1;
            }
            if (revStr(arrOfStr[i], "-at"))
            {
                sscanf(arrOfStr[i + 1], "%d", &at);
                i++;
            }
            if (revStr(arrOfStr[i], "-all"))
            {
                all = 1;
            }
            if (revStr(arrOfStr[i], "-byword"))
            {
                byword = 1;
            }

            i++;
        }
        return find(parsePath(address), str, byword, at, count, all);
    }
    else if (revStr(baseCmd, "replace"))
    {
        // needed
        char *address = malloc(128);
        char *str1 = malloc(128);
        char *str2 = malloc(128);
        int at = 0, all = 0;
        //
        for (int i = 1; arrOfStr[i] != NULL && arrOfStr[i + 1] != NULL; i += 2)
        {
            char *argKey = arrOfStr[i];
            char *argVal = arrOfStr[i + 1];

            argVal = delQutation(argVal);

            if (revStr(argKey, "-file"))
            {
                strcpy(address, argVal);
            }
            if (revStr(argKey, "-str1"))
            {
                strcpy(str1, argVal);
            }
            if (revStr(argKey, "-str2"))
            {
                strcpy(str2, argVal);
            }
        }
        int i = 5;
        while (arrOfStr[i] != NULL)
        {

            if (revStr(arrOfStr[i], "-at"))
            {
                sscanf(arrOfStr[i + 1], "%d", &at);
                i++;
            }
            else if (revStr(arrOfStr[i], "-all"))
            {
                all = 1;
            }

            i++;
        }
        return replace(parsePath(address), str1, str2, at, all);
    }
    else if (revStr(baseCmd, "grep"))
    {
        char **listFileOfInp = (char **)malloc(64 * sizeof(char *));
        char *inpStr = malloc(128);
        int currNumOfFiles = 0, halat = 0;

        if (revStr("-str", arrOfStr[1]) != 0)
        {
            strcpy(inpStr, arrOfStr[2]);
            int i = 4;
            while (arrOfStr[i] != NULL)
            {
                *(listFileOfInp + currNumOfFiles) = parsePath(delQutation(arrOfStr[i]));
                currNumOfFiles++;
                i++;
            }
        }
        else
        {
            if (revStr("-c", arrOfStr[1]))
            {
                halat = 1;
            }
            else if (revStr("-l", arrOfStr[1]))
            {
                halat = 2;
            }

            strcpy(inpStr, arrOfStr[3]);
            int i = 5;
            while (arrOfStr[i] != NULL)
            {
                *(listFileOfInp + currNumOfFiles) = parsePath(delQutation(arrOfStr[i]));
                currNumOfFiles++;
                i++;
            }
        }
        *(listFileOfInp + currNumOfFiles) = NULL;
        int j = 0;

        // function used
        return grep(listFileOfInp, inpStr, halat);
    }
    else if (revStr(baseCmd, "undo"))
    {
        // needed
        char *address = malloc(128);
        //
        for (int i = 1; arrOfStr[i] != NULL && arrOfStr[i + 1] != NULL; i += 2)
        {
            char *argKey = arrOfStr[i];
            char *argVal = arrOfStr[i + 1];

            argVal = delQutation(argVal);

            if (revStr(argKey, "-file"))
            {
                strcpy(address, argVal);
            }
        }
    }
    else if (revStr(baseCmd, "auto-indent"))
    {
        // needed
        char *address = malloc(128);
        //
        for (int i = 1; arrOfStr[i] != NULL && arrOfStr[i + 1] != NULL; i += 2)
        {
            char *argKey = arrOfStr[i];
            char *argVal = arrOfStr[i + 1];

            argVal = delQutation(argVal);

            if (revStr(argKey, "-file"))
            {
                strcpy(address, argVal);
            }
        }
        autoIndent(parsePath(address));
    }
    else if (revStr(baseCmd, "compare"))
    {
        char *address1 = malloc(128);
        char *address2 = malloc(128);
        for (int i = 1; i < 3; i++)
        {
            char *argVal = arrOfStr[i];

            if (i == 1)
            {
                strcpy(address1, argVal);
            }
            else
            {
                strcpy(address2, argVal);
            }
        }
        return compare(address1, address2);
    }
    else if (revStr(baseCmd, "tree"))
    {
        int depth = 0;
        sscanf(arrOfStr[1], "%d", &depth);
    }
    else{
        return "invalid base command";
    }
}

char **strExpand(char *stdInLine){
char **listWords;listWords = (char **)malloc(16 * sizeof(char *));
int inputSize = strlen(stdInLine);
char *arg = (char *)malloc(inputSize);
int j = 0, strPos = 0, qutation = 0;
for (int cs = 0; cs < inputSize; cs++){
char currChar = stdInLine[cs];
if (strPos == 0){
    if (currChar == ' '){
        arg[j] = '\0';
        listWords[strPos] = arg;
        arg = (char *)malloc(inputSize);
        strPos++;
        j = 0;
    }
    else{
        arg[j] = currChar;
        j++;
    }
}
else{
    if (currChar == ' ' && (!(qutation % 2))){
        arg[j] = '\0';
        listWords[strPos] = arg;
        arg = (char *)malloc(inputSize);
        strPos++;
        qutation = 0;
        j = 0;
    }
    else{
        if (currChar == '"' && stdInLine[cs - 1] != '\\'){
            arg[j] = '\"';
            qutation++;
        }
        else{
            arg[j] = currChar;
        }
        j++;
    }
}
}
if (strlen(arg) > 0){
arg[j] = '\0';
listWords[strPos] = arg;
arg = (char *)malloc(inputSize);
strPos++;
j = 0;
}
listWords[strPos] = NULL;
free(arg);
return listWords;
}