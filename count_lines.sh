#!/bin/bash   

header_size=21
# wc -l source/*.cpp include/*.h | awk '{print $1 - '$header_size'}'
sum_line=0
for i in source/*.cpp include/*.h axWidgets/*.cpp axWidgets/*.h; do
	k=$(wc -l $i | awk '{print $1 - '$header_size'}')

	# echo $k
	let sum_line=sum_line+k
done

line_to_remove=$(wc -l source/axDefaultFontBuffer.cpp | awk '{print $1}') 
# echo $to_remove

let sum_line=sum_line-line_to_remove

echo Total number of lines : $sum_line





