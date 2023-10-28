
i = 0 

while true 
do 
	curl  http://10.13.4.15:9090/ | 	curl  http://10.13.4.15:9090/  | curl  http://10.13.4.15:9090/ | 	curl  http://10.13.4.15:9090/  |  curl  http://10.13.4.15:9090/ | 	curl  http://10.13.4.15:9090/  | curl  http://10.13.4.15:9090/ | 	curl  http://10.13.4.15:9090/    2>  /dev/null 
	if [ $? -eq 0 ]; 
	then
		printf "\e[0;32msuccess %d\n" $i
	else 
		printf "\e[0;31mfail\n"
	fi
	i=$((i+1))
	curl  http://127.0.0.1:8080/  1>  /dev/null 2>  /dev/null 
	if [ $? -eq 0 ]; 
	then
		printf "\e[0;32msuccess %d\n" $i
	else 
		printf "\e[0;31mfail\n"
	fi
	i=$((i+1))
done 