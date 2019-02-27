#include "stdafx.h"

#include "ImageWorker.h"

ImageWorker::ImageWorker(bool isTopDownImage): _topDownImage(isTopDownImage), _firstScreen(true), _lastSize(0, 0, 0, 0)
{
	_initialized = true;
	if(!_displayDC.CreateDC(_T("DISPLAY"), NULL, NULL, NULL))
	{
		_initialized = false;
	}
	initializeBitmapInfo();
}
ImageWorker::~ImageWorker()
{
	_pBitmap.DeleteObject();
	_partBitmap.DeleteObject();
}

bool ImageWorker::IsInitialized()
{
	return _initialized;
}

ImageBuffer* ImageWorker::GetImageBuffer()
{
	return &_imageBuffer;
}

bool ImageWorker::GetDiffBuffer(CByteArray *byteArray, ScreenShotInfo *info, bool sizeUpdated)
{
	//reset();

	bool result = false;
	info->PartStatus = FullUpdate; // по-умолчанию - полный кадр без проверки на отличия.

	// Если все инициализировано
	if(_initialized)
	{
		bool sizeChanged = false;
		if(sizeUpdated || _lastSize.Size() != _imageBuffer.Size)
		{
			sizeChanged = true;
			_lastSize.SetRect(0, 0, _imageBuffer.Size.cx, _imageBuffer.Size.cy);
			_firstScreen = true;
		}
		if(!_firstScreen) // проверяем, что уже сделано как минимум два кадра, чтобы можно было проверять отличия.
		{
			if(_imageComparer.Initialize(&_pBitmap, &_imageBuffer)) // Инициализируем класс сравнения.
			{
				CRect changedRect;
				if(_imageComparer.CompareImages(&changedRect)) // Сравниваем, получая границы различия.
				{
					if(createBitmapBySize(changedRect, &_partBitmap))
					{
						if(cutRectFromBufferToPartImage(&_partBitmap, changedRect))
						{
							result = createByteBuffer(byteArray, &_partBitmap) && cutRectFromBufferToSameImage(&_pBitmap, changedRect);
							if(_topDownImage)
							{
								info->UpdateRectangle.SetRect(changedRect.left, _lastSize.Height() - changedRect.bottom, changedRect.right, _lastSize.Height() - changedRect.top);
							}
							else
							{
								info->UpdateRectangle.SetRect(changedRect.TopLeft(), changedRect.BottomRight());
							}
							
							info->PartStatus = Partitially;
						}
						_partBitmap.DeleteObject();
						return result;
					}
				}
				else
				{
					// Экран или окно не менялось, пропускаем.
					info->PartStatus = NoChanges;
					return false;
				}
			}
		}
		else
		{
			_firstScreen = false;
		}

		info->UpdateRectangle.SetRectEmpty();

		if(sizeChanged)
		{
			_pBitmap.DeleteObject();
			createBitmapBySize(_lastSize, &_pBitmap);
		}
		cutRectFromBufferToSameImage(&_pBitmap, _lastSize);
		result = createByteBuffer(byteArray, &_pBitmap);
	}
	return result;
}

bool ImageWorker::createByteBuffer(CByteArray *byteArray, CBitmap* pBitmap)
{
	bool result = false;
	_img.Attach((HBITMAP)pBitmap->Detach());

	//_img.Save(L"D:\\Image.jpeg", Gdiplus::ImageFormatJPEG);

	IStream *pstream;
	if(CreateStreamOnHGlobal(NULL, TRUE, &pstream) == S_OK)
	{
		if(_img.Save(pstream, Gdiplus::ImageFormatJPEG) == S_OK)
		{
			ULARGE_INTEGER ulnSize;
			LARGE_INTEGER lnOffset;
			lnOffset.QuadPart = 0;
			if(pstream->Seek(lnOffset, STREAM_SEEK_END, &ulnSize) == S_OK)
			{
				if(pstream->Seek(lnOffset, STREAM_SEEK_SET, NULL) == S_OK)
				{
					byteArray->SetSize(ulnSize.QuadPart);
					ULONG ulBytesRead;
					if(pstream->Read(byteArray->GetData(), (ULONG)ulnSize.QuadPart, &ulBytesRead) == S_OK)
					{
						result = true;
					}
				}
			}
		}
		pstream->Release();
	}

	pBitmap->Attach(_img.Detach());
	return result;
}

bool ImageWorker::cutRectFromBufferToPartImage(CBitmap* img, CRect &rect)
{
	BITMAP bmp;
	img->GetBitmap(&bmp);
	int width = bmp.bmWidth, height = bmp.bmHeight, scanLineBytes = bmp.bmWidthBytes;

	if(width != rect.Width() || height != rect.Height())
	{
		return false;
	}

	byte* imageBuf = (byte*)bmp.bmBits;

	int bufLineSizeBytes = _imageBuffer.Size.cx * RGB_PIXEL_SIZE;

	int lineSizeBytes = width * RGB_PIXEL_SIZE;

	byte *buff = _imageBuffer.Buffer.GetData();

	for(int i = rect.top; i < rect.bottom; i++)
	{
		memcpy(imageBuf + (i - rect.top) * scanLineBytes, buff + i * bufLineSizeBytes + rect.left * RGB_PIXEL_SIZE, lineSizeBytes);
	}

	return true;
}

bool ImageWorker::cutRectFromBufferToSameImage(CBitmap* img, CRect &rect)
{
	BITMAP bmp;
	img->GetBitmap(&bmp);
	int width = bmp.bmWidth, height = bmp.bmHeight, scanLineBytes = bmp.bmWidthBytes;

	if(width != _imageBuffer.Size.cx || height != _imageBuffer.Size.cy)
	{
		return false;
	}

	byte* imageBuf = (byte*)bmp.bmBits;

	int bufLineSizeBytes = _imageBuffer.Size.cx * RGB_PIXEL_SIZE;

	int lineSizeBytes = (rect.Width() + 1) * RGB_PIXEL_SIZE;

	byte *buff = _imageBuffer.Buffer.GetData();

	for(int i = rect.top; i < rect.bottom; i++)
	{
		memcpy(imageBuf + i * scanLineBytes + rect.left * RGB_PIXEL_SIZE, buff + i * bufLineSizeBytes + rect.left * RGB_PIXEL_SIZE, lineSizeBytes);
	}

	return true;
}

void ImageWorker::initializeBitmapInfo()
{
	_bitmapHeader.biSize = sizeof(BITMAPINFOHEADER);
	_bitmapHeader.biWidth = 0;
	_bitmapHeader.biHeight = 0;
	_bitmapHeader.biPlanes = 1;
	_bitmapHeader.biBitCount = RGB_PIXEL_SIZE * 8;
	_bitmapHeader.biCompression = BI_RGB;
	_bitmapHeader.biSizeImage = 0;
	_bitmapHeader.biXPelsPerMeter = 10;
	_bitmapHeader.biYPelsPerMeter = 10;
	_bitmapHeader.biClrUsed = 0;
	_bitmapHeader.biClrImportant = 0;

	ZeroMemory(&_bitmapInfo, sizeof(_bitmapInfo));
	_bitmapInfo.bmiHeader = _bitmapHeader;
	_bitmapInfo.bmiColors->rgbBlue = 0;
	_bitmapInfo.bmiColors->rgbGreen = 0;
	_bitmapInfo.bmiColors->rgbRed = 0;
	_bitmapInfo.bmiColors->rgbReserved = 0;
}

bool ImageWorker::createBitmapBySize(CRect &rect, CBitmap *bmp)
{
	_bitmapInfo.bmiHeader.biWidth = rect.Width();
	_bitmapInfo.bmiHeader.biHeight = _topDownImage ? rect.Height() : -rect.Height(); // Bitmap у нас Down-Top направленный, т.е от нижнего левого угла к верхнему.

	// Create DIB, т.е Bitmap, пиксели которого можно получить в виде массива байтов.
	HBITMAP hBitmap = CreateDIBSection(_displayDC.m_hDC, &_bitmapInfo, DIB_RGB_COLORS, NULL, NULL, 0);
	if (hBitmap == NULL)
	{
		return false;
	}

	bmp->Attach(hBitmap);
	return true;
}

//////////////////////////////////////////////////////////////////////////
/// Image buffer comparer class.
//////////////////////////////////////////////////////////////////////////

inline bool ImageBufferComparer::Initialize(CBitmap* img, ImageBuffer *buff)
{
	BITMAP bmp;
	img->GetBitmap(&bmp);

	// Если кадры различны по какому-то параметру - возвращаем false,
	// так как в данном случае сравнение будет некорректно.
	if((_width = bmp.bmWidth) != buff->Size.cx ||
	   (_height = bmp.bmHeight) != buff->Size.cy)
	{
		return false;
	}
	if(_width == 0 || _height == 0)
	{
		return false;
	}

	_scanLineBytes = bmp.bmWidthBytes;
	_image1 = (byte*)bmp.bmBits;
	_image2 = buff->Buffer.GetData();

	return _image1 && _image2;
}

//////////////////////////////////////////////////////////////////////////
/// Метод, сравнивающий две Bitmap(как массивы байтов).
/// Алгоритм сравнения:
/// 1. Начинаем сравнивать попиксельно(memcpy) каждую строчку начиная с левой стороны.
///	2. Первое встреченное различие определяет верхнюю границу области различия.
/// 3. Если положение отличного пикселя левее текущей левой границы различия - передвигаем левую границу.
/// 4. Если положение отличного пикселя ниже текущей нижней границы различия - передвигаем нижнюю границу.
/// 5. Начинаем сравнивать эту же строчку от правой стороны.
/// 6. Если положение отличного пикселя правее текущей правой границы различия - передвигаем правую границу.
/// 8. Таким образом проходим по всем строчкам кадра.
/// 7. Если все границы области различия определены - возвращаем true, иначе false.
//////////////////////////////////////////////////////////////////////////
inline bool ImageBufferComparer::CompareImages(CRect *diffRect)
{
	int left = -1, right = -1, top = -1, bottom = -1, i, j, k;

	for(i = 0; i < _height; i++)
	{
		for(j = 0; j < _width; j++)
		{
			if(comparePixels(j, i))
			{
				if(top < 0)
				{
					top = i;
				}
				bottom = i;
				if(left < 0 || j < left)
				{
					left = j;
				}
				if(right < j)
				{
					right = j;
				}
				for(k = _width - 1; k > right; k--)
				{
					if(comparePixels(k, i))
					{
						right = k;
						break;
					}
				}
				break;
			}
		}
	}
	if(left < 0 || right < 0 || top < 0 || bottom < 0)
	{
		return false;
	}

	diffRect->SetRect(left, top, min(right, _width - 1), min(bottom, _height - 1));
	return true;
}

inline bool ImageBufferComparer::comparePixels(int x, int y)
{
	return memcmp(_image1 + y * _scanLineBytes + x * RGB_PIXEL_SIZE, _image2 + RGB_PIXEL_SIZE * (y * _width + x), RGB_PIXEL_SIZE) != 0;
}