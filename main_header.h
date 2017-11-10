#include "stdio.h"
#include "stdlib.h"
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <stdbool.h>
#include <stdint.h>
#include "getopt.h"

struct instruction_status
{
  bool help;

  char * location;
  bool where;

  char * target;
  bool name;

  int minutes;
  bool mmin;

  ino_t inode;
  bool inum;

  int del;
};

struct stackNode
{
  DIR * dir_name;
  struct stackNode * next;
};

void getCommandArgs(int argc, char ** argv, struct instruction_status * instructions);

struct instruction_status * initialize_inst();

void display_instruction_status(struct instruction_status * instructions);

void destroy_inst(struct instruction_status * inst);

void scan_directory(struct instruction_status * inst, char * current_dir);
