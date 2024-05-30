import bpy

collection_name = "Keys"

mesh_data_name = "R1_Mod"

#collection = bpy.data.collections.get(collection_name).objects ## Collection
#collection = bpy.context.selected_objects ## Selected

for obj in collection:
    if obj.type == 'MESH':
        if mesh_data_name in bpy.data.meshes:
#            obj.data = bpy.data.meshes[mesh_data_name] ## set
#            obj.data = obj.data.copy() ## unset
            pass