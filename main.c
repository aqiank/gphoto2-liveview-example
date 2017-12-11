#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stddef.h>
#include <stdbool.h>
#include <signal.h>
#include <errno.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#define WINDOW_WIDTH (800)
#define WINDOW_HEIGHT (600)

#define FRAME_RATE (30)
#define FRAME_BUFFER_SIZE (4 * WINDOW_WIDTH * WINDOW_HEIGHT)

static SDL_Window *window;
static SDL_Surface *window_surface;
static uint8_t *frame_buffer;
static size_t frame_buffer_size;

void handle_signal()
{
    fprintf(stdout, "\nInterrupted! Exiting program..\n");

    if (window) {
        SDL_DestroyWindow(window);
    }

    if (SDL_WasInit(SDL_INIT_VIDEO)) {
        SDL_Quit();
    }

    exit(0);
}

void draw()
{
    SDL_RWops *rw;
    SDL_Surface *surface;

    rw = SDL_RWFromMem(frame_buffer, frame_buffer_size);
    if (!rw) {
        goto out;
    }

    surface = IMG_Load_RW(rw, 1);
    if (!surface) {
        goto out;
    }

    if (SDL_BlitSurface(surface, NULL, window_surface, NULL) < 0) {
        goto out;
    }

    if (SDL_UpdateWindowSurface(window) < 0) {
        goto out;
    }

    SDL_FreeSurface(surface);
    return;

out:
    fprintf(stderr, "%s\n", SDL_GetError());
}

size_t read_jpeg(FILE *file)
{
    bool started = false;
    size_t index = 0;

    for (;;) {
        const int c1 = fgetc(file);
        const int c2 = fgetc(file);
        if (started) {
            frame_buffer[index++] = c1;
            frame_buffer[index++] = c2;

            const uint16_t code =((c1 << 8) | c2);
            if (code == 0xffd9) {
                break;
            }
        } else {
            const uint16_t code =((c1 << 8) | c2);
            if (code == 0xffd8) {
                frame_buffer[index++] = c1;
                frame_buffer[index++] = c2;
                started = true;
            }
        }
    }

    return index;
}

int main(int argc, char **argv)
{
    FILE *file = stdin;

    signal(SIGINT, handle_signal);

    if (argc > 1) {
        file = fopen(argv[1], "r+");
        if (!file) {
            fprintf(stderr, "%s\n", strerror(errno));
            return -1;
        }
    }

    SDL_Init(SDL_INIT_VIDEO);

    window = SDL_CreateWindow(
            "gPhoto2 LiveView Example",
            SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED,
            WINDOW_WIDTH, WINDOW_HEIGHT,
            SDL_WINDOW_SHOWN
    );
    if (!window) {
        fprintf(stderr, "%s\n", SDL_GetError());
        return -1;
    }
    window_surface = SDL_GetWindowSurface(window);
    frame_buffer = (uint8_t *) malloc(FRAME_BUFFER_SIZE);

    bool running = true;
    while (running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
            case SDL_QUIT:
                running = false;
                break;
            }
        }

        frame_buffer_size = read_jpeg(file);

        draw();

        SDL_Delay(1000 / FRAME_RATE);
    }

    SDL_DestroyWindow(window);
    SDL_Quit();
}
