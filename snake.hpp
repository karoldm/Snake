#include <SDL.h>
#include "life.hpp"
#include "food.hpp"
#include "obstaculos.hpp"
#include "score.hpp"

struct TailNode{ //n� da lista
    SDL_Rect rect;
    struct TailNode *prox;
    struct TailNode *ant;
};

struct snake{ //TAD para snake
    int dx;
    int dy;
    int tam;
    struct TailNode *head;
    struct TailNode *lastTail;
};

class Snake{
    struct snake snake;
    Life life;
    Food food;
    Score score;
    Obstaculos obst;
    int dir;
    int totalScore;

    static const int INITIAL_X = 10; //posi��o inicial da cobra
    static const int INITIAL_Y = 10; //posi��o inicial da cobra
    static const int SNAKEW = 10;
    static const int SNAKEH = 10;
    static const int INITIAL_TAIL = 5; //quantidade inicial de n�s

    enum direcao{LEFT, RIGHT, UP, DOWN};

public:
    bool inGame;
    int getScore();
    void renderGame(SDL_Renderer *renderer); //renderiza todos os componentes do jogo
    void returnSnakeInicio(); //retorna a cobrinha para a posi��o inicial, mas mant�m seu tamanho
    void newGame(SDL_Renderer *renderer); //inicializa todos os componentes necess�rios para o jogo
    bool events(SDL_Event *e); //verifica os eventos e retorna false caso o bot�o de fechar a janela tenha sido clicado
    void renderSnake(SDL_Rect *tailNode, SDL_Renderer *renderer); //renderiza um TailNode
    void newSnake(); //inicializa snake
    void mudarDirecao(direcao dir); //muda a dire��o de snake
    void updateSnake(SDL_Renderer *renderer); //respons�vel por manter a cobrinha andando e checar colis�o
    bool insereFinalTail(); //fun��o para inserir um TailNode ao final da lista snake
    void checarColisao(SDL_Renderer *renderer); //checa colisao de snake com food, obstaculos e com a tela
};

