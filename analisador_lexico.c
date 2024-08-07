#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

typedef struct {
    int keyword, operator, compoundOperator;
    int identifier, number, delimiter, comments, unknown;
}tokens;

bool delimitador(char c)
{
    return(c == '.' || c == ';' 
    || c == '(' || c == ')' || c == '[' 
    || c == ']' || c == ':' || c ==',' );
}

bool operador (char c){
    return (c == '+' || c == '-' || c == '*' || c == '/' 
    || c == '<' || c == '>' || c == '=');
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
        "label", "not", "of", "or", "procedure", "program","read", "then", "type", "var", "while", "write"
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

char proximo(FILE *file)
{
    if (buf != '\0')
        return buf;

    buf = fgetc(file);
    
    return buf;
}

void consomeComentario(FILE *file)
{
    char pas, prox;

    pas = proximo(file);
    consumiu();
    prox = proximo(file);
    consumiu();

    while (1) {
        if (pas == '*' && prox == ')')
            break;
        pas = prox;
        prox = proximo(file);
        consumiu();
    }
}


tokens *count;

void analisadorLexico(FILE *file)
{
    tokens past;
    char prox[2];

    prox[1] = '\0';
    count = calloc(1, sizeof(tokens));

    char *s;
    s = malloc(sizeof(char) * 1000);
    while (*prox = proximo(file), *prox != EOF) {

        s[0] = '\0';

        if (isspace(*prox)) {
            consumiu();
            continue;
        }

        if (delimitador(*prox)) {
            consumiu();
            s = strcat(s, prox);
        
            if (s[0] == ':') {
                *prox = proximo(file);
                if (*prox == '=') {
                    s = strcat(s, prox);
                    consumiu();
                    count->compoundOperator++;
                    atual.val = strdup(s);
                    atual.tipoToken = compoundOperator;
                    return;
                }
            }
            else if (s[0] == '(') {
                *prox = proximo(file);
                if (*prox == '*') {
                    consumiu();
                    consomeComentario(file);
                    count->comments++;
                    continue;
                }
            }

            atual.val = strdup(s);
            atual.tipoToken = delimiter;
            count->delimiter++;
            return;
        }
        else if (operador(*prox)) {
            consumiu();
            s = strcat(s, prox);

            if (s[0] == '>') {
                *prox = proximo(file);
                if (*prox == '=') {
                    s = strcat(s, prox);
                    consumiu();
                    atual.val = strdup(s);
                    atual.tipoToken = compoundOperator;
                    count->compoundOperator++;
                    return;
                } 
            }
            else if (s[0] == '<') {
                *prox = proximo(file);
                if (*prox == '>' || *prox == '=') {
                    s = strcat(s, prox);
                    consumiu();
                    count->compoundOperator++;
                    atual.val = strdup(s);
                    atual.tipoToken = compoundOperator;
                    return;
                } 
            }

            atual.val = strdup(s);
            atual.tipoToken = operator;
            count->operator++;
            return;
        }
        else if (isalpha(*prox)) {
            do {
                consumiu();
                s = strcat(s, prox);
                *prox = proximo(file);
            } while(isalnum(*prox));

            if (palavraReservada(s)) {
                atual.val = strdup(s);
                atual.tipoToken = keyword;
                count->keyword++;
                return;
            }
            else {
                atual.val = strdup(s);
                atual.tipoToken = identifier;
                count->identifier++;
                return;
            }
        }
        else if (isdigit(*prox)) {
            do {
                consumiu();
                s = strcat(s, prox);
                *prox = proximo(file);
            } while(isdigit(*prox));

            if (*prox == '.') {
                char dot[2];

                dot[0] = *prox;
                dot[1] = '\0';
                consumiu();

                *prox = proximo(file);
                if (isdigit(*prox)) {
                    s = strcat(s, dot);
                    do {
                        consumiu();
                        s = strcat(s, prox);
                        *prox = proximo(file);
                    } while(isdigit(*prox));
                }
                else {
                    atual.val = strdup(s);
                    atual.tipoToken = delimiter;
                    count->delimiter++;
                    return;
                }
            }
            atual.val = strdup(s);
            atual.tipoToken = number;
            count->number++;
            return;
        }
        else {
            atual.val = strdup(s);
            atual.tipoToken = unknown;
            count->unknown++;
            consumiu();
            return;
        }
    }
}

int main(int argc, char *argv[])
{
    if (argc != 2) {
        printf("Use %s <file-name>\n", argv[0]);
        return 1;
    }

    FILE *file = fopen(argv[1], "r");

    analisadorLexico(file);

    fclose(file);

    printf("KEYWORD: %d\n", count->keyword);
    printf("IDENTIFIER: %d\n", count->identifier);
    printf("NUMBER: %d\n", count->number);
    printf("OPERATOR: %d\n", count->operator);
    printf("COMPOUND OPERATOR: %d\n", count->compoundOperator);
    printf("DELIMITER: %d\n", count->delimiter);
    printf("COMMENTS: %d\n", count->comments);
    printf("UNKNOWN: %d\n", count->unknown);

    return 0;
}
