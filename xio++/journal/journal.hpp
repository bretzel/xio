//
// Created by bretzelus on 18-11-27.
//
#pragma once

#include <xio++/string_t.hpp>
#include <map>

namespace xio {

class xio_api journal {

    struct xio_api book {
        struct xio_api table_of_contens {
            std::string description;
            std::string pglink;
            int         pgnum=-1;
            using item = std::map<std::string, std::string>;

        };

        struct xio_api chapter {
            std::string _id;


        };
    };

};

}

