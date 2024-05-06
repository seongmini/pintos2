#ifndef USERPROG_SYSCALL_H
#define USERPROG_SYSCALL_H

void syscall_init (void);

bool is_valid_ptr (const void *user_ptr);
int wait (pid_t pid);
void exit (int status);
pid_t exec (const char *cmd_line);
void halt (void);

#endif /* userprog/syscall.h */

