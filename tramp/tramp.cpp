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
    static unsigned int* jumps;

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

                jumps[i*N+j] = m;
            }
        }
    }

    return tramp_data{ lines, trampn, jumps };
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
    std::map<unsigned int, unsigned int> vposes[lines];
    
    unsigned int maxs[lines];

    unsigned int max;
    
    unsigned int current_pos;
    unsigned int visited;

    for (int i = 0; i < lines; i++) {
        if (i > 50) {
            continue;
        }
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
                if (vposes[i].find(current_pos) == vposes[i].end()) {
                    vposes[i][current_pos] = visited;
                } else {
                    if (visited > vposes[i][current_pos]) {
                        vposes[i][current_pos] = visited;
                    } else {
                        break;
                    }
                }

                visited += 1;

                current_pos += jumps[N*i+current_pos];
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
    
    //int* jump_counts;

    //jump_counts = new int[N/2*lines] { 0 };
    //
    //for (int i = 0; i < lines; i++) {
    //    if (i > 50) {
    //        continue;
    //    }
    //    for (int j = 0; j < trampn[i]; j++) {
    //        if (jumps[N*i+j] < trampn[i]/2+1) {
    //            jump_counts[N/2*i+jumps[N*i+j]] += 1;
    //        }
    //    }
    //}

    // Fill the max jumps

    // Doesn't work because it means that we can't skip as many
    // instances
    //int max_jump[lines];
    //int n_divider = 1;

    //std::cout << N/n_divider << std::endl;

    //for (int i = 0; i < lines; i++) {
    //    if (i > 50) {
    //        continue;
    //    }
    //    max_jump[i] = N/n_divider;
    //    for (int j = trampn[i]/2; j > N/n_divider; j--) {
    //        if (jump_counts[N/2*i+j] >= j) {
    //            max_jump[i] = j;
    //            break;
    //        }
    //    }
    //}
    //
    //int max_vpos_index;
 
    unsigned int max_vpos;
    std::bitset<N> path;

    unsigned int current_trampn;
    unsigned int current_ioffset;

    unsigned int current_vpos;

    for (int i = 0; i < lines; i++) {
        current_trampn = trampn[i];
        current_ioffset = N*i;

        std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();

        if (i > 50) {
            continue;
        }
        if (current_trampn < 3) {
            std::cout << "Case #" << i << ": " << current_trampn << std::endl;
            continue;
        }

        max = maxs[i];
        max_vpos = 0;

        for (int j = 0; j < current_trampn-2; j++) {
            current_vpos = vposes[i][j];
            if (current_vpos > max_vpos) {
                max_vpos = current_vpos;
            } else {
                continue;
            }

            path.reset();

            for (int k = 1; k < current_trampn-j && path[k] == 0; k++) {
                if (vposes[i][j+k] > current_vpos) {
                    break;
                }
                visited = 1;
                current_pos = j+k;
                while (current_pos < current_trampn-1) {
                    path.set(current_pos-j);
                    visited += 1;
                    current_pos += jumps[current_ioffset+current_pos];
                }
                if (visited + current_vpos > max) {
                    max = visited + current_vpos;
                }
            }
        }
        std::cout << "Case #" << i << ": " << max << std::endl;
        std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
        std::cout << "Timer: " << std::chrono::duration_cast<std::chrono::seconds>(end-start).count() << std::endl;
    }
    return 0;
}
