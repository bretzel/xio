#include <xio/logbook/logbook.h>




namespace xio::logbook
{




std::map<logbook::color, std::string> _web_colors =
{
    {logbook::AliceBlue             ,"AliceBlue"},
    {logbook::AntiqueWhite          ,"AntiqueWhite"},
    {logbook::Aqua                  ,"Aqua"},
    {logbook::Aquamarine            ,"Aquamarine"},
    {logbook::Azure                 ,"Azure"},
    {logbook::Beige                 ,"Beige"},
    {logbook::Bisque                ,"Bisque"},
    {logbook::Black                 ,"Black"},
    {logbook::BlanchedAlmond        ,"BlanchedAlmond"},
    {logbook::Blue                  ,"Blue"},
    {logbook::BlueViolet            ,"BlueViolet"},
    {logbook::Brown                 ,"Brown"},
    {logbook::BurlyWood             ,"BurlyWood"},
    {logbook::CadetBlue             ,"CadetBlue"},
    {logbook::Chartreuse            ,"Chartreuse"},
    {logbook::Chocolate             ,"Chocolate"},
    {logbook::Coral                 ,"Coral"},
    {logbook::CornflowerBlue        ,"CornflowerBlue"},
    {logbook::Cornsilk              ,"Cornsilk"},
    {logbook::Crimson               ,"Crimson"},
    {logbook::Cyan                  ,"Cyan"},
    {logbook::DarkBlue              ,"DarkBlue"},
    {logbook::DarkCyan              ,"DarkCyan"},
    {logbook::DarkGoldenRod         ,"DarkGoldenRod"},
    {logbook::DarkGray              ,"DarkGray"},
    {logbook::DarkGrey              ,"DarkGrey"},
    {logbook::DarkGreen             ,"DarkGreen"},
    {logbook::DarkKhaki             ,"DarkKhaki"},
    {logbook::DarkMagenta           ,"DarkMagenta"},
    {logbook::DarkOliveGreen        ,"DarkOliveGreen"},
    {logbook::DarkOrange            ,"DarkOrange"},
    {logbook::DarkOrchid            ,"DarkOrchid"},
    {logbook::DarkRed               ,"DarkRed"},
    {logbook::DarkSalmon            ,"DarkSalmon"},
    {logbook::DarkSeaGreen          ,"DarkSeaGreen"},
    {logbook::DarkSlateBlue         ,"DarkSlateBlue"},
    {logbook::DarkSlateGray         ,"DarkSlateGray"},
    {logbook::DarkSlateGrey         ,"DarkSlateGrey"},
    {logbook::DarkTurquoise         ,"DarkTurquoise"},
    {logbook::DarkViolet            ,"DarkViolet"},
    {logbook::DeepPink              ,"DeepPink"},
    {logbook::DeepSkyBlue           ,"DeepSkyBlue"},
    {logbook::DimGrey               ,"DimGrey"},
    {logbook::DodgerBlue            ,"DodgerBlue"},
    {logbook::FireBrick             ,"FireBrick"},
    {logbook::FloralWhite           ,"FloralWhite"},
    {logbook::ForestGreen           ,"ForestGreen"},
    {logbook::Fuchsia               ,"Fuchsia"},
    {logbook::Gainsboro             ,"Gainsboro"},
    {logbook::GhostWhite            ,"GhostWhite"},
    {logbook::Gold                  ,"Gold"},
    {logbook::GoldenRod             ,"GoldenRod"},
    {logbook::Gray                  ,"Gray"},
    {logbook::Grey                  ,"Grey"},
    {logbook::Green                 ,"Green"},
    {logbook::GreenYellow           ,"GreenYellow"},
    {logbook::HoneyDew              ,"HoneyDew"},
    {logbook::HotPink               ,"HotPink"},
    {logbook::IndianRed             ,"IndianRed"},
    {logbook::Indigo                ,"Indigo"},
    {logbook::Ivory                 ,"Ivory"},
    {logbook::Khaki                 ,"Khaki"},
    {logbook::Lavender              ,"Lavender"},
    {logbook::LavenderBlush         ,"LavenderBlush"},
    {logbook::LawnGreen             ,"LawnGreen"},
    {logbook::LemonChiffon          ,"LemonChiffon"},
    {logbook::LightBlue             ,"LightBlue"},
    {logbook::LightCoral            ,"LightCoral"},
    {logbook::LightCyan             ,"LightCyan"},
    {logbook::LightGoldenRodYellow  ,"LightGoldenRodYellow"},
    {logbook::LightGray             ,"LightGray"},
    {logbook::LightGreen            ,"LightGreen"},
    {logbook::LightPink             ,"LightPink"},
    {logbook::LightSalmon           ,"LightSalmon"},
    {logbook::LightSeaGreen         ,"LightSeaGreen"},
    {logbook::LightSkyBlue          ,"LightSkyBlue"},
    {logbook::LightSlateGray        ,"LightSlateGray"},
    {logbook::LightSteelBlue        ,"LightSteelBlue"},
    {logbook::LightYellow           ,"LightYellow"},
    {logbook::Lime                  ,"Lime"},
    {logbook::LimeGreen             ,"LimeGreen"},
    {logbook::Linen                 ,"Linen"},
    {logbook::Magenta               ,"Magenta"},
    {logbook::Maroon                ,"Maroon"},
    {logbook::MediumAquaMarine      ,"MediumAquaMarine"},
    {logbook::MediumBlue            ,"MediumBlue"},
    {logbook::MediumOrchid          ,"MediumOrchid"},
    {logbook::MediumPurple          ,"MediumPurple"},
    {logbook::MediumSeaGreen        ,"MediumSeaGreen"},
    {logbook::MediumSlateBlue       ,"MediumSlateBlue"},
    {logbook::MediumSpringGreen     ,"MediumSpringGreen"},
    {logbook::MediumTurquoise       ,"MediumTurquoise"},
    {logbook::MediumVioletRed       ,"MediumVioletRed"},
    {logbook::MidnightBlue          ,"MidnightBlue"},
    {logbook::MintCream             ,"MintCream"},
    {logbook::MistyRose             ,"MistyRose"},
    {logbook::Moccasin              ,"Moccasin"},
    {logbook::NavajoWhite           ,"NavajoWhite"},
    {logbook::Navy                  ,"Navy"},
    {logbook::OldLace               ,"OldLace"},
    {logbook::Olive                 ,"Olive"},
    {logbook::OliveDrab             ,"OliveDrab"},
    {logbook::Orange                ,"Orange"},
    {logbook::OrangeRed             ,"OrangeRed"},
    {logbook::Orchid                ,"Orchid"},
    {logbook::PaleGoldenRod         ,"PaleGoldenRod"},
    {logbook::PaleGreen             ,"PaleGreen"},
    {logbook::PaleTurquoise         ,"PaleTurquoise"},
    {logbook::PaleVioletRed         ,"PaleVioletRed"},
    {logbook::PapayaWhip            ,"PapayaWhip"},
    {logbook::PeachPuff             ,"PeachPuff"},
    {logbook::Peru                  ,"Peru"},
    {logbook::Pink                  ,"Pink"},
    {logbook::Plum                  ,"Plum"},
    {logbook::PowderBlue            ,"PowderBlue"},
    {logbook::Purple                ,"Purple"},
    {logbook::RebeccaPurple         ,"RebeccaPurple"},
    {logbook::Red                   ,"Red"},
    {logbook::RosyBrown             ,"RosyBrown"},
    {logbook::RoyalBlue             ,"RoyalBlue"},
    {logbook::SaddleBrown           ,"SaddleBrown"},
    {logbook::Salmon                ,"Salmon"},
    {logbook::SandyBrown            ,"SandyBrown"},
    {logbook::SeaGreen              ,"SeaGreen"},
    {logbook::SeaShell              ,"SeaShell"},
    {logbook::Sienna                ,"Sienna"},
    {logbook::Silver                ,"Silver"},
    {logbook::SkyBlue               ,"SkyBlue"},
    {logbook::SlateBlue             ,"SlateBlue"},
    {logbook::SlateGray             ,"SlateGray"},
    {logbook::SlateGrey             ,"SlateGrey"},
    {logbook::Snow                  ,"Snow"},
    {logbook::SpringGreen           ,"SpringGreen"},
    {logbook::SteelBlue             ,"SteelBlue"},
    {logbook::Tan                   ,"Tan"},
    {logbook::Teal                  ,"Teal"},
    {logbook::Thistle               ,"Thistle"},
    {logbook::Tomato                ,"Tomato"},
    {logbook::Turquoise             ,"Turquoise"},
    {logbook::Violet                ,"Violet"},
    {logbook::Wheat                 ,"Wheat"},
    {logbook::White                 ,"White"},
    {logbook::WhiteSmoke            ,"WhiteSmoke"},
    {logbook::Yellow                ,"Yellow"},
    {logbook::YellowGreen           ,"YellowGreen"}
};


using utils::notification;

/*

    logbook Database 
    ---------------------------------------------------------------
    
    database schema:
    
    <table name="color">
        <Primary Key="ID"/> // Hidden.
        <column Unique name="Name"/> -> Defined by Either or in one of the Referentials : Ansi ( xterm colors set ) or/and Html (web colors) 
        <column name="Html"/> \
                               -> both can have a value or one can be undefined (null).
        <column name="Ansi"/> /
    </table>


    logfmt: 
 
    enum class color : uint8_t
    {
      black = 0, red = 1, green = 2, amber = 3, blue = 4, magenta = 5, cyan = 6, hgray, dgray, hred, hgreen, yellow, hblue, hmagenta, hcyan, white,
      reset, ///< remise-&agrave;-z&eacute;ro des attributs.
    };


    logfmt <-> database schema:

    [logfmt::mode  attribute] <-> std::string/xio::utils::xstr <-> [logbookapi] <-> std::string/xio::utils::xstr <-> [database::color_table::column.name{html,ansi}]
    [logfmt::color attribute] <-> std::string/xio::utils::xstr <-> [logbookapi] <-> std::string/xio::utils::xstr <-> [database::color_table::column.name[id].value{html,ansi}]

    
    -------------------------------------------------------------------------------------------------------------------------------------------------

*/


std::string logbook::_dbname = "logbook";
xio::xdb::sqlitedb   logbook::_dbdata;


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
