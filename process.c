/* Starts a new thread running a user program loaded from
   FILENAME.  The new thread may be scheduled (and may even exit)
   before process_execute() returns.  Returns the new process's
   thread id, or TID_ERROR if the thread cannot be created. */
tid_t
process_execute (const char *file_name) 
{
  char *fn_copy;
  tid_t tid;

  /* Make a copy of FILE_NAME.
     Otherwise there's a race between the caller and load(). */
  fn_copy = palloc_get_page (0);
  if (fn_copy == NULL)
    return TID_ERROR;
  strlcpy (fn_copy, file_name, PGSIZE);

  /* Parse file_name for get file_name without arguments */
  char *f_name;
  char *saveptr;

  f_name = strtok_r(file_name, " ", &saveptr);

  /* Create a new thread to execute FILE_NAME. */
  tid = thread_create (f_name, PRI_DEFAULT, start_process, fn_copy);
  if (tid == TID_ERROR)
    palloc_free_page (fn_copy); 
  return tid;
}

/* A thread function that loads a user process and starts it
   running. */
static void
start_process (void *file_name_)
{
  char *file_name = file_name_;
  struct intr_frame if_;
  bool success;
  char* argv[32];
  int argc=0;
  char *token;
  char *saveptr;
  int load_status;

  /* Initialize interrupt frame and load executable. */
  memset (&if_, 0, sizeof if_);
  if_.gs = if_.fs = if_.es = if_.ds = if_.ss = SEL_UDSEG;
  if_.cs = SEL_UCSEG;
  if_.eflags = FLAG_IF | FLAG_MBS;

  token = strtok_r(file_name," ",&saveptr);
  const char *f_name = token;
  while (token != NULL)
  {
    argv[argc] = token;
    token = strtok_r(file_name," ",&saveptr);
    argc++;
  }

  success = load (f_name, &if_.eip, &if_.esp);

  /* If load failed, quit. */
  if (!success) 
  {
    palloc_free_page (file_name);
    thread_exit ();
  }
  else
  {
    argument_stack(argv, argc, &if_.esp);
  }

  /* Start the user process by simulating a return from an
     interrupt, implemented by intr_exit (in
     threads/intr-stubs.S).  Because intr_exit takes all of its
     arguments on the stack in the form of a `struct intr_frame',
     we just point the stack pointer (%esp) to our stack frame
     and jump to it. */
  asm volatile ("movl %0, %%esp; jmp intr_exit" : : "g" (&if_) : "memory");
  NOT_REACHED ();
}

/* added for pintOS2 - 1 argument parsing*/
void argument_stack(const char* argv[], int argc, void **esp)
{
  int esp_val;
  int padding;
  int i = argc - 1;
  char* argv_addr[argc];

  /* Copy the arguments to the stack */
  for (i; i >= 0; i--)
  {
    int arglen = strlen(argv[i]) + 1;
    *esp -= arglen;
    memcpy(*esp, argv[i], arglen);
    argv_addr[i] = (char *)*esp;
  }

  /* Word alignment */
  esp_val = (int)*esp;

  if (esp_val % 4)
  {
    padding = esp_val % 4;
    *esp -= padding;
    memset(*esp, 0, padding);
  }

  /* Add a NULL pointer */
  *esp -= 4;
  memset(*esp, 0, 4);

  /* Set the argument addresses on the stack */
  for (i = argc - 1; i >= 0; i--)
  {
    *esp -= 4;
    memcpy(*esp, argv_addr[i], 4);
  }

  /* Push the address of the argv onto the stack */
  *esp -= 4;
  memset(*esp, argv, 4);
  /* I am not sure if argv here should be argv or *argv */

  /* Push the argc onto the stack */
  *esp -= 4;
  memset(*esp, argc, 4);

  /* Push the return address onto the stack*/
  *esp -= 4;
  memset(*esp, 0, 4);

}
