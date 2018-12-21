#pragma once


#include <map>
#include <fstream>
#include <time.h>
#include <utility>
#include <vector>

#include <xio++/string_t.hpp>
#include <xio++/time_stamp.hpp>


#include <stack>

#ifdef _WIN32
    #define __PRETTY_FUNCTION__ __FUNCSIG__
    #define __FUNCTSIG__ __FUNCSIG__
#endif

// Note This logger will not be refactored completely. (Serge Lussier, bretzelus, bretzel, lussier.serge@gmail.com) 
namespace xio {


  

class xio_api logger
{

    // static member attributes:
    //  ➜ ⬅ ⬆ ⬇ ⬈ ⬉ ⬊ ⬋
    static string_t   		m_log_header; ///< text contents of the logger header at the openning. cleared as soon as the contents is writen into the file.
    static wchar_t      m_arrows[8];
    static string_t::list	m_objects_prefix_table;  ///< configured text attributes/style for each of the object type ...
    static string_t          m_filename;    ///< ...filename - empty if stdout is to be used.
    static std::ofstream    m_file_output; ///< the instance of file.
    static std::ostream*    m_file;        ///< pointer to the ofstream { stdout; or file stream}.

    static std::stack<int>  m_spanstack;
    // some date_time object/saved format
    static string_t         m_tstamp;      ///< holds the configured date-time format for normal line ouputs.
    static bool             m_stdout;      ///< if i am unable to find the way to assign stdout to m_file_output, a flag set to true...
    static bool             m_code_mode;   ///< code-mode;
    static bool            m_closed;       ///< this logger is closed. ( prevent leaks. ) 
    static std::map<int32_t, std::string> m_code_type_dictionary;

    string_t                 m_text = "";   ///< temporary inner std::string for computing and works.
    static string_t::list	m_attr_table;  ///< text attributes/style list built from the mode;

                                            // the elements that can have specific attributes/style :
    static string_t::list	m_type_table;  ///< message types list including their attributes/style + title.
    static std::string      m_eol;         ///< line break for the mode.

                                            // __function__ ; __pretty_funcation__ ; __line__ ; __file are aliased in macros at the bottom of this file.
    static string_t          m_fn_prefix;   ///< saved text attributes/style for the function name { __function__ }.
    static string_t          m_long_fn_prefix; ///< saved text attributes/style for the long function name. { __pretty_function__ }.
    static string_t          m_date_time_style; ///< saved text attributes/style for the date time;
    static int              m_indent;

public:
    enum class Arrow : int {
        ArrowRight,
        ArrowLeft,
        ArrowUp,
        ArrowDown,
        ArrowUpRight,
        ArrowUpLeft,
        ArrowDownRight,
        ArrowDownLeft
    };


    enum Mode {
        None = 0,   ///< unformated.
        Plain,      ///< unformated.
        Ansi,       ///< format ainsi standard.
        Html,       ///< format html.
        Nsml,       ///< format nsml  ( ncurses ).
        Rtf         ///< Format Rich Text Format.
    };

    /*!
    *        @brief the type/class of messsages.
    */
    enum Type { Info = 0, Err, Critical, Fatal, Debug, Excep, Notice, Warning, Succes };

    /*!
    * @brief text styles enums.
    *      &eacute;num&eacute;ration des attributs du texte :
    */
    enum Attr {
        /* colors*/ Black = 0, Red = 1, Green = 2, Amber = 3, Blue = 4, Magenta = 5, Cyan = 6, HGray, DGray, HRed, HGreen, Yellow, HBlue, HMagenta, HCyan, White,
        /* reset */ Reset, ///< remise-&agrave;-z&eacute;ro des attributs.
        /* html: */ Italic, Strong, Sup, _Sup, Sub, _Sub, Pre, EndPre ///< styles suppl&eacute;mentaires du texte au format `html`
    };

    /*!
    *        @brief end of line
    *              fin de ligne :
    */
    enum Tail {
        End = 0,  ///< end of line this object triggers commit; fin de ligne et de la ligne du logger - &eacute;criture dans le fichier de sortie.
        LF      ///< end of line - this object doesn't triggers commit. ; fin de ligne mais pas de fermeture de la ligne du logger.

    };



    /*!
    *        @brief objects enums - see their description.
    *      &eacute;num&eacute;ration des classes d'objets - voir leur descriptions.
    */
    enum Object {
        FN = 0,               ///< appends the function's short name ( __function__ ) in place ...
        FName,              ///< long function name { __pretty_function__ }
        Stamp,              ///<  output contents of m_tstamp @see std::string::date_time(const std::string& format) ; contenu de cLogger::m_tstamp
        LDay,               ///< long day name ; nom complet du jour
        Day,                ///< day number {week} ; num&eacute;ro du jour dans le mois
        YDay,               ///< day number {year} ; num&eacute;ro du jour dans l'annee
        MonthNum,          ///< month number; num&eacute;ro du mois
        MonthNameShort,   ///< month short name ; nom du mois court
        MonthNameLong,    ///< month long name ;nom du mois long (complet)
        YearShort,         ///< short year number { without century } ; ann&eacute;e, sans le si&egrave;cle
        YearLong,          ///< year including centry ; ann&eacute;e incluant le si&egrave;cle
        Hour,               ///< hour 0-12 style ; heure notation 1-12 + am|pm
        AmPm,               ///< hour 0-12 style ; heure notation 1-12 + am|pm
        Hour24,             ///< hour 24 style ; heure notation 24 heures
        THour,              ///< %T 24h format: %H:%M:%S 
        Minute,             ///< minutes
        Seconds,            ///< seconds ; secondes
        Inc,                ///< indentation - increment; incr&eacute;menter l'indentation        dec,                ///< indentation -  decrement ; d&eacute;cr&eacute;menter l'indentation
        Code,               ///< enters {basic, supported} code mode (highlighting) using ctext.
        EndCode,            ///< leave code-mode.
        Para,
        EndPara,
        Nil                 ///< null object; used as counting/bound/end value ; nulle
    };

    logger();
    logger(const logger& l);
    logger(const char* a_function_name, int aline = 0, const char* filename = 0);
    logger(logger::Type atype, const char* a_function_name, int aline = 0, const char* filename = 0);




    virtual ~logger();
    logger& operator=(const logger& other);
    bool operator==(const logger& other) const;


    // date-time stamp configuration:
    static logger resetstamp(); // see descriptions in the .cc file ...

    logger& operator,(logger::Object dt_obj);
    logger& operator,(logger::Attr attr);
    template<typename t> logger& operator,(const t& c) {
        logger::m_tstamp += c;
        return *this;
    }
    // ---------------------------------
    wchar_t operator[](logger::Arrow ar);
    // output operators
    logger& operator << (logger::Object obj);
    logger& operator << (logger::Tail til);
    logger& operator << (logger::Attr attr);
    logger& operator << (logger::Type t);
    logger& operator << (logger::Arrow arr);
    template<typename t> logger& operator << (const t& acontents) {
        if (m_code_mode)
            ;// return *this; //return highlightchunk(acontents);

        m_text << acontents;
        return *this;
    }
    // ----------------------------------

    //    logger& highlightchunk(const std::string& acontents);

    // initialization methods:
    static int init(logger::Mode mode, const std::string& lname, bool use_pid);
    static void setfile(const std::string& afilename);
    //-----------------------------------

    logger& pushattr();
    logger& popattr();

    std::string operator[](logger::Attr attr);
    std::string operator[](logger::Type tclass);
    std::string operator[](logger::Object obj);

    std::string text() const { return m_text(); }
    static bool close();
    static std::string attribute(logger::Attr att);
    static std::string brk() { return logger::m_eol; }
    static std::string text(logger::Object o);
    static std::string text(logger::Attr o);
    static std::string startcode();
    static std::string codeend();
    static std::string attrreset();
protected:
    static logger::Mode m_mode;      ///< mode...

private:
    static void _initplain();
    static void _initansi();
    static void _inithtml();
    static void _initsml();
};

}
#define Ends xio::logger::End
#define LEnd xio::logger::End
#define BR   xio::logger::LF

// Unless we prefer to use explicitly "logger(__PRETTY_FUNCTION__, __LINE__, __FILE__) << ..." , why not go lazy and reduce typing with the macros below ?
//  -- Serge Lussier
// how to name those macros ...

//#ifdef _WIN32 // Quick check if gcc or not. ( CLang++, MSVC++, Intel C++, Unix C++... ) 
//    #define  __PRETTY_FUNCTION__ __FUNCSIG__
//#endif

//#define jfdebug       logger(logger::Debug   ,__FUNCTION__,__LINE__)
//#define jfndebug      logger(logger::Debug   ,__PRETTY_FUNCTION__,__LINE__)
//#define jcdebug       logger(logger::Debug   ,0,0)
//#define jfinfo        logger(logger::Info    ,__FUNCTION__,__LINE__,0)
//#define jfninfo       logger(logger::Info    ,__PRETTY_FUNCTION__,__LINE__,0)
//#define jcinfo        logger(logger::Info    ,0,0)
//#define jferror       logger(logger::Err     ,__FUNCTION__,__LINE__,0)
//#define jfnerror      logger(logger::Err     ,__PRETTY_FUNCTION__,__LINE__,0)
//#define jcerror       logger(logger::Err     ,0,0)
//#define jffatal       logger(logger::Fatal   ,__FUNCTION__,__LINE__,0)
//#define jfnfatal      logger(logger::Fatal   ,__PRETTY_FUNCTION__,__LINE__,0)
//#define jcfatal       logger(logger::Fatal   ,0,0)
//#define jfcritical    logger(logger::Critical,__FUNCTION__,__LINE__,0)
//#define jfncritical   logger(logger::Critical,__PRETTY_FUNCTION__,__LINE__,0)
//#define jccritical    logger(logger::Critical,0,0)
//#define jfnexcep      logger(logger::Excep   ,__PRETTY_FUNCTION__,__LINE__,0)
//#define jfexcep       logger(logger::Excep   ,__FUNCTION__,__LINE__,0)
//#define jcexcep       logger(logger::Excep   ,0,0)
//#define jfnnote       logger(logger::Notice  ,__PRETTY_FUNCTION__,__LINE__,0)
//#define jfnote        logger(logger::Notice  ,__FUNCTION__,__LINE__,0)
//#define jcnote        logger(logger::Notice  ,0,0)
//#define jfwarning     logger(logger::Warning ,__FUNCTION__,__LINE__,0)
//#define jfnwarning    logger(logger::Warning ,__PRETTY_FUNCTION__,__LINE__,0)
//#define jcwarning     logger(logger::Warning ,0,0)
//#define jfsucces      logger(logger::Succes ,__FUNCTION__,__LINE__,0)
//#define jfnsucces     logger(logger::Succes ,__PRETTY_FUNCTION__,__LINE__,0)
//#define jcsucces      logger(logger::Succes ,0,0)
//
//// ---------------------------------------------------------------------------------------------------------
//#define jffdebug       logger(logger::Debug   ,__FUNCTION__,__LINE__,__FILE__)
//#define jffndebug      logger(logger::Debug   ,__PRETTY_FUNCTION__,__LINE__,__FILE__)
//#define jfcdebug       logger(logger::Debug   ,0,0)
//#define jffinfo        logger(logger::Info    ,__FUNCTION__,__LINE__,__FILE__)
//#define jffninfo       logger(logger::Info    ,__PRETTY_FUNCTION__,__LINE__,__FILE__)
//#define jfcinfo        logger(logger::Info    ,0,0)
//#define jfferror       logger(logger::Err     ,__FUNCTION__,__LINE__,__FILE__)
//#define jffnerror      logger(logger::Err     ,__PRETTY_FUNCTION__,__LINE__,__FILE__)
//#define jfcerror       logger(logger::Err     ,__PRETTY_FUNCTION__,__FILE__)
//#define jfffatal       logger(logger::Fatal   ,__FUNCTION__,__LINE__,0)
//#define jffnfatal      logger(logger::Fatal   ,__PRETTY_FUNCTION__,__LINE__,0)
//#define jfcfatal       logger(logger::Fatal   ,0,0)
//#define jffcritical    logger(logger::Critical,__FUNCTION__,__LINE__,0)
//#define jffncritical   logger(logger::Critical,__PRETTY_FUNCTION__,__LINE__,0)
//#define jfccritical    logger(logger::Critical,0,0)
//#define jffnexcep      logger(logger::Excep   ,__PRETTY_FUNCTION__,__LINE__,0)
//#define jffexcep       logger(logger::Excep   ,__FUNCTION__,__LINE__,0)
//#define jfcexcep       logger(logger::Excep   ,0,0)
//#define jffnnote       logger(logger::Notice  ,__PRETTY_FUNCTION__,__LINE__,0)
//#define jffnote        logger(logger::Notice  ,__FUNCTION__,__LINE__,0)
//#define jfcnote        logger(logger::Notice  ,0,0)
//#define jffwarning     logger(logger::Warning ,__FUNCTION__,__LINE__,0)
//#define jffnwarning    logger(logger::Warning ,__PRETTY_FUNCTION__,__LINE__,0)
//#define jfcwarning     logger(logger::Warning ,0,0)
//#define jffsucces      logger(logger::Succes ,__FUNCTION__,__LINE__,0)
//#define jffnsucces     logger(logger::Succes ,__PRETTY_FUNCTION__,__LINE__,0)
//#define jlfunc         logger(logger::Succes ,__PRETTY_FUNCTION__,__LINE__,0) << cwhite << "leaving -" << logger::end;
//#define jfcsucces      logger(logger::Succes ,0,0)

#define LogDebugFn     xio::logger(xio::logger::Debug   ,__FUNCTION__,__LINE__) // logdebugfn
#define LogDebugPFn    xio::logger(xio::logger::Debug   ,__PRETTY_FUNCTION__,__LINE__)
#define LogDebug       xio::logger(xio::logger::Debug   ,0,0)
#define LogInfoFn      xio::logger(xio::logger::Info    ,__FUNCTION__,__LINE__,0)
#define LogInfoPFn     xio::logger(xio::logger::Info    ,__PRETTY_FUNCTION__,__LINE__,0)
#define LogInfo        xio::logger(xio::logger::Info    ,0,0)
#define LogErrorFn     xio::logger(xio::logger::Err     ,__FUNCTION__,__LINE__,0)
#define LogErrorPfn    xio::logger(xio::logger::Err     ,__PRETTY_FUNCTION__,__LINE__,0)
#define LogError       xio::logger(xio::logger::Err     ,0,0)
#define LogFatalFn     xio::logger(xio::logger::Fatal   ,__FUNCTION__,__LINE__,0)
#define LogFatalPFn    xio::logger(xio::logger::Fatal   ,__PRETTY_FUNCTION__,__LINE__,0)
#define LogFatal       xio::logger(xio::logger::Fatal   ,0,0)
#define LogCriticalFn  xio::logger(xio::logger::Critical,__FUNCTION__,__LINE__,0)
#define LogCriticalPFn xio::logger(xio::logger::Critical,__PRETTY_FUNCTION__,__LINE__,0)
#define LogCritical    xio::logger(xio::logger::Critical,0,0)
#define LogExcepPFn    xio::logger(xio::logger::Excep   ,__PRETTY_FUNCTION__,__LINE__,0)
#define LogExcepFn     xio::logger(xio::logger::Excep   ,__FUNCTION__,__LINE__,0)
#define LogExcep       xio::logger(xio::logger::Excep   ,0,0)
#define LogNoticePFn   xio::logger(xio::logger::Notice  ,__PRETTY_FUNCTION__,__LINE__,0)
#define LogNoticeFn    xio::logger(xio::logger::Notice  ,__FUNCTION__,__LINE__,0)
#define LogNotice      xio::logger(xio::logger::Notice  ,0,0)
#define LogWarningFn   xio::logger(xio::logger::Warning ,__FUNCTION__,__LINE__,0)
#define LogWarningPFn  xio::logger(xio::logger::Warning ,__PRETTY_FUNCTION__,__LINE__,0)
#define LogWarning     xio::logger(xio::logger::Warning ,0,0)
#define LogSuccessFn   xio::logger(xio::logger::Succes ,__FUNCTION__,__LINE__,0)
#define LogSuccessPFn  xio::logger(xio::logger::Succes ,__PRETTY_FUNCTION__,__LINE__,0)
#define LogSuccess     xio::logger(xio::logger::Succes ,0,0)
#define LogNH          xio::logger()
// ------Extended logger infos (+ file )-------------------------------------------------------------------
#define LogDebugFnX    xio::logger(xio::logger::Debug   ,__FUNCTION__,__LINE__,__FILE__)
#define LogDebugPFnX   xio::logger(xio::logger::Debug   ,__PRETTY_FUNCTION__,__LINE__,__FILE__)
#define LogInfoFnX     xio::logger(xio::logger::Info    ,__FUNCTION__,__LINE__,__FILE__)
#define LogInfoPFnX    xio::logger(xio::logger::Info    ,__PRETTY_FUNCTION__,__LINE__,__FILE__)
#define LogErrorFnX    xio::logger(xio::logger::Err     ,__FUNCTION__,__LINE__,__FILE__)
#define LogErrorPfnX   xio::logger(xio::logger::Err     ,__PRETTY_FUNCTION__,__LINE__,__FILE__)
#define LogErrorX      xio::logger(xio::logger::Err     ,__PRETTY_FUNCTION__,__FILE__)
#define LogSuccessEnd  xio::logger(xio::logger::Succes ,__PRETTY_FUNCTION__,__LINE__,0) << xio::logger::White << "leaving -" << Ends;

#define logdebugfn     xio::logger(xio::logger::Debug   ,__FUNCTION__) // logdebugfn
#define logdebugpfn    xio::logger(xio::logger::Debug   ,__PRETTY_FUNCTION__)
#define logdebug       xio::logger(xio::logger::Debug   ,0,0)
#define loginfofn      xio::logger(xio::logger::Info    ,__FUNCTION__)
#define loginfopfn     xio::logger(xio::logger::Info    ,__PRETTY_FUNCTION__,__LINE__,0)
#define loginfo        xio::logger(xio::logger::Info    ,0,0)
#define logerrorfn     xio::logger(xio::logger::Err     ,__FUNCTION__)
#define logerrorpfn    xio::logger(xio::logger::Err     ,__PRETTY_FUNCTION__,__LINE__,0)
#define logerror       xio::logger(xio::logger::Err     ,0,0)
#define logfatalfn     xio::logger(xio::logger::Fatal   ,__FUNCTION__,__LINE__,0)
#define logfatalpfn    xio::logger(xio::logger::Fatal   ,__PRETTY_FUNCTION__,__LINE__,0)
#define logfatal       xio::logger(xio::logger::Fatal   ,0,0)
#define logcriticalfn  xio::logger(xio::logger::Critical,__FUNCTION__,__LINE__,0)
#define logcriticalpfn xio::logger(xio::logger::Critical,__PRETTY_FUNCTION__,__LINE__,0)
#define logcritical    xio::logger(xio::logger::Critical,0,0)
#define logexceppfn    xio::logger(xio::logger::Excep   ,__PRETTY_FUNCTION__,__LINE__,0)
#define logexcepfn     xio::logger(xio::logger::Excep   ,__FUNCTION__)
#define logexcep       xio::logger(xio::logger::Excep   ,0,0)
#define lognoticepfn   xio::logger(xio::logger::Notice  ,__PRETTY_FUNCTION__,__LINE__,0)
#define lognoticefn    xio::logger(xio::logger::Notice  ,__FUNCTION__)
#define lognotice      xio::logger(xio::logger::Notice  ,0,0)
#define logwarningfn   xio::logger(xio::logger::Warning ,__FUNCTION__)
#define logwarningpfn  xio::logger(xio::logger::Warning ,__PRETTY_FUNCTION__,__LINE__,0)
#define logwarning     xio::logger(xio::logger::Warning ,0,0)
#define logsuccessfn   xio::logger(xio::logger::Succes ,__FUNCTION__)
#define logsuccesspfn  xio::logger(xio::logger::Succes ,__PRETTY_FUNCTION__,__LINE__,0)
#define logsuccess     xio::logger(xio::logger::Succes ,0,0)
#define lognh          xio::logger()
// ------Extended logger infos (+ file )-------------------------------------------------------------------
#define logdebugfnx    xio::logger(xio::logger::Debug   ,__FUNCTION__,__LINE__,__FILE__)
#define logdebugpfnx   xio::logger(xio::logger::Debug   ,__PRETTY_FUNCTION__,__LINE__,__FILE__)
#define loginfofnx     xio::logger(xio::logger::Info    ,__FUNCTION__,__LINE__,__FILE__)
#define loginfopfnx    xio::logger(xio::logger::Info    ,__PRETTY_FUNCTION__,__LINE__,__FILE__)
#define logerrorfnx    xio::logger(xio::logger::Err     ,__FUNCTION__,__LINE__,__FILE__)
#define logerrorpfnx   xio::logger(xio::logger::Err     ,__PRETTY_FUNCTION__,__LINE__,__FILE__)
#define logerrorx      xio::logger(xio::logger::Err     ,__PRETTY_FUNCTION__,__FILE__)
#define logsuccessend  xio::logger(xio::logger::Succes ,__PRETTY_FUNCTION__,__LINE__,0) << xio::logger::White << "leaving -" << Ends;
    
