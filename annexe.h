//
// Created by garel on 10/09/2023.
//

#ifndef RENFORCEMENT_LEARNING_ANNEXE_H
#define RENFORCEMENT_LEARNING_ANNEXE_H

#include <stdio.h>
#include <math.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
//#include <allegro5/allegro_ttf.h>
#include <string.h>

#define SCREEN_WIDTH 1000
#define SCREEN_HEIGHT 1000

#define alpha 0.1
#define gamma 0.9

#define GRID_WIDTH 9
#define GRID_HEIGHT 9

typedef struct player {
    int Xc;
    int Yc;

}Player;

float fdistance (float x1, float x2, float y1, float y2) {
    return sqrtf((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
}

void initAgent(Player *player, Player *newplayer) {
    player->Xc = 100;
    player->Yc = 100;
    newplayer->Xc = player->Xc;
    newplayer->Yc = player->Yc;
}

int get_Action (Player *player, float Q[GRID_WIDTH][GRID_HEIGHT][4], float exploration_prob, int *oldaction){  //
    // Politique d'exploration/exploitation simple : choisir une action aléatoire avec une petite probabilité d'exploration
    int x = player->Xc;
    int y = player->Yc;

    /*int retour;

    if (oldaction == 0){
        retour = 1;
    }
    if (oldaction == 1){
        retour = 0;
    }
    if (oldaction == 2){
        retour = 3;
    }
    if (oldaction == 3){
        retour = 2;
    }*/

    if (rand() / (double)RAND_MAX < exploration_prob) {
         // Action aléatoire
         return rand() % 4; // 4 actions possibles (avancer, reculer, aller à droite, aller à gauche)
    } else {
        int bestAction = 0;


        float maxQ = Q[(x- 100)/100][(y- 100)/100][0];
        for (int a = 1; a < 4; a++) {
            float qValue = Q[(x- 100)/100][(y- 100)/100][a];

            if (qValue > maxQ) {  //&& a != b
                maxQ = qValue;
                bestAction = a;
            }
            /*if (bestAction == retour) {
                do {
                    bestAction = rand()%4; // Génère une nouvelle valeur aléatoire
                } while (bestAction == retour);
            }
            else {
                bestAction = rand() % 4;
            }*/

        }
        //oldaction = bestAction;

        return bestAction;
    }
}


float move (Player *player, int action, float reward) {
    float malus = 100;
    if (action == 0){
        if (player->Yc > 100) {
            player->Yc -= 100;
        }
        else {
            reward -=malus;
        }
    }
    if (action == 1){
        if (player->Yc < SCREEN_HEIGHT - 100) {
            player->Yc += 100;
        }
        else {
            reward -= malus;
        }
    }
    if (action == 2){
        if (player->Xc > 100) {
            player->Xc -= 100;
        }
        else {
            reward -= malus;
        }
    }
    if (action == 3){
        if (player->Xc < SCREEN_WIDTH - 100) {
            player->Xc += 100;
        }
        else {
            reward -=malus;
        }
    }
    return reward;
}

void update (Player *player, int action, int reward, Player *oldState, float Q[GRID_WIDTH][GRID_HEIGHT][4]) {
    int ox = oldState->Xc;
    int oy = oldState->Yc;

    int x = player->Xc;
    int y = player->Yc;

    // Calculez la valeur Q actuelle pour l'état et l'action actuels
    float oldQ = Q[(ox- 100)/100][(oy- 100)/100][action];


    float maxQ = Q[(ox-100)/100][(oy - 100)/100][0];
    for (int a = 0; a < 4; a++) {
        float qValue = Q[(ox-100)/100][(oy-100)/100][a];
        if (qValue > maxQ) {
            maxQ = qValue;
        }
    }

    //printf("%f", oldQ);

    // Mettez à jour la valeur Q pour l'état et l'action actuels en utilisant la formule Q-learning
    Q[(ox- 100)/100][(oy- 100)/100][action] = oldQ + alpha * (reward + gamma * Q[(ox-100)/100][(oy-100)/100][action]);   // maxQ - oldQ
    //printf("oldQ : %f\nalpha : %f\n reward : %f\n maxQ : %f\n, Q : %f\n\n\n", oldQ, alpha, reward, maxQ, Q);
    // Mettez à jour la position de l'agent avec le nouvel état
    oldState->Xc = player->Xc;
    oldState->Yc = player->Yc;
}

float calculateReward(Player player, Player oldplayer, int destination_x, int destination_y) {
    // Calculez la récompense en fonction de la distance entre la position actuelle de l'agent et les coordonnées cibles
    float distance = fdistance(player.Xc, destination_x, player.Yc, destination_y);
    float olddistance = fdistance(oldplayer.Xc, destination_x, oldplayer.Yc, destination_y);
    if (distance == olddistance) {
        return -10;
    }
    else {
        return (olddistance - distance)*10; // Plus la distance est courte, meilleure est la récompense 1/(distance + 1.0)*10000+ distance - newdistance    (olddistance - distance)*100
    }
}


#endif //RENFORCEMENT_LEARNING_ANNEXE_H
