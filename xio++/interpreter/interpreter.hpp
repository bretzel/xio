#pragma once

#include <xio++/interpreter/module.hpp>

namespace xio{

class interpreter : public xio_module{
        
public:
    interpreter();
    
    ~interpreter() override;
    alu jsr() override;
    
}; 

}
