#include "main_header.h"

void getCommandArgs(int argc, char ** argv, struct instruction_status * inst)
{
  int ch,opt_index,mins;

  if (argc >= 2)
  {
    if (argv[1][0] != '-')
    {
      inst->location = argv[1];
      inst->given = true;
    }
    else
    {
      inst->cwd = true;
      inst->location = "";
    }
  }
  else
  {
    inst->cwd = true;
    inst->help = true;
    inst->location = "";
    return;
  }

  static struct option OPTS[] =
  {
    { "name", required_argument, 0, 'n' },
    { "mmin", required_argument, 0, 'm' },
    { "inum", required_argument, 0, 'i' },
    { "delete", no_argument, 0, 'd' },
    {0,0,0,0}
  };

  while (1)
  {
    ch = getopt_long_only(argc,argv,"",OPTS,&opt_index);
    if (ch == -1) { break; }

    switch (ch)
    {
      case 'n':
                inst->target = optarg;
                inst->name = true;
                break;

      case 'm':
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

      case 'i':
                inst->inode = strtoull(optarg,NULL,10);
                inst->inum = true;
                break;
      case 'd':
                inst->del = true;
                break;
      default:
                break;
    }
  }
  return;
}

struct instruction_status * initialize_inst()
{
  struct instruction_status * rtrn = (struct instruction_status *)malloc(sizeof(struct instruction_status));
  rtrn->location = NULL;
  rtrn->target = NULL;
  rtrn->minutes = 0;
  rtrn->inode = 0;
  rtrn->cwd = false;
  rtrn->given = false;
  rtrn->name = false;
  rtrn->mmin = false;
  rtrn->less_than = false;
  rtrn->greater_than = false;
  rtrn->equal_to = false;
  rtrn->help = false;
  rtrn->del = false;
  return rtrn;
}

void destroy_inst(struct instruction_status * inst)
{
  free(inst);
  return;
}

void scan_directory(struct instruction_status * inst, char * current_dir)
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
  char * pass = NULL;
  char tempSelf[] = ".";
  char tempParent[] = "..";
  char tempSlash[] = "/";
  struct stackNode * dirStack = NULL;

  if (directory != NULL)
  {

  /* Look at each entry in the directory. */
    while((dir_entry = readdir(directory)) != NULL)
    {
      if(strcmp(dir_entry->d_name,tempSelf) != 0 && strcmp(dir_entry->d_name,tempParent) != 0)
      {
        if (inst->cwd == true && strlen(current_dir) == 0)
        {
          temp[0] = '\0';
          temp = strcat(temp,tempSelf);
          temp = strcat(temp,tempSlash);
          temp = strcat(temp,dir_entry->d_name);
        }
        else
        {
          temp[0] = '\0';
          temp = strcat(temp,tempSelf);
          temp = strcat(temp,tempSlash);
          temp = strcat(temp,current_dir);
          temp = strcat(temp,tempSlash);
          temp = strcat(temp,dir_entry->d_name);
        }
        if(stat(temp,&buf) < 0)
        {
          printf("Could not get attributes for %s. \n",temp);
        }
        else
        {
          /* If the entry is a file, run test and execute. */
          if (S_ISREG(buf.st_mode))
          {
            //printf("%s is a file! \n",temp);
            execute_instructions(inst, buf, temp);
          }
          /* If the entry is a directory, run test and, if necessary, push it onto the directory stack. */
          else if (S_ISDIR(buf.st_mode))
          {
            //printf("%s is a directory! \n",temp);
            if (inst->help == true) { printf("%s\n",temp); }
            scan_directory(inst,&temp[2]);
          }
          else
          {
            printf("%s is not a file or a directory \n",temp);
          }
        }
      }
      else if (inst->help == true && strcmp(dir_entry->d_name,tempSelf) == 0 && strlen(current_dir) == 0)
      {
        printf("%s \n",dir_entry->d_name);
      }
    }
    closedir(directory);
  }
  else
  {
    printf("Cannot open directory: %s \n",current_dir);
    free(temp);
    return;
  }
  free(temp);
  return;
}

void execute_instructions(struct instruction_status * inst, struct stat buf, char * temp)
{
  //printf("inode of %s = %lu \n",temp,buf.st_ino);
  bool isTarget = false;
  if (inst->help == true)
  {
    printf("%s \n",temp);
    return;
  }
  if (inst->name == true)
  {
    if ((strcmp(inst->target,&temp[2]) == 0)) { isTarget = true; } else { isTarget = false; }
  }
  if (inst->mmin == true) { isTarget = minutes_check(inst, buf.st_mtime); }
  if (inst->inum == true)
  {
    if(inst->inode == buf.st_ino) { isTarget = true; } else { isTarget = false; }
  }
  if (isTarget)
  {
    printf("%s \n",&temp[2]);
    if (inst->del == true)
    {
      printf("delete %s \n",temp);
    }
  }
  return;
}

bool minutes_check(struct instruction_status * inst, time_t mod)
{
  time_t min = (time(NULL) - mod)/60;
  bool rtrn = false;
  if(inst->less_than == true && min < inst->minutes)
  {
    rtrn = true;
  }
  else if (inst->greater_than == true && min > inst->minutes)
  {
    rtrn = true;
  }
  else if (inst->equal_to == true && min == inst->minutes)
  {
    rtrn = true;
  }
  return rtrn;
}
