#define white "\x1b[97m"

//funkcja do zmiany koloru, przyjmuje zmienną oraz kolor jako argument

void wypiszKolorem(char *str, char *color)
{
        printf("%s%s", color, str);
        printf(white);
}

