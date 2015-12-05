#pragma once
#include <vector>
#include <stdio.h>
#include <stdlib.h>
//#include <boost/filesystem/path.hpp>
#include <string>

class WAGFile {
public:
    WAGFile(std::string filename, std::string description, std::string author,
            std::vector<std::string> in_tags);
    std::string getName() {return name;}
    std::string getDescription() {return description;}
    std::string getAuthor() {return author;}
    std::vector<std::string> getTags() {return tags;}
    void updateFilename(std::string newName) { name = newName;}

private:
    // the path to the file location
//    boost::filesystem::path path;
    // the file's actual name x.wagz
    std::string name;
    std::string description;
    std::string author;
    std::vector<std::string> tags;
};
