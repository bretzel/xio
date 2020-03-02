//
// Created by bretzel on 20-01-27.
//

//#ifndef PLSC_CHRONOST_H
//#define PLSC_CHRONOST_H

#pragma once

#include <xio/utils/xstr.h>

#include <chrono>

namespace xio::utils
{
/*!
 * @brief Own time management class.
 * @author &copy; 2018, 2019, Serge Lussier teacc.club, lussier.serge@gmail.com
 *
 * @todo Satisfy my ambitions to implement \c chronos_t and \c date_t arithmetics just like that PHP lang. !! ouahahahah!
 *
 */

class XIOUTILS_API xchronos
{
    std::chrono::system_clock::time_point _stamp;
    std::time_t _clock = 0L;
    xstr _text;
    std::string _fmt = "%T";//"%H:%M:%S";

public:

    enum class object : uint8_t
    {
        sfn = 0,         ///< Appends the function's short name ( __FUNCTION__ ) in place ...
        lfn,             ///< Appends the long function's name { __PRETTY_FUNCTION__/ }.
        stamp,           ///< Appends output contents of m_tstamp @see std::string::date_time(const std::string& format) ; contenu de cJournal::m_tstamp
        lday,            ///< Appends day name long format; nom complet du jour
        day,             ///< day number {week} ; num&eacute;ro du jour dans le mois
        yday,            ///< day number {year} ; num&eacute;ro du jour dans l'annee
        nmo,             ///< month number; num&eacute;ro du mois
        smn,             ///< month short name ; nom du mois court
        lmn,             ///< month long name ;nom du mois long (complet)
        syn,             ///< short year number { without century } ; ann&eacute;e, sans le si&egrave;cle
        lyn,             ///< year including centry ; ann&eacute;e incluant le si&egrave;cle
        hour,            ///< hour 0-12 style ; heure notation 1-12 + am|pm
        ampm,            ///< hour 0-12 style ; heure notation 1-12 + am|pm
        hour24,          ///< hour 24 style ; heure notation 24 heures
        thour,           ///< %T 24h format: %H:%M:%S
        minute,          ///< minutes
        seconds,         ///< seconds ; secondes
        timeofday,          ///< Time Of Day.
    };
    
    xchronos() = default;
    xchronos(xchronos&& xs) noexcept;
    xchronos(const xchronos &xs);
    ~xchronos();
    xchronos &operator*();
    std::string text(const std::string &a_format_str = "");
    xchronos &operator=(xchronos &&xs) noexcept;
    xchronos &operator=(const xchronos &xs);
};


}

//#endif //LSC_CHRONOST_H
