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
        std::cout << line_data[i] << " " << i-start << std::endl;
    }

    return slice;
}

bool sort_desc(const std::tuple<int, int>& a, const std::tuple<int, int>& b) {
    return (std::get<0>(a) > std::get<0>(b));
}

int main() {
    input_data data = load_input_data("input.txt");

    int lines = data.lines;
    int* line_sizes = data.line_sizes;
    int* line_data = data.line_data;

    int current_line_size;
    int cumulated_line_sizes;

    bool before_max;

    // Subtask 1

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

    int distance_left;
    int index;
    int height;
    int current_pos;

    int max_height;
    int max_valid_height;

    cumulated_line_sizes = 0;
    current_line_size = 0;
    for (int i = 0; i < lines; i++) {
        cumulated_line_sizes += current_line_size;
        current_line_size = line_sizes[i];

        if (i != 1) {
            continue;
        }

        max_height = 0;
        max_valid_height = 0;

        std::vector<std::tuple<int, int>>& slice = slice_with_indicies(cumulated_line_sizes, cumulated_line_sizes+current_line_size, line_data);
        std::sort(slice.begin(), slice.end(), sort_desc);
        for (int j = 0; j < current_line_size; j++) {
            std::cout << std::get<0>(slice[j]) << " " << std::get<1>(slice[j]) << std::endl;
        }

        for (int j = 0; j < current_line_size; j++) {
            height = std::get<0>(slice[j]);
            index = std::get<1>(slice[j]);
            distance_left = current_line_size - 1 - index;

            if (index >= height-1 && distance_left >= height-1) {
                if (height < max_valid_height) {
                    break;
                }
                //if (j != 2) {
                //    continue;
                //}

                current_pos = cumulated_line_sizes + index - height + 1;

                if (line_data[current_pos] != 1 || line_data[cumulated_line_sizes+index+height-1] != 1) {
                    continue; 
                }

                before_max = true;

                for (; current_pos < cumulated_line_sizes + index + height-1; current_pos++) {
                    if (before_max) {
                        if (line_data[current_pos+1] - line_data[current_pos] == 1) {
                            continue;
                        }

                        if (line_data[current_pos+1] - line_data[current_pos] == -1) {
                            before_max = false;
                            continue;
                        }

                        goto broke;
                    }
                    if (!before_max) {
                        if (line_data[current_pos+1] - line_data[current_pos] == -1) {
                            continue;
                        }               

                        goto broke;
                    }
                }

                if (height > max_valid_height) {
                    max_valid_height = height;
                } 

                if (2*height - 1 > max_height) {
                    max_height = 2*height - 1;
                }

                broke:
                continue;
            }
        }
        std::cout << "Case #" << i << ": " << max_height << std::endl;

    }
    return 0;
}
