filename=$1
while read line;
do
echo $line | timeout 5 nc -u 172.16.18.152 42069
sleep 60
done < $filename
