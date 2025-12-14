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
+ Work out if segment splitting is causing a crash.
+ Check out the little bugs that crop up from view detection when walls are at
funny angles.
+ Light-emitting viewer looks kind of trash.

- Possible extensions:
º Dithering on walls.
º Reuse all the viewer code for fancy lights which essentially raycast. All the
output can just be kind of prebaked though, so long as the lights don't move.
º Randomly generating maze.
º Texturing.


- So perplexed by that crash, seems to sometimes be something to do with
  `SDL_Event` stuff, which is only from key checking, and othertimes from
  within pushing the rendered stuff so possibly bad data or something?
- Note it actually isn't a segfault a lot of the time, but rather something to
  do with freeing corrupted data, so some problem with lord knows what.
- The crash only ever seems to happen on startup, never during running.
  Although actually it does occasionally happen upon hitting x. Weird stuff.
- Miraculously, that patch to extension calculation seems to have stopped it.
  Absolutely no idea why that would be the case.
