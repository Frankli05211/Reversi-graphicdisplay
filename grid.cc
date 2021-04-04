#include "grid.h"
#include "subject.h"
#include "textdisplay.h"

// ================HELPER FUNCTION==============================================

// in_range(r, c, size) returns whether the given position(r, c) is in the range
//   of the grid board with side length size
bool in_range(int r, int c, int size) {
    if(r < 0 || r >= size) {
        return false;
    } else if (c < 0 || c >= size) {
        return false;
    } else {
        return true;
    }
}

// ================CORE FUNCTION================================================

// Destructor of Grid class
Grid::~Grid() {
    delete td;
}

// see "grid.h" for documentation
void Grid::setObserver(Observer<Info, State> *ob) {
    /*
     * we set the ob as the given new observer
     */
    this->ob = ob;
}

// see "grid.h" for documentation
bool Grid::isFull() const {
    /*
     * we check directly whether black and white cells fill all pieces in grid
     */
    return num_piece == side_length * side_length;
}

Colour Grid::whoWon() const {
    /*
     * we iteratively check all piece in theGrid. If the number of two colours
     * is not the same, function will return the colour with the larger number,
     * otherwise, function returns nocolour
     */
    unsigned int black = 0;
    unsigned int white = 0;
    for(int i = 0; i < side_length; i++) {
        for(int j = 0; j < side_length; j++) {
            Colour c = theGrid.at(i).at(j).getInfo().colour;
            if (c == Colour::Black) {
                black++;
            } else {
                white++;
            }
        }
    }

    if (black > white) {                  // the white colour is the winner
        return Colour::Black;
    } else if (white > black) {           // the black colour is the winner
        return Colour::White;
    } else {                              // neither balck nor white is the winner
        return Colour::NoColour;
    }
}

// see "grid.h" for documentation
void Grid::init(size_t n) {
    /*
     * firstly check whether the grid is empty, if it is not empty, we clear
     * the previous grid
     */
    if(!theGrid.empty()) {
        theGrid.clear();
        delete td;
    }

    side_length = n;                          // initialize the side length of grid
    num_piece = 0;                            // initialize the number of piece in grid

    /*
     * we iteratively emplace any cell to the new n*n grid
     */
    for(unsigned int i = 0; i < n; i++) {
        std::vector<Cell> theRow; 
        for(unsigned int j = 0; j < n; j++) {
            theRow.emplace_back(Cell{i, j});
        }
        theGrid.emplace_back(theRow);
    }

    /*
     * we initialize the testdisplay type by using the size n
     */
    td = new TextDisplay(n);

    /*
     * we then attach every cells with all of its neighbours as its observes
     * together with the textdisplay as its observe as well
     */
    for(int i = 0; i < side_length; i++) {
        for(int j = 0; j < side_length; j++) {
            // the neighbour at north west direction can be its observe
            if (in_range(i-1, j-1, n)) {
                theGrid.at(i).at(j).attach(&(theGrid.at(i-1).at(j-1)));
            }

            // the neighbour at north direction can be its observe
            if (in_range(i-1, j, n)) {
                theGrid.at(i).at(j).attach(&(theGrid.at(i-1).at(j)));
            }

            // the neighbour at north east direction can be its observe
            if (in_range(i-1, j+1, n)) {
                theGrid.at(i).at(j).attach(&(theGrid.at(i-1).at(j+1)));
            }

            // the neighbour at west direction can be its observe
            if (in_range(i, j-1, n)) {
                theGrid.at(i).at(j).attach(&(theGrid.at(i).at(j-1)));
            }

            // the neighbour at east direction can be its observe
            if (in_range(i, j+1, n)) {
                theGrid.at(i).at(j).attach(&(theGrid.at(i).at(j+1)));
            }

            // the neighbour at south west direction can be its observe
            if (in_range(i+1, j-1, n)) {
                theGrid.at(i).at(j).attach(&(theGrid.at(i+1).at(j-1)));
            }
            
            // the neighbour at south direction can be its observe
            if (in_range(i+1, j, n)) {
                theGrid.at(i).at(j).attach(&(theGrid.at(i+1).at(j)));
            }

            // the neighbour at south east direction can be its observe
            if (in_range(i+1, j+1, n)) {
                theGrid.at(i).at(j).attach(&(theGrid.at(i+1).at(j+1)));
            }

            // attach both testdisplay and graphicsdisplay as its observe as well
            theGrid.at(i).at(j).attach(td);
            theGrid.at(i).at(j).attach(ob);
        }
    }
}

// see "grid.h" for documentation
void Grid::setPiece(size_t r, size_t c, Colour colour) {
    if(!in_range(r, c, side_length)) {
        // throw the exception if the given position is out of range
        throw InvalidMove{};
    } else if (td == nullptr) {
        // throw the exception if the current testdisplay is not defined
        throw InvalidMove{};
    } else if (ob == nullptr) {
        // throw the exception if the current graphicsdisplay is not defined
        throw InvalidMove{};
    } else if (theGrid.at(r).at(c).getInfo().colour != Colour::NoColour) {
        // throw the exception if the given position already has a piece
        throw InvalidMove{};
    } else {
        /*
         * otherwise, the given position is valid to be placed a piece, we then pleace
         * the set the piece on the position, and then notify all of its observers
         */
        num_piece++;
        theGrid.at(r).at(c).setPiece(colour);
        theGrid.at(r).at(c).notifyObservers();
    }
}

// see "grid.h" for documentation
void Grid::toggle(size_t r, size_t c) {
    theGrid.at(r).at(c).toggle();                  // toggle the cell in the given position
}

// see "grid.h" for documentation
std::ostream& operator<<(std::ostream &out, const Grid &g) {
    return out << *(g.td);                         // print the textdisplay to the output stream
}
