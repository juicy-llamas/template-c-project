#!/bin/sh

num_t=0;
num_s=0;
mkdir -p $TEST_LOG_DIR;

max=0;
for i in $(ls test_logs); do
    tmp=$(echo $i | tail -c 2);
    [ $tmp -gt $max ] && max=$tmp;
done;
max=$(( $max + 1 ));
test_log_fil="$TEST_LOG_DIR/$TEST_LOGFILE_BASE$max";

for i in $(find "$TODIR" ! -path "*/$IGNFLS/*" -type f); do
    i=${i#$TODIR};
    test_files_arr="$TEST_FILES";
    name=${i#/};
    dir=$BDIR/${i%/*};
    if ! [ "$test_files_arr" = '' ]; then
        skip="true";
        for j in $test_files_arr; do
            if [ "$j" = "$name" ]; then
                skip="false";
                break;
            fi;
        done;
        [ $skip = "true" ] && continue;
    fi;
    mkdir -p $dir;
    echo "" ;
    echo "" >> $test_log_fil;
    echo "========================================" ;
    echo "========================================" >> $test_log_fil;
    printf "\033[1m\033[96;40m	Test $name\033[0m\n" ;
    printf "	Test $name\n" >> $test_log_fil;
    echo "" ;
    echo "" >> $test_log_fil;
    num_t=$(( num_t+=1 ));
    gcc_unused=$([ $COMPLETE ] || echo "-Wno-unused-variable";);
    gcc $TODIR$i -o $BDIR${i%%.c}.o $debug_flags $gcc_unused $base_flags;
    exit_status=$?;
    coutp="$(gcc $TODIR$i -o $BDIR${i%%.c}.o \
                $debug_flags $gcc_unused $base_flags 2>&1)";
    [ -z "$coutp" ] || { echo ""; };
    [ $exit_status -ne 0 ] && {
        echo $coutp >> $test_log_fil;
        echo "" >> $test_log_fil;
        printf "\033[93;40m\033[1mCompilation failed\033[0m\n";
        printf "Compilation Failed\n" >> $test_log_fil; continue;
    };
    gcc $TEST_O $BDIR${i%%.c}.o -o cur_test $debug_lflags $base_lflags;
    [ $? -ne 0 ] &&
        { echo "" | tee -a $test_log_fil; 
          printf "\033[95;40m\033[1mLinking failed\033[0m\n";
          printf "Linking failed\n">> $test_log_fil; continue; };
    ./cur_test 2>&1 >> /dev/null;
    exit_status=$?;
    if [ $exit_status -eq 0 ]; then
        if [ "$PRINT_SUCCESS" = "y" ]; then
            echo "" ;
            echo "" >> $test_log_fil;
            ./cur_test 2>&1 | tee -a $test_log_fil;
        fi;
        printf "\033[1m\033[92;40mSuccess\033[0m\n" ;
        printf "Success\n" >> $test_log_fil;
        num_s=$(( num_s+=1 ));
    else
        ./cur_test 2>&1 | tee -a $test_log_fil;
        echo "" ;
        echo "" >> $test_log_fil;
        printf "\033[1m\033[91;40mFailure\033[0m\n" ;
        printf "Failure\n" >> $test_log_fil;
        mkdir -p ./failed_tests;
        cp --backup=numbered cur_test ./failed_tests/$name.out;
    fi;
    rm -f cur_test;
done;
echo "" ;
echo "" >> $test_log_fil;
echo "========================================" ;
echo "========================================" >> $test_log_fil;
printf "\033[1m\033[96;40m	Conclusions\033[0m\n" ;
printf "	Conclusions\n" >> $test_log_fil;
echo "" ;
echo "" >> $test_log_fil;
if [ $num_s -eq $num_t ]; then
    printf "\033[1m\033[92;40m$num_t\033[0m test(s) succeeded\n" ;
    printf "$num_t test(s) succeeded\n" >> $test_log_fil;
else
    printf "\033[1m\033[91;40m$(( num_t-num_s )) out of%s\033[0m\n" \
                    " $num_t tests failed"; 
    printf "$(( num_t-num_s )) out of\n $num_t tests failed" \
                    >> $test_log_fil;
fi;
echo "" ;
echo "" >> $test_log_fil;
