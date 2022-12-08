#include <iostream>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>

#ifdef __APPLE__
#include <libpng16/png.h>
#else
#include <png.h>
#endif

#include "core.h"
#include "texture_png.h"

namespace ushine
{
	namespace core
	{

	class texture_loader_png::pimpl
	{
	public:

		FILE* fp;
		unsigned int   png_width;
		unsigned int   png_height;

		bool load_png(const char* path,texture& tex, application* app)
		{
			
 		    png_byte header[8];

			std::stringstream str;
		    
		    str << "* Loading PNG texture, path=" << path;
		    app->get_logger()->LogLine(str.str());
		    str.clear();


			fp = fopen(path, "rb");
			if (!fp)
			{
				app->get_logger()->LogLine(std::string("# Error: invalid path"));
				return false;
			}

			// read the header
		    fread(header, 1, 8, fp);

		    if (png_sig_cmp(header, 0, 8))
		    {
				app->get_logger()->LogLine(std::string("# Error: Not a PNG file"));
		      //  fprintf(stderr, "error: %s is not a PNG.\n", path);
		        fclose(fp);
		        return false;
		    }

			png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
		    if (!png_ptr)
		    {
		    	app->get_logger()->LogLine(std::string("# Error: png_create_read_struct returned 0"));		        
		        fclose(fp);
		        return false;
		    }

		    // create png info struct
		    png_infop info_ptr = png_create_info_struct(png_ptr);
		    if (!info_ptr)
		    {
		    	app->get_logger()->LogLine(std::string("# Error: png_create_info_struct returned 0"));
		        png_destroy_read_struct(&png_ptr, (png_infopp)NULL, (png_infopp)NULL);
		        fclose(fp);
		        return false;
		    }
		   // create png info struct
		    png_infop end_info = png_create_info_struct(png_ptr);
		    if (!end_info)
		    {
		    	app->get_logger()->LogLine(std::string("# Error: png_create_info_struct returned 0"));
		        png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp) NULL);
		        fclose(fp);
		        return false;
		    }		    
		    // the code in this if statement gets called if libpng encounters an error
		    if (setjmp(png_jmpbuf(png_ptr))) {
		    	app->get_logger()->LogLine(std::string("# Error: error from libpng"));
		        png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
		        fclose(fp);
		        return false;
		    }

		    // init png reading
		    png_init_io(png_ptr, fp);

		    // let libpng know you already read the first 8 bytes
		    png_set_sig_bytes(png_ptr, 8);

		    // read all the info up to the image data
		    png_read_info(png_ptr, info_ptr);

		    // variables to pass to get info
		    int bit_depth, color_type;
		    png_uint_32 temp_width, temp_height;

		    // get info about png
		    png_get_IHDR(png_ptr, info_ptr, &temp_width, &temp_height, &bit_depth, &color_type,
		        NULL, NULL, NULL);

		    png_width = temp_width; 
		    png_height = temp_height; 

		    // Update the png info struct.
		    png_read_update_info(png_ptr, info_ptr);

		    // Row size in bytes.
		    int rowbytes = png_get_rowbytes(png_ptr, info_ptr);
		    int bpp = (rowbytes/png_width)*8;

		    // glTexImage2d requires rows to be 4-byte aligned
		    rowbytes += 3 - ((rowbytes-1) % 4);

		    // Allocate the image_data as a big block, to be given to opengl
		    png_byte * image_data;
		    image_data = (png_byte*) malloc(rowbytes * temp_height * sizeof(png_byte)+15);
		    if (image_data == NULL)
		    {
		    	app->get_logger()->LogLine(std::string("# Error: could not allocate memory for PNG image data"));
		        png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
		        fclose(fp);
		        return false;
		    }

		    // row_pointers is for pointing to image_data for reading the png with libpng
		    png_bytep * row_pointers = (png_bytep*)malloc(temp_height * sizeof(png_bytep));
		    if (row_pointers == NULL)
		    {
		    	app->get_logger()->LogLine(std::string("# Error: could not allocate memory for PNG row pointers"));

		        png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
		        free(image_data);
		        fclose(fp);
		        return false;
		    }

		    // set the individual row_pointers to point at the correct offsets of image_data
		    int i;
		    for (i = 0; i < temp_height; i++)
		    {
		        row_pointers[temp_height - 1 - i] = image_data + i * rowbytes;
		    }

		    // read the png into image_data through row_pointers
		    png_read_image(png_ptr, row_pointers);		    

			tex.create(app, png_width, png_height);

			auto tex_buf = (png_byte*)tex.lock();
			
			if (tex_buf)
			{

				auto total_bytes = rowbytes * temp_height * sizeof(png_byte);

				if (bpp==32)
				{
				    for (unsigned i = 0; i < png_height; ++i)
				    {
				        const png_byte* srcBeg = image_data + (rowbytes *(png_height - i - 1));
				        const png_byte* srcEnd = srcBeg + rowbytes;

				        std::copy(srcBeg, srcEnd, tex_buf + rowbytes * i);
				    }

				   
				    for (unsigned i = 0; i< png_width*png_height; i++)
				    {
				    	unsigned char r,g,b,a;

				    	r = tex_buf[i*4+0];
				    	g = tex_buf[i*4+1];
				    	b = tex_buf[i*4+2];
				    	a = tex_buf[i*4+3];

						tex_buf[i*4+0]=b;
						tex_buf[i*4+1]=g;
						tex_buf[i*4+2]=r;
						tex_buf[i*4+3]=a;
				    }			    
				}
				if (bpp==24)
				{
				    for (auto j = 0; j < png_height; j++)
				    {
				    	for (auto i = 0; i < png_width; i++)
				    	{
				    		/*
				    		tex_buf[(i+j*png_width)*4]=image_data[(i+j*png_width)*3];
				    		tex_buf[(i+j*png_width)*4+1]=image_data[(i+j*png_width)*3+1];
				    		tex_buf[(i+j*png_width)*4+2]=image_data[(i+j*png_width)*3+2];
				    		tex_buf[(i+j*png_width)*4+3]=255;				    		
				    		*/
				    		tex_buf[(i+j*png_width)*4]=image_data[(i+j*png_width)*3+2];
				    		tex_buf[(i+j*png_width)*4+1]=image_data[(i+j*png_width)*3+1];
				    		tex_buf[(i+j*png_width)*4+2]=image_data[(i+j*png_width)*3+0];
				    		tex_buf[(i+j*png_width)*4+3]=255;				    		
				    	}
				    }		
				}

			}
			tex.unlock();

		   // clean up
		    png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
		    free(image_data);
		    free(row_pointers);
		    fclose(fp);


		    std::stringstream str2;
		    str2 << "* PNG texture loaded, path=" << path << "; W=" << png_width << "; H=" << png_height << "; BPP=PNG" << bpp;
		    app->get_logger()->LogLine(str2.str()); 

			return true;
		}

	};


	texture_loader_png::texture_loader_png() : impl(new pimpl)
	{

	}

	texture_loader_png::~texture_loader_png()
	{

	}

	bool texture_loader_png::load(texture& tex, std::string& path)
	{

		if (!impl->load_png(path.c_str(), tex, app))
			return false;

		return true;
	}

	};
};
