#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

typedef struct {
    int keyword, operator, compoundOperator;
    int identifier, number, delimiter, comments, unknown;
}tokens;

typedef enum {keyword, operator, compoundOperator,
    identifier, number, delimiter,
    comments, unknown} tipoToken;
    
typedef struct {
    char *val;
    int tipoToken;
} token;

token atual;

// Cabecalho das funcoes aqui
void programa(FILE *file);
void bloco(FILE *file);
void declaracaoRotulo(FILE *file);
void definicaoTipo(FILE *file);
void parteDeclaracaoVariaveis(FILE *file);
void declaracaoVariaveis (FILE *file);
void listaIdentificadores(FILE *file);
void comandoComposto (FILE *file);
void comando (FILE *file);
void comandoSemRotulo (FILE *file);
void atribuicao(FILE *file);
void desvios(FILE *file);
void comandoCondicional(FILE *file);
void comandoRepetitivo(FILE *file);
void listaExpressoes(FILE *file);
void relacao(FILE *file);
void expressao(FILE *file);
void expressaoSimples(FILE *file);
void termo(FILE *file);
void fator(FILE *file);
void variavel(FILE *file);
void numero(FILE *file);
void identificador(FILE *file);
void read(FILE *file);
void write(FILE *file);
// E termina aqui

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
                    //printf("%s\n", s);
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
            //printf("%s\n", s);
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
                    //printf("%s\n", s);
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
                    //printf("%s\n", s);
                    return;
                } 
            }

            atual.val = strdup(s);
            atual.tipoToken = operator;
            count->operator++;
            //printf("%s\n", s);
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
                //printf("%s\n", s);
                return;
            }
            else {
                atual.val = strdup(s);
                atual.tipoToken = identifier;
                count->identifier++;
                //printf("%s\n", s);
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
                    //printf("%s\n", s);
                    return;
                }
            }
            atual.val = strdup(s);
            atual.tipoToken = number;
            count->number++;
            //printf("%s\n", s);
            return;
        }
        else {
            atual.val = strdup(s);
            atual.tipoToken = unknown;
            count->unknown++;
            consumiu();
            //printf("%s\n", s);
            return;
        }
    }
}


void programa(FILE *file)
{
    if (strcmp(atual.val, "program") == 0) {
        analisadorLexico(file);
        identificador(file);

        if (!strcmp(atual.val, "(")) {
            analisadorLexico(file);
            listaIdentificadores(file);

            if (!strcmp(atual.val, ")")) {
                analisadorLexico(file);
            }
            else {
                printf("Rejeito\n");
                exit(1);
            }
        }
        if (!strcmp(atual.val, ";")) {
            analisadorLexico(file);
        }
        else {
            printf("Rejeito\n");
            exit(1);
        }

        bloco(file);

        if (!strcmp(atual.val, ".")) {
            analisadorLexico(file);
        }
        else {
            printf("Rejeito\n");
            exit(1);
        }
    } else{
        printf("Rejeito\n");
        return;
    }

    printf("Aceita\n");
}

void bloco(FILE *file)
{
    if (!strcmp(atual.val, "label")) {
        analisadorLexico(file);
        declaracaoRotulo(file);
    }
    if (!strcmp(atual.val, "var")){
        analisadorLexico(file);
        parteDeclaracaoVariaveis(file);
    }

    comandoComposto(file);
}

void declaracaoRotulo(FILE *file)
{
    if (!strcmp(atual.val, "label")){
        analisadorLexico(file);
        numero(file);
        
        while (!strcmp(atual.val, ",")){
            analisadorLexico(file);
            numero(file);
        }
        
        if(!strcmp(atual.val, ";")){
            analisadorLexico(file);
        } else {
            printf("Rejeito\n");
            exit(1);
        }
    }
    else{
        printf("Rejeito\n");
        exit(1);
    }
}

void definicaoTipo(FILE *file)
{
    if (!strcmp(atual.val, "integer"))
        analisadorLexico(file);
    else {
        printf("Rejeito\n");
        exit(1);
    }
}

void parteDeclaracaoVariaveis(FILE *file)
{
    if (strcmp(atual.val, "var")) {
        analisadorLexico(file);
        declaracaoVariaveis(file);
        analisadorLexico(file);

        if (strcmp(atual.val, ";")) {
            printf("Rejeito\n");
            exit(1);
        }

        analisadorLexico(file);
        
        while (atual.tipoToken == identifier) {
            declaracaoVariaveis(file);
            analisadorLexico(file);
            
            if (!strcmp(atual.val, ";")) {
                analisadorLexico(file);
            } else {
                printf("Rejeito\n");
                exit(1);
            }
        }
    }
    else {
        printf("Rejeito\n");
        exit(1);
    }
}

void declaracaoVariaveis (FILE *file)
{
    listaIdentificadores(file);
    
    if (!strcmp(atual.val, ":")) {
        analisadorLexico(file);
        definicaoTipo(file);
    }
    else {
        printf("Rejeito\n");
        exit(1);
    }
}

void listaIdentificadores(FILE *file)
{
    identificador(file);
    while (!strcmp(atual.val, ",")) {
        analisadorLexico(file);
        identificador(file);
    }
}

void comandoComposto (FILE *file)
{
    if (!strcmp(atual.val, "begin")) {
        analisadorLexico(file);
        comando(file);
        
        while(!strcmp(atual.val, ";")) {
            analisadorLexico(file);
            comando(file);
        }

        if (strcmp(atual.val, "end")) {
            printf("Rejeito\n");
            exit(1);
        }
        else {
            analisadorLexico(file);
        }
    }
    else {
        printf("Rejeito\n");
        exit(1);
    }
}

void comando (FILE *file)
{
    if (atual.tipoToken == number) {
        analisadorLexico(file);
        
        if (!strcmp(atual.val, ":")) {
            analisadorLexico(file);
        } else {
            printf("Rejeito\n");
            exit(1);
        }
    }
    comandoSemRotulo(file);
}


void write(FILE *file)
{
    if (!strcmp(atual.val, "write")){
        analisadorLexico(file);
        if(!strcmp(atual.val, "(")){
            analisadorLexico(file);
            numero(file);
            if (!strcmp(atual.val, ")")) {
                analisadorLexico(file);
            } else {
                printf("Rejeito\n");
            }
        }
        else {
            printf("Rejeito\n");
        }
    } else{
        printf("Rejeito\n");
    }

}

void read(FILE *file)
{
    if (!strcmp(atual.val, "read")) {
        analisadorLexico(file);
        if (!strcmp(atual.val, "(")) {
            analisadorLexico(file);
            numero(file);
            if (!strcmp(atual.val, ")")) {
                analisadorLexico(file);
            }
            else {
                printf("Rejeito\n");
            }
        }
        else {
            printf("Rejeito\n");
        }
    } else {
        printf("Rejeito\n");
    }
}

void comandoSemRotulo(FILE *file)
{
    if (atual.tipoToken == identifier) {
        analisadorLexico(file);
        atribuicao(file);
    }
    else if (!strcmp(atual.val, "goto")) {
        desvios(file);
    } else if (!strcmp(atual.val, "begin")) {
        comandoComposto(file);
    } else if (!strcmp(atual.val, "if")) {
        comandoCondicional(file);
    } else if (!strcmp(atual.val, "while")) {
        comandoRepetitivo(file);
    } 
    else if (!strcmp(atual.val, "write")) {
        write(file);
    } else if (!strcmp(atual.val, "read")) {
        read(file);
    }else{
        printf("Rejeito\n");
        exit(1);
    }
}

void atribuicao (FILE *file)
{
    variavel(file);
    if (!strcmp(atual.val, ":=")) {
        analisadorLexico(file);
        expressao(file);
    }
    else {
        printf("Rejeito\n");
        exit(1);
    }
}

void desvios(FILE *file)
{
    if (!strcmp(atual.val, "goto")) {
        analisadorLexico(file);
        numero(file);
    }
    else {
        printf("Rejeito\n");
        exit(1);
    }
}

void comandoCondicional(FILE *file)
{
    if (!strcmp(atual.val, "if")) {
        analisadorLexico(file);
        expressao(file);

        if (!strcmp(atual.val, "then")) {
            analisadorLexico(file);
            comandoSemRotulo(file);
            
            if (!strcmp(atual.val, "else")) {
                analisadorLexico(file);
                comandoSemRotulo(file);
            }
        }
        else {
            printf("Rejeito\n");
            exit(1);
        }
    }
    else {
        printf("Rejeito\n");
        exit(1);
    }
}

void comandoRepetitivo(FILE *file)
{
    if (!strcmp(atual.val, "while")){
        analisadorLexico(file);
        expressao(file);

        if (!strcmp(atual.val, "do")){
            analisadorLexico(file);
            comandoSemRotulo(file);
        } else {
            printf("Rejeito\n");
            exit(1);
        }
    }   
    else{
        printf("Rejeito\n");
        exit(1);
    }
}

void listaExpressoes(FILE *file)
{
    expressao(file);
    
    while (!strcmp(atual.val, ",")) {
        analisadorLexico(file);
        expressao(file);
    }
}

void relacao(FILE *file)
{
    if(atual.tipoToken == compoundOperator || !strcmp(atual.val, ">")
    || !strcmp(atual.val, "<") || !strcmp(atual.val, "=") 
    || strcmp(atual.val, ":=")){
        analisadorLexico(file);
    } else{
        printf("Rejeito\n");
        exit(1);
    }
}

void expressao(FILE *file)
{

    expressaoSimples(file);
    if(atual.tipoToken == compoundOperator || !strcmp(atual.val,">")
            || !strcmp(atual.val, "<") || !strcmp(atual.val, "=") 
            || strcmp(atual.val, ":=")) {
        relacao(file);
        expressaoSimples(file);
    }
    
}

void expressaoSimples(FILE *file)
{
    if(!strcmp(atual.val, "+") || !strcmp(atual.val, "-") ){
            analisadorLexico(file);
            termo(file);
        while(!strcmp(atual.val, "+") ||!strcmp(atual.val, "-") ||!strcmp(atual.val, "or")){ 
                analisadorLexico(file);
                termo(file);
        }
    }
    else {
        printf("Rejeito\n");
        exit(1);   
    }
}

void termo(FILE *file)
{
    fator(file);
    while(!strcmp(atual.val, "*") ||!strcmp(atual.val, "div") ||!strcmp(atual.val, "and")){
        analisadorLexico(file);
        fator(file);
    }
        
}

void fator(FILE *file)
{
    if (atual.tipoToken == identifier) {
        variavel(file);
    }
    else if(atual.tipoToken == number) {
        numero(file);
    }
    else if(!strcmp(atual.val, "(")) {
        analisadorLexico(file);
        expressao(file);
        if (!strcmp(atual.val, ")")) {
            analisadorLexico(file);
        }
        else {
            printf("Rejeito\n");
            exit(1);   
        }
    }
    else if (!strcmp(atual.val, "not")) {
        fator(file);
    }
    else {
        printf("Rejeito\n");
        exit(1);   
    }
}



void variavel(FILE *file)
{
    if (atual.tipoToken == identifier) {
        analisadorLexico(file);
    }
    else {
        printf("Rejeito\n");
        exit(1);
    }
}

void numero(FILE *file)
{
    if (atual.tipoToken == number)
        analisadorLexico(file);
    else {
        printf("Rejeito\n");
        exit(1);
    }
}

void identificador(FILE *file)
{
    if (atual.tipoToken == identifier)
        analisadorLexico(file);
    else {
        printf("Rejeito\n");
        exit(1);
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
    programa(file);

    fclose(file);

    return 0;
}