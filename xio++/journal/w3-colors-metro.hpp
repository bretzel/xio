
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
        /*w3cMetro*/LightGreen, // #99b433
        /*w3cMetro*/Green,      // #00a300
        /*w3cMetro*/DarkGreen,  // #1e7145
        /*w3cMetro*/Magenta,    // #ff0097  
        /*w3cMetro*/LightPurple,// #9f00a7
        /*w3cMetro*/Purple,     // #7e3878 
        /*w3cMetro*/DarkPurple, // #603cba 
        /*w3cMetro*/Darken,     // #1d1d1d 
        /*w3cMetro*/Teal,       // #00aba9
        /*w3cMetro*/LightBlue,  // #eff4ff
        /*w3cMetro*/Blue,       // #2d89ef
        /*w3cMetro*/DarkBlue,   // #2b5797
        /*w3cMetro*/Yellow,     // #ffc40d
        /*w3cMetro*/Orange,     // #e3a21a
        /*w3cMetro*/DarkOrange, // #da532c
        /*w3cMetro*/Red,        // #ee1111
        /*w3cMetro*/DarkRed     // #b91d47
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
        using W3Class = std::pair<std::string_view, std::string_view>;
        using W3ClassMap = std::map<W3CMetro, W3Class>;
    }

}