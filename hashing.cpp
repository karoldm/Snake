#include <string.h>
#include <SDL.h>
#include <SDL_ttf.h>
#include "hashing.hpp"

#include <iostream>
#include <typeinfo>

void TabelaJogadores::inicializar(){
    int i = 0;
  	for (i=0; i < MAX; i++)
		jogadores[i] = NULL; //vetor de ponteiros com valor inicial nulo
}

bool TabelaJogadores::inserir(char* nome, int score){
    struct jogador *newJogador;
    newJogador = (struct jogador*)malloc(sizeof(struct jogador)); //alocando novo jogador

    if(newJogador != NULL){ //se alocou mem�ria com sucesso
        strcpy(newJogador->nome, nome); //nome do jogador
        newJogador->score = score; //score do jogador
        newJogador->prox = NULL; //ponteiro para proximo jogador no caso de colis�o
    }
    else return false;

    int endereco = hashing(nome); //posi��o do jogador no vetor
    if(jogadores[endereco] == NULL){ //posi��o disponivel
        jogadores[endereco] = newJogador;
    }
    else { //tratamento de colis�o
        jogadores[endereco]->prox = newJogador;
        jogadores[endereco] = newJogador;
    }
    free(newJogador);
    return true;
}

struct jogador* TabelaJogadores::buscar(char* nome){
    int endereco = hashing(nome); //posi��o da tabela que o jogador est�

    struct jogador *aux;
    aux = jogadores[endereco];

    if(aux != NULL)
        while(!(strcmp(aux->nome, nome) == 0) || aux->prox != NULL)// verificando colis�o
            aux = aux->prox;
    else {
     free(aux);
     return NULL;
    }

    if(strcmp(aux->nome, nome) == 0) return aux; //se encontrou ent�o retorna um ponteiro para o jogador
    else {
     free(aux);
     return NULL;
    }
}

void TabelaJogadores::atualizarScore(int score, char* nome){
    int endereco = hashing(nome); //posi��o do jogador
    jogadores[endereco]->score = score; //atualizando score
}

int TabelaJogadores::hashing(char* chave) {
    int i, soma = 0;
    int tam = strlen(chave);
    for (i = 0; i < tam; i++){
        soma = soma + (i * chave[i]);
    }
    return (soma % MAX);
}

void TabelaJogadores::renderTabela(SDL_Renderer *renderer){
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer); //limpando tela

    Font = TTF_OpenFont("digital-7.ttf", 24);
    White = {255, 255, 255};

    messageRect.w = 100;
    messageRect.h = 8;
    messageRect.x = 200;

    int i = 0;
    std::string message;
    struct jogador* aux;
    aux = jogadores[i];

    while(i < MAX){ //percorrendo todas as posi��es do vetor
        if(aux != NULL){ //se existe um jogador naquela posi��o
            do{
                messageRect.y += 50;
                message = aux->nome; //nome do jogador
                message += ": ";
                message += std::to_string(aux->score); //score do jogador
                surfaceText = TTF_RenderText_Solid(Font,  message.c_str(), White);
                messageText = SDL_CreateTextureFromSurface(renderer, surfaceText);
                SDL_RenderCopy(renderer, messageText, NULL, &messageRect);
                aux = aux->prox; //tratamento de colis�o
            }while(aux != NULL);
        }
        i++;
        aux = jogadores[i]; //pr�xima posi��o do vetor/tabela
    }

    SDL_FreeSurface(surfaceText);
}


