#include "main_header.h"

int main(int argc, char ** argv)
{
  int wstatus;
  struct instruction_status * instructions = initialize_inst();

  getCommandArgs(argc,argv,instructions);
  if (SHOW_INST) display_instruction_status(instructions);

  if (instructions->fail == false)
  {
    if(instructions->noArgs == true)
    {
      scan_directory_noArgs(instructions, instructions->location);
    }
    else
    {
      scan_directory(instructions, instructions->location);
      if(instructions->openedGivenDirectory == true && instructions->foundOneTarget == false)
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
