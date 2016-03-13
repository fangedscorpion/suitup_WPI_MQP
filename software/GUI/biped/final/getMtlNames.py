#!/usr/bin/env python3

import json

with open("biped_rig.mtl",'r') as matfile:
    matfilecontents = matfile.read()
    matfilelines = matfilecontents.split('\n')

mtlnames = [line.replace("newmtl ","") for line in matfilelines if 'newmtl' in line]
mtlnames = ["DefaultMaterial"] + mtlnames

with open("mtls.out",'w') as f:
    json.dump(mtlnames,f,indent="  ")
