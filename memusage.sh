# Returns The Memory Used The By $1 Process
x=`pmap $1 | tail -n 1 | awk '/[0-9]K/{print $2}'`
echo $x
