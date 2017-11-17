#include "main_header.h"

int main(int argc, char ** argv)
{
  int wstatus;
  struct instruction_status * instructions = initialize_inst();

  getCommandArgs(argc,argv,instructions);
  display_instruction_status(instructions);


  scan_directory(instructions, instructions->location);


  waitpid(-1,&wstatus,0);
  printf("\n\n");
  destroy_inst(instructions);
  return 0;
}
