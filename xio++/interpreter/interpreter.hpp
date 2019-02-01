#pragma once

#include <xio++/interpreter/module.hpp>
#include <xio++/interpreter/kernel/alu.hpp>
#include <xio++/journal/logger.hpp>
namespace xio{




class xio_api interpreter : public xio_module{
        

public:
    interpreter();

    

    ~interpreter() override;
    alu jsr() override;
    
}; 



}
