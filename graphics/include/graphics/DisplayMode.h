/**
 * @file graphics/DisplayMode.h
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

namespace Graphics
{
	class DisplayMode final
	{
	public:

		DisplayMode();

		DisplayMode(const Uint32 width, const Uint32 height, const Uint32 colourDepth, const Uint32 frequency);

		DisplayMode(const DisplayMode& displayMode) = default;

		DisplayMode(DisplayMode&& displayMode) = default;

		~DisplayMode() = default;

		inline Uint32 colourDepth() const;

		inline Uint32 frequency() const;

		inline Uint32 height() const;

		inline Uint32 width() const;

		DisplayMode& operator =(const DisplayMode& displayMode) = default;

		DisplayMode& operator =(DisplayMode&& displayMode) = default;

		inline Bool operator ==(const DisplayMode& displayMode) const;

		inline Bool operator !=(const DisplayMode& displayMode) const;

		Bool operator <(const DisplayMode& displayMode) const;

	private:

		Uint32 _colourDepth;
		Uint32 _frequency;
		Uint32 _height;
		Uint32 _width;
	};

#include "inline/DisplayMode.inl"
}
