#include "main_header.h"

int main(int argc, char ** argv)
{
  struct instruction_status * instructions = initialize_inst();

  getCommandArgs(argc,argv,instructions);
  display_instruction_status(instructions);

  scan_directory(instructions, instructions->location);



  printf("\n\n");
  destroy_inst(instructions);
  return 0;
}
