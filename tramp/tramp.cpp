#include <iostream>
#include <fstream>
#include <string>
#include <map>

int N = 200000;

struct tramp_data {
    int lines;
    int* trampn;
    int* jumps;
};

tramp_data load_tramp_data(std::string filename) {
    std::ifstream input(filename);
    std::string content;

    int lines;
    static int* trampn;
    static int* jumps;

    if (input.is_open()) {
        input >> content;
        lines = std::stoi(content);
        
        trampn = new int[lines];
        jumps = new int[lines*N];

        // Fill the tramps
        int n;
        int m;
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
    int lines = data.lines;
    int* trampn = data.trampn;
    int* jumps = data.jumps;

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
    std::map<int, int> vposes[lines];
    std::map<int, int> vleft[lines];
    
    int maxs[lines];
    int max_jump[lines];

    int max;
    
    int current_pos;
    int visited;
    bool broke;

    for (int i = 0; i < lines; i++) {
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

            broke = false;

            while (current_pos < trampn[i]-1) {

                if (vposes[i].find(current_pos) == vposes[i].end()) {
                    vposes[i][current_pos] = visited;
                } else {
                    if (visited > vposes[i][current_pos]) {
                        vposes[i][current_pos] = visited;
                    } else {
                        broke = true;
                        break;
                    }
                }

                visited += 1;
                if (jumps[N*i+current_pos] > max_jump[i]) {
                    max_jump[i] = jumps[N*i+current_pos];
                }
                current_pos += jumps[N*i+current_pos];
            }

            if (visited > max) {
                max = visited;
            }
            if (!broke) {
                vleft[i][j] = visited;
            }
        }

        maxs[i] = max;
        //std::cout << "Case #" << i << ": " << max << std::endl;
    }

    // Subsection 4
    // Uses the vposes from section 3
    int max_vpos;
    for (int i = 0; i < lines; i++) {
        if (trampn[i] < 3) {
            std::cout << "Case #" << i << ": " << trampn[i] << std::endl;
            continue;
        }

        max = maxs[i];
        max_vpos = 0;

        for (int j = 0; j < trampn[i]-2; j++) {
            if (vposes[i][j] > max_vpos) {
                max_vpos = vposes[i][j];
            } else {
                continue;
            }

            for (int k = 1; k < trampn[i]-j && k <= max_jump[i]; k++) {
                if (vposes[i][j] + 1 - vposes[i][j+k] > 0) {
                    if (vleft[i].find(j+k) == vleft[i].end()) {
                        current_pos = j+k;
                        vleft[i][j+k] = 1;
                        while (current_pos < trampn[i]-1) {
                            vleft[i][j+k] += 1;
                            current_pos += jumps[N*i+current_pos];
                        }
                    }
                    if (vposes[i][j] + vleft[i][j+k] > max) {
                        max = vposes[i][j] + vleft[i][j+k];
                    }
                }
            }
        }
        std::cout << "Case #" << i << ": " << max << std::endl;
    }

    return 0;
}
