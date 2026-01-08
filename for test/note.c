//! how to test 
/*
    ? First step: check for norm
        - Run norminette on all files
        - No norm errors, warnings, or forbidden functions

    ? Check for hidden / forbidden files
        - No .DS_Store, .vscode, .idea
        - No test files submitted
        - No object files (*.o)
        - No binaries
        - No forbidden headers or libraries

    ? Compile the project
        - Must compile with:
            cc -Wall -Wextra -Werror
        - No warnings allowed
        - BUFFER_SIZE must be changeable at compile time

    ? Write your own main.c tester
        - Test different files and scenarios
        - Do NOT modify get_next_line.c or logic
        - Only change main.c for testing

    ? Use valgrind to test memory
        # valgrind --leak-check=full ./a.out
        # valgrind --leak-check=full --show-leak-kinds=all ./a.out
        - Used to return all memory leaks and a full summary

    ? Change BUFFER_SIZE value
        # cc -Wall -Wextra -Werror -g \
          main.c get_next_line.c get_next_line_utils.c \
          -D BUFFER_SIZE=1

        # cc -Wall -Wextra -Werror -g \
          main.c get_next_line.c get_next_line_utils.c \
          -D BUFFER_SIZE=42

        # cc -Wall -Wextra -Wextra -Werror -g \
          main.c get_next_line.c get_next_line_utils.c \
          -D BUFFER_SIZE=1024

        ! -g : generate debugging information
        ! -D : define a macro at compile time

    ? BUFFER_SIZE edge cases
        - BUFFER_SIZE = 1
        - BUFFER_SIZE = 0  -> must return NULL
        - BUFFER_SIZE < 0  -> must return NULL
        - BUFFER_SIZE > file size

    ? Test different file types
        - Empty file
        - File with only one line
        - File with multiple lines
        - Last line without '\\n'
        - File with only '\\n'
        - File with multiple '\\n\\n\\n'
        - Very long line (longer than BUFFER_SIZE)
        - Very long file

    ? File descriptor tests
        - Invalid fd (-1)
        - Closed fd
        - Very large fd value
        - Ensure no segfault happens

    ? Multiple file descriptors test
        - Open two or more files
        - Alternate reading lines between them
        - Ensure each fd keeps its own buffer (static per fd)

    ? read() error handling
        - Simulate read() returning -1
        - Function must return NULL
        - No memory leaks allowed

    ? Memory management checks
        - No memory leaks
        - No double free
        - No invalid free
        - No use-after-free

    ? File descriptor leak check
        # valgrind --track-fds=yes \
          --leak-check=full \
          --trace-children=yes \
          --show-leak-kinds=all ./a.out

        - Ensure all file descriptors are properly closed

    ? Expected behavior
        - Return line including '\\n' if present
        - Return last line without '\\n' if EOF reached
        - Return NULL when no more lines
        - Caller is responsible for freeing the returned line

    ? Red flags (automatic fail)
        - Using lseek
        - Reading the whole file at once
        - One static variable for all FDs
        - Segmentation fault
        - Memory leak
        - BUFFER_SIZE hardcoded

    ? Final evaluation questions
        - Why did you use static?
        - How do you handle EOF?
        - What happens if read() fails?
        - Who frees the returned line?
        - How do you support multiple file descriptors?
*/
//!

//! how to test 
/*
    ? first step check for norm
    ? the check for hidden file
    ? write the main and use the following line for test
        # valgrind --leak-check=full ./a.out
        # valgrind --leak-check=full -show-leak-kinds=all ./a.out
        this used to  return all the value and summary for every thing
        ~ tochange the value of buffer 
            # cc -Wall -Wextra -Werror -g nameFile.c nameFile2.c.... -D BUFFER_SIZE = #
            !-g :mean generate debugging 
            ! -D : mean define a macro

            valgrind --track-fds=yes --leak-check=full --trace-children=yes --show-leak-kinds=all ./a.out  ls cat file3

*/


