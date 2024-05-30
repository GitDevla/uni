import bpy

collection_name = "Switches"

collection = bpy.data.collections.get(collection_name).objects

objects = []
for obj in collection:
    objects.append((obj, obj.location.x, obj.location.y))

objects.sort(key=lambda x: (round(x[2],2), -round(x[1],2)))

print("------------------------------------------------")

starting_frame = 235;
current_frame=starting_frame
for obj, x, y in objects:
    print(obj,round(x,3), round(y,3))
    obj.hide_render = True
    obj.keyframe_insert(data_path="hide_render", frame=0)
    obj.hide_render = False
    obj.keyframe_insert(data_path="hide_render", frame=current_frame)
    current_frame+=1
#    obj.animation_data_clear()