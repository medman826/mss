/*
 * Matthew Diamond 2015
 * Member functions for the Output class.
 */

/************
 * INCLUDES *
 ************/

// Included libraries
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
#include "../Graphics_Objects/Input_Text_Box.hpp"
#include "../Graphics_Objects/Input_Toggle_Button.hpp"
#include "../Graphics_Objects/Toggle_Button.hpp"
#include "../Graphics_Objects/Waveform.hpp"
#include "../Module.hpp"
#include "Output.hpp"

using namespace std;

/***************************
 * OUTPUT MEMBER FUNCTIONS *
 ***************************/

/*
 * Constructor.
 */
Output::Output() :
    Module(OUTPUT)
{

}

/*
 * Destructor.
 */
Output::~Output()
{

}

/*
 * This function simply calls upon dependencies for
 * processing. The output module depends on all other
 * modules.
 */
void Output::process()
{
    process_dependencies();
}

void Output::update_control_values()
{

}

void Output::calculate_unique_graphics_object_locations()
{
    int x_text, x_text_box, w_text_box, h_text_box,
        x_input_toggle_button, w_input_toggle_button,
        x_button,
        w_waveform, h_waveform,
        y3, y4, y5, y6, y7, y8, y9, y10;

    x_text = upper_left.x + MODULE_BORDER_WIDTH + 2;
    x_text_box = upper_left.x + MODULE_BORDER_WIDTH + 2;
    w_text_box = ((MODULE_WIDTH - (MODULE_BORDER_WIDTH * 2)) - 4) - 10;
    h_text_box = 15;
    x_input_toggle_button = x_text_box + w_text_box;
    w_input_toggle_button = 10;
    x_button = upper_left.x + MODULE_BORDER_WIDTH + 70;
    w_waveform = ((MODULE_WIDTH - (MODULE_BORDER_WIDTH * 2)) - 4);
    h_waveform = 55;
    y3 = upper_left.y + MODULE_BORDER_WIDTH + 20;
    y4 = upper_left.y + MODULE_BORDER_WIDTH + 20;
    y5 = upper_left.y + MODULE_BORDER_WIDTH + 40;   
    y6 = upper_left.y + MODULE_BORDER_WIDTH + 97;
    y7 = upper_left.y + MODULE_BORDER_WIDTH + 112;
    y8 = upper_left.y + MODULE_BORDER_WIDTH + 133;
    y9 = upper_left.y + MODULE_BORDER_WIDTH + 189;
    y10 = upper_left.y + MODULE_BORDER_WIDTH + 204;

    graphics_object_locations.push_back({x_text, y3, 0, 0});
    graphics_object_locations.push_back({x_text, y6, 0, 0});
    graphics_object_locations.push_back({x_text, y9, 0, 0});
    graphics_object_locations.push_back({x_text_box, y5, w_waveform, h_waveform});
    graphics_object_locations.push_back({x_text_box, y8, w_waveform, h_waveform});
    graphics_object_locations.push_back({x_text_box, y7, w_text_box, h_text_box});
    graphics_object_locations.push_back({x_text_box, y10, w_text_box, h_text_box});
    graphics_object_locations.push_back({x_input_toggle_button, y7, w_input_toggle_button, h_text_box});
    graphics_object_locations.push_back({x_input_toggle_button, y10, w_input_toggle_button, h_text_box});
    graphics_object_locations.push_back({x_button, y4, 25, 15});
}

/*
 * Calculate the locations of any graphics objects that are
 * unique to this module type.
 */
void Output::initialize_unique_graphics_objects()
{
    vector<string> names, texts, prompt_texts, text_offs;
    vector<SDL_Rect> locations;
    vector<SDL_Color> colors, background_colors, color_offs, text_color_ons, text_color_offs;
    vector<TTF_Font *> fonts;
    vector<float> range_lows, range_highs;
    vector<int> input_nums;
    vector<vector<float> *> buffers;
    vector<Module *> parents;
    vector<bool> bs;

    names = {"on/off (text)", "output input left (text)", "output input right (text)"};
    locations = {graphics_object_locations[OUTPUT_AUDIO_TOGGLE_TEXT],
                 graphics_object_locations[OUTPUT_INPUT_L_TEXT],
                 graphics_object_locations[OUTPUT_INPUT_R_TEXT]};
    colors = vector<SDL_Color>(3, text_color);
    texts = {"AUDIO ON:", "LEFT SIGNAL:", "RIGHT SIGNAL:"};
    fonts = vector<TTF_Font *>(3, FONT_REGULAR);

    create_text_objects(names, locations, colors, texts, fonts);

    names = {"waveform visualizer l (waveform)", "waveform visualizer r (waveform)"};
    locations = {graphics_object_locations[OUTPUT_INPUT_L_WAVEFORM],
                 graphics_object_locations[OUTPUT_INPUT_R_WAVEFORM]};
    colors = vector<SDL_Color>(2, color);
    background_colors = vector<SDL_Color>(2, text_color);
    range_lows = vector<float>(2, -1);
    range_highs = vector<float>(2, 1);
    buffers = vector<vector<float> *>(2, NULL);

    create_waveform_objects(names, locations, colors, background_colors, range_lows, range_highs, buffers);

    names = {"output input l (input text box)", "output input r (input text box)"};
    locations = {graphics_object_locations[OUTPUT_INPUT_L_INPUT_TEXT_BOX],
                 graphics_object_locations[OUTPUT_INPUT_R_INPUT_TEXT_BOX]};
    colors = vector<SDL_Color>(2, text_color);
    texts = vector<string>(2, "");
    prompt_texts = vector<string>(2, "input");
    fonts = vector<TTF_Font *>(2, FONT_SMALL);
    parents = vector<Module *>(2, this);
    input_nums = {OUTPUT_INPUT_L, OUTPUT_INPUT_R};

    create_input_text_box_objects(names, locations, colors, texts, prompt_texts, fonts, parents, input_nums);

    names = {"output input l (input toggle button)", "output input r (input toggle button)"};
    locations = {graphics_object_locations[OUTPUT_INPUT_L_INPUT_TOGGLE_BUTTON],
                 graphics_object_locations[OUTPUT_INPUT_R_INPUT_TOGGLE_BUTTON]};
    colors = vector<SDL_Color>(2, WHITE);
    color_offs = vector<SDL_Color>(2, BLACK);
    text_color_ons = vector<SDL_Color>(2, RED);
    text_color_offs = vector<SDL_Color>(2, WHITE);
    fonts = vector<TTF_Font *>(2, FONT_SMALL);
    texts = vector<string>(2, "I");
    text_offs = texts;
    bs = vector<bool>(2, false);
    parents = vector<Module *>(2, this);
    input_nums = {OUTPUT_INPUT_L, OUTPUT_INPUT_R};

    create_input_toggle_button_objects(names, locations, colors, color_offs, text_color_ons,
                                       text_color_offs, fonts, texts, text_offs, bs, parents, input_nums);

    names = {"on/off button (toggle_button)"};
    locations = {graphics_object_locations[OUTPUT_AUDIO_TOGGLE_TOGGLE_BUTTON]};
    colors = vector<SDL_Color>(1, GREEN);
    color_offs = vector<SDL_Color>(1, BLACK);
    text_color_ons = vector<SDL_Color>(1, BLACK);
    text_color_offs = vector<SDL_Color>(1, WHITE);
    fonts = vector<TTF_Font *>(1, FONT_BOLD);
    texts = vector<string>(1, "ON");
    text_offs = vector<string>(1, "OFF");
    bs = {true};
    parents = vector<Module *>(1, this);

    create_toggle_button_objects(names, locations, colors, color_offs, text_color_ons,
                                 text_color_offs, fonts, texts, text_offs, bs, parents);
}

void Output::toggle_audio_on()
{
    AUDIO_ON = !AUDIO_ON;

    if(AUDIO_ON)
        SDL_PauseAudio(0);
    else
        SDL_PauseAudio(1);

    cout << "Audio toggled" << endl;
}
