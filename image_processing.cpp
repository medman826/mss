/*
 * Matthew Diamond 2015
 * This file contains all of the basic image processing functions.
 * It also contains helper functions for main().
 */

/************
 * INCLUDES *
 ************/

// Included libraries
#include <iostream>
#include <queue>
#include <vector>

// Included SDL components
#include "SDL2/SDL.h"

// Included files
#include "image_processing.hpp"
#include "main.hpp"
#include "signal_processing.hpp"

// Included classes
#include "Graphics_Objects/Page.hpp"
#include "Graphics_Objects/Rect.hpp"
#include "Module.hpp"
#include "Modules/Oscillator.hpp"
#include "Modules/Output.hpp"

using namespace std;

/****************************
 * INITIALIZATION FUNCTIONS *
 ****************************/

/*
 * Open a window using width and height specified in main.cpp.
 */
int open_window()
{
    WINDOW = SDL_CreateWindow("Visualizer",
                              SDL_WINDOWPOS_UNDEFINED,
                              SDL_WINDOWPOS_UNDEFINED,
                              WINDOW_WIDTH,
                              WINDOW_HEIGHT,
                              SDL_WINDOW_OPENGL
                             );

    if (WINDOW == NULL) {
        cout << "Could not create window: " << SDL_GetError() << endl;
        return 0;
    }

    cout << "Window opened." << endl;

    return 1;
}

/*
 * Create a renderer for the window created before.
 */
int create_renderer()
{
    RENDERER = SDL_CreateRenderer(WINDOW, -1, SDL_RENDERER_ACCELERATED);

    if (WINDOW == NULL) {
        cout << "Could not create renderer: " << SDL_GetError() << endl;
        return 0;
    }

    cout << "Renderer created." << endl;

    return 1;
}

/*
 * Load fonts into external variables so that all
 * modules can render text.
 */
int load_fonts()
{
    FONT_REGULAR = TTF_OpenFont("fonts/SourceCodePro-Regular.ttf", 11);
    FONT_BOLD = TTF_OpenFont("fonts/SourceCodePro-Bold.ttf", 11);

    if(!FONT_REGULAR || !FONT_BOLD)
    {
        cout << "Could not open one of the TTF fonts: " << TTF_GetError() << endl;
        return 0;
    }

    cout << "Fonts loaded." << endl;

    return 1;
}

/******************************
 * IMAGE PROCESSING FUNCTIONS *
 ******************************/

/*
 * Determine where in the window each of the modules and their
 * respective graphics objects are meant to be rendered.
 */
void calculate_graphics_objects()
{
    for(unsigned int i = 0; i < MODULES.size(); i ++)
    {
        MODULES[i]->calculate_graphics_objects();
    }
    MODULES_CHANGED = 0;
}

/*
 * Determine what graphics objects go on what pages. Create those pages.
 */
void calculate_pages()
{
    // Specify the location of the page (the entire window),
    // create the Rect object representing the background and a
    // list of graphics objects for the page, add the background
    // to the list
    SDL_Rect location = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};
    string object_name = "background";
    Rect *background = new Rect(&object_name, &location, &BLACK);
    vector<Graphics_Object *> *graphics_objects = new vector<Graphics_Object *>();
    graphics_objects->push_back(background);

    // For each module, add each of its graphics objects to the list,
    // if we're on the last module of the page and the last graphics object
    // of the module, create the final graphics object and then create the page.
    for(unsigned int i = 0; i < MODULES.size(); i ++)
    {
        for(unsigned int j = 0; j < MODULES[i]->graphics_objects.size(); j ++)
        {
            graphics_objects->push_back((MODULES[i]->graphics_objects)[j]);
            if(((unsigned int) (i % (MODULES_PER_ROW * MODULES_PER_COLUMN)) ==
               (unsigned int) ((MODULES_PER_ROW * MODULES_PER_COLUMN) - 1)) ||
               ((i == MODULES.size() - 1) && (j == MODULES[i]->graphics_objects.size() - 1)))
            {
                Page *page = new Page(&location, &BLACK, graphics_objects);
                PAGES.push_back(page);
                graphics_objects = new vector<Graphics_Object *>();
            }
        }
    }
}

/*
 * Render the GUI in the window by using the
 * member functions in each module that render
 * their representations
 */
void draw_surface()
{
    // First, lock the audio
    SDL_LockAudio();

    // Once audio is locked, copy all data necessary for graphics
    for(unsigned int i = 0; i < MODULES.size(); i ++)
    {
        MODULES[i]->copy_graphics_data();
    }

    // Once all data necessary for graphics has been copied,
    // unlock the audio
    SDL_UnlockAudio();

    // If the set of modules has changed, calculate the
    // graphics objects, then calculate the pages
    if(MODULES_CHANGED)
    {
        calculate_graphics_objects();
        calculate_pages();
        MODULES_CHANGED = false;
    }

    // If the current page exists, render it, otherwise,
    // render a black rectangle over the whole window
    if(CURRENT_PAGE < PAGES.size())
        PAGES[CURRENT_PAGE]->render_graphics_object();
    else
    {
        SDL_SetRenderDrawColor(RENDERER, BLACK.r, BLACK.g, BLACK.b, BLACK.a);
        SDL_Rect location = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};
        SDL_RenderFillRect(RENDERER, &location);
    }

    // Present what has been rendered
    SDL_RenderPresent(RENDERER);
}
