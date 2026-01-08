
#! how to run bash 
# chmod +x run_gnl_tests.sh
# ./run_gnl_tests.sh

#####################################################################################
#!/bin/bash

FILES="main.c get_next_line.c get_next_line_utils.c"
TESTER="gnl_tester.c"
SIZES=(1 2 5 10 42 128 1024)

echo "===== GNL AUTO TEST ====="

for BS in "${SIZES[@]}"
do
    echo
    echo ">>> BUFFER_SIZE = $BS"
    cc -Wall -Wextra -Werror -g \
        $TESTER $FILES \
        -D BUFFER_SIZE=$BS -o gnl_test

    if [ $? -ne 0 ]; then
        echo "❌ Compilation failed"
        exit 1
    fi

    valgrind --leak-check=full \
             --show-leak-kinds=all \
             --track-fds=yes \
             ./gnl_test

    echo "-----------------------------"
done

echo "===== END ====="

