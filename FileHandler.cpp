//
// Created by lchavarr on 4/19/16.
//

#include "FileHandler.h"
#include <iostream>

FileHandler::FileHandler(const int id)
{
    path_ = "results/path" + std::to_string(id) + ".txt";
    open(true);
}

FileHandler::FileHandler(const std::string& path)
{
    open(true);
}

FileHandler::FileHandler(const std::string& path, bool overwrite) : path_(path)
{
    open(overwrite);
}

void FileHandler::open(bool overwrite)
{
    if(overwrite)
    {
        handler_.open(path_, std::ios::trunc | std::ios::out);
    }
    else
    {
        handler_.open(path_);
    }
    if (!handler_.is_open())
    {
        std::cerr << "Unable to open " << path_ << " for appending. Nothing to do." << std::endl;
        exit(0);
    }
    handler_.precision(6);
}

void FileHandler::write(const std::string& data)
{
    handler_<< data << std::endl;
}

void FileHandler::write(const float& x, const float& y, const float& z)
{
    handler_ << std::scientific;
    handler_ << x << "\t" << y << "\t" << z << std::endl;
}

void FileHandler::close()
{
    handler_.close();
}





