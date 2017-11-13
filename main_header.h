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

struct instruction_status
{
  bool help;

  bool where;
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

enum action
{
  EXIT_TARGET_FOUND,
  CONTINUE_SCAN
};

void getCommandArgs(int argc, char ** argv, struct instruction_status * instructions);

struct instruction_status * initialize_inst();

void display_instruction_status(struct instruction_status * instructions);

void destroy_inst(struct instruction_status * inst);

void scan_directory(struct instruction_status * inst, char * current_dir);

struct stackNode * push(char * dir_name, struct stackNode * base);

struct stackNode * pop(struct stackNode * base);
