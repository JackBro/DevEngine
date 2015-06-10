/**
 * @file content/ContentLoader.h
 *
 * DevEngine
 * Copyright 2015 Eetu 'Devenec' Oinasmaa
 */

#pragma once

#include <core/FileStream.h>

namespace Content
{
	// TODO: figure out how to create each ContentLoader<T> once, reuse them and destroy them when no longer needed
	template<typename T>
	class ContentLoader
	{
	public:

		ContentLoader(const ContentLoader& contentLoader) = delete;
		ContentLoader(ContentLoader&& contentLoader) = delete;
		

		virtual T* load(Core::FileStream& fileStream) = 0;

		ContentLoader& operator =(const ContentLoader& contentLoader) = delete;
		ContentLoader& operator =(ContentLoader&& contentLoader) = delete;

		static ContentLoader<T>* loader();

	protected:

		ContentLoader() = default;

		virtual ~ContentLoader() = default;

	private:

		friend class ContentManager;
	};
}