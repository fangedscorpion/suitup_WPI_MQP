import bpy 
import json

bones = bpy.data.armatures["Armature"].bones
bdict = dict()

for key in bones.keys():
    bdict[key] = {
        'head': list(bones[key].head_local),
        'tail': list(bones[key].tail_local),
	    'frame': {
	        'x': list(bones[key].x_axis),
	        'y': list(bones[key].y_axis),
	        'z': list(bones[key].z_axis)
	    },
    } 
    if bones[key].parent is not None:
        bdict[key]['parent_name'] = bones[key].parent.name
    else:
        bdict[key]['parent_name'] = None

with open("pts.out",'w') as f:
    json.dump(bdict,f,indent="  ",sort_keys=True)

bpy.ops.export_scene.obj(filepath="biped_rig.obj")
