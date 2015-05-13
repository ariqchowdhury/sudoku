#include <vector>
#include <iostream>
#include <string>
#include <unordered_set>

class Gameboard {
    int rows;
    int cols;

    std::vector<std::vector<int>> spaces;

    // A valid row, col or subsquare will have no duplicates of the values
    // 1-9. 0s are allowed as they represent blank spaces
    
    bool valid_rows() const {
        for (auto r : spaces) {
            std::unordered_set<int> unique_vals;

            for (auto c : r) {
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

    // TODO(ARIQ): searching columns is inefficient with vector of vectors
    //             if eachcolumn had itsown vector so it was contiguous...
    //             
    bool valid_cols() const {
        for (auto i = 0; i < rows; ++i) {
            std::unordered_set<int> unique_vals;

            for (auto j = 0; j < cols; ++j) {
                auto c = spaces[j][i];
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
        return valid_rows() && valid_cols() && valid_sub_squares();
    }

};

int main() {
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

    if (gb.is_valid_state())
        gb.print();
}
