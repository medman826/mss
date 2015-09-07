/*
 * Matthew Diamond 2015
 * Header file for the driver functions.
 */

#ifndef synth_main_h
#define synth_main_h

// Included libraries
#include <vector>

// Included SDL components
#include "SDL2/SDL.h"

// Included classes
#include "Module.hpp"

// Audio information
extern int SAMPLE_RATE;
extern unsigned long long int CURRENT_SAMPLE;
extern int AUDIO_LENGTH;
extern float FREQUENCY;
extern int BUFFER_SIZE;

// The modules currently in use
extern std::vector<Module *> modules;

void audio_callback(void *, Uint8 *, int);

#endif