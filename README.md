# get_next_line-42-project

intoduction section:
 notes :
     all  information you need to konw about the method it is bellow in the section Methods  
     the summary of the header is bellow in the header section 

goals :
     each you an important concept in C programming: static variables
   
 get_next_line
   Purpose:
     This function is used to return one line at a time from a file descriptor.
     Each call returns the next line until EOF or an error occurs.
    
 Objective of the project:

     Understanding static variables.
     Learn how to work with file descriptors.
     Memory management (malloc/free).
     Controlling buffer and read system calls

    note 1: 
        allow to use 3 files just : get_next_line.c, get_next_line_utils.c, get_next_line.h
    note 2:
        Your header file get_next_line.h must at least contain the prototype of the get_next_line() function.
    note 3:    
        Add all the helper functions you need in the get_next_line_utils.c file
    note 4:
        Please note that the returned line should include the terminating \n character,
        except when the end of the file is reached and the file does not end with a \n
        character  
    note 5:
        Make sure that your function works as expected both when reading a file and when
        reading from the standard input 
    note 6:
        Your function should return the line that was read
        If there is nothing left to read or if an error occurs, it should return NULL    
    
First Idea:

    Objective:
        - Read data from a file descriptor (or stdin) in small parts using BUFFER_SIZE,
          instead of reading the whole file at once.

    Why?
        - Because we don’t know the file size in advance.
        - A line may be longer than BUFFER_SIZE.
        - So we must read in small chunks and build lines step by step.

    Process:
        Group those chunks until they form a complete line (until '\n'),
          then return that line to the user.

    Example:
        File content: "Hello World\nHow are you\n"
        BUFFER_SIZE = 5

        1st read → "Hello"
            - No '\n', store it temporarily.
        2nd read → " Worl"
            - No '\n', append to previous → "Hello Worl".
        3rd read → "d\nHow"
            - Found '\n' after "d".
            - Extract and return "Hello World\n".
            - Keep leftover "How" for next call.

        Next call:
        1st read (continuing) → leftover "How"
        2nd read → " are "
        3rd read → "you\n"
            - Found '\n', return "How are you\n".

    => So:
        - 1st line took 3 reads
        - 2nd line took 2 reads
        
Core Algorithm:

    1. Read a new chunk into the buffer.
    2. Append it to previously saved data (leftover).
    3. Search for '\n':
        * If found:
            - Extract line including '\n'.
            - Save leftover (anything after '\n') for the next call.
            - Return the line.
        * If not found:
            - Continue reading and appending until you find '\n' or hit EOF.

Implementation Notes:

    Handling read():
        - read() may return:
            # > 0 → actual bytes read
            # = 0 → EOF (end of file)
            # = -1 → error
         You must handle all 3 cases.

    Scenarios:
        1) '\n' found:
            - Extract and return line.
            - Keep leftover for next call.

        2) '\n' not found:
            - Keep reading until '\n' appears or EOF.

        3) EOF without '\n':
            - Return whatever is left (last line).
            - Next call should return NULL.

        4) Error during read:
            - Free allocated memory.
            - Return NULL.

    ? Important:
        - Use a static variable to keep leftover data between multiple calls.
        - Manage memory carefully (malloc/free).

High-level Steps:

    ~ 1) Create a buffer (size BUFFER_SIZE + 1).
    ~ 2) Use a static variable to store leftover data between calls.
    ~ 3) Read chunks using read() and append them to leftover.
    ~ 4) Search for '\n' in the combined string.
        ? If found → extract line, update leftover, return line.
        ? If not found → continue until '\n' or EOF.
    ~ 5) Handle EOF → return remaining data (if any).
    ~ 6) Handle errors → free memory, return NULL.
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static variables section :


    general information:
        a possesses distinct characteristics regarding 
        its lifetime, scope, and initialization compared to automatic (regular) variables

        declared using static keyword and have the property of retaining their value between multiple function calls   
        It is initialized only once and is not destroyed when the function returns a value  
        It extends the lifetime of the variable till the end of the program

    syntax: The static variable can be simply declared by prefixing the static keyword before the normal variable declaration
        static dataType identifier  = value;    

    usage :
        1) Return Address of Local Variable from a Function when the function return the pointer--> &namevar    
        2) Control Recursion Depth (A static variable can be used to control the recursion depth in this case and avoid the stack)

    Global Static Variables:
        Static variables are generally declared inside functions, but they can also be declared outside functions (at the global scope)
        have their scope limited to the file they are defined in (internal linkage) 
             means that they cannot be accessed outside the current translation unit 
        Other than that, there is no effect of static on global variables in C    


    notes:
        1) Static variables (like global variables) are initialized as 0 if not initialized explicitly
        2) In C, static variables can only be initialized using constant literals


    static local variables vs local variables 
                  static local variables                  | local variables
              --------------------------------------------|--------------- 
                 Exists throughout the                    |  Exists only during
                  program execution                       |   function execution
              --------------------------------------------|----------------
                 Initialized only once                    |  Reinitialized each time 
                (Cannot be re-initialized                 |     function is called
                during runtime except at declaration.)    |
              --------------------------------------------|----------------------
                 Stored in the data segment               |   Stored in the stack

     additional info :
         Local automatic variable → stack
             It loses its value after the function terminates
         Local static variable → data segment (initialized or BSS if zero)
             It holds the value between function calls
         Global static variable → data segment (internal linkage)     
             Available within the file only (internal linkage)
      
            
     use static variable inside get_next_line project:
         the main idea of using static variable to store remaining buffer between calling functions
   
    7) Why Static in get_next_line?
         We need to store leftover data after '\n'.
         Normal local var: destroyed at function exit → lose leftover.
         Static var: keeps data between function calls.
         Makes "memory box" private to function but persistent.



 example of static variable:
#include <stdio.h>

static int global_counter = 0; // accessible only in this file
void counter() {
    static int count = 0; // initialized only once
    count++;
    printf("Count: %d\n", count);
}

int main() {
    counter(); // Count: 1
    counter(); // Count: 2
    counter(); // Count: 3
    return 0;
}

the process’s memory in RAM 
 


High Address
+------------------------+
|        Stack           |  int local_var = 5;
|   (local variables)    |
+------------------------+
|        Heap            |  int *p = malloc(...);
| (dynamic allocation)   |
+------------------------+
|        BSS             |  static int a; // default = 0
| (uninitialized static) |
+------------------------+
| Initialized Data       |  int global = 10;
| (global/static init)   |  static int b = 20;
+------------------------+
|        Text            |  compiled code (main, functions)
+------------------------+
Low Address


     When a C program runs, the process’s memory in RAM is divided into segments:

     1) Text Segment (Code Segment)
         Stores the program instructions (machine code). 
         Usually read-only.so you can’t modify your own code at runtime.
          ex:
            int main() {
                return 0;
            }
             The compiled code of "main" is stored in the Text Segment.

     2) Data Segment
        Stores global and static variables.
        Divided into two parts:
             Initialized Data Segment: 
                    global/static variables with an explicit value (e.g., int x = 5;).
             BSS (Block Started by Symbol) / Uninitialized Data Segment :
                     global/static variables without an explicit value or initialized to zero (e.g., static int y;).
                     Default initialization = 0.

     3) Heap
         Stores dynamically allocated memory (malloc, calloc, realloc).
             Grows/shrinks during runtime.
            Grows upward at runtime as you request more memory/ You must free the memory manually with free().
            int *ptr = malloc(sizeof(int)); // Allocated on Heap
     4) Stack
         Stores local (automatic) variables and function call frames.
        Created when a function is called and destroyed when it returns. (local variable)
         int local_var = 5; // Stored on Stack

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

 System call section 

 The benefit of system call

     Normally, programs run in user mode
         User mode has limited access to system resources
             Example: user mode cannot access hardware directly
                 Because hardware is managed by the kernel mode
                     So, user mode needs to use system calls to access hardware

        System call = a request to the kernel to perform a task on behalf of user mode
             Example: when we want to read a file, we use the system call 'read()'
               Because user mode cannot access the file system directly
                     The kernel does it for us
///////////////////////// complete bellow//////////////////////////////

$ The difference between user mode and kernel mode
    ? User mode:
        # Where user applications run
        # Limited access to system resources
        # Cannot directly access hardware
        # Can only request services via system calls

    ? Kernel mode:
        # Where the operating system runs
        # Full access to system resources
        # Can directly access hardware
        # Controls and manages all resources

$ The difference between system call and function call
    ? System call:
        * Interface between user programs and the operating system
        * Requests kernel services like file access, device control, memory allocation
        * Examples: read(), write(), fork(), exec()

    ? Function call:
        * A simple call to a function within your program or a library
        * Executes tasks only in user mode (no kernel involvement)
        * Examples: strlen(), strcpy(), user-defined functions
*/


//! Types of system call
/*
    ? 1. Process Control System Calls
        # Manage processes
        # Examples: fork(), exec(), wait(), exit()

    ? 2. File Management System Calls
        # Manage files and directories
        # Examples: open(), read(), write(), close(), create(), unlink()

    ? 3. Device Management System Calls
        # Manage devices
        # Examples: ioctl(), read(), write()

    ? 4. Information Maintenance System Calls
        # Maintain system information
        # Examples: getpid(), alarm(), sleep()

    ? 5. Communication System Calls
        # Inter-process communication
        # Examples: pipe(), shmget(), shmat(), msgget()

    ? 6. Protection System Calls
        # Protect system resources
        # Examples: chmod(), chown(), umask()

    ? 7. Memory Management System Calls
        # Manage memory
        # Examples: brk(), sbrk(), mmap(), munmap()
*/


//! read() system call
/*
    ^ Information:
        - read() is a system call in C used to read raw data (bytes) 
          from a file descriptor (like stdin, file, or socket) into a buffer.

    ^ Syntax:
        ssize_t read(int fd, void *buf, size_t count);

    % Parameters:
        # fd: File descriptor to read from
        # buf: Pointer to buffer where read data will be stored
        # count: Maximum number of bytes to read

    % Return value:
        # On success: number of bytes read (may be less than count)
        # On error: -1, and errno is set
        # On end-of-file (EOF): 0

    % Common use cases:
        # Reading from files
        # Reading from standard input (stdin)
        # Reading from network sockets
        # Reading from pipes or shared memory

    ! Important points:
        # buf must point to a valid memory location
        # Buffer size should not be smaller than count (to avoid overflow)
        # count must be > 0
        # fd must be a valid file descriptor (from open(), etc.)
        # read() is not guaranteed to return all requested bytes in one call
            ~ It may return fewer bytes depending on availability
        # But it always returns the bytes in correct order from the file

    // Benefit of read() inside the get_next_line project:
        ? read() is the foundation: it fetches chunks of data from the file
        ? Then, the program concatenates the chunks, searches for '\n'
          and returns complete lines one by one
*/















