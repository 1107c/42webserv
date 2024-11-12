class RedirectionException : public std::exception {
    public :
        RedirectionException(int code, const std::string& locationi) : _code(code), _location(location) {}
        int getCode() const { return _code; }
        const std::string& getLocation() const { return _location; }

    private :
        int _code;
        std::string _location;
}
