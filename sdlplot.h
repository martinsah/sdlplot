#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <vector>

#define USEREVENT_TIMER_TRIGGER 0
#define XSIZE_DEFAULT 1024
#define YSIZE_DEFAULT 768
#define RUN_TIMEOUT_DEFAULT 10

struct axis_params_2d {
    int xpadding = 25;
    int ypadding = 25;
    int xcross = 0;         // point where x axis crosses the y axis
    int ycross = 0;         // point where y axis crosses the x axis
    
    double xmin = -1.;
    double xmax = 1.;
    double ymin = -1.;
    double ymax = 1.;

    double xtick_maj = 1.;
    double xtick_min = 0.1;

    double ytick_maj = 1;
    double ytick_min = 0.1;
};

struct datapoint {
    double x;
    double y;
};

struct line_params_2d {
    std::vector<datapoint> points;

    bool connect_to_xmin = false;
    bool connect_the_dots = true;
};



class sdlplot {

public:
    sdlplot();
    virtual ~sdlplot();
    SDL_bool quit = SDL_FALSE;
    bool timer_trigger = SDL_FALSE;

    axis_params_2d axis;
    line_params_2d line;

    bool run();
    void drawaxis();
    void event_poll();
    void redraw();
    void drawline();
    
    int xscale(double x);
    int yscale(double y);

    int run_timeout_ms = RUN_TIMEOUT_DEFAULT;
    int virt_xmax = 640;
    int virt_ymax = 480;
    Uint32 redraw_delay_time = 1000;
    SDL_TimerID redraw_timer_id = 0;


    int makewindow();
    int window_posx;
    int window_posy;
    int window_sizex = XSIZE_DEFAULT;
    int window_sizey = YSIZE_DEFAULT;
    int window_id;

    SDL_bool done;
    SDL_Renderer *renderer;
    SDL_Window *window;
    bool redraw_trigger;

};

void debug_print_events(SDL_Event *event);
Uint32 sdl_timer_callback(Uint32 interval, void *param);
