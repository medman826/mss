/*
 * Matthew Diamond 2015
 * Member functions for the Oscillator class.
 */

/************
 * INCLUDES *
 ************/

// Included libraries
#include <cmath>
#include <iostream>
#include <string>
#include <vector>

// Included SDL components
#include "SDL2/SDL.h"
#include "SDL2/SDL_ttf.h"

// Included files
#include "../image_processing.hpp"
#include "../main.hpp"
#include "../signal_processing.hpp"

// Included classes
#include "../Module.hpp"
#include "Oscillator.hpp"
#include "../Graphics_Objects/Text.hpp"
#include "../Graphics_Objects/Waveform.hpp"

using namespace std;

/*******************************
 * OSCILLATOR MEMBER FUNCTIONS *
 *******************************/

/*
 * Constructor
 */
Oscillator::Oscillator(string *_name, int _number)
{
    name = *_name;
    type = OSCILLATOR;
    number = _number;

    frequency = 0;
    frequency_str = "0";
    input_frequency = new vector<float>(BUFFER_SIZE, 0);
    live_frequency = false;

    phase_offset = 0;
    phase_offset_str = "0";
    input_phase_offset = new vector<float>(BUFFER_SIZE, 0);
    live_phase_offset = false;

    pulse_width = 0;
    pulse_width_str = "0";
    input_pulse_width = new vector<float>(BUFFER_SIZE, 0);
    live_pulse_width = false;

    range_low = -1;
    range_low_str = "-1";
    input_range_low = new vector<float>(BUFFER_SIZE, 0);
    live_range_low = false;

    range_high = 1;
    range_high_str = "1";
    input_range_high = new vector<float>(BUFFER_SIZE, 0);
    live_range_high = false;

    output = new vector<float>(BUFFER_SIZE, 0);
    output = new vector<float>(BUFFER_SIZE, 0);

    waveform_type = SIN;

    Graphics_Object *dummy = NULL;
    graphics_objects = new vector<Graphics_Object *>(13, dummy);
}

/*
 * Destructor.
 */
Oscillator::~Oscillator()
{

}

/*
 * Process all dependencies, then
 * fill the output buffer with a waveform given
 * the data contained within this class and the
 * audio device information.
 */
void Oscillator::process()
{
    // Check for any dependencies for frequency modulation
    process_depends();

    // Update any control values
    if(live_frequency)
        frequency = (*(input_frequency))[0];
    if(live_phase_offset)
        phase_offset = (*(input_phase_offset))[0];
    if(live_pulse_width)
        pulse_width = (*(input_pulse_width))[0];
    if(live_range_low)
        range_low = (*(input_range_low))[0];
    if(live_range_high)
        range_high = (*(input_range_high))[0];

    // Calculate an amplitude for each sample
    for(int i = 0; i < BUFFER_SIZE; i ++)
    {
        // Calculate and store the current samples amplitude
        // based on phase
        (*output)[i] = sin(current_phase);
        current_phase += (2 * M_PI * frequency / SAMPLE_RATE);
        if(current_phase > (2 * M_PI))
            current_phase -= (2 * M_PI);
    }
    if(range_low != -1 || range_high != 1)
    {
        scale_signal(output, -1, 1, range_low, range_high);
    }
}

/*
 * Calculate the locations of any graphics objects that are
 * unique to this module type.
 */
void Oscillator::calculate_unique_graphics_objects()
{
    int x_text, x_text_box, w_text_box, h_text_box,
        w_waveform, h_waveform,
        y3, y4, y5, y6, y7, y8, y9, y10, y11,
        x_range_high, w_range;
    SDL_Rect location;
    string object_name, contents, prompt;
    Text *text;
    Text_Box *text_box;
    Waveform *waveform;

    x_text = upper_left.x + MODULE_BORDER_WIDTH + 5;
    x_text_box = upper_left.x + MODULE_BORDER_WIDTH + 2;
    w_text_box = ((MODULE_WIDTH - (MODULE_BORDER_WIDTH * 2)) - 4);
    h_text_box = 15;
    w_waveform = ((MODULE_WIDTH - (MODULE_BORDER_WIDTH * 2)) - 4);
    h_waveform = 55;
    y3 = upper_left.y + MODULE_BORDER_WIDTH + 23;
    y4 = upper_left.y + MODULE_BORDER_WIDTH + 80;
    y5 = upper_left.y + MODULE_BORDER_WIDTH + 97;   
    y6 = upper_left.y + MODULE_BORDER_WIDTH + 117;
    y7 = upper_left.y + MODULE_BORDER_WIDTH + 134;
    y8 = upper_left.y + MODULE_BORDER_WIDTH + 152;
    y9 = upper_left.y + MODULE_BORDER_WIDTH + 169;
    y10 = upper_left.y + MODULE_BORDER_WIDTH + 187;
    y11 = upper_left.y + MODULE_BORDER_WIDTH + 204;
    x_range_high = upper_left.x + (MODULE_WIDTH / 2) + 1;
    w_range = (((MODULE_WIDTH / 2) - MODULE_BORDER_WIDTH) - 3);

    // If the 4th graphics object is null, that means the graphics objects have not
    // been calculated before, and we must make them from scratch
    if((*graphics_objects)[3] == NULL)
    {
        // graphics_objects[3] is the waveform visualizer
        location = {x_text_box, y3, w_waveform, h_waveform};
        object_name = "waveform visualizer (waveform)";
        waveform = new Waveform(&object_name, &location, &WHITE, output);
        (*graphics_objects)[3] = waveform;

        // graphics_objects[4] is the display text "FREQUENCY:"
        location = {x_text, y4, 0, 0};
        object_name = "oscillator frequency (text)";
        contents = "FREQUENCY:";
        text = new Text(&object_name, &location, &text_color, NULL, &contents, FONT_REGULAR);
        (*graphics_objects)[4] = text;

        // graphics_objects[5] is the text box for entering and displaying frequency
        location = {x_text_box, y5, w_text_box, h_text_box};
        object_name = "oscillator frequency (text_box)";
        contents = "";
        prompt = "# or input";
        text_box = new Text_Box(&object_name, &location, &text_color, &(frequency_str),
                                &contents, &prompt, FONT_REGULAR, this);
        (*graphics_objects)[5] = text_box;

        // graphics_objects[6] is the display text "PHASE OFFSET:"
        location = {x_text, y6, 0, 0};
        object_name = "oscillator phase offset (text)";
        contents = "PHASE OFFSET:";
        text = new Text(&object_name, &location, &text_color, NULL, &contents, FONT_REGULAR);
        (*graphics_objects)[6] = text;

        // graphics_objects[7] is the text box for entering and displaying phase offset
        location = {x_text_box, y7, w_text_box, h_text_box};
        object_name = "oscillator phase offset (text_box)";
        contents = "";
        prompt = "# or input";
        text_box = new Text_Box(&object_name, &location, &text_color, &(phase_offset_str),
                                &contents, &prompt, FONT_REGULAR, this);
        (*graphics_objects)[7] = text_box;

        // graphics_objects[8] is the display text "PULSE WIDTH:"
        location = {x_text, y8, 0, 0};
        object_name = "oscillator pulse width (text)";
        contents = "PULSE WIDTH:";
        text = new Text(&object_name, &location, &text_color, NULL, &contents, FONT_REGULAR);
        (*graphics_objects)[8] = text;

        // graphics_objects[9] is the text box for entering and displaying pulse width
        location = {x_text_box, y9, w_text_box, h_text_box};
        object_name = "oscillator pulse width (text_box)";
        contents = "";
        prompt = "# or input";
        text_box = new Text_Box(&object_name, &location, &text_color, &(pulse_width_str),
                                &contents, &prompt, FONT_REGULAR, this);
        (*graphics_objects)[9] = text_box;

        // graphics_objects[10] is the display text "RANGE (LOW - HIGH):"
        location = {x_text, y10, 0, 0};
        object_name = "oscillator range low/high (text)";
        contents = "RANGE (LOW-HIGH):";
        text = new Text(&object_name, &location, &text_color, NULL, &contents, FONT_REGULAR);
        (*graphics_objects)[10] = text;

        // graphics_objects[11] is the text box for entering and displaying range low
        location = {x_text_box, y11, w_range, h_text_box};
        object_name = "oscillator range low (text_box)";
        contents = "";
        prompt = "# or input";
        text_box = new Text_Box(&object_name, &location, &text_color, &(range_low_str),
                                &contents, &prompt, FONT_REGULAR, this);
        (*graphics_objects)[11] = text_box;

        // graphics_objects[12] is the text box for entering and displaying range high
        location = {x_range_high, y11, w_range, h_text_box};
        object_name = "oscillator range high (text_box)";
        contents = "";
        prompt = "# or input";
        text_box = new Text_Box(&object_name, &location, &text_color, &(range_high_str),
                                &contents, &prompt, FONT_REGULAR, this);
        (*graphics_objects)[12] = text_box;
    }
    // Otherwise, simply update the locations of all of the graphics objects
    else
    {
        location = {x_text_box, y3, w_waveform, h_waveform};
        (*graphics_objects)[3]->location = location;

        location = {x_text, y4, 0, 0};
        (*graphics_objects)[4]->location = location;

        location = {x_text_box, y5, w_text_box, h_text_box};
        (*graphics_objects)[5]->location = location;

        location = {x_text, y6, 0, 0};
        (*graphics_objects)[6]->location = location;

        location = {x_text_box, y7, w_text_box, h_text_box};
        (*graphics_objects)[7]->location = location;

        location = {x_text, y8, 0, 0};
        (*graphics_objects)[8]->location = location;

        location = {x_text_box, y9, w_text_box, h_text_box};
        (*graphics_objects)[9]->location = location;

        location = {x_text, y10, 0, 0};
        (*graphics_objects)[10]->location = location;

        location = {x_text_box, y11, w_range, h_text_box};
        (*graphics_objects)[11]->location = location;

        location = {x_range_high, y11, w_range, h_text_box};
        (*graphics_objects)[12]->location = location;
    }
}
