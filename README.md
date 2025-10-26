# Graphics Experiment

Where to go next:
- moving rectangle
- textured rectangle
- multiple rectangles which are looked after by some system


## Rectangle owner system:
- store rectangles in some dynamic structure and issue ID's for accessing each
- linked list would be easy to add new ones, but slow to access - perhaps
  marginally better if they are strictly each accessed once per frame (?)
  -> 'draw' function adds ID to a 'to_render' array which is then matched
  against a single full pass of linked list
- dyanmically reallocating memory would make access easy but adding and
  removing elements slower and more unsafe
  -> would want to try and create everything in advance, although this does not
  seem super realistic
- opt for linked list for now
  -> we have a problem in that the order of drawing is significant, so perhaps
  it is better to just draw as we go along, but then doing a linked list is
  kind of a pain
  -> will it make such a difference?
