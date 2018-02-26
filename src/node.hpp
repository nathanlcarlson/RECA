#ifndef NODE_HPP
#define NODE_HPP

#include <vector>
#include "utils.hpp"

class Node {

  public:

    // Select random value
    virtual double random_value() = 0;

    // Shift a value, with specified shift
    virtual double shifted_value(double _current, double _shift) = 0;

    // Shift with randomly generated shift
    virtual double shifted_value(double _current) = 0;

    // Get a shift
    virtual double get_shift() = 0;

    // TODO Make cyclic and not cyclic versions of this
    virtual void getRGB(double _v, std::vector<double>&) = 0;
};


#endif
