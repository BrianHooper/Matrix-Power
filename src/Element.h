//      Brian Hooper
//      CS530 High Performance Computing
//      Lab 1
//      4/13/18

#ifndef ELEMENT_H
#define ELEMENT_H

class Element {
public:
    /**
     * Constructor
     * @param r row position
     * @param c column position
     * @param v element value
     */
    Element(int r, int c, double v) {
        row = r;
        col = c;
        value = v;
    }

    /**
     * Checks that two elements are equal to each other
     * @param other address of other element
     * @return true if this(row, col) == other(row, col)
     */
    bool operator==(const Element &other) const {
        return row == other.row && col == other.col;
    }

    /**
     * Increments this elements value with the other elements value
     * @param other address of other element
     */
    void operator+=(const Element &other) {
        value += other.value;
    }

    /**
     * Checks that this < other
     * @param other address of other element
     * @return true of this element is less than the other
     */
    bool operator<(const Element &other) const {
        if (row == other.row) {
            return col < other.col;
        } else {
            return row < other.row;
        }
    }

    int row;
    int col;
    double value;
};

#endif //ELEMENT_H