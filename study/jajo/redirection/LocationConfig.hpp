class LocationiConfig {
    public :
        bool hasRedirect() const;
        std::pair<int, std::string> getRedirectInfo() const;

    private:
        bool _hasRedirect;
        int _redirectCode;
        std::string _redirectUrl;
}