#include <vector>
#include <iostream>
#include <string>
#include <unordered_set>
#include <cassert>

class Gameboard {
    int rows;
    int cols;

    //sub square dimensions
    static const int ss_dim = 3;

    enum class Dimension {
        Row,
        Column
    };

    std::vector<std::vector<int>> spaces;

    // A valid row, col or subsquare will have no duplicates of the values
    // 1-9. 0s are allowed as they represent blank spaces
    
    // TODO(ARIQ): searching columns is inefficient with vector of vectors
    //             if each column had its own vector so it was contiguous...
    //             
    bool valid_dimension(Dimension d) const {
        for (auto i = 0; i < rows; ++i) {
            std::unordered_set<int> unique_vals;

            for (auto j = 0; j < cols; ++j) {
                auto c = spaces[0][0];

                if (Dimension::Row == d) {
                    c = spaces[i][j];
                } else if (Dimension::Column == d) {
                    c = spaces[j][i];
                } else {
                    assert(false && "game board is only 2d");
                }

                if (c == 0) {
                    continue;
                }

                if (unique_vals.find(c) != unique_vals.end()) {
                   return false;
                } else {
                    unique_vals.insert(c);
                }
                
            }
        }

        return true;
    }

    bool valid_sub_squares() const {
        for (auto i = 0; i < rows; i += ss_dim) {
            for (auto j = 0; j < cols; j += ss_dim) {
                if (!valid_square(i, j)) {
                    return false;
                }
            }
        }
        return true;
    }

    // Given a point (m, n) in the board, check that the 3x3 subsquare 
    // containing the point is a valid sudoku subsquare
    bool valid_square(int m, int n) const {
        // int div by ss_dim gives which nth subsquare the coordinate is in
        // then mult by ss_dim to get the index of the (0,0) point of the sub
        // square
        m = (m / ss_dim) * ss_dim;
        n = (n / ss_dim) * ss_dim;

        std::unordered_set<int> unique_vals;

        for (auto i = 0; i < ss_dim; ++i) {
            for (auto j = 0; j < ss_dim; ++j) {
                auto c = spaces[i+m][j+n];

                if (c == 0) {
                    continue;
                }

                if (unique_vals.find(c) != unique_vals.end()) {
                   return false;
                } else {
                    unique_vals.insert(c);
                }

            }
        }

        return true;
    }

    // check all rows to see if there are any black spaces
    bool board_filled() const {
        for (auto r : spaces) {
            for (auto c : r) {
                if (c == 0) {
                    return false;
                }
            }
        }

        return true;
    }

public:
    Gameboard(int r, int c) :
        rows(r), cols(c) 
    {
        for (auto i = 0; i < rows; ++i) {
            std::vector<int> col_vec;
            for (auto j = 0; j < cols; ++j) {
                col_vec.push_back(0);
            }
            spaces.push_back(col_vec);
        }
    }

    ~Gameboard() {}

    void print() const {
        for (auto r : spaces) {
            for (auto c : r) {
                if (c == 0) {
                    std::cout << "[" << " " << "]";
                } else {
                    std::cout << "[" << c << "]";
                }
            }
            std::cout << std::endl;
        }
    }

    // Format for initializer string will represent blank squares with spaces 
    // and numbers side by side for adjacent spaces. Rows will also be laid out
    // in one line. Chars will be used from the string until spaces are full.
    // extra chars will be ignored, not enough chars will pad with blnks 
    void set(std::string init) {
        auto next_space = init.begin();

        for (auto &r : spaces) {
            for (auto &c : r) {
                auto space = (*next_space) - '0';
                if (space < 1 || space > 9) {
                    c = 0; 
                } else {
                    c = space;
                }
                ++next_space;
            } 
        }
    }

    // Check if the state of thespaces in the gameboard consititues
    // a valid sudoku state 
    bool is_valid_state() const {
        return valid_dimension(Dimension::Row) && 
               valid_dimension(Dimension::Column) && 
               valid_sub_squares();
    }

    // This function attempts to solve the sudoku puzzle from the given 
    // board state.
    // Use recursive backtracking to find a solution
    // This function will modify the board state with the solution
    bool solve() {
        if (!is_valid_state()) {
            return false;
        }

        if (board_filled()) {
            return true;
        }

        for (auto i = 0; i < rows; ++i) {
            for (auto j = 0; j < cols; ++j) {
                if (spaces[i][j] == 0) {
                    
                    // for the possible sudoku values
                    for (auto p : {1, 2, 3, 4, 5, 6, 7, 8, 9}) {
                        spaces[i][j] = p;

                        if (solve()) {
                            return true;
                        }

                        // attempted p lead to an invalid board so try another
                        spaces[i][j] = 0;
                    }

                    // All values lead to invalid boards so backtrack
                    return false;
                }
            }
        }

        // Went through all combinations and found no solution
        return false;
    }

};

bool tests();

int main() {
    assert(tests());

    Gameboard gb(9, 9);
    std::string board = 
        "000006003"
        "600040002"
        "010570400"
        "009007801"
        "000000000"
        "708600300"
        "004068050"
        "900030004"
        "800200000";

    gb.set(board);

    if (gb.is_valid_state()) {
        std::cout << "Initial Board:" << std::endl;
        gb.print();
    }

    if (gb.solve()) {
        std::cout << "Solution:" << std::endl;
        gb.print();
    } else {
        std::cout << "No Solution Found" << std::endl;
    }
}






bool tests() {
    Gameboard gb(9, 9);
    std::string board = 
        "000006003"
        "600040002"
        "010570400"
        "009007801"
        "000000000"
        "708600300"
        "004068050"
        "900030004"
        "800200040";

    gb.set(board);

    assert(!gb.is_valid_state() && "subsquares have duplicates so this should be invalid");

    Gameboard small(3,3);
    small.set("123456789");
    assert(small.is_valid_state() && "3x3 with all unique should be valid");
    small.set("123456389");
    assert(!small.is_valid_state() && "3x3 with duplicate should be invalid");

    return true;
}
