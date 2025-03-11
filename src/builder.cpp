#include <iostream>
#include <fstream>
#include <filesystem>
#include "base64.h"

int main(int argc, char** argv){
    if(argc != 2){
        fprintf(stderr, "Ussage: builder.exe [input bat file]\n");
        exit(1);
    }

    std::ifstream file(argv[1]);
    if(!file.is_open()){
        fprintf(stderr, "Could not open file: %s\n", argv[1]);
        exit(1);
    }
    std::string bat;
    std::string _i;
    while (getline(file, _i))
    {
        bat += _i + "\n";
    }

    std::string bat_b64 = base64::to_base64(bat);

    if(std::filesystem::exists("output"))
        if(!std::filesystem::remove_all("output")){
            fprintf(stderr, "COULD NOT REMOVE: output\n");
            exit(1);
        }
    std::filesystem::create_directory("output");

    std::ofstream b64dataF;
    b64dataF.open("output/b64data.h");
    if(!file.is_open()){
        fprintf(stderr, "COULD NOT CREATE A FILE: output/b64data.h\n");
        exit(1);
    }
    b64dataF << "const char* b64data = \"" << bat_b64 << "\";";
    b64dataF.close();
    
    system("clang++ -Wall -std=c++17 src/exe.cpp -Isrc -Ioutput -o output/bat.exe");
    return 0;
}