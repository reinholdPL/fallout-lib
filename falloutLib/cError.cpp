#include "cError.h"

namespace falloutLib
{
	cError::cError()
	{
		errorState = NO_ERROR;
	}

	errorResult cError::getErrorState()
	{
		return errorState;
	}

	void cError::setErrorState(errorResult error)
	{
		errorState = error;
	}
}