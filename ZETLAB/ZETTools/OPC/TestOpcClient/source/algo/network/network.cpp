#include "stdafx.h"
#include "network.hpp"

#include <boost/make_shared.hpp>
#include <boost/scoped_array.hpp>

#include <algorithm>

#include <winnetwk.h>

namespace utils {

network::network() :
    _hEnum (INVALID_HANDLE_VALUE),
    _isRoot(true)
{
}

network::network( LPNETRESOURCE pNetRes ) :
    _hEnum(INVALID_HANDLE_VALUE),
    _isRoot(false)
{
    memcpy_s( _buffer, 16384, pNetRes, 16384 );
}

network::~network()
{
    if (INVALID_HANDLE_VALUE != _hEnum)
        WNetCloseEnum( _hEnum );
}

bool network::open()
{
    DWORD dwError = WNetOpenEnum(
        RESOURCE_GLOBALNET, // Enumerate all resources on the network
        RESOURCETYPE_ANY, // All resources by type
        0, // All resources by usage
        _isRoot ? NULL : reinterpret_cast< LPNETRESOURCE >(_buffer),
        &_hEnum
    );

    return (NO_ERROR == dwError);
}

network_list network::browse()
{
    if (INVALID_HANDLE_VALUE == _hEnum)
        return network_list();

    DWORD        dwError = NO_ERROR;
    network_list networks;

    while (NO_ERROR == dwError)
    {
        DWORD nCount     = -1;
        DWORD bufferSize = 16384;

        dwError = WNetEnumResource( _hEnum, &nCount, _buffer, &bufferSize );

        if (ERROR_NO_MORE_ITEMS == dwError || !nCount)
            return networks;
        
        for (DWORD ii = 0; ii < nCount; ++ii)
        {
            networks.push_back(
                boost::make_shared< network >(
                    &(reinterpret_cast< LPNETRESOURCE >(_buffer))[ii]
                )
            );
        }
    }

    return std::move( networks );
}

LPCTSTR network::getName()
{
    if (_isRoot)
        return _T("Сетевое окружение");

    LPNETRESOURCE pNetRes = reinterpret_cast< LPNETRESOURCE >(_buffer);

    return pNetRes->lpRemoteName;
}

network::type network::getType()
{
    if (_isRoot)
        return type_root;

    LPNETRESOURCE pNetRes = reinterpret_cast< LPNETRESOURCE >(_buffer);

    return type(pNetRes->dwDisplayType);
}

} // namespace utils
