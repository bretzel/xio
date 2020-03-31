#pragma once

//#include <xio/logbook/logbookapi.h>
#include <xio/utils/xstr.h>
#include <stack>
#include <vector>
#include <xio/utils/expect>
#include <xio/logbook/colors.h>
#include <xio/utils/xchronos>
#include <xio/logbook/document_element.h>


namespace teacc::logbook
{




class logbook_api logfmt : public doc_element
{

};

class logbook_api logdebug : public logfmt
{};

class logbook_api loginfo : public logfmt
{};

class logbook_api logerror : public logfmt
{};

class logbook_api logfatal : public logfmt
{};

class logbook_api logwarning : public logfmt
{};

class logbook_api lognote : public logfmt
{};

class logbook_api logsucces : public logfmt
{};

class logbook_api logfail : public logfmt
{};


}
