#include "global.h"

// variables for global usage
const float FPS = 80.0;
const int WIDTH = 1404;
const int HEIGHT = 810;
ALLEGRO_EVENT_QUEUE *event_queue = NULL;
ALLEGRO_TIMER *fps = NULL;
bool key_state[ALLEGRO_KEY_MAX] = {false};
int judge_next_window = 1;
