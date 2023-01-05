#include <stdio.h>
#include<math.h>
#include <SDL2/SDL.h>
#include <stdlib.h>

//gcc -I src/include -L src/lib -o main main.c -lmingw32 -lSDL2main -lSDL2
// ou alors utiliser le make file 
//mingw32-make
#define CASE_SIZE 64
#define taille_joueur 10

#define PAS 10
#define PI 3.14159265

#define ANGLE PI/3 // Angle en rad
#define variation ANGLE/320

#define NB_RAY 100

const int WIDTH = 640, HEIGHT = 640;

int map[10][10]= {
            {1,1,1,1,1,1,1,1,1,1},
            {1,0,1,0,0,0,0,0,0,1},
            {1,0,1,0,1,1,1,0,0,1},
            {1,0,1,0,1,0,0,0,0,0},
            {1,0,1,0,1,0,0,0,0,0},
            {1,0,1,0,1,1,0,0,0,1},
            {1,0,0,0,0,1,1,0,0,1},
            {1,0,0,0,0,1,1,0,0,1},
            {1,0,0,0,0,0,0,0,0,1},
            {1,1,1,1,1,1,1,1,1,1}
        };

// Attention : le point au coordonnées (x,y) est a la case (x/CASE_SIZE, y/CASE_SIZE) ce qui correspont
// a map


void map2D(int tab2D[][10],SDL_Renderer **renderer){
    //dessine la map 2D (quadrillage)
    for (int i=0;i < 10; i++){
        for (int j=0;j<10;j++){
            SDL_Rect rect = {(j)* CASE_SIZE, i * CASE_SIZE,CASE_SIZE,CASE_SIZE};
            if (tab2D[i][j]==1){
                // noir
         
                 SDL_SetRenderDrawColor(*renderer,0,0,0,255);
                  SDL_RenderFillRect(*renderer,&rect);
            }
            else {
                 SDL_SetRenderDrawColor(*renderer,255,255,255,255);
                  SDL_RenderDrawRect(*renderer,&rect);
            }
           
    }
}
}




void dessin_joueur_2D(int *pos_x, int *pos_y,SDL_Renderer**renderer){
    //dessine un rectangle representant le player 
    SDL_Rect rect= {*pos_x-taille_joueur,*pos_y-taille_joueur,taille_joueur,taille_joueur};
     SDL_SetRenderDrawColor(*renderer,0,255,0,255);
     SDL_RenderFillRect(*renderer,&rect);

}

int dessin_ray(int *pos_x, int *pos_y,float *angle, SDL_Renderer**renderer, int tab2D[][10]){

    //dessine un ray a partir d'un point (pos_x,pos_y) et d'un angle (avec horizontal)

    // attention : les coordonnées old et new sont données en pixel  pour savoir si ce pixel est sur 
    // un mur, il faut les convertir en map (coordonnées de 0 a N=10)

 SDL_SetRenderDrawColor(*renderer,0,0,250,255);  // en vert
 int distance_pixel=0;
 

 int new_x=*pos_x;
 int new_y=*pos_y;

 int grid_x=new_x/CASE_SIZE;
 int grid_y=new_y/CASE_SIZE;

 while(grid_x < 10 && grid_y<10 && tab2D[grid_y][grid_x]!=1 ){
    new_x=*pos_x+distance_pixel*cos(*angle);
    new_y=*pos_y+distance_pixel*sin(*angle);

    grid_x=(new_x/CASE_SIZE);
    grid_y=(new_y/CASE_SIZE);

    SDL_RenderDrawPoint(* renderer,new_x, new_y);
    distance_pixel++;

 }
 return distance_pixel;
 
                       
}


void dessin_N_rayons(int *pos_x, int *pos_y,float *angle, SDL_Renderer**renderer, int tab2D[][10], int nombre_ray){

    float angle_depart=*angle;
    for (int i= 0; i< nombre_ray;i++){
        dessin_ray(pos_x,pos_y,&angle_depart,renderer,  tab2D);
        angle_depart=angle_depart+variation;

}
}

int direction(int *pos_x, int *pos_y,float* angle, SDL_Renderer**renderer)
{
  SDL_Event event;
  while(SDL_WaitEvent(&event)){
        switch(event.type){
            case SDL_QUIT:
                return 0;

            case SDL_KEYDOWN : // si une touche est préssée
                switch(event.key.keysym.sym){
                    case SDLK_UP:
                        
                        *pos_y=*pos_y-PAS;
                        return 1;

                    case SDLK_DOWN:
                        *pos_y=*pos_y+PAS;
                        return 1;

                    case SDLK_LEFT:
                        *pos_x=*pos_x-PAS;
                        return 1;

                    case SDLK_RIGHT:
                        *pos_x=*pos_x+PAS;
                        return 1;
                    case SDLK_d:
                        *angle=*angle+4*variation;
                        return 1;

                    case SDLK_a:
                        *angle=*angle-4*variation;
                        return 1;

                    case SDLK_ESCAPE:
                        return 0;
                }
        }
    }
    return 0;
}
    
void creer_fenetre(SDL_Window **window, SDL_Renderer **renderer)
{
    /*Initialise la SDL: crée la fenêtre et le rendu */

    //Initilise la SDL
    if (SDL_Init(SDL_INIT_VIDEO)==-1){
        SDL_Log("ERREUR: Initialisation echouee> %s\n",SDL_GetError()); //Affichage de l'erreur
        exit(EXIT_FAILURE);     //On quitte le programme
    }

    //taille de la fenêtre
    int L=WIDTH;
    int l=HEIGHT;
    *window=NULL;
    *window =SDL_CreateWindow("Test Vicky",SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED ,L,l,SDL_WINDOW_RESIZABLE);

    //Vérification de la création de la fenêtre
    if(*window == NULL){
        SDL_Log("ERREUR: Creation de fenetre echouee > %s\n",SDL_GetError());
        exit(EXIT_FAILURE);
    }

    //Création du rendu
    *renderer=NULL;
    *renderer= SDL_CreateRenderer(*window,-1,SDL_RENDERER_ACCELERATED);

    //Vérification de la création du rendu
    if(*renderer==NULL){
        SDL_Log("ERREUR: Creation de fenetre echouee > %s\n",SDL_GetError());
        exit(EXIT_FAILURE);
    }

    //Coloriage du fond
    SDL_SetRenderDrawColor(*renderer, 255, 0, 0, 255); 
    SDL_RenderClear(*renderer);


}



void fond_blanc(SDL_Renderer **renderer)
{
    /*Met le rendu en blanc*/
    //Coloriage du fond
    SDL_SetRenderDrawColor(*renderer, 150, 155, 155, 255); 
    SDL_RenderClear(*renderer);
}



void affiche1D(int *tab, int taille){
    for (int i=0;i<taille;i++){
        printf("tab[%d]=%d\t",i,tab[i] );
    }
}



void init1D(int *tab, int taille){
    for (int i=0;i<taille;i++){
        tab[i]=0;
    }
}

int main(int argc, char *argv[]){

    // Allocation d'un tableau 1D pour stocker la distance des NB_RAY avec les murs
    int *tab1D;
    tab1D=(int *)malloc ((NB_RAY)*sizeof(int));
    init1D(tab1D,NB_RAY);


   SDL_Window *window=NULL;
   SDL_Renderer *renderer=NULL;

   creer_fenetre(&window, &renderer);

   int px=40;
   int py=40;
   float ang=PI/3;

   printf("variation = %f\n",variation );


   SDL_Event windowEvent;

   int flag=1;

    while (flag){
        flag= direction(&px, &py,&ang,&renderer);  // pour le flag
        fond_blanc(&renderer);
        map2D(map, &renderer); // dessine la map
        dessin_joueur_2D(&px, &py,&renderer);  // actualise la pos du joueur   
        //dessin_ray(&px, &py,&ang,&renderer, map);
        dessin_N_rayons(&px,&py,&ang, &renderer, map, NB_RAY);
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyWindow(window);
    SDL_Quit();
    free(tab1D);
    return EXIT_SUCCESS;
}