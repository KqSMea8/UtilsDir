#pragma once

const int RGB_PIXEL_SIZE = 3;

enum PartUpdateStatus
{
	NoChanges = 0,
	Partitially,
	FullUpdate
};

struct ScreenShotInfo
{
	PartUpdateStatus PartStatus;
	CRect UpdateRectangle;
};

struct ImageBuffer
{
	SIZE Size;
	CByteArray Buffer;

	ImageBuffer()
	{
		Size.cx = Size.cy = 0;
	}
	~ImageBuffer()
	{
		Buffer.RemoveAll();
	}
};

class ImageBufferComparer
{
public:
	inline bool Initialize(CBitmap* bmp1, ImageBuffer *img2);
	inline bool CompareImages(CRect *diffRect);

private:
	int _width, _height, _scanLineBytes;
	byte *_image1, *_image2;

	inline bool comparePixels(int x, int y);
};

class ImageWorker
{
public:
	ImageWorker(bool isTopDownImage);
	~ImageWorker();

	bool IsInitialized();

	ImageBuffer* GetImageBuffer();

	bool GetDiffBuffer(CByteArray *byteArray, ScreenShotInfo *info, bool sizeUpdated);

private:
	bool _initialized, _topDownImage, _firstScreen;
	CRect _lastSize;
	CBitmap _pBitmap, _partBitmap;
	ImageBuffer _imageBuffer;

	CDC _displayDC;
	CImage _img;
	BITMAPINFOHEADER _bitmapHeader;
	BITMAPINFO  _bitmapInfo;
	ImageBufferComparer _imageComparer;

	inline void reset();
	void initializeBitmapInfo();
	bool cutRectFromBufferToPartImage(CBitmap* img, CRect &rect);
	bool cutRectFromBufferToSameImage(CBitmap* img, CRect &rect);
	bool createBitmapBySize(CRect &rect, CBitmap *bmp);
	bool createByteBuffer(CByteArray *byteArray, CBitmap* pBitmap);
};