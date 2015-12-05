#include "wagfile.h"


WAGFile::WAGFile(std::string in_filename, std::string in_description, std::string in_author,
                 std::vector<std::string> in_tags) :
    description(in_description), author(in_author), tags(in_tags) {


//    path = boost::filesystem::path(in_filename);
//    path.remove_filename();

}
