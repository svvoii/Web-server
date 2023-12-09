#!/bin/bash

# Array of data to send in POST requests
declare -a dataArray=("key1=value1&key2=value2" "key1=value3&key2=value4" "key1=value5&key2=value6")

# URL to send requests to
url="http://localhost:8080/"

# Loop over each item in dataArray
for data in "${dataArray[@]}"
do
	# Make POST request with current data item
	curl -X POST -d "$data" $url
done
