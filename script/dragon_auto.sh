#!/bin/bash

FILTER='libsrc|test'
AUTOMK=${PWD}/Makefile.auto
AUTOKC=${PWD}/Kconfig.auto

if [ -f "${AUTOMK}" ] | [ -f "${AUTOKC}" ];then
	echo remove old $AUTOMK $AUTOKC
	rm ${AUTOMK}
	rm ${AUTOKC}
fi

myfunc()
{
	for x in $(find .|grep -Ev ${FILTER}|grep Makefile.in)
        do
                if [ -f "$x" ];then
			#echo $x insert ${AUTOMK}
			echo  >>${AUTOMK}
			cat $x |sed "s:\$AUTOPATH:`dirname $x`:g" >>${AUTOMK}
			echo  >>${AUTOMK}
                fi
        done


        for x in $(find .|grep -Ev ${FILTER} |grep Kconfig.in)
        do
		if [ -f "$x" ];then
			#echo $x insert ${AUTOMK}
			echo  >>${AUTOKC}
                        cat $x |sed "s:\$AUTOPATH:`dirname $x`:g" >>${AUTOKC}
			echo  >>${AUTOKC}
		fi

        done
}


myfunc
#cat ${AUTOMK}
#cat ${AUTOKC}
