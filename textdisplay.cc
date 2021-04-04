#include "textdisplay.h"
#include "cell.h"

// the Constructor implementation of TextDisplay class using MIL
TextDisplay::TextDisplay(int n): gridSize{n} {
    /*
     * we iteratively modify theDisplay with char type so that it will
     * show the initialized grid
     */
    for(int i = 0; i < n; i++) {
        std::vector<char> theRow;
        for(int j = 0; j < n; j++) {
            theRow.emplace_back('-');
        }
        theDisplay.emplace_back(theRow);
    }
}

// see "testdisplay.h" for documentation
void TextDisplay::notify(Subject<Info, State> &whoNotified) {
    /*
     * we firstly find the current position and colour of whoNotified, then
     * we modify theDisplay board to put the colour on the position of that
     * cell
     */
    unsigned int r = whoNotified.getInfo().row;
    unsigned int c = whoNotified.getInfo().col;
    char colour;
        
    if (whoNotified.getInfo().colour == Colour::Black) {
        colour = 'B';
    } else if (whoNotified.getInfo().colour == Colour::White) {
        colour = 'W';
    } else {
        colour = '-';
    }

    theDisplay.at(r).at(c) = colour;
}

// see "testdisplay.h" for documentation
std::ostream &operator<<(std::ostream &out, const TextDisplay &td) {
    /*
     * we iteratively prints all character that is stored in theDisplay to show
     * the current game board
     */
    for(int i = 0; i < td.gridSize; i++) {
        for(int j = 0; j < td.gridSize; j++) {
            out << td.theDisplay.at(i).at(j);
        }
        out << std::endl;
    }
    return out;
}
