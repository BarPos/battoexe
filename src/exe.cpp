#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <Windows.h>
#include <stdio.h>
#include <process.h>
#include "base64.h"
#include "b64data.h"

#include <filesystem>
#include <fstream>

int main(int argc, char** argv){
    std::string arguments = "";
    for(int i = 0; i < argc - 1; i++){
        arguments += (" " + (std::string)argv[i+1]);
    }

    std::string filename = (std::string)tmpnam(nullptr) + ".bat";

    std::string batFile = base64::from_base64(b64data);

    std::ofstream file;
    file.open(filename);
    if(!file.is_open()){
        fprintf(stderr, "COULD NOT CREATE A TEMP FILE: %s\n", filename.c_str());
        exit(1);
    }
    file << batFile;
    file.close();

    system((filename+arguments).c_str());

    std::filesystem::remove(filename);

    return 0;
}