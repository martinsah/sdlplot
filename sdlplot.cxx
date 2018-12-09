#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <SDL2/SDL.h>
#include "sdlplot.h"
#include <iostream>

using namespace std;

sdlplot::sdlplot()
{
    //
    window_sizex = XSIZE_DEFAULT;
    window_sizey = YSIZE_DEFAULT;
    window_posx = SDL_WINDOWPOS_CENTERED;
    window_posy = SDL_WINDOWPOS_CENTERED;
    quit = SDL_FALSE;
}

int sdlplot::makewindow()
{
    SDL_Init(SDL_INIT_VIDEO & SDL_INIT_TIMER);

    window = SDL_CreateWindow(
        "SDL2Test",
        window_posx,
        window_posy,
        window_sizex,
        window_sizey,
        0);
    window_id = SDL_GetWindowID(window);
    SDL_SetWindowResizable(window,SDL_TRUE);
    
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_RenderSetLogicalSize(renderer,virt_xmax,virt_ymax);

    redraw_timer_id = SDL_AddTimer(redraw_delay_time, sdl_timer_callback, USEREVENT_TIMER_TRIGGER);
}

void sdlplot::drawaxis()
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderDrawLine(renderer, axis.xpadding, axis.ypadding, xscale(axis.xmax), axis.ypadding);
    SDL_RenderDrawLine(renderer, xscale(axis.xmax), axis.ypadding, xscale(axis.xmax), yscale(axis.ymax));
    SDL_RenderDrawLine(renderer, xscale(axis.xmax), yscale(axis.ymax), axis.xpadding, yscale(axis.ymax));
    SDL_RenderDrawLine(renderer, axis.xpadding, yscale(axis.ymax), axis.xpadding, axis.ypadding);
    cout << "\nxmax: " << xscale(axis.xmax);
    cout << " ymax: " << yscale(axis.ymax);
}

void sdlplot::drawline()
{
    int count=0;
    std::vector<SDL_Point> trpts;

    
    for(auto it = line.points.begin(); it != line.points.end(); ++it ){
        SDL_Point ipt = {xscale((*it).x),yscale((*it).y)};
        trpts.push_back(ipt);
        count++;
    }
    SDL_Point *points = trpts.data();
    if(line.connect_the_dots){
        SDL_RenderDrawLines(renderer, points, count);
    } else {
        SDL_RenderDrawPoints(renderer, points, count);
    }
}

int sdlplot::xscale(double x)
{
    double xrange  = axis.xmax - axis.xmin;
    double xpos = (x-axis.xmin)/xrange;
    return (int)(double(virt_xmax-2*axis.xpadding)*xpos) + axis.xpadding;
}

int sdlplot::yscale(double y)
{
    double yrange = axis.ymax - axis.ymin;
    double ypos = (y-axis.ymin)/yrange;
    return (int)(double(virt_ymax-2*axis.ypadding)*ypos) + axis.ypadding;
}

bool sdlplot::run()
{
    SDL_WaitEventTimeout(NULL,run_timeout_ms);
    this->event_poll();

    return(quit==SDL_TRUE);
}

void sdlplot::event_poll()
{

    SDL_Event event;

    while (SDL_PollEvent(&event)) {

        /* handle your event here */
        debug_print_events(&event);
        if (event.type == SDL_QUIT)
        {
            quit = SDL_TRUE;
        }
        if(event.type == SDL_USEREVENT){
            switch(event.user.code){
                case USEREVENT_TIMER_TRIGGER:
                    this->redraw();
                    break;
            }
        }
        if (event.type == SDL_WINDOWEVENT) {
            switch (event.window.event) {
            case SDL_WINDOWEVENT_SHOWN:
                break;
            case SDL_WINDOWEVENT_HIDDEN:
                break;
            case SDL_WINDOWEVENT_EXPOSED:
                break;
            case SDL_WINDOWEVENT_MOVED:
                break;
            case SDL_WINDOWEVENT_RESIZED:
                virt_xmax = event.window.data1;
                virt_ymax = event.window.data2;

                break;
            case SDL_WINDOWEVENT_SIZE_CHANGED:
                break;
            case SDL_WINDOWEVENT_MINIMIZED:
                break;
            case SDL_WINDOWEVENT_MAXIMIZED:
                break;
            case SDL_WINDOWEVENT_RESTORED:
                break;
            case SDL_WINDOWEVENT_ENTER:
                break;
            case SDL_WINDOWEVENT_LEAVE:
                break;
            case SDL_WINDOWEVENT_FOCUS_GAINED:
                break;
            case SDL_WINDOWEVENT_FOCUS_LOST:
                break;
            case SDL_WINDOWEVENT_CLOSE:
                quit = SDL_TRUE;
                break;
    #if SDL_VERSION_ATLEAST(2, 0, 5)
            case SDL_WINDOWEVENT_TAKE_FOCUS:
                break;
            case SDL_WINDOWEVENT_HIT_TEST:
                break;
    #endif
            default:
                break;
            }
            this->redraw();
        }        


    }
}

void sdlplot::redraw()
{
    cout << "\nsdlplot::redraw() called";
    SDL_RenderSetLogicalSize(renderer,virt_xmax,virt_ymax);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);
    this->drawaxis();
    this->drawline();
    SDL_RenderPresent(renderer);

}


Uint32 sdl_timer_callback(Uint32 interval, void *param)
{
    SDL_Event event;
    SDL_UserEvent userevent;

    userevent.type = SDL_USEREVENT;
    userevent.code = USEREVENT_TIMER_TRIGGER;
    userevent.data1 = NULL;
    event.user = userevent;
    SDL_PushEvent(&event);
    return(interval);
}

sdlplot::~sdlplot()
{
    SDL_DestroyWindow(window);
    SDL_Quit();
}