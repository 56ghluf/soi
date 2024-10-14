#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <bitset>
#include <chrono>

constexpr unsigned int N = 200000;

struct tramp_data {
    unsigned int lines;
    unsigned int* trampn;
    unsigned int* jumps;
};

tramp_data load_tramp_data(std::string filename) {
    std::ifstream input(filename);
    std::string content;

    unsigned int lines;
    static unsigned int* trampn;
    unsigned int* jumps;

    unsigned int jump_pos = 0;

    if (input.is_open()) {
        input >> content;
        lines = std::stoi(content);
        
        trampn = new unsigned int[lines];
        jumps = new unsigned int[lines*N];

        // Fill the tramps
        unsigned int n;
        unsigned int m;

        for (int i = 0; i < lines; i++) {
            input >> content;
            n = std::stoi(content);

            trampn[i] = n;

            for (int j = 0; j < n; j++) {
                input >> content;
                m = std::stoi(content);

                jumps[jump_pos] = m;
                jump_pos += 1;
            }
        }
    }

    static unsigned int* reduced_jumps = new unsigned int[jump_pos];

    for (int i = 0; i < jump_pos; i++) {
        reduced_jumps[i] = jumps[i];
    } 

    return tramp_data{ lines, trampn, reduced_jumps };
}

int main()
{
    tramp_data data = load_tramp_data("input5.txt");
    unsigned int lines = data.lines;
    unsigned int* trampn = data.trampn;
    unsigned int* jumps = data.jumps;

    // Print the data
    //for (int i = 0; i < lines; i++) {
    //    std::cout << "Tramps: " << trampn[i] << std::endl;
    //    for (int j = 0; j < trampn[i]; j++){
    //        std::cout << jumps[N*i+j] << std::endl;
    //    }
    //}

    // Subsection 1
    //for (int i = 0; i < lines; i++) {
    //    int j = 0;
    //    int tramps_visited = 1;

    //    while (j < trampn[i]-1) {
    //        j += jumps[i*N+j];
    //        tramps_visited += 1;
    //    }
    //    std::cout << "Case #" << i << ": " << tramps_visited << std::endl;
    //}

    // Susbsection 2
    // Optimisation 1: keep track of already visisted positions
    // Optimisation 2: if distance left from start position is less than
    // the max of tramps visited, return the max

    //for (int i = 0; i < lines; i++) {
    //    if (trampn[i] < 3) {
    //        std::cout << "Case #" << i << ": " << trampn[i] << std::endl;
    //        continue;
    //    }

    //    std::unordered_set<int> vposes;
    //    int max = 2;
    //    
    //    int current_pos;
    //    int visited;

    //    for (int j = 0; j < trampn[i]-2; j++) {
    //        current_pos = j;
    //        visited = 1;
    //        
    //        if (trampn[i]-j < max) {
    //            break;
    //        }

    //        while (current_pos < trampn[i]-1) {
    //            if ((vposes.find(current_pos) != vposes.end()) && visited == 1) {
    //                break;
    //            } 

    //            visited += 1;
    //            vposes.insert(current_pos);
    //            current_pos += jumps[N*i+current_pos];
    //        }

    //        if (visited > max) {
    //            max = visited;
    //        }
    //    }

    //    std::cout << "Case #" << i << ": " << max << std::endl;
    //}

    // Subsection 3
    // Optimisation 1: keep track of every already visited position
    // and the nubmer of visits in that positions.
    int* vposes;
    
    int total_jumps = 0;

    for (int i = 0; i < lines; i++) {
        total_jumps += trampn[i];
    }

    vposes = new int[total_jumps*lines];

    for (int i = 0; i < total_jumps*lines; i++) {
        vposes[i] = 0;
    }
    
    unsigned int maxs[lines];

    unsigned int max;
    
    unsigned int current_pos;
    unsigned int visited;

    unsigned int current_trampn = 0;

    unsigned int cumulated_trampn = 0; 

    for (int i = 0; i < lines; i++) {
        cumulated_trampn += current_trampn;
        current_trampn = trampn[i];
        //if (i != 20) {
        //    continue;
        //}
        if (trampn[i] < 3) {
            std::cout << "Case #" << i << ": " << trampn[i] << std::endl;
            continue;
        }

        max = 2;

        for (int j = 0; j < trampn[i]-2; j++) {
            current_pos = j;
            visited = 1;

            if (trampn[i]-j < max) {
                break;
            }

            while (current_pos < trampn[i]-1) {
                if (vposes[cumulated_trampn + current_pos] == 0) {
                    vposes[cumulated_trampn + current_pos] = visited;
                } else {
                    if (visited > vposes[cumulated_trampn + current_pos]) {
                        vposes[cumulated_trampn + current_pos] = visited;
                    } else {
                        break;
                    }
                }

                visited += 1;

                current_pos += jumps[cumulated_trampn+current_pos];
            }

            if (visited > max) {
                max = visited;
            }
        }

        maxs[i] = max;
        //std::cout << "Case #" << i << ": " << max << std::endl;
    }

    // Subsection 4
    // Uses the vposes from section 3
   
    // Subsection 5

    // Figure out the max jump for each line
    
    // Fill the max jumps
    int max_jumps[lines];

    int start_jump;
    int start_pos;

    current_trampn = 0;
    cumulated_trampn = 0;

    int max_jump;
    for (int i = 0; i < lines; i++) {
        cumulated_trampn += current_trampn;
        current_trampn = trampn[i];

        max_jump = 1;
        start_pos = 0;
        start_jump = jumps[current_trampn+start_pos];

        for (int j = 1; j < current_trampn-2; j++) {
            if (j - start_pos > start_jump) {
                start_pos = j;
                start_jump = jumps[current_trampn+j];
            }

            if (jumps[current_trampn+start_pos] > jumps[current_trampn+j]) {
                if (j - start_pos > max_jump) {
                    max_jump = j - start_pos;
                    start_pos = j;
                }
            }
        }
        max_jumps[i] = max_jump;
    }

    unsigned int max_vpos;
    std::bitset<N> path;

    unsigned int current_vpos;

    current_trampn = 0;
    cumulated_trampn = 0;

    for (int i = 0; i < lines; i++) {
        cumulated_trampn += current_trampn;
        current_trampn = trampn[i];

        //if (i != 20) {
        //    continue;
        //}
        if (current_trampn < 3) {
            std::cout << "Case #" << i << ": " << current_trampn << std::endl;
            continue;
        }

        max = maxs[i];
        max_vpos = 0;

        std::cout << max_jumps[i] << std::endl;
        for (int j = 0; j < current_trampn-2; j++) {
            current_vpos = vposes[cumulated_trampn + j];
            if (current_vpos >= max_vpos) {
                max_vpos = current_vpos;
            } else {
                continue;
            }

            path.reset();

            for (int k = 1; k < current_trampn-j && k <= max_jumps[i]; k++) {
                if (path[k] == 1) {
                    continue;
                }

                if (vposes[cumulated_trampn + j+k] > current_vpos) {
                    break;
                }
                visited = 1;
                current_pos = j+k;
                while (current_pos < current_trampn-1) {
                    path.set(current_pos-j);
                    visited += 1;
                    current_pos += jumps[cumulated_trampn+current_pos];
                }
                if (visited + current_vpos > max) {
                    max = visited + current_vpos;
                }
            }
        }
        std::cout << "Case #" << i << ": " << max << std::endl;
    }
    return 0;
}
