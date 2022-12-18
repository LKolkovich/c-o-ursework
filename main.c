#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <locale.h>
#define BUF_SIZE 50

int getText(char ***text);
int getSentence(char **sentence);
void freeText(char **text, int n);
int firstQuest(char ***text, int sentenceCount);
int deleteStartAndEnd(char *sentence);
int thirdQuest(char ***text, int senteceCount);
int checkSentencePalindrom(char *sentence);
int isPalindrom(char *word);
int findHiddenAgent(char *sentence);
void fourthQuest(char ***text, int sentenceCount);
int wordSort(char *sentence);
int wordCmp(const void *word1, const void *word2);
int putArrayToString(char *sentence, char** wordArr, int wordCount);
int printText(char*** text, int sentenceCount);
int removeDuplicates(char ***text, int sentenceCount);

int main() {
    setlocale(LC_ALL, "RUS");
    wprintf(L"Введите текст в одну строку:\n");
    char ***text = malloc(sizeof(char*));
    int sentenceCount = getText(text);
    int typeOfProgram;
    sentenceCount = removeDuplicates(text, sentenceCount);
    wprintf(L"Повторяющиеся предложения удалены:\n");
    printText(text, sentenceCount);
    while(typeOfProgram != 5) {
        wprintf(L"Выберите действие:\n");
        wprintf(L"1 - Удалить все символы в начале и конце строки так, чтобы в итоге первый и последний символ были различными (без учета регистра)\n");
        wprintf(L"2 - Отсортировать все слова в выбранном предложении в лексикографическом порядке\n");
        wprintf(L"3 - Удалить все предложения, в которых хотя бы одного слово является палиндромом\n");
        wprintf(L"4 - Вывести все предложения в которых есть слово “HiddenAgent” и которое не является первым словом\n");
        wprintf(L"5 - Закрыть программу\n");
        int sentenceNum = 0;
        scanf("%d", &typeOfProgram);
        switch (typeOfProgram) {
            case 1:
                sentenceCount = firstQuest(text, sentenceCount);
                printText(text, sentenceCount);
                break;
                // если предложение полностью симметрично, то оно удаляется полностью
                // 1 задание - пробежаться по всем предложениям. внутри предложения сравнивать первый и последний символ, а затем
                // с помощью memmove затирать сначала первый, а потом последний символ
                // вроде работает, остаётся тестить
            case 2:
                wprintf(L"Выберите предложение для сортировки слов\n");
                scanf("%d", &sentenceNum);
                if (sentenceNum > sentenceCount) {
                    wprintf(L"Выбран номер несущесвующего предложения");
                    break;
                }
                wordSort((*text)[sentenceNum - 1]);
                puts((*text)[sentenceNum - 1]);
                break;
                //printf("%s\n", (*text)[sentenceNum - 1]);
                // 2 задание - qsort
                // скопировать строку и создать массив указателей на начала слов, используя strtok
                // qsort(array, array_len, sizeof(element), strcmp)
                // посимвольно переписать слова в строку, поставив пробелы между ними
            case 3:
                sentenceCount = thirdQuest(text, sentenceCount);
                printText(text, sentenceCount);
                break;
                // слово из одного символа - тоже палиндром
                // 3 задание - пробежаться по предожениям. внутри предложения strtok берёт слово и меняет проверяет на палиндром,
                // функция возвращает логическую величину и предложение либо удаляется либо остаётся
            case 4:
                fourthQuest(text, sentenceCount);
                //вывод внутри функции
                break;
                // 4 задание - пробегаемся по предложениям, с помощью strtok получаем слова, отслеживаем их нумирацию, если слово подходящее
                // и не на первом месте выводим предложение
            case 5:
                wprintf(L"Работа программы завершена\n");
                break;
            default:
                wprintf(L"Введено неверное значение\n");
        }
    }
    freeText(*text, sentenceCount);
    //удалить следующие 2 строки
    printf("Type any simbols to close the app\n");
    scanf("%d\n", typeOfProgram);
    //да, вот эти
    return 0;
}

int getText(char ***text){
    int size = BUF_SIZE;
    *text = (char**)malloc(sizeof(char*) * size);
    int sentenceCount = 0;
    char *tmp_str;
    int strLen = 0;
    do {
        char **tmp;
        if(sentenceCount == size - 2){
            size += BUF_SIZE;
            tmp = (char**) realloc(*text, size * sizeof(char*));
            if(tmp == NULL){
                wprintf(L"Ошибка выделения памяти для предложения\n");
                freeText(*text, sentenceCount);
                return -1;
            }
            *text = tmp;
        }
        strLen = getSentence(&tmp_str);
        if(strLen == -1){
            freeText(*text, sentenceCount);
            return strLen;
        }
        (*text)[sentenceCount++] = tmp_str;
    }while(strLen != -2);
    (*text)[sentenceCount] = "\0";
    return sentenceCount; // в примере был отдельный \n для обозначения коцна ввода, т.е. в тексте могло быть \n
    // но в задании про это не написано, поэтому у меня его нет
}

int getSentence(char **sentence){
    int size = BUF_SIZE;
    int n = 0;
    *sentence = (char*) malloc(sizeof(char) * size);
    char ch;
    int lengthOfSentence = 0;
    do {
        char *tmp;
        if(lengthOfSentence == size - 2){ // чтобы всегда можно было записать \n
            size += BUF_SIZE;
            tmp = (char*) realloc(*sentence, size * sizeof(char));
            if(tmp == NULL){
                wprintf(L"Ошибка выделения памяти\n");
                free(*sentence);
                return -1;
            }
            *sentence = tmp;
        }
        ch = (char)getchar();
        if(ch == '\n'){
            (*sentence)[lengthOfSentence] = '\0';
            if((*sentence)[0] == ' ') {
                memmove((*sentence), (*sentence) + sizeof(char), lengthOfSentence);
            }
            return -2;
        }
        (*sentence)[lengthOfSentence++] = ch;
    }while((*sentence)[lengthOfSentence - 1] != '.'); // \n - конец ввода
    (*sentence)[lengthOfSentence - 1] = '\0';
    if((*sentence)[0] == ' ') {
        lengthOfSentence--;
        memmove((*sentence), (*sentence) + sizeof(char), lengthOfSentence);
    }
    lengthOfSentence -= 1;
    return lengthOfSentence;
}


void freeText(char **text, int n){
    for(int i = 0; i < n; i++){
        free(text[i]);
    }
    free(text);
}

int firstQuest(char ***text, int sentenceCount){
    int response;
    int newCount = sentenceCount;
    for(int i = 0; i < sentenceCount; i++){
        response = deleteStartAndEnd((*text)[i]);
        if(response == -1){
            memmove(*text + i, *text + i + 1, (newCount - i - 1) * sizeof(char*));
            newCount--;
        }
    }
    return newCount;
}

int deleteStartAndEnd(char *sentence){
    long len = strlen(sentence);
    while(tolower((sentence)[0]) == tolower((sentence)[len - 1]) && (len > 0)) {
        memmove(sentence, sentence + sizeof(char), len);
        len--;
        memmove(sentence + sizeof(char) * (len - 1), sentence + sizeof(char) * len, 2);
        len--;
    }
    if(len <= 0){
        free(sentence);
        return -1;
    }
    return 0;
}

int thirdQuest(char ***text, int sentenceCount){
    int newCount = sentenceCount;
    for(int i = 0; i < newCount;){
        if(0 == (checkSentencePalindrom((*text)[i]))){
            i++;
        }
        else{
            free((*text)[i]);
            memmove(*text + i, *text + i + 1, (newCount - i - 1) * sizeof(char*));
            newCount--;
        }
    }
    return newCount;
}

int checkSentencePalindrom(char *sentence){
    int len = strlen(sentence);
    char *newSentence = malloc(sizeof(char) * (len + 1));
    memcpy(newSentence, sentence, len + 1);
    char *word;
    word = strtok(newSentence, ", .\0");
    while(word != NULL){
        if(isPalindrom(word)) return 1;
        word = strtok(NULL, ", .\0");
    }
    free(newSentence);
    return 0;
}

int isPalindrom(char *word){
    int len = strlen(word);
    for(int i = 0; i < len / 2 + 1; i++){
        if(!((word[i] == word[len - i - 1]) || (i == len - i - 1))) return 0;
    }
    return 1;
}

void fourthQuest(char ***text, int sentenceCount){
    for(int i = 0; i < sentenceCount; i++){
        if(findHiddenAgent((*text)[i]) == 1){
            puts((*text)[i]);
        }
    }
}
// qwvccv dsafewrf. kgfhjdgfgyvt jsadjsh.

int findHiddenAgent(char *sentence){
    int len = strlen(sentence);
    int currentNum = 0;

    char *newSentence = malloc(sizeof(char) * (len + 1));
    memcpy(newSentence, sentence, len + 1); // точно ли len + 1
    char *word = strtok(newSentence, ", \0\n");
    while(word != NULL){
        currentNum++;
        if((strcmp(word, "HiddenAgent") == 0) && (currentNum != 1)){
            return 1;
        }
        word = strtok(NULL, ", \0\n");
    }
    return 0;
}

int wordSort(char *sentence){
    int size = BUF_SIZE;
    int len = strlen(sentence);
    char *sentenceCopy = malloc(sizeof(char) * len);
    strcpy(sentenceCopy, sentence);
    char** wordArr = malloc(sizeof(char*) * size);
    char* word = strtok(sentenceCopy, ", \n");
    int wordCount = 0;
    while(word != NULL){ // подумать о том, при каком количестве слов надо увеличивать объём памяти
        if(wordCount == size){
            size += BUF_SIZE;
            char* tmp = (char*) realloc(wordArr, size * sizeof(char*));
            if(tmp == NULL){
                wprintf(L"Ошибка выделения памяти для массива слов\n");
                return -1;
            }
            wordArr = &tmp;
        }
        wordArr[wordCount] = word;
        wordCount++;
        word = strtok(NULL, ", \n");
    }

    qsort(wordArr, wordCount, sizeof(char*), wordCmp); // заменить компаратор

    putArrayToString(sentence, wordArr, wordCount);
    return 0;
}

int putArrayToString(char *sentence, char** wordArr, int wordCount){
    int indexSentence = 0;

    for(int i = 0; i < wordCount; i++){
        char* word = wordArr[i];
        int lenWord = strlen(word);
        for(int indexLetter = 0; indexLetter < lenWord; indexLetter++){
            sentence[indexSentence++] = word[indexLetter];
        }
        sentence[indexSentence++] = ' ';
        if(i == wordCount - 1){
            sentence[indexSentence - 1] = '\0';
        }
    }
    return 0;
}


int wordCmp(const void *a, const void *b){
    const char *word1 = *((char**) a);
    const char *word2 = *((char**) b);
    int i = 0;
    int len1 = strlen(word1);
    int len2 = strlen(word2);
    while (tolower(*(word1 + i)) == tolower(*(word2 + i))){
        if(i == len1 - 1 && len1 == len2){
            return 0;
        }
        if(i == len1 - 1){
            return 1;
        }
        if(i == len2 - 1){
            return -1;
        }
        i++;
    }
    return (tolower(word1[i]) - tolower(word2[i]));
}

int printText(char*** text, int sentenceCount){
    if(sentenceCount == 0){
        return -1;
    }
    for(int i = 0; i < sentenceCount; i++){
        printf("%s", (*text)[i]);
        if(i != sentenceCount - 1) printf(". ");
    }
    printf("\n");
    return 0;
}

int removeDuplicates(char ***text, int sentenceCount){
    for(int i1 = 0; i1 < sentenceCount - 1; i1++){
        for(int i2 = i1 + 1; i2 < sentenceCount; i2++){
            if(!strcasecmp((*text)[i1],(*text)[i2])){
                free((*text)[i2]);
                memmove(*text + i2, *text + i2 + 1, (sentenceCount - i2 - 1) * sizeof(char*));
                sentenceCount--;
                i2--;
            }
        }
    }
    return sentenceCount;
}