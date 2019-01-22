//deklaracja bibliotek
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>

#define MAX_ARGS 64
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
#define defaultB "\x1b[49m"

//pogrubienie tekstu
#define bold "\x1b[1m"

//default tekst
#define reset "\x1b[0m"

//funkcja do zmiany koloru, przyjmuje zmienną oraz kolor
void wypiszKolorem(char *str, char *color)
{
        printf("%s%s", color, str);
        printf(white);
}

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

                //rozdziela input po spacjach na pojedyńcze segmenty w args[i]
                char *args[max + 1] = {strtok(input, " "), 0};
                char *pointer;
                int i = 1;
                while (pointer = strtok(NULL, " "))
                {
                        args[i] = pointer;
                        i++;
                }

                //usuwanie znaku nowej linii
                if (input[bytes_read - 1] == '\n')
                        input[bytes_read - 1] = 0;

                if (strcmp(args[0], "cd") == 0)
                {
                        int w = chdir(args[1]);
                        if (w)
                                wypiszKolorem("Can't change directory\n", red);
                }

                //exec
                /* Jak to działa?
                1. wyłuskuję pierwsza wartość 
                */
                else if (strcmp(args[0], "exec") == 0)
                {
                        char *path;
                        path = (char *)malloc(n * sizeof(char));
                        path = getenv("PATH");
                        char *paths[max + 1] = {strtok(path, ":"), 0};
                        char *wsk;
                        int k = 1;
                        while (wsk = strtok(NULL, ":"))
                        {
                                // printf("%s\n", paths[k]);
                                paths[k] = wsk;
                                k++;
                        }
                        int n = 0;
                        for (int n; n < k; n++)
                        {
                                struct stat file;
                                char *filepath = strcat(paths[n], strcat("/", args[1]));
                                // printf("%s\n", filepath);
                                if (stat(filepath, &file) != -1 && file.st_mode & S_IFREG)
                                {
                                        // printf(filepath);
                                        char *xd[] = {args[2]};
                                        execvp(filepath, xd);
                                        break;
                                }
                        }
                        //wypiszKolorem("Incorrect command\n", red);
                }

                //exit
                else if (strcmp(args[0], "exit") == 0)
                {
                        break;
                }

                //mkdir
                else if (strcmp(args[0], "mkdir") == 0)
                {
                        int w = mkdir(args[1], 0777);
                        if (w)
                                wypiszKolorem("Can't create directory\n", red);
                }

                //rmdir
                else if (strcmp(args[0], "rmdir") == 0)
                {
                        int w = rmdir(args[1]);
                        if (w)
                                wypiszKolorem("Can't remove directory\n", red);
                }

                //clear
                else if (strcmp(args[0], "clear") == 0)
                {
                        system("clear");
                }

                //help z pogrubieniem i zmianami koloru
                else if (strcmp(args[0], "help") == 0)
                {
                        printf(lightRed bold "Imię: ");
                        printf(reset);
                        printf(cyan "Mikołaj\n");
                        printf(red bold "Nazwisko: ");
                        printf(reset);
                        printf(lightYellow "Celka\n");
                        printf(red bold "Funkcjonalności: ");
                        printf(reset);
                        printf(lightMagenta "znak zachęty, cd, exit, help, bład przy niepoprawnych komendach");
                        printf(white "\n");
                }

                //w razie podania nieinstniejącej komendy wyskakuje powiadomiwnie "nieznana komenda"
                else
                {
                        wypiszKolorem("unknown command ", red);
                        printf("'%s'\n", args[0]);
                }
        }
        exit(EXIT_SUCCESS);
        return 0;
}
