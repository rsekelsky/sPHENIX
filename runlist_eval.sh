#!/bin/bash

NUMBER_EVENTS=100
PARTICLE=kaon

for P in {2..71..2}; do
    OUTPUT_FILE="/sphenix/user/sekelsky/particle_simulations/${PARTICLE}_nofield/emission_evals/eval_p${P}.root"
    INPUT_FILE="/sphenix/user/sekelsky/particle_simulations/${PARTICLE}_nofield/${PARTICLE}_DST_p${P}.root"
    root -b -q Fun4All_RICHReco.C\(${NUMBER_EVENTS},\"${INPUT_FILE}\"\)
    mv eval_RICH.root ${OUTPUT_FILE}
done