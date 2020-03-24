// PROJECT: BATTLE CITY
// CREATED BY: LIMBER RODRIGUEZ ROJAS
// LAST MOD DATE: March 23 2020

#include <stdlib.h>
#include <stdio.h> 
#include <SDL/SDL.h> // SDL library
#include <time.h> // to use it as seed for random

typedef struct wall { // border walls
	int x,y; //position
	int w,h; //dimensions
} wall_t;

typedef struct tanque { //tank
	int x,y; //position
	int w,h; //dimensions
	int escudo; // shield
  int distan; // distance
	int vidas; // lifes
} tanque_t;

typedef struct block { // bocks
	int x,y; // position
	int w,h; // dimensions
	int tipo; // type (identifier for bock)
} block_t;

typedef struct bala { // bullet
	int x,y; // position
	int w,h; // dimensions
	int direc; // direction
} bala_t;

typedef struct tanque_enem { // Enemy tank 
	int x,y; // position
	int w,h; // dimensions
	int direc; // direction
	int tipo; // type
	int distan; // distance
	int vidas; // lifes
} tanque_enem_t;

typedef struct power_up { // power ups
	int x,y; // position
	int w,h; // dimensions
	int tipo; // type
} power_up_t;

//Create Arrays and variables to store the info for the game
static wall_t wall[4]; // create all 4 border walls
static tanque_t tanque; // create player
static block_t block[338]; // create array of blocks for the map 
static bala_t bala[20]; // create array for bullets (right now a maximum of 20 bullets on the map)
static bala_t bala_enem[20]; // create bullets for all the enemies (right now a maximum of 20 bullets for enemies on the map)
static tanque_enem_t tanque_enem[6]; // assign array to store enemies for the map (max 6 per map)
static power_up_t power_up; // power up for the game

// --------------------------Init surfaces for the game ----------------------------------------
// Icons and screen
static SDL_Surface *screen;
static SDL_Surface *numbermap;
static SDL_Surface *iconoEnemigo;
static SDL_Surface *iconoVidas;
static SDL_Surface *iconoBandera;
// Player tank
static SDL_Surface *playerTank_up;
static SDL_Surface *playerTank_down;
static SDL_Surface *playerTank_right;
static SDL_Surface *playerTank_left;

// Block of the game
static SDL_Surface *long_steel;
static SDL_Surface *trees2; 
static SDL_Surface *long_block;

// Images for win, loose, and start game
static SDL_Surface *base;
static SDL_Surface *battle_city;
static SDL_Surface *inicio_3;
static SDL_Surface *ganaste;
static SDL_Surface *game_over;
// Type 0 of enemy tank
static SDL_Surface *b_enem_arriba;
static SDL_Surface *b_enem_abajo;
static SDL_Surface *b_enem_der;
static SDL_Surface *b_enem_izq;
// type 1 of enemy tank
static SDL_Surface *b1_enem_arriba;
static SDL_Surface *b1_enem_abajo;
static SDL_Surface *b1_enem_der;
static SDL_Surface *b1_enem_izq;
// type 2 of enemy tank
static SDL_Surface *b2_enem_arriba;
static SDL_Surface *b2_enem_abajo;
static SDL_Surface *b2_enem_der;
static SDL_Surface *b2_enem_izq;
// type 3 of enemy tank color V
static SDL_Surface *b3_enem_arriba_v;
static SDL_Surface *b3_enem_abajo_v;
static SDL_Surface *b3_enem_der_v;
static SDL_Surface *b3_enem_izq_v;
// type 3 of enemy tank color A
static SDL_Surface *b3_enem_arriba_a;
static SDL_Surface *b3_enem_abajo_a;
static SDL_Surface *b3_enem_der_a;
static SDL_Surface *b3_enem_izq_a;
// type 3 of enemy tank color B
static SDL_Surface *b3_enem_arriba_b;
static SDL_Surface *b3_enem_abajo_b;
static SDL_Surface *b3_enem_der_b;
static SDL_Surface *b3_enem_izq_b;
//power ups
static SDL_Surface *powerup_casco;
static SDL_Surface *powerup_tank;
static SDL_Surface *powerup_granada;
// shield for the player
static SDL_Surface *shield_2_black;


static void draw_bala() { // draw bullet
	SDL_Rect src;

	for (int i=0;i<20;i++){
		src.x = bala[i].x;
		src.y = bala[i].y;
		src.w = bala[i].w;
		src.h = bala[i].h;

		int r = SDL_FillRect(screen,&src,255); // BUllets are like a rectangle so, fill rect

		if (r !=0) printf("fill rectangle faliled in func draw_bala");
		
	}
}

static void draw_bala_enem() {// draw enemies bullets
	SDL_Rect src;

	for (int i=0;i<20;i++){ // check for all bullets and its position and print it 
		src.x = bala_enem[i].x;
		src.y = bala_enem[i].y;
		src.w = bala_enem[i].w;
		src.h = bala_enem[i].h;

		int r = SDL_FillRect(screen,&src,255);

		if (r !=0) printf("fill rectangle faliled in func draw_bala_enem()");
		
	}
}

static void draw_background() { // draw color bockborund
 
	SDL_Rect src;

	src.x = 0;
	src.y = 0;
	src.w = (screen->w);
	src.h = (screen->h);

	int r = SDL_FillRect(screen,&src,0);
	
	if (r !=0) printf("Faliled in func draw_background()");
	
}

 void generar_power_up(){ //  Generate power up on the screen on random position and type
	power_up.x = (rand() % 385) + 32;
	power_up.y = (rand() % 321) + 16;
	power_up.tipo = rand() % 3 ;
}

static void check_colision_block(bala_t a[20], block_t b[338]){ // check for collisions for bullet and block
	int left_a, left_b;
	int right_a, right_b;
	int top_a, top_b;
	int bottom_a, bottom_b;

	for (int i = 0; i<338; i++){ // For all the blocks
		for (int j = 0; j<20; j++){ // For all the bullets
			left_b = b[i].x;
			right_b = b[i].x + b[i].w;
			top_b = b[i].y;
			bottom_b = b[i].y + b[i].h;

			left_a = a[j].x;
			right_a = a[j].x + a[j].w;
			top_a = a[j].y;
			bottom_a = a[j].y + a[j].h;
			
			if (((left_a+2) >= left_b) & (bottom_a >= top_b) & (top_a <= bottom_b) & (left_a <= right_b)) { //Make sure the bullet is between the blocks
				if (b[i].tipo != 3){
					a[j].x = 800; // if it hits something, move the bullet outside the screen, except when the type is grass or none
				}
				if ((b[i].tipo == 1) || (b[i].tipo == 4)){
					b[i].x = 800; // if it hits a block, move the block to a position outside the table
				}
			}

			if ((top_a <= 16) || (bottom_a >= 432) || (left_a <= 32) || (right_a >= 448)){ // if bullet is on the border of the screen, get it out
				a[j].x = 800; // Put it on a position outside the screen
			}
		}
	}
}


static void check_colision_bala_tank_enem(bala_t a[20], tanque_enem_t b[6]){
	int left_a, left_b;
	int right_a, right_b;
	int top_a, top_b;
	int bottom_a, bottom_b;


	for (int i = 0; i<6; i++){ // For all tanks
		for (int j = 0; j<20; j++){ // For all bullets of player
			left_b = b[i].x;
			right_b = b[i].x + b[i].w;
			top_b = b[i].y;
			bottom_b = b[i].y + b[i].h;

			left_a = a[j].x;
			right_a = a[j].x + a[j].w;
			top_a = a[j].y;
			bottom_a = a[j].y + a[j].h;
			
			if (((left_a+2) > left_b) & (bottom_a > top_b) & (top_a < bottom_b) & (left_a < right_b)){
				a[j].x = 900; 
				if (b[i].vidas == 1){
					tanque_enem[i].x = 801; //move tank outside of the screen
					tanque_enem[i].vidas = 0;
					if (b[i].tipo == 2) generar_power_up(); // We create a power up when we kill a type 2 enemy. always
					
					break;
				}
				else if (b[i].vidas > 1 & b[i].vidas < 4){ 
					tanque_enem[i].vidas -= 1;
					break;
				}
			}
		}
	}
}

static void check_colision_bala_tank_aliado(bala_t a[20], tanque_t b){
	int left_a, left_b;
	int right_a, right_b;
	int top_a, top_b;
	int bottom_a, bottom_b;

	left_b = b.x;
	right_b = b.x + b.w;
	top_b = b.y;
	bottom_b = b.y + b.h;

	for (int j = 0; j<20; j++){

		left_a = a[j].x;
		right_a = a[j].x + a[j].w;
		top_a = a[j].y;
		bottom_a = a[j].y + a[j].h;
		
		if (((left_a+2) > left_b) & (bottom_a > top_b) & (top_a < bottom_b) & (left_a <= right_b)){
			a[j].x = 800;
			if (tanque.escudo != 1){
				tanque.x = 164; // move to the original pos
				tanque.y = 404;
				tanque.vidas -= 1; // substract a life
				tanque.escudo = 1; // put a shield again
				break;
			}
			
		}
	}
}

static void check_colision_tanks(tanque_enem_t a[6], tanque_t b){ //collision between tanks enemy and player
	int left_a, left_b;
	int right_a, right_b;
	int top_a, top_b;
	int bottom_a, bottom_b;

	left_b = b.x;
	right_b = b.x + b.w;
	top_b = b.y;
	bottom_b = b.y + b.h;

	for (int j = 0; j<6; j++){

		left_a = a[j].x;
		right_a = a[j].x + a[j].w;
		top_a = a[j].y;
		bottom_a = a[j].y + a[j].h;
		
		if ((left_a > left_b) & (bottom_a > top_b) & (top_a < bottom_b) & (left_a <= right_b)){ // collision on the right of the player
			if (tanque.escudo != 1){
				tanque.x = 164; // move to the original pos
				tanque.y = 404;
				tanque.vidas -= 1;
				tanque.escudo = 1;
				break;
			}
			else a[j].direc = 2; // change direction to the opposite site and because of the shield, the player does not die
			
		}
		
		if ((left_a < left_b) & (bottom_a > top_b) & (top_a < bottom_b) & (right_a >= left_b)){ // collision on the left of  the player
			if (tanque.escudo != 1){
				tanque.x = 164;
				tanque.y = 404;
				tanque.vidas -= 1;
				tanque.escudo = 1;
				break;
			}
			else a[j].direc = 3;
			
		}

		if ((bottom_a > bottom_b) & (right_a > left_b) & (left_a < right_b) & (top_a <= bottom_b)){ // collision on the bottom of the player
			if (tanque.escudo != 1){
				tanque.x = 164;
				tanque.y = 404;
				tanque.vidas -= 1;
				tanque.escudo = 1;
				break;
			}
			else a[j].direc = 0;

		}
		if ((bottom_a < (bottom_b)) & (right_a > left_b) & (left_a < right_b) & (bottom_a >= top_b)){ // collisoin on the top of the player
			if (tanque.escudo != 1){
				tanque.x = 164;
				tanque.y = 404;
				tanque.vidas -= 1;
				tanque.escudo = 1;
				break;
			}
			else a[j].direc = 1;
			
		}
		
	}
}

int check_colision_power_ups(power_up_t a, tanque_t b){
	int left_a, left_b;
	int right_a, right_b;
	int top_a, top_b;
	int bottom_a, bottom_b;
	int tipo_a;

	left_b = b.x;
	right_b = b.x + b.w;
	top_b = b.y;
	bottom_b = b.y + b.h;

	left_a = a.x;
	right_a = a.x + a.w;
	top_a = a.y;
	bottom_a = a.y + a.h;
	tipo_a = a.tipo;
	
	if ((left_a > left_b) & (bottom_a > top_b) & (top_a < bottom_b) & (left_a <= right_b)){
		power_up.x = 800; // take icon out of the screen
		return 1;	
	}

	if ((left_a < left_b) & (bottom_a > top_b) & (top_a < bottom_b) & (right_a >= left_b)){
		power_up.x = 800;
		return 1;
	}

	if ((bottom_a > bottom_b) & (right_a > left_b) & (left_a < right_b) & (top_a <= bottom_b)){
		power_up.x = 800;
		return 1;
	}

	if ((bottom_a < (bottom_b)) & (right_a > left_b) & (left_a < right_b) & (bottom_a >= top_b)){
		power_up.x = 800;
		return 1;
	}

	else{
		return 0;
	}
}

static void aplicar_power_ups(){ // apply power ups

	if (check_colision_power_ups(power_up, tanque) == 1){

		if (power_up.tipo == 0) tanque.escudo = 1; // shield
		
		else if (power_up.tipo == 1) tanque.vidas += 1; // 1 up
		
		else{ // bomb= destroy all current enemies on the screen
			if (tanque_enem[3].x == 800){
				tanque_enem[0].x = 801;
				tanque_enem[1].x = 801;
				tanque_enem[2].x = 801;
				tanque_enem[0].vidas = 0;
				tanque_enem[1].vidas = 0;
				tanque_enem[2].vidas = 0;
			}
			else{
				tanque_enem[3].x = 801;
				tanque_enem[4].x = 801;
				tanque_enem[5].x = 801;
				tanque_enem[3].vidas = 0;
				tanque_enem[4].vidas = 0;
				tanque_enem[5].vidas = 0;
			}
		}
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////---------------------------------------Collisions between blocks and ememies--------------------------------------------------------------------/////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//if return value is !=0 collision occured. if return is 0, no collision.
int check_collision_left_enem(tanque_enem_t a, block_t b[338]) {
	int left_a, left_b;
	int right_a, right_b;
	int top_a, top_b;
	int bottom_a, bottom_b;

	left_a = a.x;
	right_a = a.x + a.w;
	top_a = a.y;
	bottom_a = a.y + a.h;

	if (a.x != 800){ // To make sure it is a tank enemy on the screen
		for (int i = 0; i<338; i++){
			if (b[i].tipo != 3){
				left_b = b[i].x;
				right_b = b[i].x + b[i].w;
				top_b = b[i].y;
				bottom_b = b[i].y + b[i].h;
				
				if ((left_a > left_b) & (bottom_a > top_b) & (top_a < bottom_b) & (left_a <= right_b)) return 1;
				
			}
		}
	}
	return 0; // No left collision
}

//if return value is !=0 collision occured. if return is 0, no collision.
int check_collision_right_enem(tanque_enem_t a, block_t b[338]) {
	int left_a, left_b;
	int right_a, right_b;
	int top_a, top_b;
	int bottom_a, bottom_b;

	left_a = a.x;
	right_a = a.x + a.w;
	top_a = a.y;
	bottom_a = a.y + a.h;

	if (a.x != 800){
		for (int i = 0; i<338; i++){
			if (b[i].tipo != 3){
				left_b = b[i].x;
				right_b = b[i].x + b[i].w;
				top_b = b[i].y;
				bottom_b = b[i].y + b[i].h;
				
				if ((left_a < left_b) & (bottom_a > top_b) & (top_a < bottom_b) & (right_a >= left_b)) return 1;
				
			}		
			
		}
	}

	return 0; // NO right collision
}

//if return value is !=0 collision occured. if return is 0, no collision.
int check_collision_up_enem(tanque_enem_t a, block_t b[338]) {
	int left_a, left_b;
	int right_a, right_b;
	int top_a, top_b;
	int bottom_a, bottom_b;

	left_a = a.x;
	right_a = a.x + a.w;
	top_a = a.y;
	bottom_a = a.y + a.h;

	if (a.x != 800){
		for (int i = 0; i<338; i++){
			if (b[i].tipo != 3){
				left_b = b[i].x;
				right_b = b[i].x + b[i].w;
				top_b = b[i].y;
				bottom_b = b[i].y + b[i].h;
				
				if ((bottom_a > bottom_b) & (right_a > left_b) & (left_a < right_b) & (top_a <= bottom_b)) return 1;	
			}
		}
	}
	return 0; // No up collision
}

//if return value is !=0 collision occured. if return is 0, no collision.
int check_collision_down_enem(tanque_enem_t a, block_t b[338]) {
	int left_a, left_b;
	int right_a, right_b;
	int top_a, top_b;
	int bottom_a, bottom_b;

	left_a = a.x;
	right_a = a.x + a.w;
	top_a = a.y;
	bottom_a = a.y + a.h;

	if (a.x != 800){
		for (int i = 0; i<338; i++){
			if (b[i].tipo != 3){
				left_b = b[i].x;
				right_b = b[i].x + b[i].w;
				top_b = b[i].y;
				bottom_b = b[i].y + b[i].h;
				
				if ((bottom_a < (bottom_b)) & (right_a > left_b) & (left_a < right_b) & (bottom_a >= top_b)) return 1;
				
			}
		}
	}
	return 0;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------Collision between enemies-----------------------------------------------------------------


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//if return value is !=0 collision occured. if return is 0, no collision.
int check_collision_left_enem_enem(tanque_enem_t a, tanque_enem_t b[6]) {
	int left_a, left_b;
	int right_a, right_b;
	int top_a, top_b;
	int bottom_a, bottom_b;

	left_a = a.x;
	right_a = a.x + a.w;
	top_a = a.y;
	bottom_a = a.y + a.h;

	if (a.x != 800){
		for (int i = 0; i<6; i++){
			left_b = b[i].x;
			right_b = b[i].x + b[i].w;
			top_b = b[i].y;
			bottom_b = b[i].y + b[i].h;
			if ((left_a > left_b) & (bottom_a > top_b) & (top_a < bottom_b) & (left_a <= right_b)) return 1;
			
		}
	}
	return 0; // NO collision on left
}


//if return value is !=0 collision occured. if return is 0, no collision.
int check_collision_right_enem_enem(tanque_enem_t a, tanque_enem_t b[6]) {
	int left_a, left_b;
	int right_a, right_b;
	int top_a, top_b;
	int bottom_a, bottom_b;

	left_a = a.x;
	right_a = a.x + a.w;
	top_a = a.y;
	bottom_a = a.y + a.h;

	if (a.x != 800){
		for (int i = 0; i<6; i++){
				
			left_b = b[i].x;
			right_b = b[i].x + b[i].w;
			top_b = b[i].y;
			bottom_b = b[i].y + b[i].h;
			
			if ((left_a < left_b) & (bottom_a > top_b) & (top_a < bottom_b) & (right_a >= left_b)) return 1;	
			
		}
	}

	return 0;
}

//if return value is !=0 collision occured. if return is 0, no collision.
int check_collision_up_enem_enem(tanque_enem_t a, tanque_enem_t b[6]) {
	int left_a, left_b;
	int right_a, right_b;
	int top_a, top_b;
	int bottom_a, bottom_b;

	left_a = a.x;
	right_a = a.x + a.w;
	top_a = a.y;
	bottom_a = a.y + a.h;

	if (a.x != 800){
		for (int i = 0; i<6; i++){
				
			left_b = b[i].x;
			right_b = b[i].x + b[i].w;
			top_b = b[i].y;
			bottom_b = b[i].y + b[i].h;
			
			if ((bottom_a > bottom_b) & (right_a > left_b) & (left_a < right_b) & (top_a <= bottom_b)) return 1;
			
		}
	}

	return 0;
}

//if return value is !=0 collision occured. if return is 0, no collision.
int check_collision_down_enem_enem(tanque_enem_t a, tanque_enem_t b[6]) {
	int left_a, left_b;
	int right_a, right_b;
	int top_a, top_b;
	int bottom_a, bottom_b;

	left_a = a.x;
	right_a = a.x + a.w;
	top_a = a.y;
	bottom_a = a.y + a.h;

	if (a.x != 800){
		for (int i = 0; i<6; i++){
				
			left_b = b[i].x;
			right_b = b[i].x + b[i].w;
			top_b = b[i].y;
			bottom_b = b[i].y + b[i].h;
			
			if ((bottom_a < (bottom_b)) & (right_a > left_b) & (left_a < right_b) & (bottom_a >= top_b)) return 1;
			
		}
	}

	return 0;
}



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////// -------------------------------------------------Collision between player and blocks ------------------------------------////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//if return value is !=0 collision occured. if return is 0, no collision.
int check_collision_left(tanque_t a, block_t b[338]) {
	int left_a, left_b;
	int right_a, right_b;
	int top_a, top_b;
	int bottom_a, bottom_b;

	left_a = a.x;
	right_a = a.x + a.w;
	top_a = a.y;
	bottom_a = a.y + a.h;

	for (int i = 0; i<338; i++){
		if (b[i].tipo != 3){
			left_b = b[i].x;
			right_b = b[i].x + b[i].w;
			top_b = b[i].y;
			bottom_b = b[i].y + b[i].h;
			
			if ((left_a > left_b) & (bottom_a > top_b) & (top_a < bottom_b) & (left_a <= right_b)) return 1;		
			
		}
	}

	return 0;
}

//if return value is !=0 collision occured. if return is 0, no collision.
int check_collision_right(tanque_t a, block_t b[338]) {
	int left_a, left_b;
	int right_a, right_b;
	int top_a, top_b;
	int bottom_a, bottom_b;

	left_a = a.x;
	right_a = a.x + a.w;
	top_a = a.y;
	bottom_a = a.y + a.h;

	for (int i = 0; i<338; i++){
		if (b[i].tipo != 3){
			left_b = b[i].x;
			right_b = b[i].x + b[i].w;
			top_b = b[i].y;
			bottom_b = b[i].y + b[i].h;
			
			if ((left_a < left_b) & (bottom_a > top_b) & (top_a < bottom_b) & (right_a >= left_b)) return 1;
		
		}
	}

	return 0;
}

//if return value is !=0 collision occured. if return is 0, no collision.
int check_collision_up(tanque_t a, block_t b[338]) {
	int left_a, left_b;
	int right_a, right_b;
	int top_a, top_b;
	int bottom_a, bottom_b;

	left_a = a.x;
	right_a = a.x + a.w;
	top_a = a.y;
	bottom_a = a.y + a.h;

	for (int i = 0; i<338; i++){
		if (b[i].tipo != 3){
			left_b = b[i].x;
			right_b = b[i].x + b[i].w;
			top_b = b[i].y;
			bottom_b = b[i].y + b[i].h;
			
			if ((bottom_a > bottom_b) & (right_a > left_b) & (left_a < right_b) & (top_a <= bottom_b)) return 1;
					
		}
	}

	return 0;
}

//if return value is !=0 collision occured. if return is 0, no collision.
int check_collision_down(tanque_t a, block_t b[338]) {
	int left_a, left_b;
	int right_a, right_b;
	int top_a, top_b;
	int bottom_a, bottom_b;

	left_a = a.x;
	right_a = a.x + a.w;
	top_a = a.y;
	bottom_a = a.y + a.h;

	for (int i = 0; i<338; i++){
		if (b[i].tipo != 3){
			left_b = b[i].x;
			right_b = b[i].x + b[i].w;
			top_b = b[i].y;
			bottom_b = b[i].y + b[i].h;
			
			if ((bottom_a < bottom_b) & (right_a > left_b) & (left_a < right_b) & (bottom_a >= top_b)) return 1;
					
		}
	}

	return 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static void draw_block(block_t a[338]){ // it will depend on the type
	SDL_Rect src;
	SDL_Rect dest;

	src.x = 0;
	src.y = 0;

	for (int i=0; i<338; i++){
		dest.x = a[i].x;
		dest.y = a[i].y;
		dest.w = a[i].w;
		dest.h = a[i].h;
		src.w = a[i].w;
		src.h = a[i].h;

		if (a[i].tipo == 1) SDL_BlitSurface(long_block, &src, screen, &dest);
		
		if (a[i].tipo == 2) SDL_BlitSurface(long_steel, &src, screen, &dest);
		
		if (a[i].tipo == 3) SDL_BlitSurface(trees2, &src, screen, &dest);

	}
}

static void set_mapa_1(){ 

	int matriz[ 26 ][ 13 ] = { { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, // Meaning:
							   { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, // 0 = Free space on the map
							   { 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0 }, // 1 = brick block
							   { 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0 }, // 2 = iron block
							   { 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0 }, // 3 = Grass
							   { 0, 1, 0, 1, 0, 1, 2, 1, 0, 1, 0, 1, 0 },
							   { 0, 1, 0, 1, 0, 1, 2, 1, 0, 1, 0, 1, 0 },
							   { 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0 },
							   { 0, 1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 1, 0 },
							   { 0, 1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 1, 0 },
							   { 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0 },
							   { 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0 },
							   { 1, 0, 1, 1, 0, 0, 0, 0, 0, 1, 1, 0, 1 },
							   { 2, 0, 1, 1, 0, 0, 0, 0, 0, 1, 1, 0, 2 },
							   { 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0 },
							   { 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0 },
							   { 0, 1, 0, 1, 0, 1, 1, 1, 0, 1, 0, 1, 0 },
							   { 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0 },
							   { 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0 },
							   { 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0 },
							   { 0, 1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 1, 0 },
							   { 0, 1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 1, 0 },
							   { 0, 1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 1, 0 },
							   { 0, 1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 1, 0 },
							   { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
							   { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 } };

	int num = 0;
	int var;
	int x = 0; // x pos on the screen
	int y = 0; // y pos on the screen
	for (int i = 0; i<26; i++){ // Traslation of the matrix the array of blocks
		y += 16;
		x = 0;
		for (int j =0; j<13; j++){
			var = matriz[i][j];
			x += 32; // Every block has dimension of 32 (x) x 16 (y)
			if (var == 2){
				block[num].x = x;
				block[num].y = y;
				block[num].tipo = 2;
			}
			if (var == 1){
				block[num].x = x;
				block[num].y = y;
				block[num].tipo = 1;
			  }
			if (var == 3){
				block[num].x = x;
				block[num].y = y;
				block[num].tipo = 3;
			}
			num++;
		}
 	}

 	// The base and other parts are different than 32 x 16 so, we have to load them apart (small blocks)
	// now we draw the base icon
	block[0].x = 224;
	block[0].y = 400;
	block[0].tipo = 4;
	block[0].h = 32;
	block[0].w = 32;

	// Now the small blocks on the base
	block[1].x = 224 - 16;
	block[1].y = 400 + 16;
	block[1].tipo = 1;
	block[1].h = 16;
	block[1].w = 16;

	block[2].x = 224 - 16;
	block[2].y = 400;
	block[2].tipo = 1;
	block[2].h = 16;
	block[2].w = 16;

	block[3].x = 224 - 16;
	block[3].y = 400 -16;
	block[3].tipo = 1;
	block[3].h = 16;
	block[3].w = 32;

	block[4].x = 224 + 16;
	block[4].y = 400 - 16;
	block[4].tipo = 1;
	block[4].h = 16;
	block[4].w = 32;

	block[5].x = 224 + 32;
	block[5].y = 400;
	block[5].tipo = 1;
	block[5].h = 16;
	block[5].w = 16;

	block[6].x = 224 + 32;
	block[6].y = 400 + 16;
	block[6].tipo = 1;
	block[6].h = 16;
	block[6].w = 16;
	
}

static void set_mapa_2(){

	int matriz[ 26 ][ 13 ] = { { 0, 0, 0, 2, 0, 0, 0, 2, 0, 0, 0, 0, 0 },//
							   { 0, 0, 0, 2, 0, 0, 0, 2, 0, 0, 0, 0, 0 }, 
							   { 0, 1, 0, 2, 0, 0, 0, 1, 0, 1, 0, 1, 0 },//
							   { 0, 1, 0, 2, 0, 0, 0, 1, 0, 1, 0, 1, 0 },
							   { 0, 1, 0, 0, 0, 0, 1, 1, 0, 1, 2, 1, 0 },//
							   { 0, 1, 0, 0, 0, 0, 1, 1, 0, 1, 2, 1, 0 },
							   { 0, 0, 0, 1, 0, 0, 0, 0, 0, 2, 0, 0, 0 },//
							   { 0, 0, 0, 1, 0, 0, 0, 0, 0, 2, 0, 0, 0 },
							   { 3, 0, 0, 1, 0, 0, 2, 0, 0, 1, 3, 1, 2 },//
							   { 3, 0, 0, 1, 0, 0, 2, 0, 0, 1, 3, 1, 2 },
							   { 3, 3, 0, 0, 0, 1, 0, 0, 2, 0, 3, 0, 0 },//
							   { 3, 3, 0, 0, 0, 1, 0, 0, 2, 0, 3, 0, 0 },
							   { 0, 1, 1, 1, 3, 3, 3, 2, 0, 0, 3, 1, 0 },//
							   { 0, 1, 1, 1, 3, 3, 3, 2, 0, 0, 3, 1, 0 },
							   { 0, 0, 0, 2, 3, 1, 0, 1, 0, 1, 0, 1, 0 },//
							   { 0, 0, 0, 2, 3, 1, 0, 1, 0, 1, 0, 1, 0 },
							   { 2, 1, 0, 2, 0, 1, 0, 1, 0, 0, 0, 1, 0 },//
							   { 2, 1, 0, 2, 0, 1, 0, 1, 0, 0, 0, 1, 0 },
							   { 0, 1, 0, 1, 0, 1, 1, 1, 0, 1, 2, 1, 0 },//
							   { 0, 1, 0, 1, 0, 1, 1, 1, 0, 1, 2, 1, 0 },
							   { 0, 1, 0, 1, 0, 1, 1, 1, 0, 0, 0, 0, 0 },//
							   { 0, 1, 0, 1, 0, 1, 1, 1, 0, 0, 0, 0, 0 },
							   { 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0 },//
							   { 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0 },
							   { 0, 1, 0, 1, 0, 0, 0, 0, 0, 1, 1, 1, 0 },//
							   { 0, 1, 0, 1, 0, 0, 0, 0, 0, 1, 1, 1, 0 }
							    };

	int num = 0;
	int var;
	int x = 0;
	int y = 0;
	for (int i = 0; i<26; i++){
		y += 16;
		x = 0;
		for (int j =0; j<13; j++){
			var = matriz[i][j];
			x += 32;
			if (var == 2){
				block[num].x = x;
				block[num].y = y;
				block[num].tipo = 2;
			}
			if (var == 3){
				block[num].x = x;
				block[num].y = y;
				block[num].tipo = 3;
			}
			if (var == 1){
				block[num].x = x;
				block[num].y = y;
				block[num].tipo = 1;
			}
			num++;
		}
	}

	// Now we draw the base
	block[0].x = 224;
	block[0].y = 400;
	block[0].tipo = 4;
	block[0].h = 32;
	block[0].w = 32;

	// Now we draw the small blocks
	block[1].x = 224 - 16;
	block[1].y = 400 + 16;
	block[1].tipo = 1;
	block[1].h = 16;
	block[1].w = 16;

	block[2].x = 224 - 16;
	block[2].y = 400;
	block[2].tipo = 1;
	block[2].h = 16;
	block[2].w = 16;

	block[4].x = 224 - 16;
	block[4].y = 400 -16;
	block[4].tipo = 1;
	block[4].h = 16;
	block[4].w = 32;

	block[5].x = 224 + 16;
	block[5].y = 400 - 16;
	block[5].tipo = 1;
	block[5].h = 16;
	block[5].w = 32;

	block[6].x = 224 + 32;
	block[6].y = 400;
	block[6].tipo = 1;
	block[6].h = 16;
	block[6].w = 16;

	block[8].x = 224 + 32;
	block[8].y = 400 + 16;
	block[8].tipo = 1;
	block[8].h = 16;
	block[8].w = 16;
	
}

static void set_mapa_3(){

	int matriz[ 26 ][ 13 ] = { { 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0 },//
							   { 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0 }, 
							   { 0, 3, 3, 3, 1, 0, 0, 0, 0, 0, 0, 0, 0 },//
							   { 0, 3, 3, 3, 1, 0, 0, 0, 0, 0, 2, 2, 2 },
							   { 1, 3, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0 },//
							   { 1, 3, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
							   { 3, 3, 3, 3, 0, 0, 0, 1, 0, 1, 1, 1, 0 },//
							   { 3, 3, 3, 3, 0, 0, 0, 1, 0, 1, 1, 1, 0 },
							   { 3, 3, 3, 3, 1, 1, 1, 1, 0, 1, 0, 0, 0 },//
							   { 3, 3, 3, 3, 1, 1, 1, 0, 0, 1, 0, 0, 0 },
							   { 3, 3, 3, 3, 0, 0, 1, 0, 0, 0, 0, 0, 0 },//
							   { 3, 3, 3, 3, 0, 0, 1, 0, 0, 0, 0, 0, 0 },
							   { 0, 3, 0, 0, 0, 0, 2, 2, 2, 0, 0, 3, 0 },//
							   { 0, 3, 0, 0, 0, 0, 2, 2, 2, 0, 0, 3, 0 },
							   { 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 3, 3, 3 },//
							   { 0, 1, 0, 1, 0, 0, 0, 0, 0, 3, 3, 3, 3 },
							   { 1, 0, 0, 1, 0, 0, 1, 1, 1, 3, 3, 3, 3 },//
							   { 1, 0, 0, 1, 0, 0, 0, 0, 0, 3, 3, 3, 3 },
							   { 0, 0, 0, 0, 0, 1, 0, 0, 0, 3, 3, 3, 3 },//
							   { 0, 0, 0, 0, 0, 1, 0, 1, 1, 3, 3, 3, 3 },
							   { 1, 0, 0, 0, 0, 0, 0, 1, 1, 3, 3, 3, 0 },//
							   { 1, 0, 0, 0, 0, 0, 0, 0, 0, 3, 3, 3, 0 },
							   { 1, 1, 0, 0, 0, 0, 0, 0, 0, 3, 3, 3, 0 },//
							   { 1, 1, 0, 0, 0, 0, 0, 0, 0, 3, 3, 3, 0 },
							   { 2, 1, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0 },//
							   { 2, 1, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0 }
							    };

	int num = 0;
	int var;
	int x = 0;
	int y = 0;
	for (int i = 0; i<26; i++){
		y += 16;
		x = 0;
		for (int j =0; j<13; j++){
			var = matriz[i][j];
			x += 32;
			if (var == 2){
				block[num].x = x;
				block[num].y = y;
				block[num].tipo = 2;
			}
			if (var == 3){
				block[num].x = x;
				block[num].y = y;
				block[num].tipo = 3;
			}
			if (var == 1){
				block[num].x = x;
				block[num].y = y;
				block[num].tipo = 1;
			}
			num++;
		}
	}

	// Now we draw the base
	block[0].x = 224;
	block[0].y = 400;
	block[0].tipo = 4;
	block[0].h = 32;
	block[0].w = 32;

	// Now we draw the small blocks of the map
	block[1].x = 224 - 16;
	block[1].y = 400 + 16;
	block[1].tipo = 1;
	block[1].h = 16;
	block[1].w = 16;

	block[2].x = 224 - 16;
	block[2].y = 400;
	block[2].tipo = 1;
	block[2].h = 16;
	block[2].w = 16;

	block[3].x = 224 - 16;
	block[3].y = 400 -16;
	block[3].tipo = 1;
	block[3].h = 16;
	block[3].w = 32;

	block[5].x = 224 + 16;
	block[5].y = 400 - 16;
	block[5].tipo = 1;
	block[5].h = 16;
	block[5].w = 32;

	block[6].x = 224 + 32;
	block[6].y = 400;
	block[6].tipo = 1;
	block[6].h = 16;
	block[6].w = 16;

	block[7].x = 224 + 32;
	block[7].y = 400 + 16;
	block[7].tipo = 1;
	block[7].h = 16;
	block[7].w = 16;


	block[9].x = 128;
	block[9].y = 336;
	block[9].tipo = 2;
	block[9].h = 16;
	block[9].w = 16;

	block[10].x = 128;
	block[10].y = 352;
	block[10].tipo = 2;
	block[10].h = 16;
	block[10].w = 16;

	block[11].x = 128;
	block[11].y = 368;
	block[11].tipo = 2;
	block[11].h = 16;
	block[11].w = 16;

	block[12].x = 128;
	block[12].y = 384;
	block[12].tipo = 2;
	block[12].h = 16;
	block[12].w = 16;

	block[18].x = 416;
	block[18].y = 112;
	block[18].tipo = 1;
	block[18].h = 16;
	block[18].w = 16;

	block[19].x = 416;
	block[19].y = 128;
	block[19].tipo = 1;
	block[19].h = 16;
	block[19].w = 16;

	block[20].x = 400;
	block[20].y = 144;
	block[20].tipo = 1;
	block[20].h = 16;
	block[20].w = 16;

	block[14].x = 400;
	block[14].y = 160;
	block[14].tipo = 1;
	block[14].h = 16;
	block[14].w = 16;

	block[22].x = 400;
	block[22].y = 176;
	block[22].tipo = 1;
	block[22].h = 16;
	block[22].w = 16;

	block[23].x = 400;
	block[23].y = 192;
	block[23].tipo = 1;
	block[23].h = 16;
	block[23].w = 16;

	block[24].x = 224 - 16;
	block[24].y = 272;
	block[24].tipo = 1;
	block[24].h = 16;
	block[24].w = 16;

	block[25].x = 224 - 16;
	block[25].y = 272 + 16;
	block[25].tipo = 1;
	block[25].h = 16;
	block[25].w = 16;

	block[13].x = 224 - 64;
	block[13].y = 272;
	block[13].tipo = 1;
	block[13].h = 16;
	block[13].w = 16;

	block[26].x = 224 - 64;
	block[26].y = 272 + 16;
	block[26].tipo = 1;
	block[26].h = 16;
	block[26].w = 16;

	block[31].x = 224 - 112;
	block[31].y = 272;
	block[31].tipo = 1;
	block[31].h = 16;
	block[31].w = 16;

	block[32].x = 224 - 112;
	block[32].y = 272 + 16;
	block[32].tipo = 1;
	block[32].h = 16;
	block[32].w = 16;

	block[33].x = 224 - 160;
	block[33].y = 272;
	block[33].tipo = 1;
	block[33].h = 16;
	block[33].w = 16;

	block[34].x = 224 - 160;
	block[34].y = 272 + 16;
	block[34].tipo = 1;
	block[34].h = 16;
	block[34].w = 16;
}

static void draw_base(){
	SDL_Rect src;
	SDL_Rect dest;

	src.x = 0;
	src.y = 0;
	dest.x = block[0].x;
	dest.y = block[0].y;
	dest.w = block[0].w;
	dest.h = block[0].h;
	src.w = block[0].w;
	src.h = block[0].h;

	SDL_BlitSurface(base, &src, screen, &dest);
}

static void initialize_values() {
	// Border walls ----------------------------
	wall[0].x = 0;
	wall[0].y = 0;
	wall[0].w = 32;
	wall[0].h = screen->h;

	wall[1].x = 32;
	wall[1].y = 0;
	wall[1].w = (screen->w) - 32;
	wall[1].h = 16;

	wall[2].x = 448;
	wall[2].y = 16;
	wall[2].w = 64;
	wall[2].h = (screen->h) - 16;

	wall[3].x = 32;
	wall[3].y = 432;
	wall[3].w = (screen->w) - 96;
	wall[3].h = 16;

  // initial position of the player----------------
	tanque.x = 164;
	tanque.y = 404;
	tanque.h = 24;
	tanque.w = 24;
	tanque.escudo = 1; // starts with shield
	tanque.distan = 0; // No distance traveled
	tanque.vidas = 3; // 3 lives up

  // initialize all blocks out of the screen
	for (int x = 0; x < 338; x++){
		block[x].x = 800; // out of the screen
		block[x].y = 0;
		block[x].tipo = 0; // means there is no block
		block[x].w = 32;
		block[x].h = 16;
	}

	// Initialize bullets out of the screen
	for (int i = 0; i<20; i++){
		bala[i].x = 900; // out of the screen position
		bala[i].y = 0;
		bala[i].h = 5;
		bala[i].w = 5;
		bala[i].direc = 4;
	}

	// Initialize bullets for enemies out of the screen
	for (int h = 0; h<20; h++){
		bala_enem[h].x = 900; // out fo the screen pos
		bala_enem[h].y = 0;
		bala_enem[h].h = 5;
		bala_enem[h].w = 5;
		bala_enem[h].direc = 4;
	}

	// Initial values for the enemies
	for (int j = 0; j<6; j++){
		tanque_enem[j].x = 800; // out of the screen position
		tanque_enem[j].y =  0;
		tanque_enem[j].h = 24;
		tanque_enem[j].w = 24;
		tanque_enem[j].direc = 4;
		tanque_enem[j].tipo = 0;
		tanque_enem[j].distan = 0;
		tanque_enem[j].vidas = 4;
	}

	// Initialize of the power ups icons out of the screen
	power_up.x = 800; // out of the screen position
	power_up.y = 0;
	power_up.w = 32;
	power_up.h = 32;
}

static void draw_power_up(power_up_t a){ // draw the power up on the screen
	SDL_Rect src;
	SDL_Rect dest;

	src.x = 0;
	src.y = 0;
	dest.x = a.x;
	dest.y = a.y;
	dest.w = a.w;
	dest.h = a.h;
	src.w = a.w;
	src.h = a.h;
	
	if (a.tipo == 0) SDL_BlitSurface(powerup_casco, &src, screen, &dest); // shield power up
	
	else if (a.tipo == 1) SDL_BlitSurface(powerup_tank, &src, screen, &dest); // tank life power up
	
	else SDL_BlitSurface(powerup_granada, &src, screen, &dest); // kill all power up (granada)
}

int num_enemigos_vivos(tanque_enem_t a[6]){ // count the number of enemies alive
	int suma = 0;
	for (int i = 0; i<6; i++){
		if (a[i].vidas != 0) suma += 1;
	}
	return suma;
}

static void set_tanque_enemigo(tanque_enem_t a[6]){ // create enemies
	int valor_x = 36;

	if ((a[0].x == 800) & (a[1].x == 800) & (a[2].x == 800)){ // if they are out of the screen, they haven't been created, so, create them
		for (int i = 0; i<3; i++){
			tanque_enem[i].direc = rand() % 4; // a direction to start moving
			tanque_enem[i].tipo = rand() % 4; //it has to be a random from 0 to 3 to know wich enemy
			tanque_enem[i].x = valor_x;
			tanque_enem[i].y = 18;
			valor_x += 192;
			if (tanque_enem[i].tipo != 3) tanque_enem[i].vidas = 1;
			
			else tanque_enem[i].vidas = 3;
		}
	}
	

	if ((a[3].x == 800) & (a[4].x == 800) & (a[5].x == 800) & (a[0].x == 801) & (a[1].x == 801) & (a[2].x == 801)){ // if enemies second wave of enemies haven't been created and already kill the first three
		for (int i = 3; i<6; i++){
			tanque_enem[i].direc = rand() % 4;
			tanque_enem[i].tipo = rand() % 4; // we make a random from 0 to 3 to know the type of the enemies
			tanque_enem[i].x = valor_x;
			tanque_enem[i].y = 18;
			valor_x += 192;
			if (tanque_enem[i].tipo != 3) tanque_enem[i].vidas = 1;
			
			else tanque_enem[i].vidas = 3;
		}
	}
}

 void set_bala_enem(int d, int a) {

	for (int num = 0; num<20; num++){

		if (bala_enem[num].x == 800){
			// if enemy moving down
			if (d == 0) {
				bala_enem[num].y = tanque_enem[a].y + tanque_enem[a].h;
				bala_enem[num].x = tanque_enem[a].x + (tanque_enem[a].w/2);
				bala_enem[num].direc = 0;
				break;
			}
			
			// if enemy moving up
			if (d == 1) {
				bala_enem[num].y = tanque_enem[a].y;
				bala_enem[num].x = tanque_enem[a].x + (tanque_enem[a].w/2);
				bala_enem[num].direc = 1;
				break;
			}
			// If enemy moving right
			if (d == 2) {
				bala_enem[num].y = tanque_enem[a].y + (tanque_enem[a].h/2);
				bala_enem[num].x = tanque_enem[a].x + tanque_enem[a].w;
				bala_enem[num].direc = 2;
				break;
			}
			// If enemy moving left
			if (d == 3) {
				bala_enem[num].y = tanque_enem[a].y + (tanque_enem[a].h/2);
				bala_enem[num].x = tanque_enem[a].x;
				bala_enem[num].direc = 3;
				break;
			}
		}
	}
}


static void move_tanque_enem(tanque_enem_t a[6]){ // to move the enemy tank
	int numrandom;
	for (int j =0; j<6; j++){
		numrandom = (rand()%100 + 200); // This is to create a number to shoot a bullet, you can use smaller values to get more frecuent shoots for enemies
		if ((a[j].x != 800) & (a[j].x != 801)){ // if they are on the screen
			if (a[j].direc == 0) { // if its going down
				
				if(a[j].y >= 408) { // Border of the screen
					a[j].direc = rand() % 4;
					a[j].y = 408;
				
				} else { 

					if ((check_collision_down_enem(a[j], block) == 0) & (check_collision_down_enem_enem(a[j], a) == 0)) { // outputs 1 when no collision
						a[j].y += 1;
						a[j].distan += 1;
						if ((a[j].tipo == 2) || (a[j].tipo == 1)){
							a[j].y += 1; 
							a[j].distan += 1;
						}

					}
					else // Collision ocurred
						a[j].direc = rand() % 4; // Change direction to a random value
				}
			}
			
			// if its going up
			if (a[j].direc == 1) {

				if(a[j].y <= 16) {
					a[j].direc = rand() % 4;
					a[j].y = 16;

				} else {
				
					if ((check_collision_up_enem(a[j], block) == 0) & (check_collision_up_enem_enem(a[j], a) == 0)){
						a[j].y -= 1;
						a[j].distan += 1;
						if ((a[j].tipo == 2) || (a[j].tipo == 1)){
							a[j].y -= 1; // estabab en 5
							a[j].distan += 1;
						}
					}
					else
						a[j].direc = rand() % 4;
				}
			}
			// If its going right
			if (a[j].direc == 2) {

				if(a[j].x >= 424) {
					a[j].direc = rand() % 4;
					a[j].x = 424;

				} else {
				
					if ((check_collision_right_enem(a[j], block) == 0) & (check_collision_right_enem_enem(a[j], a) == 0)){
						a[j].x += 1;
						a[j].distan += 1;
						if ((a[j].tipo == 2) || (a[j].tipo == 1)){
							a[j].x += 1; // estabab en 5
							a[j].distan += 1;
						}
					}
					else
						a[j].direc = rand() % 4;
				}
			}
			// If its going left
			if (a[j].direc == 3) {

				if(a[j].x <= 32) {
					a[j].direc = rand() % 4;
					a[j].x = 32;

				} 
				else {
					if ((check_collision_left_enem(a[j], block) == 0) & (check_collision_left_enem_enem(a[j], a) == 0)){
						a[j].x -= 1; // estabab en 5
						a[j].distan += 1;
						if ((a[j].tipo == 2) || (a[j].tipo == 1)){
							a[j].x -= 1; // estabab en 5
							a[j].distan += 1;
						}
					}
					else
						a[j].direc = rand() % 4;
				}
			}
			if (a[j].distan >= numrandom){
				set_bala_enem(a[j].direc, j);
				a[j].distan = 0;
			}
		}
	}
}

static void draw_tanque_enem(tanque_enem_t a[6]){
	
	SDL_Rect src;
	SDL_Rect dest;

	src.x = 0;
	src.y = 0;
	src.w = 24;
	src.h = 24;
	dest.w = 24;
	dest.h = 24;

	for (int i = 0; i<6; i++){

		dest.x = a[i].x;
		dest.y = a[i].y;
		if (a[i].tipo == 0){
			if (a[i].direc == 0){
				SDL_BlitSurface(b_enem_abajo, &src, screen, &dest);
			}
			if (a[i].direc == 1){
				SDL_BlitSurface(b_enem_arriba, &src, screen, &dest);
			}
			if (a[i].direc == 2){
				SDL_BlitSurface(b_enem_der, &src, screen, &dest);
			}
			if (a[i].direc == 3){
				SDL_BlitSurface(b_enem_izq, &src, screen, &dest);
			}
		}
		if (a[i].tipo == 1){
			if (a[i].direc == 0){
				SDL_BlitSurface(b1_enem_abajo, &src, screen, &dest);
			}
			if (a[i].direc == 1){
				SDL_BlitSurface(b1_enem_arriba, &src, screen, &dest);
			}
			if (a[i].direc == 2){
				SDL_BlitSurface(b1_enem_der, &src, screen, &dest);
			}
			if (a[i].direc == 3){
				SDL_BlitSurface(b1_enem_izq, &src, screen, &dest);
			}
		}
		if (a[i].tipo == 2){
			if (a[i].distan >= 250){ // change color with distance
				if (a[i].direc == 0){
					SDL_BlitSurface(b2_enem_abajo, &src, screen, &dest);
				}
				if (a[i].direc == 1){
					SDL_BlitSurface(b2_enem_arriba, &src, screen, &dest);
				}
				if (a[i].direc == 2){
					SDL_BlitSurface(b2_enem_der, &src, screen, &dest);
				}
				if (a[i].direc == 3){
					SDL_BlitSurface(b2_enem_izq, &src, screen, &dest);
				}
			}
			else{
				if (a[i].direc == 0){
					SDL_BlitSurface(b1_enem_abajo, &src, screen, &dest);
				}
				if (a[i].direc == 1){
					SDL_BlitSurface(b1_enem_arriba, &src, screen, &dest);
				}
				if (a[i].direc == 2){
					SDL_BlitSurface(b1_enem_der, &src, screen, &dest);
				}
				if (a[i].direc == 3){
					SDL_BlitSurface(b1_enem_izq, &src, screen, &dest);
				}
			}
		}
		
		if (a[i].tipo == 3){ // depending on the quantity of lives, it changes colors
			if (a[i].vidas == 3){
				if (a[i].direc == 0){
					SDL_BlitSurface(b3_enem_abajo_v, &src, screen, &dest);
				}
				if (a[i].direc == 1){
					SDL_BlitSurface(b3_enem_arriba_v, &src, screen, &dest);
				}
				if (a[i].direc == 2){
					SDL_BlitSurface(b3_enem_der_v, &src, screen, &dest);
				}
				if (a[i].direc == 3){
					SDL_BlitSurface(b3_enem_izq_v, &src, screen, &dest);
				}
			}
			if (a[i].vidas == 2){
				if (a[i].direc == 0){
					SDL_BlitSurface(b3_enem_abajo_a, &src, screen, &dest);
				}
				if (a[i].direc == 1){
					SDL_BlitSurface(b3_enem_arriba_a, &src, screen, &dest);
				}
				if (a[i].direc == 2){
					SDL_BlitSurface(b3_enem_der_a, &src, screen, &dest);
				}
				if (a[i].direc == 3){
					SDL_BlitSurface(b3_enem_izq_a, &src, screen, &dest);
				}
			}
			if (a[i].vidas == 1){
				if (a[i].direc == 0){
					SDL_BlitSurface(b3_enem_abajo_b, &src, screen, &dest);
				}
				if (a[i].direc == 1){
					SDL_BlitSurface(b3_enem_arriba_b, &src, screen, &dest);
				}
				if (a[i].direc == 2){
					SDL_BlitSurface(b3_enem_der_b, &src, screen, &dest);
				}
				if (a[i].direc == 3){
					SDL_BlitSurface(b3_enem_izq_b, &src, screen, &dest);
				}
			}
		}
		

	}
}

static void draw_tanque(int x){ // To draw the player

	SDL_Rect src;
	SDL_Rect dest;

	src.x = 1;
	src.y = 1;
	dest.x = tanque.x;
	dest.y = tanque.y;

	dest.w = tanque.w;
	dest.h = tanque.h;
	src.w = tanque.w;
	src.h = tanque.h;

	// Depends on the orientation its poniting
	if (x == 1) SDL_BlitSurface(playerTank_up, &src, screen, &dest);
	
	else if (x == 0) SDL_BlitSurface(playerTank_down, &src, screen, &dest);
	
	else if (x == 2) SDL_BlitSurface(playerTank_right, &src, screen, &dest);
	
	else if (x == 3) SDL_BlitSurface(playerTank_left, &src, screen, &dest);
	
}

static void draw_inicio(){ // Initial screen
	SDL_Rect src;
	SDL_Rect dest;

	src.x = 0;
	src.y = 0;
	src.w = battle_city->w;
	src.h = battle_city->h;

	dest.x = 68;
	dest.y = 32;
	dest.w = battle_city->w;
	dest.h = battle_city->h;
	
	SDL_BlitSurface(battle_city, &src, screen, &dest);

	src.x = 0;
	src.y = 0;
	src.w = inicio_3->w;
	src.h = inicio_3->h;

	dest.x = 47;
	dest.y = 200;
	dest.w = inicio_3->w;
	dest.h = inicio_3->h;

	SDL_BlitSurface(inicio_3, &src, screen, &dest);

}

static void draw_game_over(){
	SDL_Rect src;
	SDL_Rect dest;

	src.x = 0;
	src.y = 0;
	src.w = game_over->w;
	src.h = game_over->h;

	dest.x = 132;
	dest.y = 64;
	dest.w = game_over->w;
	dest.h = game_over->h;
	
	SDL_BlitSurface(game_over, &src, screen, &dest);
}

static void draw_ganaste(){ // draw you win
	SDL_Rect src;
	SDL_Rect dest;

	src.x = 0;
	src.y = 0;
	src.w = ganaste->w;
	src.h = ganaste->h;

	dest.x = 60;
	dest.y = 0;
	dest.w = ganaste->w;
	dest.h = ganaste->h;
	
	SDL_BlitSurface(ganaste, &src, screen, &dest);
}

static void draw_walls() {

	SDL_Rect src;
	int i;

	for (i = 0; i < 4; i++) {
	
		src.x = wall[i].x;
		src.y = wall[i].y;
		src.w = wall[i].w;
		src.h = wall[i].h;
		
		int r = SDL_FillRect(screen,&src,SDL_MapRGB(screen->format, 99, 101, 98));
		
		if (r !=0) printf("fill rectangle faliled in func draw_walls()");
		
	}
}

static void draw_enemy_icon(){

	SDL_Rect src;
	SDL_Rect dest;

	src.x = 0;
	src.y = 0;
	src.w = iconoEnemigo->w;
	src.h = iconoEnemigo->h;

	dest.x = 472;
	dest.y = 32;
	dest.w = iconoEnemigo->w;
	dest.h = iconoEnemigo->h;
	
	SDL_BlitSurface(iconoEnemigo, &src, screen, &dest);
}

static void draw_lives_icon(){
	SDL_Rect src;
	SDL_Rect dest;


	src.x = 0;
	src.y = 0;
	src.w = iconoVidas->w;
	src.h = iconoVidas->h;

	dest.x = 472;
	dest.y = 192;
	dest.w = iconoVidas->w;
	dest.h = iconoVidas->h;
	
	SDL_BlitSurface(iconoVidas, &src, screen, &dest);
}

static void draw_flag_icon(){
	SDL_Rect src;
	SDL_Rect dest;

	src.x = 0;
	src.y = 0;
	src.w = iconoBandera->w;
	src.h = iconoBandera->h;

	dest.x = 464;
	dest.y = 340;
	dest.w = iconoBandera->w;
	dest.h = iconoBandera->h;
	
	SDL_BlitSurface(iconoBandera, &src, screen, &dest);
}

static void draw_enemy_number(int a) {
	
	SDL_Rect src;
	SDL_Rect dest;

	src.x = 64*a;
	src.y = 0;
	src.w = 64;
	src.h = 64;

	dest.x = 448;
	dest.y = 48;
	dest.w = 64;
	dest.h = 64;

	SDL_BlitSurface(numbermap, &src, screen, &dest);
}

static void draw_lives_number(int a) {
	
	SDL_Rect src;
	SDL_Rect dest;

	src.x = 64*a;
	src.y = 0;
	src.w = 64;
	src.h = 64;

	dest.x = 448;
	dest.y = 208;
	dest.w = 64;
	dest.h = 64;

	SDL_BlitSurface(numbermap, &src, screen, &dest);
}

static void draw_flags_number() {
	
	SDL_Rect src;
	SDL_Rect dest;

	src.x = 64;
	src.y = 0;
	src.w = 64;
	src.h = 64;

	dest.x = 448;
	dest.y = 372;
	dest.w = 64;
	dest.h = 64;

	SDL_BlitSurface(numbermap, &src, screen, &dest);
}

// -------------------------------------MOVE-------------------------------------
static void move_tanque(int d) {


	// if the down arrow is pressed move paddle down
	if (d == 0) {
		
		if(tanque.y >= 408)  tanque.y = 408; // if its on the border, doesnt move
		 
		else { 

			if (check_collision_down(tanque, block) == 0){ // returns 1 when no collision
				tanque.y += 2; // move the tank
				
				if (tanque.escudo == 1) tanque.distan += 2; // add the number of spaces, to delete the shield later...
			}
		}
	}
	
	// if the up arrow is pressed move paddle up
	if (d == 1) {

		if(tanque.y <= 16) tanque.y = 16;
		 
		else {
		
			if (check_collision_up(tanque, block) == 0){
				tanque.y -= 2;
				
				if (tanque.escudo == 1) tanque.distan += 2;
			}
		}
	}
	// If the right arrow is pressed move paddel right
	if (d == 2) {

		if(tanque.x >= 424) tanque.x = 424;
 
		else {
		
			if (check_collision_right(tanque, block) == 0){
				tanque.x += 2;
				
				if (tanque.escudo == 1) tanque.distan += 2;
			}
		}
	}
	// If the left arrow is pressed move paddel left
	if (d == 3) {

		if(tanque.x <= 32) tanque.x = 32;
 
		else {
		
			if (check_collision_left(tanque, block) == 0){
				tanque.x -= 2; 
				
				if (tanque.escudo == 1) tanque.distan += 2;
			}
		}
	}

	if (tanque.escudo == 1){
		if (tanque.distan >= 300){ // If the player already move 300 or more spaces, remove the shield
			tanque.distan = 0;
			tanque.escudo = 0;
		}
	}
	
}

static void draw_escudo(){ // Draw shield

	SDL_Rect src;
	SDL_Rect dest;
	src.x = 0;
	src.y = 0;
	src.w = shield_2_black->w;
	src.h = shield_2_black->h;

	dest.x = tanque.x - 4;
	dest.y = tanque.y - 3 ;
	dest.w = shield_2_black->w;
	dest.h = shield_2_black->h;

	if (tanque.escudo == 1) SDL_BlitSurface(shield_2_black, &src, screen, &dest);
	
}


// -------------------------------------MOVE BULLET-------------------------------------
static void move_bala() {

	int j;
	for (j=0;j<20;j++){
		// if the bullet is moving down
		if (bala[j].x != 800){
			if (bala[j].direc == 0) {
				bala[j].y += 4;
			}
			
			// if the bullet is going up
			if (bala[j].direc == 1) {
				bala[j].y -= 4;
			}
			// If the bullet is moving right
			if (bala[j].direc == 2) {
				bala[j].x += 4;
			}
			// If the bullet is moving left
			if (bala[j].direc == 3) {
				bala[j].x -= 4;
			}
		}
	}
}

// -------------------------------------MOVE ENEMY BULLET-------------------------------------
static void move_bala_enem() {

	int j;
	for (j=0;j<20;j++){
		// if the enemy bullet is going down
		if (bala_enem[j].x != 800){
			if (bala_enem[j].direc == 0) {
				bala_enem[j].y += 4;
			}
			
			// if the enemy bullet is going up
			if (bala_enem[j].direc == 1) {
				bala_enem[j].y -= 4;
			}
			// If the enemy bullet is going right
			if (bala_enem[j].direc == 2) {
				bala_enem[j].x += 4;
			}
			// If the enemy bullet is going left
			if (bala_enem[j].direc == 3) {
				bala_enem[j].x -= 4;
			}
		}
	}
}

static void set_bala(int d, int num) { // We define the initial position and the direction of the bullet depending on the player


	// if the direction of the enemy is down
	if (d == 0) {
		bala[num].y = tanque.y + tanque.h;
		bala[num].x = tanque.x + (tanque.w/2);
		bala[num].direc = 0;
	}
	
	// if the direction of the enemy is up
	if (d == 1) {
		bala[num].y = tanque.y;
		bala[num].x = tanque.x + (tanque.w/2);
		bala[num].direc = 1;
	}
	// if the direction of the enemy is right
	if (d == 2) {
		bala[num].y = tanque.y + (tanque.h/2);
		bala[num].x = tanque.x + tanque.w;
		bala[num].direc = 2;
	}
	// if the direction of the enemy is left
	if (d == 3) {
		bala[num].y = tanque.y + (tanque.h/2);
		bala[num].x = tanque.x;
		bala[num].direc = 3;
	}
}


int main() {
	srand(time(NULL)); // Seed for random in functions
	SDL_Surface *temp;
	SDL_Rect dest;
	SDL_Event event;
	int done = 0;
	atexit(SDL_Quit);
	// SDL init
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("No se pudo iniciar SDL: %s\n",SDL_GetError());
		exit(1);
	}
	// We active video mode
	screen = SDL_SetVideoMode(512,448,8,SDL_DOUBLEBUF);
	if (screen == NULL) {
		printf("No se puede inicializar el modo gráfico: %s\n",SDL_GetError());
		exit(1);
	}

	// Initialize values
	initialize_values();
	
	// Load image with numbers
	temp = SDL_LoadBMP("numbermap.bmp");
	if ( temp == NULL ) {
		printf("No pude cargar gráfico: %s\n", SDL_GetError());
		exit(1);
	}
	/* Set the numbermaps colorkey. */
	Uint32 colorkey = SDL_MapRGB(temp->format, 255, 0, 255);
	SDL_SetColorKey(temp, SDL_SRCCOLORKEY, colorkey);

	//convert the numbermaps surface to the same type as the screen
	numbermap = SDL_DisplayFormat(temp);
	
	if (numbermap == NULL) {
	
		printf("Unable to convert bitmap.\n");
		return 1;
	}

	SDL_FreeSurface(temp);

	// When background white SDL_SetColorKey(image,SDL_SRCCOLORKEY|SDL_RLEACCEL,SDL_MapRGB(image->format,255,255,255)); -------------------------------------

	// Load enemy graphic (bmp image)
	temp = SDL_LoadBMP("enemy.bmp");
	if ( temp == NULL ) {
		printf("No pude cargar gráfico: %s\n", SDL_GetError());
		exit(1);
	}
	/* Set the numbermaps colorkey. */
	Uint32 colorkey2 = SDL_MapRGB(temp->format, 255, 255, 255);
	Uint32 colorkey3 = SDL_MapRGB(temp->format, 0, 0, 0);
	SDL_SetColorKey(temp, SDL_SRCCOLORKEY, colorkey2);

	//convert the numbermaps surface to the same type as the screen
	iconoEnemigo = SDL_DisplayFormat(temp);
	
	if (iconoEnemigo == NULL) {
	
		printf("Unable to convert bitmap.\n");
		return 1;
	}

	SDL_FreeSurface(temp);

	// Load the lives bmp icon (bmp figure)
	temp = SDL_LoadBMP("lives.bmp");
	if ( temp == NULL ) {
		printf("No pude cargar gráfico: %s\n", SDL_GetError());
		exit(1);
	}
	/* Set the numbermaps colorkey. */
	SDL_SetColorKey(temp, SDL_SRCCOLORKEY, colorkey2);

	//convert the numbermaps surface to the same type as the screen
	iconoVidas = SDL_DisplayFormat(temp);
	
	if (iconoVidas == NULL) {
	
		printf("Unable to convert bitmap.\n");
		return 1;
	}

	SDL_FreeSurface(temp);

	// Load the flag icon
	temp = SDL_LoadBMP("flag.bmp");
	if ( temp == NULL ) {
		printf("No pude cargar gráfico: %s\n", SDL_GetError());
		exit(1);
	}
	/* Set the numbermaps colorkey. */
	SDL_SetColorKey(temp, SDL_SRCCOLORKEY, colorkey2);

	//convert the numbermaps surface to the same type as the screen
	iconoBandera = SDL_DisplayFormat(temp);
	
	if (iconoBandera == NULL) {
	
		printf("Unable to convert bitmap.\n");
		return 1;
	}

	SDL_FreeSurface(temp);

	// Load the tnak up image
	temp = SDL_LoadBMP("playerTank_up.bmp");
	if ( temp == NULL ) {
		printf("No pude cargar gráfico: %s\n", SDL_GetError());
		exit(1);
	}
	/* Set the numbermaps colorkey. */
	SDL_SetColorKey(temp, SDL_SRCCOLORKEY, colorkey2);

	//convert the numbermaps surface to the same type as the screen
	playerTank_up = SDL_DisplayFormat(temp);
	
	if (playerTank_up == NULL) {
	
		printf("Unable to convert bitmap.\n");
		return 1;
	}

	SDL_FreeSurface(temp);

	// Load the tank down image
	temp = SDL_LoadBMP("playerTank_down.bmp");
	if ( temp == NULL ) {
		printf("No pude cargar gráfico: %s\n", SDL_GetError());
		exit(1);
	}
	/* Set the numbermaps colorkey. */
	SDL_SetColorKey(temp, SDL_SRCCOLORKEY, colorkey2);

	//convert the numbermaps surface to the same type as the screen
	playerTank_down = SDL_DisplayFormat(temp);
	
	if (playerTank_down == NULL) {
	
		printf("Unable to convert bitmap.\n");
		return 1;
	}

	SDL_FreeSurface(temp);

	// Load the tank  right image
	temp = SDL_LoadBMP("playerTank_right.bmp");
	if ( temp == NULL ) {
		printf("No pude cargar gráfico: %s\n", SDL_GetError());
		exit(1);
	}
	/* Set the numbermaps colorkey. */
	SDL_SetColorKey(temp, SDL_SRCCOLORKEY, colorkey2);

	//convert the numbermaps surface to the same type as the screen
	playerTank_right = SDL_DisplayFormat(temp);
	
	if (playerTank_right == NULL) {
	
		printf("Unable to convert bitmap.\n");
		return 1;
	}

	SDL_FreeSurface(temp);

	// Load the tank left image
	temp = SDL_LoadBMP("playerTank_left.bmp");
	if ( temp == NULL ) {
		printf("No pude cargar gráfico: %s\n", SDL_GetError());
		exit(1);
	}
	/* Set the numbermaps colorkey. */
	SDL_SetColorKey(temp, SDL_SRCCOLORKEY, colorkey2);

	//convert the numbermaps surface to the same type as the screen
	playerTank_left = SDL_DisplayFormat(temp);
	
	if (playerTank_left == NULL) {
	
		printf("Unable to convert bitmap.\n");
		return 1;
	}

	SDL_FreeSurface(temp);

	// Load the block of bricks
	temp = SDL_LoadBMP("long_block.bmp");
	if ( temp == NULL ) {
		printf("No pude cargar gráfico: %s\n", SDL_GetError());
		exit(1);
	}
	/* Set the numbermaps colorkey. */
	SDL_SetColorKey(temp, SDL_SRCCOLORKEY, colorkey2);

	//convert the numbermaps surface to the same type as the screen
	long_block = SDL_DisplayFormat(temp);
	
	if (long_block == NULL) {
	
		printf("Unable to convert bitmap.\n");
		return 1;
	}

	SDL_FreeSurface(temp);

	// Load the enemy up
	temp = SDL_LoadBMP("b_enem_arriba.bmp");
	if ( temp == NULL ) {
		printf("No pude cargar gráfico: %s\n", SDL_GetError());
		exit(1);
	}

	//convert the numbermaps surface to the same type as the screen
	b_enem_arriba = SDL_DisplayFormat(temp);
	
	if (b_enem_arriba == NULL) {
	
		printf("Unable to convert bitmap.\n");
		return 1;
	}

	SDL_FreeSurface(temp);

	// Load the b enemy down
	temp = SDL_LoadBMP("b_enem_abajo.bmp");
	if ( temp == NULL ) {
		printf("No pude cargar gráfico: %s\n", SDL_GetError());
		exit(1);
	}

	//convert the numbermaps surface to the same type as the screen
	b_enem_abajo = SDL_DisplayFormat(temp);
	
	if (b_enem_abajo == NULL) {
	
		printf("Unable to convert bitmap.\n");
		return 1;
	}

	SDL_FreeSurface(temp);

	// Load the b enemy right
	temp = SDL_LoadBMP("b_enem_der.bmp");
	if ( temp == NULL ) {
		printf("No pude cargar gráfico: %s\n", SDL_GetError());
		exit(1);
	}

	//convert the numbermaps surface to the same type as the screen
	b_enem_der = SDL_DisplayFormat(temp);
	
	if (b_enem_der == NULL) {
	
		printf("Unable to convert bitmap.\n");
		return 1;
	}

	SDL_FreeSurface(temp);

	// Load the b enemy left
	temp = SDL_LoadBMP("b_enem_izq.bmp");
	if ( temp == NULL ) {
		printf("No pude cargar gráfico: %s\n", SDL_GetError());
		exit(1);
	}

	//convert the numbermaps surface to the same type as the screen
	b_enem_izq = SDL_DisplayFormat(temp);
	
	if (b_enem_izq == NULL) {
	
		printf("Unable to convert bitmap.\n");
		return 1;
	}

	SDL_FreeSurface(temp);

	// Laod b1 enemy up
	temp = SDL_LoadBMP("b1_enem_arriba.bmp");
	if ( temp == NULL ) {
		printf("No pude cargar gráfico: %s\n", SDL_GetError());
		exit(1);
	}

	//convert the numbermaps surface to the same type as the screen
	b1_enem_arriba = SDL_DisplayFormat(temp);
	
	if (b1_enem_arriba == NULL) {
	
		printf("Unable to convert bitmap.\n");
		return 1;
	}

	SDL_FreeSurface(temp);

	// Load the b1 enemy down
	temp = SDL_LoadBMP("b1_enem_abajo.bmp");
	if ( temp == NULL ) {
		printf("No pude cargar gráfico: %s\n", SDL_GetError());
		exit(1);
	}

	//convert the numbermaps surface to the same type as the screen
	b1_enem_abajo = SDL_DisplayFormat(temp);
	
	if (b1_enem_abajo == NULL) {
	
		printf("Unable to convert bitmap.\n");
		return 1;
	}

	SDL_FreeSurface(temp);

	// Load the b1 enemy right
	temp = SDL_LoadBMP("b1_enem_der.bmp");
	if ( temp == NULL ) {
		printf("No pude cargar gráfico: %s\n", SDL_GetError());
		exit(1);
	}

	//convert the numbermaps surface to the same type as the screen
	b1_enem_der = SDL_DisplayFormat(temp);
	
	if (b1_enem_der == NULL) {
	
		printf("Unable to convert bitmap.\n");
		return 1;
	}

	SDL_FreeSurface(temp);

	// Load the b1 enemy left
	temp = SDL_LoadBMP("b1_enem_izq.bmp");
	if ( temp == NULL ) {
		printf("No pude cargar gráfico: %s\n", SDL_GetError());
		exit(1);
	}

	//convert the numbermaps surface to the same type as the screen
	b1_enem_izq = SDL_DisplayFormat(temp);
	
	if (b1_enem_izq == NULL) {
	
		printf("Unable to convert bitmap.\n");
		return 1;
	}

	SDL_FreeSurface(temp);


	// Load the b2 enemy up
	temp = SDL_LoadBMP("b2_enem_arriba.bmp");
	if ( temp == NULL ) {
		printf("No pude cargar gráfico: %s\n", SDL_GetError());
		exit(1);
	}

	//convert the numbermaps surface to the same type as the screen
	b2_enem_arriba = SDL_DisplayFormat(temp);
	
	if (b2_enem_arriba == NULL) {
	
		printf("Unable to convert bitmap.\n");
		return 1;
	}

	SDL_FreeSurface(temp);

	// Load the b2 enemy down
	temp = SDL_LoadBMP("b2_enem_abajo.bmp");
	if ( temp == NULL ) {
		printf("No pude cargar gráfico: %s\n", SDL_GetError());
		exit(1);
	}

	//convert the numbermaps surface to the same type as the screen
	b2_enem_abajo = SDL_DisplayFormat(temp);
	
	if (b2_enem_abajo == NULL) {
	
		printf("Unable to convert bitmap.\n");
		return 1;
	}

	SDL_FreeSurface(temp);

	// Load the b2 enemy right
	temp = SDL_LoadBMP("b2_enem_der.bmp");
	if ( temp == NULL ) {
		printf("No pude cargar gráfico: %s\n", SDL_GetError());
		exit(1);
	}

	//convert the numbermaps surface to the same type as the screen
	b2_enem_der = SDL_DisplayFormat(temp);
	
	if (b2_enem_der == NULL) {
	
		printf("Unable to convert bitmap.\n");
		return 1;
	}

	SDL_FreeSurface(temp);

	// Load the b2 enemy left
	temp = SDL_LoadBMP("b2_enem_izq.bmp");
	if ( temp == NULL ) {
		printf("No pude cargar gráfico: %s\n", SDL_GetError());
		exit(1);
	}

	//convert the numbermaps surface to the same type as the screen
	b2_enem_izq = SDL_DisplayFormat(temp);
	
	if (b2_enem_izq == NULL) {
	
		printf("Unable to convert bitmap.\n");
		return 1;
	}

	SDL_FreeSurface(temp);

	// Load the b3 enemy up V
	temp = SDL_LoadBMP("b3_enem_arriba_v.bmp");
	if ( temp == NULL ) {
		printf("No pude cargar gráfico: %s\n", SDL_GetError());
		exit(1);
	}

	//convert the numbermaps surface to the same type as the screen
	b3_enem_arriba_v = SDL_DisplayFormat(temp);
	
	if (b3_enem_arriba_v == NULL) {
	
		printf("Unable to convert bitmap.\n");
		return 1;
	}

	SDL_FreeSurface(temp);

	// Load the b3 enemy down V
	temp = SDL_LoadBMP("b3_enem_abajo_v.bmp");
	if ( temp == NULL ) {
		printf("No pude cargar gráfico: %s\n", SDL_GetError());
		exit(1);
	}

	//convert the numbermaps surface to the same type as the screen
	b3_enem_abajo_v = SDL_DisplayFormat(temp);
	
	if (b3_enem_abajo_v == NULL) {
	
		printf("Unable to convert bitmap.\n");
		return 1;
	}

	SDL_FreeSurface(temp);

	// Load the b3 enemy right V
	temp = SDL_LoadBMP("b3_enem_der_v.bmp");
	if ( temp == NULL ) {
		printf("No pude cargar gráfico: %s\n", SDL_GetError());
		exit(1);
	}

	//convert the numbermaps surface to the same type as the screen
	b3_enem_der_v = SDL_DisplayFormat(temp);
	
	if (b3_enem_der_v == NULL) {
	
		printf("Unable to convert bitmap.\n");
		return 1;
	}

	SDL_FreeSurface(temp);

	// Load the b3 enemy left V
	temp = SDL_LoadBMP("b3_enem_izq_v.bmp");
	if ( temp == NULL ) {
		printf("No pude cargar gráfico: %s\n", SDL_GetError());
		exit(1);
	}

	//convert the numbermaps surface to the same type as the screen
	b3_enem_izq_v = SDL_DisplayFormat(temp);
	
	if (b3_enem_izq_v == NULL) {
	
		printf("Unable to convert bitmap.\n");
		return 1;
	}

	SDL_FreeSurface(temp);

	// Load the b3 enemy up A
	temp = SDL_LoadBMP("b3_enem_arriba_a.bmp");
	if ( temp == NULL ) {
		printf("No pude cargar gráfico: %s\n", SDL_GetError());
		exit(1);
	}

	//convert the numbermaps surface to the same type as the screen
	b3_enem_arriba_a = SDL_DisplayFormat(temp);
	
	if (b3_enem_arriba_a == NULL) {
	
		printf("Unable to convert bitmap.\n");
		return 1;
	}

	SDL_FreeSurface(temp);

	// Load the b3 enemy down A
	temp = SDL_LoadBMP("b3_enem_abajo_a.bmp");
	if ( temp == NULL ) {
		printf("No pude cargar gráfico: %s\n", SDL_GetError());
		exit(1);
	}

	//convert the numbermaps surface to the same type as the screen
	b3_enem_abajo_a = SDL_DisplayFormat(temp);
	
	if (b3_enem_abajo_a == NULL) {
	
		printf("Unable to convert bitmap.\n");
		return 1;
	}

	SDL_FreeSurface(temp);

	// Load the b3 enemy right A
	temp = SDL_LoadBMP("b3_enem_der_a.bmp");
	if ( temp == NULL ) {
		printf("No pude cargar gráfico: %s\n", SDL_GetError());
		exit(1);
	}

	//convert the numbermaps surface to the same type as the screen
	b3_enem_der_a = SDL_DisplayFormat(temp);
	
	if (b3_enem_der_a == NULL) {
	
		printf("Unable to convert bitmap.\n");
		return 1;
	}

	SDL_FreeSurface(temp);

	// Load the b3 enemy left A
	temp = SDL_LoadBMP("b3_enem_izq_a.bmp");
	if ( temp == NULL ) {
		printf("No pude cargar gráfico: %s\n", SDL_GetError());
		exit(1);
	}

	//convert the numbermaps surface to the same type as the screen
	b3_enem_izq_a = SDL_DisplayFormat(temp);
	
	if (b3_enem_izq_a == NULL) {
	
		printf("Unable to convert bitmap.\n");
		return 1;
	}

	SDL_FreeSurface(temp);

	// Load the b3 enemy up B
	temp = SDL_LoadBMP("b3_enem_arriba_b.bmp");
	if ( temp == NULL ) {
		printf("No pude cargar gráfico: %s\n", SDL_GetError());
		exit(1);
	}

	//convert the numbermaps surface to the same type as the screen
	b3_enem_arriba_b = SDL_DisplayFormat(temp);
	
	if (b3_enem_arriba_b == NULL) {
	
		printf("Unable to convert bitmap.\n");
		return 1;
	}

	SDL_FreeSurface(temp);

	// Load the b3 enemy down B
	temp = SDL_LoadBMP("b3_enem_abajo_b.bmp");
	if ( temp == NULL ) {
		printf("No pude cargar gráfico: %s\n", SDL_GetError());
		exit(1);
	}

	//convert the numbermaps surface to the same type as the screen
	b3_enem_abajo_b = SDL_DisplayFormat(temp);
	
	if (b3_enem_abajo_b == NULL) {
	
		printf("Unable to convert bitmap.\n");
		return 1;
	}

	SDL_FreeSurface(temp);

	// Load the b3 enemy right B
	temp = SDL_LoadBMP("b3_enem_der_b.bmp");
	if ( temp == NULL ) {
		printf("No pude cargar gráfico: %s\n", SDL_GetError());
		exit(1);
	}

	//convert the numbermaps surface to the same type as the screen
	b3_enem_der_b = SDL_DisplayFormat(temp);
	
	if (b3_enem_der_b == NULL) {
	
		printf("Unable to convert bitmap.\n");
		return 1;
	}

	SDL_FreeSurface(temp);

	// Load the b3 enemy left B
	temp = SDL_LoadBMP("b3_enem_izq_b.bmp");
	if ( temp == NULL ) {
		printf("No pude cargar gráfico: %s\n", SDL_GetError());
		exit(1);
	}

	//convert the numbermaps surface to the same type as the screen
	b3_enem_izq_b = SDL_DisplayFormat(temp);
	
	if (b3_enem_izq_b == NULL) {
	
		printf("Unable to convert bitmap.\n");
		return 1;
	}

	SDL_FreeSurface(temp);

	// Power up shield
	temp = SDL_LoadBMP("powerup_casco.bmp");
	if ( temp == NULL ) {
		printf("No pude cargar gráfico: %s\n", SDL_GetError());
		exit(1);
	}

	//convert the numbermaps surface to the same type as the screen
	powerup_casco = SDL_DisplayFormat(temp);
	
	if (powerup_casco == NULL) {
	
		printf("Unable to convert bitmap.\n");
		return 1;
	}

	SDL_FreeSurface(temp);

	// Power up tank
	temp = SDL_LoadBMP("powerup_tank.bmp");
	if ( temp == NULL ) {
		printf("No pude cargar gráfico: %s\n", SDL_GetError());
		exit(1);
	}

	//convert the numbermaps surface to the same type as the screen
	powerup_tank = SDL_DisplayFormat(temp);
	
	if (powerup_tank == NULL) {
	
		printf("Unable to convert bitmap.\n");
		return 1;
	}

	SDL_FreeSurface(temp);

	// Power up granada killall
	temp = SDL_LoadBMP("powerup_granada.bmp");
	if ( temp == NULL ) {
		printf("No pude cargar gráfico: %s\n", SDL_GetError());
		exit(1);
	}

	//convert the numbermaps surface to the same type as the screen
	powerup_granada = SDL_DisplayFormat(temp);
	
	if (powerup_granada == NULL) {
	
		printf("Unable to convert bitmap.\n");
		return 1;
	}

	SDL_FreeSurface(temp);

	// Load the shield
	temp = SDL_LoadBMP("shield_2_black.bmp");
	if ( temp == NULL ) {
		printf("No pude cargar gráfico: %s\n", SDL_GetError());
		exit(1);
	}
	/* Set the numbermaps colorkey. */
	SDL_SetColorKey(temp, SDL_SRCCOLORKEY, colorkey3);  // colorkey3 makes black transparent

	//convert the numbermaps surface to the same type as the screen
	shield_2_black = SDL_DisplayFormat(temp);
	
	if (shield_2_black == NULL) {
	
		printf("Unable to convert bitmap.\n");
		return 1;
	}

	SDL_FreeSurface(temp);

	// Load the base icon
	temp = SDL_LoadBMP("base.bmp");
	if ( temp == NULL ) {
		printf("No pude cargar gráfico: %s\n", SDL_GetError());
		exit(1);
	}
	/* Set the numbermaps colorkey. */
	SDL_SetColorKey(temp, SDL_SRCCOLORKEY, colorkey2);  

	//convert the numbermaps surface to the same type as the screen
	base = SDL_DisplayFormat(temp);
	
	if (base == NULL) {
	
		printf("Unable to convert bitmap.\n");
		return 1;
	}

	SDL_FreeSurface(temp);

	// Load battle city logo
	temp = SDL_LoadBMP("battle_city.bmp");
	if ( temp == NULL ) {
		printf("No pude cargar gráfico: %s\n", SDL_GetError());
		exit(1);
	}
	/* Set the numbermaps colorkey. */
	SDL_SetColorKey(temp, SDL_SRCCOLORKEY, colorkey2);  

	//convert the numbermaps surface to the same type as the screen
	battle_city = SDL_DisplayFormat(temp);
	
	if (battle_city == NULL) {
	
		printf("Unable to convert bitmap.\n");
		return 1;
	}

	SDL_FreeSurface(temp);

	// Load start logo
	temp = SDL_LoadBMP("inicio_3.bmp");
	if ( temp == NULL ) {
		printf("No pude cargar gráfico: %s\n", SDL_GetError());
		exit(1);
	}
	/* Set the numbermaps colorkey. */
	SDL_SetColorKey(temp, SDL_SRCCOLORKEY, colorkey3);  

	//convert the numbermaps surface to the same type as the screen
	inicio_3 = SDL_DisplayFormat(temp);
	
	if (inicio_3 == NULL) {
	
		printf("Unable to convert bitmap.\n");
		return 1;
	}

	SDL_FreeSurface(temp);

	// Load win logo
	temp = SDL_LoadBMP("ganaste.bmp");
	if ( temp == NULL ) {
		printf("No pude cargar gráfico: %s\n", SDL_GetError());
		exit(1);
	}
	/* Set the numbermaps colorkey. */
	SDL_SetColorKey(temp, SDL_SRCCOLORKEY, colorkey3); 

	//convert the numbermaps surface to the same type as the screen
	ganaste = SDL_DisplayFormat(temp);
	
	if (ganaste == NULL) {
	
		printf("Unable to convert bitmap.\n");
		return 1;
	}

	SDL_FreeSurface(temp);

	// Load steel block
	temp = SDL_LoadBMP("long_steel.bmp");
	if ( temp == NULL ) {
		printf("No pude cargar gráfico: %s\n", SDL_GetError());
		exit(1);
	}

	//convert the numbermaps surface to the same type as the screen
	long_steel = SDL_DisplayFormat(temp);
	
	if (long_steel == NULL) {
	
		printf("Unable to convert bitmap.\n");
		return 1;
	}

	SDL_FreeSurface(temp);

	// Load grass block
	temp = SDL_LoadBMP("trees2.bmp");
	if ( temp == NULL ) {
		printf("No pude cargar gráfico: %s\n", SDL_GetError());
		exit(1);
	}
	/* Set the numbermaps colorkey. */
	SDL_SetColorKey(temp, SDL_SRCCOLORKEY, colorkey2);

	//convert the numbermaps surface to the same type as the screen
	trees2 = SDL_DisplayFormat(temp);
	
	if (trees2 == NULL) {
	
		printf("Unable to convert bitmap.\n");
		return 1;
	}

	SDL_FreeSurface(temp);

	// Load the game over logo
	temp = SDL_LoadBMP("game_over.bmp");
	if ( temp == NULL ) {
		printf("No pude cargar gráfico: %s\n", SDL_GetError());
		exit(1);
	}
	/* Set the numbermaps colorkey. */
	SDL_SetColorKey(temp, SDL_SRCCOLORKEY, colorkey2);  

	//convert the numbermaps surface to the same type as the screen
	game_over = SDL_DisplayFormat(temp);
	
	if (game_over == NULL) {
	
		printf("Unable to convert bitmap.\n");
		return 1;
	}

	SDL_FreeSurface(temp);

	Uint8 *keystate = 0;
	Uint32 next_game_tick = SDL_GetTicks();
	int sleep = 0;
	int quit = 0;
	int x = 1;
	int num = 0;
	int temporal;
	int estado_espacio;
	int estado_juego = 0; // 0 Start screen - 1 Map1 - 2 Map2 - 3 Map3 - 4- Game Over  - 5- win
	set_mapa_1();

	/* Animate */
	while (quit == 0) {
		
		if (num == 20){
			num = 0;
		}

		draw_background();

		/* Update SDL's internal input state information. */
		SDL_PumpEvents();

		/* Grab a snapshot of the keyboard. */
		keystate = SDL_GetKeyState(NULL);

		/* Respond to input. */
		if (keystate[SDLK_ESCAPE]) {
			quit = 1;
		}

		if (estado_juego == 0){

			draw_inicio();
			if (keystate[SDLK_SPACE]) {
				estado_juego = 1;
				estado_espacio = 1;
			}
		}

		if ((estado_juego != 5) & (estado_juego != 4) & (estado_juego != 0)){// Make sure it is not the initial screen, or game over or win ...
			if (keystate[SDLK_DOWN]) {
				move_tanque(0);
				x = 0;
			}

			if (keystate[SDLK_UP]) {
				move_tanque(1);
				x = 1;
			}
			if (keystate[SDLK_RIGHT]){
				move_tanque(2);
				x = 2;

			}
			if (keystate[SDLK_LEFT]){
				move_tanque(3);
				x = 3;
			}

			draw_tanque(x);
			draw_walls();
			draw_bala_enem();
			draw_bala();
			set_tanque_enemigo(tanque_enem);
			move_tanque_enem(tanque_enem);
			draw_tanque_enem(tanque_enem);
			draw_escudo();
			draw_enemy_icon();
			draw_lives_icon();
			draw_flag_icon();
			draw_enemy_number(num_enemigos_vivos(tanque_enem));
			draw_lives_number(tanque.vidas);
			draw_flags_number();
			draw_block(block);
			draw_base();
			if (estado_espacio == 0){ // THis is to create only one bullet when pressing SPACE
				if (keystate[SDLK_SPACE]){
					set_bala(x, num);
					num += 1;
					estado_espacio = 1;
				}
			}
			if (!(keystate[SDLK_SPACE])) {
				estado_espacio = 0;
			}
			check_colision_block(bala, block);
			check_colision_block(bala_enem, block);
			check_colision_bala_tank_enem(bala, tanque_enem);
			move_bala();
			move_bala_enem();
			check_colision_bala_tank_aliado(bala_enem, tanque);
			check_colision_tanks(tanque_enem, tanque);
			draw_power_up(power_up);
			aplicar_power_ups();

			if (num_enemigos_vivos(tanque_enem) == 0){
				
				if (estado_juego == 3){ // Win (If you create more levels, you call them the same way)
					estado_juego = 5; 
				}
				if (estado_juego == 2){ // Change to map 3
					initialize_values();
					set_mapa_3();
					estado_juego = 3;
				}
				if (estado_juego == 1){ // Change to map 2
					initialize_values();
					set_mapa_2();
					estado_juego = 2;
				}
			}
		}

		if (estado_juego == 4){
			draw_game_over();
		}

		if (estado_juego == 5){
			draw_ganaste();
		}

		if ((tanque.vidas < 0) || (block[0].x == 800)){
			estado_juego = 4;
		}

		SDL_Flip(screen); // Ask SDL to update the entire screen 

		next_game_tick += 1000 / 60;
		sleep = next_game_tick - SDL_GetTicks();
	
		if( sleep >= 0 ) {
            		SDL_Delay(sleep);
        }
	}
	return 0;
}
