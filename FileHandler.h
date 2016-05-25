//
// Created by lchavarr on 4/19/16.
//

#ifndef SOLCTRA_FILEHANDLER_H
#define SOLCTRA_FILEHANDLER_H

#pragma offload_attribute(push, target(mic))
#include <string>
#include <fstream>

class FileHandler
{
public:
    FileHandler(const int id);
    FileHandler(const std::string& path);
    FileHandler(const std::string& path, bool overwrite);
    void write(const std::string& data);
    void write(const double& x, const double& y, const double& z);
    void close();

private:
    std::string path_;
    std::ofstream handler_;

    void open(bool overwrite);
};
#pragma offload_attribute(pop)


#endif //SOLCTRA_FILEHANDLER_H
