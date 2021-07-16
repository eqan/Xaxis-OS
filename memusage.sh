# Returns The Memory Used The By $1 Process
echo `pmap $1 | tail -n 1 | awk '/[0-9]K/{print $2}'` >> ~/Desktop/Xaxis-OS/processes_mem
#echo $x >> processes_mem
