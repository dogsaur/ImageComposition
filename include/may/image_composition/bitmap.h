#ifndef MAY_IMAGE_COMPOSITION_BITMAP
#define MAY_IMAGE_COMPOSITION_BITMAP
#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
#include <fstream>
namespace may
{
namespace image_composition
{
typedef struct Pix Color;
struct Pix
{
	Pix(){}
    Pix(short r, short g, short b)
    {
        R = r;
        G = g;
        B = b;
    }
    short R;
    short G;
    short B;
};
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

struct BmpInfoHeader
{
    // size of this header in the file
    UInt32 info_size;
    // image dimensions
    Int32 width, height;
    // number of planes, always set to one
    UInt16 planes;
    // bits per pixel
    UInt16 bit_count;
    // compression type
    UInt32 compression;
    // image size in bytes, may be zero for 24 bpp images
    UInt32 image_size;
    // image resolution
    Int32 x_pixels_per_meter, y_pixels_per_meter;
    // number of used colors, may be zero
    UInt32 clr_used;
    // number of important colors, may be zero
    UInt32 clr_important;
    void from_stream( std::ifstream & stream);
};

struct BmpFileHeader
{
    // attributes

    // the magic number "BM"
    UInt16 magic;
    // size of the whole file
    UInt32 size;
    // offset (from this field) to the data
    UInt32 offset;

    // methods
    BmpFileHeader()
    {
        magic = 0x4D42;
    }
    void from_stream( std::ifstream & stream);
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
private:
    int width;
    int height;
    BmpFileHeader file_header;
    BmpInfoHeader info_header;
	bool grayscale;
    std::ifstream stream;
    std::vector< std::vector<Color> >color_table;

    void read_data ();
    void read_color_table ();
    void read_1bit_data ();
    void read_rle4_data ();
    void read_4bit_data ();
    void read_rle8_data ();
    void read_8bit_data ();
    void read_rgb_data ();
};
}
}
#endif