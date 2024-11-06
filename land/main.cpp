#include <iostream>
#include <fstream>
#include <string>
#include <tuple>
//#include <map>
//#include <bitset>
//#include <chrono> 

constexpr int N = 200000;

struct input_data {
    int lines;
    int* line_sizes;
    int* line_data;
};

input_data load_input_data(std::string filename) {
    std::ifstream input(filename);
    std::string content;

    int lines;
    static int* line_sizes;
    int* line_data;

    int line_data_pos = 0;

    if (input.is_open()) {
        input >> content;
        lines = std::stoi(content);
        
        line_sizes = new int[lines];
        line_data = new int[lines*N];

        // Fill the tramps
        int n;
        int m;

        for (int i = 0; i < lines; i++) {
            input >> content;
            n = std::stoi(content);

            line_sizes[i] = n;

            for (int j = 0; j < n; j++) {
                input >> content;
                m = std::stoi(content);

                line_data[line_data_pos] = m;
                line_data_pos += 1;
            }
        }
    }

    static int* reduced_line_data = new int[line_data_pos];

    for (int i = 0; i < line_data_pos; i++) {
        reduced_line_data[i] = line_data[i];
    } 

    return input_data{ lines, line_sizes, reduced_line_data };
}

std::vector<std::tuple<int, int>>& slice_with_indicies(int start, int end, int* line_data) {
    static std::vector<std::tuple<int, int>> slice(end-start);

    for (int i = start; i < end; i++) {
        slice[i] = std::make_tuple(line_data[i], i-start);
    }

    return slice;
}

bool sort_desc(const std::tuple<int, int>& a, const std::tuple<int, int>& b) {
    return (std::get<0>(a) > std::get<0>(b));
}

int main() {
    input_data data = load_input_data("input1.txt");

    int lines = data.lines;
    int* line_sizes = data.line_sizes;
    int* line_data = data.line_data;

    int current_line_size;
    int cumulated_line_sizes;

    // Subtask 1
   
    //bool before_max;

    //current_line_size = 0;
    //cumulated_line_sizes = 0;
    //for (int i = 0; i < lines; i++) {
    //    cumulated_line_sizes += current_line_size;
    //    current_line_size = line_sizes[i];
    //    //if (i != 10) {
    //    //    cumulated_line_sizes += line_sizes[i];
    //    //    continue;
    //    //}

    //    if (current_line_size % 2 != 1) {
    //        std::cout << "Case #" << i << ": NO" << std::endl;
    //        continue;
    //    }

    //    if (line_data[cumulated_line_sizes] != 1 || line_data[cumulated_line_sizes+current_line_size-1] != 1) {
    //        std::cout << "Case #" << i << ": NO" << std::endl;
    //        continue;
    //    }
    //    
    //    before_max = true;

    //    for (int j = 0; j < current_line_size-1; j++) {
    //        if (before_max) {
    //            if (line_data[cumulated_line_sizes+j+1] - line_data[cumulated_line_sizes+j] == 1) {
    //                continue;
    //            }

    //            if (line_data[cumulated_line_sizes+j+1] - line_data[cumulated_line_sizes+j] == -1) {
    //                before_max = false;
    //                continue;
    //            }

    //            std::cout << "Case #" << i << ": NO" << std::endl;
    //            goto broke;
    //        }

    //        if (!before_max) {
    //            if (line_data[cumulated_line_sizes+j+1] - line_data[cumulated_line_sizes+j] == -1) {
    //                continue;
    //            }

    //            std::cout << "Case #" << i << ": NO" << std::endl;
    //            goto broke;
    //        }       
    //    }

    //    std::cout << "Case #" << i << ": YES" << std::endl;

    //    broke:
    //}

    // Subsection 2

    std::vector<std::tuple<int, int>>& slice;

    cumulated_line_sizes = 0;
    current_line_size = 0;
    for (int i = 0; i < lines; i++) {
        cumulated_line_sizes += current_line_size;
        current_line_size = line_sizes[i];

        slice = slice_with_indicies(cumulated_line_sizes, cumulated_line_sizes+current_line_size, line_data);
    }
    return 0;
}
