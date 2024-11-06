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
