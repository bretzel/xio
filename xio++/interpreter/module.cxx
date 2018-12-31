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

#include "module.hpp"
#include <xio++/journal/logger.hpp>

using namespace xio;


xio_module::xio_module()
{
}

xio_module::xio_module(xio::object* a_parent):xio_stack(a_parent)
{
}


xio_module::xio_module(object* a_parent, token_t* a_token, alu* a):xio_stack(a_parent, a_token, a)
{
}


alu xio_module::jsr()
{
    return xio_stack::jsr();
}

xio_t::result xio::xio_module::build()
{
    text.tokens = cfg.tokens;
	logdebugfn << " Parse URI:" << Ends;
    parse_uri();
    /*
    text.src    = cfg.src;
    
    if(!text.tokens)
        return { (message::push(message::xclass::error), " module::build: tokens stream is null.") };
    
    lexer_t::result r = text.lexer(text.tokens)[cfg.uri.c_str()];
    if(!r)
        return { r.notice() };
    */
    return { nullptr };
    
}


message::xcode xio::xio_module::parse_uri()
{
    string_t::word::list words;
    string_t str = cfg.uri;
    std::size_t count = str.words(words, ":/",true);
    auto it = words.begin();
    for(;it != words.end(); it++){
        logdebugfn << " token:[" << logger::Yellow << (*it)() << logger::Reset << ']' << Ends;
    }
    
//     if((*it)() == "text"){
//         ++it;
//         if((*it)() != ":"){
//             message::push(message::xclass::error), "error in module::parse_uri: expected separator ':', got ", (*it)(), "instead.";
//             return message::xcode::rejected;
//         }
//         ++it;
//         if((*it)() != "//"){
//             message::push(message::xclass::error), "error in module::parse_uri: expected separator '//', got ", (*it)(), "instead.";
//             return message::xcode::rejected;
//         }
//         ++it;
//         const char* m = (*it)().c_str();
//         const char* n = cfg.uri.c_str();
//         cfg.src = reinterpret_cast<const char*>(m-n);
//         logdebugfn << logger::White << "source: [" << logger::Yellow << cfg.src << logger::White << "]" << Ends;
//     }
//     
    
    return message::xcode::accepted;
}



