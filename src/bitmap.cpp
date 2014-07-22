#include "may/image_composition/bitmap.h"
namespace may
{
	namespace image_composition
	{
		void BmpFileHeader::from_stream(std::ifstream & stream)
		{
			UInt16 filemagic;
			stream.read((char*)&filemagic, sizeof(filemagic));
			stream.read((char*)&size, sizeof(size));
			stream.seekg(4, std::ios::cur);
			stream.read((char*)&offset, sizeof(offset));
		}

		void BmpInfoHeader::from_stream(std::ifstream & stream)
		{
			const UInt32 info_impl_size = 40;
			stream.read((char*)&info_size, sizeof(info_size));
			stream.read((char*)&width, sizeof(width));
			stream.read((char*)&height, sizeof(height));
			stream.read((char*)&planes, sizeof(planes));
			stream.read((char*)&bit_count, sizeof(bit_count));
			stream.read((char*)&compression, sizeof(compression));
			stream.read((char*)&image_size, sizeof(image_size));
			stream.read((char*)&x_pixels_per_meter, sizeof(x_pixels_per_meter));
			stream.read((char*)&y_pixels_per_meter, sizeof(y_pixels_per_meter));
			stream.read((char*)&clr_used, sizeof(clr_used));
			stream.read((char*)&clr_important, sizeof(clr_important));
			stream.seekg(info_size - info_impl_size, std::ios::cur);
		}


		
		BitmapImage::BitmapImage() {}
		BitmapImage::BitmapImage(std::string& filename)
		{
			Load(filename);
		}
		int BitmapImage::GetHeight()
		{
			return height;
		}
		int BitmapImage::GetWidth()
		{
			return width;
		}
		int BitmapImage::Load(const std::string& filename)
		{
			stream.open(filename.c_str());
			if (!stream.good())
			{
				return 1;
			}
			file_header.from_stream(stream);
			info_header.from_stream(stream);
			grayscale = false; // default is rgb

			//only 24 bit surport
			//if (info_header.bit_count != 24)
			// read_color_table ();
			width = info_header.width;
			height = info_header.height;
			read_data();
			return 0;
		}
		void BitmapImage::read_data()
		{
			// read (and eventually map) the bands
			switch (info_header.bit_count)
			{
				/*case 1:
					read_1bit_data ();
					break;
					case 4:
					if (info_header.compression)
					{
					read_rle4_data ();
					}
					else
					{
					read_4bit_data ();
					}
					break;
					case 8:
					if (info_header.compression)
					{
					read_rle8_data ();
					}
					else
					{
					read_8bit_data ();
					}
					break;*/
			case 24:
				read_rgb_data();
				break;
			}
		}
		void BitmapImage::read_1bit_data(){}
		void BitmapImage::read_4bit_data(){}
		void BitmapImage::read_rle4_data(){}
		void BitmapImage::read_8bit_data(){}
		void BitmapImage::read_rle8_data(){}

		void BitmapImage::read_rgb_data()
		{
			const unsigned pixels_num = info_header.width * info_header.height;
			Pixels.resize(info_header.height, std::vector<Pix>(info_header.width));

			// seek to the data
			stream.seekg(file_header.offset, std::ios::beg);
			const unsigned int pad_size = (3 * info_header.width % 4) ? 4 - (3 * info_header.width % 4) : 0;
			for (int row = info_header.height - 1; row >= 0; --row)
			{

				for (int col = 0; col < info_header.width; ++col)
				{
					Pixels[row][col].B = stream.get();
					Pixels[row][col].G = stream.get();
					Pixels[row][col].R = stream.get();
				}
				stream.seekg(pad_size, std::ios::cur);
			}
		}
	}
}

