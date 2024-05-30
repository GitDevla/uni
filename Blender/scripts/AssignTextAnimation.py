import bpy

## Parameters
START_FRAME = 560
FRAMES_BETWEEN_KEYPRESS = 8
TEXT_TO_TYPE = "print('Hello World')"

## Constatnts
action_press = bpy.data.actions.get("key_press")
action_down = bpy.data.actions.get("key_down")
action_up = bpy.data.actions.get("key_up")
keys_collection = bpy.data.collections.get("Keys")

text_obj = bpy.data.objects["ConsoleText"]
geo_nodes_modifier = text_obj.modifiers.get("GeometryNodes")
node_tree = geo_nodes_modifier.node_group
value_node = node_tree.nodes.get("Slice String")

## Setup the text that will be typed
value_node.inputs.get("String").default_value = TEXT_TO_TYPE
value_node.inputs.get("Length").default_value = 0
value_node.inputs.get("Length").keyframe_insert(data_path="default_value", frame=START_FRAME)


def getKeyCap(character):
    key_name = f"Key_{character.lower()}"
    return keys_collection.objects.get(key_name)

def appendAction(obj, action):
    obj_action = obj.animation_data.action
    
    for fcurve in action.fcurves:
        new_fcurve = obj_action.fcurves.find(data_path=fcurve.data_path, index=fcurve.array_index)
        if not new_fcurve:
            new_fcurve = obj_action.fcurves.new(data_path=fcurve.data_path, index=fcurve.array_index)
            
        new_fcurve.keyframe_points.add(count=len(fcurve.keyframe_points))
        for i, keyframe in enumerate(fcurve.keyframe_points):
            new_fcurve.keyframe_points[-len(fcurve.keyframe_points) + i].co = keyframe.co
            new_fcurve.keyframe_points[-len(fcurve.keyframe_points) + i].interpolation = keyframe.interpolation
            new_fcurve.keyframe_points[-len(fcurve.keyframe_points) + i].handle_left = keyframe.handle_left
            new_fcurve.keyframe_points[-len(fcurve.keyframe_points) + i].handle_right = keyframe.handle_right

def offsetAction(anim, delta):
  for fcurve in anim.fcurves:
    for keyframe in fcurve.keyframe_points:
        keyframe.co[0] += delta
        keyframe.handle_left[0] += delta
        keyframe.handle_right[0] += delta  

def assignAction(obj,action,frame):
    new_action = action.copy()
    offsetAction(new_action,frame)
    appendAction(obj,new_action)

## Clear all animations
for key in keys_collection.objects:
    key.animation_data.action = None

## Create empty animations
for key in keys_collection.objects:
    key.animation_data_create()
    key.animation_data.action = bpy.data.actions.new(name=f"Empty Action")
    
## Assign the animations
current_frame = START_FRAME
for char in TEXT_TO_TYPE:
    if char == " ":
        char = "Space"
    need_shift = char.isupper()
    if char == "(":
        char = "9"
        need_shift = True
    elif char == ")":
        char = "0"
        need_shift = True
    elif char == "!":
        char = "1"
        need_shift = True

    if need_shift:
        assignAction(getKeyCap("LShift"), action_down, current_frame)
        current_frame += FRAMES_BETWEEN_KEYPRESS
        assignAction(getKeyCap(char), action_press, current_frame)
        current_frame += FRAMES_BETWEEN_KEYPRESS
        assignAction(getKeyCap("LShift"), action_up, current_frame)
        current_frame += FRAMES_BETWEEN_KEYPRESS
    else:
        key_obj = getKeyCap(char.lower())
        assignAction(key_obj,action_press,current_frame)
        current_frame += FRAMES_BETWEEN_KEYPRESS
        
    value_node.inputs.get("Length").default_value += 1
    value_node.inputs.get("Length").keyframe_insert(data_path="default_value", frame=current_frame-5)
