#include <allegro.h>
#include <math.h>

volatile long speed_counter = 0;
volatile long color_counter = 0;

void increment_speed_counter() {
	speed_counter++;
}
END_OF_FUNCTION(increment_speed_counter);

void increment_color_counter() {
	color_counter++;
}
END_OF_FUNCTION(increment_color_counter);

int main()
{
	allegro_init();
	install_keyboard();
	
	const int fps = 60;
	
	LOCK_VARIABLE(color_counter);
	LOCK_FUNCTION(increment_color_counter);
	install_int_ex(increment_color_counter, BPS_TO_TIMER(40));
	int color = 0;
	
	LOCK_VARIABLE(speed_counter);
	LOCK_FUNCTION(increment_speed_counter);
	install_int_ex(increment_speed_counter, BPS_TO_TIMER(fps));
	
	set_color_depth(24);
	set_gfx_mode(GFX_AUTODETECT_WINDOWED, 640, 480, 0, 0);
	
	BITMAP* square;
	square = create_bitmap(10, 10);
	clear_to_color(square, makecol(255, 0, 0));
	
	BITMAP* buffer = create_bitmap(640, 480);
	
	double square_x_pos = 50;
	double square_y_pos = 400;
	
	double speed = 0;
	double gravity = -9.81;
	
	int height = 10;
	
	while(!key[KEY_ESC]) {
		while(speed_counter > 0) {
			if(key[KEY_UP])
				gravity -= 0.1;
			else if(key[KEY_DOWN])
				gravity += 0.1;
			speed_counter--;
		}
		square_y_pos += speed / fps;
		speed += gravity / fps;
		if(square_y_pos<0) {
			speed = -speed;
			square_y_pos = 0;
		}
		if (color_counter > 255) color_counter = 0;
		/*square = create_bitmap(10, (int)((speed * speed) / speed));*/
		clear_to_color(square, makecol(color_counter, 0, 0));
		height = (int)(sqrt(speed * speed));
		stretch_sprite(buffer, square, (int)square_x_pos, (480 - height) - (int)square_y_pos, 10, height);
		/*draw_sprite(buffer, square, (int)square_x_pos, 470 - (int)square_y_pos);*/
		textprintf_ex(buffer, font, 70, 0, makecol(0, 255, 0), -1, "Speed: %ipx/s", (int)speed);
		textprintf_ex(buffer, font, 70, 10, makecol(0, 255, 0), -1, "Height: %ipx", (int)square_y_pos);
		textprintf_ex(buffer, font, 70, 20, makecol(0, 255, 0), -1, "Gravity: %f", -gravity);
		textprintf_ex(buffer, font, 70, 30, makecol(0, 255, 0), -1, "M: 3kg");
		textprintf_ex(buffer, font, 70, 40, makecol(0, 255, 0), -1, "Ep: %i", (int)(3 * -gravity * square_y_pos));
		textprintf_ex(buffer, font, 70, 50, makecol(0, 255, 0), -1, "Ek: %i", (int)((3 * speed * speed) / 2));
		
		blit(buffer, screen, 0, 0, 0, 0, 640, 480);
		clear_bitmap(buffer);
	
	}
	
	destroy_bitmap(square);
	destroy_bitmap(buffer);
	
	return 0;
}
END_OF_MAIN();
