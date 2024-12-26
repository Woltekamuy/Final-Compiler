#pragma once
#include <vector>
#include "Error.h"

class Errors {
private:
	static std::vector<Error> errors;

public:
	static std::vector<Error>& getErrors() { return errors; }
	static bool hasError() { return !errors.empty(); }
};