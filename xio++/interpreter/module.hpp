/*
 * Copyright (c) 2018 Serge Lussier <lussier.serge@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following
 * conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */

#pragma once 

#include <xio++/interpreter/kernel/stack.hpp>
#include <xio++/interpreter/compiler/lexer.hpp>

namespace xio {

/**
 * @todo write docs
 */
class xio_module : public xio_stack
{
    
    struct {
        lexer_t          lexer;
        token_t::list_t* tokens;
        const char* src;
    }text;
    
public:
    
    struct config_t{
        std::string id;
        std::string uri; // "[file:file_path/filename.ext | text:'code source;']"
        token_t::list_t* tokens;
        const char* src;
    }cfg;
    
    xio_module();
    xio_module(object* a_parent, token_t* a_token, alu* a=nullptr);
    xio_module(object* a_parent);    
    xio_module::config_t& config() { return cfg;}
    
    const std::string& name() { return cfg.id; }
    const std::string& uri() { return cfg.uri; }
    
    virtual xio_t::result build();
    virtual message::xcode parse_uri();
    
    alu jsr() override;
    
    
    
};

}


