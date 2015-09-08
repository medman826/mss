/*
 * Matthew Diamond 2015
 * Member functions for the Output class.
 */

// Included libraries
#include <string>

// Included classes
#include "Module.hpp"
#include "Output.hpp"

using namespace std;

/*
 * Constructor
 */
Output::Output(void)
{
  name = "output";
  type = OUTPUT;
  input_l = NULL;
  input_r = NULL;
}

/*
 * Dummy function
 */
Output::~Output(void)
{

}

/*
 * This function simply calls upon dependencies for
 * processing. The output module depends on all other
 * modules.
 */
void Output::process()
{
  process_depends();
}