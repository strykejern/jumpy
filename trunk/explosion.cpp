#include <allegro.h>

class explosion {
public:
	int pos_x;
	int pos_y;
	int frame;
	BITMAP** image;
	int frame_count;
	bool alive;

	explosion(BITMAP** Image, int Frame_count, int Pos_x, int Pos_y) {
		pos_x = Pos_x+20;
		pos_y = Pos_y+10;
		frame = 0;
		frame_count = Frame_count;
		image = Image;
		alive = true;
	}
	explosion(BITMAP** Image, int Frame_count, int Pos_x, int Pos_y, int size_x, int size_y) {
		pos_x = Pos_x+(size_x/2);
		pos_y = Pos_y+10;
		frame = 0;
		frame_count = Frame_count;
		image = Image;
		alive = true;
	}

	void update_frame() {
		if (frame!=-1) {
			if (frame < frame_count-1)
				frame++;
			else
				frame = -1;
		}
		else alive = false;
	}

	void draw(BITMAP* buffer) {
		if (frame!=-1)
			draw_trans_sprite(buffer, image[frame/4], pos_x - 35, SCREEN_H - pos_y - 50);
	}
};
