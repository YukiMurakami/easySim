#!/bin/sh
IFS_BACKUP=$IFS
IFS=$'\n'

#echo "Warning : already downloaded"
#exit 0


DIRECTORY="murakami@kiwi.logos.ic.i.u-tokyo.ac.jp:/home/murakami/experiment"
PARA="wikipediaEpisodes"
CP="-0.20"
PLACE=("Memphis-5" "Alexandria-11" "Babylon-19" "Tyre-30" "Gordion-45" "Babylon-52" "Persepolis-61" "Gaugamela-62" "Bactra-88" "Sagala-103" "Issus-121" "Granicus-136" "Miletus-146" "Samarkand-158" "Alexandria-173" "Ecbatana-174" "A\ Indus-179" "Granicus-196" "Sardis-201" "Alexandria-203" "Memphis-297" "Sidon-341" "Ankara-356" "Issus-372" "Arbela-378" "Alexandria-413" "Aornos-414" "Nicaea-422" "Babylon-436" "Susa-455" "Patala-462" "A\ Eschate-478")
PLAYOUT="20000"


echo "playout:"${PLAYOUT}"download"
a=0
while [ $a -ne 32 ]
do
scp ${DIRECTORY}"0214-6/"${PARA}${PLACE[a]}${CP}".txt" ./${PLAYOUT}
a=`expr $a + 1`
done


PLAYOUT="200000"
echo "playout:"${PLAYOUT}"download"
a=0
while [ $a -ne 32 ]
do
if [ $a -lt 8 ]
then
    scp ${DIRECTORY}"0214-7/"${PARA}${PLACE[a]}${CP}".txt" ./${PLAYOUT}
else
    if [ $a -lt 16 ]
    then
        scp ${DIRECTORY}"0214-8/"${PARA}${PLACE[a]}${CP}".txt" ./${PLAYOUT}
    else
        if [ $a -lt 24 ]
        then
            scp ${DIRECTORY}"0214-9/"${PARA}${PLACE[a]}${CP}".txt" ./${PLAYOUT}
        else
            scp ${DIRECTORY}"0214-10/"${PARA}${PLACE[a]}${CP}".txt" ./${PLAYOUT}
        fi
    fi
fi
a=`expr $a + 1`
done