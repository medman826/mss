/*
 * Matthew Diamond 2015
 * The Text graphics object. This object is just some text
 * in some place in the window, with some color.
 */

#ifndef synth_text_h
#define synth_text_h

/************
 * INCLUDES *
 ************/

// Included SDL componenets
#include "SDL2/SDL.h"

/*************************
 * TEXT CLASS DEFINITION *
 *************************/

class Text: public Graphics_Object
{
    public:
        // The font of the text, the text as a string,
        // and the text as a texture
        TTF_Font *font;
        std::string text;
        SDL_Texture *texture;

        // Constructor and destructor
        Text(std::string, SDL_Rect, SDL_Color *,
             std::string, TTF_Font *);
        virtual ~Text();

        // Virtual member functions
        virtual void render();
        virtual void clicked();

        // Member functions
        //   Update the text displayed
        void update_text(std::string);
        //   Set the text color
        void set_color(SDL_Color *);
};

#endif
