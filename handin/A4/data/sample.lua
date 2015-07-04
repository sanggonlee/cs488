-- "space colony" theme scene --

mat1 = gr.material({0.7, 1.0, 0.7}, {0.5, 0.7, 0.5}, 25)
shiny_black = gr.material({0.15, 0.15, 0.15}, {1.0, 1.0, 1.0}, 3)
shiny_white = gr.material({1.0, 1.0, 1.0}, {1.0, 1.0, 1.0}, 40)
orange = gr.material({0.7, 0.0, 0.7}, {0.0, 0.0, 0.0}, 20)

scene_root = gr.node('root')

black_ball = gr.nh_sphere('black_ball', {20, -100, -50}, 40)
scene_root:add_child(black_ball)
black_ball:set_material(shiny_black)

ball2 = gr.nh_sphere('ball2', {-100, -100, -130}, 30)
scene_root:add_child(ball2)
ball2:set_material(shiny_white)

ball3 = gr.nh_sphere('ball3', {-90, -55, -115}, 15)
scene_root:add_child(ball3)
ball3:set_material(shiny_white)

container1 = gr.nh_box('container1', {-80, -110, -15}, 20)
scene_root:add_child(container1)
container1:set_material(mat1)

container2 = gr.nh_box('container2', {80, -110, -60}, 25)
scene_root:add_child(container2)
container2:set_material(orange)

planet = gr.nh_sphere('planet', {0, -3000, 100}, 2900)
scene_root:add_child(planet)
planet:set_material(shiny_white)


require('readobj')
shuttle = gr.mesh('shuttle', readobj('shuttle.obj'))
scene_root:add_child(shuttle)
shuttle:set_material(shiny_white)
shuttle:scale(3, 3, 3)


white_light = gr.light({20.0, 100.0, 50.0}, {0.9, 0.9, 0.9}, {1, 0, 0})
--orange_light = gr.light({400.0, 100.0, 150.0}, {0.7, 0.0, 0.7}, {1, 0, 0})

gr.render(scene_root, 'sample.png', 500, 500,
	  {10, 30, 200}, {0, 0, -1}, {0, 1, 0}, 50,
	  {0.2, 0.2, 0.2}, {white_light})
