/*=======================================================================\
#                                                                        $
#   Copyright (C) 2010 Nokia Corporation.                                $
#                                                                        $
#   Author: Ilya Dogolazky <ilya.dogolazky@nokia.com>                    $
#                                                                        $
#     This file is part of qmlog                                         $
#                                                                        $
#     qmlog is free software; you can redistribute it and/or modify      $
#     it under the terms of the GNU Lesser General Public License        $
#     version 2.1 as published by the Free Software Foundation.          $
#                                                                        $
#     qmlog is distributed in the hope that it will be useful, but       $
#     WITHOUT ANY WARRANTY;  without even the implied warranty  of       $
#     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.               $
#     See the GNU Lesser General Public License  for more details.       $
#                                                                        $
#   You should have received a copy of the GNU  Lesser General Public    $
#   License along with qmlog. If not, see http://www.gnu.org/licenses/   $
\_______________________________________________________________________*/

#define _BSD_SOURCE

#include <cassert>
#include "LoggerSettings.h"


LoggerSettings::LoggerSettings(int new_verbosity_level, int new_location_mask, int new_message_format)
  : verbosity_level(new_verbosity_level)
  , location_mask(new_location_mask)
  , message_format(new_message_format)
{
}

void LoggerSettings::setVerbosityLevel(int new_verbosity_level)
{
  //TODO insert assert here
  verbosity_level = new_verbosity_level;
}

void LoggerSettings::setLocationMask(int new_location_mask)
{
  //TODO insert assert here
  location_mask = new_location_mask;
}

void LoggerSettings::setMessageFormat(int new_message_format)
{
  //TODO insert assert here
  message_format = new_message_format;
}

bool LoggerSettings::isLogShown(int aLevel) const
{
  return aLevel <= verbosity_level;
}

bool LoggerSettings::isLocationShown(int aLevel) const
{
  return (1 << aLevel) & location_mask;
}

bool LoggerSettings::isMTimerMs() const
{
  return (message_format & EMTimerMs) && !(message_format & EMTimerNs);
}

bool LoggerSettings::isMTimerNs() const
{
  return !(message_format & EMTimerMs) && (message_format & EMTimerNs);
}

bool LoggerSettings::isMTimer() const
{
  return message_format & EMTimer;
}

bool LoggerSettings::isTzAbbr() const
{
  return message_format & ETzAbbr;
}

bool LoggerSettings::isDate() const
{
  return message_format & EDate;
}

bool LoggerSettings::isTimeMs() const
{
  return (message_format & ETimeMs) && !(message_format & ETimeMicS);
}

bool LoggerSettings::isTimeMicS() const
{
  return !(message_format & ETimeMs) && (message_format & ETimeMicS);
}

bool LoggerSettings::isTime() const
{
  return message_format & ETime;
}

bool LoggerSettings::isTzSymLink() const
{
  return message_format & ETzSymLink;
}

bool LoggerSettings::isDateTimeInfo() const
{
  return message_format & EDateTimeInfo;
}

bool LoggerSettings::isName() const
{
  return message_format & EName;
}

bool LoggerSettings::isPid() const
{
  return message_format & EPid;
}

bool LoggerSettings::isProcessInfo() const
{
  return message_format & EProcessInfo;
}


bool LoggerSettings::isFunc() const
{
  return message_format & EFunc;
}

bool LoggerSettings::isFileLine() const
{
  return message_format & EFileLine;
}

bool LoggerSettings::isMessage() const
{
  return message_format & EMessage;
}


bool LoggerSettings::isDebugInfo() const
{
  return message_format & EDebugInfo;
}

bool LoggerSettings::isWordWrap() const
{
  return message_format & EWordWrap;
}
