#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "includes/memory.c"
#include "includes/files.c"

typedef struct seven_segment_signal {
    int segments_count;
    bool a, b, c, d, e, f, g;
} SIGNAL;

typedef struct display {
    SIGNAL** unique_signals;
    SIGNAL** output_signals;
} DISPLAY;

SIGNAL* to_signal(char* str) {
    SIGNAL* signal = (SIGNAL*) calloc(1, sizeof(SIGNAL));
    signal->segments_count = 0;
    for (int i = 0; i < strlen(str); i++) {
        switch (str[i]) {
            case 'a': signal->a = true; signal->segments_count++; break;
            case 'b': signal->b = true; signal->segments_count++; break;
            case 'c': signal->c = true; signal->segments_count++; break;
            case 'd': signal->d = true; signal->segments_count++; break;
            case 'e': signal->e = true; signal->segments_count++; break;
            case 'f': signal->f = true; signal->segments_count++; break;
            case 'g': signal->g = true; signal->segments_count++; break;
        }
    }
    return signal;
}

intptr_t to_display(char* line) {
    DISPLAY* display = (DISPLAY*) calloc(1, sizeof(DISPLAY));
    display->unique_signals = (SIGNAL**) calloc(10, sizeof(SIGNAL*));
    display->output_signals = (SIGNAL**) calloc(4, sizeof(SIGNAL*));

    char* unique_signals = strtok(line, "|");
    unique_signals[strlen(unique_signals) - 1] = '\0'; // trim trailing space
    char* output_signals = strtok(NULL, "|") + 1; // trim starting space
    output_signals[strlen(output_signals) - 1] = '\0'; // trim trailing new line

    int i = 0;
    char* unique_signal = strtok(unique_signals, " ");
    do {
        display->unique_signals[i++] = to_signal(unique_signal);
    } while(unique_signal = strtok(NULL, " "));

    i = 0;
    char* output_signal = strtok(output_signals, " ");
    do {
        display->output_signals[i++] = to_signal(output_signal);
    } while(output_signal = strtok(NULL, " "));

    return (intptr_t) display;
}

char get_single_segment(SIGNAL* signal) {
    if (signal->a + signal->b + signal->c + signal->d + signal->e + signal->f + signal->g != 1) {
        exit(2021);
    }

    return
        signal->a ? 'a' :
        signal->b ? 'b' :
        signal->c ? 'c' :
        signal->d ? 'd' :
        signal->e ? 'e' :
        signal->f ? 'f' : 'g';
}

SIGNAL* substract_segements(SIGNAL* sig, SIGNAL* from, bool check_contains) {

    if (check_contains && sig->a && !from->a) {
        return NULL;
    }
    if (check_contains && sig->b && !from->b) {
        return NULL;
    }
    if (check_contains && sig->c && !from->c) {
        return NULL;
    }
    if (check_contains && sig->d && !from->d) {
        return NULL;
    }
    if (check_contains && sig->e && !from->e) {
        return NULL;
    }
    if (check_contains && sig->f && !from->f) {
        return NULL;
    }
    if (check_contains && sig->g && !from->g) {
        return NULL;
    }

    SIGNAL* diff = (SIGNAL*) malloc(sizeof(SIGNAL));
    diff->a = !sig->a && from->a;
    diff->b = !sig->b && from->b;
    diff->c = !sig->c && from->c;
    diff->d = !sig->d && from->d;
    diff->e = !sig->e && from->e;
    diff->f = !sig->f && from->f;
    diff->g = !sig->g && from->g;
    return diff;
}

bool contains(SIGNAL *signal, char c) {
    return c == 'a' ? signal->a :
           c == 'b' ? signal->b :
           c == 'c' ? signal->c :
           c == 'd' ? signal->d :
           c == 'e' ? signal->e :
           c == 'f' ? signal->f :
           c == 'g' ? signal->g : false;
}

int to_digit(SIGNAL *signal, char t, char tl, char tr, char m, char bl, char br, char b) {
    switch (signal->segments_count) {
        case 2: return 1;
        case 3: return 7;
        case 4: return 4;
        case 7: return 8;
        case 5:
            return
                contains(signal, tr) && contains(signal, br) ? 3 :
                contains(signal, tl) ? 5 :
                2;
        case 6:
            return
                !contains(signal, m) ? 0 :
                contains(signal, tr) ? 9 :
                6;
    }
}

int decode_output(DISPLAY* display) {
    /**
     * Segments count per digit:
     * 1        => 2 segments
     * 4        => 4 segments
     * 7        => 3 segments
     * 8        => 7 segments
     * 2, 3, 5  => 5 segments
     * 0, 6, 9  => 6 segments
     */

    // maps positions to display wirings
    char top, top_left, top_right, middle, bottom_left, bottom_right, bottom;

    SIGNAL *one_signal, *four_signal, *seven_signal, *eight_signal;
    SIGNAL
        **five_segments_signals = (SIGNAL**) malloc(3 * sizeof(SIGNAL**)),
        **six_segments_signals = (SIGNAL**) malloc(3 * sizeof(SIGNAL**));
    int fss_idx = 0;
    int sss_idx = 0;

    // Sort out which signals map uniquely to digits (or a digit) by segements count
    for (int i = 0; i < 10; i++) {
        SIGNAL* signal = display->unique_signals[i];
        switch (signal->segments_count) {
            case 2: one_signal = signal; break;
            case 3: seven_signal = signal; break;
            case 4: four_signal = signal; break;
            case 7: eight_signal = signal; break;
            case 5: five_segments_signals[fss_idx++] = signal; break;
            case 6: six_segments_signals[sss_idx++] = signal; break;
        }
    }

    // Find overlaping 6 segments number with 4, which is 9
    SIGNAL *nine_signal = NULL;
    SIGNAL *top_bottom_segments = NULL;
    for (int i = 0; i < 3; i++) {
        if (top_bottom_segments = substract_segements(four_signal, six_segments_signals[i], true)) {
            nine_signal = six_segments_signals[i];
            break;
        }
    }
    if (!nine_signal) { exit (2021); }

    SIGNAL *bottom_segment = substract_segements(seven_signal, top_bottom_segments, false);
    bottom = get_single_segment(bottom_segment);
    top = get_single_segment(substract_segements(bottom_segment, top_bottom_segments, true));

    // Find overalpping 5 segments signal with 1, which is 3
    SIGNAL *three_signal = NULL;
    SIGNAL *top_middle_bottom_segments = NULL;
    for (int i = 0; i < 3; i++) {
        if (top_middle_bottom_segments = substract_segements(one_signal, five_segments_signals[i], true)) {
            three_signal = five_segments_signals[i];
            break;
        }
    }
    if (!three_signal) { exit (2021); }

    SIGNAL* middle_segment = substract_segements( top_bottom_segments, top_middle_bottom_segments, true);
    middle = get_single_segment(middle_segment);
    top_left = get_single_segment(substract_segements(three_signal, nine_signal, false));
    bottom_left = get_single_segment(substract_segements(nine_signal, eight_signal, false));
    
    // Find non-overalpping 5 segments signal with 9, which is 2
    SIGNAL *two_signal = NULL;
    for (int i = 0; i < 3; i++) {
        if (!substract_segements(five_segments_signals[i], nine_signal, true)) {
            two_signal = five_segments_signals[i];
            break;
        }
    }
    if (!two_signal) { exit (2021); }

    SIGNAL *top_right_segment = substract_segements(
        substract_segements(two_signal, one_signal, false),
        one_signal,
        false
    );
    top_right = get_single_segment(top_right_segment);
    bottom_right = get_single_segment(substract_segements( top_right_segment, one_signal, false));

    int number = 0;
    for (int i = 0; i < 4; i++) {
        number *= 10;
        number += to_digit(
            display->output_signals[i],
            top, top_left, top_right, middle, bottom_left, bottom_right, bottom
        );
    }
    return number;
}

void main() {
    intptr_t* displays = (intptr_t*) malloc(300 * sizeof(intptr_t));
    int displays_count = map_lines("day8-input.txt", displays, 300, &to_display);

    int part_1 = 0;
    int part_2 = 0;
    for (int i = 0; i < displays_count; i++) {
        DISPLAY* display = (DISPLAY*) displays[i];
        for (int j = 0; j < 4; j++) {
            SIGNAL* output_signal = display->output_signals[j];
            switch(output_signal->segments_count) {
                case 2:
                case 3:
                case 4:
                case 7:
                    part_1++;
            }
        }
        
        part_2 += decode_output(display);
    }

    printf("Count of unique digits in output: %d\n", part_1);
    printf("Sum of outputs: %d\n", part_2);
}