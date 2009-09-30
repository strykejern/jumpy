#include <allegro.h>
#include <math.h>
#include <vector>
#include <time.h>
#include "shot.cpp"
#include "explosion.cpp"
#include "ball.cpp"
#include "helicopter.cpp"

#define PI 3.14

volatile long speed_counter = 0;
const int fpsen = 20;
const double gravity = -9.81 / 2;
double angles_x[41];
double angles_y[41];



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
	const int spd = 16;
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
    
    //BITMAP* image3 = load_bitmap("./sprites/chopper3.tga", 0);
    BITMAP* image2 = load_bitmap("./sprites/chopper.tga", 0);
    BITMAP* image = load_bitmap("./sprites/chopper2.tga", 0);
    
    BITMAP* background = load_bitmap("./sprites/background.bmp", 0);
    
    BITMAP* image_sphere = load_bitmap("./sprites/sphere1.tga", 0);
    BITMAP* image_sphere2 = load_bitmap("./sprites/sphere2.tga", 0);
    
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
    heli.push_back(helicopter(image_explosion, 17,image->w,  image->h,  40,  40, 0, 0,   0, gravity / 2, 0.8, 0.5));
    heli.push_back(helicopter(image_explosion, 17,image2->w, image2->h, 720, 40, 0, 0,   0, gravity / 2, 0.8, 0.5));
    //heli.push_back(helicopter(image_explosion, 17,image3->w, image3->h, 350, 200, 0, 100, 0, gravity / 2, 0.8, 0.5));
    heli[0].flip = !heli[0].flip;
    
    srand(time(NULL));
    
    std::vector<ball> spheres;
	for (int x=0;x<10;++x) 
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
        	heli[1].get_input(key[KEY_UP], key[KEY_DOWN], key[KEY_LEFT], key[KEY_RIGHT], true);
        	heli[0].get_input(key[KEY_W], key[KEY_S], key[KEY_A], key[KEY_D], true);
        	//heli[2].get_input(key[KEY_Y], key[KEY_H], key[KEY_G], key[KEY_J], true);
        	
        	for (unsigned int x=0;x<heli.size();++x) {
            	if (!(heli[x].health>0))explosions.push_back(explosion(image_explosion, 17, (int)heli[x].pos_x, (int)heli[x].pos_y, rand()%(heli[x].size_x*2), rand()%(heli[x].size_y*2)));
        		heli[x].update_phys();
        	}
            
            for (unsigned int x=0;x<spheres.size();++x) {
            	spheres[x].update_phys();
            }
            	
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
						if (heli[x].health>0) explosions.push_back(explosion(image_explosion, 17, (int)heli[x].pos_x, (int)heli[x].pos_y, heli[x].size_x, heli[x].size_y));
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
            	if (heli[0].health>0) explosions.push_back(explosion(image_explosion, 17, (int)heli[0].pos_x, (int)heli[0].pos_y, heli[0].size_x, heli[0].size_y));
            	if (heli[1].health>0) explosions.push_back(explosion(image_explosion, 17, (int)heli[1].pos_x, (int)heli[1].pos_y, heli[1].size_x, heli[1].size_y));
            }
            
            for (unsigned int x=0;x<heli.size();++x) {
            	for (unsigned int y=0;y<heli[x].shots.size();++y) {
            		for (unsigned int z=0;z<heli.size();++z)
            			if (z!=x)
            				if (check_collision(heli[x].shots[y], heli[z])) {
            					heli[z].explode((heli[z].pos_x - heli[x].shots[y].pos_x)+heli[z].speed_x, (heli[z].pos_y - heli[x].shots[y].pos_y)+heli[z].speed_y,10);
            					if (heli[z].health>0) explosions.push_back(explosion(image_explosion, 17, (int)heli[z].pos_x, (int)heli[z].pos_y));
            				}
            		for (unsigned int z=0;z<spheres.size();++z)
            			if (check_collision(heli[x].shots[y], spheres[z])) {
							spheres[z].explode((spheres[z].pos_x - heli[x].shots[y].pos_x)+spheres[z].speed_x, (spheres[z].pos_y - heli[x].shots[y].pos_y)+spheres[z].speed_y);
							explosions.push_back(explosion(image_explosion, 17, (int)spheres[z].pos_x, (int)spheres[z].pos_y));
							if (z==0) {
								spheres.erase(spheres.begin());
								spheres.push_back(ball(image_sphere->w,image_sphere->h,(double)((int)rand()%(SCREEN_W-image_sphere->w)),(double)((int)rand()%(SCREEN_H-image_sphere->h)),(double)((int)rand()%60),(double)((int)rand()%60),0.0,gravity * 2,0.95,0.95));
								heli[x].health += 50;
							}
						}
            	}
            	for (unsigned int y=x+1;y<heli.size();++y)
            		if (check_collision(heli[x], heli[y])) {
            			heli[x].explode((heli[x].pos_x - heli[y].pos_x)+heli[x].speed_x, (heli[x].pos_y - heli[y].pos_y)+heli[x].speed_y,10);
            			if (heli[x].health>0) explosions.push_back(explosion(image_explosion, 17, (int)heli[x].pos_x, (int)heli[x].pos_y));
            			heli[y].explode((heli[y].pos_x - heli[x].pos_x)+heli[y].speed_x, (heli[y].pos_y - heli[x].pos_y)+heli[y].speed_y,10);
            			if (heli[y].health>0) explosions.push_back(explosion(image_explosion, 17, (int)heli[y].pos_x, (int)heli[y].pos_y));
            		}
            }
            
            speed_counter--;
        }
        
		blit(background, buffer, 0, 0, 0, 0, width, height);
		
		for (unsigned int x=0;x<spheres.size();++x) {
			if (x==0)
				spheres[x].draw(buffer, image_sphere2);
			else
				spheres[x].draw(buffer, image_sphere);
		}
			
        heli[0].draw(buffer, image, image_shot);
        heli[1].draw(buffer, image2, image_shot);
        //heli[2].draw(buffer, image3, image_shot);
        
        draw_health(buffer, 10, 10, heli[0].health);
        draw_health(buffer, SCREEN_W-110, 10, heli[1].health);
        //draw_health(buffer, SCREEN_W/2, 10, heli[2].health);
        
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
	health = (health>0) ? health/8 : 0;
	int color = 0;
	if (health>70&&health<=100) color = makecol(0,255,0);
	else if (health>30&&health<=70) color = makecol(255,128,0);
	else if (health<30) color = makecol(255,0,0);
	else if (health>100) color = makecol(0,255,255);
	rectfill(buffer, pos_x  , pos_y  , pos_x+101, pos_y+10, makecol(0  ,0,0));
	rectfill(buffer, pos_x+1, pos_y+1, pos_x+((health<=100)?health:100), pos_y+9 , color);
	textprintf_ex(buffer, font, pos_x+40, pos_y+2, makecol(255,255,255), -1, "%i", health);
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
	
	int heli_col_x = 0;
	int heli_col_y = 0;
	int heli_bb_top 	= (int)heli.pos_y + (heli.size_y - heli_col_y);
	int heli_bb_left 	= (int)heli.pos_x + heli_col_x;
	int heli_bb_bottom 	= (int)heli.pos_y + heli_col_y;
	int heli_bb_right 	= (int)heli.pos_x + (heli.size_x - heli_col_x);
	
	int heli2_col_x = 0;
	int heli2_col_y = 0;
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
