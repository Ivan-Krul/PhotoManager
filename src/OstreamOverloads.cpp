#include "OstreamOverloads.h"

std::ostream& operator<<(std::ostream& out, Date date) {
   out << std::setfill('0');
   if (date.year == cNullDate.year) out << "XXXX";
   else out << std::setw(4) << date.year;

   out << '.';

   if (date.month == cNullDate.month) out << "XX";
   else out << std::setw(2) << date.month;

   out << '.';

   if (date.day == cNullDate.day) out << "XX";
   else out << std::setw(2) << date.day;

   out << ' ';

   if (date.hours == cNullDate.hours) out << "XX";
   else out << std::setw(2) << date.hours;

   out << ':';

   if (date.minutes == cNullDate.minutes) out << "XX";
   else out << std::setw(2) << date.minutes;

   out << ':';

   if (date.seconds == cNullDate.seconds) out << "XX";
   else out << std::setw(2) << date.seconds;

   return out;
}