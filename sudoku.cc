#include <vector>
#include <iostream>

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

    ~Gameboard() {
    }

};

int main() {
    Gameboard gb(3, 3);
}
