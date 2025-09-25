#include "annexe.h"


int main() {
    ALLEGRO_DISPLAY *display = NULL;
    ALLEGRO_EVENT_QUEUE *event_queue = NULL;
    ALLEGRO_TIMER *timer = NULL;

    if (!al_init()) {
        fprintf(stderr, "Erreur lors de l'initialisation d'Allegro.\n");
        return -1;
    }
    if (!al_init_image_addon()) {
        fprintf(stderr, "Erreur lors de l'initialisation image d'Allegro.\n");
        return -1;
    }

    display = al_create_display(SCREEN_WIDTH, SCREEN_HEIGHT);
    if (!display) {
        fprintf(stderr, "Erreur lors de la création de la fenêtre.\n");
        return -1;
    }


    al_init_primitives_addon();
    al_install_keyboard();
    //al_init_ttf_addon();
    al_init_font_addon();

    event_queue = al_create_event_queue();
    ALLEGRO_FONT *font =  al_create_builtin_font();                       //al_load_ttf_font("BruceForeverRegular.ttf" , 5, ALLEGRO_ALIGN_CENTER);
    timer = al_create_timer(1.0/20); // 60 FPS

    al_register_event_source(event_queue, al_get_keyboard_event_source());
    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_timer_event_source(timer));

    al_clear_to_color(al_map_rgb(0, 0, 0)); // Rectangle blanc
    al_set_target_backbuffer(display);



    al_start_timer(timer);

    srand(time(NULL));

    int fini = 0;

    int episode = 0;
    int max_episodes = 1;

    float Q[GRID_WIDTH][GRID_HEIGHT][4];
    for (int i = 0; i < GRID_WIDTH; ++i) {
        for (int j = 0; j < GRID_HEIGHT; ++j) {
            for (int k = 0; k < 4; ++k) {
                Q[i][j][k] = 0.0;
            }
        }
    }

    int reward =0;

    Player player;
    Player oldplayer;

    int oldaction = 0;

    int destination_x = 100 * (rand() % 9 + 1);
    int destination_y = 100 * (rand() % 9 + 1);

    float exploration_prob = 0.1; // Probabilité d'exploration (ajustez selon vos besoins)


    initAgent(&player, &oldplayer);

    while (!fini) {
        ALLEGRO_EVENT ev;
        al_wait_for_event(event_queue, &ev);

        if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            fini = 1;
        }

        else if (ev.type == ALLEGRO_EVENT_KEY_DOWN) {
            if (ev.keyboard.keycode == ALLEGRO_KEY_UP){
                if (player.Yc > 100) {
                    player.Yc -= 100;
                }
            }
            if (ev.keyboard.keycode == ALLEGRO_KEY_DOWN){
                if (player.Yc < SCREEN_HEIGHT - 100) {
                    player.Yc += 100;
                }
            }
            if (ev.keyboard.keycode == ALLEGRO_KEY_LEFT){
                if (player.Xc > 100) {
                    player.Xc -= 100;
                }
            }
            if (ev.keyboard.keycode == ALLEGRO_KEY_RIGHT){
                if (player.Xc < SCREEN_WIDTH - 100) {
                    player.Xc += 100;
                }
            }
        }
        else if (ev.type == ALLEGRO_EVENT_TIMER) {
            al_clear_to_color(al_map_rgb(100, 150, 100));


            for (int i = 0; i < SCREEN_WIDTH/10; ++i) {
                al_draw_line(0 + i * 100, 0, 0 + i * 100, SCREEN_HEIGHT, al_map_rgb(100, 200, 100), 2);
            }

            for (int j = 0; j < SCREEN_HEIGHT/10; ++j) {
                al_draw_line(0, 0 + j * 100, SCREEN_WIDTH, 0 + j * 100, al_map_rgb(100, 200, 100), 2);
            }

            al_draw_filled_rectangle(destination_x - 10, destination_y - 10, destination_x + 10, destination_y + 10, al_map_rgb(200, 100, 100));


            al_draw_filled_rectangle(player.Xc - 10, player.Yc - 10, player.Xc + 10, player.Yc + 10, al_map_rgb(100, 100, 200));


            for (int i = 0; i < 1; ++i) {//while (episode < max_episodes)
                // Obtenir l'action de l'agent (à partir d'un algorithme d'apprentissage par renforcement)
                int action = get_Action(&player, Q, exploration_prob, oldaction);



                float reward_mur = move(&player, action, reward);

                float reward = calculateReward(player, oldplayer, destination_x, destination_y) + reward_mur;

                //printf("player X : %d\n player Y : %d\n\n newplayer X : %d\n newplayer Y : %d\n\n\n\n", player.Xc, player.Yc, oldplayer.Xc, oldplayer.Yc);

                //printf("%f\n", Q);
                // Mettre à jour l'agent (à partir de l'algorithme d'apprentissage par renforcement)
                update(&player, action, reward, &oldplayer, Q);
                oldaction = action;
                // Gérer les événements Allegro (assurez-vous que l'agent ne bouge pas trop rapidement)
                // ...
                // Incrémenter le compteur d'épisodes si nécessaire
                if (player.Xc == destination_x && player.Yc == destination_y) {
                    episode++;
                    initAgent(&player, &oldplayer); // Réinitialiser l'agent pour le prochain épisode
                }

                /*if (episode <= max_episodes) {
                    exploration_prob = 0.1;
                }
                if (episode >= max_episodes) {
                    exploration_prob = 0.05;
                }
                if (reward <= 0) {
                    al_draw_filled_rectangle(0, 0, 50, 50, al_map_rgb(200, 100, 100));
                }*/

                char chaine3 [20];
                char chaine2 [20];

                sprintf(chaine3, "%d", action);
                sprintf(chaine2, "%f", reward);

                char *text1 = "action :";
                char *text2 = chaine3;

                char *text3 = "reward";
                char *text4 = chaine2;

                al_draw_text(font, al_map_rgb(0, 0, 0), 50, 89, ALLEGRO_ALIGN_CENTER, text1);
                al_draw_text(font, al_map_rgb(0, 0, 0), 50, 100, ALLEGRO_ALIGN_CENTER, text2);

                al_draw_text(font, al_map_rgb(0, 0, 0), 50, 189, ALLEGRO_ALIGN_CENTER, text3);
                al_draw_text(font, al_map_rgb(0, 0, 0), 50, 200, ALLEGRO_ALIGN_CENTER, text4);



                //printf("action : %d\n", action);

                if (player.Xc == 900 && player.Yc == 100 && action == 1){
                    //printf("reward : %f\n", reward);
                }

                for (int i = 0; i < GRID_WIDTH; ++i) {
                    for (int j = 0; j < GRID_HEIGHT; ++j) {
                        for (int k = 0; k < 4; ++k) {

                            //printf("Q [%d][%d][%d] : %f, ", i, j, k, Q[i][j][k]);
                            char chaine [20];
                            sprintf(chaine, "%f", round(Q [i][j][k]));
                            char *text_t = chaine;
                            if (k == 0) {
                                al_draw_text(font, al_map_rgb(0, 0, 0), 100 * i + 100, 100 * j + 100 - 25, ALLEGRO_ALIGN_CENTER, text_t);
                            }
                            if (k == 1) {
                                al_draw_text(font, al_map_rgb(0, 0, 0), 100 * i + 100, 100 * j + 100 + 25, ALLEGRO_ALIGN_CENTER, text_t);
                            }
                            if (k == 2) {
                                al_draw_text(font, al_map_rgb(0, 0, 0), 100 * i + 100 - 50, 100 * j + 100 + 10, ALLEGRO_ALIGN_CENTER, text_t);
                            }
                            if (k == 3) {
                                al_draw_text(font, al_map_rgb(0, 0, 0), 100 * i + 100 + 50, 100 * j + 100 - 10, ALLEGRO_ALIGN_CENTER, text_t);
                            }

                        }
                    }
                }

                /*printf("explo : %f\n", exploration_prob);
                for (int j = 0; j < 4; ++j) {
                    //printf("7 : %f\n", Q [8][7][j]);
                    printf("i : %d    Q : %f\n", j, Q [8][8][j]);
                }
                printf("reward : %f \n", reward);*/

            }
            al_flip_display();
        }
    }

    al_destroy_event_queue(event_queue);
    al_destroy_display(display);
    al_destroy_timer(timer);
    return 0;
}
