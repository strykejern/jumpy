#include <allegro.h>
#include <math.h>

volatile long speed_counter = 0;
const int fpsen = 20;
const double gravity = -9.81 / 2;

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

    rectangle(int Size_x, int Size_y) {
        size_x = Size_x;
        size_y = Size_y;
        fric = 1;
        bounce= 1;
    }
    rectangle(int Size_x, int Size_y, double Pos_x, double Pos_y) {
        size_x = Size_x;
        size_y = Size_y;
        pos_x = Pos_x;
        pos_y = Pos_y;
        fric = 1;
        bounce= 1;
    }
    rectangle(int Size_x, int Size_y, double Pos_x, double Pos_y, double Speed_x, double Speed_y) {
        size_x = Size_x;
        size_y = Size_y;
        pos_x = Pos_x;
        pos_y = Pos_y;
        speed_x = Speed_x;
        speed_x = Speed_x;
        fric = 1;
        bounce= 1;
    }
    rectangle(int Size_x, int Size_y, double Pos_x, double Pos_y, double Speed_x, double Speed_y, double Accel_x, double Accel_y) {
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
    rectangle(int Size_x, int Size_y, double Pos_x, double Pos_y, double Speed_x, double Speed_y, double Accel_x, double Accel_y, double Fric, double Bounce) {
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
    	// Draw circle
        //int draw_pos_x = (int)(pos_x);
        //int draw_pos_y = (int)(SCREEN_H - pos_y);
        //circlefill(buffer, draw_pos_x, draw_pos_y, radius, makecol(255, 0, 0));
        
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
    }
};

class explosion {
	int frame_count;
	BITMAP** image;
public:
	int pos_x;
	int pos_y;
	int frame;
	
	explosion(BITMAP** Image, int Frame_count) {
		pos_x = 0;
		pos_y = 0;
		frame = -1;
		image = Image;
		frame_count = Frame_count;
	}
	
	void update_frame() {
		if (frame!=-1) {
			if (frame < frame_count)
				frame++;
			else
				frame = -1;
		}
	}
	
	void draw(BITMAP* buffer) {
		draw_trans_sprite(buffer, image[frame], pos_x, pos_y);
	}
};

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
    
    BITMAP* background = load_bitmap("./sprites/background.bmp", 0);
    
    BITMAP** image_explosion;
    
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
   
    rectangle heli(image->w, image->h, 40, 40, 0, 0, 0, gravity / 2, 0.8, 0.5);
    ball ballen(10, 40, 40, 40, 20, 0, gravity * 2);
   
    while (!key[KEY_ESC]) {
        while (speed_counter > 0) {
        	if (key[KEY_LEFT]) heli.accel_x = -1;
        	else if (key[KEY_RIGHT]) heli.accel_x = 1;
        	else heli.accel_x = 0;
        	
        	if (key[KEY_UP]) heli.accel_y = 3;
        	else if (key[KEY_DOWN]) heli.accel_y = gravity -0.5;
        	else heli.accel_y = gravity;
        	
            heli.update_phys();
            ballen.update_phys();
            speed_counter--;
        }
        
        
		blit(background, buffer, 0, 0, 0, 0, width, height);
		ballen.draw(buffer);
        heli.draw(buffer, image);
       
        textprintf_ex(buffer, font, 160, 0, makecol(0, 255, 0), -1, "Speed_x: %i", (int)heli.speed_x);
        textprintf_ex(buffer, font, 160, 10, makecol(0, 255, 0), -1, "Speed_y: %i", (int)heli.speed_y);
        textprintf_ex(buffer, font, 160, 20, makecol(0, 255, 0), -1, "Pos_x: %i", (int)heli.pos_x);
        textprintf_ex(buffer, font, 160, 30, makecol(0, 255, 0), -1, "Pos_y: %i", (int)heli.pos_y);
        textprintf_ex(buffer, font, 160, 40, makecol(0, 255, 0), -1, "Accel_y: %f", heli.accel_y);
        textprintf_ex(buffer, font, 160, 50, makecol(0, 255, 0), -1, "Accel_x: %f", heli.accel_x);
        //textprintf_ex(buffer, font, 160, 60, makecol(0, 255, 0), -1, "M: 3kg");
        //int ball_ep = (int)((3 * ((heli.accel_y>0) ? 1 : (heli.accel_y<0) ? -1 : 0) * heli.accel_y * heli.pos_y) + (3 * ((heli.accel_x>0) ? 1 : (heli.accel_x<0) ? -1 : 0) * -heli.accel_x * heli.pos_x));
        //int ball_ek = (int)((3 * heli.speed_x * heli.speed_x / 2) + (3 * heli.speed_y * heli.speed_y / 2));
        //textprintf_ex(buffer, font, 160, 70, makecol(0, 255, 0), -1, "Ep: %i", ball_ep);
        //textprintf_ex(buffer, font, 160, 80, makecol(0, 255, 0), -1, "Ek: %i", ball_ek);
        //textprintf_ex(buffer, font, 160, 90, makecol(0, 255, 0), -1, "Ep + Ek: %i", ball_ep + ball_ek);
       
        blit(buffer, screen, 0, 0, 0, 0, width, height);
        clear_bitmap(buffer);
    }
   
    destroy_bitmap(buffer);
   
    return 0;
}
END_OF_MAIN();
