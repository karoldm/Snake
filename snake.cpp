#include <iostream>
#include <SDL.h>
#include "snake.hpp"
#include "globalVariables.hpp"

void Snake::newGame(SDL_Renderer *renderer){
    score.loadScore(renderer); //exibindo score do jogador
    score.updateScore(renderer, 0); //atualizando score
    life.loadLife(renderer); //exibindo barra de vida
    obst.loadObstaculos(renderer); //carregando obstaculos
    food.loadFood(renderer, obst.getVetObst()); //carregando ma�a
    newSnake();
    totalScore = 0;
    inGame = true;
}

void Snake::newSnake(){
    snake.dx = 1;
    snake.dy = 0;
    snake.head->rect.x = INITIAL_X;
    snake.head->rect.y = INITIAL_Y;
    snake.head->rect.w = SNAKEW;
    snake.head->rect.h = SNAKEH;
    snake.head->prox = NULL;
    snake.head->ant = NULL;
    snake.lastTail = snake.head;

    for(int i = 0; i < INITIAL_TAIL; i++) insereFinalTail(); //inserindo n�s iniciais

}

void Snake::returnSnakeInicio(){
    snake.dx = 1;
    snake.dy = 0;
    snake.head->rect.x = INITIAL_X;
    snake.head->rect.y = INITIAL_Y;

}

void Snake::renderSnake(SDL_Rect *tailNode, SDL_Renderer *renderer){ //renderiza n� por n�

    SDL_SetRenderDrawColor(renderer, 0, 255, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderFillRect(renderer, tailNode);

}

void Snake::updateSnake(SDL_Renderer *renderer){

    for(struct TailNode *aux = snake.lastTail; aux != NULL; aux = aux->ant){ //percorrendo cobra

        if(aux->ant == NULL){ //cabe�a da cobra
            snake.head->rect.x += snake.dx * SNAKEW; //nova posi��o
            snake.head->rect.y += snake.dy * SNAKEH; //novaposi��o
        }

        else{

            aux->rect.x = aux->ant->rect.x; //"copiando posi��o" da cabe�a
            aux->rect.y = aux->ant->rect.y; //"copiando posi��o" da cabe�a
        }

        renderSnake(&aux->rect, renderer);
    }
    checarColisao(renderer);
}

bool Snake::insereFinalTail(){ //insere um novo n� na lista snake
    struct TailNode *newNode = (struct TailNode*)malloc(sizeof(struct TailNode)); //alocando novo n�

    if(newNode == NULL) return false;

    newNode->rect.w = SNAKEW;
    newNode->rect.h = SNAKEH;
    newNode->rect.x = snake.lastTail->rect.x+SNAKEW;
    newNode->rect.y = snake.lastTail->rect.y;

    newNode->prox = NULL;
    newNode->ant = snake.lastTail;
    snake.lastTail->prox = NULL;
    snake.lastTail = newNode;

    snake.tam++;
    return true;
}

void Snake::checarColisao(SDL_Renderer *renderer){

    //colisao com food
    if(abs(snake.head->rect.x - (food.getFoodPos().x+5)) < SNAKEW && abs(snake.head->rect.y - (food.getFoodPos().y+5)) < SNAKEH){
        insereFinalTail(); //aumentando tail
        food.loadFood(renderer, obst.getVetObst()); //gerando uma nova posi��o para food
        totalScore = totalScore + 10; //aumentandos os pontos
        score.updateScore(renderer, totalScore); //atualizando score

    }

    //colisao com a tela. Ao colidir a cobra "da a volta" na tela
    if(snake.head->rect.x > SCREEN_WIDTH - SNAKEW)  snake.head->rect.x = 0;
    else if(snake.head->rect.x < 0 - SNAKEW)  snake.head->rect.x = SCREEN_WIDTH;
    else if(snake.head->rect.y < 0 - SNAKEH-50)  snake.head->rect.y = SCREEN_HEIGHT-50;
    else if(snake.head->rect.y > SCREEN_HEIGHT - SNAKEH-50)  snake.head->rect.y = 0;

    //colisao com obstaculos
    for(int i = 0; i < QUANTIDADE_OBSTACULOS; i++)
        if(abs(snake.head->rect.x - (obst.getVetObst()[i].x+10)) < SNAKEW && abs(snake.head->rect.y - (obst.getVetObst()[i].y+12)) < SNAKEH){
            life.popLife(); //a cada colis�o com os obstaculos o jogador perde um ponto de vida
            if(!life.getAlive()) inGame = false;
            returnSnakeInicio(); //cobrinha retorna a posi��o inicial

        }
}

bool Snake::events(SDL_Event *e){
    while(SDL_PollEvent(e) != 0){
        if(e->type == SDL_KEYDOWN){
            switch(e->key.keysym.sym){
                case SDLK_RIGHT:
                    mudarDirecao(RIGHT);
                    break;
                case SDLK_LEFT:
                    mudarDirecao(LEFT);
                    break;
                case SDLK_UP:
                    mudarDirecao(UP);
                    break;
                case SDLK_DOWN:
                    mudarDirecao(DOWN);
                    break;
            }
        }
    }
    return true;
}

void Snake::mudarDirecao(direcao dir){
    if(dir == RIGHT && snake.dx != -1){
        snake.dx = 1;
        snake.dy = 0;
    }
    else if(dir == LEFT && snake.dx != 1){
        snake.dx = -1;
        snake.dy = 0;
    }
    else if(dir == UP && snake.dy != 1){
        snake.dy = -1;
        snake.dx = 0;
    }
    else if(dir == DOWN && snake.dy != -1){
        snake.dy = 1;
        snake.dx = 0;
    }
}

void Snake::renderGame(SDL_Renderer *renderer){
    //renderizando os componentes atualizados
    updateSnake(renderer);
    obst.renderObstaculos(renderer);
    score.renderScore(renderer);
    life.renderLife(renderer);
    food.renderFood(renderer);
}

int Snake::getScore(){
    return totalScore;
}

