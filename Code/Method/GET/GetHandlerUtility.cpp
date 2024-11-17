#include "../../MethodHeader/GetHandler.hpp"

bool GetHandler::checkFileAccess(Request& request) {
    int mode = F_OK;
    if (access(request.getMappingUrl().c_str(), mode) != 0) {
        request.setError(404);
        return false;
    }

    mode = R_OK;
    if (access(request.getMappingUrl().c_str(), mode) != 0) {
        request.setError(403);
        return false;
    }
    return true;
}