#!/bin/bash

cat > list_mass.txt << EOF
EOF

for mass in `less list_mass.txt`;do

    echo CATBO_M${mass}

    combineCards.py CatB=card_m${mass}_catB_bern_forFit.txt CatO=card_m${mass}_catO_bern_forFit.txt > card_m${mass}_catBO_bern.txt

    combine -M AsymptoticLimits card_m${mass}_catBO_bern.txt --setParameters pdf_index=0 --freezeParameters pdf_index -m ${mass} --name _CATBO_M${mass}_PDF0_2016_2017_2018 --rMin -50 --rMax 50

done