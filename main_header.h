#include "stdio.h"
#include "stdlib.h"
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include "getopt.h"
#include "math.h"
#include "time.h"

struct instruction_status
{
  bool help;

  bool cwd;
  bool given;
  char * location;

  bool name;
  char * target;

  bool mmin;
  bool less_than;
  bool greater_than;
  bool equal_to;
  int minutes;

  bool inum;
  ino_t inode;

  int del;
};

struct stackNode
{
  char * dir_name;
  struct stackNode * next;
};

void getCommandArgs(int argc, char ** argv, struct instruction_status * instructions);

struct instruction_status * initialize_inst();

void display_instruction_status(struct instruction_status * instructions);

void destroy_inst(struct instruction_status * inst);

void scan_directory(struct instruction_status * inst, char * current_dir);

void execute_instructions(struct instruction_status * inst, struct stat buf, char * temp);

bool minutes_check(struct instruction_status * inst, time_t mod);
