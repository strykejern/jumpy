#include <allegro.h>
#include <math.h>
#include <vector>
#include <time.h>

#define PI 3.14

volatile long speed_counter = 0;
const int fpsen = 20;
const double gravity = -9.81 / 2;
double angles_x[41];
double angles_y[41];

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
        health = 500;
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

bool check_collision(ball, ball);
bool check_collision(ball, helicopter);
bool check_collision(helicopter, helicopter);
bool check_collision(shot, helicopter);
bool check_collision(shot, ball);
void draw_health(BITMAP*, int, int, int);

void increment_speed_counter() {
    speed_counter++;
}
END_OF_FUNCTION(increment_speed_counter);

int main() {
	const int spd = 32;
	for (int x=0;x<41;++x) {
		angles_y[x] = (spd*cos(PI/180*(x-20+90)));
		angles_x[x] = (spd*sin(PI/180*(x-20+90)));
	}
	
    allegro_init();
    install_keyboard();
    install_timer();
   
    LOCK_VARIABLE(speed_counter);
    LOCK_FUNCTION(increment_speed_counter);
    install_int_ex(increment_speed_counter, BPS_TO_TIMER(60));
   
    const int width = 1024;
    const int height = 768;
   
    set_color_depth(32);
    set_gfx_mode(GFX_AUTODETECT_WINDOWED, width, height, 0, 0);
    set_alpha_blender();
	
    BITMAP* buffer = create_bitmap(width, height);
    
    BITMAP* image2 = load_bitmap("./sprites/chopper.tga", 0);
    BITMAP* image = load_bitmap("./sprites/chopper2.tga", 0);
    
    BITMAP* background = load_bitmap("./sprites/background.bmp", 0);
    
    BITMAP* image_sphere = load_bitmap("./sprites/sphere1.tga", 0);
    
    BITMAP* image_shot = load_bitmap("./sprites/shot.tga", 0);
    
    BITMAP* image_explosion[17];
    
    image_explosion[0] = load_bitmap("./sprites/exp_frame_0.tga", 0);
    image_explosion[1] = load_bitmap("./sprites/exp_frame_1.tga", 0);
    image_explosion[2] = load_bitmap("./sprites/exp_frame_2.tga", 0);
    image_explosion[3] = load_bitmap("./sprites/exp_frame_3.tga", 0);
    image_explosion[4] = load_bitmap("./sprites/exp_frame_4.tga", 0);
    image_explosion[5] = load_bitmap("./sprites/exp_frame_5.tga", 0);
    image_explosion[6] = load_bitmap("./sprites/exp_frame_6.tga", 0);
    image_explosion[7] = load_bitmap("./sprites/exp_frame_7.tga", 0);
    image_explosion[8] = load_bitmap("./sprites/exp_frame_8.tga", 0);
    image_explosion[9] = load_bitmap("./sprites/exp_frame_9.tga", 0);
    image_explosion[10] = load_bitmap("./sprites/exp_frame_10.tga", 0);
    image_explosion[11] = load_bitmap("./sprites/exp_frame_11.tga", 0);
    image_explosion[12] = load_bitmap("./sprites/exp_frame_12.tga", 0);
    image_explosion[13] = load_bitmap("./sprites/exp_frame_13.tga", 0);
    image_explosion[14] = load_bitmap("./sprites/exp_frame_14.tga", 0);
    image_explosion[15] = load_bitmap("./sprites/exp_frame_15.tga", 0);
    image_explosion[16] = load_bitmap("./sprites/exp_frame_16.tga", 0);
	
	std::vector<helicopter> heli;
    heli.push_back(helicopter(image_explosion, 17,image->w, image->h, 40, 40, 0, 0, 0, gravity / 2, 0.8, 0.5));
    heli.push_back(helicopter(image_explosion, 17,image2->w, image2->h, 720, 40, 0, 0, 0, gravity / 2, 0.8, 0.5));
    heli[0].flip = !heli[0].flip;
    
    srand(time(NULL));
    
    std::vector<ball> spheres;
	for (int x=0;x<01;++x) 
		spheres.push_back(
			ball(
				image_sphere->w,
				image_sphere->h,
				(double)((int)rand()%(SCREEN_W-image_sphere->w)),
				(double)((int)rand()%(SCREEN_H-image_sphere->h)),
				(double)((int)rand()%60),
				(double)((int)rand()%60),
				0.0,
				gravity * 2,
				0.95,
				0.95));
	
	std::vector<explosion> explosions;
	
    while (!key[KEY_ESC]) {
        while (speed_counter > 0) {
        	heli[0].get_input(key[KEY_UP], key[KEY_DOWN], key[KEY_LEFT], key[KEY_RIGHT], true);
        	heli[1].get_input(key[KEY_W], key[KEY_S], key[KEY_A], key[KEY_D], true);
        	
        	for (unsigned int x=0;x<heli.size();++x)
        		if (heli[x].health>0)
        			heli[x].update_phys();
            
            for (unsigned int x=0;x<spheres.size();++x)
            	spheres[x].update_phys();
            	
            for (unsigned int x=0;x<explosions.size();++x)
            	if (explosions[x].alive)
            		explosions[x].update_frame();
            	else
            		explosions.erase(explosions.begin()+x);
            
            for (unsigned int x=0;x<heli.size();++x)
				for (unsigned int y=0;y<spheres.size();++y)
					if (check_collision(spheres[y], heli[x])) {
						heli[x].explode((heli[x].pos_x - spheres[y].pos_x)+heli[x].speed_x, (heli[x].pos_y - spheres[y].pos_y)+heli[x].speed_y, 1);
						spheres[y].explode((spheres[y].pos_x - heli[x].pos_x)+spheres[y].speed_x, (spheres[y].pos_y - heli[x].pos_y)+spheres[y].speed_y);
						if (heli[x].health>0) explosions.push_back(explosion(image_explosion, 17, (int)heli[x].pos_x, (int)heli[x].pos_y));
						explosions.push_back(explosion(image_explosion, 17, (int)spheres[y].pos_x, (int)spheres[y].pos_y));
					}
            
            for (unsigned int x=0;x<spheres.size();++x)
            	for (unsigned int y=x+1;y<spheres.size();++y)
					if (check_collision(spheres[x], spheres[y])) {
						spheres[x].explode((spheres[x].pos_x - spheres[y].pos_x)+spheres[x].speed_x, (spheres[x].pos_y - spheres[y].pos_y)+spheres[x].speed_y);
						spheres[y].explode((spheres[y].pos_x - spheres[x].pos_x)+spheres[y].speed_x, (spheres[y].pos_y - spheres[x].pos_y)+spheres[y].speed_y);
						explosions.push_back(explosion(image_explosion, 17, (int)spheres[x].pos_x, (int)spheres[x].pos_y));
						explosions.push_back(explosion(image_explosion, 17, (int)spheres[y].pos_x, (int)spheres[y].pos_y));
					}
            
            if (check_collision(heli[0], heli[1])) {
            	heli[0].explode((heli[0].pos_x - heli[1].pos_x)+heli[0].speed_x, (heli[0].pos_y - heli[1].pos_y)+heli[0].speed_y, 5);
            	heli[1].explode((heli[1].pos_x - heli[0].pos_x)+heli[1].speed_x, (heli[1].pos_y - heli[0].pos_y)+heli[1].speed_y, 5);
            	if (heli[0].health>0) explosions.push_back(explosion(image_explosion, 17, (int)heli[0].pos_x, (int)heli[0].pos_y));
            	if (heli[1].health>0) explosions.push_back(explosion(image_explosion, 17, (int)heli[1].pos_x, (int)heli[1].pos_y));
            }
            
            for (unsigned int x=0;x<heli.size();++x)
            	for (unsigned int y=0;y<heli[x].shots.size();++y) {
            		for (unsigned int z=0;z<heli.size();++z)
            			if (z!=x)
            				if (check_collision(heli[x].shots[y], heli[z])) {
            					heli[z].explode(heli[z].speed_x,heli[z].speed_y,10);
            					if (heli[z].health>0) explosions.push_back(explosion(image_explosion, 17, (int)heli[z].pos_x, (int)heli[z].pos_y));
            				}
            		for (unsigned int z=0;z<spheres.size();++z)
            			if (check_collision(heli[x].shots[y], spheres[z])) {
							spheres[z].explode(spheres[z].speed_x,spheres[z].speed_y);
							explosions.push_back(explosion(image_explosion, 17, (int)spheres[z].pos_x, (int)spheres[z].pos_y));
						}
            	}
            
            speed_counter--;
        }
        
		blit(background, buffer, 0, 0, 0, 0, width, height);
		
		for (unsigned int x=0;x<spheres.size();++x)
			spheres[x].draw(buffer, image_sphere);
			
        heli[0].draw(buffer, image, image_shot);
        heli[1].draw(buffer, image2, image_shot);
        
        draw_health(buffer, 10, 10, heli[0].health);
        draw_health(buffer, SCREEN_W-110, 10, heli[1].health);
        
        for (unsigned int x=0;x<explosions.size();++x)
        	explosions[x].draw(buffer);
		
        blit(buffer, screen, 0, 0, 0, 0, width, height);
        clear_bitmap(buffer);
    }
   
    destroy_bitmap(buffer);
    
   
    return 0;
}
END_OF_MAIN();

bool check_collision(ball heli2, ball heli) {
	bool collision = true;
	
	int heli_col_x = heli.size_x / 8;
	int heli_col_y = heli.size_y / 8;
	int heli_bb_top 	= (int)heli.pos_y + (heli.size_y - heli_col_y);
	int heli_bb_left 	= (int)heli.pos_x + heli_col_x;
	int heli_bb_bottom 	= (int)heli.pos_y + heli_col_y;
	int heli_bb_right 	= (int)heli.pos_x + (heli.size_x - heli_col_x);
	
	int heli2_col_x = heli2.size_x / 8;
	int heli2_col_y = heli2.size_y / 8;
	int heli2_bb_top	= (int)heli2.pos_y + (heli2.size_y - heli2_col_y);
	int heli2_bb_left	= (int)heli2.pos_x + heli2_col_x;
	int heli2_bb_bottom = (int)heli2.pos_y + heli2_col_y;
	int heli2_bb_right	= (int)heli2.pos_x + (heli2.size_x - heli2_col_x);
	
	if 		(heli_bb_top	< heli2_bb_bottom)	collision = false;
	else if (heli_bb_right	< heli2_bb_left) 	collision = false;
	else if (heli_bb_bottom > heli2_bb_top) 	collision = false;
	else if (heli_bb_left	> heli2_bb_right) 	collision = false;
	
	return collision;
}

void draw_health(BITMAP* buffer, int pos_x, int pos_y, int health) {
	health = (health>0) ? health : 0;
	rectfill(buffer, pos_x  , pos_y  , pos_x+101, pos_y+10, makecol(0  ,0,0));
	rectfill(buffer, pos_x+1, pos_y+1, pos_x+health, pos_y+9 , makecol(255,0,0));
	textprintf_ex(buffer, font, pos_x+40, pos_y+2, makecol(0,255,0), -1, "%i", health);
}

bool check_collision(ball heli2, helicopter heli) {
	bool collision = true;
	
	int heli_col_x = heli.size_x / 8;
	int heli_col_y = heli.size_y / 8;
	int heli_bb_top 	= (int)heli.pos_y + (heli.size_y - heli_col_y);
	int heli_bb_left 	= (int)heli.pos_x + heli_col_x;
	int heli_bb_bottom 	= (int)heli.pos_y + heli_col_y;
	int heli_bb_right 	= (int)heli.pos_x + (heli.size_x - heli_col_x);
	
	int heli2_col_x = heli2.size_x / 8;
	int heli2_col_y = heli2.size_y / 8;
	int heli2_bb_top	= (int)heli2.pos_y + (heli2.size_y - heli2_col_y);
	int heli2_bb_left	= (int)heli2.pos_x + heli2_col_x;
	int heli2_bb_bottom = (int)heli2.pos_y + heli2_col_y;
	int heli2_bb_right	= (int)heli2.pos_x + (heli2.size_x - heli2_col_x);
	
	if 		(heli_bb_top	< heli2_bb_bottom)	collision = false;
	else if (heli_bb_right	< heli2_bb_left) 	collision = false;
	else if (heli_bb_bottom > heli2_bb_top) 	collision = false;
	else if (heli_bb_left	> heli2_bb_right) 	collision = false;
	
	return collision;
}

bool check_collision(helicopter heli, helicopter heli2) {
	bool collision = true;
	
	int heli_col_x = heli.size_x / 8;
	int heli_col_y = heli.size_y / 8;
	int heli_bb_top 	= (int)heli.pos_y + (heli.size_y - heli_col_y);
	int heli_bb_left 	= (int)heli.pos_x + heli_col_x;
	int heli_bb_bottom 	= (int)heli.pos_y + heli_col_y;
	int heli_bb_right 	= (int)heli.pos_x + (heli.size_x - heli_col_x);
	
	int heli2_col_x = heli2.size_x / 8;
	int heli2_col_y = heli2.size_y / 8;
	int heli2_bb_top	= (int)heli2.pos_y + (heli2.size_y - heli2_col_y);
	int heli2_bb_left	= (int)heli2.pos_x + heli2_col_x;
	int heli2_bb_bottom = (int)heli2.pos_y + heli2_col_y;
	int heli2_bb_right	= (int)heli2.pos_x + (heli2.size_x - heli2_col_x);
	
	if 		(heli_bb_top	< heli2_bb_bottom)	collision = false;
	else if (heli_bb_right	< heli2_bb_left) 	collision = false;
	else if (heli_bb_bottom > heli2_bb_top) 	collision = false;
	else if (heli_bb_left	> heli2_bb_right) 	collision = false;
	
	return collision;
}

bool check_collision(shot heli, helicopter heli2) {
	bool collision = true;
	
	int heli_col_x = heli.size_x / 8;
	int heli_col_y = heli.size_y / 8;
	int heli_bb_top 	= (int)heli.pos_y + (heli.size_y - heli_col_y);
	int heli_bb_left 	= (int)heli.pos_x + heli_col_x;
	int heli_bb_bottom 	= (int)heli.pos_y + heli_col_y;
	int heli_bb_right 	= (int)heli.pos_x + (heli.size_x - heli_col_x);
	
	int heli2_col_x = heli2.size_x / 8;
	int heli2_col_y = heli2.size_y / 8;
	int heli2_bb_top	= (int)heli2.pos_y + (heli2.size_y - heli2_col_y);
	int heli2_bb_left	= (int)heli2.pos_x + heli2_col_x;
	int heli2_bb_bottom = (int)heli2.pos_y + heli2_col_y;
	int heli2_bb_right	= (int)heli2.pos_x + (heli2.size_x - heli2_col_x);
	
	if 		(heli_bb_top	< heli2_bb_bottom)	collision = false;
	else if (heli_bb_right	< heli2_bb_left) 	collision = false;
	else if (heli_bb_bottom > heli2_bb_top) 	collision = false;
	else if (heli_bb_left	> heli2_bb_right) 	collision = false;
	
	return collision;
}

bool check_collision(shot heli, ball heli2) {
	bool collision = true;
	
	int heli_col_x = heli.size_x / 8;
	int heli_col_y = heli.size_y / 8;
	int heli_bb_top 	= (int)heli.pos_y + (heli.size_y - heli_col_y);
	int heli_bb_left 	= (int)heli.pos_x + heli_col_x;
	int heli_bb_bottom 	= (int)heli.pos_y + heli_col_y;
	int heli_bb_right 	= (int)heli.pos_x + (heli.size_x - heli_col_x);
	
	int heli2_col_x = heli2.size_x / 8;
	int heli2_col_y = heli2.size_y / 8;
	int heli2_bb_top	= (int)heli2.pos_y + (heli2.size_y - heli2_col_y);
	int heli2_bb_left	= (int)heli2.pos_x + heli2_col_x;
	int heli2_bb_bottom = (int)heli2.pos_y + heli2_col_y;
	int heli2_bb_right	= (int)heli2.pos_x + (heli2.size_x - heli2_col_x);
	
	if 		(heli_bb_top	< heli2_bb_bottom)	collision = false;
	else if (heli_bb_right	< heli2_bb_left) 	collision = false;
	else if (heli_bb_bottom > heli2_bb_top) 	collision = false;
	else if (heli_bb_left	> heli2_bb_right) 	collision = false;
	
	return collision;
}
