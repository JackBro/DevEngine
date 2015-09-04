/**
 * @file platform/wgl/WGLGraphicsConfigChooser.h
 *
 * DevEngine
 * Copyright 2015 Eetu 'Devenec' Oinasmaa
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include <core/Array.h>
#include <core/Types.h>
#include <core/Vector.h>
#include <platform/windows/WindowsGraphics.h>

namespace Graphics
{
	class GraphicsConfig;
}

namespace Platform
{
	using PixelFormatAttributeList = Core::Array<Int32, 7u>;

	class GraphicsConfigChooser final
	{
	public:

		explicit GraphicsConfigChooser(HDC deviceContextHandle);

		GraphicsConfigChooser(const GraphicsConfigChooser& graphicsConfigChooser) = delete;
		GraphicsConfigChooser(GraphicsConfigChooser&& graphicsConfigChooser) = delete;

		~GraphicsConfigChooser() = default;

		Graphics::GraphicsConfig chooseConfig() const;

		GraphicsConfigChooser& operator =(const GraphicsConfigChooser& graphicsConfigChooser) = delete;
		GraphicsConfigChooser& operator =(GraphicsConfigChooser&& graphicsConfigChooser) = delete;

	private:
		
		using PixelFormatIndexList = Core::Vector<Int32>;

		static const PixelFormatAttributeList PIXEL_FORMAT_ATTRIBUTE_IDS;

		HDC _deviceContextHandle;

		Uint32 getPixelFormatCount() const;
		PixelFormatIndexList getPixelFormatIndices(const Uint32 formatCount) const;
		
		Int32 chooseBestPixelFormat(const PixelFormatIndexList& formatIndices,
			PixelFormatAttributeList& formatAttributes) const;

		PixelFormatAttributeList getPixelFormatAttributes(const Int32 formatIndex) const;
	};
}
