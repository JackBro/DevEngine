/**
 * @file platform/windows/WindowsWindow.cpp
 *
 * DevEngine
 * Copyright 2015 Eetu 'Devenec' Oinasmaa
 */

#include <core/Log.h>
#include <core/Memory.h>
#include <graphics/Image.h>
#include <platform/windows/WindowsWindow.h>

using namespace Core;
using namespace Graphics;
using namespace Platform;

// Implementation

// Public

Window::Impl::Impl(HWND windowHandle)
	: _windowHandle(windowHandle),
	  _isCursorVisible(true),
	  _isFullscreen(false),
	  _isOpen(true)
{
	_rectangle = getRectangle();
}

void Window::Impl::setFullscreen(const Bool isFullscreen)
{
	if(isFullscreen != _isFullscreen)
	{
		setFullscreenStyle(isFullscreen);

		if(isFullscreen)
			setRectangle(getFullscreenRectangle(), isFullscreen);
		else
			setRectangle(_rectangle);

		_isFullscreen = isFullscreen;
	}
}

void Window::Impl::setIcon(const Image* image)
{
	if(image == nullptr)
		_icon = Icon();
	else
		_icon = Icon(image);

	SendMessageW(_windowHandle, WM_SETICON, ICON_BIG, reinterpret_cast<long>(_icon.handle()));
}
	
void Window::Impl::setRectangle(const Core::Rectangle& rectangle, const Bool isFullscreenRectangle)
{
	const Int32 result = SetWindowPos(_windowHandle, HWND_TOP, rectangle.x, rectangle.y, rectangle.width,
		rectangle.height, SWP_FRAMECHANGED | SWP_NOACTIVATE | SWP_NOZORDER);

	if(result == 0)
	{
		defaultLog << LogLevel::Error << COMPONENT_TAG << " Failed to set the rectangle of the window." <<
			Log::Flush();

		DE_ERROR_WINDOWS(0x000300);
	}

	if(!isFullscreenRectangle)
		_rectangle = rectangle;
}

void Window::Impl::setTitle(const String8& title) const
{
	const String16 title16 = toString16(title);
	const Int32 result = SetWindowTextW(_windowHandle, title16.c_str());

	if(result == 0)
	{
		defaultLog << LogLevel::Error << COMPONENT_TAG << " Failed to set the title of the window." << Log::Flush();
		DE_ERROR_WINDOWS(0x000301);
	}
}

Bool Window::Impl::shouldHideCursor(const Bool isCursorInClientArea) const
{
	if(!_isCursorVisible && isCursorInClientArea)
	{
		SetCursor(nullptr);
		return true;
	}

	return false;
}

// Private

const Char8* Window::Impl::COMPONENT_TAG = "[Platform::Window - Windows]";

Core::Rectangle Window::Impl::getRectangle() const
{
	RECT rectangle;
	const Int32 result = GetWindowRect(_windowHandle, &rectangle);

	if(result == 0)
	{
		defaultLog << LogLevel::Error << COMPONENT_TAG << " Failed to get the rectangle of the window." <<
			Log::Flush();

		DE_ERROR_WINDOWS(0x000302);
	}

	return Core::Rectangle(rectangle.left, rectangle.top, rectangle.right - rectangle.left,
		rectangle.bottom - rectangle.top);
}

void Window::Impl::setFullscreenStyle(const Bool isFullscreen) const
{
	const Int32 style = GetWindowLongPtrW(_windowHandle, GWL_STYLE);
	Int32 newStyle = style;

	if(isFullscreen)
		newStyle &= ~WS_CAPTION;
	else
		newStyle |= WS_CAPTION;

	const Int32 result = SetWindowLongPtrW(_windowHandle, GWL_STYLE, newStyle);

	if(result == 0)
	{
		defaultLog << LogLevel::Error << COMPONENT_TAG << " Failed to set the style of the window." << Log::Flush();
		DE_ERROR_WINDOWS(0x000303);
	}
}

Core::Rectangle Window::Impl::getFullscreenRectangle() const
{
	const RECT monitorRectangle = getMonitorRectangle();
		
	return Core::Rectangle(monitorRectangle.left, monitorRectangle.top, monitorRectangle.right - monitorRectangle.left,
		monitorRectangle.bottom - monitorRectangle.top);
}

RECT Window::Impl::getMonitorRectangle() const
{
	HMONITOR monitorHandle = MonitorFromWindow(_windowHandle, MONITOR_DEFAULTTONEAREST);
	MONITORINFO monitorInfo;
	monitorInfo.cbSize = sizeof(MONITORINFO);
	const Int32 result = GetMonitorInfoW(monitorHandle, &monitorInfo);

	if(result == 0)
	{
		defaultLog << LogLevel::Error << COMPONENT_TAG << " Failed to get the info of the window's monitor." <<
			Log::Flush();

		DE_ERROR_WINDOWS(0x000304);
	}

	return monitorInfo.rcMonitor;
}


// Graphics::Window

// Public

WindowHandle Window::handle() const
{
	return _impl->handle();
}

void Window::hide() const
{
	_impl->hide();
}

Core::Rectangle Window::rectangle() const
{
	return _impl->rectangle();
}

void Window::setCursorVisibility(const Bool isCursorVisible) const
{
	_impl->setCursorVisibility(isCursorVisible);
}

void Window::setFullscreen(const Bool isFullscreen) const
{
	_impl->setFullscreen(isFullscreen);
}

void Window::setIcon(const Image* image) const
{
	_impl->setIcon(image);
}

void Window::setRectangle(const Core::Rectangle& rectangle) const
{
	if(!_impl->isFullscreen())
		_impl->setRectangle(rectangle);
}

void Window::setTitle(const String8& title) const
{
	_impl->setTitle(title);
}

Bool Window::shouldClose() const
{
	return _impl->shouldClose();
}

void Window::show() const
{
	_impl->show();
}

// Private

Window::Window(WindowHandle windowHandle)
	: _impl(DE_NEW(Impl)(static_cast<HWND>(windowHandle))) { }

Window::~Window()
{
	DE_DELETE(_impl, Impl);
}
