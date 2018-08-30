#pragma once

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/wait.h>
#include<sys/types.h>
#include<limits.h>
#include<fcntl.h>
#include<errno.h>
#include<signal.h>

#define buffer 1024
#define command_length 100
#define arg_num 10




void my_sig_handler(int);
int parseInp(char *);
int execInp(char** );
int execInpNormal(char**);
void remoLa();
void readLa();
