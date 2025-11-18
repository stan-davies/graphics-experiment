# Graphics Experiment

Where to go next:
- 3D looking stuff
'-> putting textures on geometry is perfectly possible but looks weird because
    the pixels of the texture are being mapped without any kind of perspective
    projection (since this is just in 2D). We could fix this by moving over to 
    a system that utilises opengl and so we can do a custom fragment shader to 
    sort it all out, but that seems like something of a jump for now
'-> so plain colours for now, add awesome texturing stuff later with opengl

- perhaps use binary system for ppms    = o or some other compression
'-> not bothering with texturing for now so don't worry about that yet lol!



- faux-3D stuff plan:
        º make it seems 3D with triangle magic
        '-> only render the visible extent of walls by picking the nearest wall
        and having it occupy (perhaps by storing occupied intervals) a certain
        interval in the field of view, then when drawing farther away walls,
        culling any part of them in occupied regions. We can also then
        terminate early if the field of view has been fully occupied.
        º fix this problem with walls flattening to the left and the other one
        about the slightly off interpolation
        º alternatively, go for this line intersection approach and just iron
        out the few issues with that


- add a different way of making the world
'-> add vertices by clicking on wall
'-> click anywhere to start drawing a shape, once it contains three vertices,
they all come alive


*collision detetction =(*
