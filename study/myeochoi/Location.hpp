#ifndef LOCATION_HPP
#define LOCATION_HPP

#include <iostream>
#include <fstream>
#include <vector>

class Location
{
    private :
        std::string _path;
        std::string _root;
        std::string _index;
        std::vector<std::string> _methods;
        bool _autoindex;
        std::string _cgi;

    public :
        Location();
        Location(const Location &other);
        Location &operator&=(const Location &other);
        ~Location();
        bool setPath(const std::string& path);
        bool setRoot(const std::string& root);
        bool setIndex(const std::string& index);
        bool setMethods(const std::vector<std::string>& methods);
        bool setAutoindex(const std::string& autoindex);
        bool setCgi(const std::string& cgi);

};

#endif