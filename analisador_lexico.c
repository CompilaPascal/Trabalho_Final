#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

bool delimitador(char c)
{
    return(c == ' '  || c == '.' || c == ';' || c == '('
    || c == ')' || c == '[' || c == ']' 
     || c == ':' || c ==',' );
}

bool operador (char c){
    return (c == '+' || c == '-' || c == '*' || c == '/' 
    || c == '>' || c == '>' || c == '=');
}

bool letras(char c)
{
    return (c >= 65 && c <= 90) || (c >= 97 && c <= 122);
}

bool numeros(char c)
{
    return (c >= 48 && c <= 57);
}

bool especiaisCompostos (char *c)
{
    return !strcmp(c, ":=") || !strcmp(c, "<=") || !strcmp(c, ">=") || !strcmp(c, "<>");
}

int compara(const void *a, const void *b)
{
    return strcmp(*(char **) a, *(char **) b);
}

bool palavraReservada(char *c){
    char **str;
    int tam;
    

    const char *palavras[] =
    {
        "and", "array", "begin", "div", "do", "else", "end", "function", "goto", "if", 
        "label", "not", "of", "or", "procedure", "program", "then", "type", "var", "while", "read", "write"
    };

    tam = sizeof(palavras) / sizeof(palavras[0]);
    str = bsearch(&c, palavras, tam, sizeof(char *), compara);

    if (str)
        return true;
    
    return false;
}
char buf = '\0';

void consumiu()
{
    buf = '\0';
}

char proximo()
{
    if (buf != '\0') {
        char temp;
        temp = buf;
        buf = '\0';
        return temp;
    }

    buf = getchar();
    
    return buf;
}

void consomeComentario()
{
    char pas, prox;

    pas = proximo();
    consumiu();
    prox = proximo();
    consumiu();

    while (1) {
        if (pas == '*' && prox == ')')
            break;
        pas = prox;
        prox = proximo();
        consumiu();
    }
}

void analisadorLexico()
{
    char prox[2];
    prox[1] = '\0';
    while (*prox = proximo(), *prox != EOF) {
        char *s;

        s = malloc(sizeof(char) * 102);
        s[0] = '\0';
        if (*prox == ' ' || *prox == '\n') {
            consumiu();
            continue;
        }

        if (delimitador(*prox)) {
            consumiu();
            s = strcat(s, prox);
        
            if (s[0] == ':') {
                *prox = proximo();
                if (*prox == '=') {
                    s = strcat(s, prox);
                    consumiu();
                    printf("Token delimitador composto: [%s]\n", s);

                    continue;
                }
            }
            else if (s[0] == '.') {
                *prox = proximo();
                if (*prox == '.') {
                    s = strcat(s, prox);
                    consumiu();
                    printf("Token delimitador composto: [%s]\n", s);
                    continue;
                }
            }
            else if (s[0] == '(') {
                *prox = proximo();
                if (*prox == '*') {
                    s = strcat(s, prox);
                    consumiu();
                    consomeComentario();
                    printf("Comentario\n");
                    continue;
                }
            }

            printf("Token delimitador: [%s]\n", s);
        
            s[0] = '\0';
        }
        else if (letras(*prox)) {
            do {
                consumiu();
                s = strcat(s, prox);
                *prox = proximo();
            } while(letras(*prox) || numeros(*prox));

            if (palavraReservada(s))
                printf("Token palavra reservada: [%s]\n", s);
            else
                printf("Token palavra: [%s]\n", s);

            s[0] = '\0';
        }
        else if (numeros(*prox)) {
            do {
                consumiu();
                s = strcat(s, prox);
                *prox = proximo();
            } while(numeros(*prox));

            if (letras(*prox)) {
                printf("erro\n");
                break;
            }
            
            printf("Token numero: [%s]\n", s);
            s[0] = '\0';
        }
        else {
            printf("erro\n");
            break;
        }
    }
}

int main()
{
    analisadorLexico();
    return 0;
}
