// safe-delete.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <filesystem>

using std::filesystem::recursive_directory_iterator;

std::string random_string() {
    static const char charlist[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";

    std::string tmp_s = "\\";
    for (int i = 0; i < rand() % (12) + 1; ++i) {
        tmp_s += charlist[rand() % (sizeof(charlist) - 1)];
    }
    return tmp_s;
}

int main()
{
    int pass = 1;
    std::string path;

    std::getline(std::cin, path);
    
    std::cout << "Enter the ammount of passes: ";
    std::cin >> pass;
    std::cout << std::endl;

    try {
        for (const auto& file : recursive_directory_iterator(path)) {
            if (file.is_directory())
                continue;

            auto file_size = file.file_size();
            auto file_path = file.path().string();

            std::fstream self;
            self.open(file_path.c_str(), std::ios::out | std::ios::in);
            
            for (int i = 0; i < pass; i++) {
                for (uintmax_t it = 0; it < file_size; it++) {
                    // converting it to char will automatically clamp it in 0-255 range, however this is an unexpected behavour..
                    // but it works d-_-b
                    // also we will shift a char each time we make a new pass, so the text will actually be re-written
                    self.put(char(it + i)); 
                }
            }
            self.close();

            // change name of the file
            auto new_name = file.path().parent_path().string() + random_string();
            std::filesystem::rename(file_path.c_str(), new_name);

            // remove the file using our new(renamed) path
            std::filesystem::remove(new_name);
        }
    }
    catch (std::filesystem::filesystem_error& ex) {
        std::cout << "error occured: " << ex.what() << '\n';
    }
}

