#include <allegro.h>

const int fps = 60;

volatile long speed_counter = 0;
void increment_speed_counter() {
	speed_counter++;
}
END_OF_FUNCTION(increment_speed_counter);

class thing {
	public:
	double speed_x;
	double speed_y;
	double phys_speed_y() {
		return -speed_y;
	}
	

	double pos_x;
	double pos_y;
	double phys_pos_y() {
		return SCREEN_H - pos_y;
	}
	
	BITMAP* body;
	
	void set_pos_x(double x) {
		pos_x = x;
	}
	void set_pos_y(double y) {
		pos_y = SCREEN_H - y;
	}

public:
	thing(double x, double y, BITMAP* image) {
		set_pos_x(x);
		set_pos_y(y);
		body = image;
	}
	
	void move(double fps) {
		set_pos_x(pos_x + (speed_x / fps));
		set_pos_y(phys_pos_y() + (phys_speed_y() / fps));
		
		if (phys_pos_y() < 0)
			set_speed_y(-phys_speed_y());
		if ((pos_x < 0) || (pos_x > SCREEN_W))
			set_speed_x(-speed_x);
	}

	void update_speeds(double gravity, double friction, double fps) {
		set_speed_x(speed_x - (friction / fps));
		set_speed_y(phys_speed_y() - (gravity / fps));
	}

	void draw(BITMAP* buffer) {
		draw_sprite(buffer, body, (int)pos_x, (int)pos_y);
	}

	void set_speed_x(double speed) {
		speed_x = speed;
	}
	void set_speed_y(double speed) {
		speed_y = -speed;
	}
};

int main()
{
	allegro_init();
	install_keyboard();
	
	LOCK_VARIABLE(speed_counter);
	LOCK_FUNCTION(increment_speed_counter);
	install_int_ex(increment_speed_counter, BPS_TO_TIMER(fps));
	
	set_color_depth(24);
	set_gfx_mode(GFX_AUTODETECT_WINDOWED, 1024, 768, 0, 0);
	
	BITMAP* image = create_bitmap(10, 10);
	clear_to_color(image, makecol(255, 0, 0));
	
	BITMAP* buffer = create_bitmap(1024, 768);
	
	thing ting (40, 40, image);
	ting.set_speed_x(100);
	ting.set_speed_y(50);
	
	while(!key[KEY_ESC]) {
		while (speed_counter > 0) {
			//Input here
			speed_counter--;
		}
		ting.move(fps);
		ting.update_speeds(9.81, 0, fps);
		ting.draw(buffer);
		
		textprintf_ex(buffer, font, 70, 0, makecol(0, 255, 0), -1, "Speed_x: %i", (int)ting.speed_x);
		textprintf_ex(buffer, font, 70, 10, makecol(0, 255, 0), -1, "Speed_y: %i", (int)ting.phys_speed_y());
		textprintf_ex(buffer, font, 70, 20, makecol(0, 255, 0), -1, "Pos_x: %i", (int)ting.pos_x);
		textprintf_ex(buffer, font, 70, 30, makecol(0, 255, 0), -1, "Pos_y: %i", (int)ting.phys_pos_y());
		
		blit(buffer, screen, 0, 0, 0, 0, 1024,768);
		clear_bitmap(buffer);
	}
	
	destroy_bitmap(buffer);
	destroy_bitmap(image);
	
	return 0;
}
END_OF_MAIN();
