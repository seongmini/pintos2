struct thread
  {
    /* Owned by thread.c. */
    tid_t tid;                          /* Thread identifier. */
    enum thread_status status;          /* Thread state. */
    char name[16];                      /* Name (for debugging purposes). */
    uint8_t *stack;                     /* Saved stack pointer. */
    int priority;                       /* Priority. */
    struct list_elem allelem;           /* List element for all threads list. */

   /* for alarm*/
   int64_t local_tick;                  /* tick for wake up */

   int init_priority;                   /* for keep original priority */
   struct lock *wait_on_lock;           /* lock that thread is waiting for get */
   struct list donations;               /* list of threads that donate their priority */
   struct list_elem donation_elem;      /* element for manage the list donations */

   int nice;                            /* value for nice */
   int recent_cpu;                      /* value for recent_cpu */

    /* Shared between thread.c and synch.c. */
    struct list_elem elem;              /* List element. */

#ifdef USERPROG
    /* Owned by userprog/process.c. */
    uint32_t *pagedir;                  /* Page directory. */
    /* signal to indicate the child's executable-loading status:
    * - 0: has not been loaded
    * - -1: load failed
    * - 1: load success*/
    int child_load_status;
    /* monitor used to wait the child, owned by wait-syscall and waiting
    for child to load executable */
    struct lock lock_child;
    struct condition cond_child;
    /* list of children, which should be a list of struct child_status */
    struct list children;
    /* file struct represents the execuatable of the current thread */
    struct file *exec_file;
#endif

    /* Owned by thread.c. */
    unsigned magic;                     /* Detects stack overflow. */
  };
