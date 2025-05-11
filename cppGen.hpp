
#include "ast.h"

namespace cpp {

class cppGenerator
{

public:
    cppGenerator(ASTNode *node);
    ~cppGenerator();

    void generate();

private:
    ASTNode *node;
};

}