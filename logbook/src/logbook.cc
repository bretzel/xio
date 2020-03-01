#include <xio/logbook/logbook.h>




namespace xio::logbook
{




std::map<webcolor, std::string> _web_webcolors =
{
    {webcolor::AliceBlue             ,"AliceBlue"},
    {webcolor::AntiqueWhite          ,"AntiqueWhite"},
    {webcolor::Aqua                  ,"Aqua"},
    {webcolor::Aquamarine            ,"Aquamarine"},
    {webcolor::Azure                 ,"Azure"},
    {webcolor::Beige                 ,"Beige"},
    {webcolor::Bisque                ,"Bisque"},
    {webcolor::Black                 ,"Black"},
    {webcolor::BlanchedAlmond        ,"BlanchedAlmond"},
    {webcolor::Blue                  ,"Blue"},
    {webcolor::BlueViolet            ,"BlueViolet"},
    {webcolor::Brown                 ,"Brown"},
    {webcolor::BurlyWood             ,"BurlyWood"},
    {webcolor::CadetBlue             ,"CadetBlue"},
    {webcolor::Chartreuse            ,"Chartreuse"},
    {webcolor::Chocolate             ,"Chocolate"},
    {webcolor::Coral                 ,"Coral"},
    {webcolor::CornflowerBlue        ,"CornflowerBlue"},
    {webcolor::Cornsilk              ,"Cornsilk"},
    {webcolor::Crimson               ,"Crimson"},
    {webcolor::Cyan                  ,"Cyan"},
    {webcolor::DarkBlue              ,"DarkBlue"},
    {webcolor::DarkCyan              ,"DarkCyan"},
    {webcolor::DarkGoldenRod         ,"DarkGoldenRod"},
    {webcolor::DarkGray              ,"DarkGray"},
    {webcolor::DarkGrey              ,"DarkGrey"},
    {webcolor::DarkGreen             ,"DarkGreen"},
    {webcolor::DarkKhaki             ,"DarkKhaki"},
    {webcolor::DarkMagenta           ,"DarkMagenta"},
    {webcolor::DarkOliveGreen        ,"DarkOliveGreen"},
    {webcolor::DarkOrange            ,"DarkOrange"},
    {webcolor::DarkOrchid            ,"DarkOrchid"},
    {webcolor::DarkRed               ,"DarkRed"},
    {webcolor::DarkSalmon            ,"DarkSalmon"},
    {webcolor::DarkSeaGreen          ,"DarkSeaGreen"},
    {webcolor::DarkSlateBlue         ,"DarkSlateBlue"},
    {webcolor::DarkSlateGray         ,"DarkSlateGray"},
    {webcolor::DarkSlateGrey         ,"DarkSlateGrey"},
    {webcolor::DarkTurquoise         ,"DarkTurquoise"},
    {webcolor::DarkViolet            ,"DarkViolet"},
    {webcolor::DeepPink              ,"DeepPink"},
    {webcolor::DeepSkyBlue           ,"DeepSkyBlue"},
    {webcolor::DimGrey               ,"DimGrey"},
    {webcolor::DodgerBlue            ,"DodgerBlue"},
    {webcolor::FireBrick             ,"FireBrick"},
    {webcolor::FloralWhite           ,"FloralWhite"},
    {webcolor::ForestGreen           ,"ForestGreen"},
    {webcolor::Fuchsia               ,"Fuchsia"},
    {webcolor::Gainsboro             ,"Gainsboro"},
    {webcolor::GhostWhite            ,"GhostWhite"},
    {webcolor::Gold                  ,"Gold"},
    {webcolor::GoldenRod             ,"GoldenRod"},
    {webcolor::Gray                  ,"Gray"},
    {webcolor::Grey                  ,"Grey"},
    {webcolor::Green                 ,"Green"},
    {webcolor::GreenYellow           ,"GreenYellow"},
    {webcolor::HoneyDew              ,"HoneyDew"},
    {webcolor::HotPink               ,"HotPink"},
    {webcolor::IndianRed             ,"IndianRed"},
    {webcolor::Indigo                ,"Indigo"},
    {webcolor::Ivory                 ,"Ivory"},
    {webcolor::Khaki                 ,"Khaki"},
    {webcolor::Lavender              ,"Lavender"},
    {webcolor::LavenderBlush         ,"LavenderBlush"},
    {webcolor::LawnGreen             ,"LawnGreen"},
    {webcolor::LemonChiffon          ,"LemonChiffon"},
    {webcolor::LightBlue             ,"LightBlue"},
    {webcolor::LightCoral            ,"LightCoral"},
    {webcolor::LightCyan             ,"LightCyan"},
    {webcolor::LightGoldenRodYellow  ,"LightGoldenRodYellow"},
    {webcolor::LightGray             ,"LightGray"},
    {webcolor::LightGreen            ,"LightGreen"},
    {webcolor::LightPink             ,"LightPink"},
    {webcolor::LightSalmon           ,"LightSalmon"},
    {webcolor::LightSeaGreen         ,"LightSeaGreen"},
    {webcolor::LightSkyBlue          ,"LightSkyBlue"},
    {webcolor::LightSlateGray        ,"LightSlateGray"},
    {webcolor::LightSteelBlue        ,"LightSteelBlue"},
    {webcolor::LightYellow           ,"LightYellow"},
    {webcolor::Lime                  ,"Lime"},
    {webcolor::LimeGreen             ,"LimeGreen"},
    {webcolor::Linen                 ,"Linen"},
    {webcolor::Magenta               ,"Magenta"},
    {webcolor::Maroon                ,"Maroon"},
    {webcolor::MediumAquaMarine      ,"MediumAquaMarine"},
    {webcolor::MediumBlue            ,"MediumBlue"},
    {webcolor::MediumOrchid          ,"MediumOrchid"},
    {webcolor::MediumPurple          ,"MediumPurple"},
    {webcolor::MediumSeaGreen        ,"MediumSeaGreen"},
    {webcolor::MediumSlateBlue       ,"MediumSlateBlue"},
    {webcolor::MediumSpringGreen     ,"MediumSpringGreen"},
    {webcolor::MediumTurquoise       ,"MediumTurquoise"},
    {webcolor::MediumVioletRed       ,"MediumVioletRed"},
    {webcolor::MidnightBlue          ,"MidnightBlue"},
    {webcolor::MintCream             ,"MintCream"},
    {webcolor::MistyRose             ,"MistyRose"},
    {webcolor::Moccasin              ,"Moccasin"},
    {webcolor::NavajoWhite           ,"NavajoWhite"},
    {webcolor::Navy                  ,"Navy"},
    {webcolor::OldLace               ,"OldLace"},
    {webcolor::Olive                 ,"Olive"},
    {webcolor::OliveDrab             ,"OliveDrab"},
    {webcolor::Orange                ,"Orange"},
    {webcolor::OrangeRed             ,"OrangeRed"},
    {webcolor::Orchid                ,"Orchid"},
    {webcolor::PaleGoldenRod         ,"PaleGoldenRod"},
    {webcolor::PaleGreen             ,"PaleGreen"},
    {webcolor::PaleTurquoise         ,"PaleTurquoise"},
    {webcolor::PaleVioletRed         ,"PaleVioletRed"},
    {webcolor::PapayaWhip            ,"PapayaWhip"},
    {webcolor::PeachPuff             ,"PeachPuff"},
    {webcolor::Peru                  ,"Peru"},
    {webcolor::Pink                  ,"Pink"},
    {webcolor::Plum                  ,"Plum"},
    {webcolor::PowderBlue            ,"PowderBlue"},
    {webcolor::Purple                ,"Purple"},
    {webcolor::RebeccaPurple         ,"RebeccaPurple"},
    {webcolor::Red                   ,"Red"},
    {webcolor::RosyBrown             ,"RosyBrown"},
    {webcolor::RoyalBlue             ,"RoyalBlue"},
    {webcolor::SaddleBrown           ,"SaddleBrown"},
    {webcolor::Salmon                ,"Salmon"},
    {webcolor::SandyBrown            ,"SandyBrown"},
    {webcolor::SeaGreen              ,"SeaGreen"},
    {webcolor::SeaShell              ,"SeaShell"},
    {webcolor::Sienna                ,"Sienna"},
    {webcolor::Silver                ,"Silver"},
    {webcolor::SkyBlue               ,"SkyBlue"},
    {webcolor::SlateBlue             ,"SlateBlue"},
    {webcolor::SlateGray             ,"SlateGray"},
    {webcolor::SlateGrey             ,"SlateGrey"},
    {webcolor::Snow                  ,"Snow"},
    {webcolor::SpringGreen           ,"SpringGreen"},
    {webcolor::SteelBlue             ,"SteelBlue"},
    {webcolor::Tan                   ,"Tan"},
    {webcolor::Teal                  ,"Teal"},
    {webcolor::Thistle               ,"Thistle"},
    {webcolor::Tomato                ,"Tomato"},
    {webcolor::Turquoise             ,"Turquoise"},
    {webcolor::Violet                ,"Violet"},
    {webcolor::Wheat                 ,"Wheat"},
    {webcolor::White                 ,"White"},
    {webcolor::WhiteSmoke            ,"WhiteSmoke"},
    {webcolor::Yellow                ,"Yellow"},
    {webcolor::YellowGreen           ,"YellowGreen"}
};


using utils::notification;



std::string logbook::_dbname = "logbook";
//xio::xdb::sqlitedb   logbook::_dbdata;


logbook::~logbook()
{

}

std::string logbook::dbname()
{
    return logbook::_dbname;
}

logbook::result logbook::open()
{
    

    return notification::code::implement;
}


}
