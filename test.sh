#!/bin/bash


if (($# < 1))
then
    echo "You must specify program name to run and test data directory!" >&2
    exit 1
fi

PROGRAM_PATH=$1
if [[ "$PROGRAM_PATH" != "./"* ]]
then
    PROGRAM_PATH=$"./$PROGRAM_PATH"
fi

TEST_DIR=$2
if [[ "$TEST_DIR" != *"/" && "$TEST_DIR" != "" ]]
then
    TEST_DIR=$"$TEST_DIR/"
fi

for file_name in "$TEST_DIR"*.in
do
    TMPFILE_OUT=$(mktemp)
    TMPFILE_ERR=$(mktemp)

    "$PROGRAM_PATH"<"$file_name" >$TMPFILE_OUT 2>$TMPFILE_ERR

    if (($? == "0"))
    then
        DIFF_OUT=$(diff "$TMPFILE_OUT" "${file_name%in}out")
        DIFF_ERR=$(diff "$TMPFILE_ERR" "${file_name%in}err")
        
        if [[ "$DIFF_OUT" == "" ]] && [[ "$DIFF_ERR" == "" ]]
        then
            RESULTS="results : ok | "
        else
            RESULTS="results : WRONG! | "
        fi

        $(`valgrind --error-exitcode=123 --leak-check=full --show-leak-kinds=all --errors-for-leak-kinds=all --main-stacksize=1600000000 --log-file=/dev/null "$PROGRAM_PATH"<"$file_name" >/dev/null 2>&1`)
        if (($? == "0"))
        then
            RESULTS=$"$RESULTS valgrind check : ok      ${file_name#*/}"
        else
            RESULTS=$"$RESULTS valgrind check : WRONG, exit code = $?     ${file_name#*/}"
        fi

        echo "$RESULTS"
    else
        echo "$file_name ERROR!, program terminated with code $?"
    fi
    rm -f "$TMPFILE_OUT $TMPFILE_ERR"
done;
