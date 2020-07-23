available_disk_number = `echo "1+"$$(ls /dev |  sort | gsed -n "s/^disk\([0-9]\)*/\1/p" | tail -n 1) | bc`

all:
	echo $(available_disk_number)
