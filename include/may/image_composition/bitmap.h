#ifndef MAY_IMAGE_COMPOSITION_BITMAP
#define MAY_IMAGE_COMPOSITION_BITMAP
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
namespace may
{
	namespace image_composition
	{
		typedef struct Pix Color;
		typedef unsigned char UInt8;
		typedef unsigned short UInt16;
		//typedef unsigned int UInt32;
		typedef unsigned long UInt32;
		typedef unsigned long long UInt64;
		typedef char Int8;
		typedef short Int16;
		//typedef int Int32;
		typedef long Int32;
		typedef long long Int64;
		struct Pix
		{
			Pix(){}
			Pix(UInt8 r, UInt8 g, UInt8 b)
			{
				R = r;
				G = g;
				B = b;
			}
			UInt8 R;
			UInt8 G;
			UInt8 B;
		};


		struct BmpInfoHeader
		{
			UInt32 info_size;			// size of this header in the file
			Int32 width, height;		// image dimensions
			UInt16 planes;				// number of planes, always set to one
			UInt16 bit_count;			// bits per pixel
			UInt32 compression;			// compression type
			UInt32 image_size;			// image size in bytes, may be zero for 24 bpp images
			Int32 x_pixels_per_meter, y_pixels_per_meter; // image resolution
			UInt32 clr_used;			// number of used colors, may be zero
			UInt32 clr_important;		// number of important colors, may be zero
			void from_stream(std::fstream & stream);
			void to_stream(std::fstream &stream);
		};

		struct BmpFileHeader
		{
			UInt16 magic;    // the magic number "BM"
			UInt32 size;    // size of the whole file
			UInt32 offset;    // offset (from this field) to the data

			BmpFileHeader()
			{
				magic = 0x4D42;
			}
			void from_stream(std::fstream & stream);
			void to_stream(std::fstream &stream);
		};

		class BitmapImage
		{
		public:
			std::vector< std::vector<Pix> > Pixels;
			BitmapImage();
			BitmapImage(std::string& filename);
			int GetWidth();
			int GetHeight();
			int Load(const std::string& filename);
			int Save(const std::string& filename);
		private:
			int width;
			int height;
			BmpFileHeader file_header;
			BmpInfoHeader info_header;
			bool grayscale;
			std::fstream stream;
			std::vector< std::vector<Color> >color_table;

			void read_data();
			void read_color_table();
			void read_1bit_data();
			void read_rle4_data();
			void read_4bit_data();
			void read_rle8_data();
			void read_8bit_data();
			void read_rgb_data();

			void write_data();
		};
	}
}
#endif