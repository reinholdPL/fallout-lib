#ifndef __C_ERROR_H
#define __C_ERROR_H

#include "utils.h"

namespace falloutLib
{
    class cError
    {
    public:
        cError();
        errorResult getErrorState();
        void setErrorState(errorResult error);
        
    private:
        errorResult errorState;
    };

}

#endif