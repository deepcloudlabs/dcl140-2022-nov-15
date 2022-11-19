for i in $(seq 1 10) 
do 
time ./parallel_sum | tail -3
done
for i in $(seq 1 10) 
do 
time ./serial_sum | tail -3
done
