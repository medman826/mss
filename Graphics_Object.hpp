/*
 * Matthew Diamond 2015
 * The header for a base class to represent a generic graphics object.
 * Other classes will extend this class and represent different
 * types of graphics objects. This file defines the class.
 * The following classes are derived from the graphics object class:
 *   - Page
 *   - Rect
 *   - Text
 *   - Toggle_Button
 *   - Text_Box
 *   - Waveform
 */

#ifndef synth_graphics_object_h
#define synth_graphics_object_h

/************
 * INCLUDES *
 ************/

// No includes necessary

/**********************************
 * TYPES OF GRAPHICS_OBJECTS ENUM *
 **********************************/

enum Graphics_Objects
{
    PAGE,
    RECT,
    TEXT,
    TOGGLE_BUTTON,
    TEXT_BOX,
    WAVEFORM
};

/************************************
 * GRAPHICS_OBJECT CLASS DEFINITION *
 ************************************/

class Graphics_Object
{
    public:
        // Module information
        std::string name;
        int type;
        SDL_Rect location;
        SDL_Color color;
        // Constructor and destructor
        Graphics_Object();
        virtual ~Graphics_Object();
        // Virtual member functions
        //   render() is called each time a new frame is needed for
        //   the window to display, it should render the graphics object in
        //   the window
        virtual void render_graphics_object() = 0;
};

#endif
