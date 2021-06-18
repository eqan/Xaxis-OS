
#Assigning Resources And Starting OS

cores=$3
ram=$(($1*1024*1024))
disk=$(($2*1024*1024))
echo $disk > osspecs
echo $ram >> osspecs
echo $cores >> osspecs
echo "root" >> osspecs
g++ runos.cpp -o Xaxis -lpthread
taskset -c $cores ./Xaxis
