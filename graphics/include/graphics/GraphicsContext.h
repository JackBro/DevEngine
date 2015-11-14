/**
 * @file graphics/GraphicsContext.h
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

namespace Graphics
{
	class GraphicsConfig;
	class Window;

	class GraphicsContext final
	{
	public:

		~GraphicsContext();

		GraphicsContext(const GraphicsContext& graphicsContext) = delete;
		GraphicsContext(GraphicsContext&& graphicsContext) = delete;

		void makeCurrent() const;

		void makeNonCurrent() const;

		void swapBuffers() const;

		GraphicsContext& operator =(const GraphicsContext& graphicsContext) = delete;
		GraphicsContext& operator =(GraphicsContext&& graphicsContext) = delete;

	private:

		friend class GraphicsDeviceManager;

		class Impl;

		Impl* _impl;

		GraphicsContext(Window* window, const GraphicsConfig& graphicsConfig);
	};
}
