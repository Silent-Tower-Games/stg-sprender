START=`date +%s%N`

PIDS=()

for filename in ./assets/shaders/*.fx; do
    wine /usr/bin/efb.exe /I ./assets/shaders/ $filename $filename'b'
    PIDS+=$!' '
done

for pid in ${PIDS[*]}; do
    wait $pid
done

let END=`date +%s%N`-$START

echo That took ${END} NS
