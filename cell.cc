#include "cell.h"

// ================HELPER FUNCTION==============================================

// get_direction(a, b) returns the direction of b corresponding to a
Direction get_direction(Subject<Info, State> &a, Subject<Info, State> &b) {
    /*
     * we get the position of both a and b firstly
     */
    unsigned int ar = a.getInfo().row;
    unsigned int ac = a.getInfo().col;
    unsigned int br = b.getInfo().row;
    unsigned int bc = b.getInfo().col;

    if (ar < br && ac < bc) {             // b is located at south east of a
        return Direction::SE;
    } else if (ar < br && ac == bc) {     // b is located at south of a
        return Direction::S;
    } else if (ar < br && ac > bc) {      // b is located at south west of a
        return Direction::SW;
    } else if (ar == br && ac < bc) {     // b is located at east of a
        return Direction::E;
    } else if (ar == br && ac > bc) {     // b is located at west of a
        return Direction::W;
    } else if (ar > br && ac < bc) {      // b is located at north east of a
        return Direction::NE;
    } else if (ar > br && ac == bc) {     // b is located at north of a
        return Direction::N;
    } else {                              // b is located at north west of a
        return Direction::NW;
    }
}

// ================CORE FUNCTION================================================

// Constructor implementation of Cell class using MIL
Cell::Cell(size_t r, size_t c): r{r}, c{c} {}

// see "cell.h" for documentation
void Cell::setPiece(Colour colour) {
    /*
     * initialize the colour with the given colour, and initilialize the state as
     * NewPiece, NoColour and N
     */
    this->colour = colour;
    State newState{ StateType::NewPiece, Colour::NoColour, Direction::N };
    this->setState(newState);
}

// see "cell.h" for documentation
void Cell::toggle() {
    if (colour == Colour::Black) {
        colour = Colour::White;
    } else if (colour == Colour::White) {
        colour = Colour::Black;
    }
}

// see "cell.h" for documentation
void Cell::notify(Subject<Info, State> &whoFrom) {
    if (colour == Colour::NoColour) {
        // if the current observer contains no piece, function returns nothing
        return;
    } else {
        /*
         * otherwise, we check the state of whoFrom in the following cases,
         * case 1: if the state of the whoFrom is NewPiece, then we directly
         *         change the state of the current cell
         * case 2: if the state of the whoFrom is Relay, then we just change
         *         the state of the cell that is in the straight line of the
         *         whoFrom and new piece
         * case 3: if the state of the whoFrom is Reply, then we just toggle
         *         the piece that is in the straight line of the whoFrom and
         *         the new piece
         */
        if (whoFrom.getState().type == StateType::NewPiece) {
            if (colour == whoFrom.getInfo().colour) {
                // if we find that the observer of the new piece has the same
                //   colour with the new piece, function does nothing
                return;
            } else {
                // if we find that the observer of the new pieve has different
                //   colour with the new piece, function changes the state of
                //   the current cell
                Direction newDir = get_direction(*this, whoFrom);
                State newState{ StateType::Relay, whoFrom.getInfo().colour, newDir };
                this->setState(newState);
                this->notifyObservers();
            }
        } else if (whoFrom.getState().type == StateType::Relay) {
            Direction myDir = get_direction(*this, whoFrom);
            if (myDir != whoFrom.getState().direction) {
                // if we find that the observer is not in the straight line between
                //   whoFrom and new piece, function does nothing
                return;
            } else {
                // otherwise, we checks whether the current cell has the same colour
                //   with the new piece. If they have the same colour, we then change
                //   the state of the current cell as reply, otherwise we change it as
                //   relay
                if (colour == whoFrom.getState().colour) {
                    Direction newDir = get_direction(whoFrom, *this);
                    State newState{ StateType::Reply, colour, newDir };
                    this->setState(newState);
                    this->notifyObservers();
                } else {
                    Direction newDir = whoFrom.getState().direction;
                    State newState{ StateType::Relay, whoFrom.getState().colour, newDir };
                    this->setState(newState);
                    this->notifyObservers();
                }
            }
        } else {
            Direction myDir = get_direction(*this, whoFrom);
            if (myDir != whoFrom.getState().direction) {
                // if we find that the observer is not in the straight line between
                //   whoFrom and new pieve, function does nothing
                return;
            } else {
                // we then check if we back to the new piece, we stops the function and returns
                //   nothing, otherwise we toggle the current cell, and change the state of the
                //   current cell to reply
                if (this->getState().type == StateType::NewPiece) {
                    return;
                } else {
                    if (colour != this->getState().colour) {
                        this->toggle();
                    }
                    Direction newDir = whoFrom.getState().direction;
                    State newState{ StateType::Reply, whoFrom.getInfo().colour, newDir };
                    this->setState(newState);
                    this->notifyObservers();
                }
            }
        }
    }
}

// see "cell.h" for documentation
Info Cell::getInfo() const {
    Info myInfo{r, c, colour};
    return myInfo;
}
