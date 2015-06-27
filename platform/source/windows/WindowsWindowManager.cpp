/**
 * @file platform/windows/WindowsWindowManager.cpp
 *
 * DevEngine
 * Copyright 2015 Eetu 'Devenec' Oinasmaa
 */

#include <algorithm>
#include <core/List.h>
#include <core/Log.h>
#include <core/Memory.h>
#include <core/UtilityMacros.h>
#include <core/debug/Assert.h>
#include <graphics/DisplayMode.h>
#include <graphics/GraphicsAdapter.h>
#include <graphics/GraphicsAdapterManager.h>
#include <graphics/Window.h>
#include <graphics/WindowManager.h>

#define OEMRESOURCE
#include <platform/windows/Windows.h> // Needs to be the first header to include Windows.h
#undef OEMRESOURCE

#include <platform/wgl/WGLGraphicsExtensionManager.h>
#include <platform/wgl/WGLTemporaryGraphicsContext.h>
#include <platform/windows/WindowsWindow.h>

using namespace Core;
using namespace Graphics;
using namespace Platform;

// Implementation

class WindowManager::Impl final
{
public:

	Impl()
	{
		const WNDCLASSEX windowClassInfo = createWindowClassInfo();
		registerWindowClass(windowClassInfo);
		initialiseGraphicsExtensions();
	}

	Impl(const Impl& impl) = delete;
	Impl(Impl&& impl) = delete;

	~Impl()
	{
		destroyWindowObjects();
		deregisterWindowClass();
	}

	Window* createWindowObject()
	{
		HWND windowHandle = createWindow();
		Window* window = DE_NEW(Window)(windowHandle);
		setWindowUserDataPointer(windowHandle, window->_impl);
		_windows.push_back(window);

		return window;
	}

	void destroyWindowObject(Window* window)
	{
		DE_ASSERT(window != nullptr);
		WindowList::const_iterator iterator = std::find(_windows.begin(), _windows.end(), window);
		DE_ASSERT(iterator != _windows.end());
		destroyWindow(static_cast<HWND>(window->handle()));
		_windows.erase(iterator);
		DE_DELETE(window, Window);
	}

	Impl& operator =(const Impl& impl) = delete;
	Impl& operator =(Impl&& impl) = delete;

	static void processMessages()
	{
		MSG message;

		while(PeekMessageW(&message, nullptr, 0u, 0u, PM_REMOVE) != FALSE)
			DispatchMessageW(&message);
	}

private:

	using WindowList = List<Window*>;

	static constexpr Uint32 WINDOW_DEFAULT_HEIGHT = 600u;
	static constexpr Uint32 WINDOW_DEFAULT_WIDTH  = 800u;
	static constexpr Uint32 WINDOW_STYLE		  = WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU;

	static const Char8* COMPONENT_TAG;
	static const Char16* WINDOW_CLASS_NAME;
	static const Char16* WINDOW_DEFAULT_TITLE;

	WindowList _windows;

	void destroyWindowObjects()
	{
		for(WindowList::const_iterator i = _windows.begin(), end = _windows.end(); i != end; ++i)
		{
			destroyWindow(static_cast<HWND>((*i)->handle()));
			DE_DELETE(*i, Window);
		}
	}

	static WNDCLASSEX createWindowClassInfo()
	{
		WNDCLASSEXW windowClassInfo = WNDCLASSEXW();
		windowClassInfo.cbSize = sizeof(WNDCLASSEXW);
		windowClassInfo.hCursor = loadCursor();
		windowClassInfo.hInstance = GetModuleHandleW(nullptr);
		windowClassInfo.lpfnWndProc = processMessage;
		windowClassInfo.lpszClassName = WINDOW_CLASS_NAME;
		windowClassInfo.style = CS_OWNDC;

		return windowClassInfo;
	}

	static void registerWindowClass(const WNDCLASSEX& windowClassInfo)
	{
		const Uint32 result = RegisterClassExW(&windowClassInfo);

		if(result == 0u)
		{
			defaultLog << LogLevel::Error << COMPONENT_TAG << " Failed to register the window class." << Log::Flush();
			DE_ERROR_WINDOWS(0x000403);
		}
	}

	static void initialiseGraphicsExtensions()
	{
		HWND windowHandle = createWindow();
		TemporaryGraphicsContext temporaryGraphicsContext(windowHandle);
		temporaryGraphicsContext.initialise();
		GraphicsExtensionManager::initialiseExtensions(temporaryGraphicsContext);
		temporaryGraphicsContext.deinitialise();
		destroyWindow(windowHandle);
	}

	static void deregisterWindowClass()
	{
		const Int32 result = UnregisterClassW(WINDOW_CLASS_NAME, GetModuleHandleW(nullptr));
		
		if(result == 0u)
		{
			defaultLog << LogLevel::Error << COMPONENT_TAG << " Failed to deregister the window class." <<
				Log::Flush();

			DE_ERROR_WINDOWS(0x000400);
		}
	}

	static HWND createWindow()
	{
		const RECT windowRectangle = createWindowRectangle();
		const Int32 windowWidth = windowRectangle.right - windowRectangle.left;
		const Int32 windowHeight = windowRectangle.bottom - windowRectangle.top;
		
		HWND windowHandle = CreateWindowExW(0u, WINDOW_CLASS_NAME, WINDOW_DEFAULT_TITLE, WINDOW_STYLE,
			windowRectangle.left, windowRectangle.top, windowWidth, windowHeight, nullptr, nullptr,
			GetModuleHandleW(nullptr), nullptr);

		if(windowHandle == nullptr)
		{
			defaultLog << LogLevel::Error << COMPONENT_TAG << " Failed to create a window." << Log::Flush();
			DE_ERROR_WINDOWS(0x000401);
		}

		return windowHandle;
	}

	static void setWindowUserDataPointer(HWND windowHandle, Window::Impl* windowImpl)
	{
		SetLastError(0u);
		const Int32 result = SetWindowLongPtrW(windowHandle, GWLP_USERDATA, reinterpret_cast<long>(windowImpl));

		if(result == 0 && GetLastError() != 0u)
		{
			defaultLog << LogLevel::Error << COMPONENT_TAG << " Failed to set the user data pointer of a window." <<
				Log::Flush();

			DE_ERROR_WINDOWS(0x000405);
		}
	}

	static void destroyWindow(HWND windowHandle)
	{
		const Int32 result = DestroyWindow(windowHandle);

		if(result == 0)
		{
			defaultLog << LogLevel::Error << COMPONENT_TAG << " Failed to destroy a window." << Log::Flush();
			DE_ERROR_WINDOWS(0x000402);
		}
	}

	static HCURSOR loadCursor()
	{
		HANDLE cursorHandle = LoadImageW(nullptr, MAKEINTRESOURCEW(OCR_NORMAL), IMAGE_CURSOR, 0, 0, LR_SHARED);

		if(cursorHandle == nullptr)
			return nullptr;
		else
			return static_cast<HCURSOR>(cursorHandle);
	}

	static RECT createWindowRectangle()
	{
		const GraphicsAdapterManager& graphicsAdapterManager = GraphicsAdapterManager::instance();
		const DisplayMode& currentDisplayMode = graphicsAdapterManager.graphicsAdapters()[0]->currentDisplayMode();

		RECT rectangle;
		rectangle.left = currentDisplayMode.width() / 2 - WINDOW_DEFAULT_WIDTH / 2;
		rectangle.top = currentDisplayMode.height() / 2 - WINDOW_DEFAULT_HEIGHT / 2;
		rectangle.right = rectangle.left + WINDOW_DEFAULT_WIDTH;
		rectangle.bottom = rectangle.top + WINDOW_DEFAULT_HEIGHT;

		const Int32 result = AdjustWindowRectEx(&rectangle, WINDOW_STYLE, 0, 0u);

		if(result == 0)
		{
			defaultLog << LogLevel::Error << COMPONENT_TAG << " Failed to create a rectangle for a window." <<
				Log::Flush();

			DE_ERROR_WINDOWS(0x000404);
		}

		return rectangle;
	}

	static LRESULT CALLBACK processMessage(HWND windowHandle, Uint32 message, WPARAM wParam, LPARAM lParam)
	{
		Window::Impl* windowImpl = reinterpret_cast<Window::Impl*>(GetWindowLongPtrW(windowHandle, GWLP_USERDATA));

		switch(message)
		{
			case WM_CLOSE:
				windowImpl->close();
				break;

			case WM_SETCURSOR:
				if(windowImpl->shouldHideCursor(LOWORD(lParam) == HTCLIENT))
					return 1;

			default:
				return DefWindowProcW(windowHandle, message, wParam, lParam);
		}
		
		return 0;
	}
};

const Char8* WindowManager::Impl::COMPONENT_TAG			= "[Platform::WindowManager - Windows]";
const Char16* WindowManager::Impl::WINDOW_CLASS_NAME	= DE_CHAR16("devengine");
const Char16* WindowManager::Impl::WINDOW_DEFAULT_TITLE = DE_CHAR16("DevEngine");


// Public

WindowManager::WindowManager()
	: _impl(nullptr) { }

WindowManager::~WindowManager()
{
	DE_DELETE(_impl, Impl);
}

Window* WindowManager::createWindow()
{
	DE_ASSERT(_impl != nullptr);
	return _impl->createWindowObject();
}

void WindowManager::destroyWindow(Window* window)
{
	DE_ASSERT(_impl != nullptr);
	_impl->destroyWindowObject(window);
}

void WindowManager::initialise()
{
	DE_ASSERT(_impl == nullptr);
	_impl = DE_NEW(Impl)();
}

void WindowManager::processMessages() const
{
	DE_ASSERT(_impl != nullptr);
	Impl::processMessages();
}
