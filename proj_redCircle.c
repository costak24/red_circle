/**************************************************************************************************************************************************************************************************************/
/* the purpose of this program is to create a first person shooter modeling the events of the movie, John Wick
 * the rendering of first person views will be completed by using the technique of raycasting
 * this program is compatible with an Altera DE2 media computer
 * support for Altera DE0-CV is not offered at this time


/**************************************************************************************************************************************************************************************************************/

#include <stdio.h>
#include <math.h>
#include <time.h>

#include "worldMaps.h"


#define playerHeight 32
#define cubeResolution 64
#define south -1 // -y
#define east -1  // -x
#define north 1  //  y
#define west 1   //  x
/**************************************************************************************************************************************************************************************************************/

//many variables are type double for compatibility w/ math.h


int main(void)
{
	//game initializers here
	//homescreen

}

/***************************************************************************************************************************************************************************************************************/
//vga setup

//draws a colored pixel to the vga port
void vga_drawPixel(int vga_x, int vga_y, short vga_color)
{
	volatile char *pixel_address = (volatile char*) (0x08000000 + (vga_y<<7) + (vga_x));
	*pixel_address = vga_color;
}

//writes all black to the vga port
void clearScreen()
{
	int vga_x, vga_y;
	for(vga_x = 0; vga_x <= 79; vga_x++)
	{
		for(vga_y = 0; vga_y <= 59; vga_y++)
		{
			vga_drawPixel(vga_x, vga_y, 0x0);
		}
	}
}

/***************************************************************************************************************************************************************************************************************/

