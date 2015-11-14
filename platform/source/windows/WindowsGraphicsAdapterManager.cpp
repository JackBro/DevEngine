/**
 * @file platform/windows/WindowsGraphicsAdapterManager.cpp
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

#include <algorithm>
#include <core/Memory.h>
#include <core/String.h>
#include <core/Types.h>
#include <graphics/DisplayMode.h>
#include <graphics/GraphicsAdapter.h>
#include <graphics/GraphicsAdapterManager.h>
#include <platform/windows/Windows.h>

using namespace Core;
using namespace Graphics;

// External

static DISPLAY_DEVICEW createAdapterInfo();
static DEVMODEW createDisplayModeInfo();
static DisplayMode getAdapterDisplayMode(const Char16* adapterName, const Uint32 modeIndex, DEVMODEW& modeInfo);
static Uint32 getAdapterDisplayModes(const Char16* adapterName, DisplayModeList& modes);

static Uint32 getCurrentAdapterDisplayModeIndex(const Char16* adapterName, DEVMODEW& modeInfo,
	const DisplayModeList& modes);


// Implementation

class GraphicsAdapterManager::Impl final
{
public:

	Impl()
	{
		Bool areAdaptersAvailable = true;
		DISPLAY_DEVICEW adapterInfo = ::createAdapterInfo();

		for(Uint32 i = 0u; areAdaptersAvailable; ++i)
			areAdaptersAvailable = initialiseAdapter(i, adapterInfo);
	}

	Impl(const Impl& impl) = delete;
	Impl(Impl&& impl) = delete;

	~Impl()
	{
		for(GraphicsAdapterList::const_iterator i = _graphicsAdapters.begin(), end = _graphicsAdapters.end(); i != end;
			++i)
		{
			DE_DELETE(*i, GraphicsAdapter);
		}
	}

	const GraphicsAdapterList& graphicsAdapters() const
	{
		return _graphicsAdapters;
	}

	Impl& operator =(const Impl& impl) = delete;
	Impl& operator =(Impl&& impl) = delete;

private:

	GraphicsAdapterList _graphicsAdapters;

	Bool initialiseAdapter(const Uint32 adapterIndex, DISPLAY_DEVICEW& adapterInfo)
	{
		const Int32 result = EnumDisplayDevicesW(nullptr, adapterIndex, &adapterInfo, 0u);

		if(result != 0 && (adapterInfo.StateFlags & DISPLAY_DEVICE_ATTACHED_TO_DESKTOP) != 0u)
		{
			DisplayModeList displayModes;
			const Uint32 currentDisplayModeIndex = ::getAdapterDisplayModes(adapterInfo.DeviceName, displayModes);

			GraphicsAdapter* graphicsAdapter = DE_NEW(GraphicsAdapter)(toString8(adapterInfo.DeviceName), displayModes,
				currentDisplayModeIndex);

			if((adapterInfo.StateFlags & DISPLAY_DEVICE_PRIMARY_DEVICE) != 0u)
				_graphicsAdapters.insert(_graphicsAdapters.begin(), graphicsAdapter);
			else
				_graphicsAdapters.push_back(graphicsAdapter);
		}

		return result != 0;
	}
};


// Graphics::GraphicsAdapterManager

// Public

GraphicsAdapterManager::GraphicsAdapterManager()
	: _impl(DE_NEW(Impl)())
{
	logAdapters();
}

GraphicsAdapterManager::~GraphicsAdapterManager()
{
	DE_DELETE(_impl, Impl);
}

const GraphicsAdapterList& GraphicsAdapterManager::graphicsAdapters() const
{
	return _impl->graphicsAdapters();
}


// External

static DISPLAY_DEVICEW createAdapterInfo()
{
	DISPLAY_DEVICEW adapterInfo;
	adapterInfo.cb = sizeof(DISPLAY_DEVICEW);

	return adapterInfo;
}

static DEVMODEW createDisplayModeInfo()
{
	DEVMODEW displayModeInfo = DEVMODEW();
	displayModeInfo.dmSize = sizeof(DEVMODEW);

	return displayModeInfo;
}

static DisplayMode getAdapterDisplayMode(const Char16* adapterName, const Uint32 modeIndex, DEVMODEW& modeInfo)
{
	const Int32 result = EnumDisplaySettingsW(adapterName, modeIndex, &modeInfo);

	if(result == 0)
	{
		return DisplayMode();
	}
	else
	{
		return DisplayMode(modeInfo.dmPelsWidth, modeInfo.dmPelsHeight, modeInfo.dmBitsPerPel,
			modeInfo.dmDisplayFrequency);
	}
}

static Uint32 getAdapterDisplayModes(const Char16* adapterName, DisplayModeList& modes)
{
	DisplayMode displayMode(1u, 0u, 0u, 0u);
	DEVMODEW displayModeInfo = ::createDisplayModeInfo();

	for(Uint32 i = 0u; displayMode.width() != 0u; ++i)
	{
		displayMode = ::getAdapterDisplayMode(adapterName, i, displayModeInfo);

		if(displayMode.width() != 0u && std::find(modes.begin(), modes.end(), displayMode) == modes.end())
			modes.push_back(displayMode);
	}

	modes.shrink_to_fit();
	std::sort(modes.begin(), modes.end());

	return ::getCurrentAdapterDisplayModeIndex(adapterName, displayModeInfo, modes);
}

static Uint32 getCurrentAdapterDisplayModeIndex(const Char16* adapterName, DEVMODEW& modeInfo,
	const DisplayModeList& modes)
{
	const DisplayMode displayMode = ::getAdapterDisplayMode(adapterName, ENUM_CURRENT_SETTINGS, modeInfo);
	DisplayModeList::const_iterator iterator = std::find(modes.begin(), modes.end(), displayMode);

	return iterator - modes.begin();
}
