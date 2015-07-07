#!/bin/bash


FILTER='libsrc|test'
AUTOMK=${PWD}/Makefile.auto
AUTOKC=${PWD}/Kconfig.auto

if [ -f "${AUTOMK}" ] | [ -f "${AUTOKC}" ];then
	#echo remove old $AUTOMK $AUTOKC
	rm ${AUTOMK}
	rm ${AUTOKC}
fi

myfunc()
{
		for x in $(find $1|grep -Ev ${FILTER}|grep Makefile.in)
        do
                if [ -f "$x" ];then
			#echo $x insert ${AUTOMK}
			echo  >>${AUTOMK}
			cat $x |sed "s:\$AUTOPATH:`dirname $x`:g" >>${AUTOMK}
			echo  >>${AUTOMK}
                fi
        done


        for x in $(find $1|grep -Ev ${FILTER} |grep Kconfig.in)
        do
		if [ -f "$x" ];then
			#echo $x insert ${AUTOMK}
			echo  >>${AUTOKC}
                        cat $x |sed "s:\$AUTOPATH:`dirname $x`:g" >>${AUTOKC}
			echo  >>${AUTOKC}
		fi

        done
}

for dir in $(ls -d ${TOPDIR}/*/ |grep -Ev "widget| bjs|lib|libsrcXsamples" )
do
	myfunc $dir
done


echo 'comment "------------Dragon Widget Configuretion---------"' >>${AUTOKC}

#echo 'menu "HUI widget Option"' >>${AUTOKC}
myfunc ${TOPDIR}/widget
#echo 'endmenu' >>${AUTOKC}

#cat ${AUTOMK}
#cat ${AUTOKC}
