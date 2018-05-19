#!/bin/bash

NUMBER_EVENTS=100

for P in {2..71..2}; do
    OUTPUT_FILE="/sphenix/user/sekelsky/proton_DST_p${P}.root"
    root -b -q Fun4All_G4_EICDetector_RICH.C\(${NUMBER_EVENTS},${P}\)
    mv G4EICDetector.root ${OUTPUT_FILE}
done