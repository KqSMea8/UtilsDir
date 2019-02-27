#include <stdafx.h>
#include "hightlight_rule.hpp"

#include <pugixml-1.0/src/pugixml.hpp>

#include <boost/lexical_cast.hpp>

#include <algorithm>

namespace utils {

#define deserialize(obj, accessor, field, type) \
    obj accessor field = boost::lexical_cast< type >(it.child( L#field ).first_child().value())

highlight_rule_list deserialize_rules( LPCTSTR lpszXml )
{
    using namespace pugi;

    xml_document xml;
    
    if (!xml.load( lpszXml ))
        return highlight_rule_list();

    highlight_rule_list rules;

    for (auto it = xml.first_child().first_child(); it; it = it.next_sibling())
    {
        highlight_rule rule;

        deserialize(rule, ., where,     int);
        deserialize(rule, ., condition, bool);
        deserialize(rule, ., what,      std::wstring);
        deserialize(rule, ., color,     COLORREF);

        rules.push_back( rule );
    }

    return std::move(rules);
}

std::wstring serialize_rules( const highlight_rule_list& rules )
{
    using namespace pugi;

    xml_document xml;
    xml_node     root;

    root = xml.append_child( _T("Rules") );

    for (auto rule = rules.begin(), end = rules.end(); rule != end; ++rule)
    {
        xml_node it = root.append_child( _T("Rule") );
    }

    return std::wstring();
}

} // namespace utils
