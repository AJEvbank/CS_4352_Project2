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

#define SHOW_INST 1

//Contains the state of the instructions given to the program by the user.
struct instruction_status
{
  bool fail; //If fatal error in command args.

  bool noArgs; //If 1 or no arguments given.
  bool foundOneTarget; //If target(s) found.
  bool openedGivenDirectory; //If where-to-look successfully opened.

  bool cwd; //If starting point is cwd.
  bool given; //If starting point is location.
  char * location;
  bool dot_first_location; // Format of given directory.

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
  char * destination; //Destination for mv instruction.

};

struct stackNode
{
  char * dir_name;
  struct stackNode * next;
};

//Gets the arguments from the command line and sets up the instruction state of
//the program.
void getCommandArgs(int argc, char ** argv, struct instruction_status * instructions);

//Initializes the instruction state of the program and the initial values of the
//instructions.
struct instruction_status * initialize_inst();

//Function to display information about the instructions given to the program by the user.
void display_instruction_status(struct instruction_status * instructions);

//Destroys the instruction state container.
void destroy_inst(struct instruction_status * inst);

//Examines each object in current_dir and runs the appropriate function on it.
void scan_directory(struct instruction_status * inst, char * current_dir);

//Tests and executes the appropriate instructions on the object named temp.
void execute_instructions(struct instruction_status * inst, struct stat buf, char * temp);

//Compares mod to the instructions given by the user and returns true if mod falls
//into the range given by the user.
bool minutes_check(struct instruction_status * inst, time_t mod);

//If the user has provided one or no arguments, then the program will use this function
//instead of scan_directory(). It examines each in object in current_dir object
//and displays the name of the object in the correct format using execute_instructions_noArgs().
void scan_directory_noArgs(struct instruction_status * inst, char * current_dir);

//Displays the name of the object temp in the correct format.
void execute_instructions_noArgs(struct instruction_status * inst, struct stat buf, char * temp);

//Examines the object named temp and returns true if temp falls into the criteria given
//by the user.
bool isTarget(struct instruction_status * inst, struct stat buf, char * temp);
