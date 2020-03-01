#include "tdd.h"
#include <xio/utils/xstr.h>
#include <xio/sqlitedb/sqlitedb.h>
#include <xio/utils/notification.h>
#include <xio/logbook/logbook.h>
#include <fstream>

xio::utils::xstr W3C_Colors = R"(
        AliceBlue	   
        AntiqueWhite
        Aqua	       
        Aquamarine	   
        Azure	       
        Beige	       
        Bisque	       
        Black	       
        BlanchedAlmond
        Blue	       
        BlueViolet	   
        Brown	       
        BurlyWood	   
        CadetBlue	   
        Chartreuse	   
        Chocolate	   
        Coral	       
        CornflowerBlue
        Cornsilk	   
        Crimson	       
        Cyan	       
        DarkBlue	   
        DarkCyan	   
        DarkGoldenRod
        DarkGray	   
        DarkGrey	   
        DarkGreen	   
        DarkKhaki	   
        DarkMagenta	   
        DarkOliveGreen
        DarkOrange	   
        DarkOrchid	   
        DarkRed	       
        DarkSalmon	   
        DarkSeaGreen
        DarkSlateBlue
        DarkSlateGray
        DarkSlateGrey
        DarkTurquoise
        DarkViolet	   
        DeepPink    
        DeepSkyBlue	   
        DimGrey	       
        DodgerBlue	   
        FireBrick	   
        FloralWhite	   
        ForestGreen	   
        Fuchsia	       
        Gainsboro	   
        GhostWhite	   
        Gold	       
        GoldenRod	   
        Gray	       
        Grey	       
        Green	       
        GreenYellow	   
        HoneyDew	   
        HotPink	       
        IndianRed	   
        Indigo	       
        Ivory	       
        Khaki	       
        Lavender	   
        LavenderBlush
        LawnGreen	   
        LemonChiffon
        LightBlue	   
        LightCoral	   
        LightCyan	   
        LightGoldenRodYellow
        LightGray	    
        LightGreen	    
        LightPink	    
        LightSalmon	    
        LightSeaGreen	
        LightSkyBlue	
        LightSlateGray	
        LightSteelBlue	
        LightYellow	    
        Lime	        
        LimeGreen	    
        Linen	        
        Magenta	        
        Maroon	        
        MediumAquaMarine
        MediumBlue	    
        MediumOrchid	
        MediumPurple	
        MediumSeaGreen	
        MediumSlateBlue	
        MediumSpringGreen
        MediumTurquoise	
        MediumVioletRed	
        MidnightBlue	
        MintCream	    
        MistyRose	    
        Moccasin	    
        NavajoWhite	    
        Navy	        
        OldLace	        
        Olive	        
        OliveDrab	    
        Orange	        
        OrangeRed	    
        Orchid	        
        PaleGoldenRod	
        PaleGreen	    
        PaleTurquoise	
        PaleVioletRed	
        PapayaWhip	    
        PeachPuff	    
        Peru	        
        Pink	        
        Plum	        
        PowderBlue	    
        Purple	        
        RebeccaPurple	
        Red	            
        RosyBrown	    
        RoyalBlue	    
        SaddleBrown	    
        Salmon	        
        SandyBrown	    
        SeaGreen	    
        SeaShell	    
        Sienna	        
        Silver	        
        SkyBlue     	
        SlateBlue	    
        SlateGray	    
        SlateGrey	    
        Snow	        
        SpringGreen	    
        SteelBlue	    
        Tan	            
        Teal	        
        Thistle	        
        Tomato	        
        Turquoise	    
        Violet	        
        Wheat	        
        White	        
        WhiteSmoke	    
        Yellow	        
        YellowGreen	    

)";

auto main() -> int {
    xio::utils::xstr str = "Hello, World!\n";
    xio::utils::xstr::word::collection words;



    str << " logbook dbname()='" << xio::logbook::logbook::dbname() << "'\n";
    std::size_t count = W3C_Colors.words(words, "", false);
    str << " w3c colors count:" << count << "\n";
    std::ofstream out;
    out.open("logbook-colors.h");
    if (!out.good())
    {
        //...
    }
    for (auto w : words)
    {
        xio::utils::xstr line = "{%-22s,\"%s\"},\n";
        line << w() << w();
        //std::cout << '{' << w() << "\t\t,\"" << w() << "\"},\n";
        std::cout << line();
        out << line();
    }
    out.close();
    
    std::cout << str();

    xio::xdb::sqlitedb db{ "logbook" };
    xio::xdb::sqlitedb::code c = db.open();


    xio::utils::notification::clear(
        [](xio::utils::notification& n) 
        {
            std::cout << n() << '\n';
        }
    );

    return 0;
}