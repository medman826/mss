/*
 * Matthew Diamond 2015
 * The output module. All audio requested by the callback
 * function comes through here. This module simply
 * processes the modules it depends upon, then makes available
 * full audio buffers to the audio callback function. This file
 * defines the class.
 */

#ifndef synth_output_h
#define synth_output_h

/************
 * INCLUDES *
 ************/

// No includes necessary

/********************************
 * OUTPUT GRAPHICS OBJECTS ENUM *
 ********************************/

enum Ouput_Graphics_Objects
{
    OUTPUT_AUDIO_TOGGLE_TEXT = 3,
    OUTPUT_AUDIO_TOGGLE_TOGGLE_BUTTON,
    OUTPUT_INPUT_L_WAVEFORM,
    OUTPUT_INPUT_L_TEXT,
    OUTPUT_INPUT_L_TEXT_BOX,
    OUTPUT_INPUT_L_TOGGLE_BUTTON,
    OUTPUT_INPUT_R_WAVEFORM,
    OUTPUT_INPUT_R_TEXT,
    OUTPUT_INPUT_R_TEXT_BOX,
    OUTPUT_INPUT_R_TOGGLE_BUTTON
};

/****************************
 * OUTPUT DEPENDENCIES ENUM *
 ****************************/

enum Ouput_Dependencies
{
    OUTPUT_INPUT_L = 0,
    OUTPUT_INPUT_R
};

/***************************
 * OUTPUT CLASS DEFINITION *
 ***************************/

class Output: public Module
{
    public:
        // Constructor and destructor
        Output(int);
        virtual ~Output();

        // Member functions
        virtual void process();
        virtual void calculate_unique_graphics_objects();
        virtual void update_unique_graphics_objects();
        virtual void update_unique_control_values();
        void toggle_audio_on();
        void set_input_l(Module *);
        void set_input_r(Module *);
};

#endif
