/**
 * @file platform/wgl/WGLGraphicsExtensionManager.h
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
#include <platform/windows/Windows.h>

namespace Platform
{
	class GraphicsContextBase;

	class GraphicsExtensionManager final
	{
	public:

		GraphicsExtensionManager() = delete;

		GraphicsExtensionManager(const GraphicsExtensionManager& graphicsExtensionManager) = delete;
		GraphicsExtensionManager(GraphicsExtensionManager&& graphicsExtensionManager) = delete;

		~GraphicsExtensionManager() = delete;

		static void initialiseExtensions(const GraphicsContextBase& graphicsContext);

		GraphicsExtensionManager& operator =(const GraphicsExtensionManager& graphicsExtensionManager) = delete;
		GraphicsExtensionManager& operator =(GraphicsExtensionManager&& graphicsExtensionManager) = delete;

	private:

		static const Char8* COMPONENT_TAG;

		static void validateContextState();
		static void getContextExtensionFunctions();
		static void logSupportedContextExtensions(HDC deviceContextHandle);

		template<typename T>
		static inline T getExtensionFunction(const Char8* functionName);
	};

#include "inline/WGLGraphicsExtensionManager.inl"
}
