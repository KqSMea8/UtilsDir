#ifndef highlight_rules_hpp__
#define highlight_rules_hpp__

#include <string>

#include <vector>

#include <windows.h>

namespace utils {

struct highlight_rule
{
    int          where;
    bool         condition;
    std::wstring what;
    COLORREF     color;
};
typedef std::vector< highlight_rule > highlight_rule_list;

highlight_rule_list deserialize_rules( LPCTSTR lpszXml );

std::wstring serialize_rules( const highlight_rule_list& rules );

} // namespace utils

#endif // highlight_rules_hpp__
