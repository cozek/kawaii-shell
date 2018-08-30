//Submission for IIITG MTech, Computer Systems Lab Assignment

#include "shell.h"


/*-------------------------------+
 Changes the behaviour of the Ctrl C signal to just print a string instead of
 actually invoking the SIGINT for process interrupt.

--------------------------------*/
void my_sig_handler(int sig)
{
  switch (sig) {
    case SIGINT:
        printf("\nYou just killed a child, you monster!\n");
        break;
  }
}

/*-------------------------------+
Displays the final output from the STDOUT buffer, i.e, the output.txt file on disk
inside the shell
--------------------------------*/
void readLa()
{
  int child = fork();
  if(child == 0)
  {
      if(execlp("cat","cat","output.txt",NULL) == -1)
      {
        perror("Error");
      }
      exit(0);
  }
  else if(child >0)
  {
    wait(NULL);
  }
}


/*-------------------------------+
 removes the file buffer and restores signals to default behaviour
--------------------------------*/
void remoLa()
{
  signal(SIGINT, SIG_DFL);
  signal(SIGTSTP, SIG_DFL);
  remove("output.txt");
}


/*-------------------------------+
Parses the input given by the user.
First checks if the input contains the
pipe '|' character.
            If it does contain the pipe, the the string is seperated at the
            pipe '|' character, and the output is then parsed again and
            seperted at every space and put into a char *array which is passed to
            the execution function execInp() .And this process is repeated till the last
            pipe is encountered.
            man strsep for nature of the function
If it contains no pipe,
            Strsep is used to tokenize the usr_input and store every token in a
            char * array and pass the array to the execution function execInpNormal().

Sidejob of this function is the replace the old buffer
with the new buffer in case pipe needs to be parsed. And the end
to display the output via the readLa().
--------------------------------*/
int parseInp(char *c)
{


  char *pPosition = strchr(c, '|');
  char *params[arg_num];

  if  (pPosition != 0)
  {

    for (int i = 0; i < arg_num;i++)
    {
      params[i] = strsep(&c, "|");
      if(params[i]==NULL) break;
    }

    for (int j = 0; j<arg_num ; j++)
    {

      if (params[j]==NULL) break;

      char *rams[arg_num];
      for (int i = 0; i < arg_num;i++)
      {
        rams[i] = strsep(&params[j], " ");
        if(rams[i]==NULL) break;
      }


      execInp(rams);
      if (j>0)
      {
        remove("output.txt");
        rename("outputty.txt","output.txt");
      }


    }
    readLa();

  }
  else{

    char *rams[arg_num];
    for (int i = 0; i < arg_num;i++)
    {
      rams[i] = strsep(&c, " ");
      if(rams[i]==NULL) break;
    }


    execInpNormal(rams);


  }
  return 0;

}


/*-------------------------------+
Function for execution of command that do not have the pipe behaviour.
It also, checks for shell commands such as cd, exit, quit.
The function forks a child and invokes execvp() on the command inside
the child and waits for it to finish execution.

Currently cd cannot change to directories with space in their names.
This is due to the nature of the parser tokenizing the user input at
every space.

Also modifies the behaviour of SIGINT, so that when this signal arrives,
the child receives the signal, but the parent handles.
Even though the child inherits the handling behaviour of the parent, but the
behaviour gets overwritten by the execvp().
--------------------------------*/
int execInpNormal(char** params)
{

  if ((strcmp(params[0], "cd") == 0))
  {


    if(chdir(params[1]) ==-1)
    {
      perror("Chdir Error:");
    }
    return 0;
  }

  if ((strcmp(params[0],"exit")==0) || (strcmp(params[0],"quit")==0))
  {
    printf("(▰˘◡˘▰) See you later Senpai (▰˘◡˘▰)\n");
    exit(0);
  }


  signal(SIGINT, my_sig_handler);

  pid_t child = fork();

  if (child == 0)
  {


    if (execvp(params[0],params) == -1)
    {
      perror("Error");
    }
    exit(0); //ctrl quick exit now
  }
  else if (child > 0)
  {

    wait(NULL);


  }
  return 0;
}


/*-------------------------------+
The behaviour of this function is the same as the one above. But it is
modified in the sense that it handles pipes. This handling is done via a filebuffer
instead of the more conventional pipe().

The child checks for the existance of the buffer , here called the
"output.txt", if the buffer is not present, (obviously the buffer does not needs
to exist if piping is not required.) If the buffer does not exist, open the buffer
with freopen() in w+ mode and stream STDOUT to it. And then executes the first pipe
command.
When the command post the pipe comes, open the buffer and dup it to STDIN,
and create another buffer and dup it to STDOUT. Close the leaking file descriptors.
Then execute the second part of the pipe. This command gets the STDIN from the output
of the previous command.
The parser replaces the new buffer with the old buffer before executing the next pipe
command. And prints the final output into the console.
The command goes back to main, who removes the buffer from the disk.

--------------------------------*/
int execInp(char** params)
{
  signal(SIGINT, my_sig_handler);

  int child = fork();
  if(child == 0) //child
  {

    int out;
    out = open("output.txt",O_RDONLY);

    if (out < 0){
      // printf("here we create");
      FILE *fp;
      fp = freopen("output.txt", "w+",stdout);
      if(execvp(params[0],params) == -1)
      {
        perror("error");
      }
      exit(0);
    }
    else{

      // printf("Here i am\n");
      int in,out;
      in = open("output.txt",O_RDONLY);
      out = open("outputty.txt", O_WRONLY | O_TRUNC | O_CREAT, S_IRUSR | S_IRGRP | S_IWGRP | S_IWUSR);

      dup2(in,0);
      dup2(out,1);
      close(in);close(out);
      if(execvp(params[0],params)==-1)
      {
        perror("Error");
        exit(0);
      }


    }

  }
  else if(child >0) //parent
  {

    wait(NULL);
  }



  return 0;
}
