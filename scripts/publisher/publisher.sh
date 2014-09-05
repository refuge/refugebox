#!/bin/bash

#
# Basic script sending 2 fields
# to the thingspeak.com site
# 
# The KEY is the API key assigned to a given channel
#
# This script is supposed to listen to the tty device /dev/ttymxc3
# which sends the results with the following format:
#    field1;field2
# with each field using the format:
#    label:value
# Example:
#    P1:1024.3;T1:26.5
#

pipe=/dev/ttymxc3
KEY=PUT_YOU_API_KEY_HERE

while true
do
    if read line <$pipe; then

	values=""

	IFS=\; read -a fields <<< "$line"
	for (( i=0 ; i < ${#fields[@]} ; i++ )) ; do
		f=${fields[i]}
		echo "field: $f"

		IFS=: read -a elements <<< "$f"
		label=${elements[0]}
		value=${elements[1]}

		if [ ! -z "$values" ]; then
			values="${values}&"
		fi

		num=$((i + 1))
		values="${values}field${num}=${value}"
	done
	
	url="http://api.thingspeak.com/update?key=${KEY}&${values}"
	echo "Calling url: ${url}"
	curl "$url" 2>&1 >/dev/null 
	echo "done"
    fi
done

echo "Reader exiting"
