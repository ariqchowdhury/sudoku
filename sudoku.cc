#include <vector>
#include <iostream>
#include <string>

class Gameboard {
    int rows;
    int cols;

    std::vector<std::vector<int>> spaces;

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
                std::cout << "[" << c << "]";
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

};

int main() {
    Gameboard gb(3, 3);
    gb.set("123456789");
    gb.print();
}
