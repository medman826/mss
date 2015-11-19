/*
 * Matthew Diamond 2015
 * Member functions for the VCA class.
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
#include "../function_forwarder.hpp"
#include "../image_processing.hpp"
#include "../main.hpp"
#include "../signal_processing.hpp"

// Included classes
#include "../Module.hpp"
#include "VCA.hpp"
#include "../Graphics_Objects/Input_Text_Box.hpp"
#include "../Graphics_Objects/Input_Toggle_Button.hpp"
#include "../Graphics_Objects/Text.hpp"
#include "../Graphics_Objects/Toggle_Button.hpp"
#include "../Graphics_Objects/Waveform.hpp"

using namespace std;

/************************
 * VCA MEMBER FUNCTIONS *
 ************************/

/*
 * Constructor.
 */
Vca::Vca() :
    Module(VCA)
{
    // The signal input needs to be 0, while the others
    // need to be 1 to start out
    input_floats[0] = 0;
    input_floats[1] = 0;
    input_floats[2] = 1;
}

/*
 * Destructor.
 */
Vca::~Vca()
{

}

/*
 * Process all dependencies, then multiply the original signal by
 * 1 - the control values, and multiply the original signal by
 * the control values scaled. One done, sum the two to get the
 * final output signal for the VCA module.
 */
void Vca::process()
{
    // Process any dependencies
    process_dependencies();

    if(inputs_live[VCA_SIGNAL] && inputs_live[VCA_CV])
    {
        for(unsigned short i = 0; i < output.size(); i ++)
        {
            if(inputs_live[VCA_CV_AMOUNT])
                input_floats[VCA_CV_AMOUNT] = inputs[VCA_CV_AMOUNT]->at(i);

            output[i] = (inputs[VCA_SIGNAL]->at(i) * (1 - input_floats[VCA_CV_AMOUNT])) +
                           (inputs[VCA_SIGNAL]->at(i) * inputs[VCA_CV]->at(i) * input_floats[VCA_CV_AMOUNT]);
        }
    }

    processed = true;
}

void Vca::update_control_values()
{
    
}

void Vca::calculate_unique_graphics_object_locations()
{
    int x_text, x_text_box, w_text_box, h_text_box,
        x_input_toggle_button, w_input_toggle_button,
        w_waveform, h_waveform,
        x_signal_cv, x_signal_input_toggle_button, w_signals,
        y3, y4, y5, y6, y7;

    x_text = upper_left.x + MODULE_BORDER_WIDTH + 2;
    x_text_box = upper_left.x + MODULE_BORDER_WIDTH + 2;
    w_text_box = ((MODULE_WIDTH - (MODULE_BORDER_WIDTH * 2)) - 4) - 11;
    h_text_box = 15;
    x_input_toggle_button = x_text_box + w_text_box + 1;
    w_input_toggle_button = 10;
    w_waveform = ((MODULE_WIDTH - (MODULE_BORDER_WIDTH * 2)) - 4);
    h_waveform = 135;
    y3 = upper_left.y + MODULE_BORDER_WIDTH + 23;
    y4 = y3 + 136;
    y5 = y4 + 15;
    y6 = y5 + 15;
    y7 = y6 + 15;
    x_signal_cv = upper_left.x + (MODULE_WIDTH / 2) + 1;
    w_signals = (((MODULE_WIDTH / 2) - MODULE_BORDER_WIDTH) - 3) - 11;
    x_signal_input_toggle_button = x_text_box + w_signals + 1;

    graphics_object_locations.push_back({x_text, y4, 0, 0});
    graphics_object_locations.push_back({x_text, y6, 0, 0});
    graphics_object_locations.push_back({x_text_box, y3, w_waveform, h_waveform});
    graphics_object_locations.push_back({x_text_box, y5, w_signals, h_text_box});
    graphics_object_locations.push_back({x_signal_cv, y5, w_signals, h_text_box});
    graphics_object_locations.push_back({x_text_box, y7, w_text_box, h_text_box});
    graphics_object_locations.push_back({x_signal_input_toggle_button, y5, w_input_toggle_button, h_text_box});
    graphics_object_locations.push_back({x_input_toggle_button, y5, w_input_toggle_button, h_text_box});
    graphics_object_locations.push_back({x_input_toggle_button, y7, w_input_toggle_button, h_text_box});
}

/*
 * Calculate the locations of any graphics objects that are
 * unique to this module type.
 */
void Vca::initialize_unique_graphics_objects()
{
    vector<string> names, texts, prompt_texts, text_offs;
    vector<SDL_Rect> locations;
    vector<SDL_Color> colors, background_colors, color_offs, text_colors, text_color_ons, text_color_offs;
    vector<TTF_Font *> fonts;
    vector<float> range_lows, range_highs;
    vector<int> input_nums;
    vector<vector<float> *> buffers;
    vector<Module *> parents;
    vector<bool> bs;

    names = {"vca signal & cv input (text)", "vca cv amount (text)"};
    locations = {graphics_object_locations[VCA_INPUT_TEXT],
                 graphics_object_locations[VCA_CV_AMOUNT_TEXT]};
    colors = vector<SDL_Color>(2, text_color);
    texts = {"SIGNAL & CV INPUT:", "CV AMOUNT:"};
    fonts = vector<TTF_Font *>(2, FONT_REGULAR);

    create_text_objects(names, locations, colors, texts, fonts);

    names = {"waveform visualizer (waveform)"};
    locations = {graphics_object_locations[VCA_OUTPUT_WAVEFORM]};
    colors = {color};
    background_colors = {text_color};
    range_lows = {-1};
    range_highs = {1};
    buffers = {&output};

    create_waveform_objects(names, locations, colors, background_colors, range_lows, range_highs, buffers);

    names = {"vca signal (input text box)", "vca cv (input text box)", "vca cv amount (input text box)"};
    locations = {graphics_object_locations[VCA_SIGNAL_INPUT_TEXT_BOX],
                 graphics_object_locations[VCA_CV_INPUT_TEXT_BOX],
                 graphics_object_locations[VCA_CV_AMOUNT_INPUT_TEXT_BOX]};
    colors = vector<SDL_Color>(3, text_color);
    text_colors = vector<SDL_Color>(3, color);
    prompt_texts = {"input", "input", "# or input"};
    fonts = vector<TTF_Font *>(3, FONT_SMALL);
    parents = vector<Module *>(3, this);
    input_nums = {VCA_SIGNAL, VCA_CV, VCA_CV_AMOUNT};

    create_input_text_box_objects(names, locations, colors, text_colors, prompt_texts, fonts, parents, input_nums);

    names = {"vca signal input (input toggle button)", "vca cv input (input toggle button)",
             "vca cv amount input (input toggle button)"};
    locations = {graphics_object_locations[VCA_SIGNAL_INPUT_TOGGLE_BUTTON],
                 graphics_object_locations[VCA_CV_INPUT_TOGGLE_BUTTON],
                 graphics_object_locations[VCA_CV_AMOUNT_INPUT_TOGGLE_BUTTON]};
    colors = vector<SDL_Color>(3, RED);
    color_offs = vector<SDL_Color>(3, text_color);
    text_color_ons = vector<SDL_Color>(3, WHITE);
    text_color_offs = vector<SDL_Color>(3, color);
    fonts = vector<TTF_Font *>(3, FONT_SMALL);
    texts = vector<string>(3, "I");
    text_offs = texts;
    bs = {inputs_live[VCA_SIGNAL], inputs_live[VCA_CV],
          inputs_live[VCA_CV_AMOUNT]};
    parents = vector<Module *>(3, this);
    input_nums = {VCA_SIGNAL, VCA_CV,
                  VCA_CV_AMOUNT};

    create_input_toggle_button_objects(names, locations, colors, color_offs, text_color_ons,
                                       text_color_offs, fonts, texts, text_offs, bs, parents, input_nums);
}
