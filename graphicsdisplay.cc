#include "graphicsdisplay.h"
#include "cell.h"

// Constructor implementation of class GraphicsDisplay
GraphicsDisplay::GraphicsDisplay(int n, int side_length)
    : gridSize{n},
      side_length{side_length}
{
    /*
     * we firstlty create a heap allocated Xwindow, and then simply fill the
     * grid with the blue rectangle
     */
    xw = new Xwindow{side_length, side_length};
    int colour = Xwindow::Blue;
    xw->fillRectangle(0, 0, side_length, side_length, colour);
}

GraphicsDisplay::~GraphicsDisplay() {
    /*
     * delete the stored Xwindow by calling its destructor
     */
    delete xw;
}

void GraphicsDisplay::notify(Subject<Info, State> &whoNotified) {
    /*
     * we firstly find the current position and colour of whoNotified, then
     * we find the side length for each piece. After that we find the
     * corresponding position in Xwindow, and we change the colour of
     * the rectangle in the given position
     */
    unsigned int r = whoNotified.getInfo().row;
    unsigned int c = whoNotified.getInfo().col;
    Colour colour = whoNotified.getInfo().colour;
    int length = side_length / gridSize;
    r *= length;
    c *= length;
    
    if (colour == Colour::Black) {
        int square_c = Xwindow::Black;             // get the colour of the square
        xw->fillRectangle(c, r, length, length, square_c); // draw the square
    } else if (colour == Colour::White) {
        int square_c = Xwindow::White;             // get the colour of the square
        xw->fillRectangle(c, r, length, length, square_c); // draw the square
    } else {
        int square_c = Xwindow::Blue;              // get the colour of the square
        xw->fillRectangle(c, r, length, length, square_c); // draw the square
    }
}
