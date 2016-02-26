import bpy 
import json

bones = bpy.data.armatures["Armature"].bones
bdict = {
    key: {
        'head': list(bones[key].head_local),
        'tail': list(bones[key].tail_local)
    } for key in bones.keys()
}

with open("pts.out",'w') as f:
    json.dump(bdict,f,indent="  ",sort_keys=True)

bpy.ops.export_scene.obj(filepath="biped_rig.obj")
