#!/bin/sh


PARA="wikipediaEpisodesEcbatana-174-0.10.txt"
DIRECTORY="murakami@kiwi.logos.ic.i.u-tokyo.ac.jp:/home/murakami/experiment0102/"
scp ${DIRECTORY}${PARA} ./
./a.out ${PARA}

PARA="wikipediaEpisodesEcbatana-174-0.05.txt"
DIRECTORY="murakami@kiwi.logos.ic.i.u-tokyo.ac.jp:/home/murakami/experiment0107/"
scp ${DIRECTORY}${PARA} ./
./a.out ${PARA}

PARA="wikipediaEpisodesEcbatana-174-0.01.txt"
DIRECTORY="murakami@kiwi.logos.ic.i.u-tokyo.ac.jp:/home/murakami/experiment0108/"
scp ${DIRECTORY}${PARA} ./
./a.out ${PARA}

PARA="wikipediaEpisodesEcbatana-174-0.30.txt"
DIRECTORY="murakami@kiwi.logos.ic.i.u-tokyo.ac.jp:/home/murakami/experiment0108-2/"
scp ${DIRECTORY}${PARA} ./
./a.out ${PARA}

PARA="wikipediaEpisodesEcbatana-174-0.15.txt"
DIRECTORY="murakami@kiwi.logos.ic.i.u-tokyo.ac.jp:/home/murakami/experiment0109/"
scp ${DIRECTORY}${PARA} ./
./a.out ${PARA}
