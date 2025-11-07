# Graphics Experiment

Where to go next:
- 3D looking stuff
'-> putting textures on geometry is perfectly possible but looks weird because
    the pixels of the texture are being mapped without any kind of perspective
    projection (since this is just in 2D). We could fix this by moving over to 
    a system that utilises opengl and so we can do a custom fragment shader to 
    sort it all out, but that seems like something of a jump for now
'-> so plain colours for now, add awesome texturing stuff later with opengl
'-> so create a doom like system perhaps!? do all the projection stuff myself
    and calculate which triangles need drawing with that
- move game loop to own system
- work out what to do about passing key presses to the subject update function
                        // update function will really only need `e.key`, which
                        // contains key pressed as well as key up/down,
                        // pressed/released, etc I have not decided how to deal
                        // with not looking at data if the event is not a
                        // keydown, so for know we send everyting and check in
                        // update function
                        // could attach a bool with 'keypress' but then that is
                        // almost just what we are doing anyway
                        // split only arises for stuff with passive behaviour
- take window dimensions into a more malleable form

- perhaps use binary system for ppms    = o or some other compression
'-> not bothering with texturing for now so don't do that lol!



- faux-3D stuff plan:
        º make sure only the right walls are being picked up (?)
        º make it seems 3D with triangle magic
