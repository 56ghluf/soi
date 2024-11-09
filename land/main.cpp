#include <iostream>
#include <fstream>
#include <string>
#include <tuple>
//#include <map>
//#include <bitset>
//#include <chrono> 

constexpr int N = 1000000;

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

void slice_with_indicies(int start, int end, std::vector<std::tuple<int, int>>& slice, int* line_data) {
    for (int i = start; i < end; i++) {
        slice.at(i-start) = std::make_tuple(line_data[i], i-start);
    }
}

void max_heights_with_indicies(int start, int end, std::vector<std::tuple<int, int>>& slice, int* line_data) {
    for (int i = start; i < end; i++) {
        slice.at(i-start) = std::make_tuple(
                line_data[i] > end-i || line_data[i] > i-start + 1 ?
                std::min(end-i, i-start+1) : line_data[i],
                i-start
                );
    }
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
    int cumulated_line_size;

    bool before_max;

    // Subtask 1

    //current_line_size = 0;
    //cumulated_line_size = 0;
    //for (int i = 0; i < lines; i++) {
    //    cumulated_line_size += current_line_size;
    //    current_line_size = line_sizes[i];
    //    //if (i != 10) {
    //    //    cumulated_line_size += line_sizes[i];
    //    //    continue;
    //    //}

    //    if (current_line_size % 2 != 1) {
    //        std::cout << "Case #" << i << ": NO" << std::endl;
    //        continue;
    //    }

    //    if (line_data[cumulated_line_size] != 1 || line_data[cumulated_line_size+current_line_size-1] != 1) {
    //        std::cout << "Case #" << i << ": NO" << std::endl;
    //        continue;
    //    }
    //    
    //    before_max = true;

    //    for (int j = 0; j < current_line_size-1; j++) {
    //        if (before_max) {
    //            if (line_data[cumulated_line_size+j+1] - line_data[cumulated_line_size+j] == 1) {
    //                continue;
    //            }

    //            if (line_data[cumulated_line_size+j+1] - line_data[cumulated_line_size+j] == -1) {
    //                before_max = false;
    //                continue;
    //            }

    //            std::cout << "Case #" << i << ": NO" << std::endl;
    //            goto broke;
    //        }

    //        if (!before_max) {
    //            if (line_data[cumulated_line_size+j+1] - line_data[cumulated_line_size+j] == -1) {
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

    //int distance_left;
    //int index;
    //int height;
    //int current_pos;

    //int max_height;
    //int max_valid_height;

    //cumulated_line_size = 0;
    //current_line_size = 0;

    //for (int i = 0; i < lines; i++) {
    //    cumulated_line_size += current_line_size;
    //    current_line_size = line_sizes[i];

    //    max_valid_height = 0;

    //    std::vector<std::tuple<int, int>> slice(current_line_size);
    //    slice_with_indicies(cumulated_line_size, cumulated_line_size+current_line_size, slice, line_data);
    //    
    //    std::sort(slice.begin(), slice.end(), sort_desc);

    //    for (int j = 0; j < current_line_size; j++) {
    //        height = std::get<0>(slice[j]);
    //        index = std::get<1>(slice[j]);
    //        distance_left = current_line_size - 1 - index;

    //        if (index >= height-1 && distance_left >= height-1) {
    //            if (height < max_valid_height) {
    //                break;
    //            }

    //            current_pos = cumulated_line_size + index - height + 1;

    //            if (line_data[current_pos] != 1 || line_data[cumulated_line_size+index+height-1] != 1) {
    //                continue; 
    //            }

    //            before_max = true;

    //            for (; current_pos < cumulated_line_size + index + height-1; current_pos++) {
    //                if (before_max) {
    //                    if (line_data[current_pos+1] - line_data[current_pos] == 1) {
    //                        continue;
    //                    }

    //                    if (line_data[current_pos+1] - line_data[current_pos] == -1) {
    //                        before_max = false;
    //                        continue;
    //                    }

    //                    goto broke;
    //                }
    //                if (!before_max) {
    //                    if (line_data[current_pos+1] - line_data[current_pos] == -1) {
    //                        continue;
    //                    }               

    //                    goto broke;
    //                }
    //            }

    //            if (height > max_valid_height) {
    //                max_valid_height = height;
    //            } 

    //            if (2*height - 1 > max_height) {
    //                max_height = 2*height - 1;
    //            }

    //            broke:
    //            continue;
    //        }
    //    }
    //    std::cout << "Case #" << i << ": " << max_height << std::endl;
    //}
    
    // Subtask 3
    //int max_len;

    //cumulated_line_size = 0;
    //current_line_size = 0;

    //for (int i = 0; i < lines; i++) {
    //    cumulated_line_size += current_line_size;
    //    current_line_size = line_sizes[i];

    //    //if (i != 13) {
    //    //    continue;
    //    //}
    //    
    //    if (current_line_size % 2 == 0) {
    //        max_len = 0;
    //        goto broke;
    //    }

    //    before_max = true;

    //    for (int j = 0; j < current_line_size; j++) {
    //        if (j > current_line_size / 2) {
    //            before_max = false;
    //        }

    //        if (before_max && line_data[cumulated_line_size+j] < j + 1) {
    //            max_len = 0;
    //            goto broke;
    //        }

    //        if (!before_max && line_data[cumulated_line_size+j] < current_line_size-j) {
    //            max_len = 0;
    //            goto broke;
    //        }
    //    }

    //    max_len = current_line_size;

    //    broke:

    //    std::cout << "Case #" << i << ": " << max_len << std::endl;
    //}
    
    // Subtask 4
    
    //cumulated_line_size = 0;
    //current_line_size = 0;

    //int current_pos;
    //int start_pos;
    //int max_height;
    //int index;

    //int max_len;

    //for (int i = 0; i < lines; i++) {
    //    cumulated_line_size += current_line_size;
    //    current_line_size = line_sizes[i];
    //    
    //    //if (i != 2) {
    //    //    continue;
    //    //}

    //    std::vector<std::tuple<int, int>> max_heights(current_line_size);
    //    max_heights_with_indicies(cumulated_line_size, cumulated_line_size+current_line_size, max_heights, line_data);

    //    std::sort(max_heights.begin(), max_heights.end(), sort_desc);

    //    max_len = 0;

    //    for (int j = 0; j < current_line_size; j++) {
    //        max_height = std::get<0>(max_heights[j]);

    //        if (max_height < max_len / 2 + 1)
    //            break;

    //        index = std::get<1>(max_heights[j]);

    //        start_pos = index - max_height + 1; 

    //        before_max = true;
    //        current_pos = start_pos;

    //        for (; current_pos < 2*index - start_pos; current_pos++) {
    //            if (before_max) {
    //                if (current_pos - start_pos + 1 > line_data[cumulated_line_size+current_pos]) {
    //                    start_pos = current_pos - line_data[cumulated_line_size+current_pos] + 1; 
    //                    //goto broke;
    //                }

    //                if (current_pos == index) {
    //                    before_max = false;
    //                }
    //            } else if (2*index - start_pos - current_pos + 1 > line_data[cumulated_line_size+current_pos]) {
    //                start_pos = 2*index - current_pos - line_data[cumulated_line_size+current_pos] + 1;
    //                //goto broke;
    //            }
    //        }

    //        if (2*(index-start_pos) + 1 > max_len)
    //            max_len = 2*(index-start_pos) + 1;
    //    }
    //    std::cout << "Case #" << i << ": " << max_len << std::endl;
    //}
    
    int current_max_height;
    int smallest_height;
    int index;
    int max_len;

    cumulated_line_size = 0;
    current_line_size = 0;

    for (int i = 0; i < lines; i++) {
        cumulated_line_size += current_line_size;
        current_line_size = line_sizes[i];
        
        std::vector<std::tuple<int, int>> max_heights(current_line_size);
        max_heights_with_indicies(cumulated_line_size, cumulated_line_size+current_line_size, max_heights, line_data);

        std::sort(max_heights.begin(), max_heights.end(), sort_desc);

        max_len = 0;

        for (int j = 0; j < current_line_size; j++) {
            current_max_height = std::get<0>(max_heights[j]);
            if (current_max_height < max_len / 2 + 1)
                break;
            index = std::get<1>(max_heights[i]);

            for (int offset = 1; offset < current_max_height; offset++) {
                smallest_height = std::min(line_data[cumulated_line_size+index-offset], line_data[cumulated_line_size+index+offset]);

                if (smallest_height < current_max_height - offset) {
                    current_max_height -= current_max_height - offset - smallest_height;
                }
            }

            if (current_max_height*2 - 1 > max_len) {
                max_len = current_max_height*2 - 1;
            }
        }

        std::cout << "Case #" << i << ": " << max_len << std::endl;
    }

    return 0;
}
