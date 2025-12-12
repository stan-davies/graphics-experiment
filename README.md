# Graphics Experiment

- putting textures on geometry is perfectly possible but looks weird because
    the pixels of the texture are being mapped without any kind of perspective
    projection (since this is just in 2D). We could fix this by moving over to 
    a system that utilises opengl and so we can do a custom fragment shader to 
    sort it all out, but that seems like something of a jump for now
- so don't bother with textures for now, try and complete a simple project
    that goes without and you can always do textures as a later thing with
    opengl
- In that case, a binary system for ppms might be a great idea.


- So for now,
+ Iron out bugs in the segment splitting, including that occasional segfault.
+ Iron out the issues in view detection.
+ Motion doesn't work quite right, also add collision detection.
+ Make viewer emit light, so vertex distance is mapped onto brightness.

- Possible extensions:
º Dithering on walls.
º Reuse all the viewer code for fancy lights which essentially raycast. All the
output can just be kind of prebaked though, so long as the lights don't move.
º Randomly generating maze.
º Collision detection.
º Texturing.


- So perplexed by that segfault, seems to sometimes be something to do with
  `SDL_Event` stuff, which is only from key checking, and othertimes from
  within pushing the rendered stuff so possibly bad data or something?
