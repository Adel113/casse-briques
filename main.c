#include <stdio.h>
#include <stdlib.h>
#include <SDL_ttf.h>
#include <stdbool.h>
#include <SDL_image.h>
#include <SDL.h>
#define SCREEN_WIDTH 1080
#define SCREEN_HEIGHT 640




#define BALL_SIZE 30
#define BALL_SPEED 5
typedef struct {
    int x;
    int y;
    int vx ;
    int vy;
} Ball;

#define PADDLE_WIDTH 120
#define PADDLE_HEIGHT 14
#define PADDLE_SPEED 40
typedef struct {
    int x ;
    int y;
    bool active;
} Paddle;


#define BRICK_WIDTH 81
#define BRICK_HEIGHT 50
#define NB_BRICKS 12
typedef struct {
    int x ;
    int y;
    bool active;
} Brick;


// initialise la balle
void InitBall(Ball* ball) {
    ball->x = SCREEN_WIDTH / 2 - BALL_SIZE / 2;
    ball->y = SCREEN_HEIGHT / 2 - BALL_SIZE / 2;
    ball->vx = BALL_SPEED ;
    ball->vy = BALL_SPEED ;

}
// initialise le paddle
void InitPaddle(Paddle* paddle) {
    paddle->x = SCREEN_WIDTH / 2 - PADDLE_WIDTH / 2;
    paddle->y = SCREEN_HEIGHT - PADDLE_HEIGHT - 8;
}
//initialise les briques
void InitBricks(Brick bricks[NB_BRICKS]) {
    for (int i = 0; i < NB_BRICKS; i++) {
        bricks[i].x = i * (BRICK_WIDTH + 10);
        bricks[i].y = 50;
        bricks[i].active = true;
    }
}


int vierestantes = 5;

void handle_ball_collision(Ball* ball){
    int ball_bottom = ball->y + BALL_SIZE;
    for (int i = 0; i < vierestantes; i++) {
        if ( ball_bottom >= SCREEN_HEIGHT-10) {
            vierestantes--;
        }
    }
}


//Verifiez si la balle entre en collision avec le rect
bool VerifierCollisionball(Ball* ball, SDL_Rect* rect ) {
    int ball_left = ball->x;
    int ball_right = ball->x + BALL_SIZE;
    int ball_top = ball->y;
    int ball_bottom = ball->y + BALL_SIZE;
    int rect_left = rect->x;
    int rect_right = rect->x + rect->w;
    int rect_top = rect->y;
    int rect_bottom = rect->y + rect->h;
    if (ball_right < rect_left || ball_left > rect_right)
        return false;
    if (ball_bottom < rect_top || ball_top > rect_bottom)
        return false;
    return true;
}

// Vérifiez si la balle entre en collision avec l'une des briques
void VerifierCollisionbricks(Ball* ball, Brick bricks[NB_BRICKS]) {
    for (int i = 0; i < NB_BRICKS; i++) {
        Brick* brick = &bricks[i];
        if (!brick->active)
            continue;
        SDL_Rect rect = {brick->x, brick->y, BRICK_WIDTH, BRICK_HEIGHT};
        if (VerifierCollisionball(ball, &rect)) {
            brick->active = false;
            ball->vy = -ball->vy;
        }
    }
}




int WinMain(int argc, char* argv[]) {
    // Initialise la SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }
    SDL_Window* window = SDL_CreateWindow("Casebreak", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == NULL) {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) {
        printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }


    SDL_Event e;
    bool start = false;

    int ttf_init_result = TTF_Init();
    if (ttf_init_result != 0) {
        fprintf(stderr, "Erreur d'initialisation TTF : %s\n", TTF_GetError());
        return 0;
    }

    TTF_Font * font = TTF_OpenFont("C:\\Users\\sidi ahmed\\Documents\\COMICATE.TTF", 50);
    if (font == NULL) {
        fprintf(stderr, "Erreur d'ouverture de police : %s\n", TTF_GetError());
        TTF_Quit();
        return 0;
    }

    // 1ere fenêtre
    while (!start) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                TTF_CloseFont(font);
                TTF_Quit();
                return 0;
            }
            if (e.type == SDL_MOUSEBUTTONUP) {
                start = true;
                break;
            }
        }
        SDL_RenderClear(renderer);
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 31);
        SDL_Color color = {0, 0, 0, 0};
        SDL_Surface * textSurface = TTF_RenderText_Blended(font, "=Cliquez sur la surface !=", color);
        if (textSurface == NULL) {
            fprintf(stderr, "Erreur de rendu de texte : %s\n", TTF_GetError());
            SDL_RenderPresent(renderer);
            TTF_CloseFont(font);
            TTF_Quit();
            return 0;
        }
        SDL_Texture * textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
        if (textTexture == NULL) {
            fprintf(stderr, "Erreur de création de texture à partir de surface : %s\n", SDL_GetError());
            SDL_FreeSurface(textSurface);
            SDL_RenderPresent(renderer);
            TTF_CloseFont(font);
            TTF_Quit();
            return 0;
        }
        SDL_Rect textRect = {200, 280, textSurface->w, textSurface->h};
        SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
        SDL_FreeSurface(textSurface);
        SDL_DestroyTexture(textTexture);
        SDL_RenderPresent(renderer);
    }
    TTF_CloseFont(font);
    TTF_Quit();




    Ball ball;
    InitBall(&ball);
    Paddle paddle ;
    InitPaddle(&paddle);
    Brick bricks [NB_BRICKS];
    InitBricks(bricks);

    int timer=SDL_GetTicks();
    bool quit = false;


    while (!quit) {

        if(SDL_GetTicks()-timer>30){timer= SDL_GetTicks();
            // deplacemnt de la balle

            ball.y += ball.vy;
            ball.x += ball.vx;

            //verification des collisions

            VerifierCollisionbricks(&ball, bricks);
            SDL_Rect top_wall = {0, 0, SCREEN_WIDTH, 10};
            SDL_Rect bottom_wall = {0, SCREEN_HEIGHT - 10, SCREEN_WIDTH, 10};
            SDL_Rect left_wall = {0, 0, 10, SCREEN_HEIGHT};
            SDL_Rect right_wall = {SCREEN_WIDTH - 10, 0, 10, SCREEN_HEIGHT};
            SDL_Rect paddle_rect = {paddle.x, paddle.y, PADDLE_WIDTH, PADDLE_HEIGHT};

            //verification des collisions

            if (VerifierCollisionball(&ball, &top_wall) || VerifierCollisionball(&ball, &bottom_wall)) {
                ball.vy = -ball.vy;
            } else if (VerifierCollisionball(&ball, &left_wall) || VerifierCollisionball(&ball, &right_wall)) {
                ball.vx = -ball.vx;
            } else if (VerifierCollisionball(&ball, &paddle_rect)) {
                ball.vy = -ball.vy;
            } else if (VerifierCollisionball(&ball , &bricks)){
                ball.vx =  ball.vx ;
            }

            // deplacement  de paddle
            while (SDL_PollEvent(&e) != 0) {
                if (e.type == SDL_QUIT) {
                    quit = true;
                } else if (e.type == SDL_KEYDOWN) {
                    switch (e.key.keysym.sym) {
                        case SDLK_LEFT:
                            paddle.x -= PADDLE_SPEED;
                            if (paddle.x < 0)
                                paddle.x = 0;
                            break;
                        case SDLK_RIGHT:
                            paddle.x += PADDLE_SPEED;
                            if (paddle.x + PADDLE_WIDTH > SCREEN_WIDTH)
                                paddle.x = SCREEN_WIDTH - PADDLE_WIDTH;
                            break;
                    }
                }

            }
        }

        // je veux faire ceci ::: tant que ( vierestantes > 3) j'ajoute une ligne de briques a chaque 5 bricks supprimer
        // si les brique = 0 j'affiche un texte ( vous avez gagner ) , a condition viesrestantes != 0

        SDL_SetRenderDrawColor(renderer, 255, 255, 0, 0);
        SDL_RenderClear(renderer);
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_Rect ball_rect = {ball.x, ball.y, BALL_SIZE, BALL_SIZE};
        SDL_RenderFillRect(renderer, &ball_rect);
        SDL_Rect paddle_rect = {paddle.x, paddle.y, PADDLE_WIDTH, PADDLE_HEIGHT};
        SDL_RenderFillRect(renderer, &paddle_rect);

        for (int i = 0; i < NB_BRICKS ; i++) {
            Brick* brick = &bricks[i];
            if (brick->active) {
                SDL_Rect brick_rect = {brick->x  , brick->y , BRICK_WIDTH, BRICK_HEIGHT};
                SDL_SetRenderDrawColor(renderer, 0,0,0,0);
                SDL_RenderFillRect(renderer, &brick_rect);


            }

        }


        for(int i = 0; i < vierestantes; i++){
            SDL_Rect lifeRect = {10 +  i *20, 10, 10, 10};
            SDL_SetRenderDrawColor(renderer, 0,0,255,255);
            SDL_RenderFillRect(renderer, &lifeRect);
        }
        handle_ball_collision(&ball );

        SDL_RenderPresent(renderer);
    }
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
