#include "main_header.h"

void getCommandArgs(int argc, char ** argv, struct instruction_status * inst)
//Gets the arguments from the command line and sets up the instruction state of
//the program.
{
  int ch,opt_index;

  if (argc == 1) //If the user provided no arguments...
  {
    inst->cwd = true;
    inst->noArgs = true;
    inst->location = "";
    inst->dot_first_location = true;
    return;
  }
  else if(argc == 2) //If the user provided only one argument.
  {
    inst->noArgs = true;
    inst->location = argv[1];
    inst->given = true;
    if (argv[1][0] == '.') { inst->dot_first_location = true; }
    if (strcmp(argv[1],".") == 0) { inst->cwd = true; inst->location = ""; inst->given = false; }
    return;
  }
  else
  {
    if (argv[1][0] != '-' && strcmp(argv[1],".") != 0)
    {
      inst->location = argv[1];
      inst->given = true;
      if (argv[1][0] == '.') { inst->dot_first_location = true; }
    }
    else
    {
      inst->cwd = true;
      inst->location = "";
      inst->dot_first_location = true;
    }
  }

  static struct option OPTS[] =
  {
    { "name", required_argument, 0, 'n' },
    { "mmin", required_argument, 0, 'm' },
    { "inum", required_argument, 0, 'i' },
    { "delete", no_argument, 0, 'd' },
    { "exec", required_argument, 0, 'e' },
    {0,0,0,0}
  };

  while (1)
  {
    ch = getopt_long_only(argc,argv,"",OPTS,&opt_index);
    if (ch == -1) { break; }

    switch (ch)
    {
      case 'n': //-name
                inst->target = optarg;
                inst->name = true;
                break;

      case 'm': //-mmin
                inst->mmin = true;
                inst->minutes = fabs(atoi(optarg));
                if (optarg[0] == '-')
                {
                  inst->less_than = true;
                }
                else if (optarg[0] == '+')
                {
                  inst->greater_than = true;
                }
                else
                {
                  inst->equal_to = true;
                }
                break;

      case 'i': //-inum
                inst->inode = strtoull(optarg,NULL,10);
                inst->inum = true;
                break;
      case 'd': //-delete
                if(inst->rm == false)
                {
                  inst->del = true;
                }
                break;
      case 'e': //-exec
                inst->exec = true;
                bool criteria = (inst->name == true || inst->mmin == true || inst->inum == true);
                if (strcmp(optarg,"cat") == 0)
                {
                  if (criteria) //Cat needs criteria.
                  {
                    inst->cat = true;
                  }
                  else
                  {
                    printf("criteria -inum, -name, or -mmin required for cat \n");
                    inst->fail = true;
                  }
                }
                else if (strcmp(optarg,"rm") == 0)
                {
                  if (criteria) //Rm needs criteria.
                  {
                    inst->rm = true;
                    inst->del = false; //Rm overrides -delete.
                  }
                  else
                  {
                    printf("criteria -inum, -name, or -mmin required for rm \n");
                    inst->fail = true;
                  }
                }
                else if (strcmp(optarg,"mv") == 0)
                {
                  if (criteria) //Mv needs criteria.
                  {
                    inst->mv = true;
                    if (optind < argc)
                    {
                      inst->destination = argv[optind];
                    }
                    else
                    {
                      printf("No valid destination provided for option -exec mv. \n");
                      inst->mv = false;inst->exec = false; inst->fail = true;
                    }
                  }
                  else
                  {
                    printf("criteria -inum, -name, or -mmin required for mv \n");
                    inst->fail = true;
                  }

                }
                else
                {
                  printf("No valid argument provided for option -exec. \n");
                  inst->exec = false; inst->fail = true;
                }
                break;
      default:
                break;
    }
  }
  return;
}

struct instruction_status * initialize_inst()
//Initializes the instruction state of the program and the initial values of the
//instructions.
{
  struct instruction_status * rtrn = (struct instruction_status *)malloc(sizeof(struct instruction_status));
  rtrn->location = NULL;
  rtrn->target = NULL;
  rtrn->minutes = 0;
  rtrn->inode = 0;
  rtrn->cwd = false;
  rtrn->given = false;
  rtrn->dot_first_location = false;
  rtrn->name = false;
  rtrn->mmin = false;
  rtrn->less_than = false;
  rtrn->greater_than = false;
  rtrn->equal_to = false;
  rtrn->noArgs = false;
  rtrn->foundOneTarget = false;
  rtrn->openedGivenDirectory = false;
  rtrn->del = false;
  rtrn->exec = false;
  rtrn->cat = false;
  rtrn->rm = false;
  rtrn->mv = false;
  rtrn->fail = false;
  return rtrn;
}

void destroy_inst(struct instruction_status * inst)
//Destroys the instruction state container.
{
  free(inst);
  return;
}

void scan_directory(struct instruction_status * inst, char * current_dir)
//Examines each object in current_dir and runs the appropriate function on it.
{
  DIR * directory = NULL;
  /* Open the directory */
  if (inst->cwd == true && strlen(current_dir) == 0)
  {
    directory = opendir(".");
  }
  else
  {
    directory = opendir(current_dir);
  }

  struct dirent * dir_entry;
  struct stat buf;
  char * temp = (char *)malloc(sizeof(char) * 256);
  char tempSelf[] = ".";
  char tempParent[] = "..";
  char tempSlash[] = "/";

  if (directory != NULL)
  {
    inst->openedGivenDirectory = true; //Notes if successfully opened.
  /* Look at each entry in the directory. */
    while((dir_entry = readdir(directory)) != NULL)
    {
      if(strcmp(dir_entry->d_name,tempSelf) != 0 && strcmp(dir_entry->d_name,tempParent) != 0 && strcmp(dir_entry->d_name,"find") != 0)
      {
        if ((inst->cwd == true && strlen(current_dir) == 0)) //If in cwd, then no need for current_dir in name.
        {
          temp[0] = '\0';
          temp = strcat(temp,tempSelf);
          temp = strcat(temp,tempSlash);
          temp = strcat(temp,dir_entry->d_name);
        }
        else
        {
          temp[0] = '\0';
          if(strncmp(current_dir,"./",2) != 0)
          {
            temp = strcat(temp,tempSelf);
            temp = strcat(temp,tempSlash);
          }
          temp = strcat(temp,current_dir);
          temp = strcat(temp,tempSlash);
          temp = strcat(temp,dir_entry->d_name);
        }
        if(stat(temp,&buf) < 0) //Get the file attributes.
        {
          printf("Could not get attributes for %s. \n",temp);
        }
        else
        {
          /* If the entry is a file, run test and execute. */
          if (S_ISREG(buf.st_mode))
          {
            execute_instructions(inst, buf, temp);
          }
          /* If the entry is a directory, run test and call scan_directory on it. */
          else if (S_ISDIR(buf.st_mode))
          {
            scan_directory(inst,temp);
          }
          else
          {
            printf("%s is not a file or a directory \n",temp);
          }
        }
      }
    }
    closedir(directory);
  }
  else
  {
    printf("Cannot open directory: %s \n",current_dir);
  }
  free(temp);
  return;
}

void scan_directory_noArgs(struct instruction_status * inst, char * current_dir)
//If the user has provided one or no arguments, then the program will use this function
//instead of scan_directory(). It examines each in object in current_dir object
//and displays the name of the object in the correct format using execute_instructions_noArgs().
{
  DIR * directory = NULL;
  /* Open the directory */
  if (inst->cwd == true && strlen(current_dir) == 0)
  {
    directory = opendir(".");
  }
  else
  {
    directory = opendir(current_dir);
  }

  struct dirent * dir_entry;
  struct stat buf;
  char * temp = (char *)malloc(sizeof(char) * 256);
  char tempSelf[] = ".";
  char tempParent[] = "..";
  char tempSlash[] = "/";

  if (directory != NULL)
  {
    inst->openedGivenDirectory = true;
  /* Look at each entry in the directory. */
    while((dir_entry = readdir(directory)) != NULL)
    {
      if(strcmp(dir_entry->d_name,tempSelf) != 0 && strcmp(dir_entry->d_name,tempParent) != 0 && strcmp(dir_entry->d_name,"find") != 0)
      {
        if (inst->cwd == true && strlen(current_dir) == 0) //If in cwd, no need for current_dir in name.
        {
          temp[0] = '\0';
          temp = strcat(temp,tempSelf);
          temp = strcat(temp,tempSlash);
          temp = strcat(temp,dir_entry->d_name);
        }
        else
        {
          temp[0] = '\0';
          if(strncmp(current_dir,"./",2) != 0)
          {
            temp = strcat(temp,tempSelf);
            temp = strcat(temp,tempSlash);
          }
          temp = strcat(temp,current_dir);
          temp = strcat(temp,tempSlash);
          temp = strcat(temp,dir_entry->d_name);
        }
        if(stat(temp,&buf) < 0) //Get the file attributes.
        {
          printf("Could not get attributes for %s. \n",temp);
        }
        else
        {
          /* If the entry is a file, run test and execute. */
          if (S_ISREG(buf.st_mode))
          {
            execute_instructions_noArgs(inst, buf, temp);
          }
          /* If the entry is a directory, call scan_directory_noArgs on it. */
          else if (S_ISDIR(buf.st_mode))
          {
            execute_instructions_noArgs(inst, buf, temp);
            scan_directory_noArgs(inst,temp);
          }
          else
          {
            printf("%s is not a file or a directory \n",temp);
          }
        }
      }
      else if (strcmp(dir_entry->d_name,tempSelf) == 0 && strcmp(inst->location,current_dir) == 0 && inst->dot_first_location == true)
      {
        printf("%s \n",dir_entry->d_name); //Print "." in where-to-look directory given by user.
      }
    }
    closedir(directory);
  }
  else
  {
    printf("Cannot open directory: %s \n",current_dir);
  }
  free(temp);
  return;
}

void execute_instructions(struct instruction_status * inst, struct stat buf, char * temp)
//Tests and executes the appropriate instructions on the object named temp.
{
  if (isTarget(inst,buf,temp)) //If the file fits the user's given criteria.
  {
    inst->foundOneTarget = true;
    if (inst->exec == true) //-exec overrides other instructions.
    {
      char * arg0 = (char *)malloc(sizeof(char) * 128);
      char **arg1 = (char **)malloc(sizeof(char *) * 32);
      if(inst->cat == true) //Open and print the file.
      {
        char ch;
        FILE * fd = fopen(temp,"r");
        if (fd == NULL) { printf("Cannot open %s. \n",temp); }
        ch = fgetc(fd);
        while (ch != EOF) { printf("%c",ch); ch = fgetc(fd); }
        fclose(fd);
        return;
      }
      else if(inst->rm == true)  // Build the command string for execv call.
      {
        arg0 = strcpy(arg0,"/bin/rm");
        arg1[0] = "rm";
        arg1[1] = temp;
        arg1[2] = (char *)0;
      }
      else if(inst->mv == true)  // Build the command string for execv call.
      {
        arg0 = "/bin/mv";
        arg1[0] = "mv";
        arg1[1] = temp;
        arg1[2] = inst->destination;
        arg1[3] = (char *)0;
      }
      if (fork() == 0) // Create child process to be overwritten by execv call.
      {
        if(execv(arg0,arg1) == -1) // Run system call.
        {
          printf("%s failed on %s \n",arg0,temp);
          exit(0);
          return;
        }
      }
      else
      {
        return;
      }
    }
    else if (inst->del == true) //-delete overrides displaying the name
    {
      if(remove(temp) != 0){ printf("remove failed on %s \n",temp); }
    }
    else // The default instruction to execute.
    {
      if(inst->dot_first_location == false) { printf("%s\n",&temp[2]); } //Display the file name in the correct format.
      else { printf("%s\n",temp); }
    }
  }
  return;
}

void execute_instructions_noArgs(struct instruction_status * inst, struct stat buf, char * temp)
//Displays the name of the object temp in the correct format.
{
  if (inst->dot_first_location == true) // Test for the correct format.
  {
    printf("%s \n",temp);
  }
  else
  {
    printf("%s \n",&temp[2]);
  }
  return;
}

bool minutes_check(struct instruction_status * inst, time_t mod)
//Compares mod to the instructions given by the user and returns true if mod falls
//into the range given by the user.
{
  time_t min = (time(NULL) - mod)/60; // Get the time difference between the present and mod.
  bool rtrn = false;
  if(inst->less_than == true && min < inst->minutes) // If required and passes less_than test.
  {
    rtrn = true;
  }
  else if (inst->greater_than == true && min > inst->minutes) // If required and passes greater_than test.
  {
    rtrn = true;
  }
  else if (inst->equal_to == true && min == inst->minutes) // If required and passes equal_to test.
  {
    rtrn = true;
  }
  return rtrn;
}

bool isTarget(struct instruction_status * inst, struct stat buf, char * temp)
//Examines the object named temp and returns true if temp falls into the criteria given
//by the user.
{
  bool isTarget = false;
  char * file_nombre = strrchr(temp,'/'); // Marks object name w/o path.

  if (inst->name == true) // If required and passes, isTarget = true, otherwise return false.
  {
    if ((strcmp(inst->target,&file_nombre[1]) == 0)) { isTarget = true; } else { return false; }
  }
  if (inst->mmin == true) { isTarget = minutes_check(inst, buf.st_mtime); if(!isTarget) return false; } // If required and passes, isTarget = true, otherwise return false.
  if (inst->inum == true) // If required and passes, isTarget = true, otherwise return false.
  {
    if(inst->inode == buf.st_ino) { isTarget = true; } else { return false; }
  }
  return isTarget;
}
