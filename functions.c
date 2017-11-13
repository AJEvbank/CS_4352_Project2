#include "main_header.h"

void getCommandArgs(int argc, char ** argv, struct instruction_status * inst)
{
  int ch,opt_index,mins;

  if (argc >= 2)
  {
    if (argv[1][0] != '-')
    {
      inst->location = argv[1];
      inst->where = true;
    }
    else
    {
      char * buffer = NULL;
      buffer = getcwd(buffer,sizeof(char) * 1024);
      inst->location = buffer;
    }
  }
  else
  {
    inst->help = true;
    char * buffer = NULL;
    buffer = getcwd(buffer,sizeof(char) * 1024);
    inst->location = buffer;
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
                inst->inode = atoi(optarg);
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
  rtrn->where = false;
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
  if (inst->where == false) free(inst->location);
  free(inst);
  return;
}

void scan_directory(struct instruction_status * inst, char * current_dir)
{

  /* Open the directory */
  DIR * directory = opendir(current_dir);
  struct dirent * dir_entry;
  struct stat buf;
  char * temp = (char *)malloc(sizeof(char) * 256);
  // char * pass = NULL;
  char tempSelf[] = ".";
  char tempParent[] = "..";
  char tempSlash[] = "/";
  struct stackNode * dirStack = NULL;

  if (directory != NULL)
  {

  /* Look at each entry in the directory. */
    while((dir_entry = readdir(directory)) != NULL)
    {
      if(strcmp(dir_entry->d_name,tempSelf) != 0 && strcmp(dir_entry->d_name,tempParent) != 0 && (dir_entry->d_name)[0] != '.')
      {
        if (!inst->help)
        {
          temp[0] = '\0';
          temp = strcat(temp,tempSelf);
          temp = strcat(temp,tempSlash);
          temp = strcat(temp,current_dir);
          temp = strcat(temp,tempSlash);
          temp = strcat(temp,dir_entry->d_name);
        }
        else
        {
          temp = dir_entry->d_name;
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
            printf("%s is a file! \n",temp);
          }
          /* If the entry is a directory, run test and, if necessary, push it onto the directory stack. */
          else if (S_ISDIR(buf.st_mode))
          {
            printf("%s is a directory! \n",temp);
            // if (!inst->help) { pass = &temp[2]; } else { pass = temp; }
            dirStack = push(&temp[2],dirStack);
          }
          else
          {
            printf("%s is not a file or a directory \n",temp);
          }
        }
      }
    }
  /* Run the recursive call on each entry in the directory stack. */
    while (dirStack != NULL)
    {
      printf("%s \n",dirStack->dir_name);
      scan_directory(inst,dirStack->dir_name);
      dirStack = pop(dirStack);
    }
    closedir(directory);
  }
  else
  {
    printf("Cannot open directory: %s \n",current_dir);
    free(temp);
    return;
    //exit(13);
  }
  free(temp);
  return;
}

struct stackNode * push(char * dir_name, struct stackNode * base)
{
  struct stackNode * temp = (struct stackNode *)malloc(sizeof(struct stackNode));
  temp->dir_name = (char *)malloc(sizeof(char) * 256);
  strcpy(temp->dir_name,dir_name);
  temp->next = base;
  return temp;
}

struct stackNode * pop(struct stackNode * base)
{
  if (base == NULL)
  {
    return NULL;
  }
  else
  {
    struct stackNode * temp = base;
    base = base->next;
    free(temp->dir_name);
    free(temp);
    return base;
  }
}
