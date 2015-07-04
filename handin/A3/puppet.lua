--
-- Puppet model 'Pinocchio' by Sang kon Lee (sk5lee)
-- 

rootnode = gr.node('root')

red = gr.material({1.0, 0.0, 0.0}, {0.1, 0.1, 0.1}, 10)
blue = gr.material({0.0, 0.0, 1.0}, {0.1, 0.1, 0.1}, 10)
green = gr.material({0.0, 1.0, 0.0}, {0.1, 0.1, 0.1}, 10)
white = gr.material({1.0, 1.0, 1.0}, {0.1, 0.1, 0.1}, 10)
yellow = gr.material({1.0, 1.0, 0.0}, {0.1, 0.1, 0.1}, 10)

torso = gr.sphere('torso')
rootnode:add_child(torso)
torso:set_material(white)
torso:scale(2.0, 3.0, 1.5)

--
-- upper body parts
--

shoulders_joint = gr.joint('shoulders_joint', {-90, 0, 90}, {0, 0, 0})
torso:add_child(shoulders_joint)

shoulders = gr.sphere('shoulders')
shoulders_joint:add_child(shoulders)
shoulders:set_material(blue)
shoulders:translate(0.0, 2.5, 0.0)
shoulders:scale(3.0, 0.5, 1.5)

-- neck and head + etc
neck_joint = gr.joint('neck_joint', {0, 0, 0}, {-30, 0, -30})
shoulders:add_child(neck_joint)

neck = gr.sphere('neck')
neck_joint:add_child(neck)
neck:set_material(green)
neck:translate(0.0, 0.9, 0.0)
neck:scale(0.7, 0.7, 0.7)

head_joint = gr.joint('head_joint', {-30, 0, 30}, {-60, 0, 60})
neck:add_child(head_joint)

head = gr.sphere('head')
head_joint:add_child(head)
head:set_material(white)
head:translate(0.0, 1.8, 0.0)
head:scale(1.6, 1.6, 1.6)

nose = gr.sphere('nose')
head:add_child(nose)
nose:set_material(red)
nose:translate(0.0, 0.0, 2.0)
nose:scale(0.2, 0.2, 2.0)

-- left arm parts
left_upper_arm_joint = gr.joint('left_upper_arm_joint', {-160, 0, 160}, {0, 0, 0})
shoulders:add_child(left_upper_arm_joint)

left_upper_arm = gr.sphere('left_upper_arm')
left_upper_arm_joint:add_child(left_upper_arm)
left_upper_arm:set_material(yellow)
left_upper_arm:translate(3.0, -1.7, 0.0)
left_upper_arm:scale(0.6, 1.8, 0.6)

left_forearm_joint = gr.joint('left_forearm_joint', {-160, 0, 160}, {0, 0, 0})
left_upper_arm:add_child(left_forearm_joint)

left_forearm = gr.sphere('left_forearm')
left_forearm_joint:add_child(left_forearm)
left_forearm:set_material(green)
left_forearm:translate(0.0, -3.0, 0.0)
left_forearm:scale(0.4, 1.7, 0.7)

left_hand_joint = gr.joint('left_hand_joint', {-90, 0, 90}, {0, 0, 0})
left_forearm:add_child(left_hand_joint)

left_hand = gr.sphere('left_hand')
left_hand_joint:add_child(left_hand)
left_hand:set_material(white)
left_hand:translate(0.0, -1.8, 0.0)
left_hand:scale(0.4, 0.4, 0.4)

-- right arm parts
right_upper_arm_joint = gr.joint('right_upper_arm_joint', {-160, 0, 160}, {0, 0, 0})
shoulders:add_child(right_upper_arm_joint)

right_upper_arm = gr.sphere('right_upper_arm')
right_upper_arm_joint:add_child(right_upper_arm)
right_upper_arm:set_material(yellow)
right_upper_arm:translate(-3.0, -1.7, 0.0)
right_upper_arm:scale(0.6, 1.8, 0.6)

right_forearm_joint = gr.joint('right_forearm_joint', {-160, 0, 160}, {0, 0, 0})
right_upper_arm:add_child(right_forearm_joint)

right_forearm = gr.sphere('right_forearm')
right_forearm_joint:add_child(right_forearm)
right_forearm:set_material(green)
right_forearm:translate(0.0, -3.0, 0.0)
right_forearm:scale(0.4, 1.7, 0.7)

right_hand_joint = gr.joint('right_hand_joint', {-90, 0, 90}, {0, 0, 0})
right_forearm:add_child(right_hand_joint)

right_hand = gr.sphere('right_hand')
right_hand_joint:add_child(right_hand)
right_hand:set_material(white)
right_hand:translate(0.0, -1.8, 0.0)
right_hand:scale(0.4, 0.4, 0.4)

--
-- lower body parts
--

hips_joint = gr.joint('hips_joint', {-30, 0, 30}, {0, 0, 0})
torso:add_child(hips_joint)

hips = gr.sphere('hips')
hips_joint:add_child(hips)
hips:set_material(red)
hips:translate(0.0, -3.0, 0.0)
hips:scale(2.0, 1.0, 1.0)

-- left leg parts
left_thigh_joint = gr.joint('left_thigh_joint', {-90, 0, 90}, {0, 0, 0})
hips:add_child(left_thigh_joint)

left_thigh = gr.sphere('left_thigh')
left_thigh_joint:add_child(left_thigh)
left_thigh:set_material(red)
left_thigh:translate(1.5, -2.8, 0.0)
left_thigh:scale(0.7, 3.0, 0.7)

left_calf_joint = gr.joint('left_calf_joint', {0, 0, 90}, {0, 0, 0})
left_thigh:add_child(left_calf_joint)

left_calf = gr.sphere('left_calf')
left_calf_joint:add_child(left_calf)
left_calf:set_material(white)
left_calf:translate(0.0, -4.0, 0.0)
left_calf:scale(0.6, 3.5, 0.6)

left_foot_joint = gr.joint('left_foot_joint', {-90, 0, 90}, {0, 0, 0})
left_calf:add_child(left_foot_joint)

left_foot = gr.sphere('left_foot')
left_foot_joint:add_child(left_foot)
left_foot:set_material(red)
left_foot:translate(0.0, -3.0, 0.6)
left_foot:scale(0.5, 0.5, 1.8)

-- right leg parts
right_thigh_joint = gr.joint('right_thigh_joint', {-90, 0, 90}, {0, 0, 0})
hips:add_child(right_thigh_joint)

right_thigh = gr.sphere('right_thigh')
right_thigh_joint:add_child(right_thigh)
right_thigh:set_material(red)
right_thigh:translate(-1.5, -2.8, 0.0)
right_thigh:scale(0.7, 3.0, 0.7)

right_calf_joint = gr.joint('right_calf_joint', {0, 0, 90}, {0, 0, 0})
right_thigh:add_child(right_calf_joint)

right_calf = gr.sphere('right_calf')
right_calf_joint:add_child(right_calf)
right_calf:set_material(white)
right_calf:translate(0.0, -4.0, 0.0)
right_calf:scale(0.6, 3.5, 0.6)

right_foot_joint = gr.joint('right_foot_joint', {-90, 0, 90}, {0, 0, 0})
right_calf:add_child(right_foot_joint)

right_foot = gr.sphere('right_foot')
right_foot_joint:add_child(right_foot)
right_foot:set_material(red)
right_foot:translate(0.0, -3.0, 0.6)
right_foot:scale(0.5, 0.5, 1.8)

--rootnode:translate(-0.75, 0.25, -10.0)
--rootnode:rotate('y', -20.0)

return rootnode
