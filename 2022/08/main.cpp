#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <chrono>

using namespace std;

const int SIZE_X = 99;
const int SIZE_Y = 99;

vector<vector<int>> read_data_from_file(string filename) {
    ifstream read_file(filename);

    string row = "";
    vector<vector<int>> data(SIZE_Y, vector<int> (SIZE_X, 0));
    //vector<vector<int>> data = {};

    // no file?
    if (not read_file) {
        cout << "File not found: " << filename << endl;
        return data;
    }

    int y = 0;
    while (getline(read_file, row)) {
        int x = 0;
        for (auto &ch : row) {
            data[y][x] = static_cast<int>(ch-48);
            x++;
        }
        y++;
    }
//    while (getline(read_file, row)) {
//        data.push_back({});
//        for (auto &ch : row) {
//            data.back().push_back(static_cast<int>(ch-48));
//        }
//    }

    read_file.close();

    return data;
}

vector<int> slice_1d(vector<vector<int>> &data, int y0, int y1,
                                                int x0, int x1) {
    vector<int> result = {};
    // left
    if ((y0 == y1) and (x0 == 0)) {
        for (int i = 0; i < x1; i++) {
            result.push_back(data.at(y0).at(i));
        }
    }
    // right
    else if ((y0 == y1) and (x0 != x1)) {
        for (int i = x0+1; i < x1; i++) {
            result.push_back(data.at(y0).at(i));
        }
    }
    // up
    else if ((y0 == 0) and (x0 == x1)) {
        for (int i = 0; i < y1; i++) {
            result.push_back(data.at(i).at(x0));
        }
    }
    // down
    else {
        for (int i = y0+1; i < y1; i++) {
            result.push_back(data.at(i).at(x0));
        }
    }
    return result;
}

int calc_scenic(int &digit, vector<int> &arr) {
    int score = 0;
    for (auto& num : arr) {
        if (num >= digit) {
            score++;
            break;
        }
        if (num < digit) {
            score++;
        }
    }

    return score;
}

void silver(vector<vector<int>> &data) {
    int x = 0;
    int y = 0;
    int total_x = data[0].size();
    int total_y = data.size();
    int total_visible = 0;
    int max_scenic_score = 0;

    for (auto& row : data) {
        for(auto& digit : row) {
            if ((y > 0 and x > 0) and (y < total_y-1 and x < total_x-1))
            {
                // slices
                vector<int> lf = slice_1d(data, y, y, 0, x);
                vector<int> rg = slice_1d(data, y, y, x, total_x);
                vector<int> up = slice_1d(data, 0, y, x, x);
                vector<int> dn = slice_1d(data, y, total_y, x, x);

                // silver
                vector<int> max_trees_around = {};
                max_trees_around.push_back(*max_element(lf.begin(), lf.end()));
                max_trees_around.push_back(*max_element(rg.begin(), rg.end()));
                max_trees_around.push_back(*max_element(up.begin(), up.end()));
                max_trees_around.push_back(*max_element(dn.begin(), dn.end()));

                for(auto &tree : max_trees_around) {
                    if (digit > tree) {
                        total_visible++;
                        break;
                    }
                }

                // gold
                std::reverse(lf.begin(), lf.end());
                std::reverse(up.begin(), up.end());
                int score = calc_scenic(digit, lf);
                score *= calc_scenic(digit, rg);
                score *= calc_scenic(digit, up);
                score *= calc_scenic(digit, dn);

                if (score > max_scenic_score) {
                    max_scenic_score = score;
                }
            }
            x++;
        }
        x = 0;
        y++;
    }

    // 1854 for silver, 527340 for gold
    cout << "silver: " << total_visible + (total_x-1)*2 + (total_y-1)*2 << endl;
    cout << "gold: " << max_scenic_score << endl;
}

int main()
{
    auto start = std::chrono::high_resolution_clock::now();
    // cout << "hello!" << endl;
    vector<vector<int>> data = read_data_from_file("puzzle.input");
    silver(data);
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    std::printf("%.3f", (double)duration.count()/1000);
    std::cout << " ms" << endl;

    return 0;
}
