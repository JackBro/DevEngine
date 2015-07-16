/**
 * @file graphics/GraphicsBuffer.h
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

#include <core/Types.h>
#include <graphics/GraphicsResource.h>

namespace Graphics
{
	enum class AccessMode;

	class GraphicsBuffer : public GraphicsResource
	{
	public:

		GraphicsBuffer(const GraphicsBuffer& graphicsBuffer) = delete;
		GraphicsBuffer(GraphicsBuffer&& graphicsBuffer) = delete;

		void demapData() const;

		Byte* mapData(const Uint32 size, const Uint32 bufferOffset = 0u) const;

		void setData(const Byte* data, const Uint32 size, const Uint32 bufferOffset = 0u) const;

		GraphicsBuffer& operator =(const GraphicsBuffer& graphicsBuffer) = delete;
		GraphicsBuffer& operator =(GraphicsBuffer&& graphicsBuffer) = delete;

	protected:

		GraphicsBuffer(const Uint32 size, const AccessMode& accessMode);

		virtual ~GraphicsBuffer();

	private:

		friend class GraphicsDevice;
		friend class VertexBufferState;

		class Impl;

		Impl* _impl;
	};
}
