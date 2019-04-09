
#pragma once

#include <map>
#include <utility>
#include <string_view>

namespace xio {

    /*
        Source : https://www.w3schools.com/lib/w3-colors-metro.css
    */
    enum class W3CMetro :int16_t
    {
        w3cMetroLightGreen, // #99b433
        w3cMetroBreen,      // #00a300
        w3cMetroDarkGreen,  // #1e7145
        w3cMetroMagenta,    // #ff0097  
        w3cMetroLightPurple,// #9f00a7
        w3cMetroPurple,     // #7e3878 
        w3cMetroDarkPurple, // #603cba 
        w3cMetroDarken,     // #1d1d1d 
        w3cMetroTeal,       // #00aba9
        w3cMetroLightBlue,  // #eff4ff
        w3cMetroBlue,       // #2d89ef
        w3cMetroDarkBlue,   // #2b5797
        w3cMetroYellow,     // #ffc40d
        w3cMetroOrange,     // #e3a21a
        w3cMetroDarkOrange, // #da532c
        w3cMetroRed,        // #ee1111
        w3cMetroDarkRed     // #b91d47
    };


    /*
    

.w3-metro-light-green
.w3-metro-green
.w3-metro-dark-green
.w3-metro-magenta
.w3-metro-light-purple
.w3-metro-purple
.w3-metro-dark-purple
.w3-metro-darken
.w3-metro-teal
.w3-metro-light-blue
.w3-metro-blue
.w3-metro-dark-blue
.w3-metro-yellow
.w3-metro-orange
.w3-metro-dark-orange
.w3-metro-red
.w3-metro-dark-red
    */

    namespace w3c {
        using NoW3CSS = std::pair<std::string_view, std::string_view>;
        using NoW3CSSMap = std::map<W3CMetro, NoW3CSS>;
    }

}