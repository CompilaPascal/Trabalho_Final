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
    return(c == ' '  || c == '.' || c == ';' 
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

tokens *count;

void analisadorLexico()
{
    char prox[2];

    prox[1] = '\0';
    count = calloc(1, sizeof(tokens));

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
                    count->compoundOperator ++;
                    //printf("COMPOUND OPERATOR: [%s]\n", s);
                    continue;
                }
            }
            else if (s[0] == '(') {
                *prox = proximo();
                if (*prox == '*') {
                    s = strcat(s, prox);
                    consumiu();
                    consomeComentario();
                    //printf("COMENTARIO\n");
                    count->comments++;
                    continue;
                }
            }

            count->delimiter++;
            //printf("DELIMITER [%s]\n", s);        
            s[0] = '\0';
        }
        else if (operador(*prox)) {
            consumiu();
            s = strcat(s, prox);

            if (s[0] == '<' || s[0] == '>') {
                *prox = proximo();
                if (*prox == '=') {
                    s = strcat(s, prox);
                    consumiu();
                    count->compoundOperator++;
                    //printf("COMPOUND OPERATOR: [%s]\n", s);
                    continue;
                } 
            }

            if (s[0] == '<') {
                *prox = proximo();
                if (*prox == '>') {
                    s = strcat(s, prox);
                    consumiu();
                    count->compoundOperator++;
                    //printf("COMPOUND OPERATOR: [%s]\n", s);
                    continue;
                } 
            }

            //printf("OPERATOR [%s]\n", s);
            count->operator++;

        }
        else if (isalpha(*prox)) {
            do {
                consumiu();
                s = strcat(s, prox);
                *prox = proximo();
            } while(isalpha(*prox) || isdigit(*prox));

            if (palavraReservada(s)) {
                count->keyword++;
               // printf("KEYWORD [%s]\n", s);
            }
            else {
                count->identifier++;
                //printf("IDENTIFIER: [%s]\n", s);
            }

            s[0] = '\0';
        }
        else if (isdigit(*prox)) {
            do {
                consumiu();
                s = strcat(s, prox);
                *prox = proximo();
            } while(isdigit(*prox));

            if (isalpha(*prox)) {
                count->unknown++;
                //printf("primeiro UKNOWN\n");
                continue;
                //break;
            }
            
            count->number++;
            //printf("NUMBER: [%s]\n", s);
            s[0] = '\0';
        }
        else {
            //printf("UNKNOWN [%s]\n", s);
            count->unknown++;
            consumiu();
            //break;
        }
    }
}

int main()
{
    analisadorLexico();

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
