#include <esat/window.h>
#include <esat/draw.h>
#include <esat/sprite.h>
#include <esat/input.h>
#include <esat/time.h>
#include <stdlib.h>
#include <stdio.h>

const int sc=1, wX=256*sc, wY=192*sc;

esat::SpriteHandle sheet, *suelo;

esat::SpriteTransform stMap;

struct object{
	int speed,direccion;
	bool shoot; 
	esat::SpriteHandle *sprite;
	esat::SpriteTransform st;
};
object player;

void InitSprites(){
	sheet = esat::SpriteFromFile("./recursos/imagenes/sheet.png");
	
	suelo = (esat::SpriteHandle *)malloc(6*sizeof(esat::SpriteHandle));
	stMap.scale_x = sc;
	stMap.scale_y = sc;
	
	for(int i = 0; i < 6; i++){
		*(suelo+i) = esat::SubSprite(sheet, i*8, 46, 8, 8);
	}
}

/*--------Map-----------*/

void DrawMap(){
	//suelo
	stMap.x = 0;
	stMap.y = 184*sc;
	esat::DrawSprite(*(suelo+1), stMap);
	
	for(int i = 0; i < 30; i++){
		stMap.x += 8*sc;
		esat::DrawSprite(*(suelo+3), stMap);
	}
	
	stMap.x += 8*sc;
	esat::DrawSprite(*(suelo+5), stMap);
}

/*----------------------*/


/*---------Player-------*/
no ha funcionado
void InitPlayer(){
	player.st.x=wX/2;
	player.st.y=wY/2;
	player.st.scale_x=1;
	player.st.scale_y=1;
	player.speed=5;
	player.shoot=false;
	player.sprite=(esat::SpriteHandle*)malloc(16*sizeof(esat::SpriteHandle));
}

void PlayerSprites(){
	for(int i=0;i<16;i++){
		*(player.sprite+i)=esat::SpriteFromFile("./recursos/imagenes/tanques_12.png");
	}
}

void PlayerControls(){
	//if(){
		if(esat::IsSpecialKeyPressed(esat::kSpecialKey_Space)){
			player.shoot=true;
		}
		if(esat::IsSpecialKeyPressed(esat::kSpecialKey_Up) && player.st.y>=(wY*0.15)){
			player.st.y-=player.speed;
		}else{
			player.st.y+=player.speed;
		}
		if(esat::IsSpecialKeyPressed(esat::kSpecialKey_Left)){
			player.st.x-=player.speed;
		}
		if(esat::IsSpecialKeyPressed(esat::kSpecialKey_Right)){
			player.st.x+=player.speed;
		}
		
	if(player.st.x+esat::SpriteWidth(*(player.sprite+0))<33.5){
		player.st.x=wX;
	}
	if(player.st.x>wX){
		player.st.x=0;
	}
}

//player.st.x>plat.st.x+widht && player.st.x+esat::SpriteWidth(*(player.sprite+0))<plat.st.x 
//player.st.y>plat.st.y+height && player.st.y+esat::SpriteHeight(*(player.sprite+0))<plat.st.y

void PlayerColisions(){
	
}

void DrawPlayer(){
	esat::DrawSprite(*(player.sprite+0),player.st);
}

void PlayerAll(){
	PlayerSprites();
	PlayerControls();
	DrawPlayer();
}

/*----------------------*/


int esat::main(int argc, char **argv) {
 
  double current_time,last_time;
  unsigned char fps=25;
 
  esat::WindowInit(wX,wY);
  WindowSetMouseVisibility(true);

  InitSprites();
  
  InitPlayer();
  
  while(esat::WindowIsOpened() && !esat::IsSpecialKeyDown(esat::kSpecialKey_Escape)) {
	last_time = esat::Time();
	
	esat::DrawBegin();
    esat::DrawClear(0,0,0);
	
	DrawMap();
	
    PlayerAll();
	
    esat::DrawEnd();
	
	do{
    current_time = esat::Time();
    }while((current_time-last_time)<=1000.0/fps);
    esat::WindowFrame();
  }
  
  free(suelo);
  free(player.sprite);
  esat::WindowDestroy();
  return 0;
}