#include "main_header.h"

void display_instruction_status(struct instruction_status * inst)
{
  printf("\n\n");
  if (!inst->help)
  {
    if (inst->where)
    {
      printf("location = %s \n",inst->location);
    }
    else
    {
      printf("cwd = %s \n",inst->location);
    }

    if(inst->name)
    {
      printf("target = %s \n",inst->target);
    }
    else
    {
      printf("no filename given \n");
    }

    if (inst->mmin)
    {
      printf("minutes = %d \n",inst->minutes);
      printf("lt = %d, gt = %d, et = %d \n",inst->less_than,inst->greater_than,inst->equal_to);
    }
    else
    {
      printf("no minutes given \n");
    }

    if (inst->inum)
    {
      printf("inode number = %ju \n",(uintmax_t)inst->inode);
    }
    else
    {
      printf("no inode number given \n");
    }

    if (inst->del)
    {
      printf("delete the target \n");
    }
    else
    {
      printf("do not delete the target \n");
    }
  }
  else
  {
    printf("program in help mode \n");
    printf("location = %s \n",inst->location);
  }
  printf("\n\n");
  return;
}
