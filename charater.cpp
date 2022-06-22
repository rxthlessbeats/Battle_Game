#include "charater.h"
#include <math.h>

// the state of character
enum {STOP = 0, MOVE, FLASH, ATK, ATK1, ATK2, DEAD, POT, NO, RESULT};
typedef struct character
{
    int x, y; // the position of image
    int width, height; // the width and height of image
    bool dir; // left: false, right: true
    int state; // the state of character
    ALLEGRO_BITMAP *img_move[8];
    ALLEGRO_BITMAP *img_atk[6];
    ALLEGRO_BITMAP *img_atk2[8];
    ALLEGRO_BITMAP *img_sword[1];
    ALLEGRO_BITMAP *img_dead[3];
    ALLEGRO_SAMPLE_INSTANCE *atk_Sound;
    ALLEGRO_SAMPLE_INSTANCE *atk3_Sound;
    ALLEGRO_SAMPLE_INSTANCE *flash_Sound;
    ALLEGRO_SAMPLE_INSTANCE *death_Sound; //minion death
    ALLEGRO_SAMPLE_INSTANCE *hurt_Sound; //chara hurt
    ALLEGRO_SAMPLE_INSTANCE *pick_Sound; // pick
    ALLEGRO_SAMPLE_INSTANCE *drink_Sound; //drink water
    int anime; // counting the time of animation
    int anime_time; // indicate how long the animation
    int countt;
    int blood;
    int sword_count1, sword_count2, sword_count_boss;
    int dead_check;
} Character;

typedef struct bosss
{
    int x, y; // the position of image
    int width, height; // the width and height of image
    bool dir; // left: false, right: true
    int state; // the state of character
    ALLEGRO_BITMAP *img_move[1];
    ALLEGRO_BITMAP *img_fire[1];
    ALLEGRO_BITMAP *img_atk1[9];
    ALLEGRO_SAMPLE_INSTANCE *atk_Sound;
    ALLEGRO_SAMPLE_INSTANCE *atk2_Sound;
    int anime; // counting the time of animation
    int anime_time; // indicate how long the animation
    int updown = 0;
    int countt = 2;
    int blood;
}Boss;

typedef struct little
{
    int x, y;
    int state; // the state of character
    ALLEGRO_BITMAP *img[4];
    ALLEGRO_BITMAP *shield[1];
    ALLEGRO_BITMAP *flash[1];
    int anime;
    int anime_time;
} Equip;

Character chara;
ALLEGRO_SAMPLE *sample = NULL; //atk
ALLEGRO_SAMPLE *sample2 = NULL; //atk3
ALLEGRO_SAMPLE *sample3 = NULL; //flash
ALLEGRO_SAMPLE *sample4 = NULL; //chara hurt
ALLEGRO_SAMPLE *sample5 = NULL; //pick
ALLEGRO_SAMPLE *sample6 = NULL; //drink water
Character minion;
ALLEGRO_SAMPLE *minion_sample = NULL; //death_sound
Character minion1;
ALLEGRO_SAMPLE *minion1_sample = NULL;
Character minion2;
ALLEGRO_SAMPLE *minion2_sample = NULL;
Boss boss;
ALLEGRO_SAMPLE *boss_sample = NULL; //fire
ALLEGRO_SAMPLE *boss_sample2 = NULL; //fireball
ALLEGRO_SAMPLE *boss_sample3 = NULL; //hurt
Equip equip;
int fire_x;
int sword_x;
int equipment = 0;
int alive = 3, cut = 5;
int music = 0;

bool chara_alive = true;
bool boss_alive = true;
ALLEGRO_BITMAP *result[4];
ALLEGRO_SAMPLE *win_sample = NULL; //hurt
ALLEGRO_SAMPLE *lose_sample = NULL; //hurt
ALLEGRO_SAMPLE_INSTANCE *win_sample1;
ALLEGRO_SAMPLE_INSTANCE *lose_sample1;

void character_init(){
    // load character images
    char temp[50];
    sprintf( temp, "./image/sword.png", 0 );
    chara.img_sword[0] = al_load_bitmap(temp);
    for(int i = 0 ; i <= 7 ; i++){
        char temp[50];
        sprintf( temp, "./image/run/HeroKnight_Run_%d.png", i );
        chara.img_move[i] = al_load_bitmap(temp);
    }
    for(int i = 0 ; i <= 5 ; i++){
        char temp[50];
        sprintf( temp, "./image/attack1/HeroKnight_Attack1_%d.png", i );
        chara.img_atk[i] = al_load_bitmap(temp);
    }
    for(int i = 0 ; i <= 7 ; i++){
        char temp[50];
        sprintf( temp, "./image/attack3/HeroKnight_Attack3_%d.png", i );
        chara.img_atk2[i] = al_load_bitmap(temp);
    }
    for(int i = 0 ; i <= 3 ; i++){
        char temp[50];
        sprintf( temp, "./image/result_%d.png", i );
        result[i] = al_load_bitmap(temp);
    }
    // load effective sound
    sample = al_load_sample("./sound/atk_sound.wav");
    chara.atk_Sound  = al_create_sample_instance(sample);
    al_set_sample_instance_playmode(chara.atk_Sound, ALLEGRO_PLAYMODE_ONCE);
    al_attach_sample_instance_to_mixer(chara.atk_Sound, al_get_default_mixer());

    sample2 = al_load_sample("./sound/attack3.wav");
    chara.atk3_Sound  = al_create_sample_instance(sample2);
    al_set_sample_instance_playmode(chara.atk3_Sound, ALLEGRO_PLAYMODE_ONCE);
    al_attach_sample_instance_to_mixer(chara.atk3_Sound, al_get_default_mixer());

    sample3 = al_load_sample("./sound/flash.wav");
    chara.flash_Sound  = al_create_sample_instance(sample3);
    al_set_sample_instance_playmode(chara.flash_Sound, ALLEGRO_PLAYMODE_ONCE);
    al_attach_sample_instance_to_mixer(chara.flash_Sound, al_get_default_mixer());

    sample4 = al_load_sample("./sound/character_hurt.wav");
    chara.hurt_Sound  = al_create_sample_instance(sample4);
    al_set_sample_instance_playmode(chara.hurt_Sound, ALLEGRO_PLAYMODE_ONCE);
    al_attach_sample_instance_to_mixer(chara.hurt_Sound, al_get_default_mixer());

    sample5 = al_load_sample("./sound/pick.wav");
    chara.pick_Sound  = al_create_sample_instance(sample5);
    al_set_sample_instance_playmode(chara.pick_Sound, ALLEGRO_PLAYMODE_ONCE);
    al_attach_sample_instance_to_mixer(chara.pick_Sound, al_get_default_mixer());

    sample6 = al_load_sample("./sound/water.wav");
    chara.drink_Sound  = al_create_sample_instance(sample6);
    al_set_sample_instance_playmode(chara.drink_Sound, ALLEGRO_PLAYMODE_ONCE);
    al_attach_sample_instance_to_mixer(chara.drink_Sound, al_get_default_mixer());

    lose_sample = al_load_sample("./sound/defeat.mp3");
    lose_sample1  = al_create_sample_instance(lose_sample);
    al_set_sample_instance_playmode(lose_sample1, ALLEGRO_PLAYMODE_ONCE);
    al_attach_sample_instance_to_mixer(lose_sample1, al_get_default_mixer());

    win_sample = al_load_sample("./sound/victory.wav");
    win_sample1  = al_create_sample_instance(win_sample);
    al_set_sample_instance_playmode(win_sample1, ALLEGRO_PLAYMODE_ONCE);
    al_attach_sample_instance_to_mixer(win_sample1, al_get_default_mixer());
    // initial the geometric information of character
    chara.width = al_get_bitmap_width(chara.img_move[0]);
    chara.height = al_get_bitmap_height(chara.img_move[0]);
    chara.x = WIDTH/2;
    chara.y = HEIGHT/2;
    chara.dir = false;

    // initial the animation component
    chara.state = STOP;
    chara.anime = 0;
    chara.anime_time = 48;
    chara.countt = 10;
    chara.blood = 6;
}

void charater_process(ALLEGRO_EVENT event){
    // process the animation
    if( event.type == ALLEGRO_EVENT_TIMER ){
        if( event.timer.source == fps ){
            chara.anime++;
            chara.anime %= chara.anime_time;
            if (chara.anime == 1) chara.countt += 1;
        }
    // process the keyboard event
    }else if( event.type == ALLEGRO_EVENT_KEY_DOWN ){
        key_state[event.keyboard.keycode] = true;
        chara.anime = 0;
    }else if( event.type == ALLEGRO_EVENT_KEY_UP ){
        key_state[event.keyboard.keycode] = false;
    }
    if (chara.blood <= 0) chara_alive = false;
    if (boss.blood <= 0) boss_alive = false;
}

void charater_update(){
    // use the idea of finite state machine to deal with different state
    if (chara_alive == true && boss_alive == true) {
        if( key_state[ALLEGRO_KEY_W] ){
            chara.y -= 5;
            chara.state = MOVE;
        }else if( key_state[ALLEGRO_KEY_A] ){
            chara.dir = false;
            chara.x -= 5;
            chara.state = MOVE;
        }else if( key_state[ALLEGRO_KEY_S] ){
            chara.y += 5;
            chara.state = MOVE;
        }else if( key_state[ALLEGRO_KEY_D] ){
            chara.dir = true;
            chara.x += 5;
            chara.state = MOVE;
        }else if (key_state[ALLEGRO_KEY_F]) {
            if (chara.countt >= 10) {
                chara.countt = 0;
                chara.state = FLASH;
            }
        }else if( key_state[ALLEGRO_KEY_SPACE] ){
            chara.state = ATK;
        }else if( key_state[ALLEGRO_KEY_C] ){
            chara.state = ATK2;

        }
        else if( chara.anime == chara.anime_time-1 ){
            chara.anime = 0;
            chara.state = STOP;
        }else if ( chara.anime == 0 ){
            chara.state = STOP;
        }
    } else {
        chara.state = RESULT;
    }
}

void character_draw(){
    // with the state, draw corresponding image
    if( chara.state == STOP ){
        if( chara.dir )
            al_draw_bitmap(chara.img_move[0], chara.x, chara.y, ALLEGRO_FLIP_HORIZONTAL);
        else
            al_draw_bitmap(chara.img_move[0], chara.x, chara.y, 0);
    }else if( chara.state == MOVE ){
        if( chara.dir ){
            if( chara.anime < chara.anime_time){
                al_draw_bitmap(chara.img_move[chara.anime/6], chara.x, chara.y, ALLEGRO_FLIP_HORIZONTAL);
            }else{
                // al_draw_bitmap(chara.img_move[1], chara.x, chara.y, ALLEGRO_FLIP_HORIZONTAL);
            }
        }else{
            if( chara.anime < chara.anime_time ){
                al_draw_bitmap(chara.img_move[chara.anime/6], chara.x, chara.y, 0);
            }else{
                //al_draw_bitmap(chara.img_move[1], chara.x, chara.y, 0);
            }
        }
    }else if ( chara.state == FLASH ) {
        al_play_sample_instance(chara.flash_Sound);
        if( key_state[ALLEGRO_KEY_I]) {
            chara.y -= 20;
            if (chara.anime == 5) al_draw_bitmap(chara.img_atk[0], chara.x, chara.y, ALLEGRO_FLIP_HORIZONTAL);
        } else if( key_state[ALLEGRO_KEY_K]) {
            chara.y += 20;
            if (chara.anime == 5) al_draw_bitmap(chara.img_atk[0], chara.x, chara.y, ALLEGRO_FLIP_HORIZONTAL);
        } else if( key_state[ALLEGRO_KEY_J]) {
            chara.x -= 20;
            if (chara.anime == 5) al_draw_bitmap(chara.img_atk[0], chara.x, chara.y, ALLEGRO_FLIP_HORIZONTAL);
        } else if( key_state[ALLEGRO_KEY_L]) {
            chara.x += 20;
            if (chara.anime == 5) al_draw_bitmap(chara.img_atk[0], chara.x, chara.y, ALLEGRO_FLIP_HORIZONTAL);
        }
    }else if( chara.state == ATK ){
        if( chara.dir ){
            if( chara.anime < chara.anime_time ){
                // al_draw_bitmap(chara.img_atk[0], chara.x, chara.y, ALLEGRO_FLIP_HORIZONTAL);
                al_draw_bitmap(chara.img_atk[chara.anime/8], chara.x, chara.y, ALLEGRO_FLIP_HORIZONTAL);
                al_play_sample_instance(chara.atk_Sound);

                //boss
                if ((chara.x -50 <= boss.x + boss.width) && (chara.x >= boss.x)) {
                    if ((chara.y - 15 <= boss.y + boss.height) && (chara.y + chara.height - 100 >= boss.y)) {
                        if (chara.anime == 5) {
                            if (alive == 0) {
                                boss.blood -= 1;
                                // printf("%d\n", boss.blood);
                            } else {
                                if (cut == 0) {
                                    boss.blood -= 1;
                                    cut = 5;
                                } else cut -= 1;
                            }
                        }
                    }
                }
                //printf("%d %d %d\n", chara.x - 100, chara.x + chara.width, minion.x);
                //minion
                if ((chara.x -100 <= minion.x) && (chara.x + chara.width >= minion.x)) {
                    if ((chara.y - 15 <= minion.y) && (chara.y + chara.height >= minion.y)) {
                        if (chara.anime == 5) {
                            minion.blood -= 1;
                            //printf("1: %d\n", minion.blood);
                        }
                    }
                }
                if ((chara.x -100 <= minion1.x) && (chara.x + chara.width >= minion1.x)) {
                    if ((chara.y - 15 <= minion1.y) && (chara.y + chara.height >= minion1.y)) {
                        if (chara.anime == 5) {
                            minion1.blood -= 1;
                            //printf("2: %d\n", minion1.blood);
                        }
                    }
                }
                if ((chara.x -100 <= minion2.x) && (chara.x + chara.width >= minion2.x)) {
                    if ((chara.y - 15 <= minion2.y) && (chara.y + chara.height >= minion2.y)) {
                        if (chara.anime == 5) {
                            minion2.blood -= 1;
                            //printf("2: %d\n", minion1.blood);
                        }
                    }
                }
            }else{
                // al_draw_bitmap(chara.img_atk[9], chara.x, chara.y, ALLEGRO_FLIP_HORIZONTAL);

            }
        }else{
            if( chara.anime < chara.anime_time ) al_draw_bitmap(chara.img_atk[chara.anime/8], chara.x, chara.y, 0);
            al_play_sample_instance(chara.atk_Sound);

                //boss
                //printf("%d %d %d %d\n", chara.y - 15, boss.y + boss.width, chara.y + chara.height, boss.y);
                if ((chara.x -50 <= boss.x + boss.width) && (chara.x >= boss.x)) {
                    if ((chara.y - 15 <= boss.y + boss.height) && (chara.y + chara.height - 100 >= boss.y)) {
                        if (chara.anime == 5) {
                            if (alive == 0) {
                                boss.blood -= 1;
                            } else {
                                if (cut == 0) {
                                    boss.blood -= 1;
                                    cut = 5;
                                } else cut -= 1;
                            }
                        }
                    }
                }
                //minion
                if ((chara.x -100 <= minion.x) && (chara.x + chara.width >= minion.x)) {
                    if ((chara.y - 15 <= minion.y) && (chara.y + chara.height >= minion.y)) {
                        if (chara.anime == 5) {
                            minion.blood -= 1;
                            //printf("1: %d\n", minion.blood);
                        }
                    }
                }
                if ((chara.x -100 <= minion1.x) && (chara.x + chara.width >= minion1.x)) {
                    if ((chara.y - 15 <= minion1.y) && (chara.y + chara.height >= minion1.y)) {
                        if (chara.anime == 5) {
                            minion1.blood -= 1;
                            //printf("2: %d\n", minion1.blood);
                        }
                    }
                }
                if ((chara.x -100 <= minion2.x) && (chara.x + chara.width >= minion2.x)) {
                    if ((chara.y - 15 <= minion2.y) && (chara.y + chara.height >= minion2.y)) {
                        if (chara.anime == 5) {
                            minion2.blood -= 1;
                            //printf("2: %d\n", minion1.blood);
                        }
                    }
                }
            /* if( chara.anime < chara.anime_time/2 ){
                al_draw_bitmap(chara.img_atk[chara.anime], chara.x, chara.y, 0);
            }else{
                // al_draw_bitmap(chara.img_atk[1], chara.x, chara.y, 0);
                al_play_sample_instance(chara.atk_Sound);
            }*/
        }
    }else if( chara.state == ATK2 ){
        if (chara.anime == 1) {
            sword_x = chara.x;
            chara.sword_count1 = 1;
            chara.sword_count2 = 1;
            chara.sword_count_boss = 1;
        }
        if( chara.dir ){
            if( chara.anime < chara.anime_time ){
                al_draw_bitmap(chara.img_atk2[chara.anime/6], chara.x, chara.y, ALLEGRO_FLIP_HORIZONTAL);
                al_draw_bitmap(chara.img_sword[0], sword_x, chara.y, ALLEGRO_FLIP_HORIZONTAL);
                al_play_sample_instance(chara.atk3_Sound);
                sword_x += 20;

                //printf("%d %d %d %d\n",sword_x , minion.x, chara.x, minion.x);
                if (sword_x >= minion.x && chara.x - 4 <= minion.x) {
                    if ((chara.y - 15 <= minion.y) && (chara.y + chara.height >= minion.y)) {
                        if (chara.sword_count1 == 1) {
                            minion.blood -= 1;
                            chara.sword_count1 = 0;
                            //printf("1: %d\n", minion.blood);
                        }
                    }
                }
                if (sword_x >= minion1.x && chara.x - 4 <= minion1.x) {
                    if ((chara.y - 15 <= minion1.y) && (chara.y + chara.height >= minion1.y)) {
                        if (chara.sword_count2 == 1) {
                            minion1.blood -= 1;
                            chara.sword_count2 = 0;
                            //printf("2: %d\n", minion1.blood);
                        }
                    }
                }
                if (sword_x >= minion2.x && chara.x - 4 <= minion2.x) {
                    if ((chara.y - 15 <= minion2.y) && (chara.y + chara.height >= minion2.y)) {
                        if (chara.sword_count2 == 1) {
                            minion2.blood -= 1;
                            chara.sword_count2 = 0;
                            //printf("2: %d\n", minion1.blood);
                        }
                    }
                }
                //al_play_sample_instance(chara.atk_Sound);
            }
            else{
                // al_draw_bitmap(chara.img_atk2[1], chara.x, chara.y, ALLEGRO_FLIP_HORIZONTAL);
                //al_play_sample_instance(chara.atk_Sound);
            }
        }else{
            if( chara.anime < chara.anime_time ) {
                al_draw_bitmap(chara.img_atk2[chara.anime/6], chara.x, chara.y, 0);
                al_draw_bitmap(chara.img_sword[0], sword_x, chara.y, 0);
                al_play_sample_instance(chara.atk3_Sound);
                sword_x -= 20;

                //boss
                if (sword_x  <= boss.x && chara.x >= boss.x) {
                    if ((chara.y + 10 <= boss.y + boss.height) && (chara.y + chara.height - 120 >= boss.y)) {
                        if (chara.sword_count_boss == 1) {
                            if (alive == 0) {
                                boss.blood -= 1;
                                printf("555555\n");
                                chara.sword_count_boss = 0;
                            } else {
                                if (cut == 0) {
                                    boss.blood -= 1;
                                    printf("6666666\n");
                                    chara.sword_count_boss = 0;
                                    cut =5;
                                } else cut -= 1;
                            }
                            chara.sword_count_boss = 0;
                        }
                    }
                }
                //minion
                if (sword_x  <= minion.x && chara.x >= minion.x) {
                    if ((chara.y - 15 <= minion.y) && (chara.y + chara.height >= minion.y)) {
                        if (chara.sword_count1 == 1) {
                            minion.blood -= 1;
                            chara.sword_count1 = 0;
                            //printf("1: %d\n", minion.blood);
                        }
                    }
                }
                if (sword_x  <= minion1.x && chara.x >= minion1.x) {
                    if ((chara.y - 15 <= minion1.y) && (chara.y + chara.height >= minion1.y)) {
                        if (chara.sword_count2 == 1) {
                            minion1.blood -= 1;
                            chara.sword_count2 = 0;
                            //printf("2: %d\n", minion1.blood);
                        }
                    }
                }
                if (sword_x  <= minion2.x && chara.x >= minion2.x) {
                    if ((chara.y - 15 <= minion2.y) && (chara.y + chara.height >= minion2.y)) {
                        if (chara.sword_count2 == 1) {
                            minion2.blood -= 1;
                            chara.sword_count2 = 0;
                            //printf("2: %d\n", minion1.blood);
                        }
                    }
                }
                //al_play_sample_instance(chara.atk_Sound);
            }
            //else{
                // al_draw_bitmap(chara.img_atk2[1], chara.x, chara.y, 0);
                //al_play_sample_instance(chara.atk_Sound);
            //}
        }
    } else if ( chara.state == RESULT) {
        if (chara_alive == false) {
            if (chara.countt >= 16 && chara.countt <= 22) al_draw_bitmap(result[3], 0, 0, 0);
            else al_draw_bitmap(result[2], 0, 0, 0);
            if (music == 0) {
                music += 1;
                chara.countt = 0;
                al_play_sample_instance(lose_sample1);
            }
        }
        else {
            if (chara.countt >= 12 && chara.countt <= 44) al_draw_bitmap(result[1], 0, 0, 0);
            else al_draw_bitmap(result[0], 0, 0, 0);
            if (music == 0) {
                music += 1;
                chara.countt = 0;
                al_play_sample_instance(win_sample1);
            }
        }
    }
}
void character_destory(){
    for (int i = 0; i < 6; i++) al_destroy_bitmap(chara.img_atk[i]);
    for (int i = 0; i < 8; i++) al_destroy_bitmap(chara.img_atk2[i]);
    for (int i = 0; i < 10; i++) al_destroy_bitmap(chara.img_move[i]);
    for (int i = 0; i < 4; i++) al_destroy_bitmap(result[i]);
    al_destroy_sample_instance(chara.atk_Sound);
    al_destroy_sample_instance(chara.atk3_Sound);
    al_destroy_sample_instance(chara.flash_Sound);
    al_destroy_sample_instance(chara.hurt_Sound);
    al_destroy_sample_instance(chara.pick_Sound);
    al_destroy_sample_instance(chara.drink_Sound);
}
//============================================================================================================================

void boss_init(){
    // load character images
    char temp[50];
    sprintf( temp, "./image/boss/boss_0.png", 0 );
    boss.img_move[0] = al_load_bitmap(temp);

    sprintf( temp, "./image/fire_ball.png", 0 );
    boss.img_fire[0] = al_load_bitmap(temp);

    for(int i = 0 ; i <= 8 ; i++){
        char temp[50];
        sprintf( temp, "./image/boss/boss_%d.png", i );
        boss.img_atk1[i] = al_load_bitmap(temp);
    }
    // load effective sound
    boss_sample = al_load_sample("./sound/kou_dong_fire.wav");
    boss.atk_Sound  = al_create_sample_instance(boss_sample);
    al_set_sample_instance_playmode(boss.atk_Sound, ALLEGRO_PLAYMODE_ONCE);
    al_attach_sample_instance_to_mixer(boss.atk_Sound, al_get_default_mixer());

    boss_sample2 = al_load_sample("./sound/kou_dong_fireball.wav");
    boss.atk2_Sound  = al_create_sample_instance(boss_sample2);
    al_set_sample_instance_playmode(boss.atk2_Sound, ALLEGRO_PLAYMODE_ONCE);
    al_attach_sample_instance_to_mixer(boss.atk2_Sound, al_get_default_mixer());

    // initial the geometric information of character
    boss.width = al_get_bitmap_width(boss.img_move[0]);
    boss.height = al_get_bitmap_height(boss.img_move[0]);
    boss.x = WIDTH*0;
    boss.y = HEIGHT/2;
    boss.dir = false;

    // initial the animation component
    boss.state = MOVE;
    boss.anime = 0;
    boss.anime_time = 108;
    boss.blood = 7;
}

void boss_process(ALLEGRO_EVENT event){
    // process the animation
    if( event.type == ALLEGRO_EVENT_TIMER ){
        if( event.timer.source == fps ){
            boss.anime++;
            boss.anime %= boss.anime_time;
            if (boss.anime == 0) boss.countt += 1;
        }
    // process the keyboard event

    }
}

void boss_update(){
    // use the idea of finite state machine to deal with different state
        //chara.y -= 5;
    //if (boss.anime % 107 == 0) boss.countt += 1;
    if (chara_alive == true && boss_alive == true) {
        if (boss.countt == 5) {
            boss.state = ATK2;
            if (boss.anime == 0) fire_x = boss.x + 80;
        }
        else if (boss.countt == 10) {
            boss.state = ATK2;
            if (boss.anime == 0) fire_x = boss.x + 80;
        }
        else if (boss.countt == 15) {
            boss.countt = 0;
            boss.state = ATK1;
        }
        else if (boss.countt > 0 && boss.countt != 5 && boss.countt != 10) boss.state = MOVE;
    } else boss.state = RESULT;
}

void boss_draw(){
    // with the state, draw corresponding image
    if( boss.state == MOVE ){
        if (boss.updown == 0) {
            boss.y -= 2;
            al_draw_bitmap(boss.img_move[0], boss.x, boss.y, 0);
            if (boss.y <= HEIGHT/5) boss.updown = 1;
        } else {
            boss.y += 2;
            al_draw_bitmap(boss.img_move[0], boss.x, boss.y, 0);
            if (boss.y >= HEIGHT*2/3) boss.updown = 0;
        }
    } else if (boss.state == ATK1) {
        if (boss.anime < boss.anime_time) {
            al_draw_bitmap(boss.img_atk1[boss.anime/12], boss.x, boss.y, 0);
            al_play_sample_instance(boss.atk_Sound);
            //¶Ë®`§P©w
            int width = al_get_bitmap_width(boss.img_move[8]);
            //printf("%d %d %d %d %d\n",chara.x , width, chara.y, boss.y - 100, boss.y + boss.height);
            if (boss.anime == 73) {
                if (chara.x <= width - 50 && chara.x >= boss.width -50) {
                    if (chara.y <= boss.y + boss.height && chara.y + chara.height >= boss.y) {
                        chara.blood -= 3;
                        al_play_sample_instance(chara.hurt_Sound);
                        printf("%d\n", chara.blood);
                    }
                }
            }
        }
    } else if (boss.state == ATK2) {
        al_play_sample_instance(boss.atk2_Sound);
        if (boss.anime < boss.anime_time ) {
            al_draw_bitmap(boss.img_atk1[boss.anime/18], boss.x, boss.y, 0);
        }
        if (boss.anime > boss.anime_time * 6 / 9) {
            fire_x += 32;
            al_draw_bitmap(boss.img_fire[0], fire_x, boss.y, 0);

            if (boss.anime == 107) {
                if (chara.x <= fire_x && chara.x >= boss.width -50) {
                    if (chara.y <= boss.y + boss.height && chara.y + chara.height >= boss.y) {
                        chara.blood -= 2;
                        al_play_sample_instance(chara.hurt_Sound);
                        printf("%d\n", chara.blood);
                    }
                }
            }
        }
    } else if (boss.state == RESULT) int a = 1;
}

void boss_destory(){
    al_destroy_bitmap(boss.img_move[0]);
    al_destroy_bitmap(boss.img_fire[0]);
    for (int i = 0; i < 9; i++) al_destroy_bitmap(boss.img_atk1[i]);
    al_destroy_sample_instance(boss.atk_Sound);
    al_destroy_sample_instance(boss.atk2_Sound);
}
//=================================================================================================================


void minion_init(){
    for(int i = 0 ; i <= 2 ; i++){
        char temp[50];
        sprintf( temp, "./image/dead/dead%d.png", i );
        minion.img_dead[i] = al_load_bitmap(temp);
    }
    for(int i = 0 ; i <= 3 ; i++){
        char temp[50];
        sprintf( temp, "./image/minion/minion_%d.png", i );
        minion.img_move[i] = al_load_bitmap(temp);
    }
    for(int i = 0 ; i <= 3 ; i++){
        char temp[50];
        sprintf( temp, "./image/minion_attack/minion_attack_%d.png", i );
        minion.img_atk[i] = al_load_bitmap(temp);
    }

    minion_sample = al_load_sample("./sound/minion_death.wav");
    minion.death_Sound  = al_create_sample_instance(minion_sample);
    al_set_sample_instance_playmode(minion.death_Sound, ALLEGRO_PLAYMODE_ONCE);
    al_attach_sample_instance_to_mixer(minion.death_Sound, al_get_default_mixer());

    minion.width = al_get_bitmap_width(minion.img_move[0]);
    minion.height = al_get_bitmap_height(minion.img_move[0]);
    minion.x = WIDTH*0;
    minion.y = HEIGHT*3/4;
    minion.dir = false;

    minion.state = STOP;
    minion.anime = 0;
    minion.anime_time = 48;
    minion.countt = 0;
    minion.blood = 4;
    minion.dead_check = 1;
    minion.sword_count1 = 0;
}

void minion_process(ALLEGRO_EVENT event){
    // process the animation
    if( event.type == ALLEGRO_EVENT_TIMER ){
        if( event.timer.source == fps ){
            minion.anime++;
            minion.anime %= minion.anime_time;
        }
    // process the keyboard event
    }else if( event.type == ALLEGRO_EVENT_KEY_DOWN ){
        key_state[event.keyboard.keycode] = true;
        minion.anime = 0;
    }else if( event.type == ALLEGRO_EVENT_KEY_UP ){
        key_state[event.keyboard.keycode] = false;
    }
    if (minion.anime == 0) {
        minion.countt += 1;
        minion.sword_count1 += 1;
    }
}
int take_pot = 0, take_equip = 0;
void minion_update(){
    if (chara_alive == true && boss_alive == true) {
        int dx = chara.x - minion.x;
        if (minion.countt < 15) minion.state = STOP;
        else if (minion.blood == 0 && minion.dead_check == 1) {
            printf("%d %d\n", minion.blood, minion.dead_check );
            minion.state = DEAD;
            minion.dead_check = 0;
            alive -= 1;
        }
        else if ((take_pot == 1) || (key_state[ALLEGRO_KEY_G] && minion.dead_check == 0 && dx > -30 && dx < 30)) {
            minion.state = NO;
            take_pot = 1;
            if (take_equip == 0) {
                al_play_sample_instance(chara.pick_Sound);
                equipment += 1;
                take_equip = 1;
            }
        }
        else if (minion.dead_check == 0) minion.state = POT;
        else minion.state = MOVE;
    } else minion.state = RESULT;
}

void minion_draw(){
    if (minion.state == STOP) {
        al_draw_bitmap(minion.img_move[chara.anime/12], minion.x, minion.y, 0);
    } else if (minion.state == DEAD) {
        al_play_sample_instance(minion.death_Sound);
        printf("%d %d\n", minion.blood, minion.dead_check );
        al_draw_bitmap(minion.img_dead[chara.anime/24], minion.x, minion.y, 0);
    } else if (minion.state == POT) {
        al_draw_bitmap(minion.img_dead[2], minion.x, minion.y, 0);
    } else if (minion.state == MOVE) {
        int dx = chara.x - minion.x;
        int dy = chara.y - minion.y;
        if (dx > 15) {
            if (dy > 2) {
                minion.x += 1;
                minion.y += 1;
                al_draw_bitmap(minion.img_move[chara.anime/12], minion.x, minion.y, 0);
            } else {
                minion.x += 1;
                minion.y -= 1;
                al_draw_bitmap(minion.img_move[chara.anime/12], minion.x, minion.y, 0);
            }
        } else if (dx < 15) {
            if (dy > 2) {
                minion.x -= 1;
                minion.y += 1;
                al_draw_bitmap(minion.img_move[chara.anime/12], minion.x, minion.y, ALLEGRO_FLIP_HORIZONTAL);
            } else {
                minion.x -= 1;
                minion.y -= 1;
                al_draw_bitmap(minion.img_move[chara.anime/12], minion.x, minion.y, ALLEGRO_FLIP_HORIZONTAL);
            }
        } else {
            if (dy > 2) {
                minion.y += 1;
                al_draw_bitmap(minion.img_move[chara.anime/12], minion.x, minion.y, 0);
            } else if (dy == 2 && minion.sword_count1 >= 5) {
                al_draw_bitmap(minion.img_atk[chara.anime/12], minion.x, minion.y, 0);
                if (minion.anime == 47) {
                    minion.sword_count1 = 0;
                    chara.blood -= 1;
                    al_play_sample_instance(chara.hurt_Sound);
                    printf("%d\n", chara.blood);
                }
            }
            else {
                minion.y -= 1;
                al_draw_bitmap(minion.img_move[chara.anime/12], minion.x, minion.y, 0);
            }
        }
    } else if (minion.state == RESULT) int a = 1;
    else int a = 1;
}

void minion_destory(){
    for (int i = 0; i < 5; i++) al_destroy_bitmap(minion.img_move[i]);
    for (int i = 0; i < 3; i++) al_destroy_bitmap(minion.img_dead[i]);
    for (int i = 0; i < 5; i++) al_destroy_bitmap(minion.img_atk[i]);
    al_destroy_sample_instance(minion.death_Sound);
}
//=========================================================================

void minion1_init(){
    for(int i = 0 ; i <= 2 ; i++){
        char temp[50];
        sprintf( temp, "./image/dead/dead%d.png", i );
        minion1.img_dead[i] = al_load_bitmap(temp);
    }
    for(int i = 0 ; i <= 3 ; i++){
        char temp[50];
        sprintf( temp, "./image/minion/minion_%d.png", i );
        minion1.img_move[i] = al_load_bitmap(temp);
    }
    for(int i = 0 ; i <= 3 ; i++){
        char temp[50];
        sprintf( temp, "./image/minion_attack/minion_attack_%d.png", i );
        minion1.img_atk[i] = al_load_bitmap(temp);
    }

    minion1_sample = al_load_sample("./sound/minion_death.wav");
    minion1.death_Sound  = al_create_sample_instance(minion1_sample);
    al_set_sample_instance_playmode(minion1.death_Sound, ALLEGRO_PLAYMODE_ONCE);
    al_attach_sample_instance_to_mixer(minion1.death_Sound, al_get_default_mixer());

    minion1.width = al_get_bitmap_width(minion1.img_move[0]);
    minion1.height = al_get_bitmap_height(minion1.img_move[0]);
    minion1.x = WIDTH*5/6;
    minion1.y = HEIGHT/4;
    minion1.dir = false;

    minion1.state = STOP;
    minion1.anime = 0;
    minion1.anime_time = 48;
    minion1.countt = 30;
    minion1.blood = 4;
    minion1.dead_check = 1;
    minion1.sword_count1 = 0;
}

void minion1_process(ALLEGRO_EVENT event){
    // process the animation
    if( event.type == ALLEGRO_EVENT_TIMER ){
        if( event.timer.source == fps ){
            minion1.anime++;
            minion1.anime %= minion1.anime_time;
        }
    // process the keyboard event
    }else if( event.type == ALLEGRO_EVENT_KEY_DOWN ){
        key_state[event.keyboard.keycode] = true;
        minion1.anime = 0;
    }else if( event.type == ALLEGRO_EVENT_KEY_UP ){
        key_state[event.keyboard.keycode] = false;
    }
    if (minion1.anime == 0) {
        minion1.countt += 1;
        minion1.sword_count1 += 1;
    }
}

int take_pot1 = 0, take_equip1 = 0;
void minion1_update(){
    if (chara_alive == true && boss_alive == true) {
        int dx = chara.x - minion1.x;
        if (minion1.countt < 1) minion1.state = STOP;
        else if (minion1.blood == 0 && minion1.dead_check == 1) {
            //printf("%d %d\n", minion1.blood, minion1.dead_check );
            minion1.state = DEAD;
            minion1.dead_check = 0;
            alive -= 1;
        } else if ((take_pot1 == 1) || (key_state[ALLEGRO_KEY_G] && minion1.dead_check == 0 && dx > -30 && dx < 30)) {
            minion1.state = NO;
            take_pot1 = 1;
            if (take_equip1 == 0) {
                al_play_sample_instance(chara.pick_Sound);
                equipment += 1;
                take_equip1 = 1;
            }
        } else if (minion1.dead_check == 0) minion1.state = POT;
        else minion1.state = MOVE;
    } else minion1.state = RESULT;
}

void minion1_draw(){
    if (minion1.state == STOP) {
        al_draw_bitmap(minion1.img_move[chara.anime/12], minion1.x, minion1.y, 0);
    } else if (minion1.state == DEAD) {
        //printf("%d %d\n", minion1.blood, minion1.dead_check );
        al_play_sample_instance(minion.death_Sound);
        al_draw_bitmap(minion1.img_dead[chara.anime/24], minion1.x, minion1.y, 0);
    } else if (minion1.state == POT) {
        al_draw_bitmap(minion1.img_dead[2], minion1.x, minion1.y, 0);
    } else if (minion1.state == MOVE) {
        int dx = chara.x - minion1.x;
        int dy = chara.y - minion1.y;
        if (dx > 3) {
            if (dy > 2) {
                minion1.x += 1;
                minion1.y += 1;
                al_draw_bitmap(minion1.img_move[chara.anime/12], minion1.x, minion1.y, 0);
            } else {
                minion1.x += 1;
                minion1.y -= 1;
                al_draw_bitmap(minion1.img_move[chara.anime/12], minion1.x, minion1.y, 0);
            }
        } else if (dx < 3) {
            if (dy > 2) {
                minion1.x -= 1;
                minion1.y += 1;
                al_draw_bitmap(minion1.img_move[chara.anime/12], minion1.x, minion1.y, ALLEGRO_FLIP_HORIZONTAL);
            } else {
                minion1.x -= 1;
                minion1.y -= 1;
                al_draw_bitmap(minion1.img_move[chara.anime/12], minion1.x, minion1.y, ALLEGRO_FLIP_HORIZONTAL);
            }
        } else {
            if (dy > 2) {
                minion1.y += 1;
                al_draw_bitmap(minion1.img_move[chara.anime/12], minion1.x, minion1.y, 0);
            } else if (dy == 2 && minion1.sword_count1 >= 5) {
                al_draw_bitmap(minion1.img_atk[chara.anime/12], minion1.x, minion1.y, 0);
                if (minion1.anime == 47) {
                    minion1.sword_count1 = 0;
                    chara.blood -= 1;
                    al_play_sample_instance(chara.hurt_Sound);
                    printf("%d\n", chara.blood);
                }
            }
            else {
                minion1.y -= 1;
                al_draw_bitmap(minion1.img_move[chara.anime/12], minion1.x, minion1.y, 0);
            }
        }
    } else if (minion1.state == RESULT) int a = 1;
    else int a = 1;
}

void minion1_destory(){
    for (int i = 0; i < 5; i++) al_destroy_bitmap(minion1.img_move[i]);
    for (int i = 0; i < 3; i++) al_destroy_bitmap(minion1.img_dead[i]);
    for (int i = 0; i < 5; i++) al_destroy_bitmap(minion1.img_atk[i]);
    al_destroy_sample_instance(minion1.death_Sound);
}
//===============================================================================
void minion2_init(){
    for(int i = 0 ; i <= 2 ; i++){
        char temp[50];
        sprintf( temp, "./image/dead/dead%d.png", i );
        minion2.img_dead[i] = al_load_bitmap(temp);
    }
    for(int i = 0 ; i <= 3 ; i++){
        char temp[50];
        sprintf( temp, "./image/minion/minion_%d.png", i );
        minion2.img_move[i] = al_load_bitmap(temp);
    }
    for(int i = 0 ; i <= 3 ; i++){
        char temp[50];
        sprintf( temp, "./image/minion_attack/minion_attack_%d.png", i );
        minion2.img_atk[i] = al_load_bitmap(temp);
    }

    minion2_sample = al_load_sample("./sound/atk_sound.wav");
    minion2.atk_Sound  = al_create_sample_instance(minion2_sample);
    al_set_sample_instance_playmode(minion2.atk_Sound, ALLEGRO_PLAYMODE_ONCE);
    al_attach_sample_instance_to_mixer(minion2.atk_Sound, al_get_default_mixer());

    minion2.width = al_get_bitmap_width(minion2.img_move[0]);
    minion2.height = al_get_bitmap_height(minion2.img_move[0]);
    minion2.x = WIDTH*5/6;
    minion2.y = HEIGHT*3/4;
    minion2.dir = false;

    minion2.state = STOP;
    minion2.anime = 0;
    minion2.anime_time = 48;
    minion2.countt = 0;
    minion2.blood = 4;
    minion2.dead_check = 1;
    minion2.sword_count1 = 0;
}

void minion2_process(ALLEGRO_EVENT event){
    // process the animation
    if( event.type == ALLEGRO_EVENT_TIMER ){
        if( event.timer.source == fps ){
            minion2.anime++;
            minion2.anime %= minion2.anime_time;
        }
    // process the keyboard event
    }else if( event.type == ALLEGRO_EVENT_KEY_DOWN ){
        key_state[event.keyboard.keycode] = true;
        minion.anime = 0;
    }else if( event.type == ALLEGRO_EVENT_KEY_UP ){
        key_state[event.keyboard.keycode] = false;
    }
    if (minion2.anime == 0) {
        minion2.countt += 1;
        minion2.sword_count1 += 1;
    }
}

int take_pot2 = 0, take_equip2 = 0;
void minion2_update() {
    if (chara_alive == true && boss_alive == true) {
        int dx = chara.x - minion2.x;
        if (minion2.countt < 25) minion2.state = STOP;
        else if (minion2.blood == 0 && minion2.dead_check == 1) {
            printf("%d %d\n", minion2.blood, minion2.dead_check );
            minion2.state = DEAD;
            minion2.dead_check = 0;
            alive -= 1;
        } else if ((take_pot2 == 1) || (key_state[ALLEGRO_KEY_G] && minion2.dead_check == 0 && dx > -30 && dx < 30)) {
            minion2.state = NO;
            take_pot2 = 1;
            if (take_equip2 == 0) {
                al_play_sample_instance(chara.pick_Sound);
                equipment += 1;
                take_equip2 = 1;
                printf("%d\n", equipment);
            }
        } else if (minion2.dead_check == 0) minion2.state = POT;
        else minion2.state = MOVE;
    } else minion2.state = RESULT;
}

void minion2_draw() {
    if (minion2.state == STOP) {
        al_draw_bitmap(minion2.img_move[chara.anime/12], minion2.x, minion2.y, 0);
    } else if (minion2.state == DEAD) {
        al_play_sample_instance(minion.death_Sound);
        //printf("%d %d\n", minion.blood, minion2.dead_check );
        al_draw_bitmap(minion2.img_dead[chara.anime/24], minion2.x, minion2.y, 0);
    } else if (minion2.state == POT) {
        al_draw_bitmap(minion2.img_dead[2], minion2.x, minion2.y, 0);
    } else if (minion2.state == MOVE) {
        int dx = chara.x - minion2.x;
        int dy = chara.y - minion2.y;
        if (dx > -10) {
            if (dy > 2) {
                minion2.x += 1;
                minion2.y += 1;
                al_draw_bitmap(minion2.img_move[chara.anime/12], minion2.x, minion2.y, 0);
            } else {
                minion2.x += 1;
                minion2.y -= 1;
                al_draw_bitmap(minion2.img_move[chara.anime/12], minion2.x, minion2.y, 0);
            }
        } else if (dx < -10) {
            if (dy > 2) {
                minion2.x -= 1;
                minion2.y += 1;
                al_draw_bitmap(minion2.img_move[chara.anime/12], minion2.x, minion2.y, ALLEGRO_FLIP_HORIZONTAL);
            } else {
                minion2.x -= 1;
                minion2.y -= 1;
                al_draw_bitmap(minion2.img_move[chara.anime/12], minion2.x, minion2.y, ALLEGRO_FLIP_HORIZONTAL);
            }
        } else {
            if (dy > 2) {
                minion2.y += 1;
                al_draw_bitmap(minion2.img_move[chara.anime/12], minion2.x, minion2.y, 0);
            } else if (dy == 2 && minion2.sword_count1 >= 5) {
                al_draw_bitmap(minion2.img_atk[chara.anime/12], minion2.x, minion2.y, 0);
                if (minion2.anime == 47) {
                    minion2.sword_count1 = 0;
                    chara.blood -= 1;
                    al_play_sample_instance(chara.hurt_Sound);
                    printf("%d\n", chara.blood);
                }
            }
            else {
                minion2.y -= 1;
                al_draw_bitmap(minion2.img_move[chara.anime/12], minion2.x, minion2.y, 0);
            }
        }
    } else if (minion2.state == RESULT) int a = 1;
    else int a = 1;
}

void minion2_destory(){
    for (int i = 0; i < 5; i++) al_destroy_bitmap(minion2.img_move[i]);
    for (int i = 0; i < 3; i++) al_destroy_bitmap(minion2.img_dead[i]);
    for (int i = 0; i < 5; i++) al_destroy_bitmap(minion2.img_atk[i]);
    al_destroy_sample_instance(minion2.atk_Sound);
}
//===============================================================================
void equip_init(){
    for(int i = 0 ; i <= 3 ; i++){
        char temp[50];
        sprintf( temp, "./image/equip/equip_%d.png", i );
        equip.img[i] = al_load_bitmap(temp);
    }
    for(int i = 0 ; i <= 0 ; i++){
        char temp[50];
        sprintf( temp, "./image/shield.png", i );
        equip.shield[i] = al_load_bitmap(temp);
    }
    for(int i = 0 ; i <= 0 ; i++){
        char temp[50];
        sprintf( temp, "./image/flash.png", i );
        equip.flash[i] = al_load_bitmap(temp);
    }

    equip.x = 1224;
    equip.y = 40;

    equip.state = STOP;
    equip.anime = 0;
    equip.anime_time = 48;
}

void equip_process(ALLEGRO_EVENT event){
    // process the animation
    if( event.type == ALLEGRO_EVENT_TIMER ){
        if( event.timer.source == fps ){
            equip.anime++;
            equip.anime %= equip.anime_time;
        }
    }
}

void equip_update() {
    equip.state = MOVE;
    if( key_state[ALLEGRO_KEY_H] && equip.anime == 5) {
        if (equipment > 0 && chara.blood < 6) {
            al_play_sample_instance(chara.drink_Sound);
            equipment -= 1;
            chara.blood += 1;
        }
    }
}

void equip_draw() {
    if (chara_alive == true && boss_alive == true) {
        if (equip.state == MOVE) al_draw_bitmap(equip.img[equipment], equip.x, equip.y, 0);

        al_draw_filled_rectangle(924, 10, 1404, 30, al_map_rgb(255, 255, 255));
        al_draw_filled_rectangle(924 + (6 - chara.blood) * 80, 10, 1404, 30, al_map_rgb(180, 0, 0));

        al_draw_filled_rectangle(0, 10, 490, 30, al_map_rgb(255, 255, 255));
        al_draw_filled_rectangle(0, 10, 490 - (7 - boss.blood) * 70, 30, al_map_rgb(180, 0, 0));


        if (alive != 0 && cut != 0)  al_draw_bitmap(equip.shield[0], 0, 40, 0);
        if (chara.countt >= 10) al_draw_bitmap(equip.flash[0], 1160, 40, 0);
    }
}

void equip_destory(){
    for (int i = 0; i < 5; i++) al_destroy_bitmap(equip.img[i]);
    al_destroy_bitmap(equip.shield[0]);
    al_destroy_bitmap(equip.flash[0]);
}
