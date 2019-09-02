#include <allegro.h>

#include "bezigon.h"


int main()
{
	Bezigon bez;
	
	srand(time(0));
	allegro_init(); /* init allegro */
	install_keyboard();
	install_mouse();
	
	set_gfx_mode(GFX_AUTODETECT, 640, 480, 0, 0);
	
	
	bez.set_color_fill(makecol(128,128,255));
	
	show_mouse(screen);
	do
	{
		scare_mouse();
		clear_bitmap(screen);
		unscare_mouse();
		
		bez.nuke_nodes(); // clean the bezigon
	
		while (!(mouse_b&2))
		{
	
			if (mouse_b&1)
			{
				bez.cadd_node(mouse_x, mouse_y,  0.25 ); //(float)(rand()%100) / 100.0);
				scare_mouse();
				
				putpixel(screen, mouse_x, mouse_y, makecol(255,255,255) );
				
				// render while drawing -- optional
				clear(screen);
				bez.set_quality(32); // dibuja en baja calidad al ir mostrando
	 			bez.draw(screen); // debug, ir dibujando mientras agrega puntos!
				// end of render while drawing
				
				unscare_mouse();
				while (mouse_b);
			}
	
		}
	
		scare_mouse();
		
		bez.set_quality(64);
		bez.set_show_tangents(TRUE);
		bez.set_show_control_points(TRUE);
		bez.set_show_debug_points(FALSE);
		
		bez.draw(screen);
	
		
		unscare_mouse();
	} while(readkey()>>8 != KEY_ESC);
	
	
	set_gfx_mode(GFX_TEXT,0,0,0,0);
	
	bez.dump_print_nodes();
	bez.nuke_nodes();
	
	return 0;
} 
END_OF_MAIN();
