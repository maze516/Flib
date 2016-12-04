////////////////////////////////////////////////////////////////////////// <!--
/// Copyright (C) 2014-2016 Frontier (fr0nt13r789@gmail.com)           ///
///                                                                    ///
/// Flib is licensed under the terms of GNU GPL.                       ///
/// Therefore you may freely use it in your project,                   ///
/// modify it, redistribute it without any warranty on the             ///
/// condition that this disclaimer is not modified/removed.            ///
/// You may not misclaim the origin of this software.                  ///
///                                                                    ///
/// If you use this software in your program/project a                 ///
/// note about it and an email for the author (fr0nt13r789@gmail.com)  ///
/// is not required but highly appreciated.                            ///
///                                                                    ///
/// You should have received a copy of GNU GPL with this software      ///
///                                                                    ///
////////////////////////////////////////////////////////////////////////// -->
#include <FRONTIER/Graphics/Color.hpp>
#include <FRONTIER/Graphics/Image.hpp>
#include <FRONTIER/System/Vector2.hpp>
#include <FRONTIER/System/Vector3.hpp>
#include <FRONTIER/System/Vector4.hpp>
#include <FRONTIER/System/util/C.hpp>
#include <fstream>
#include <cstring>
/*
#include <FRONTIER/System/Error.hpp>
#include <FRONTIER/System/Rect.hpp>
*/
/*
#define STB_IMAGE_WRITE_IMPLEMENTATION
#define FRONTIER_STBI_WRITE_MEM
#define STBI_NO_SIMD
#include "stb_image/stb_image_write.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image/stb_image.h"

#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include "stb_image/stb_image_resize.h"

#include "ico/ico.hpp"

#include "jpge/jpge.cpp"
*/

namespace fg
{
	/////////////////////////////////////////////////////////////
	Image::Image() : m_texels(nullptr)
	{
		
	}

	/////////////////////////////////////////////////////////////
	Image::~Image()
	{
		delete m_texels;
	}

	/////////////////////////////////////////////////////////////
	Image::Image(fm::vec2s size,Color color) : m_texels(nullptr)
	{
		create(size,color);
	}
	/*
	/////////////////////////////////////////////////////////////
	Image::Image(const std::string &filename);
	*/
	/////////////////////////////////////////////////////////////
	Image::Image(const Image &copy) : m_texels(new Color[copy.m_size.area()]),
									  m_size(copy.m_size)
	{
		std::memcpy(m_texels,copy.m_texels,m_size.area() * sizeof(*m_texels));
	}

	/////////////////////////////////////////////////////////////
	Image::Image(Image &&move) : m_texels(move.m_texels),
								 m_size(move.m_size)
	{
		move.m_texels = nullptr;
		move.m_size = fm::vec2s();
	}
	/*
	/////////////////////////////////////////////////////////////
	Image(const Image &copy,const fm::rect2s &sourceRect);
	*/
	/////////////////////////////////////////////////////////////
	Image::reference Image::create(fm::vec2s size,Color color)
	{
		if (size != m_size)
		{
			m_size = size;
			
			delete m_texels;
			m_texels = new Color[m_size.area()];
		}
		
		Cv(m_size)
			m_texels[x * m_size.h + y] = color;
		
		return *this;
	}

	/////////////////////////////////////////////////////////////
	Image::reference Image::create(fm::vec2s size,const Color *texels)
	{
		if (size != m_size)
		{
			m_size = size;
			
			delete m_texels;
			m_texels = new Color[m_size.area()];
		}
		
		memcpy(m_texels,texels,m_size.area() * sizeof(*texels));
		
		return *this;
	}


	std::string getExtension(const std::string &text)
	{
		int dotPos = text.find_last_of(".");
		std::string ret = text.substr(dotPos+1,text.size()-dotPos);

		// lowercase
		C(ret.length())
			ret[i] = (ret[i]>='A' && ret[i]<='Z') ? (ret[i]-'A'+'a') : ret[i];

		return ret;
	}
	
	namespace priv
	{
		class BMPHeader
		{
		public:
			fm::Uint8  signature[2];
			fm::Uint32 fileSize;
			fm::Uint32 reserved;
			fm::Uint32 offsetToPicData;
		};
		
		class BMPInfoHeader
		{
		public:
			fm::Uint32 headerSize;
			fm::Uint32 width;
			fm::Uint32 height;
			fm::Uint16 planes;
			fm::Uint16 bitsPerPixel;
			fm::Uint32 compression;
			fm::Uint32 imageSize;
			fm::Uint32 yPixelPermeter;
			fm::Uint32 xPixelPermeter;
			fm::Uint32 nColorsPallette;
			fm::Uint32 impColor;
			
			enum {
				ByteCount = sizeof(fm::Uint16)*2 + sizeof(fm::Uint32)*(3+6)
			};
		};
		
		class BMPBitmap
		{
		public:
			BMPHeader header;
			BMPInfoHeader info;
			
			enum {
				ByteCount = sizeof(fm::Uint8)*2 + sizeof(fm::Uint16)*2 + sizeof(fm::Uint32)*(3+3+6)
			};
			
			fm::Uint8 *getData()
			{
				fm::Uint8 *data = new fm::Uint8[ByteCount];
				fm::Uint8 *ptr  = data;
				
				auto add32 = [&](fm::Uint32 val) { C(4) *ptr++ = ((fm::Uint8*)&val)[i]; };
				auto add16 = [&](fm::Uint32 val) { C(2) *ptr++ = ((fm::Uint8*)&val)[i]; };
				auto add8  = [&](fm::Uint32 val) { C(1) *ptr++ = ((fm::Uint8*)&val)[i]; };
				
				add8(header.signature[0]);
				add8(header.signature[1]);
				add32(header.fileSize);
				add32(header.reserved);
				add32(header.offsetToPicData);
				add32(info.headerSize);
				add32(info.width);
				add32(info.height);
				add16(info.planes);
				add16(info.bitsPerPixel);
				add32(info.compression);
				add32(info.imageSize);
				add32(info.yPixelPermeter);
				add32(info.xPixelPermeter);
				add32(info.nColorsPallette);
				add32(info.impColor);
				
				return data;
			}
		};
	}
	
	/////////////////////////////////////////////////////////////
	void Image::saveToFile(const std::string &filename) const
	{
		std::string ext = getExtension(filename);
		
		if (ext == "bmp")
		{
			priv::BMPBitmap bitmap{{
				{'B','M'},
				m_size.area() * sizeof(Color) + priv::BMPBitmap::ByteCount,
				0,
				priv::BMPBitmap::ByteCount
			},{
				priv::BMPInfoHeader::ByteCount,
				m_size.w,
				m_size.h,
				1,
				32,
				0,
				m_size.area() * sizeof(Color),
				0x130B,
				0x130B,
				0,
				0
			}};
			
			
			std::ofstream out(filename.c_str(),std::ios::binary);
			
			fm::Uint8* headerPtr = bitmap.getData();
			
			out.write((char*)headerPtr,priv::BMPBitmap::ByteCount);
			out.write((char*)m_texels,m_size.area() * sizeof(Color));
			
			delete[] headerPtr;
		}
	}
	/*
	/////////////////////////////////////////////////////////////
	reference create(const Image &copy,const fm::rect2s &sourceRect);

	/////////////////////////////////////////////////////////////
	void copyTo(Image &destination,fm::vec2s destPos,const fm::rect2s &sourceRect,bool useAlpha = false) const;

	/////////////////////////////////////////////////////////////
	void copyTo(Image &destination,fm::Size destX,fm::Size destY,const fm::rect2s &sourceRect,bool useAlpha = false) const;

	/////////////////////////////////////////////////////////////
	reference copyFrom(const Image &source,fm::vec2s destPos,const fm::rect2s &sourceRect,bool useAlpha = false);

	/////////////////////////////////////////////////////////////
	reference copyFrom(const Image &source,fm::Size destX,fm::Size destY,const fm::rect2s &sourceRect,bool useAlpha = false);

	/////////////////////////////////////////////////////////////
	void copyTo(Image &destination,fm::vec2s destPos,bool useAlpha = false) const;

	/////////////////////////////////////////////////////////////
	void copyTo(Image &destination,fm::Size destX,fm::Size destY,bool useAlpha = false) const;

	/////////////////////////////////////////////////////////////
	reference copyFrom(const Image &source,fm::vec2s destPos,bool useAlpha = false);

	/////////////////////////////////////////////////////////////
	reference copyFrom(const Image &source,fm::Size destX,fm::Size destY,bool useAlpha = false);

	/////////////////////////////////////////////////////////////
	fm::Result loadFromFile(const std::string &filename);

	/////////////////////////////////////////////////////////////
	fm::Result loadFromMemory(const void *buffer,fm::Size byteCount);

	/////////////////////////////////////////////////////////////
	fm::Result saveToFile(const std::string &filename) const;

	/////////////////////////////////////////////////////////////
	fm::Uint8 *saveToMemory(fm::Size &byteCount,const std::string &ext) const;

	/////////////////////////////////////////////////////////////
	Image scale(fm::Size w,fm::Size h,bool linearFilter = true);

	/////////////////////////////////////////////////////////////
	Image scale(fm::vec2s size,bool linearFilter = true);
	*/
	/////////////////////////////////////////////////////////////
	Image::reference Image::swap(Image &img)
	{
		std::swap(m_texels,img.m_texels);
		std::swap(m_size,img.m_size);
		
		return *this;
	}

	/////////////////////////////////////////////////////////////
	fm::vec2s Image::size() const
	{
		return m_size;
	}

	/////////////////////////////////////////////////////////////
	Color &Image::at(fm::vec2s pos)
	{
		return m_texels[pos.x * m_size.h + pos.y];
	}

	/////////////////////////////////////////////////////////////
	const Color &Image::at(fm::vec2s pos) const
	{
		return m_texels[pos.x * m_size.h + pos.y];
	}

	/////////////////////////////////////////////////////////////
	Image::reference Image::set(fm::vec2s pos,Color color)
	{
		m_texels[pos.x * m_size.h + pos.y] = color;
		
		return *this;
	}

	/////////////////////////////////////////////////////////////
	Color *Image::getPtr()
	{
		return m_texels;
	}

	/////////////////////////////////////////////////////////////
	const Color *Image::getPtr() const
	{
		return m_texels;
	}


	////////////////////////////////////////////////////////////
	Image::reference Image::flipHorizontally()
	{
		Cx(m_size.w/2.f)
			Cy(m_size.h)
				std::swap(at(fm::vec2s(x,y)),at(fm::vec2s(m_size.w-1-x,y)));

		return *this;
	}


	////////////////////////////////////////////////////////////
	Image::reference Image::flipVertically()
	{
		Cx(m_size.w)
			Cy(m_size.h/2.f)
				std::swap(at(fm::vec2s(x,y)),at(fm::vec2s(x,m_size.h-1-y)));

		return *this;
	}
	/*
	/////////////////////////////////////////////////////////////
	static std::vector<Image> loadMultipleImagesFromFile(const std::string &file,fm::Error *error = fm::nullPtr);

	/////////////////////////////////////////////////////////////
	static std::vector<Image> loadMultipleImagesFromMemory(const fm::Uint8 *data,fm::Size byteCount,const std::string &ext,fm::Error *error = fm::nullPtr);

	/////////////////////////////////////////////////////////////
	fm::Result saveMultipleImagesToFile(Image const* const* images,fm::Size imageCount,const std::string &file);

	/////////////////////////////////////////////////////////////
	fm::Size saveMultipleImagesToMemory(Image const* const* images,fm::Size imageCount,fm::Uint8 *(&memory),const std::string &ext);
	*/
	
	
	
	
	
	
	
	/*
	
	std::string getExtension(const std::string &text)
	{
		int dotPos = text.find_last_of(".");
		std::string ret = text.substr(dotPos+1,text.size()-dotPos);

		// lowercase
		C(ret.length())
			ret[i] = (ret[i]>='A' && ret[i]<='Z') ? (ret[i]-'A'+'a') : ret[i];

		return ret;
	}

	/// constructors /////////////////////////////////////////////////////////
	Image::Image() : m_texels(nullptr),
					 m_size(0,0)
	{

	}

	////////////////////////////////////////////////////////////
	Image::Image(fm::Size width,fm::Size height,const Color &color)
	{
		create(width,height,color);
	}

	////////////////////////////////////////////////////////////
	Image::Image(const fm::vec2s &size,const Color &color)
	{
		create(size.w,size.h,color);
	}

	////////////////////////////////////////////////////////////
	Image::Image(const Image &copy)
	{
		create(copy);
	}

	////////////////////////////////////////////////////////////
	Image::Image(const Image &copy,const fm::rect2s &sourceRect)
	{
		create(copy,sourceRect);
	}

	////////////////////////////////////////////////////////////
	Image::Image(const std::string &filename)
	{
		loadFromFile(filename);
	}

	/// functions /////////////////////////////////////////////////////////
	Image::reference Image::create(fm::Size width,fm::Size height,const Color &color)
	{
		if (width && height)
		{
			m_sizeW = width;
			m_sizeH = height;
			m_pixels = std::vector<Color>();
			m_pixels.resize(width*height,color);
		}
		else
			m_sizeW = 0,
			m_sizeH = 0,
			m_pixels = std::vector<Color>();

		return *this;
	}


    ////////////////////////////////////////////////////////////
	Image::reference Image::create(const fm::vec2s &size,const Color &color)
	{
		return create(size.w,size.h,color);
	}


	////////////////////////////////////////////////////////////
	Image::reference Image::create(fm::Size width,fm::Size height,const Color *pixels)
	{
		if (width && height)
		{
			m_sizeW = width;
			m_sizeH = height;
			m_pixels = std::vector<Color>((Color *)pixels,((Color *)pixels)+width*height);
		}
		else
			m_sizeW = 0,
			m_sizeH = 0,
			m_pixels = std::vector<Color>();

		return *this;
	}


	////////////////////////////////////////////////////////////
	Image::reference Image::create(const fm::vec2s &size,const Color *pixels)
	{
		return create(size.w,size.h,pixels);
	}


	////////////////////////////////////////////////////////////
    Image::reference Image::create(const Image &copy)
	{
		create(copy.getSize());
		copyFrom(copy,0,0,fm::rect2s(0,0,0,0));

		return *this;
	}


	////////////////////////////////////////////////////////////
    Image::reference Image::create(const Image &copy,const fm::rect2s &sourceRect)
	{
		create(sourceRect.area() ? sourceRect.size : fm::vec2s(copy.getSize())-sourceRect.pos);
		copyFrom(copy,0,0,sourceRect);

		return *this;
	}


	////////////////////////////////////////////////////////////
	void Image::copyTo(Image &destination,const fm::vec2s &destPos,const fm::rect2s &sourceRect,bool useAlpha) const
	{
		destination.copyFrom(*this,destPos,sourceRect,useAlpha);
	}


	////////////////////////////////////////////////////////////
	void Image::copyTo(Image &destination,fm::Size destX,fm::Size destY,const fm::rect2s &sourceRect,bool useAlpha) const
	{
		destination.copyFrom(*this,destX,destY,sourceRect,useAlpha);
	}


	////////////////////////////////////////////////////////////
	Image::reference Image::copyFrom(const Image &source,fm::Size destX,fm::Size destY,const fm::rect2s &sourceRect,bool useAlpha)
	{
		// only copy if the source is valid
		if (!source.m_sizeW)
			return *this;

		// copy the source rect so for adjusting
		fm::rect2s sRect = sourceRect;

		// copy the whole image
		if (!sRect.area())
		{
			sRect.pos(0,0);
			sRect.size(source.m_sizeW,source.m_sizeH);
		}
		else
		{
			// do not be out of the source
			if (sRect.size.w + sRect.pos.x > source.m_sizeW) sRect.size.w = source.m_sizeW - sRect.pos.x;
			if (sRect.size.h + sRect.pos.y > source.m_sizeH) sRect.size.h = source.m_sizeH - sRect.pos.y;
		}

		// do not be out of the destination
		if (destX + sRect.size.w > m_sizeW) sRect.size.w = m_sizeW - destX;
		if (destY + sRect.size.h > m_sizeH) sRect.size.h = m_sizeH - destY;

		// use alpha mixing
		if (useAlpha)
		{
			/// used technique: http://en.wikipedia.org/wiki/Alpha_compositing#Alpha_blending
			/// for EVERY PIXEL -> slow
			Cxy(sRect.size.w,sRect.size.h)
			{
				// get the source pixell
				fm::vec4 src = source.getPixel(sourceRect.pos+fm::vec2s(x,y));

				// get the destination pixell
				fm::vec4 dst = getPixel(destX+x,destY+y);

				// compute result alpha
				float alpha = src.a+dst.a*(1-src.a);

				// avoid division by 0
				if (!alpha)
					setPixel(destX+x,destY+y,fm::vec4(0,0,0,0));
				else
					setPixel(destX+x,destY+y,fm::vec4(fm::vec3(src*src.a+dst*dst.a*(1.f-src.a))/alpha,alpha));
			}
		}
		else
		{
			fm::Size copyBytesPerRow = sRect.size.w*sizeof(Color);

			// use memcpy (a lot faster)
			if (sRect.size.w == m_sizeW && sRect.size.w == source.m_sizeW)
				std::memcpy(&getPixel(destX,destY),&source.getPixel(sourceRect.pos.x,sourceRect.pos.y), copyBytesPerRow*sRect.size.h);
			else
				C(sRect.size.h)
					std::memcpy(&getPixel(destX,destY+i), &source.getPixel(sourceRect.pos.x,sourceRect.pos.y+i), copyBytesPerRow);
		}
		return *this;
	}


	////////////////////////////////////////////////////////////
	Image::reference Image::copyFrom(const Image &source,const fm::vec2s &destPos,const fm::rect2s &sourceRect,bool useAlpha)
	{
		return copyFrom(source,destPos.x,destPos.y,sourceRect,useAlpha);
	}


	////////////////////////////////////////////////////////////
	void Image::copyTo(Image &destination,const fm::vec2s &destPos,bool useAlpha) const
	{
		destination.copyFrom(*this,destPos,fm::rect2s(0,0,0,0),useAlpha);
	}


	////////////////////////////////////////////////////////////
	void Image::copyTo(Image &destination,fm::Size destX,fm::Size destY,bool useAlpha) const
	{
		destination.copyFrom(*this,destX,destY,fm::rect2s(0,0,0,0),useAlpha);
	}


	////////////////////////////////////////////////////////////
	Image::reference Image::copyFrom(const Image &source,fm::Size destX,fm::Size destY,bool useAlpha)
	{
		return copyFrom(source,destX,destY,fm::rect2s(0,0,0,0),useAlpha);
	}


	////////////////////////////////////////////////////////////
	Image::reference Image::copyFrom(const Image &source,const fm::vec2s &destPos,bool useAlpha)
	{
		return copyFrom(source,destPos.x,destPos.y,fm::rect2s(0,0,0,0),useAlpha);
	}


	////////////////////////////////////////////////////////////
	fm::Result Image::loadFromFile(const std::string &filename)
	{
		// empty current pixels
		m_pixels = std::vector<Color>();

		// ask stbi to load the file
		int width, height, channels;
		unsigned char *ptr = stbi_load(filename.c_str(), &width, &height, &channels, STBI_rgb_alpha);

		// if he succeeded
		if (ptr && width && height)
		{
			// copy the data
			m_sizeW = width;
			m_sizeH = height;
			m_pixels.resize(width * height);
			memcpy(&m_pixels[0], ptr, m_pixels.size()*sizeof(fg::Color));

			// let stbi free its used memory
			stbi_image_free(ptr);

			return fm::Result();
		}

		return fm::Error("IOError","FileNotFound",filename + " couldn't be opened for reading","Image.loadFromFile",__FILE__,__LINE__);
	}

	/////////////////////////////////////////////////////////////
	fm::Result Image::loadFromMemory(const void *buffer,fm::Size byteCount)
	{
		// empty current pixels
		m_pixels = std::vector<Color>();

		// ask stbi to load the file
		int width, height, channels;
		unsigned char *ptr = stbi_load_from_memory((const unsigned char*)buffer, byteCount, &width, &height, &channels, STBI_rgb_alpha);

		// if he succeeded
		if (ptr && width && height)
		{
			// copy the data
			m_sizeW = width;
			m_sizeH = height;
			m_pixels.resize(width * height);
			memcpy(&m_pixels[0], ptr, m_pixels.size());

			// let stbi free its used memory
			stbi_image_free(ptr);

			return fm::Result();
		}

		return fm::Error("STBIError","stbi_failure","Iternal error with stbi: "+std::string(stbi_failure_reason()),"Image.loadFromMemory",__FILE__,__LINE__);
	}


	////////////////////////////////////////////////////////////
	const fm::vec2s &Image::getSize() const
	{
		return *((fm::vec2s*)&m_sizeW);
	}


	////////////////////////////////////////////////////////////
	Image::reference Image::setPixel(fm::Size x,fm::Size y,const Color &color)
	{
		m_pixels[x+y*m_sizeW] = color;
		return *this;
	}


	////////////////////////////////////////////////////////////
	Image::reference Image::setPixel(const fm::vec2s &pos,const Color &color)
	{
		return setPixel(pos.x,pos.y,color);
	}


	////////////////////////////////////////////////////////////
	const Color &Image::getPixel(fm::Size x,fm::Size y) const
	{
		return m_pixels[x+y*m_sizeW];
	}


	////////////////////////////////////////////////////////////
	const Color &Image::getPixel(const fm::vec2s &pos) const
	{
		return getPixel(pos.x,pos.y);
	}


	////////////////////////////////////////////////////////////
	Color &Image::getPixel(fm::Size x,fm::Size y)
	{
		return m_pixels[x+y*m_sizeW];
	}


	////////////////////////////////////////////////////////////
	Color &Image::getPixel(const fm::vec2s &pos)
	{
		return getPixel(pos.x,pos.y);
	}


	////////////////////////////////////////////////////////////
	Color *Image::getPixelsPtr()
	{
		return &m_pixels[0];
	}


	////////////////////////////////////////////////////////////
	const Color *Image::getPixelsPtr() const
	{
		return &m_pixels[0];
	}


	////////////////////////////////////////////////////////////
	Image::reference Image::flipHorizontally()
	{
		Cx(m_sizeW/2.f)
			Cy(m_sizeH)
				std::swap(m_pixels[x+y*m_sizeW],m_pixels[(m_sizeW-x-1)+y*m_sizeW]);

		return *this;
	}


	////////////////////////////////////////////////////////////
	Image::reference Image::flipVertically()
	{
		Cx(m_sizeW)
			Cy(m_sizeH/2.f)
				std::swap(m_pixels[x+y*m_sizeW],m_pixels[x+(m_sizeH-y-1)*m_sizeW]);

		return *this;
	}


	////////////////////////////////////////////////////////////
	fm::Result Image::saveToFile(const std::string &filename) const
	{
		// if the image is valid
		if (!m_pixels.empty() && (m_sizeW > 0) && (m_sizeH > 0))
		{
			// get extension
			std::string extension = getExtension(filename);

			int error = 0;

			if (extension == "bmp")
			{
				// BMP format
				if (stbi_write_bmp(filename.c_str(), m_sizeW, m_sizeH, 4, &m_pixels[0]))
					return fm::Result();
				else
					error = 1;
			}
			else if (extension == "tga")
			{
				// TGA format
				if (stbi_write_tga(filename.c_str(), m_sizeW, m_sizeH, 4, &m_pixels[0]))
					return fm::Result();
				else
					error = 1;
			}
			else if (extension == "jpg" || extension == "jpeg") // its a bit more messy
			{
				if (jpge::compress_image_to_jpeg_file(filename.c_str(), m_sizeW, m_sizeH, 4, (unsigned char *)&m_pixels[0]))
					return fm::Result();
				else
					error = 2;
			}
			else
			{
				std::string pngfilename = filename;

				if (extension != "png") // unkown extension
					pngfilename += ".png";

				// PNG format
				if (stbi_write_png(pngfilename.c_str(), m_sizeW, m_sizeH, 4, &m_pixels[0],0))
					return fm::Result();
				else
					error = 1;
			}

			if (error == 1)
				return fm::Error("STBIError","stbi_failure","Iternal error with stbi: "+std::string(stbi_failure_reason()),"Image.saveToFile",__FILE__,__LINE__);

			if (error == 2)
				return fm::Error("STBIError","stbi_failure","Iternal error with stbi: "+std::string(stbi_failure_reason()),"Image.saveToFile",__FILE__,__LINE__);
		}

		return fm::Error("ImageError","SavingEmptyImage","Image couldn't be saved because it's empty ("+filename+")","Image.saveToFile",__FILE__,__LINE__);
	}

	/////////////////////////////////////////////////////////////
	fm::Uint8 *Image::saveToMemory(fm::Size &byteCount,const std::string &ext) const
	{
		// if the image is valid
		if (!m_pixels.empty() && (m_sizeW > 0) && (m_sizeH > 0))
		{
			// get extension
			std::string extension = getExtension(ext);

			if (extension == "bmp" || extension == "tga")
			{
				std::stringstream ss;
				int result;
				if (extension == "bmp")
					result = stbi_write_bmpMEM(ss,m_sizeW,m_sizeH,4,&m_pixels[0]);
				else
					result = stbi_write_tgaMEM(ss,m_sizeW,m_sizeH,4,&m_pixels[0]);

				if (result)
				{
					byteCount = ss.tellp();
					unsigned char *ret = new unsigned char[byteCount];
					std::string data = ss.str();
					for (fm::Size i=0;i<data.length();i++)
						ret[i] = data[i];
					return ret;
				}

				byteCount = 0;
				return fm::nullPtr;
			}
			else if(extension == "png")
			{
			    int len;
				unsigned char *png = stbi_write_png_to_memMEM((unsigned char *)&m_pixels[0],0, m_sizeW, m_sizeH, 4, &len);
				if (len)
				{
					byteCount = len;
					return png;
				}

				byteCount = 0;
				if (png)
					delete[] png;

				return fm::nullPtr;
			}
			else if(extension == "jpg" || extension == "jpeg") // its a bit more messy
			{
				unsigned char *buffer = fm::nullPtr;
				fm::Size allBytes = m_sizeW*m_sizeH*sizeof(fg::Color);
				byteCount = allBytes/16;
				bool success = false;
				while (byteCount < allBytes*1.5 && !success)
				{
					buffer = new unsigned char[byteCount];
					int bufSize = byteCount;
					success = jpge::compress_image_to_jpeg_file_in_memory(buffer,bufSize,m_sizeW,m_sizeH,4,(unsigned char *)&m_pixels[0]);
					if (!success)
					{
						delete[] buffer;
						byteCount*=2;
					}
				}

				if (!success)
				{
					byteCount = 0;
					return fm::nullPtr;
				}

				unsigned char *ret = new unsigned char[byteCount];
				memcpy(ret,buffer,byteCount);
				delete[] buffer;
				return ret;
			}
		}

		byteCount = 0;
		return fm::nullPtr;
	}

	/////////////////////////////////////////////////////////////
	Image Image::scale(fm::Size w,fm::Size h,bool linearFilter)
	{
		Image ret;
		ret.create(w,h);

		if (linearFilter)
		{
			// use stb's image resize
			if (!stbir_resize_uint8((unsigned char*)&m_pixels[0],m_sizeW,m_sizeH,0,
									(unsigned char*)&ret.m_pixels[0],w,h,0,
									4))
				return Image();
		}
		else
		{
			// simple nearest neighbour interpolation
			float rw = float(m_sizeW)/w;
			float rh = float(m_sizeH)/h;

			for (fm::Size x=0;x<w;x++)
				for (fm::Size y=0;y<h;y++)
					ret.setPixel(x,y,getPixel(x*rw,y*rh));
		}


		return ret;
	}

	/////////////////////////////////////////////////////////////
	Image Image::scale(const fm::vec2s &size,bool linearFilter)
	{
		return scale(size.w,size.h,linearFilter);
	}
	
	/////////////////////////////////////////////////////////////
	Image::reference Image::swap(Image &img)
	{
		m_pixels.swap(img.m_pixels);
		
		fm::Size tmp;
		
		tmp = m_sizeW; m_sizeW = img.m_sizeW; img.m_sizeW = tmp;
		tmp = m_sizeH; m_sizeH = img.m_sizeH; img.m_sizeH = tmp;
		
		return *this;
	}

	/////////////////////////////////////////////////////////////
	std::vector<Image> Image::loadMultipleImagesFromFile(const std::string &file,fm::Error *error)
	{
		std::ifstream in(file.c_str(),std::ios_base::binary);

		if (!in)
		{
			if (error) *error = fm::Error("IOError","FileNotFound",file + " couldn't be opened for reading","Image.loadMultipleImagesFromFile",__FILE__,__LINE__);
			return std::vector<Image>();
		}

		std::string str;

		in.seekg(0, std::ios::end);
		str.reserve(in.tellg());
		in.seekg(0, std::ios::beg);

		str.assign((std::istreambuf_iterator<char>(in)),
					std::istreambuf_iterator<char>());

		std::string errors;

		std::vector<Image> ret = Ico::getImages((const fm::Uint8*)(fm::UintPtr)&str[0],str.length()*sizeof(str[0]),errors);
		if (errors != std::string() && error)
			*error = fm::Error("ImageError","IcoError","Error loading " + file + " : " + errors,"Image.loadMultipleImagesFromFile",__FILE__,__LINE__);

		return ret;
	}


	/////////////////////////////////////////////////////////////
	std::vector<Image> Image::loadMultipleImagesFromMemory(const fm::Uint8 *data,fm::Size byteCount,const std::string &ext,fm::Error *error)
	{
		(void)ext;

		std::string errors;

		std::vector<Image> ret = Ico::getImages(data,byteCount,errors);
		if (errors != std::string() && error)
			*error = fm::Error("ImageError","IcoError","Error reading icon " + errors,"Image.loadMultipleImagesFromMemory",__FILE__,__LINE__);

		return ret;
	}

	/////////////////////////////////////////////////////////////
	fm::Result Image::saveMultipleImagesToFile(Image const* const *images,fm::Size imageCount,const std::string &file)
	{
		if (!imageCount)
			return fm::Result("ImageError","SavingZeroImages","Attempt to save 0 images to icon file " + file,"Image.saveMultipleImagesToFile",__FILE__,__LINE__);

		std::ofstream ki(file.c_str(),std::ios_base::binary);

		if (!ki)
			return fm::Error("IOError","FileNotWritable",file + " couldn't be opened for writing","Image.saveMultipleImagesToFile",__FILE__,__LINE__);


		Ico::writeImages(ki,images,imageCount);

		return fm::Result();
	}


	/////////////////////////////////////////////////////////////
	fm::Size Image::saveMultipleImagesToMemory(Image const* const* images,fm::Size imageCount,fm::Uint8 *(&memory),const std::string &ext = "ico")
	{
		(void)ext;

		if (!imageCount)
			return 0;

		// write to a stringstream
		std::stringstream ss(std::ios_base::binary);
		Ico::writeImages(ss,images,imageCount);

		// read back buffer
		std::string data = ss.str();

		// allocate and fill memory
		memory = new fm::Uint8[data.length()];
		memcpy(memory,&data[0],data.length());

		return data.length();
	}*/
}
