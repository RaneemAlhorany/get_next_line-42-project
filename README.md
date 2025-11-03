# get_next_line-42-project

intoduction section:
 notes :
     all  information you need to konw about the method it is bellow in the section Methods  
     the summary of the header is bellow in the header section 

goals :
     teach you an important concept in C programming: static variables
   
 get_next_line
   Purpose:
       This function is used to return one line at a time from a file descriptor
       Each call returns the next line until EOF or an error occurs
    
 Objective of the project:

     Understanding static variables
     Learn how to work with file descriptors
     Memory management (malloc/free)
     Controlling buffer and read system calls

    note 1: 
        allow to use 3 files just : get_next_line.c, get_next_line_utils.c, get_next_line.h
    note 2:
        Your header file get_next_line.h must at least contain the prototype of the get_next_line() function
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
          instead of reading the whole file at once

    Why?
        - Because we don’t know the file size in advance
        - A line may be longer than BUFFER_SIZE
        - So we must read in small chunks and build lines step by step

    Process:
        Group those chunks until they form a complete line (until '\n'),
          then return that line to the user

    Example:
        File content: "Hello World\nHow are you\n"
        BUFFER_SIZE = 5

        1st read → "Hello"
            - No '\n', store it temporarily
        2nd read → " Worl"
            - No '\n', append to previous → "Hello Worl"
        3rd read → "d\nHow"
            - Found '\n' after "d"
            - Extract and return "Hello World\n"
            - Keep leftover "How" for next call

        Next call:
        1st read (continuing) → leftover "How"
        2nd read → " are "
        3rd read → "you\n"
            - Found '\n', return "How are you\n"

    => So:
        - 1st line took 3 reads
        - 2nd line took 2 reads
        
Core Algorithm:

    1. Read a new chunk into the buffer
    2. Append it to previously saved data (leftover)
    3. Search for '\n':
        * If found:
            - Extract line including '\n'
            - Save leftover (anything after '\n') for the next call
            - Return the line
        * If not found:
            - Continue reading and appending until you find '\n' or hit EOF

Implementation Notes:

    Handling read():
        - read() may return:
            # > 0 → actual bytes read
            # = 0 → EOF (end of file)
            # = -1 → error
         You must handle all 3 cases

    Scenarios:
        1) '\n' found:
            - Extract and return line
            - Keep leftover for next call

        2) '\n' not found:
            - Keep reading until '\n' appears or EOF

        3) EOF without '\n':
            - Return whatever is left (last line)
            - Next call should return NULL

        4) Error during read:
            - Free allocated memory
            - Return NULL

    ? Important:
        - Use a static variable to keep leftover data between multiple calls
        - Manage memory carefully (malloc/free)

High-level Steps:

    ~ 1) Create a buffer (size BUFFER_SIZE + 1)
    ~ 2) Use a static variable to store leftover data between calls
    ~ 3) Read chunks using read() and append them to leftover
    ~ 4) Search for '\n' in the combined string
        ? If found → extract line, update leftover, return line
        ? If not found → continue until '\n' or EOF
    ~ 5) Handle EOF → return remaining data (if any)
    ~ 6) Handle errors → free memory, return NULL
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
         Local automatic variable : stack
             It loses its value after the function terminates
         Local static variable : data segment (initialized or BSS if zero)
             It holds the value between function calls
         Global static variable : data segment (internal linkage)     
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
         Stores the program instructions (machine code)
         Usually read-only.so you can’t modify your own code at runtime
          ex:
            int main() {
                return 0;
            }
             The compiled code of "main" is stored in the Text Segment

     2) Data Segment
        Stores global and static variables
        Divided into two parts:
             Initialized Data Segment: 
                    global/static variables with an explicit value (e.g., int x = 5;)
             BSS (Block Started by Symbol) / Uninitialized Data Segment :
                     global/static variables without an explicit value or initialized to zero (e.g., static int y;)
                     Default initialization = 0

     3) Heap
         Stores dynamically allocated memory (malloc, calloc, realloc)
             Grows/shrinks during runtime
            Grows upward at runtime as you request more memory/ You must free the memory manually with free()
            int *ptr = malloc(sizeof(int)); // Allocated on Heap

            
     4) Stack
         Stores local (automatic) variables and function call frames
        Created when a function is called and destroyed when it returns (local variable)
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

                     
The difference between user mode and kernel mode

                User mode:
                   # Where user applications run
                   # Limited access to system resources
                   # Cannot directly access hardware
                   # Can only request services via system calls
                   
                Kernel mode:
                   # Where the operating system runs
                   # Full access to system resources
                   # Can directly access hardware
                   # Controls and manages all resources



The difference between system call and function call

     System call:
        * Interface between user programs and the operating system
        * Requests kernel services like file access, device control, memory allocation
        * Examples: read(), write(), fork(), exec()

     Function call:
        * A simple call to a function within your program or a library
        * Executes tasks only in user mode (no kernel involvement)
        * Examples: strlen(), strcpy(), user-defined functions


Types of system call

    1. Process Control System Calls
        # Manage processes
        # Examples: fork(), exec(), wait(), exit()

    2. File Management System Calls
        # Manage files and directories
        # Examples: open(), read(), write(), close(), create(), unlink()

    3. Device Management System Calls
        # Manage devices
        # Examples: ioctl(), read(), write()

    4. Information Maintenance System Calls
        # Maintain system information
        # Examples: getpid(), alarm(), sleep()

    5. Communication System Calls
        # Inter-process communication
        # Examples: pipe(), shmget(), shmat(), msgget()

    6. Protection System Calls
        # Protect system resources
        # Examples: chmod(), chown(), umask()

    7. Memory Management System Calls
        # Manage memory
        # Examples: brk(), sbrk(), mmap(), munmap()



read() system call

     Information:
          read() is a system call in C used to read raw data (bytes) 
          from a file descriptor (like stdin, file, or socket) into a buffer


     Syntax:
        ssize_t read(int fd, void *buf, size_t count);


     Parameters:
        # fd: File descriptor to read from
        # buf: Pointer to buffer where read data will be stored
        # count: Maximum number of bytes to read


     Return value:
        # On success: number of bytes read (may be less than count)
        # On error: -1, and errno is set
        # On end-of-file (EOF): 0


     Common use cases:
        # Reading from files
        # Reading from standard input (stdin)
        # Reading from network sockets
        # Reading from pipes or shared memory


     Important points:
        # buf must point to a valid memory location
        # Buffer size should not be smaller than count (to avoid overflow)
        # count must be > 0
        # fd must be a valid file descriptor (from open(), etc......)
        # read() is not guaranteed to return all requested bytes in one call
                 It may return fewer bytes depending on availability
        # But it always returns the bytes in correct order from the file


     Benefit of read() inside the get_next_line project:
         read() is the foundation: it fetches chunks of data from the file
         Then, the program concatenates the chunks, searches for '\n'
             and returns complete lines one by one


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
method section 

 methods 

        i have 3  files : main file , helper file (utils) , header file
        
            1- helper functions that used in the get_next_line functions (utils)
                    all the method explain bellow this file  
            2- get_next_line function  (main functions)
                    all the method explain bellow this file  
            3- header file (get_next_line.h) that contain the prototypes of the functions and the libraries


 get_next_line function (main function)  this for  madateroy functions that i will use it in the get_next_line function
 get_next_line : to combine all the  functions and return the line by line


 get_next_line

         information:
              this method is the main function of the "get_next_line" project
              its job is to read a line (including '\n' if present) from a file descriptor (fd)
               each time it’s called, until reaching the end of the file
               it uses helper functions to manage memory and partial reads (static variable "chunk")
               
         prototype of the method:
              char *get_next_line(int fd);

         description:
             ~ this function reads from a file descriptor line by line
             ~ it keeps the unread part of the file stored in a static variable "chunk"
                    so that the next call continues reading from where it stopped
             ~ the function dynamically allocates memory for each line returned
             ~ when there are no more lines to read, it returns NULL

         parameters:
             int fd : file descriptor (the file or input source to read from)
    
         return:
              char * (string): returns one line from the file each time it's called
              returns NULL when there are no more lines or an error occurs

     
         how it works (step-by-step):
                decleration the static variable :
                           static char *chunk;
                       
                 step 1) validate inputs:
                     check if the file descriptor is valid and if BUFFER_SIZE is greater than zero
                           if (fd < 0 || BUFFER_SIZE <= 0)
                                  return (NULL);
                    
                step 2) allocate memory for the buffer:
                    allocate (BUFFER_SIZE + 1) bytes to store temporary data read from the file , +1 for null character
                           char *buffer = malloc(BUFFER_SIZE + 1);
                     if allocation fails, return NULL
                            if (!buffer)
                                  return (NULL);

                step 3) read and store data into "chunk":
                     call "read_and_store(&chunk, fd, buffer)" to read data from the file
                     it appends new data to the existing "chunk"
                            if (read_and_store(&chunk, fd, buffer) == -1)
                                   return (free_chunk(&chunk));
                     if reading fails, free the chunk and return NULL
                     
                step 4) handle empty or invalid chunks:
                     if "chunk" is NULL or empty, free it and return NULL
                            if (!chunk || *chunk == '\0')
                                   return (free_chunk(&chunk));

                step 5) extract the next line:
                     call "extract_line(&chunk)" to get the next line from the chunk
                     if extraction fails, free the chunk and return NULL
                            char *line = extract_line(&chunk);
                            if (!line)
                                   return (free_chunk(&chunk));

                step 6) update the chunk (remove the extracted line):
                     call "update_chunk(&chunk)" to keep only the remaining unread part
                            chunk = update_chunk(&chunk);
                    
                step 7) return the extracted line:
                     return the pointer to the extracted line string
                            return (line);
     
         important notes:
                "chunk" is declared as static, meaning its value persists between function calls
                each call to get_next_line returns the next line in the file until EOF
                the function uses dynamic memory : make sure to free each line after use to avoid leaks
                this design allows multiple calls to continue reading sequentially from the same fd
     
         summary:
              get_next_line acts like a controlled reader:
                 - it reads a file line by line
                 - stores remaining unread data in "chunk"
                 - dynamically manages memory
                 - ensures no data loss between calls
     
         example of usage:
         
             int fd = open("file.txt", O_RDONLY);
             char *line;
             while ((line = get_next_line(fd)) != NULL)
             {
                 printf("%s", line);
                 free(line);
             }
             close(fd);
             

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
get_next_line function (main function)  this for bones  functions that i will use it in the get_next_line function
get_next_line : to combine all the functions and return the line by line

 get_next_line
            
             information:
                      this method is part of the get_next_line project (bonus version)
                      
             prototype of the method:
                      char *get_next_line(int fd);
                      
             description:
                     # this method is used to read from a file descriptor (fd) and return one line at a time
                     # it reads continuously from the file until it finds a newline character ('\n') or reaches EOF
                     # it keeps track of the remaining unread data using a static variable (chunk) so that
                            consecutive calls continue reading from where the previous call stopped
                     # this version supports reading from multiple files simultaneously by using a static array (chunk[1024]),
                       where each file descriptor has its own separate buffer
                       
             parameters:
                     int fd :
                         file descriptor from which data is read
                         
             return:
                      returns a pointer to the next line read from the file descriptor (including '\n' if present)
                      returns NULL if:
                         - there’s an error in reading or memory allocation,
                         - the end of the file is reached (no more data),
                         - or the file descriptor is invalid
                         
             how to implement it:
             
                   decleration the variable need to use
                         char *line;
                              used to return the line that read from the file
                         char *buffer;       
                              used as a temporary place that store the data read from the file 
                         static char *chunk [1024];
                              Each file (fd) has a special section of memory to store what is left over from the previous read
                              Using the [1024] array corresponds to the number of file descriptors that can be opened in the system
                             
                   check the validity of parameters and limits: 
                         if (fd < 0 || fd >= 1024 || BUFFER_SIZE <= 0)
                             return (NULL);
                                 the file descriptor should be valid (bigger than 0)
                                 the file descriptore should not be more than or equal 1024 (execuud the limit) 
                                 
                   allocate memory for the buffer used to read data from the file:
                         buffer = malloc(BUFFER_SIZE + 1);
                         if (!buffer)
                             return (NULL);
                             
                   read from the file and store the content in the corresponding chunk for this fd + handle read errors:
                         if (read_and_store(&chunk[fd], fd, buffer) == -1)
                             return (free_chunk(&chunk[fd]));
                                      $ It reads from the file and stores the content inside the chunk of fd
                                      $ If it finds newline or end of file : it stops
                                      $ Returns -1 in case of error
                                      $ If the read fails (e.g. read() == -1), the memory is cleaned and NULL is returned
                         
                   check if there is no data left in the chunk:
                         if (!chunk[fd] || !*chunk[fd])
                             return (free_chunk(&chunk[fd]));
                                   If the data is empty or completely finished : release the memory and stop execution  

                   extract a single line from the chunk:
                         line = extract_line(&chunk[fd]);
                                It extracts only one line of current data within chunk
                         
                   handle extraction failure:
                         if (!line)
                             return (free_chunk(&chunk[fd]));
                             
                   update the chunk to keep only the remaining data after the extracted line:
                         chunk[fd] = update_chunk(&chunk[fd]);
                         
                   finally, return the extracted line:
                         return (line);
                         
             notes:
                     # this method manages dynamic memory carefully to avoid leaks
                     # it works with multiple file descriptors at the same time by keeping separate static buffers
                     # free_chunk() is used to free memory in all failure cases
                     # extract_line() and update_chunk() are used together to return one line and keep the rest for later
                     # read_and_store() ensures continuous reading until a full line is available or EOF is reached

                      
             the difference between the mandatory and bonus versions:
                     - mandatory: uses a single static variable (char *chunk)
                     - bonus: uses a static array (char *chunk[1024]) to support multiple file descriptors

                    
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//! get_next_line function (main function)  this for bones madateroy functions that i will use it in the get_next_line function
//? sharing method between get_next_line and get_next_line_bonus 

     1 - free_chunks    : to free the memory allocated for the chunks variable    
     2 - extract_line   : to extract the line from the chunks variable until it reach \n or the end of the file 
     3 - update_chunk   : to trim the chunks variable after extracting the line (remove the extracted line from chunks variable)
     4 - read_and_store : to read from the file and store it inside the satatic variable (chunk)



1) free_chunk

       information:
              this method is part of the get_next_line project
   
       prototype of the method:
             char *free_chunk(char **chunk);
   
       description:
             this method is used to free the memory allocated for the chunk variable (static variable in get_next_line function)
             it takes a double pointer to char (char **) as an argument,
                       which allows it to modify the original pointer passed to it
   
       parameters:
              char **chunks : double pointer to the chunks variable that needs to be freed
   
       return: 
             array of char (char *) : returns NULL after freeing the memory
   
       how to implement it:
             check if the chunks pointer is not NULL 
              and 
             if the dereferenced pointer (*chunks) is also not NULL
                         if (chunks && *chunks)
             if both conditions are true, proceed to free the memory    
                     free the memory allocated for the chunks variable using free function
                             free(*chunks);
                     set the dereferenced pointer (*chunks) to NULL to avoid dangling pointers
                             *chunks = NULL;
             after the if condition 
             return NULL to indicate that the memory has been freed and the pointer is no longer valid
                      return (NULL);

       notes:
             make sure that the chunks pointer is valid and not NULL before calling this function
             after calling this function, the original pointer passed to it will be set to NULL,
                     so it should not be used again without reallocation
     
       the diffrent between free and free_chunk :
             free 
                 is a standard library function in C that is used to
                  deallocate memory that was previously allocated using malloc, calloc, or realloc.
             free_chunk 
                 is a custom function that is specifically designed to free a particular variable 
                 (chunk) and set its pointer to NULL to avoid dangling pointers

       the different between wnithout star and  * and ** and when to use it :
                without star
                    # is used to declare a regular variable of a specific data type (e.g., int, char, float)
                    # It holds the actual value of that data type
                * (single asterisk) 
                    # is used to declare a pointer to a single variable or data type
                    # It allows you to store the memory address of that variable
                    # maen : if you have int *ptr; , ptr is a pointer to an integer variable
                ** (double asterisk)
                    # is used to declare a pointer to a pointer
                    # It allows you to store the memory address of another pointer,
                            effectively creating a level of indirection    
                    # maen : if you have int **ptr_to_ptr; , ptr_to_ptr is a pointer to a pointer to an integer variable
   
               when to use it :
                   # use without star when you want to work with the actual value of a variable
                   # use * when you want to work with the address of a single variable (pointer)
                   # use ** when you want to work with the address of a pointer (pointer to pointer)   


2) extract_line
    
       information:
             this method is part of the get_next_line project
   
       prototype of the method:
             char *extract_line(char **chunk);
   
       description:
            # this method is used to extract a single line from the chunk variable
            # it allocates memory for a new string containing all characters from the start of the chunk 
                   up to and including the first newline character ('\n') if it exists
            # it does not modify or free the chunk; it only extracts the line to be returned
   
       parameters:
             char **chunk ;
                double pointer to the chunk variable that stores the accumulated data read from the file descriptor

       return:
            array of char (char *) : 
                returns a newly allocated string containing the extracted line
                +
                returns NULL if memory allocation fails 
                    or if the chunk is empty or invalid
   
       how to implement it:
            check if the chunk is valid (not NULL and not empty)
                if (!chunk || !*chunk || !**chunk)
                    return (NULL);
   
            initialize an index variable to 0
                size_t index = 0;
   
            loop through the chunk until you find a newline ('\n') or reach the end of the string
                while ((*chunk)[index] && (*chunk)[index] != '\n')
                    index++;
   
            if a newline character is found, include it by incrementing the index
                if ((*chunk)[index] == '\n')
                    index++;
   
            allocate memory for the new line (size = index + 1 for the null terminator)
                char *line = malloc(index + 1);
   
            if malloc fails, return NULL
                if (!line)
                    return (NULL);
   
            copy the portion of the chunk that represents the line into the newly allocated string
                ft_memcpy(line, *chunk, index);
   
            add the null terminator at the end of the new string
                line[index] = '\0';
   
            return the extracted line
                return (line);
   
       notes:
            # this method focuses only on line extraction; it does not handle updating or freeing the chunk
            # use ft_memcpy (from libft (UPDATE VERSION)) to safely copy the characters from the chunk to the new string
            # always check for NULL pointers before using the chunk to avoid segmentation faults
   
       the different between wnithout star and  * and ** and when to use it :
   
            without star
                # is used to declare a regular variable of a specific data type (e.g., int, char, float)
                # It holds the actual value of that data type
   
            * (single asterisk) 
                # is used to declare a pointer to a single variable or data type
                # It allows you to store the memory address of that variable
                # mean : if you have int *ptr; , ptr is a pointer to an integer variable
   
            ** (double asterisk)
                # is used to declare a pointer to a pointer
                # It allows you to store the memory address of another pointer,
                     effectively creating a level of indirection    
                # mean : if you have int **ptr_to_ptr; , ptr_to_ptr is a pointer to a pointer to an integer variable
   
            when to use it :
                ~ use without star when you want to work with the actual value of a variable
                ~ use * when you want to work with the address of a single variable (pointer)
                ~ use ** when you want to work with the address of a pointer (pointer to pointer)              
   
3) update_chunk

       information:
            this method is part of the get_next_line project
   
       prototype of the method:
            char *update_chunk(char **chunk);
   
       description:
           # this method is used to update the chunk variable after extracting a line
           # it removes the part of the chunk that was already returned by extract_line,
                   keeping only the remaining unread data after the first newline ('\n')
           # it handles memory management safely by freeing the old chunk 
             and allocating a new one for the remaining data
   
       parameters:
           char **chunk :
                   double pointer to the chunk variable that holds the accumulated data
                   read from the file descriptor
   
       return:
           # array of char (char *) 
               returns a newly allocated string containing the remaining unread data 
               after the extracted line.
           # returns NULL if:
               - there is no more data after the newline,
               - memory allocation fails,
               - or the chunk itself is invalid or empty
   
       how to implement it:
   
           check if the chunk is valid (not NULL and not empty)
                 if (!chunk || !*chunk || !**chunk)
                     return (NULL);
   
           find the position of the first newline character ('\n') in the chunk
               char *data_after_nl = ft_strchr(*chunk, '\n');
   
           if there is a newline, move the pointer one step forward to start after it
               if (data_after_nl)
                   data_after_nl++; 
               else
                   return (free_chunk(chunk)); // no newline found → free and return NULL
   
           if there’s no remaining data after the newline, free the chunk and return NULL
               if (!*data_after_nl)
                   return (free_chunk(chunk));
   
           calculate the length of the remaining data after the newline
               size_t len = ft_strlen(data_after_nl);
   
           allocate memory for the new chunk that will store the remaining data
               char *new_chunk = malloc(len + 1);
   
           if malloc fails, free the old chunk and return NULL
               if (!new_chunk)
                   return (free_chunk(chunk));
   
           copy the remaining data (after the newline) into the new chunk
               ft_memcpy(new_chunk, data_after_nl, len);
   
           add the null terminator at the end of the new chunk
               new_chunk[len] = '\0';
   
           free the old chunk and reset its pointer to NULL to avoid memory leaks
               free(*chunk);
               *chunk = NULL;
   
           return the newly allocated chunk that contains only the remaining data
               return (new_chunk);
   
       notes:
           # this method is responsible for memory management between lines
           # it ensures that no memory leaks occur by freeing the old chunk every time
           # it must be called after extract_line to prepare the chunk for the next read
           # always check for NULL return before using the updated chunk to avoid crashes
           # ft_memcpy (from libft (update version)) is used for safe byte-level copying
   
       the difference between without star, * and ** and when to use it :
   
           without star
               # is a normal variable that holds a value directly
               # example: char c = 'A'; — c holds the value itself
   
           * (single asterisk)
               # is used to declare a pointer that stores the memory address of a variable.
               # example: char *ptr = &c; — ptr holds the address of c
   
           ** (double asterisk)
               # is used to declare a pointer to another pointer.
               # example: char **ptr_to_ptr = &ptr; — ptr_to_ptr holds the address of ptr
   
           when to use it:
               ~ use no star for direct value access.
               ~ use * when you need to access or modify data indirectly via memory address.
               ~ use ** when you need to modify the pointer itself inside a function.

   
5) read_and_store
   
       information:
           this method is part of the get_next_line project
   
       prototype of the method:
           int read_and_store(char **chunk, int fd, char *buffer);
   
       description:
           # this method reads data from the file descriptor (fd) into the buffer,
                   and appends the read content to the existing chunk until a newline ('\n')
                   is found or the end of the file is reached
           # it dynamically expands the chunk to accumulate all data read so far
                   while ensuring no memory leaks occur
           # it stops reading as soon as a newline is detected inside the chunk
   
       parameters:
           1) char **chunk :
               double pointer to the variable that stores all accumulated data read from the file descriptor
           2) int fd :
               the file descriptor from which the data is being read
           3) char *buffer :
               temporary buffer used for each read operation (with size BUFFER_SIZE + 1). +1 for null character
   
       return:
           # returns a positive value (true) if data has been successfully read and stored.
           # returns 0 if the end of the file is reached without new data
           # returns -1 in case of an error (allocation failure or read error)
   
       how to implement it:
   
           # decleration the following cariables:
               char *temp;
   
           # read data from the file descriptor into the buffer for first time 
               ssize_t bytes = read(fd, buffer, BUFFER_SIZE);
   
                   ~ fd          : file descriptor
                   ~ buffer      : temporary buffer used for each read operatio
                   ~ BUFFER_SIZE : specifies the maximum number of bytes to read at once
               The result of read(fd, buffer, BUFFER_SIZE) is stored in 'bytes', 
                   which indicates how many bytes were actually read.
   
           # loop while bytes read are greater than 0 (mean there is data read from the file)
               while (bytes > 0)
               {
   
                   # terminate the buffer with '\0' to treat it as a string
                       buffer[bytes] = '\0';
                   # end and close the string after read from the file
   
                   # concatenate the buffer content to the existing chunk
                       temp = ft_strjoin(*chunk, buffer);
                   # used to save the data that is inside the chunk then add new value from the buffer to it 
                   # (take new copy form old and new and combine it)
   
                   # if ft_strjoin fails (malloc error), free buffer and return -1
                       if (!temp)
                       {
                           free(buffer);
                           return (-1);
                       }
                  # thats mean there are something wrong happened
   
                   # free the old chunk and replace it with the new concatenated one
                       free(*chunk);
                       *chunk = temp;
                  # when the error dosent oceread, it will follow to implemet the next instructions
                  #  so we need to free the previous value then store new value inside the chunk
   
                  # if a newline exists inside the chunk, stop reading and go out from the loop
                       if (ft_strchr(*chunk, '\n'))
                           break;
   
                  # continue reading next chunk of data then check in the loop there is data or not
                       bytes = read(fd, buffer, BUFFER_SIZE);
               }
   
           # after reading, free the buffer
               free(buffer);
   
           # if a read error occurred (bytes < 0), return -1
               if (bytes < 0)
                   return (-1);
   
           # otherwise, return whether we have data (1) or not (0)
               return (bytes > 0 && (*chunk && **chunk));
          # note : 
               bytes > 0: Ensures that the number of bytes is greater than zero
                   ~ *chunk: Checks that the pointer chunk is not NULL
                   ~ **chunk: Checks that the data pointed to by chunk is not NULL or zero
                   # All three conditions must be true for the function to return 1, 
                   #  indicating that valid data exists. If any condition is false, it returns 
       notes:
           # this method ensures memory safety by freeing buffer after usage
           # it depends on ft_strjoin and ft_strchr from libft (new vserion)
           # it updates the chunk in place by allocating a new combined string and freeing the old one
           # reading stops as soon as a newline character is found, optimizing performance


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

helper functions (utils)  this for bones + madateroy functions that i will use it in the get_next_line function

     1- ft_strlen : to get the length of the string 
     2- ft_strchr : to check if the string contain a specific character 
     3- ft_memcpy : to copy from one string to another
     4- ft_strjoin : to concatenate two strings 



 1) strlen

        information:
            this method is part of string.h library (its new of the libft version that need to use it )
    
        prototype of the method:
            size_t strlen (const char * s);
       
        description:
            # this method is used to return the length of the string
            # it counts the number of characters in a string until it reaches the null terminator '\0'
            # if the string is empty it will return 0
    
        return: 
            return the number of the lenght (the type of the return value is size_t (long unsigned int))
    
        the new version add the check null in the first line :
            if (!s)
                return (0);
    
        how to work :
    
            # check if the string is null or not , if it null return 0 
                if (!s)
                    return (0);
         
            # intiilize counter that use to count the length of the string (the type of it : size_t)
                size_t  len = 0;
    
            # then use the loop to move over each char and calculate the number of charcter inside string    
                while (s[len])
                    len++;
    
            # return the counter (that represent the number of the string)
                return (len);


    ///////////////////////// complete bellow  (04 +  02 header + 05 + 06)//////////////////////////////
  
 //! 2) strchr
/*
    ^ information:
        % this method is part of string.h library  (its new of the libft version that need to use it )
    ^prototype of the method:
        $ char *strchr (const char *s , int c);
    ^ description:
        ~ returns a pointer to the first occurrence of the character c int in the string s
        ~ if the character is not found it return null
    ^ return: 
        * return a pointer to matched character (from the character to the end of characters) or null if the character is not found
    ^ notes and how to solve it : 
        % check if the string is null or not , if it return (NULL)
            if (!s) 
                return (NULL);
        % need counter with data type ==> size_t
              $ size_t	index = 0;
        % loop to check if there is a value inside the string or not 
              $ while (s[index])
                # inside the loop check which characters inside string is equaly to c (you should use cast to convert it to char)
                    ~ if (s[index] == (char) c)
                # if any character inside the string match the value of the specific character then you should return 
                #    the pointer of the specific charater in the array and use & to specify the location of the character
                    ~ return ((char *) &s[index]) or return ((char *) (s + index));
                # the end of the loop you should increment the counter to check the next character if match or not because the previous one not match
                    ~ index++;    
        % after loop add check for null character , because the loop stop when it reach the null character
              $ if (c == '\0') or  if (s[index] == (char) c)
                $ return ((char *) &s[index]); or return ((char *) (s + index));
        % if there is no match you should return null
             $ return (NULL);
*/
   
//! 3) memcpy
/*
    ^ information:
        % this method is part of the <string.h> library in C (originally called memcpy). (its new of the libft version that need to use it )
    ^ prototype of the method:
        $ void *memcpy(void *dest, const void *src, size_t n);
    ^ description:
        ~ this function copies n bytes from memory area 'src' to memory area 'dest'.
        ~ It performs a raw memory copy, byte by byte.
        ~ Important: memory areas must NOT overlap (use ft_memmove instead if overlapping is possible).
    ^ parameters:
        1) void *dest : pointer to the destination memory area where the data will be copied.
        2) const void *src : pointer to the source memory area from which the data will be copied.
        3) size_t n : number of bytes to copy.
    ^ return:
        ? returns a pointer to the destination memory area (dest).
    ^ how to implement it:
        #  check if  one of both 'dest' and 'src' are NULL; if so, return NULL.
            if (!dest || !src)
                return (NULL);
        #  cast pointers to (unsigned char *) to allow byte-by-byte copying:
              unsigned char *d = dest;
              const unsigned char *s = src;
        #  prepare a counter of type size_t (to match the type of n).
                size_t index = -1;
        #  use a loop to copy each byte from s to d:
              while (++index < n)
                  d[index] = s[index];
        #  return the pointer to the destination memory area.
                return dest;
    ^ notes:
        ! use ft_memmove if there's a chance that 'src' and 'dest' overlap.
        ! make sure that both pointers are valid and not NULL before using this function.
        ! ft_memcpy is faster than ft_memmove in non-overlapping cases.   
*/

//! 4) ft_strjoin
/*
    ^ information:
        % this method is part of string.h library 
    ^prototype of the method:
        $ char *ft_strjoin(char const *s1, char const *s2);    
    ^ description:
        ~ this method is used to concatenate two strings into a new string
        ~ it allocates memory for the new string, which is the combined length of both input strings plus one for the null terminator
    ^ parameters:
        1) char const *s1 : pointer to the first null-terminated string
        2) char const *s2 : pointer to the second null-terminated string
    ^ return: 
        ? returns a pointer to the newly allocated string containing the concatenated result of s1 and s2
        ? returns NULL if memory allocation fails
    ^ how to solve it :
        ! you must to check if the input strings are not NULL before proceeding with concatenation
        ! if it is not NULL you can proceed with the concatenation
        ~ to concatenate the two strings you can follow these steps:
            # check if the both input string is NULL or not; if so, return NULL
                 if (!s1 && !s2)
                        return (NULL);
            #  calculate the lengths of both input strings using strlen method
                 size_t len_s1 = strlen(s1);
                 size_t len_s2 = strlen(s2);
            #  allocate memory for the new string using malloc
                ? char *new_str = malloc(len of the first string + len of the second string + 1 (this for \0));
                % We add +1 to allocate space for the null terminator \0
                char *strjoin = malloc(len_s1 + len_s2 + 1);
            #  check if memory allocation was successful
                 if (!strjoin)     
                        return NULL;
                % If malloc fails, it returns NULL, indicating that memory allocation was unsuccessful.
            !  copy the contents of both input strings into the newly allocated memory
            #  first copy the contents of s1 into strjoin using ft_memcpy method
                 ft_memcpy(strjoin, s1, len_s1);
                % We use memcpy to copy the content from the original string s1 to the newly allocated memory strjoin.  
            #  second copy the contents of s2 into strjoin using ft_memcpy method
                 ft_memcpy(strjoin + len_s1, s2, len_s2);
                % We use memcpy to copy the content from the original string s2 to the newly allocated memory strjoin.
            #  add the null terminator at the end of new_str
                ? strjoin[len_s1 + len_s2] = '\0';        
            #  return the pointer to the newly allocated concatenated string
                ? return strjoin;
    ^ notes:
        ! make sure to include the  "libft.h" library to use strlen function
        ! make sure to include stdlib.h library to use malloc function
*/

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//^ relation diagram (how the methods work together):
/*
        ┌───────────────────────────────┐
        │         get_next_line         │
        └──────────────┬────────────────┘
                       │
                       ▼
             ┌───────────────────┐
             │   read_and_store  │
             └───────────────────┘
                  ^ description:
                      ~ reads data from the file descriptor (fd)
                      ~ stores it in the static variable (chunk)
                      ~ stops when a newline '\n' is found or EOF reached
                  ^ output:
                      updated chunk containing all accumulated data

                       │
                       ▼
             ┌───────────────────┐
             │   extract_line    │
             └───────────────────┘
                  ^ description:
                      ~ extracts one complete line from chunk
                      ~ allocates memory for the line and returns it
                      ~ does not modify or free the original chunk
                  ^ output:
                      line (char *) containing characters up to and including '\n'

                       │
                       ▼
             ┌───────────────────┐
             │   update_chunk    │
             └───────────────────┘
                  ^ description:
                      ~ updates chunk after one line is extracted
                      ~ removes the extracted part and keeps the remaining data
                      ~ frees the old chunk and replaces it with a new one
                  ^ output:
                      new_chunk (remaining data after '\n')

                       │
                       ▼
             ┌───────────────────┐
             │    free_chunk     │
             └───────────────────┘
                  ^ description:
                      ~ frees memory of the chunk and sets it to NULL
                      ~ used when there’s no more data or in case of error
                  ^ output:
                      returns NULL

    ^ simplified logic flow (step-by-step overview):

        1️⃣ read_and_store()
            ⤷ reads and accumulates file data into chunk until '\n' or EOF.
        2️⃣ extract_line()
            ⤷ extracts one line from chunk without altering the rest.
        3️⃣ update_chunk()
            ⤷ updates chunk to hold only remaining data (after the line).
        4️⃣ free_chunk()
            ⤷ frees chunk when finished or error occurs.

    ^ memory management note:
        ! each step ensures there are no memory leaks:
            - buffer freed after reading
            - old chunk freed after join/update
            - chunk always set to NULL after freeing

*/


   











