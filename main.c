#include "main_header.h"

int main(int argc, char ** argv)
{
  int wstatus;
  struct instruction_status * instructions = initialize_inst();

  getCommandArgs(argc,argv,instructions);
  display_instruction_status(instructions);

  if(instructions->noArgs == true)
  {
    scan_directory_noArgs(instructions, instructions->location);
  }
  else
  {
    scan_directory(instructions, instructions->location);
    if(instructions->foundOneTarget == false)
    {
      printf("%s: no such file \n",instructions->target);
    }
  }


  waitpid(-1,&wstatus,0);
  destroy_inst(instructions);
  return 0;
}
