#include <stdlib.h>
#include <stdio.h>
#include <string.h>
//  #include <iostream.h>

int crename_(char file_name[120], int * inum)
{
  char com[360];
  char fname[120];
  int ilen, i;

  ilen = strlen(file_name);
  for(i=0;i<ilen;i++)
  {
    if(file_name[i]=='.')
    {
      file_name[i]=0;
      break;
    } 
  }

  strcpy(fname,file_name);
  
  sprintf(com,"mv %s.ntpl %s_%i.ntpl",fname,fname,*inum);
  
  system(com);
 
  return 1;
}
