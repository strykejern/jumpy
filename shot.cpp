#include <allegro.h>

class shot {
public:
	double pos_x, pos_y;
	double speed_x, speed_y;
	static int size_x, size_y;
	int tilt_angle;
	int flip;
	bool alive;

	shot (double Pos_x, double Pos_y, double Speed_x, double Speed_y, int Tilt_angle) {
		pos_x = Pos_x;
		pos_y = Pos_y;
		speed_x = Speed_x;
		speed_y = Speed_y;
		tilt_angle = Tilt_angle;
		if (speed_x<0) flip = -1;
		else flip = 1;
		alive = true;
	}

	void update_phys() {
		pos_x += speed_x;
		pos_y += speed_y;
		if ((pos_x<0)||(pos_x>SCREEN_W)) alive = false;
		else if ((pos_y<0)||(pos_y>SCREEN_H)) alive = false;
	}

	void draw(BITMAP* buffer, BITMAP* image) {
		shot::size_x = image->w;
		shot::size_y = image->h;

		BITMAP* temp = create_bitmap(20, 10);
		clear_bitmap(temp);
		if (flip==1) draw_sprite(temp, image, 0, 0);
		else draw_sprite_h_flip(temp, image, 0, 0);

		BITMAP* temp2 = create_bitmap(20, 20);
		clear_bitmap(temp2);
		rotate_sprite(temp2, temp, 0, 20 / 2, itofix(tilt_angle));
		draw_trans_sprite(buffer, temp2, (int)pos_x, SCREEN_H - (int)pos_y);
		destroy_bitmap(temp);
		destroy_bitmap(temp2);
	}
};

int shot::size_x;
int shot::size_y;