//Submission for IIITG MTech, Computer Systems Lab Assignment

#include "shell.h"

int main (int argc, char* argv[])
{
  char usr_inp[command_length];


  while(1)
  {

    remoLa();

    char* username = getenv("USER");

    printf(">%s(づ｡◕‿‿◕｡)づ : ",username);
    if(fgets(usr_inp,sizeof(usr_inp),stdin)==NULL) //fgets returns NULL when it encounters ctrl D aka EOF
    {
      exit(0);
    }
    if(usr_inp[0]=='\n')
    {
      printf("No Input, Exiting Shell, Bye Senpai \n(▰˘◡˘▰)\n" );
      exit(0);
    }


    if(usr_inp[strlen(usr_inp)-1] == '\n')
    {
      usr_inp[strlen(usr_inp)-1] = '\0';
    }

    parseInp(usr_inp);


    remoLa();
  }

  return 0;
}
