#ifndef GRAPHICSDISPLAY_H
#define GRAPHICSDISPLAY_H
#include "window.h"
#include "info.h"
#include "observer.h"
#include "state.h"
class Cell;

class GraphicsDisplay: public Observer<Info, State> {
    Xwindow *xw;             // pointer to store the Xwindow object
    int gridSize;            // the size of the current grid
    int side_length;         // the side_length of the graphic square

  public:
    // Constructor of GraphicsDisplay
    GraphicsDisplay(int n, int side_length);
    // Destructor of GraphicsDisplay
    ~GraphicsDisplay();

    // notify(whoNotified) will notify the graphic to make a change in the
    //   position of whoNotified with its colour
    void notify(Subject<Info, State> &whoNotified) override;
};

#endif
