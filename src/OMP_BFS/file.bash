#!/bin/bash

YELLOW='\033[0;92m'
NORMAL='\033[0;39m'
BLUE='\033[0;34m'

TIMEFORMAT='%3U;%3E;%3S;%P'
TIMESTAMP=$(date +%F.%T)

NMEASURES=50
GRAPH_NUM=(1 2 3 4)
ARRAY_THS=(0 1 2 4 6 8 12 16 24 32)
ARRAY_OPT=(0 1 2 3)


SCRIPTPATH=$2

for num in "${GRAPH_NUM[@]}"; do
	for nTh in "${ARRAY_THS[@]}"; do
		for opt in "${ARRAY_OPT[@]}"; do

            nThStr=$(printf "%02d" $nTh)

            OUT_FILE=$SCRIPTPATH/measure_bfs/$num/BFS-$nThStr-O$opt.csv

            mkdir -p $(dirname $OUT_FILE)

            printf "${NORMAL}"
            echo $(basename $OUT_FILE)
            echo 'dimGraph;timeInt;'>$OUT_FILE

            if [[ $nTh -eq 0 ]]; then
                ($1/main_bfsO$opt graph_00$num $NMEASURES)2>&1 | sed -e 's/,/./g' -e 's/\n/;/g'  >> $OUT_FILE
            else
                ($1/main_bfs_parallelO$opt graph_00$num $nTh $NMEASURES)2>&1 | sed -e 's/,/./g' -e 's/\n/;/g'  >> $OUT_FILE
            fi
            
            printf "\n"

        done
    done
done

