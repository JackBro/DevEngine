/**
 * @file graphics/PNGReader.cpp
 *
 * DevEngine
 * Copyright 2015-2016 Eetu 'Devenec' Oinasmaa
 *
 * DevEngine is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * DevEngine is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with DevEngine. If not, see <http://www.gnu.org/licenses/>.
 */

#include <algorithm>
#include <cstddef>
#include <core/Error.h>
#include <core/FileStream.h>
#include <core/Log.h>
#include <core/Memory.h>
#include <core/Types.h>
#include <core/debug/Assert.h>
#include <graphics/Image.h>
#include <graphics/ImageFormat.h>
#include <graphics/PNGReader.h>

using namespace Core;
using namespace Graphics;

// External

using Size = std::size_t;

static const Char8* COMPONENT_TAG = "[Graphics::PNGReader] ";

static Void* allocateMemory(png_struct* pngStructure, Size size);
static void deallocateMemory(png_struct* pngStructure, Void* pointer);
static void handleError(png_struct* pngStructure, const Char8* message);
static void handleWarning(png_struct* pngStructure, const Char8* message);
static void readData(png_struct* pngStructure, Uint8* buffer, Size size);


// Public

PNGReader::PNGReader()
	: _pngInfo(nullptr),
	  _pngStructure(nullptr)
{
	initialiseStructure();
	initialiseInfo();
}

PNGReader::~PNGReader()
{
	png_destroy_read_struct(&_pngStructure, &_pngInfo, nullptr);
}

Image* PNGReader::readImage(FileStream& fileStream)
{
	validateSignature(fileStream);
	png_set_read_fn(_pngStructure, &fileStream, ::readData);
	const Int32 transforms = PNG_TRANSFORM_EXPAND | PNG_TRANSFORM_PACKING | PNG_TRANSFORM_SCALE_16;
	png_read_png(_pngStructure, _pngInfo, transforms, nullptr);
	const Uint32 height = png_get_image_height(_pngStructure, _pngInfo);
	const Uint rowByteCount = png_get_rowbytes(_pngStructure, _pngInfo);
	ByteList data(height * rowByteCount);
	data.shrink_to_fit();
	Uint8** rows = png_get_rows(_pngStructure, _pngInfo);

	for(Uint32 i = 0u; i < height; ++i)
		std::copy(rows[i], rows[i] + rowByteCount, data.data() + i * rowByteCount);

	return createImage(data);
}

// Private

void PNGReader::initialiseStructure()
{
	_pngStructure =
		png_create_read_struct_2(PNG_LIBPNG_VER_STRING, nullptr, ::handleError, ::handleWarning, nullptr,
			::allocateMemory, ::deallocateMemory);

	if(_pngStructure == nullptr)
	{
		defaultLog << LogLevel::Error << ::COMPONENT_TAG << "Failed to initialise the PNG structure." <<
			Log::Flush();

		DE_ERROR(0x0);
	}
}

void PNGReader::initialiseInfo()
{
	_pngInfo = png_create_info_struct(_pngStructure);

	if(_pngInfo == nullptr)
	{
		defaultLog << LogLevel::Error << ::COMPONENT_TAG << "Failed to initialise the PNG info." <<
			Log::Flush();

		DE_ERROR(0x0);
	}
}

void PNGReader::validateSignature(FileStream& fileStream)
{
	Uint8 signature[8];
	fileStream.read(signature, sizeof(signature));
	const Int32 result = png_sig_cmp(signature, 0u, sizeof(signature));

	if(result != 0)
	{
		defaultLog << LogLevel::Error << ::COMPONENT_TAG << "The signature of the PNG file is invalid." <<
			Log::Flush();

		DE_ERROR(0x0);
	}

	png_set_sig_bytes(_pngStructure, 8);
}

Image* PNGReader::createImage(const ByteList& data) const
{
	const Uint32 width = png_get_image_width(_pngStructure, _pngInfo);
	const Uint32 height = png_get_image_height(_pngStructure, _pngInfo);

	return DE_NEW(Image)(width, height, getImageFormat(), data);
}

ImageFormat PNGReader::getImageFormat() const
{
	switch(png_get_color_type(_pngStructure, _pngInfo))
	{
		case PNG_COLOR_TYPE_GRAY:
			return ImageFormat::R8;

		case PNG_COLOR_TYPE_GRAY_ALPHA:
			return ImageFormat::RA8;

		case PNG_COLOR_TYPE_RGB:
			return ImageFormat::RGB8;

		case PNG_COLOR_TYPE_RGB_ALPHA:
			return ImageFormat::RGBA8;

		default:
			DE_ASSERT(false);
			return ImageFormat();
	}
}


// External

static Void* allocateMemory(png_struct* pngStructure, Size size)
{
	static_cast<Void>(pngStructure);
	return DE_ALLOCATE(size);
}

static void deallocateMemory(png_struct* pngStructure, Void* pointer)
{
	static_cast<Void>(pngStructure);
	DE_DEALLOCATE(pointer);
}

static void handleError(png_struct* pngStructure, const Char8* message)
{
	static_cast<Void>(pngStructure);

	defaultLog << LogLevel::Error << ::COMPONENT_TAG << "PNG error: " << message << '.' << Log::Flush();
	DE_ERROR(0x0);
}

static void handleWarning(png_struct* pngStructure, const Char8* message)
{
	static_cast<Void>(pngStructure);
	defaultLog << LogLevel::Warning << ::COMPONENT_TAG << "PNG warning: " << message << '.' << Log::Flush();
}

static void readData(png_struct* pngStructure, Uint8* buffer, Size size)
{
	FileStream* fileStream = static_cast<FileStream*>(png_get_io_ptr(pngStructure));

	if(fileStream->isPastEndOfFile())
	{
		defaultLog << LogLevel::Error << ::COMPONENT_TAG << "Reached the end of the file unexpectedly." <<
			Log::Flush();

		DE_ERROR(0x0);
	}

	const Uint32 bytesToRead = static_cast<Uint32>(size);
	const Uint32 bytesRead = fileStream->read(buffer, bytesToRead);

	if(bytesRead < bytesToRead)
	{
		defaultLog << LogLevel::Error << ::COMPONENT_TAG << "Failed to read the specified number of bytes." <<
			Log::Flush();

		DE_ERROR(0x0);
	}
}
