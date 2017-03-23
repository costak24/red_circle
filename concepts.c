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
#define FOV 60
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
	raycaster_init();
	player_init();
	raycaster();

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
	for(vga_x = 0; vga_x <= 319; vga_x++)
	{
		for(vga_y = 0; vga_y <= 200; vga_y++)
		{
			vga_drawPixel(vga_x, vga_y, 0x0);
		}
	}
}

/***************************************************************************************************************************************************************************************************************/
//initialize defaults

void raycaster_init()
{
	//define ray variables for position and direction
	double castray_Xpos; //current map coordinates, not to be confused with map squares
	double castray_Ypos;
	double castray_Xdir;
	double castray_Ydir;
	double castray_Xlength;
	double castray_Ylength;
	double castray_Xinc;
	double castray_Yinc;

	int castray_Xsquare; //NEXT map square, must be int
	int castray_Ysquare;
	int castray_wallFlag = 0; //found wall?
	int castray_walldir;  //facing y or x?
	int castray_firstCast = 0;

	//time variables
	double time_last = 0;
	double time_current = 0;
	double time_total = 0;
}

void player_init()
{
	//player position
	double player_Xpos = 4; //just a test position
	double player_Ypos = 22;

	int player_mapCol = int(player_Xpos>>cubeResolution); //use in both ray trace and player movement?
	int player_mapRow = int(player_Ypos>>cubeResolution);

	//player's view direction 
	double player_Xdir = -1; //facing south initially
	double player_Ydir = 0; 

	/* player's FOV plane
	 * must be perpendicular to direction vector.
	 * to change FOV, ratio of direction vector to FOV plane must be adjusted 
	 */

	double player_Xplane = 0;
	double player_Yplane = .6;  //60% fov

	//raycaster casts columns, so only an x coordinate is needed
	double player_Xcast;
	double player_Xcam;
}

void raycaster()
{
	double angleInc = 0.1875;
	double angleNew = -1 * (1 / FOV);
	//ray begins at the players position
	castray_Xpos = player_Xpos;
	castray_Ypos = player_Ypos;

	//320 columns for de2
	for(player_Xcast = 0; player_Xcast <= 319; player_Xcast++)
	{
		while(castray_wallFlag == 0)
		{
			//INITIAL distance to next horizontal and vertical intersections
			if(castray_firstCast == 0)
			{
				//next intersection in direction + fmod pixels = distance
				(player_Xdir < 0) ? castray_Xsquare = east : castray_Xsquare = west;
				(player_Ydir < 0) ? castray_Ysquare = south : castray_Ysquare = north;
				(castray_findDirX < 0) ? castray_Xlength = cubeResolution - fmod(player_Xpos, cubeResolution) : castray_Xlength = fmod(player_Xpos, cubeResolution);
				(castray_findDirY < 0) ? castray_Ylength = cubeResolution - fmod(player_Ypos, cubeResolution) : castray_Ylength = fmod(player_Ypos, cubeResolution);
				castray_firstCast++;
			}
			else
			{
				(player_Xdir < 0) ? castray_Xsquare = east : castray_Xsquare = west;
				(player_Ydir < 0) ? castray_Ysquare = south : castray_Ysquare = north;
			}

			if(castray_Xlength >= castray_Ylength)//use xinc variable lengths, not total vector lengths
			{
				//if y is shorter check y (vert)
				castray_Xinc = cubeResolution;
				castray_Yinc = castray_Xinc * tan(angleNew);

				if(red_circle_B[player_mapCol][player_mapRow + castray_Ysquare] > 0)
				{
					castray_wallFlag = 1;
					castray_Ylength *= cos(angleNew);
					color = red_circle_B[player_mapCol][player_mapRow];
				}
				else
				{
					castray_Ylength += castray_Yinc;
				}
			}
			else if(castray_Xlength < castray_Ylength)
			{
				//if x is shorter check x (horiz)
				castray_Yinc = cubeResolution;
				castray_Xinc = castray_Yinc / tan(angleNew);

				if(red_circle_B[player_mapCol + castray_Xsquare][player_mapRow] > 0)
				{
					castray_wallFlag = 1;
					castray_Xlength *= cos(angleNew);
					color = red_circle_B[player_mapCol][player_mapRow];
				}
				else
				{
					castray_Xlength += castray_Xinc;
				}
			}
		}
		double sliceDist = sqrt(pow(castray_Xlength, 2) + pow(castray_Ylength, 2));
		int projHeight = (cubeResolution / sliceDist) * 277;
		vga_castCol(projHeight);
		//check player movement, game data values
		angleNew += angleInc;
		castray_wallFlag = 0;
		castray_firstCast = 0;
	}
}

int vga_castCol(int projHeight /*variables for floor and ceiling?*/)
{
	int vga_castPixel;
	for(vga_castPixel = 0; vga_castPixel <= 199; vga_castPixel++)
	{
		if(vga_castPixel < ((1 / 2) * (200 - projHeight))
		{
			color = 0;
		}
		else if(vga_castPixel >= projHeight + (1 / 2) * (200 - projHeight))
		{
			color = 0;
		}

		switch(color)
		{
			case 0:
				color = 0x0;
				break;
			case 1:
				color = 0x10;
				break;
			case 2:
				color = 0x0;
				break;
			case 3:
				color = 0x0;
				break;
			case 4:
				color = 0x38;
				break;
			case 5:
				color = 0x46;
				break;
			case 6:
				color = 0x82;
				break;
		}
		vga_drawPixel(player_Xcast, vga_castPixel, color);
	}
}