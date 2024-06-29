#include <stdio.h>
#include <stdlib.h>
typedef enum {keyword, operator, compoundOperator,
    identifier, number, delimiter,
    comments, unknown} tipoToken;
    
typedef struct {
    char *val;
    int tipoToken;
} token;

token atual;

void programa()
{
    if (strcmp(atual.val, "program") == 0) {
        analisadorLexico();
        identificador(atual);
        if (strcmp(atual.val, "(")) {
            analisadorLexico();
            listaIdentificadores();
            analisadorLexico();
            if (strcmp(atual.val, ")") != 0) {
                printf("ERRO\n");
                exit(1);
            }
        }
        analisadorLexico();
        if (strcmp(atual.val, ";") != 0) {
            printf("ERRO\n");
            exit(1);
        }
        analisadorLexico();
        bloco();
        if (strcmp(atual.val, ".")) {
            printf("ERRO\n");
            exit(1);
        }
    }
}

void bloco()
{
    if (!strcmp(atual.val, "label")) {
        declaracaoRotulo();
    }
    if (!strcmp(atual.val, "var")){
        parteDeclaracaoVariaveis();
    }

    comandoComposto();
}

void declaracaoRotulo()
{
    if (!strcmp(atual.val, "label")){
        analisadorLexico();
        numero();
        while (!strcmp(atual.val, ",")){
            numero();
        }
        if(strcmp(atual.val, ";")){
            printf("ERRO\n");
            exit(1);
        }
    }
    else{
        printf("ERRO\n");
        exit(1);
    }
}

void definicaoTipo()
{
    if (!strcmp(atual.val, "integer"))
        analisadorLexico();
    else {
        printf("ERRO\n");
        exit(1);
    }
}

void parteDeclaracaoVariaveis()
{
    if (strcmp(atual.val, "var")) {
        analisadorLexico();
        declaracaoVariaveis();
        analisadorLexico();
        if (strcmp(atual.val, ";")) {
            printf("ERRO\n");
            exit(1);
        }
        analisadorLexico();
        while (atual.tipoToken == identifier) {
            declaracaoVariaveis();
            analisadorLexico();
            if (strcmp(atual.val, ";")) {
                printf("ERRO\n");
                exit(1);
            }
            analisadorLexico();
        }
    }
    else {
        printf("ERRO\n");
        exit(1);
    }
}

void declaracaoVariaveis ()
{
    listaIdentificadores();
    if (!strcmp(atual.val, ":")) {
        analisadorLexico();
        tipo();
    }
    else {
        printf("ERRO\n");
        exit(1);
    }
}

void listaIdentificadores()
{
    identificador();
    while (!strcmp(atual.val, ",")) 
        identificador();
}

void comandoComposto ()
{
    if (!strcmp(atual.val, "begin")) {
        analisadorLexico();
        comando();
        while(!strcmp(atual.val, ";"))
            comando();

        if (strcmp(atual.val, "end")) {
            printf("ERRO\n");
            exit(1);
        }
    }
    else {
        printf("ERRO\n");
        exit(1);
    }
}

void comando ()
{
    if (atual.tipoToken == number) {
        analisadorLexico();
        if (strcmp(atual.val, ":")) {
            printf("ERRO\n");
            exit(1);
        }
    }
    comandoSemRotulo();
}

void comandoSemRotulo()
{
    if (atual.tipoToken == identifier) 
        atribuicao();
    else if (!strcmp(atual.val, "goto")) 
        desvios();
    else if (!strcmp(atual.val, "begin")) 
        comandoComposto();
    else if (!strcmp(atual.val, "if")) 
        comandoCondicional();
    else if (!strcmp(atual.val, "while")) 
        comandoRepetitivo();
    else {
        printf("ERRO\n");
        exit(1);
    }
}

void atribuicao ()
{
    variavel();
    if (!strcmp(atual.val, ":=")) {
        analisadorLexico();
        expressao();
    }
    else {
        printf("ERRO\n");
        exit(1);
    }
}

void desvios()
{

}

void comandoCondicional()
{

}

void comandoRepetitivo()
{
    
}

void listaExpressoes()
{

}

void relacao()
{

}

void expressao()
{

}

void expressaoSimples()
{

}

void termo()
{


}

void fator()
{
    
}



void variavel()
{
    if (atual.tipoToken == identifier)
        analisadorLexico();
    else {
        printf("ERRO");
        exit(1);
    }
}

void numero()
{
    if (atual.tipoToken == number)
        analisadorLexico();
    else {
        printf("ERRO");
        exit(1);
    }
}

void identificador()
{
    if (atual.tipoToken == identifier)
        analisadorLexico();
    else {
        printf("ERRO");
        exit(1);
    }
}



int main()
{

    return 0;
}