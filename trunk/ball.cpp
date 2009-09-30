#include <allegro.h>

const float fpsen = 30.0f;
const double gravity = -9.81 / 2;

class ball {
public:
    double pos_x, pos_y;

    double speed_x, speed_y;

    double accel_x, accel_y;

    double fric, bounce;

    int size_x, size_y;

    ball(int Size_x, int Size_y, double Pos_x, double Pos_y, double Speed_x, double Speed_y, double Accel_x, double Accel_y) {
        size_x = Size_x;
        size_y = Size_y;
        pos_x = Pos_x;
        pos_y = Pos_y;
        speed_x = Speed_x;
        speed_y = Speed_y;
        accel_x = Accel_x / fpsen;
        accel_y = Accel_y / fpsen;
        fric = 1;
        bounce= 1;
    }
    ball(int Size_x, int Size_y, double Pos_x, double Pos_y, double Speed_x, double Speed_y, double Accel_x, double Accel_y, double Fric, double Bounce) {
        size_x = Size_x;
        size_y = Size_y;
        pos_x = Pos_x;
        pos_y = Pos_y;
        speed_x = Speed_x;
        speed_y = Speed_y;
        accel_x = Accel_x / fpsen;
        accel_y = Accel_y / fpsen;
        fric = Fric;
        bounce = Bounce;
    }

    void update_phys() {
		speed_x = (speed_x>100) ? 100 : ((speed_x<-100) ? -100 : speed_x);
		speed_y = (speed_y>200) ? 200 : ((speed_y<-200) ? -200 : speed_y);
		double new_pos_x = pos_x + (speed_x / fpsen);
		double new_pos_y = pos_y + (speed_y / fpsen);

		bool collision_x = false;
		bool collision_y = false;

		// If new y position is below ground level
		if (new_pos_y < 0) {
			collision_y = true;

			double t = pos_y / speed_y; // pos + t(speed) = 0

			speed_y = speed_y + (accel_y * t); // Speed when at 0
			speed_y = -(speed_y * bounce); // Reverse speed because we are at 0
			speed_x *= fric;
			pos_y = (1-t) * speed_y / fpsen; // Move from 0 to rest of t
			speed_y = speed_y + (accel_y * (1-t)); // Speed when moving from 0 to rest of t
		}
		else if (new_pos_y > SCREEN_H - size_y) {
			collision_y = true;

			double t = (size_y - (SCREEN_H - pos_y)) / -speed_y; // pos + t(speed/fpsen) = 0

			speed_y = speed_y + (accel_y * t); // Speed when at 0
			speed_y = -speed_y; // Reverse speed because we are at 0
			pos_y = SCREEN_H - ((1-t) * speed_y / fpsen) - size_y; // Move from 0 to rest of t
			speed_y = speed_y + (accel_y * (1-t)); // Speed when moving from 0 to rest of t
		}

		// If new x position is below 0(left)
		if (new_pos_x < 0) {
			collision_x = true;

			double t = pos_x / speed_x; // pos + t(speed/fpsen) = radius

			speed_x = speed_x + (accel_x * t); // Speed when at 0
			speed_x = -speed_x; // Reverse speed because we are at 0
			pos_x = (1-t) * speed_x / fpsen; // Move from 0 to rest of t
			speed_x = speed_x + (accel_x * (1-t)); // Speed when moving from 0 to rest of t
		}
		else if (new_pos_x > SCREEN_W - size_x) {
			collision_x = true;

			double t = (size_x -(SCREEN_W - pos_x)) / -speed_x; // pos + t(speed/fpsen) = 0

			speed_x = speed_x + (accel_x * t); // Speed when at 0
			speed_x = -speed_x; // Reverse speed because we are at 0
			pos_x = SCREEN_W - ((1-t) * speed_x / fpsen) - size_x; // Move from 0 to rest of t
			speed_x = speed_x + (accel_x * (1-t)); // Speed when moving from 0 to rest of t
		}

		// If no collision, do nomral acceleration and movement
		if (!collision_x) {
			pos_x = new_pos_x;
			speed_x = speed_x + accel_x;
		}
		if (!collision_y) {
		pos_y = new_pos_y;
		speed_y = speed_y + accel_y;
		}
    }

    void draw(BITMAP* buffer, BITMAP* image) {
        // Draw ball/square or whatever
        int draw_pos_x = (int)pos_x;
        int draw_pos_y = (int)(SCREEN_H - pos_y - size_y);

    	draw_trans_sprite(buffer, image, draw_pos_x, draw_pos_y);

    	//pang->draw(buffer);
    	//pang->update_frame();
    }

	void explode() {
		//pang->frame = 0;
		speed_x = 0;
		speed_y = 0;
	}
	void explode(double Speed_x, double Speed_y) {
		//pang->frame = 0;
		speed_x = Speed_x * fric;
		speed_y = Speed_y * bounce;
	}
};
