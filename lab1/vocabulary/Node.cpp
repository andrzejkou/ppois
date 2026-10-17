#include "Node.h"

Node::Node(const std::string &eng, const std::string &rus)
    : eng(eng), rus(rus), left(nullptr), right(nullptr) {}
