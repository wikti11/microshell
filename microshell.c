#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <memory.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <stdio.h>
#include <dirent.h>
#include <malloc.h>
#include <signal.h>
#define LIMIT "\x20\n"
#define DEF_SIZE 2048
#define SIZE 1024
#define HISTORY_MAX_SIZE 100
static const char *history[HISTORY_MAX_SIZE];
int history_count = 0;
int path_count = 0;

size_t file_list(const char *path, char ***ls) {
    size_t count = 0;
    size_t length = 0;
    DIR *dp = NULL;
    struct dirent *ep = NULL;

    dp = opendir(path);
    if(NULL == dp) {
        return 0;
    }

    *ls = NULL;
    ep = readdir(dp);
    while(NULL != ep){
        count++;
        ep = readdir(dp);
    }

    rewinddir(dp);
    *ls = calloc(count, sizeof(char *));

    count = 0;
    ep = readdir(dp);
    while(NULL != ep){
        (*ls)[count++] = strdup(ep->d_name);
        ep = readdir(dp);
    }

    closedir(dp);
    return count;
}

void add_command_to_history( const char *command )
{
    if (history_count < HISTORY_MAX_SIZE) {
        history[history_count++] = strdup( command );
    } else {
        free( history[0] );
        for (unsigned index = 1; index < HISTORY_MAX_SIZE; index++) {
            history[index - 1] = history[index];
        }
        history[HISTORY_MAX_SIZE - 1] = strdup( command );
    }
}

char *removeElemsFromString(char *str, char ch)
{
    char *current = str;
    char *tail = str;

    while(*tail)
    {
        if(*tail == ch)
        {
            tail++;
        }
        else
        {
            *current++ = *tail++;
        }
    }
    *current = 0;
    return str;
}

void help(){
    char command[5];
    printf("\n");
    printf("Autor: Wiktor Leszczynski\n\n");
    printf("Powloka obsluguje nastepujace polecenia: \n");
    printf("- help \n- exit \n- cd \n- cp \n- mv \n- echo\n- clear\n- mkdir\n\n");
    printf("Aby dowiedziec sie wiecej o danym poleceniu wpisz jego nazwe lub");
    printf("\033[0;32m");
    printf(" quit ");
    printf("\033[0m");
    printf("aby wyjsc: ");
    scanf("%s",command);
    printf("\n");

    removeElemsFromString(command, '"');

    if (strcmp(command,"help")==0){
        printf("- wyswietla imie i nazwisko autora powloki\n");
        printf("- informuje o dzialaniu i obsludze polecen\n\n");
    }
    else if (strcmp(command,"exit")==0){
        printf("- zamyka powloke\n\n");
    }
    else if (strcmp(command,"cd")==0){
        printf("- umozliwa przemieszczanie sie pomiedzy katalogami\n");
        printf("- 'cd .' wyswietla obecny katalog\n");
        printf("- 'cd ~' wyswietla katalog domowy\n");
        printf("- 'cd ..' wyswietla katalog nadrzedny\n");
        printf("- 'cd sciezka' wyswietla katalog do ktorego odwoluje sie sciezka\n\n");
    }
    else if (strcmp(command,"cp")==0){
        printf("- 'cp plik_oryginalny plik_skopiowany' kopiuje plik w obecnej lokalizacji\n");
        printf("- 'cp plik_oryginalny /przykladowa/sciezka/pliku.txt' kopiuje plik do wybranej lokalizacji\n");
        printf("- 'cp /sciezka1/plik.txt /sciezka2/plik_skopiowany.txt' kopiuje plik z dowolnej lokalizacji do innej\n\n");
    }
    else if (strcmp(command,"mv")==0){
        printf("- 'mv plik_oryginalny /przykladowa/sciezka/pliku.txt' przenosi plik z obecnej lokalizacji do wybranej\n\n");
    }
    else if (strcmp(command,"echo")==0){
        printf("- wypisuje w konsoli tekst wpisany przez uzytkownika\n\n");
    }
    else if (strcmp(command,"clear")==0){
        printf("- czysci wiersz polecen\n\n");
    }
    else if (strcmp(command,"mkdir")==0){
        printf("- 'mkdir nazwa_katalogu' pozwala na stworzenie katalogu w obecnej lokalizacji\n\n");
    }
    else if (strcmp(command,"quit")==0){

    }
    else{
        printf("\033[0;31m");
        printf("Bledna komenda\n\n");
        printf("\033[0m");
    }
    add_command_to_history(command);
}

void change_dir(){
    char changing_to[DEF_SIZE];
    scanf("%s",changing_to);
    removeElemsFromString(changing_to, '"');

    add_command_to_history(changing_to);
    history_count--;
    history[history_count-1] = strcat(history[history_count-1], " ");
    history[history_count-1] = strcat(history[history_count-1], history[history_count]);

    if (strcmp(changing_to,"~")==0){
        chdir(getenv("HOME"));
    }

    else{
        if (strstr(changing_to, "~")){
            char sub[SIZE];
            int c = 0;
            while(c < strlen(changing_to)){
                sub[c] = changing_to[c+1];
                c++;
            }
            if (chdir(strcat(getenv("HOME"), sub))){
                perror("cd\n");
            }
        }
        else {
            if (chdir(changing_to)) {
                perror("cd\n");
            }
        }
    }

}

void cdWithArgs(char changing_to[DEF_SIZE]){
    if (strcmp(changing_to,"~")==0){
        chdir(getenv("HOME"));
    }

    else{
        if (strstr(changing_to, "~")){
            char sub[SIZE];
            int c = 0;
            while(c < strlen(changing_to)){
                sub[c] = changing_to[c+1];
                c++;
            }
            if (chdir(strcat(getenv("HOME"), sub))){
                perror("cd\n");
            }
        }
        else {
            if (chdir(changing_to)) {
                perror("cd\n");
            }
        }
    }

}

void echo(){
    char echoing[DEF_SIZE];
    scanf("%s",echoing);
    removeElemsFromString(echoing, '"');
    printf("%s\n",echoing);

    add_command_to_history(echoing);
    history_count--;
    history[history_count-1] = strcat(history[history_count-1], " ");
    history[history_count-1] = strcat(history[history_count-1], history[history_count]);
}

void error(){
    printf("\033[0;31m");
    printf("Bledna komenda. ");
    printf("\033[0m");
    printf("Zapoznaj sie ze wszystkimi dostepnymi komendami wpisujac ");
    printf("\033[0;32m");
    printf("'help'\n");
    printf("\033[0m");
}

void mk_dir(){
    char name[SIZE];
    int new_file;
    scanf("%s",name);
    removeElemsFromString(name, '"');
    new_file=mkdir(name,0777);
    if(!new_file){
        printf("\033[0;32m");
        printf("Katalog utworzony pomyslnie.\n ");
        printf("\033[0m");
    }
    else{
        printf("\033[0;31m");
        printf("Blad. Nie mozna utworzyc katalogu. \n");
        printf("\033[0m");
    }
    add_command_to_history(name);
    history_count--;
    history[history_count-1] = strcat(history[history_count-1], " ");
    history[history_count-1] = strcat(history[history_count-1], history[history_count]);
}

void cp() {
    char check;
    char original_file[DEF_SIZE];
    char copy_file[DEF_SIZE];
    FILE *input, *output;
    scanf("%s %s", original_file, copy_file);
    removeElemsFromString(original_file, '"');
    removeElemsFromString(copy_file, '"');

    if(access(input, F_OK) == 0){
        input = fopen(original_file, "r");
        output = fopen(copy_file, "w");
        if (input == NULL || output == NULL) {
            fclose(input);
            printf("Blad. Nie mozna skopiowac pliku. \033[0m \n");
        } else {
            while ((check = fgetc(input)) != EOF) {
                fputc(check, output);
            }
            printf("\033[0;32m");
            printf("Plik skopiowany pomyslnie.\n ");
            printf("\033[0m");
            fclose(input);
            fclose(output);
        }
    }
    else{
        printf("\033[0;31m");
        printf("Blad. Nie mozna przeniesc pliku. \n");
        printf("\033[0m");
    }
    add_command_to_history(original_file);
    add_command_to_history(copy_file);

    history_count--;
    history[history_count-1] = strcat(history[history_count-1], " ");
    history[history_count-1] = strcat(history[history_count-1], history[history_count]);

    history_count--;
    history[history_count-1] = strcat(history[history_count-1], " ");
    history[history_count-1] = strcat(history[history_count-1], history[history_count]);
}

void mv(){
    char check;
    char source[DEF_SIZE];
    char destination[DEF_SIZE];
    FILE *input, *output;
    scanf("%s %s",source,destination);
    removeElemsFromString(source, '"');
    removeElemsFromString(destination, '"');

    if(access(input, F_OK) == 0){
        input=fopen(source,"r");
        output=fopen(destination,"w");

        if(input==NULL||output==NULL){
            fclose(input);
            printf("\033[0;31m");
            printf("Blad. Nie mozna przeniesc pliku. \n");
            printf("\033[0m");
        }
        else{
            while((check=fgetc(input))!=EOF){
                fputc(check,output);
            }
        }
        fclose(input);
        fclose(output);
        int deleted=remove(source);
        if(deleted==0){
            printf("\033[0;32m");
            printf("Plik przeniesiony pomyslnie.\n ");
            printf("\033[0m");
        }
        else{
            printf("\033[0;31m");
            printf("Blad. Nie mozna usunac pliku zrodlowego. \n");
            printf("\033[0m");
        }
    }

    else{
        printf("\033[0;31m");
        printf("Blad. Nie mozna przeniesc pliku. \n");
        printf("\033[0m");
    }
    add_command_to_history(source);
    add_command_to_history(destination);
    history_count--;
    history[history_count-1] = strcat(history[history_count-1], " ");
    history[history_count-1] = strcat(history[history_count-1], history[history_count]);

    history_count--;
    history[history_count-1] = strcat(history[history_count-1], " ");
    history[history_count-1] = strcat(history[history_count-1], history[history_count]);

}

void prompt(){
    char *name=getenv("USER");
    char cwd[DEF_SIZE];
    printf("\033[0;36m");
    printf("@%s: ",name);
    printf("\033[0m");
    printf("\033[0;35m");
    printf("[%s]\n",getcwd(cwd,DEF_SIZE));
    printf("\033[0m");
    printf("$ ");
}

int path_splitter(char input[SIZE]){
    int isWorking = 0;
    char *paths[HISTORY_MAX_SIZE];
    char *path=getenv("PATH");
    char *path_copied=(char*)malloc(strlen(path)+1);

    if(path_copied!=NULL){
        strcpy(path_copied,path);
        paths[path_count]=strtok(path_copied,":");
        path_count++;
        while(paths[path_count] != NULL){
            paths[++path_count] = strtok(NULL, ":");
        }

        free(path_copied);
        path_copied=NULL;

        char **files;
        size_t count;
        int i;

        for (int j = 0; j < path_count; j++){
            count = file_list(paths[j], &files);
        }

        for (i = 0; i < count; i++) {
            if(strcmp(files[i], input)) {
                isWorking = 1;
                break;
            }
        }
    }
    return isWorking;
}

void someExecv(char* wordinput, char* argument){

    int status;
    pid_t pid;

    if((pid=fork())<0){
        printf("\033[0;31m");
        printf("Blad. Forkowanie nieudane.\n");
        printf("\033[0m");
        exit(1);
    }
    else if(pid==0){

        char* argument_list[] = {wordinput, argument, NULL};

        int status_code = execvp(wordinput, argument_list);

        if (status_code == -1) {
            error();

        }
    }
    else{
        while(wait(&status)!=pid);
    }


}



int main(){
    char wordinput[SIZE];
    signal(SIGINT,SIG_IGN);


    while (strcmp(wordinput,"exit")!=0){

        prompt();
        scanf("%s",wordinput);
        removeElemsFromString(wordinput, '"');
        add_command_to_history(wordinput);


        if (strcmp(wordinput,"help")==0){
            help();
        }
        else if(strcmp(wordinput,"cd")==0){
            change_dir();
        }
        else if(strcmp(wordinput,"echo")==0){
            echo();
        }
        else if(strcmp(wordinput,"clear")==0){
            system("clear");
        }
        else if(strcmp(wordinput,"mkdir")==0){
            mk_dir();
        }
        else if(strcmp(wordinput,"cp")==0){
            cp();
        }
        else if(strcmp(wordinput,"mv")==0){
            mv();
        }
        else if(strcmp(wordinput, "exit") == 0){
            printf("Exiting...\n");
        }
        else if(strcmp(wordinput, "\33")){
            if(wordinput[2] == 'A'){
                if (strlen(wordinput) % 4 == 0);{
                    int posBasedOnArrowPress = 0;
                    posBasedOnArrowPress = strlen(wordinput) / 3 - 1;

                    if (posBasedOnArrowPress >= 0 && posBasedOnArrowPress <= history_count) {

                        printf("Wykonywanie: %s\n\n", history[posBasedOnArrowPress]);
                        char *token = strtok(history[posBasedOnArrowPress], " ");
                        char *temp = NULL;
                        temp = strdup(token);
                        token = strtok(NULL, " ");

                        if (strcmp(temp, "help") == 0) {
                            help();
                        }
                        else if (strcmp(temp, "clear") == 0) {
                            system("clear");
                        }
                        else if (strcmp(temp, "cd") == 0) {
                            cdWithArgs(token);
                        }
                        else if (path_splitter(temp) != 1) {
                            someExecv(temp, token);
                        } else {

                            error();
                        }
                        free(temp);

                    }
                    else{
                        error();
                    }
                }
            }
            else {
                if (path_splitter(wordinput) != 1){
                    char argument[SIZE];
                    scanf("%s", argument);
                    add_command_to_history(argument);
                    history_count--;
                    history[history_count-1] = strcat(history[history_count-1], " ");
                    history[history_count-1] = strcat(history[history_count-1], history[history_count]);
                    someExecv(wordinput, argument);
                }
            }
        }
        else {
            error();
        }

    }

}
