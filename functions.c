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
