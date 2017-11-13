#include "main_header.h"

void getCommandArgs(int argc, char ** argv, struct instruction_status * inst)
{
  int ch,opt_index;

  if (argc >= 2)
  {
    if (argv[1][0] != '-')
    {
      inst->location = argv[1];
      inst->where = true;
    }
    else
    {
      char * buffer = (char *)malloc(sizeof(char) * 1024);
      getcwd(buffer,sizeof(char) * 2048);
      inst->location = buffer;
    }
  }
  else { inst->help = true; return; }

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
                inst->minutes = atoi(optarg);
                inst->mmin = true;
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
}\

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
  char * temp;
  char tempSelf[] = ".";
  char tempParent[] = "..";
  char tempSlash[] = "/";

  if (directory != NULL)
  {

  /* Look at each entry in the directory. */
    while((dir_entry = readdir(directory)) != NULL)
    {
      temp = dir_entry->d_name;
      if(strcmp(temp,tempSelf) != 0 && strcmp(temp,tempParent) != 0)
      {
        stat(temp,&buf);
        /* If the entry is a file, run test and execute. */
        if (S_ISREG(buf.st_mode))
        {
          printf("%s is a file! \n",temp);
        }
        /* If the entry is a directory, run test and, if necessary, push it onto the directory stack. */
        else if (S_ISDIR(buf.st_mode))
        {
          printf("%s is a directory! \n",temp);
        }
        else
        {
          printf("%s is not anything important \n",temp);
        }
      }
    }
  /* Run the recursive call on each entry in the directory stack. */
    closedir(directory);
  }
  else
  {
    printf("Cannot open directory: %s \n",current_dir);
    exit(13);
  }
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
