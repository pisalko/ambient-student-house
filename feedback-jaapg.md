# Arduino

## Code Structure

* code structure good and readable.

## Code Quality (naming, comments)

* comments very sparse. minimum add a header block with file, author, date, and description (i.e. responsibilities of this code)
* Code formatting could be more consistent (Ctrl-T anyone?)
* never do `while(!s);` instead do: `while (!s) {} `
* parsing code for messages good (ESp server connect)

* good states and switch case statements in main loop of mood lighting.

* curtains code has no error detection. Readability cold be improved as well.

# Windows


## Code Structure

* All code in a single class. Why not create a Room class etc.
* Should separate parsing data from taking actions. lines 119-170. put these actions in separate methods
* good error handling in room registration.


## code Quality (naming, comments)

* Not enough comments.

# Protocol

* Not a single protocol for all devices.

