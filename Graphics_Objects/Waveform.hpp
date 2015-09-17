/*
 * Matthew Diamond 2015
 * The waveform graphics object. This object is a waveform
 * visualizer for a given buffer rendered wherever is desired.
 */

#ifndef synth_waveform_h
#define synth_waveform_h

/************
 * INCLUDES *
 ************/

// Included SDL componenets
#include "SDL2/SDL.h"

/*************************
 * TEXT CLASS DEFINITION *
 *************************/

class Waveform: public Graphics_Object
{
    public:
        TTF_Font *font;
        std::vector<float> *buffer;
        // Constructor and destructor
        Waveform(std::string *, SDL_Rect *, SDL_Color *, std::vector<float> *);
        virtual ~Waveform();
        // Member functions
        virtual void render_graphics_object();
};

#endif
