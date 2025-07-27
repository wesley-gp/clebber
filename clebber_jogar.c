#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>


void inicio(){
    printf("Bem vindo ao Cleber! ^-^\n\n");
    printf("O jogo consiste em quatro rounds onde voce tera que advinhar a proxima palavra mudando somente uma letra da palavra anterior, nao se preocupe! temos dicas.\n");
    printf("Regras:\n - So pode mudar uma letra da palavra anterior \n - Voce tem tres erros no geral, ou seja, ao acertar uma palavra os valor de erros nao reinicia \n - So possui um skip :)\n\n");
}

void escolher(int* jogo_escolhido, int id){
    int aux;
    printf("Escolha um valor entre %d e %d\n",1,id);
    scanf("%d",&aux);

    while(aux > id || aux < 1){
        printf("Valor invalido, escolha outro: ");
        scanf("%d",&aux);
    }
		printf("entendido, valor %d escolhido.\n",aux);
    *jogo_escolhido = aux;

}

int analisarPalavraInserida(char palavraAnterior[20],char palavraInserida[20]){
    if(strlen(palavraInserida) != strlen(palavraAnterior)){
        printf("O seu palpite possui uma quantidade de letras diferente!\n");
        return 1;
    }else{
        int letrasDiferentes = 0;
        for(int i = 0; i < strlen(palavraAnterior); i++){
            if(palavraAnterior[i] != palavraInserida[i]){
                letrasDiferentes++;
            }
        }

        if(letrasDiferentes > 1){
            printf("A palavra inserida possui mais de uma letra diferente!\n");
            return 1;
        }else{
            return 0;
        }
    }
}

int acerto(char palavraCerta[20],char palavraInserida[20]){
    if(strcmp(palavraCerta,palavraInserida) == 0){
        return 0;
    }else{
        return 1;
    }
}

void converterMinusculo(char *palavra){
    for(int i = 0; palavra[i]; i++){
        palavra[i] = tolower(palavra[i]);
    }
}

int pular(int skip, char resposta){
    if(skip > 0 && resposta == 's'){
        return 1;
    }else{
        return 0;
    }
}

typedef struct{
    char palavra[20];
    char dica[100];
}PalavraDica; //criacao da struct


int main(){

    int tam_max = 10;

    inicio();

    FILE *arquivo;

    arquivo = fopen("dicas.txt","r"); //abre o arquivo que contem as palavras habilitando so para leitura

    
    if(arquivo == NULL){
        printf("Erro ao abrir o arquivo de palavras!\n");
        return 1; // Sai do programa com erro
    }

    char chave[100]; 
    int idx;

    fscanf(arquivo,"%[^:]: %d",chave,&idx); //pega a quantidade de jogos existentes atraves do fscanf


    int jogo_escolhido; 
    
    escolher(&jogo_escolhido,idx); //chama o jogo

    int jogo_atual = 0;

    char linhas[1000];

    while(jogo_atual < jogo_escolhido-1){
        fgets(linhas,1000,arquivo);
        if(strcmp(linhas,"#\n") == 0){
            jogo_atual++;
        }
    } //le os jogos ate que o id do jogo_atual seja igual ao id do jogo escolhido

    char primeira_palavra[20];
    int id;

    fscanf(arquivo,"%d %s",&id, primeira_palavra); //pega a primeira palavra, que nao possui nenhuma dica, para que possamos usar struct

    int contador = 1; //esse contador percorre a quantidade de linhas

    PalavraDica vetor[4]; //criando o vetor de structs
    int contarPalavrasEDicas = 0; //variavel auxiliar que sera utilizada para alterar o indice do vetor vetor 

    while(fgets(linhas,1000,arquivo) != NULL){
        linhas[strcspn(linhas, "\n")] = 0;

        if(strcmp(linhas,"#") == 0){
            break;
        } //pegar ate o fim do jogo que foi sorteado

        if(strcmp(primeira_palavra,linhas) == 0){
            continue;
        } //pula a primeira palavra, ja que pegamos ela antes e ela nao possui dica
        

        if(contador != 1 && contador != tam_max){
            if(contador%2 == 0){
                strcpy(vetor[contarPalavrasEDicas/2].dica,linhas); //pega a dica
            }else{
                strcpy(vetor[contarPalavrasEDicas/2].palavra,linhas); //pega a palavra
            }
            contarPalavrasEDicas++; //o indice sempre sera o dobro, porque ele aumenta para cada palavra e para cada dica, portanto, pegaremos metade do valor dele
        }

        contador++; //esse contador percorre a quantidade de linhas
    }

    printf("Quantidade de letras das palavras: %d\n",strlen(primeira_palavra));
    int vidas = 3;
    int acertos = 0;
    int skip = 1;
    char palavraAnterior[20]; //salva a palavra anterior

    converterMinusculo(primeira_palavra);

    strcpy(palavraAnterior,primeira_palavra);

    printf("Palavra: %s\n",primeira_palavra); //imprime a primeira palavra 

    while(vidas > 0 && acertos < 4){
        char palavraInserida[20]; //salva a palavra inserida
        char palavraCerta[20]; //salva a palavra certa 

        strcpy(palavraCerta,vetor[acertos].palavra);
        converterMinusculo(palavraCerta);

        printf("dica para a proxima palavra: %s\n",vetor[acertos].dica);
        printf("faca seu palpite (a palavra nao deve ser acentuada): ");
        scanf(" %[^\n]",palavraInserida);

        converterMinusculo(palavraInserida);
        

        int analisar = analisarPalavraInserida(palavraAnterior,palavraInserida); //analisa se a palavra inserida é valida, ou seja, se é do mesmo tamanho ou tem mais de uma letra diferente

        while(analisar == 1){
            printf("faca outro palpite: ");
            scanf(" %[^\n]",palavraInserida);
            analisar = analisarPalavraInserida(palavraAnterior,palavraInserida);
        };

        if(acerto(palavraCerta,palavraInserida) == 0){
            acertos++;
            strcpy(palavraAnterior,palavraCerta);
            strcpy(palavraCerta,vetor[acertos].palavra);
        }else{
            vidas--;
            printf("Errou! Vidas restantes: %d\n", vidas);
            printf("Deseja pular a palavra? (s/n)");
            char resposta;
            scanf(" %c",&resposta);

            resposta = tolower(resposta);

            if (resposta == 's') {
                if (pular(skip, resposta)) {
                printf("Burrao! A palavra era: %s (-_-)\n",palavraCerta);
                skip--;
                acertos++;
                strcpy(palavraAnterior, vetor[acertos].palavra);
                continue;
            }else {
                    printf("Voce nao pode mais pular rs\n");
                }
            }
            
        }

    }

    if (acertos == 4) {
        printf("Parabens! Voce completou todos os rounds! (*^O^*)\n");
    } else {
        printf("Fim de jogo! Mais sorte na proxima! /(>_<)/\n");
    }

    fclose(arquivo);

    return 0;
}



