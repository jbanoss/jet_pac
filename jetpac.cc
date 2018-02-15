#include <esat/window.h>
#include <esat/draw.h>
#include <esat/sprite.h>
#include <esat/input.h>
#include <esat/time.h>
#include <stdlib.h>
#include <stdio.h>

const int sc=3, wX=256*sc, wY=192*sc;
int an=0;

esat::SpriteHandle sheet, *suelo;

esat::SpriteTransform stMap;

struct object{
	int speed,direccion;
	bool shoot, fly, inFloor; 
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
	
	//plat izq
	stMap.x = 32*sc;
	stMap.y = 72*sc;
	esat::DrawSprite(*(suelo+0), stMap);
	
	for(int i = 0; i < 4; i++){
		stMap.x += 8*sc;
		esat::DrawSprite(*(suelo+2), stMap);
	}
	
	stMap.x += 8*sc;
	esat::DrawSprite(*(suelo+4), stMap);
	
	//plat med
	stMap.x = 120*sc;
	stMap.y = 96*sc;
	esat::DrawSprite(*(suelo+0), stMap);
	
	for(int i = 0; i < 2; i++){
		stMap.x += 8*sc;
		esat::DrawSprite(*(suelo+2), stMap);
	}
	
	stMap.x += 8*sc;
	esat::DrawSprite(*(suelo+4), stMap);
	
	//plat der
	stMap.x = 192*sc;
	stMap.y = 48*sc;
	esat::DrawSprite(*(suelo+0), stMap);
	
	for(int i = 0; i < 4; i++){
		stMap.x += 8*sc;
		esat::DrawSprite(*(suelo+2), stMap);
	}
	
	stMap.x += 8*sc;
	esat::DrawSprite(*(suelo+4), stMap);
}

/*----------------------*/


/*---------Player-------*/

void InitPlayer(){
	player.st.x=wX/2;
	player.st.y=wY/2;
	player.st.scale_x=sc;
	player.st.scale_y=sc;
	player.speed=7.5;
  player.direccion=0;
	player.shoot=false;
  player.fly=false;
  player.inFloor=false;
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
		if(esat::IsSpecialKeyDown(esat::kSpecialKey_Space)){
			//player.shoot=true;
			//Disparar();
		}
    //flying
		if(esat::IsSpecialKeyPressed(esat::kSpecialKey_Up)){
			player.st.y-=player.speed;
      player.fly=true;
      ++an%=4;
		}else{
			player.st.y+=3;
      player.fly=false;
		}
    if(player.st.y<16*sc){
      player.st.y=16*sc;
    }
    if(player.st.y+esat::SpriteHeight(*(player.sprite+0))*sc>184*sc){
      player.st.y=184*sc-esat::SpriteHeight(*(player.sprite+0))*sc;
      player.inFloor=true;
    }else{
      player.inFloor=false;
    }
    
    //Left
		if(esat::IsSpecialKeyPressed(esat::kSpecialKey_Left)){
			player.st.x-=player.speed;
      player.direccion=0;      
      ++an%=4;
		}
    //Right
		if(esat::IsSpecialKeyPressed(esat::kSpecialKey_Right)){
			player.st.x+=player.speed;
      player.direccion=1;
      ++an%=4;
		}
		
	if(player.st.x+esat::SpriteWidth(*(player.sprite+0))<0){
		player.st.x=wX;
	}
	if(player.st.x>wX){
		player.st.x=0;
	}
}

void DrawPlayer(){
  //left
  if(player.direccion==0){
    if(player.fly){
      esat::DrawSprite(*(player.sprite+(an+8)),player.st);
    }
    if(!player.fly && player.inFloor){
      esat::DrawSprite(*(player.sprite+an),player.st);
    }
    if(!player.fly && !player.inFloor){
      esat::DrawSprite(*(player.sprite+(an+8)),player.st);
    }
  }

  
  
  //right
  if(player.direccion==1){
    if(player.fly){
      esat::DrawSprite(*(player.sprite+(an+12)),player.st);
    }  
    if(!player.fly && player.inFloor){
      esat::DrawSprite(*(player.sprite+(an+4)),player.st);
    }
    if(!player.fly && !player.inFloor){
      esat::DrawSprite(*(player.sprite+(an+12)),player.st);
    } 
  }
}

void PlayerAll(){
	PlayerSprites();
	PlayerControls();
	DrawPlayer();
}

/*----------------------*/


/*--------Enemies-------*/

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