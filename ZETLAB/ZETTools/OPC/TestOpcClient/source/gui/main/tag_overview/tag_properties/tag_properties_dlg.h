#pragma once

#include <AnchoredDialog/AnchoredDialog.h>
#include <Localized/Localized.h>

#include <zlo.hpp>

#include <vector>

class CTagPropertiesDlg :
    public CAnchoredDialog,
    public CLocalized
{
public:
    typedef std::vector< zlo::client::group::item* > item_list;

public:
    CTagPropertiesDlg( CWnd* pParent = NULL );

    enum { IDD = IDD_TAG_PROPERTIES };

public:
    virtual INT_PTR DoModal( const item_list& tags );

    int QualityToIndex( const WORD& quality ) const;

protected:
    virtual void DoDataExchange( CDataExchange* pDX );
    virtual BOOL OnInitDialog();

    DECLARE_MESSAGE_MAP()
    DECLARE_ANCHOR_MAP()

private:
    void InitValue();

    void InitQuality();

public:
    CString m_value;
    int     m_qualityIndex;

protected:
    HICON   m_hIcon;

private:
    item_list _tags;
};

static const WORD _qualities[17] = {
    0xffff,
    0,
    0x40,
    0xc0,
    0x4,
    0x8,
    0xc,
    0x10,
    0x14,
    0x18,
    0x1c,
    0x20,
    0x44,
    0x50,
    0x54,
    0x58,
    0xd8
};
