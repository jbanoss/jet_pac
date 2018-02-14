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

void InitPlayer(){
	player.st.x=wX/2;
	player.st.y=wY/2;
	player.st.scale_x=sc;
	player.st.scale_y=sc;
	player.speed=5;
  player.direccion=0;
	player.shoot=false;
  player.fly=false;
	player.sprite=(esat::SpriteHandle*)malloc(16*sizeof(esat::SpriteHandle));
}

void PlayerSprites(){
	for(int i=0;i<8;i++){
		*(player.sprite+i)=esat::SubSprite(sheet,i*14,0,14,22);
	}
  for(int i=0;i<8;i++){
    *(player.sprite+(i+8))=esat::SubSprite(sheet,i*17,22,17,24);
  }
}

void PlayerControls(){
	//if(){
		if(esat::IsSpecialKeyPressed(esat::kSpecialKey_Space)){
			player.shoot=true;
		}
		if(esat::IsSpecialKeyPressed(esat::kSpecialKey_Up) && player.st.y>=(wY*0.15)){
			player.st.y-=player.speed;
      player.fly=true;
		}else{
			player.st.y+=3;
      player.fly=false;
		}
		if(esat::IsSpecialKeyPressed(esat::kSpecialKey_Left)){
			player.st.x-=player.speed;
      player.direccion=0;
		}
		if(esat::IsSpecialKeyPressed(esat::kSpecialKey_Right)){
			player.st.x+=player.speed;
      player.direccion=1;
		}
		
	if(player.st.x+esat::SpriteWidth(*(player.sprite+0))<0){
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
  if(player.direccion==0 && !player.fly){
      esat::DrawSprite(*(player.sprite+0),player.st);
  }
  if(player.direccion==0 && player.fly){
      esat::DrawSprite(*(player.sprite+8),player.st);
  }
  
  if(player.direccion==1 && !player.fly){
      esat::DrawSprite(*(player.sprite+4),player.st);
  }
  if(player.direccion==1 && player.fly){
      esat::DrawSprite(*(player.sprite+12),player.st);
  }
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