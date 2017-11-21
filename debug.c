#include "main_header.h"

void display_instruction_status(struct instruction_status * inst)
{
  printf("\n\n");
  if (!inst->noArgs)
  {
    if (inst->given)
    {
      printf("location = %s \n",inst->location);
    }
    else if (inst->cwd)
    {
      printf("cwd = %s, length = %lu \n",inst->location,strlen(inst->location));
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
    if (inst->exec == true)
    {
      if (inst->cat == true)
      {
        printf("cat the target \n");
      }
      else if (inst->rm)
      {
        printf("rm the target \n");
      }
      else if (inst->mv == true)
      {
        printf("mv the target to %s \n",inst->destination);
      }
      else
      {
        printf("exec is true without any instructions \n");
      }
    }
  }
  else
  {
    printf("program in noArgs mode \n");
    printf("location = %s \n",inst->location);
    printf("dot_first_location = %d \n",inst->dot_first_location);
  }
  printf("\n\n");
  return;
}
