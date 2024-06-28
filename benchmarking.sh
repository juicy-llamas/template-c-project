#!/bin/sh

# this is the benchmarking file

check_err () {
    [ ! $? -eq 0 ] && { 
        printf "\n\033[$2;40m\033[1m$1\033[0m\n";
        exit 1;
    }
}

mkdir -p "$BENCHODIR"

printf "\n========================================\n"
printf "\033[1m\033[96;40m	Compiling Bench Main Fn\033[0m\n\n"

TRIALS=${TRIALS:-512}

BENCH_O=${BENCHMAIN##*/}
BENCH_O=${BENCH_O%.c}.o
gcc "$BENCHMAIN" -o "$BDIR/$BENCH_O" $build_flags \
    -DTRIALS=$TRIALS $base_flags

for i in $(find "$BENCHDIR" ! -path "$BENCHMAIN" ! -path "*/$IGNFLS/*" \
        -type f); do
    i=${i##*/}
    name=${i%.*}
    
    printf "========================================\n"
    printf "\033[1m\033[96;40m	Compiling Benchmark $name\033[0m\n\n"

    # automated benchmarks?
    if [ "${i##*.}" == "json" ]; then 
        echo "automatic bench support coming"
    else
        cp "$BENCHDIR/$i" "$BENCHODIR/$i" 
    fi
   
    gcc "$BENCHODIR/$i" -o "$BDIR/$name.o" $build_flags $base_flags
    check_err "Compilation failed\n" 93

    gcc "$BDIR/$name.o" "$BDIR/$BENCH_O" $TEST_O -o \
        $BDIR/bench_test_$name $build_lflags $base_lflags
    check_err "Linking failed\n" 95
done

# benchmark tests as well with -DBENCH_TESTING
if [ ! -z $BENCH_TEST ]; then
    gcc_unused=$([ $COMPLETE ] || echo "-Wno-unused-variable";);
    
    for i in $(find "$TODIR" ! -path "*/$IGNFLS/*" -type f); do
        i=${i##*/}
        name=${i%.*}
    
        printf "========================================\n"
        printf "\033[1m\033[96;40m	Compiling Benchmark $name\033[0m\n\n"

        gcc "$TODIR/$i" -o "$BDIR/$name.o" $build_flags -DBENCH_TESTING \
            $gcc_unused $base_flags
        check_err "Compilation failed\n" 93

        gcc "$BDIR/$name.o" "$BDIR/$BENCH_O" $TEST_O -o \
            $BDIR/bench_test_$name $build_lflags $gcc_unused $base_lflags
        check_err "Linking failed\n" 95
    done
fi

# Now it's time to run the benchmarks
for i in $(find "$BDIR" -maxdepth 1 -path "*bench_test_*" -type f); do
    name=${i##*bench_test_} # ~~./bench_test_~~ test_name
    
    
    printf "========================================\n"
    printf "\033[1m\033[96;40m	Running Benchmark $name\033[0m\n\n"
    
    ind=0
    BENCHES=${BENCHES:-1}
    tot=0
    min=999999999999

    while [ $ind -lt $BENCHES ]; do
        printf "BENCH $ind\r"
        k=$($BDIR/bench_test_$name)
        # !!!!! NOTE: YOU NEED A FLOATING POINT CALCULATOR APP.
        # I'm just gonna assume you have python
        tot=$(python3 -c "print( $k + $tot )")
        if [ ! -z $(python3 -c "if $k < $min: print( 'yes' )" ) ]; then
            min=$k
        fi
        ind=$(( ind + 1 ))
    done

    avg=$(python3 -c "print( $tot / $BENCHES * 1000000 )")
    min=$(python3 -c "print( $min * 1000000 )")
    printf "\nAVERAGE TIME for $name: $avg microseconds\n"
    printf "MIN TIME for $name: $min microseconds\n"
done
