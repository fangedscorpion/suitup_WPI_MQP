mv biped_rig.mtl biped_rig.mtl.bak # keep custom materials safe
blender biped_rig.blend --background --python getPts.py
mv biped_rig.mtl.bak biped_rig.mtl # replace exported blender materials with custom materials
./getMtlNames.py