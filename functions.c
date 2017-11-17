#include "main_header.h"

void getCommandArgs(int argc, char ** argv, struct instruction_status * inst)
{
  int ch,opt_index;

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
    inst->noArgs = true;
    inst->location = "";
    return;
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
      case 'e':
                inst->exec = true;
                if (strcmp(optarg,"cat") == 0)
                {
                  if (inst->name == true)
                  {
                    inst->cat = true;
                  }
                  else
                  {
                    printf("name argument required for cat \n");
                  }
                }
                else if (strcmp(optarg,"rm") == 0)
                {
                  inst->rm = true;
                }
                else if (strcmp(optarg,"mv") == 0)
                {
                  inst->mv = true;
                  printf("optind = %d, argc = %d \n",optind,argc);
                  if (optind < argc)
                  {
                    inst->destination = argv[optind];
                  }
                  else
                  {
                    printf("No valid destination provided for option -exec mv. \n");
                    inst->mv = false;inst->exec = false;
                  }
                }
                else
                {
                  printf("No valid argument provided for option -exec. \n");
                  inst->exec = false;
                }
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
  rtrn->noArgs = false;
  rtrn->del = false;
  rtrn->exec = false;
  rtrn->cat = false;
  rtrn->rm = false;
  rtrn->mv = false;
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
  char tempSelf[] = ".";
  char tempParent[] = "..";
  char tempSlash[] = "/";

  if (directory != NULL)
  {

  /* Look at each entry in the directory. */
    while((dir_entry = readdir(directory)) != NULL)
    {
      if(strcmp(dir_entry->d_name,tempSelf) != 0 && strcmp(dir_entry->d_name,tempParent) != 0 && strcmp(dir_entry->d_name,"find") != 0)
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
            execute_instructions(inst, buf, temp);
          }
          /* If the entry is a directory, run test and, if necessary, push it onto the directory stack. */
          else if (S_ISDIR(buf.st_mode))
          {
            if (inst->noArgs == true) { printf("%s\n",temp); }
            scan_directory(inst,&temp[2]);
          }
          else
          {
            printf("%s is not a file or a directory \n",temp);
          }
        }
      }
      else if (inst->noArgs == true && strcmp(dir_entry->d_name,tempSelf) == 0 && strlen(current_dir) == 0)
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
  bool isTarget = false;
  char * file_nombre = strrchr(temp,'/');

  if (SHOW_OUTPUT) printf("checking %s \n",file_nombre);
  if (SHOW_INODES) printf("file %s inode = %lu \n",temp,buf.st_ino);

  if (inst->noArgs == true)
  {
    printf("%s \n",temp);
    return;
  }
  if (inst->name == true)
  {
    if ((strcmp(inst->target,&file_nombre[1]) == 0)) { isTarget = true; } else { return; }
  }
  if (inst->mmin == true) { isTarget = minutes_check(inst, buf.st_mtime); if(!isTarget) return; }
  if (inst->inum == true)
  {
    if (SHOW_INODES2) printf("ino = %lu \n",buf.st_ino);
    if(inst->inode == buf.st_ino) { isTarget = true; } else { return; }
  }
  if (isTarget)
  {
    if (inst->exec == true)
    {
      char * arg0 = (char *)malloc(sizeof(char) * 128);
      char **arg1 = (char **)malloc(sizeof(char *) * 32);
      if(inst->cat == true)
      {
        //printf("cat %s \n",temp);
        arg0 = strcpy(arg0,"/bin/cat");
        arg1[0] = "cat";
        arg1[1] = temp;
        arg1[2] = (char *)0;
      }
      else if(inst->rm == true)
      {
        //printf("rm %s \n",temp);
        inst->del = false;
        arg0 = strcpy(arg0,"/bin/rm");
        arg1[0] = "rm";
        arg1[1] = temp;
        arg1[2] = (char *)0;
      }
      else if(inst->mv == true)
      {
        //printf("mv %s to %s \n",temp,inst->destination);
        arg0 = "/bin/mv";
        arg1[0] = "mv";
        arg1[1] = temp;
        arg1[2] = inst->destination;
        arg1[3] = (char *)0;
      }
      // printf("arg0 = %s \n",arg0);
      // int i = 0;
      // while (arg1[i] != (char *)0) { printf("arg1[%d] = %s \n",i,arg1[i]); i++; }
      if (fork() == 0)
      {
        if(execv(arg0,arg1) == -1)
        {
          printf("%s failed on %s \n",arg0,temp);
          return;
        }
      }
      else
      {
        return;
      }
    }
    else if (inst->del == true)
    {
      if(remove(temp) != 0){ printf("remove failed on %s \n",temp); }
    }
    else
    {
      printf("%s \n",temp);
    }
  }
  return;
}

bool minutes_check(struct instruction_status * inst, time_t mod)
{
  time_t min = (time(NULL) - mod)/60;
  bool rtrn = false;
  if (SHOW_MINS) printf("minutes = %d : mtime = %lu : min = %lu \n",inst->minutes,mod,min);
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

char * com_string(struct instruction_status * inst)
{
  return (char *)0;
}
