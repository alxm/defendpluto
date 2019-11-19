#!/bin/bash

output=""
make_arg=$1

record_result ()
{
    output="$output\n\033[$1;1m$2\033[0m"
}

color_text ()
{
    echo -e "\033[$1;1m$2\033[0m"
}

for f in Makefile*
do
    color_text 34 ">>>> START $f"
    make -k -j8 -f "$f" $make_arg

    if [ $? -eq 0 ]
    then
        record_result 32 "[ Success ] $f"
    else
        record_result 31 "[ Failure ] $f"
    fi

    color_text 35 "<<< END $f\n"
done

echo -e $output
