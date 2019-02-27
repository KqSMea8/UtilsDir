#pragma once

#include <zlo.hpp>

#include <boost/signals.hpp>

#include "../../../algo/network/network.hpp"

#include "../../utils/tree/tree.h"

class CHostTree : public CTreeCtrlEx
{
public:
    virtual void LoadItemImages();

public:
    HTREEITEM AddNetwork  ( HTREEITEM hNetwork, utils::network_ptr network );

    HTREEITEM AddHost     ( HTREEITEM hNetwork, zlo::host_ptr host );

private:
    HTREEITEM AddOpcServer( HTREEITEM hHost,    zlo::server_ptr server );

protected:
    afx_msg void OnSelectionChange( NMHDR* pNMHDR, LRESULT* pResult );
    afx_msg void OnDoubleClick    ( NMHDR* pNMHDR, LRESULT* pResult );
    afx_msg void OnExpanding      ( NMHDR* pNMHDR, LRESULT* pResult );

    DECLARE_MESSAGE_MAP()

public:
    void OnChange( const boost::function< void(zlo::host*, zlo::server*) >& f ) {
        _onChange.connect( f );
    }

    void OnConnect( const boost::function< void(zlo::server*) >& f ) {
        _onConnect.connect( f );
    }

private:
    boost::signal< void(zlo::host*, zlo::server*) > _onChange;
    boost::signal< void(zlo::server*) >             _onConnect;

private:
    int Network;
    int Host;
    int OpcServer;

    // Holders
    utils::network_list _networks;
    zlo::host_list      _hosts;
    zlo::server_list    _servers;
};
