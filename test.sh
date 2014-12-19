#! /bin/bash
# v[0]=0
# v[1]=3
# v[2]=7

# srcList = ls 'source/'

echo '-------------------------------------------'
echo 'Core'
echo '-------------------------------------------'

cd 'include'

for x in $(ls)
do
	wc ${x}
done

cd '../source'

for x in $(ls)
do
	wc ${x}
done

echo '-------------------------------------------'
echo 'Widgets'
echo '-------------------------------------------'
cd '../axWidgets'

for x in $(ls)
do
	wc ${x}
done

