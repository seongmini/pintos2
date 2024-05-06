#include "userprog/syscall.h"
#include <stdio.h>
#include <syscall-nr.h>
#include "threads/interrupt.h"
#include "threads/thread.h"

static void syscall_handler (struct intr_frame *);

void
syscall_init (void) 
{
  intr_register_int (0x30, 3, INTR_ON, syscall_handler, "syscall");
}

static void
syscall_handler (struct intr_frame *f) 
{

  int *p = f->esp;


  printf ("system call!\n");
  //thread_exit ();
}

bool is_valid_ptr (const void *user_ptr)
{

}

int wait (pid_t pid)
{

}

void exit (int status)
{

}

pid_t exec (const char *cmd_line)
{

}

void halt (void)
{
  
}