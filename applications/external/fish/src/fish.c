#include "dolphin/dolphin.h"
#include <furi.h>
#include <furi_hal.h>
#include <gui/gui.h>
#include <stdlib.h>

// Scoring
int SCORE = 0;
int MAX_SWIM = 1;
int HIGH_SCORE = 10;
int LEVEL = 1;

// Display score string
char score_str[16];

// Coordinates for drawing
int player_x = 6;
int player_y = 28;
int kelp_x = 124;
int kelp_y = 56;
int jellyfish_x = 64;
int jellyfish_y = 0;

// Variables for movement
bool is_jumping = false;
bool is_random_kelp = true;
bool is_random_jellyfish = true;
int kelp_x_rand;
int kelp_y_rand;
int jellyfish_x_rand;
int jellyfish_y_rand;

int total_health;
bool is_health = false;

// sprite coordinates for drawing
int player[][2] = {
    {7, 4},
    {8, 4},
    {3, 5},
    {4, 5},
    {6, 5},
    {9, 5},
    {3, 6},
    {5, 6},
    {10, 6},
    {3, 7},
    {5, 7},
    {10, 7},
    {3, 8},
    {4, 8},
    {6, 8},
    {9, 8},
    {7, 9},
    {8, 9}};
int kelp[][2] =
    {{2, 2}, {4, 2}, {3, 3}, {2, 4}, {4, 4}, {3, 5}, {2, 6}, {4, 6}, {3, 7}, {2, 8}, {4, 8}, {3, 9}};
int jellyfish[][2] = {{3, 2}, {4, 2}, {5, 2}, {6, 2}, {7, 2}, {8, 2}, {2, 3}, {9, 3}, {2, 4},
                      {9, 4}, {3, 5}, {4, 5}, {5, 5}, {6, 5}, {7, 5}, {8, 5}, {3, 6}, {6, 6},
                      {8, 6}, {4, 7}, {6, 7}, {9, 7}, {2, 8}, {4, 8}, {7, 8}, {3, 9}};
int health[][2] = {{5, 3},   {6, 3},   {7, 3},   {4, 4},   {7, 4},   {8, 4},   {3, 5},  {5, 5},
                   {6, 5},   {8, 5},   {9, 5},   {3, 6},   {5, 6},   {9, 6},   {10, 6}, {3, 7},
                   {4, 7},   {7, 7},   {8, 7},   {10, 7},  {4, 8},   {5, 8},   {7, 8},  {11, 8},
                   {5, 9},   {6, 9},   {11, 9},  {6, 10},  {7, 10},  {11, 10}, {8, 11}, {9, 11},
                   {10, 11}, {12, 11}, {13, 11}, {11, 12}, {13, 12}, {11, 13}, {12, 13}};

// Custom sound track
bool is_music = true;
int sound_intro_frequency[] = {1000, 500, 1000, 500, 1000, 500};
int sound_intro_timing[] = {500, 500, 500, 500, 500, 500};

int sound_score_frequency[] = {750};
int sound_score_timing[] = {40};

void play_beep(int frequency[], int timing[], int length) {
    for(int i = 0; i < length; i += 1) {
        if(furi_hal_speaker_acquire(40)) {
            furi_hal_speaker_start(frequency[i], 0.5f);
            furi_delay_ms(timing[i]);
            furi_hal_speaker_stop();
            furi_hal_speaker_release();
        }
    }
}

void collide_rect() {
    int player_left = player_x;
    int player_top = player_y - 1;
    int player_right = player_x + 8;
    int player_bottom = player_y + 7;

    int kelp_left = kelp_x - kelp_x_rand * 8;
    int kelp_top = kelp_y - kelp_y_rand * 8;
    int kelp_right = kelp_x;
    int kelp_bottom = kelp_y + 4;

    int jellyfish_left = jellyfish_x - jellyfish_x_rand * 7;
    int jellyfish_top = jellyfish_y + 7;
    int jellyfish_right = jellyfish_x;
    int jellyfish_bottom = jellyfish_y + jellyfish_y_rand * 8 + 7;

    bool kelp_collision = player_left <= kelp_right && player_right >= kelp_left &&
                          player_top <= kelp_bottom && player_bottom >= kelp_top;

    bool jellyfish_collision = player_left <= jellyfish_right && player_right >= jellyfish_left &&
                               player_top <= jellyfish_bottom && player_bottom >= jellyfish_top;

    if(kelp_collision || jellyfish_collision || player_bottom >= 64 || player_top <= 0) {
        // Subtract 1 hp
        total_health -= 1;

        // Coordinates for drawing
        kelp_x = 124;
        is_random_kelp = true;
        jellyfish_x = 64;
        is_random_jellyfish = true;
        player_x = 6;
        player_y = 28;

        LEVEL += 1;
    }

    if(total_health == 0) {
        // Score variables
        MAX_SWIM = 2;
        HIGH_SCORE = 100;

        SCORE = 0;
        LEVEL += 1;
        total_health = 4;

        // Coordinates for drawing
        kelp_x = 124;
        is_random_kelp = true;
        jellyfish_x = 64;
        is_random_jellyfish = true;
        player_x = 6;
        player_y = 28;

        is_health = false;

        is_music = true;
    }
}

void draw_health(Canvas* canvas) {
    int array_size = sizeof(health) / sizeof(health[0]);
    for(int j = 0; j < total_health; j++) {
        for(int i = 0; i < array_size; i++) {
            int x = health[i][0] + (10 * j);
            int y = health[i][1];
            if(x != 0 && y != 0) {
                canvas_draw_dot(canvas, x + 2, y + 50);
            }
        }
    }
}

void draw_player(Canvas* canvas) {
    if(is_jumping) {
        if(MAX_SWIM == 2) {
            player_y -= 3;
        } else {
            player_y -= 3;
        }
    }

    else {
        if(MAX_SWIM == 3) {
            player_y += 2;
        } else {
            player_y += 3;
        }
    }

    int array_size = sizeof(player) / sizeof(player[0]);
    for(int i = 0; i < array_size; i++) {
        int x = player[i][0];
        int y = player[i][1];
        if(x != 0 && y != 0) {
            canvas_draw_dot(canvas, x + player_x, y + player_y);
        }
    }
}

void draw_kelp(Canvas* canvas) {
    if(is_random_kelp) {
        kelp_x_rand = (SCORE + LEVEL) % 3 + 1;
        kelp_y_rand = (SCORE + LEVEL) % 3 + 1;
        is_random_kelp = false;
    }

    int array_size = sizeof(kelp) / sizeof(kelp[0]);
    for(int a = 1; a < kelp_y_rand + 1; a++) {
        for(int b = 1; b < kelp_x_rand + 1; b++) {
            for(int i = 0; i < array_size; i++) {
                int x = kelp[i][0];
                int y = kelp[i][1];
                if(x != 0 && y != 0) {
                    canvas_draw_dot(canvas, (x + kelp_x) - (b * 4), y + kelp_y - a * 8);
                }
            }
        }
    }

    kelp_x -= MAX_SWIM;

    if(kelp_x <= -8) {
        kelp_x = 124;
        is_random_kelp = true;
        SCORE += 10;

        play_beep(sound_score_frequency, sound_score_timing, 1);
    }
}

void draw_jellyfish(Canvas* canvas) {
    if(is_random_jellyfish) {
        jellyfish_x_rand = (SCORE + LEVEL) % 3 + 1;
        jellyfish_y_rand = (SCORE + LEVEL) % 3 + 1;
        is_random_jellyfish = false;
    }

    int array_size = sizeof(jellyfish) / sizeof(jellyfish[0]);
    for(int a = 1; a < jellyfish_y_rand + 1; a++) {
        for(int b = 1; b < jellyfish_x_rand + 1; b++) {
            for(int i = 0; i < array_size; i++) {
                int x = jellyfish[i][0];
                int y = jellyfish[i][1];
                if(x != 0 && y != 0) {
                    canvas_draw_dot(canvas, (x + jellyfish_x) - (b * 8), y + jellyfish_y + a * 8);
                }
            }
        }
    }

    jellyfish_x -= MAX_SWIM;

    if(jellyfish_x <= -8) {
        jellyfish_x = 124;
        is_random_jellyfish = true;
        SCORE += 10;

        play_beep(sound_score_frequency, sound_score_timing, 1);
    }
}

static void draw_callback(Canvas* canvas, void* context) {
    UNUSED(context);
    furi_delay_ms(40);

    canvas_clear(canvas);
    collide_rect();
    draw_player(canvas);
    draw_kelp(canvas);
    draw_jellyfish(canvas);
    draw_health(canvas);

    snprintf(score_str, sizeof(score_str), "%d", SCORE);
    canvas_draw_str(canvas, 2, 8, score_str);

    if(SCORE >= HIGH_SCORE && MAX_SWIM < 5) {
        HIGH_SCORE += 100;
        MAX_SWIM += 2;
    }

    if(SCORE % 100 == 0 && is_health) {
        total_health += 1;
        is_health = false;
    }

    if((SCORE - 10) % 100 == 0 && SCORE != 0) {
        is_health = true;
    }

    canvas_commit(canvas);

    if(is_music == true) {
        is_music = false;
        play_beep(sound_intro_frequency, sound_intro_timing, 6);
    }
}

static void input_callback(InputEvent* event, void* context) {
    FuriMessageQueue* queue = (FuriMessageQueue*)context;
    if(event->type == InputTypeShort || event->type == InputTypeRepeat ||
       event->type == InputTypePress) {
        if(event->key == InputKeyOk) {
            is_jumping = true;
        }
    }

    if(event->type == InputTypeRelease) {
        if(event->key == InputKeyOk) {
            is_jumping = false;
        }
    }

    furi_message_queue_put(queue, event, FuriWaitForever);
}

int main() {
    // Declare initial size of jellyfish bloom and kelp blobs
    kelp_x_rand = (SCORE + LEVEL) % 3 + 1;
    kelp_y_rand = (SCORE + LEVEL) % 3 + 1;
    jellyfish_x_rand = (SCORE + LEVEL) % 3 + 1;
    ;
    jellyfish_y_rand = (SCORE + LEVEL) % 3 + 1;

    // Gui stuff
    FuriMessageQueue* queue = furi_message_queue_alloc(8, sizeof(InputEvent));
    ViewPort* view_port = view_port_alloc();
    view_port_draw_callback_set(view_port, draw_callback, NULL);
    view_port_input_callback_set(view_port, input_callback, queue);
    Gui* gui = (Gui*)furi_record_open("gui");
    gui_add_view_port(gui, view_port, GuiLayerFullscreen);
    dolphin_deed(DolphinDeedPluginGameStart);

    InputEvent event;

    bool running = true;
    while(running) {
        if(furi_message_queue_get(queue, &event, FuriWaitForever) == FuriStatusOk) {
            if(event.type == InputTypeShort && event.key == InputKeyBack) {
                running = false;
            }
        }
        view_port_update(view_port);
    }

    // Free furi stuff
    view_port_enabled_set(view_port, false);
    furi_message_queue_free(queue);
    gui_remove_view_port(gui, view_port);
    view_port_free(view_port);
    furi_record_close(RECORD_GUI);

    return 0;
}
