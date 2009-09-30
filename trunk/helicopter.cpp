#include <allegro.h>
#include <vector>
#include "shot.cpp"

#define PI 3.14

const float fpsen = 30.0f;
const double gravity = -9.81 / 2;

class helicopter {
public:
    double pos_x, pos_y;
    double speed_x, speed_y;
    double accel_x, accel_y;
    double fric, bounce;
    int size_x, size_y;
    int tilt_angle;
    bool flip;
    std::vector<shot> shots;
    int limiter_shots, limiter_flip;
    int health;

    helicopter(BITMAP* explosion_image[], int frame_count,int Size_x, int Size_y, double Pos_x, double Pos_y, double Speed_x, double Speed_y, double Accel_x, double Accel_y, double Fric, double Bounce) {
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
        tilt_angle = 0;
        limiter_shots = 0;
        limiter_flip = 0;
        health = 800;
        flip = false;
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

    	for (unsigned int x=0;x<shots.size();++x) {
    		if (shots[x].alive)
    			shots[x].update_phys();
    		else
    			shots.erase(shots.begin()+x);
    	}
    }

    void draw(BITMAP* buffer, BITMAP* image, BITMAP* image_shot) {
        // Draw helicopter
        int draw_pos_x = (int)pos_x;
        int draw_pos_y = (int)(SCREEN_H - pos_y - size_y);

        if(!flip) {
        	BITMAP* temp = create_bitmap(size_x, size_y * 2);
        	clear_bitmap(temp);

        	if(!(pos_y<size_y)) {
				if (accel_x>0){
					rotate_sprite(temp, image, 0, size_y / 2, itofix(tilt_angle++));
				}
				else if (accel_x<0){
					rotate_sprite(temp, image, 0, size_y / 2, itofix(tilt_angle--));
				}
				else {
					rotate_sprite(temp, image, 0, size_y / 2, itofix(tilt_angle));
					if (tilt_angle>0) tilt_angle--;
					else if (tilt_angle<0) tilt_angle++;
				}
        	}
        	else {
        		draw_sprite(temp, image, 0, 0);
        	}

        	draw_trans_sprite(buffer, temp, draw_pos_x, draw_pos_y);
        	destroy_bitmap(temp);
        }
        else {
        	BITMAP* temp = create_bitmap(size_x, size_y);
        	BITMAP* temp2 = create_bitmap(size_x, size_y * 2);
        	clear_bitmap(temp);
        	clear_bitmap(temp2);

        	draw_sprite_h_flip(temp, image, 0, 0);

        	if (!(pos_y<size_y)) {
				if (accel_x>0) {
					rotate_sprite(temp2, temp, 0, size_y / 2, itofix(tilt_angle++));
				}
				else if (accel_x<0) {
					rotate_sprite(temp2, temp, 0, size_y / 2, itofix(tilt_angle--));
				}
				else {
					rotate_sprite(temp2, temp, 0, size_y / 2, itofix(tilt_angle));
					if (tilt_angle>0) tilt_angle--;
					else if (tilt_angle<0) tilt_angle++;
				}
        	}
        	else {
				draw_sprite(temp2, temp, 0, 0);
			}

        	draw_trans_sprite(buffer, temp2, draw_pos_x, draw_pos_y);
        	destroy_bitmap(temp2);
        	destroy_bitmap(temp);
        }

    	if (tilt_angle<-20) tilt_angle = -20;
    	else if (tilt_angle>20) tilt_angle = 20;

    	for (unsigned int x=0;x<shots.size();++x)
    		shots[x].draw(buffer, image_shot);
    }

	void explode() {
		//pang->frame = 0;
		speed_x = 0;
		speed_y = 0;
	}
	void explode(double Speed_x, double Speed_y, int health_loss) {
		speed_x = Speed_x * fric;
		speed_y = Speed_y * bounce;
		health -= health_loss;
	}

	void get_input(bool up, bool down, bool left, bool right, bool shooting) {
		if (health>0) {
			if (up) accel_y = 3;
			else accel_y = gravity;

			if (down) {
				if (limiter_flip>30) {
					flip = !flip;
					limiter_flip = 0;
				}
			}
			limiter_flip++;

			if (left) accel_x = -2;
			else if (right) accel_x = 2;
			else accel_x = 0;

			if (shooting) shoot_pre();
		}
		else {
			accel_y = gravity;
			accel_x = 0;
			tilt_angle = (flip) ? -20 : 20;
			bounce = 0;
			fric = 0;
		}
	}

	void shoot() {
		if (limiter_shots>10) {
			double x, y;
			int spd = 32;
			if (speed_x>=0) {
				y = (spd*cos(PI/180*(tilt_angle+90)));
				x = (spd*sin(PI/180*(tilt_angle+90)));
			}
			else {
				y = -(spd*cos(PI/180*(tilt_angle+90)));
				x = -(spd*sin(PI/180*(tilt_angle+90)));
			}
			shots.push_back(shot(pos_x+(size_x/2), pos_y+(size_y/2), x, y, tilt_angle));
			limiter_shots = 0;
		}
		else limiter_shots += 1;
	}
	void shoot_pre() {
		if (limiter_shots>10) {
			double x, y;
			if (!flip) {
				y = angles_y[tilt_angle+20];
				x = angles_x[tilt_angle+20];
			}
			else {
				y = -angles_y[tilt_angle+20];
				x = -angles_x[tilt_angle+20];
			}
			shots.push_back(shot(pos_x+(size_x/2), pos_y+(size_y/2), x, y, tilt_angle));
			limiter_shots = 0;
		}
		else limiter_shots += 1;
	}
};
