#!/bin/sh
make

CLEAN="t"

builder() {
	SOURCE="$1"
	SNAME="`basename \"${SOURCE}\"`"
	DNAME="`dirname \"${SOURCE}\"`"
	mkdir -p ../output/${DNAME}
	../build/prog -s -n 8 -d ${SOURCE} -b ../output/${DNAME}/${SNAME}.bin -p ../output/${DNAME}/${SNAME}.png
}

cd data/
for dname in */*; do
	builder "${dname}" &
done
wait

cd ..
for bname in output/*/*.bin; do
	python load_c_data.py "${bname}" &
done
wait

if [ "${CLEAN}" = "t" ]; then
	rm output/*/*.bin
	rm output/*/*.png
fi
