//deklaracja bibliotek
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>

//deklaracja mojej biblioteki
#include "libr.h"

#define MAX_arg 64
#define max 128

//kolory
#define black "\x1b[30m"
#define red "\x1b[31m"
#define green "\x1b[32m"
#define yellow "\x1b[33m"
#define blue "\x1b[34m"
#define magenta "\x1b[35m"
#define cyan "\x1b[36m"
#define lightGray "\x1b[37m"
#define darkGray "\x1b[90m"
#define lightRed "\x1b[91m"
#define matrix "\x1b[92m"
#define lightYellow "\x1b[93m"
#define lightBlue "\x1b[94m"
#define lightMagenta "\x1b[95m"
#define lightCyan "\x1b[96m"
#define white "\x1b[97m"
#define redB "\x1b[41m"
#define yellowB "\x1b[43m"

//domyślne tło
#define defaultB "\x1b[49m"

//pogrubienie tekstu
#define bold "\x1b[1m"

//default tekst
#define reset "\x1b[0m"

int main()
{
        //zmienna do pętli while
        int a = 1;

        char *input = NULL;

        size_t n = 32;

        char buff[FILENAME_MAX];

        while (a)
        {
                //wyświetlanie loginu aktualnie zalogowanego użytkownika
                wypiszKolorem(getenv("LOGNAME"), matrix);

                //wyświetlanie ścieżki do bieżącego katalogu roboczego
                wypiszKolorem(getcwd(buff, FILENAME_MAX), lightBlue);
                printf(white " $ ");

                //czytanie wejścia z kosoli
                size_t bytes_read = getline(&input, &n, stdin);

                //rozdziela input po spacjach na pojedyńcze segmenty w arg[i]
                char *arg[max + 1] = {strtok(input, " "), 0};
                char *pointer;
                int i = 1;
                while (pointer = strtok(NULL, " "))
                {
                        arg[i] = pointer;
                        i++;
                }

                //usuwanie znaku nowej linii
                if (input[bytes_read - 1] == '\n')
                        input[bytes_read - 1] = 0;

                //help z pogrubieniem i zmianami koloru
                if (strcmp(arg[0], "help") == 0)
                {
                        printf(matrix bold "Autor: \n");
                        printf(lightRed bold "Imię: ");
                        printf(reset);
                        printf(cyan "Mikołaj\n");
                        printf(red bold "Nazwisko: ");
                        printf(reset);
                        printf(lightYellow "Celka\n");
                        printf(red bold "Funkcjonalności:\n");
                        printf(reset);
                        printf(lightMagenta "1: Znak zachęty\n2: Ścieżka do bieżącego katalogu roboczego\n3: Obsługa polecenia cd\n4: Obsługa polecenia exit\n5: Obsługa polecania help\n6: Obsługa dodatkowych poleceń *mkdir, rmdir, clear*\n7: Wypisywanie komunikatu *unknown command* gdy niemożliwym jest poprawne zinte-   rpretowanie polecenia\n8: Bajery:\n8.1 - wyświetlanie loginu aktualnie zalogowanego użytkownika\n8.2 - kolory\n8.3 - pogrubianie czcionki\n8.4 - wyświetlanie komendy o błędzie gdy chcemy:\n8.4.1 - usunąć nieistniejący katalog oraz katalog który ma zawartość\n8.4.2 - utworzyć istniejący katalog\n8.4.3 - przejść do nieistniejącego katalogu");
                        printf(white "\n");
                }

                //cd
                if (strcmp(arg[0], "cd") == 0)
                {
                        int w = chdir(arg[1]);
                        if (w == -1)
                        {
                                wypiszKolorem("Can't change directory. ", red);
                                printf(red "Directory '");
                                printf(matrix "%s", arg[1]);
                                printf(red "' does not exit.\n");
                        }
                }

                //mkdir
                if (strcmp(arg[0], "mkdir") == 0)
                {
                        int w = mkdir(arg[1], 0777);
                        if (w)
                                wypiszKolorem("Can't create directory\n", red);
                }

                //rmdir
                if (strcmp(arg[0], "rmdir") == 0)
                {
                        int w = rmdir(arg[1]);
                        if (w)
                                wypiszKolorem("Can't remove directory\n", red);
                }

                //clear
                if (strcmp(arg[0], "clear") == 0)
                {
                        system("clear");
                }

                //exit
                if (strcmp(arg[0], "exit") == 0)
                {
                        exit(EXIT_SUCCESS);
                }

                //
                if (strcmp(arg[0], "cd") != 0 && (strcmp(arg[0], "help") != 0) && (strcmp(arg[0], "mkdir") != 0) && (strcmp(arg[0], "clear") != 0) && (strcmp(arg[0], "exit") != 0) && (strcmp(arg[0], "rmdir") != 0))
                {
                        int pid = fork();
                        //oczekiwanie na dowolny proces potomny
                        if (pid < 0)
                        {
                                perror("fork");
                                exit(EXIT_FAILURE);
                        }
                        //oczekiwanie na każdy proces potomny którego ID grupy procesu jest równe ID grupy procesu wywołującego funkcję
                        else if (pid == 0)
                        {
                                execvp(arg[0], arg);
                                wypiszKolorem("unknown command ", red);
                                printf("'%s'\n", arg[0]);
                        }
                        //zwalnianie zasobów
                        else
                                waitpid(pid, NULL, 0);
                }
        }
        exit(EXIT_SUCCESS);
        return 0;
}
