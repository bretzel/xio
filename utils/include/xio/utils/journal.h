//
// Created by steacc::utilssier on 19-11-20.
//

//#ifndef teacc::utils_JOURNAL_H
//#define teacc::utils_JOURNAL_H
#pragma once

#include <xio/utils/xstr.h>
#include <map>
#include <fstream>
#include <ctime>
#include <utility>
#include <vector>
#include <stack>


namespace teacc::utils{

class XIOUTILS_API journal
{
    // static member attributes:
    //  ➜ ⬅ ⬆ ⬇ ⬈ ⬉ ⬊ ⬋
    static xstr   		m_log_header; ///< text contents of the journal header at the openning. cleared as soon as the contents is writen into the file.
    static wchar_t          m_arrows[8];
    static xstr::collection	m_objects_prefix_table;  ///< configured text attributes/style for each of the object type ...
    static xstr           m_filename;    ///< ...filename - empty if stdout is to be used.
    static std::ofstream    m_file_output; ///< the instance of file.
    static std::ostream*    m_file;        ///< pointer to the ofstream { stdout; or file stream}.

    static std::stack<int>  m_spanstack;
    // some date_time object/saved format
    static xstr           m_tstamp;      ///< holds the configured date-time format for normal line ouputs.
    static bool             m_stdout;      ///< if i am unable to find the way to assign stdout to m_file_output, a flag set to true...
    static bool             m_code_mode;   ///< code-mode;
    static bool             m_closed;       ///< this journal is closed. ( prevent leaks. )
    static std::map<int32_t, std::string> m_code_type_dictionary;

    xstr                  m_text = "";   ///< temporary inner std::string for computing and works.
    static xstr::collection	m_attr_table;  ///< text attributes/style list built from the mode;

    // the elements that can have specific attributes/style :
    static xstr::collection	m_type_table;  ///< message types list including their attributes/style + title.
    static std::string      m_eol;         ///< line break for the mode.

    // __function__ ; __pretty_funcation__ ; __line__ ; __file are aliased in macros at the bottom of this file.
    static xstr           m_fn_prefix;   ///< saved text attributes/style for the function name { __function__ }.
    static xstr           m_long_fn_prefix; ///< saved text attributes/style for the long function name. { __pretty_function__ }.
    static xstr           m_date_time_style; ///< saved text attributes/style for the date time;
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
        End = 0,  ///< end of line this object triggers commit; fin de ligne et de la ligne du journal - &eacute;criture dans le fichier de sortie.
        LF      ///< end of line - this object doesn't triggers commit. ; fin de ligne mais pas de fermeture de la ligne du journal.

    };



    /*!
    *        @brief objects enums - see their description.
    *      &eacute;num&eacute;ration des classes d'objets - voir leur descriptions.
    */
    enum Object {
        FN = 0,               ///< appends the function's short name ( __function__ ) in place ...
        FName,              ///< long function name { __pretty_function__ }
        Stamp,              ///<  output contents of m_tstamp @see std::string::date_time(const std::string& format) ; contenu de cJournal::m_tstamp
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

    journal();
    journal(const journal& l);
    journal(const char* a_function_name, int aline = 0, const char* filename = 0);
    journal(journal::Type atype, const char* a_function_name, int aline = 0, const char* filename = 0);




    virtual ~journal();
    journal& operator=(const journal& other);
    bool operator==(const journal& other) const;


    // date-time stamp configuration:
    static journal resetstamp(); // see descriptions in the .cc file ...

    journal& operator,(journal::Object dt_obj);
    journal& operator,(journal::Attr attr);
    template<typename T> journal& operator,(const T& c) {
        //------------Disable Timestamp temorary:--------------------------
        if(true){
            journal::m_tstamp += c;
            return *this;
        }
        //-----------------------------------------------------------------
        m_text += ',' + c;
        return *this;
    }
    // ---------------------------------
    wchar_t operator[](journal::Arrow ar);
    // output operators
    journal& operator << (journal::Object obj);
    journal& operator << (journal::Tail til);
    journal& operator << (journal::Attr attr);
    journal& operator << (journal::Type t);
    journal& operator << (journal::Arrow arr);
    template<typename T> journal& operator << (const T& a_contents) {
        //if (m_code_mode)
        //    ;// return *this; //return highlightchunk(acontents);
        
        m_text << a_contents;
        //(*this) << Attr::Reset;
        return *this;
    }
    // ----------------------------------

    //    journal& highlightchunk(const std::string& acontents);

    // initialization methods:
    static int init(journal::Mode mode, const std::string& lname, bool use_pid);
    static void setfile(const std::string& afilename);
    //-----------------------------------

    journal& pushattr();
    journal& popattr();

    std::string operator[](journal::Attr attr);
    std::string operator[](journal::Type tclass);
    std::string operator[](journal::Object obj);

    std::string text() const { return m_text(); }
    static bool close();
    static std::string attribute(journal::Attr att);
    static std::string brk() { return journal::m_eol; }
    static std::string text(journal::Object o);
    static std::string text(journal::Attr o);
    static std::string startcode();
    static std::string codeend();
    static std::string attrreset();
protected:
    static journal::Mode m_mode;      ///< mode...

private:
    static void _initplain();
    static void _initansi();
    static void _inithtml();
    static void _initsml();

};





}


#define Ends teacc::utils::journal::End
#define ends teacc::utils::journal::End
#define LEnd teacc::utils::journal::End
#define BR   teacc::utils::journal::LF




// Short (Test)
#define DebugFn     teacc::utils::journal(teacc::utils::journal::Debug   ,__FUNCTION__,__LINE__) // logdebugfn
#define DebugPFn    teacc::utils::journal(teacc::utils::journal::Debug   ,__PRETTY_FUNCTION__,__LINE__)
//#define Debug       teacc::utils::journal(teacc::utils::journal::Debug   ,0,0)
#define InfoFn      teacc::utils::journal(teacc::utils::journal::Info    ,__FUNCTION__,__LINE__,0)
#define InfoPFn     teacc::utils::journal(teacc::utils::journal::Info    ,__PRETTY_FUNCTION__,__LINE__,0)
//#define Info        teacc::utils::journal(teacc::utils::journal::Info    ,0,0)
#define ErrorFn     teacc::utils::journal(teacc::utils::journal::Err     ,__FUNCTION__,__LINE__,0)
#define ErrorPfn    teacc::utils::journal(teacc::utils::journal::Err     ,__PRETTY_FUNCTION__,__LINE__,0)
// -----------------------
#define LogDebugFn     teacc::utils::journal(teacc::utils::journal::Debug   ,__FUNCTION__,__LINE__) // logdebugfn
#define LogDebugPFn    teacc::utils::journal(teacc::utils::journal::Debug   ,__PRETTY_FUNCTION__,__LINE__)
#define LogDebug       teacc::utils::journal(teacc::utils::journal::Debug   ,0,0)
#define LogInfoFn      teacc::utils::journal(teacc::utils::journal::Info    ,__FUNCTION__,__LINE__,0)
#define LogInfoPFn     teacc::utils::journal(teacc::utils::journal::Info    ,__PRETTY_FUNCTION__,__LINE__,0)
#define LogInfo        teacc::utils::journal(teacc::utils::journal::Info    ,0,0)
#define LogErrorFn     teacc::utils::journal(teacc::utils::journal::Err     ,__FUNCTION__,__LINE__,0)
#define LogErrorPfn    teacc::utils::journal(teacc::utils::journal::Err     ,__PRETTY_FUNCTION__,__LINE__,0)
#define LogError       teacc::utils::journal(teacc::utils::journal::Err     ,0,0)
#define LogFatalFn     teacc::utils::journal(teacc::utils::journal::Fatal   ,__FUNCTION__,__LINE__,0)
#define LogFatalPFn    teacc::utils::journal(teacc::utils::journal::Fatal   ,__PRETTY_FUNCTION__,__LINE__,0)
#define LogFatal       teacc::utils::journal(teacc::utils::journal::Fatal   ,0,0)
#define LogCriticalFn  teacc::utils::journal(teacc::utils::journal::Critical,__FUNCTION__,__LINE__,0)
#define LogCriticalPFn teacc::utils::journal(teacc::utils::journal::Critical,__PRETTY_FUNCTION__,__LINE__,0)
#define LogCritical    teacc::utils::journal(teacc::utils::journal::Critical,0,0)
#define LogExcepPFn    teacc::utils::journal(teacc::utils::journal::Excep   ,__PRETTY_FUNCTION__,__LINE__,0)
#define LogExcepFn     teacc::utils::journal(teacc::utils::journal::Excep   ,__FUNCTION__,__LINE__,0)
#define LogExcep       teacc::utils::journal(teacc::utils::journal::Excep   ,0,0)
#define LogNoticePFn   teacc::utils::journal(teacc::utils::journal::Notice  ,__PRETTY_FUNCTION__,__LINE__,0)
#define LogNoticeFn    teacc::utils::journal(teacc::utils::journal::Notice  ,__FUNCTION__,__LINE__,0)
#define LogNotice      teacc::utils::journal(teacc::utils::journal::Notice  ,0,0)
#define LogWarningFn   teacc::utils::journal(teacc::utils::journal::Warning ,__FUNCTION__,__LINE__,0)
#define LogWarningPFn  teacc::utils::journal(teacc::utils::journal::Warning ,__PRETTY_FUNCTION__,__LINE__,0)
#define LogWarning     teacc::utils::journal(teacc::utils::journal::Warning ,0,0)
#define LogSuccessFn   teacc::utils::journal(teacc::utils::journal::Succes ,__FUNCTION__,__LINE__,0)
#define LogSuccessPFn  teacc::utils::journal(teacc::utils::journal::Succes ,__PRETTY_FUNCTION__,__LINE__,0)
#define LogSuccess     teacc::utils::journal(teacc::utils::journal::Succes ,0,0)
#define LogNH          teacc::utils::journal()
// ------Extended logger infos (+ file )-------------------------------------------------------------------
#define LogDebugFnX    teacc::utils::journal(teacc::utils::journal::Debug   ,__FUNCTION__,__LINE__,__FILE__)
#define LogDebugPFnX   teacc::utils::journal(teacc::utils::journal::Debug   ,__PRETTY_FUNCTION__,__LINE__,__FILE__)
#define LogInfoFnX     teacc::utils::journal(teacc::utils::journal::Info    ,__FUNCTION__,__LINE__,__FILE__)
#define LogInfoPFnX    teacc::utils::journal(teacc::utils::journal::Info    ,__PRETTY_FUNCTION__,__LINE__,__FILE__)
#define LogErrorFnX    teacc::utils::journal(teacc::utils::journal::Err     ,__FUNCTION__,__LINE__,__FILE__)
#define LogErrorPfnX   teacc::utils::journal(teacc::utils::journal::Err     ,__PRETTY_FUNCTION__,__LINE__,__FILE__)
#define LogErrorX      teacc::utils::journal(teacc::utils::journal::Err     ,__PRETTY_FUNCTION__,__FILE__)
#define LogSuccessEnd  teacc::utils::journal(teacc::utils::journal::Succes ,__PRETTY_FUNCTION__,__LINE__,0) << teacc::utils::journal::White << "leaving -" << Ends;

#define logdebugfn     teacc::utils::journal(teacc::utils::journal::Debug   ,__FUNCTION__) // logdebugfn
#define logdebugpfn    teacc::utils::journal(teacc::utils::journal::Debug   ,__PRETTY_FUNCTION__)
#define logdebug       teacc::utils::journal(teacc::utils::journal::Debug   ,0,0)
#define loginfofn      teacc::utils::journal(teacc::utils::journal::Info    ,__FUNCTION__)
#define loginfopfn     teacc::utils::journal(teacc::utils::journal::Info    ,__PRETTY_FUNCTION__,__LINE__,0)
#define loginfo        teacc::utils::journal(teacc::utils::journal::Info    ,0,0)
#define logerrorfn     teacc::utils::journal(teacc::utils::journal::Err     ,__FUNCTION__)
#define logerrorpfn    teacc::utils::journal(teacc::utils::journal::Err     ,__PRETTY_FUNCTION__,__LINE__,0)
#define logerror       teacc::utils::journal(teacc::utils::journal::Err     ,0,0)
#define logfatalfn     teacc::utils::journal(teacc::utils::journal::Fatal   ,__FUNCTION__,__LINE__,0)
#define logfatalpfn    teacc::utils::journal(teacc::utils::journal::Fatal   ,__PRETTY_FUNCTION__,__LINE__,0)
#define logfatal       teacc::utils::journal(teacc::utils::journal::Fatal   ,0,0)
#define logcriticalfn  teacc::utils::journal(teacc::utils::journal::Critical,__FUNCTION__,__LINE__,0)
#define logcriticalpfn teacc::utils::journal(teacc::utils::journal::Critical,__PRETTY_FUNCTION__,__LINE__,0)
#define logcritical    teacc::utils::journal(teacc::utils::journal::Critical,0,0)
#define logexceppfn    teacc::utils::journal(teacc::utils::journal::Excep   ,__PRETTY_FUNCTION__,__LINE__,0)
#define logexcepfn     teacc::utils::journal(teacc::utils::journal::Excep   ,__FUNCTION__)
#define logexcep       teacc::utils::journal(teacc::utils::journal::Excep   ,0,0)
#define lognoticepfn   teacc::utils::journal(teacc::utils::journal::Notice  ,__PRETTY_FUNCTION__,__LINE__,0)
#define lognoticefn    teacc::utils::journal(teacc::utils::journal::Notice  ,__FUNCTION__)
#define lognotice      teacc::utils::journal(teacc::utils::journal::Notice  ,0,0)
#define logwarningfn   teacc::utils::journal(teacc::utils::journal::Warning ,__FUNCTION__)
#define logwarningpfn  teacc::utils::journal(teacc::utils::journal::Warning ,__PRETTY_FUNCTION__,__LINE__,0)
#define logwarning     teacc::utils::journal(teacc::utils::journal::Warning ,0,0)
#define logsuccessfn   teacc::utils::journal(teacc::utils::journal::Succes ,__FUNCTION__)
#define logsuccesspfn  teacc::utils::journal(teacc::utils::journal::Succes ,__PRETTY_FUNCTION__,__LINE__,0)
#define logsuccess     teacc::utils::journal(teacc::utils::journal::Succes ,0,0)
#define lognh          teacc::utils::journal()
// ------Extended logger infos (+ file )-------------------------------------------------------------------
#define logdebugfnx    teacc::utils::journal(teacc::utils::journal::Debug   ,__FUNCTION__,__LINE__,__FILE__)
#define logdebugpfnx   teacc::utils::journal(teacc::utils::journal::Debug   ,__PRETTY_FUNCTION__,__LINE__,__FILE__)
#define loginfofnx     teacc::utils::journal(teacc::utils::journal::Info    ,__FUNCTION__,__LINE__,__FILE__)
#define loginfopfnx    teacc::utils::journal(teacc::utils::journal::Info    ,__PRETTY_FUNCTION__,__LINE__,__FILE__)
#define logerrorfnx    teacc::utils::journal(teacc::utils::journal::Err     ,__FUNCTION__,__LINE__,__FILE__)
#define logerrorpfnx   teacc::utils::journal(teacc::utils::journal::Err     ,__PRETTY_FUNCTION__,__LINE__,__FILE__)
#define logerrorx      teacc::utils::journal(teacc::utils::journal::Err     ,__PRETTY_FUNCTION__,__FILE__)
#define logsuccessend  teacc::utils::journal(teacc::utils::journal::Succes ,__PRETTY_FUNCTION__,__LINE__,0) << teacc::utils::journal::White << "leaving -" << Ends;

// -------- Most shorten aliases -----------------------------------------------------------
//#define fndebug       teacc::utils::journal(teacc::utils::journal::Debug   ,__FUNCTION__) // logdebugfn
//#define fdebug       teacc::utils::journal(teacc::utils::journal::Debug   ,__PRETTY_FUNCTION__)
//#define dlog          teacc::utils::journal(teacc::utils::journal::Debug   ,0,0)
//#define finfo         teacc::utils::journal(teacc::utils::journal::Info    ,__FUNCTION__)
//#define flninfo       teacc::utils::journal(teacc::utils::journal::Info    ,__PRETTY_FUNCTION__,__LINE__,0)
//#define infolog       teacc::utils::journal(teacc::utils::journal::Info    ,0,0)
//#define errlog        teacc::utils::journal(teacc::utils::journal::Err     ,__FUNCTION__)
//#define flnerror      teacc::utils::journal(teacc::utils::journal::Err     ,__PRETTY_FUNCTION__,__LINE__,0)
//#define  teacc::utils::journal(teacc::utils::journal::Err     ,0,0)
//#define  teacc::utils::journal(teacc::utils::journal::Fatal   ,__FUNCTION__,__LINE__,0)
//#define  teacc::utils::journal(teacc::utils::journal::Fatal   ,__PRETTY_FUNCTION__,__LINE__,0)
//#define  teacc::utils::journal(teacc::utils::journal::Fatal   ,0,0)
//#define  teacc::utils::journal(teacc::utils::journal::Critical,__FUNCTION__,__LINE__,0)
//#define  teacc::utils::journal(teacc::utils::journal::Critical,__PRETTY_FUNCTION__,__LINE__,0)
//#define  teacc::utils::journal(teacc::utils::journal::Critical,0,0)
//#define  teacc::utils::journal(teacc::utils::journal::Excep   ,__PRETTY_FUNCTION__,__LINE__,0)
//#define  teacc::utils::journal(teacc::utils::journal::Excep   ,__FUNCTION__)
//#define  teacc::utils::journal(teacc::utils::journal::Excep   ,0,0)
//#define  teacc::utils::journal(teacc::utils::journal::Notice  ,__PRETTY_FUNCTION__,__LINE__,0)
//#define  teacc::utils::journal(teacc::utils::journal::Notice  ,__FUNCTION__)
//#define  teacc::utils::journal(teacc::utils::journal::Notice  ,0,0)
//#define  teacc::utils::journal(teacc::utils::journal::Warning ,__FUNCTION__)
//#define  teacc::utils::journal(teacc::utils::journal::Warning ,__PRETTY_FUNCTION__,__LINE__,0)
//#define  teacc::utils::journal(teacc::utils::journal::Warning ,0,0)
//#define  teacc::utils::journal(teacc::utils::journal::Succes ,__FUNCTION__)
//#define  teacc::utils::journal(teacc::utils::journal::Succes ,__PRETTY_FUNCTION__,__LINE__,0)
//#define  teacc::utils::journal(teacc::utils::journal::Succes ,0,0)
//#define  teacc::utils::journal()



namespace teacc::utils::log
{
    using color = teacc::utils::journal::Attr;
}

// #endif //teacc::utils_JOURNAL_H
