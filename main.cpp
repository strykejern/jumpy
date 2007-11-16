#include <allegro.h>
#include <math.h>
#include <iostream>

volatile long speed_counter = 0;
const int fpsen = 20;
const double gravity = -9.81 / 2;

class explosion {
	int frame_count;
	BITMAP** image;
public:
	int pos_x;
	int pos_y;
	int frame;
	
	explosion(BITMAP* Image[], int Frame_count) {
		pos_x = 0;
		pos_y = 0;
		frame = -1;
		image = Image;
		frame_count = Frame_count * 4;
	}
	
	void update_frame() {
		if (frame!=-1) {
			if (frame < frame_count-1)
				frame++;
			else
				frame = -1;
		}
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
   
    int radius;

    ball(int Radius) {
        radius = Radius;
        fric = 1;
        bounce= 1;
    }
    ball(int Radius, double Pos_x, double Pos_y) {
        radius = Radius;
        pos_x = Pos_x;
        pos_y = Pos_y;
        fric = 1;
        bounce= 1;
    }
    ball(int Radius, double Pos_x, double Pos_y, double Speed_x, double Speed_y) {
        radius = Radius;
        pos_x = Pos_x;
        pos_y = Pos_y;
        speed_x = Speed_x;
        speed_x = Speed_x;
        fric = 1;
        bounce= 1;
    }
    ball(int Radius, double Pos_x, double Pos_y, double Speed_x, double Speed_y, double Accel_x, double Accel_y) {
        radius = Radius;
        pos_x = Pos_x;
        pos_y = Pos_y;
        speed_x = Speed_x;
        speed_y = Speed_y;
        accel_x = Accel_x / fpsen;
        accel_y = Accel_y / fpsen;
        fric = 1;
        bounce= 1;
    }
    ball(int Radius, double Pos_x, double Pos_y, double Speed_x, double Speed_y, double Accel_x, double Accel_y, double Fric, double Bounce) {
        radius = Radius;
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
        double new_pos_x = pos_x + (speed_x / fpsen);
        double new_pos_y = pos_y + (speed_y / fpsen);
       
        bool collision_x = false;
        bool collision_y = false;
       
        // If new y position is below ground level
        if (new_pos_y < 0 + radius) {
            collision_y = true;
           
            double t = (radius - pos_y) / speed_y; // pos + t(speed/fpsen) = 0
           
            speed_y = speed_y + (accel_y * t); // Speed when at 0
            speed_y = -(speed_y * bounce); // Reverse speed because we are at 0
            speed_x *= fric;
            pos_y = radius + ((1-t) * speed_y / fpsen); // Move from 0 to rest of t
            speed_y = speed_y + (accel_y * (1-t)); // Speed when moving from 0 to rest of t
        }
        else if (new_pos_y > SCREEN_H - radius) {
            collision_y = true;
           
            double t = (radius - (SCREEN_H - pos_y)) / -speed_y; // pos + t(speed/fpsen) = 0
           
            speed_y = speed_y + (accel_y * t); // Speed when at 0
            speed_y = -speed_y; // Reverse speed because we are at 0
            pos_y = SCREEN_H - (radius + ((1-t) * speed_y / fpsen)); // Move from 0 to rest of t
            speed_y = speed_y + (accel_y * (1-t)); // Speed when moving from 0 to rest of t
        }
       
        // If new x position is below 0(left)
        if (new_pos_x < 0 + radius) {
            collision_x = true;
           
            double t = (radius - pos_x) / speed_x; // pos + t(speed/fpsen) = radius
           
            speed_x = speed_x + (accel_x * t); // Speed when at 0
            speed_x = -speed_x; // Reverse speed because we are at 0
            pos_x = radius + ((1-t) * speed_x / fpsen); // Move from 0 to rest of t
            speed_x = speed_x + (accel_x * (1-t)); // Speed when moving from 0 to rest of t
        }
        else if (new_pos_x > SCREEN_W - radius) {
            collision_x = true;
           
            double t = (radius - (SCREEN_W - pos_x)) / -speed_x; // pos + t(speed/fpsen) = 0
           
            speed_x = speed_x + (accel_x * t); // Speed when at 0
            speed_x = -speed_x; // Reverse speed because we are at 0
            pos_x = SCREEN_W - (radius + ((1-t) * speed_x / fpsen)); // Move from 0 to rest of t
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
   
    void draw(BITMAP* buffer) {
    	// Draw circle
        int draw_pos_x = (int)(pos_x);
        int draw_pos_y = (int)(SCREEN_H - pos_y);
        circlefill(buffer, draw_pos_x, draw_pos_y, radius, makecol(255, 0, 0));
        
        // Draw rectangle
        //int draw_pos_x = (int)pos_x - radius;
        //int draw_pos_y = (int)(SCREEN_H - pos_y - radius);
        //draw_sprite(buffer, rectangle, draw_pos_x, draw_pos_y);
    }
};

class rectangle {
public:
    double pos_x, pos_y;
   
    double speed_x, speed_y;
   
    double accel_x, accel_y;
    
    double fric, bounce;
   
    int size_x, size_y;
    
    explosion* pang;

    rectangle(BITMAP* explosion_image[], int frame_count,int Size_x, int Size_y) {
        size_x = Size_x;
        size_y = Size_y;
        fric = 1;
        bounce= 1;
        pang = new explosion(explosion_image, frame_count);
    }
    rectangle(BITMAP* explosion_image[], int frame_count,int Size_x, int Size_y, double Pos_x, double Pos_y) {
        size_x = Size_x;
        size_y = Size_y;
        pos_x = Pos_x;
        pos_y = Pos_y;
        fric = 1;
        bounce= 1;
        pang = new explosion (explosion_image, frame_count);
    }
    rectangle(BITMAP* explosion_image[], int frame_count,int Size_x, int Size_y, double Pos_x, double Pos_y, double Speed_x, double Speed_y) {
        size_x = Size_x;
        size_y = Size_y;
        pos_x = Pos_x;
        pos_y = Pos_y;
        speed_x = Speed_x;
        speed_x = Speed_x;
        fric = 1;
        bounce= 1;
        pang = new explosion (explosion_image, frame_count);
    }
    rectangle(BITMAP* explosion_image[], int frame_count,int Size_x, int Size_y, double Pos_x, double Pos_y, double Speed_x, double Speed_y, double Accel_x, double Accel_y) {
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
        pang = new explosion (explosion_image, frame_count);
    }
    rectangle(BITMAP* explosion_image[], int frame_count,int Size_x, int Size_y, double Pos_x, double Pos_y, double Speed_x, double Speed_y, double Accel_x, double Accel_y, double Fric, double Bounce) {
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
        pang = new explosion (explosion_image, frame_count);
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
        // Draw rectangle
        int draw_pos_x = (int)pos_x;
        int draw_pos_y = (int)(SCREEN_H - pos_y - size_y);
        
        if(speed_x>0) {
        	BITMAP* temp = create_bitmap(size_x, size_y * 2);
        	
        	if(!(pos_y<size_y)) {
				if (accel_x>0)
					rotate_sprite(temp, image, 0, size_y / 2, itofix(20));
				else if (accel_x<0)
					rotate_sprite(temp, image, 0, size_y / 2, itofix(-20));
				else {
					destroy_bitmap(temp);
					temp = image;
				}
        	}
        	else {
        		destroy_bitmap(temp);
        		temp = image;
        	}
        	
        	draw_trans_sprite(buffer, temp, draw_pos_x, draw_pos_y);
        }
        else if(speed_x<0) {
        	BITMAP* temp = create_bitmap(size_x, size_y);
        	BITMAP* temp2 = create_bitmap(size_x, size_y * 2);
        	
        	draw_sprite_h_flip(temp, image, 0, 0);
        	
        	if (!(pos_y<size_y)) {
				if (accel_x>0)
					rotate_sprite(temp2, temp, 0, size_y / 2, itofix(20));
				else if (accel_x<0)
					rotate_sprite(temp2, temp, 0, size_y / 2, itofix(-20));
				else {
					destroy_bitmap(temp2);
					temp2 = temp;
				}
        	}
        	else {
				destroy_bitmap(temp2);
				temp2 = temp;
			}
        	
        	draw_trans_sprite(buffer, temp2, draw_pos_x, draw_pos_y);
        	destroy_bitmap(temp);
        }
    
    	pang->draw(buffer);
    	pang->update_frame();
    	if (pang->frame>-1)
    		std::cout << pang->frame << std::endl;
    }
	
	void explode() {
		if (pang->frame==-1) {
			pang->pos_x = (int)pos_x + (size_x / 2);
			pang->pos_y = (int)pos_y - (size_y / 2);
			pang->frame = 0;
			speed_x = 0.01;
			speed_y = 0.01;
		}
	}
};

bool check_collision(ball ballen, rectangle heli);

void increment_speed_counter() {
    speed_counter++;
}
END_OF_FUNCTION(increment_speed_counter);

int main() {
    allegro_init();
    install_keyboard();
    install_timer();
   
    LOCK_VARIABLE(speed_counter);
    LOCK_FUNCTION(increment_speed_counter);
    install_int_ex(increment_speed_counter, BPS_TO_TIMER(60));
   
    const int width = 800;
    const int height = 600;
   
    set_color_depth(32);
    set_gfx_mode(GFX_AUTODETECT_WINDOWED, width, height, 0, 0);
    set_alpha_blender();
	
    BITMAP* buffer = create_bitmap(width, height);
    
    BITMAP* image = load_bitmap("./sprites/chopper.tga", 0);
    BITMAP* image2 = load_bitmap("./sprites/chopper.tga", 0);
    
    BITMAP* background = load_bitmap("./sprites/background.bmp", 0);
    
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
	
    rectangle heli(image_explosion, 17,image->w, image->h, 40, 40, 0, 0, 0, gravity / 2, 0.8, 0.5);
    rectangle heli2(image_explosion, 17,image2->w, image2->h, 40, 40, 0, 0, 0, gravity / 2, 0.8, 0.5);
    ball ballen(10, 70, 80, 40, 20, 0, gravity * 2);
   
    while (!key[KEY_ESC]) {
        while (speed_counter > 0) {
        	if (key[KEY_LEFT]) heli.accel_x = -1;
        	else if (key[KEY_RIGHT]) heli.accel_x = 1;
        	else heli.accel_x = 0;
        	
        	if (key[KEY_UP]) heli.accel_y = 3;
        	else if (key[KEY_DOWN]) heli.accel_y = gravity -0.5;
        	else heli.accel_y = gravity;
        	
        	if (key[KEY_A]) heli2.accel_x = -1;
        	else if (key[KEY_D]) heli2.accel_x = 1;
        	else heli2.accel_x = 0;
        	
        	if (key[KEY_W]) heli2.accel_y = 3;
        	else if (key[KEY_S]) heli2.accel_y = gravity -0.5;
        	else heli2.accel_y = gravity;
        	
            heli.update_phys();
            heli2.update_phys();
            ballen.update_phys();
            speed_counter--;
            
            if (check_collision(ballen, heli))
            	heli.explode();
            if (check_collision(ballen, heli2))
            	heli2.explode();
        }
        
		blit(background, buffer, 0, 0, 0, 0, width, height);
		ballen.draw(buffer);
        heli.draw(buffer, image);
        heli2.draw(buffer, image2);
       
        blit(buffer, screen, 0, 0, 0, 0, width, height);
        clear_bitmap(buffer);
    }
   
    destroy_bitmap(buffer);
    
   
    return 0;
}
END_OF_MAIN();

bool check_collision(ball ballen, rectangle heli) {
	bool collision = true;
	
	int heli_bb_top 	= (int)heli.pos_y + heli.size_y;
	int heli_bb_left 	= (int)heli.pos_x;
	int heli_bb_bottom 	= (int)heli.pos_y;
	int heli_bb_right 	= (int)heli.pos_x + heli.size_x;
	
	int ballen_bb_top	= (int)ballen.pos_y + ballen.radius;
	int ballen_bb_left	= (int)ballen.pos_x - ballen.radius;
	int ballen_bb_bottom= (int)ballen.pos_y - ballen.radius;
	int ballen_bb_right	= (int)ballen.pos_x + ballen.radius;
	
	if 		(heli_bb_top	< ballen_bb_bottom) collision = false;
	else if (heli_bb_right	< ballen_bb_left) 	collision = false;
	else if (heli_bb_bottom > ballen_bb_top) 	collision = false;
	else if (heli_bb_left	> ballen_bb_right) 	collision = false;
	
	return collision;
}
