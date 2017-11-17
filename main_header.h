#include "stdio.h"
#include "stdlib.h"
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include "getopt.h"
#include "math.h"
#include "time.h"

#define SHOW_OUTPUT 0
#define SHOW_INODES 0
#define SHOW_INODES2 0
#define SHOW_MINS 0

struct instruction_status
{
  bool noArgs;

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

  bool exec;
  bool cat;
  bool rm;
  bool mv;
  char * destination;

};

struct stackNode
{
  char * dir_name;
  struct stackNode * next;
};

extern char **environ;

void getCommandArgs(int argc, char ** argv, struct instruction_status * instructions);

struct instruction_status * initialize_inst();

void display_instruction_status(struct instruction_status * instructions);

void destroy_inst(struct instruction_status * inst);

void scan_directory(struct instruction_status * inst, char * current_dir);

void execute_instructions(struct instruction_status * inst, struct stat buf, char * temp);

bool minutes_check(struct instruction_status * inst, time_t mod);

char * com_string(struct instruction_status * inst);
