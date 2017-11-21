#include "main_header.h"

int main(int argc, char ** argv)
{
  int wstatus;
  struct instruction_status * instructions = initialize_inst();

  getCommandArgs(argc,argv,instructions);
  if (SHOW_INST) display_instruction_status(instructions);

  if (instructions->fail == false) //If the user provided invalid instructions, abort.
  {
    if(instructions->noArgs == true) //In the case of 1 or 0 arguments.
    {
      scan_directory_noArgs(instructions, instructions->location);
    }
    else //In the case of 2 or more arguments.
    {
      scan_directory(instructions, instructions->location);
      if(instructions->openedGivenDirectory == true && instructions->foundOneTarget == false) //If no file was found matching the criteria.
      {
        if (instructions->name == true)
        {
          printf("%s: not found \n",instructions->target);
        }
        else
        {
          printf("target not found \n");
        }

      }
    }
  }

  waitpid(-1,&wstatus,0);
  destroy_inst(instructions);
  return 0;
}
