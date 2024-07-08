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

            if (!strcmp(atual.val, ")")) {
                analisadorLexico();
            }
            else {
                printf("ERRO\n");
                exit(1);
            }
        }
        if (!strcmp(atual.val, ";") != 0) {
            analisadorLexico();
        }
        else {
            printf("ERRO\n");
            exit(1);
        }

        bloco();

        if (!strcmp(atual.val, ".")) {
            analisadorLexico();
        }
        else {
            printf("ERRO\n");
            exit(1);
        }
    }
}

void bloco()
{
    if (!strcmp(atual.val, "label")) {
        analisadorLexico();
        declaracaoRotulo();
    }
    if (!strcmp(atual.val, "var")){
        analisadorLexico();
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
            analisadorLexico();
            numero();
        }
        
        if(!strcmp(atual.val, ";")){
            analisadorLexico();
        } else {
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
            
            if (!strcmp(atual.val, ";")) {
                analisadorLexico();
            } else {
                printf("ERRO\n");
                exit(1);
            }
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
        
        while(!strcmp(atual.val, ";")) {
            analisadorLexico();
            comando();
        }

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
        
        if (!strcmp(atual.val, ":")) {
            analisadorLexico();
        } else {
            printf("ERRO\n");
            exit(1);
        }
    }
    comandoSemRotulo();
}

void comandoSemRotulo()
{
    if (atual.tipoToken == identifier) {
        analisadorLexico();
        atribuicao();
    }
    else if (!strcmp(atual.val, "goto")) {
        analisadorLexico();
        desvios();
    } else if (!strcmp(atual.val, "begin")) {
        analisadorLexico();
        comandoComposto();
    } else if (!strcmp(atual.val, "if")) {
        analisadorLexico();
        comandoCondicional();
    } else if (!strcmp(atual.val, "while")) {
        analisadorLexico();
        comandoRepetitivo();
    } else {
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
    if (!strcmp(atual.val, "goto")) {
        analisadorLexico();
        numero();
    }
    else {
        printf("ERRO\n");
        exit(1);
    }
}

void comandoCondicional()
{
    if (!strcmp(atual.val, "if")) {
        analisadorLexico();
        expressao();

        if (!strcmp(atual.val, "then")) {
            analisadorLexico();
            comandoSemRotulo();
            
            if (!strcmp(atual.val, "else")) {
                analisadorLexico();
                comandoSemRotulo();
            }
        }
        else {
            printf("ERRO\n");
            exit(1);
        }
    }
    else {
        printf("ERRO\n");
        exit(1);
    }
}

void comandoRepetitivo()
{
    if (!strcmp(atual.val, "while")){
        analisadorLexico();
        expressao();

        if (!strcmp(atual.val, "do")){
            analisadorLexico();
            comandoSemRotulo();
        } else {
            printf("ERRO\n");
            exit(1);
        }
    }   
    else{
        printf("ERRO\n");
        exit(1);
    }
}

void listaExpressoes()
{
    expressao();
    
    while (!strcmp(atual.val, ",")) {
        analisadorLexico();
        expressao();
    }
}

void relacao()
{
    if(atual.tipoToken == compoundOperator || !strcmp(atual.val, '>')
    || !strcmp(atual.val, '<') || !strcmp(atual.val, '=') 
    || strcmp(atual.val, ':=')){
        analisadorLexico();
    } else{
        printf("ERRO\n");
        exit(1);
    }
}

void expressao()
{

    expressaoSimples();
    if(atual.tipoToken == compoundOperator || !strcmp(atual.val, '>')
            || !strcmp(atual.val, '<') || !strcmp(atual.val, '=') 
            || strcmp(atual.val, ':=')) {
        relacao();
        expressaoSimples();
    }
    
}

void expressaoSimples()
{
    if(!strcmp(atual.val, "+") || !strcmp(atual.val, "-") ){
            analisadorLexico();
            termo();
        while(!strcmp(atual.val, "+") ||!strcmp(atual.val, "-") ||!strcmp(atual.val, "or")){ 
                analisadorLexico();
                termo();
        }
    }
    else {
        printf("ERRO\n");
        exit(1);   
    }
}

void termo()
{
    fator();
    while(!strcmp(atual.val, "*") ||!strcmp(atual.val, "div") ||!strcmp(atual.val, "and")){
        analisadorLexico();
        fator();
    }
        
}

void fator()
{
    if (atual.tipoToken == identifier) {
        variavel();
    }
    else if(atual.tipoToken == number) {
        numero();
    }
    else if(!strcmp(atual.val, "(")) {
        analisadorLexico();
        expressao();
        if (!strcmp(atual.val, ")")) {
            analisadorLexico();
        }
        else {
            printf("ERRO\n");
            exit(1);   
        }
    }
    else if (!strcmp(atual.val, "not")) {
        fator();
    }
    else {
        printf("ERRO\n");
        exit(1);   
    }
}



void variavel()
{
    if (atual.tipoToken == identifier) {
        analisadorLexico();
        if (){
            
        }
    }
    else {
        printf("ERRO\n");
        exit(1);
    }
}

void numero()
{
    if (atual.tipoToken == number)
        analisadorLexico();
    else {
        printf("ERRO\n");
        exit(1);
    }
}

void identificador()
{
    if (atual.tipoToken == identifier)
        analisadorLexico();
    else {
        printf("ERRO\n");
        exit(1);
    }
}



int main()
{

    return 0;
}
