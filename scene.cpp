#include "scene.h"
#include <allegro5/allegro_image.h>

ALLEGRO_FONT *font = NULL;
ALLEGRO_BITMAP *background = NULL;
ALLEGRO_BITMAP *help = NULL;

int checkkk = 0;
// function of menu
void menu_init(){
    al_init_image_addon();
    //font = al_load_ttf_font("./font/pirulen.ttf",12,0);
    background = al_load_bitmap("./image/menu.png");
    help = al_load_bitmap("./image/help.png");
}
void menu_process(ALLEGRO_EVENT event){
    if( event.type == ALLEGRO_EVENT_KEY_UP ) {
        if( event.keyboard.keycode == ALLEGRO_KEY_ENTER ) {
            judge_next_window = 2;
        } else if ( event.keyboard.keycode == ALLEGRO_KEY_SPACE ) {
            checkkk = 1;
        } else if ( event.keyboard.keycode == ALLEGRO_KEY_BACKSPACE ) {
            checkkk = 0;
        }

    }
}
void menu_draw(){
    //al_clear_to_color(al_map_rgb(100,100,100));
    //al_draw_text(font, al_map_rgb(255,255,255), WIDTH/2, HEIGHT/2+220 , ALLEGRO_ALIGN_CENTRE, "Press 'Enter' to start");
    //al_draw_rectangle(WIDTH/2-150, 510, WIDTH/2+150, 550, al_map_rgb(255, 255, 255), 0);
    if (checkkk == 0) al_draw_bitmap(background, 0, 0, 0);
    else al_draw_bitmap(help, 0, 0, 0);
}
void menu_destroy(){
    //al_destroy_font(font);
    al_destroy_bitmap(background);
    al_destroy_bitmap(help);
}

// function of game_scene
void game_scene_init(){
    character_init();
    boss_init();
    minion_init();
    minion1_init();
    minion2_init();
    equip_init();
    background = al_load_bitmap("./image/stage.png");
    //al_clear_to_color(al_map_rgb(0,0,0));
}
void game_scene_draw(){
    al_draw_bitmap(background, 0, 0, 0);
    //al_clear_to_color(al_map_rgb(0,0,0));
    character_draw();
    boss_draw();
    minion_draw();
    minion1_draw();
    minion2_draw();
    equip_draw();
}
void game_scene_destroy(){
    al_destroy_bitmap(background);
    character_destory();
    boss_destory();
    minion_destory();
    minion1_destory();
    minion2_destory();
    equip_destory();
}
