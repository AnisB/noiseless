// Library includes
#include "ppm_export.h"
#include "base/log.h"

namespace noiseless
{
	bool export_rgb_to_ppm(const char* filename, int width, int height, const uint8_t* raw_image)
	{
		// try to open the file
		FILE *file = fopen( filename, "wb" );

		// were we able to open it?
	  	if ( file ) 
	  	{
	  		// write the header
			fprintf( file, "P6 %d %d 255\n", width, height );

			// spit the buffer
			fwrite( raw_image, sizeof(uint8_t), width * height * 3, file );

			// close the file
			fclose( file );

			// done
			return true;
		}
		else 
		{
	    	PRINT_ERROR( "Could not open the file" << filename );
			return false;
		}
	}
}