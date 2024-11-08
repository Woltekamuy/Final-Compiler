#pragma once

class Func {
private:
    int index;
    int args;

public:
    Func(int idx, int arguments) : index(idx), args(arguments) {}

    int getIndex() const;
    int getArgs() const;
};