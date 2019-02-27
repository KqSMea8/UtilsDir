#ifndef network_hpp__
#define network_hpp__

#include <boost/shared_ptr.hpp>
#include <boost/noncopyable.hpp>

#include <list>

#include <windows.h>

namespace utils {

// Forward declarations
class network;
typedef boost::shared_ptr< network > network_ptr;
typedef std::list< network_ptr >     network_list;

class network : public boost::noncopyable
{
public:
    enum type {
         type_generic = 0x0,
         type_domain,
         type_server,
         type_share,
         type_file,
         type_group,
         type_network,
         type_root,
         type_share_admin,
         type_directory,
         type_tree
    };

public:
    network();
    network( LPNETRESOURCE pNetRes );
    ~network();

public:
    bool open();

    bool isOpen() const { return (INVALID_HANDLE_VALUE != _hEnum); };

    network_list browse();

    LPCTSTR getName();

    type getType();

private:
    bool   _isRoot;
    HANDLE _hEnum;
    BYTE   _buffer[16384]; // 16K
};

} // namespace utils

#endif // network_hpp__
