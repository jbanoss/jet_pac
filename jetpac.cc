#include <esat/window.h>
#include <esat/draw.h>
#include <esat/sprite.h>
#include <esat/input.h>
#include <esat/time.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

const int sc=3, wX=256*sc, wY=192*sc;
int nivel=1,nEnemigos=0,SpawnEnemy=0;
int an=0, animBonus = 0, r; //r de random
int cont_fuel=1;
bool Ship_Complete=false;
bool Respawn_Fuel=false;
bool bonusExist = false;

esat::SpriteHandle sheet, *suelo = NULL, bonusS,*spriteEnemy;

esat::SpriteTransform stMap, stBonus;

struct object{
	int speed,direccion,lado,inclinacion,sube,danza,anim,
	color,contSpawn,contMuerte,tipo;
	float x,y,aux;
	bool shoot, fly, inFloor; 
	bool inicio=true,rebote,dead=false,choqueIzq,choqueDer,choqueUp,choqueDown;

	esat::SpriteHandle *sprite;
	esat::SpriteHandle *sprite2;
	esat::SpriteHandle sprite3;//enemigo
	esat::SpriteHandle sprite4;//enemigo
	esat::SpriteTransform st;
};
object player;
object *enemigo= NULL;

struct Bonus{
	int animacion = 0, numS;
	esat::SpriteHandle *sprite = NULL;
	bool spawn = false;
}*bonus = NULL;

struct Tship{
	int ship_x;
	int ship_y;
	esat::SpriteHandle Sprite_Ship;	
	esat::SpriteTransform ts;
};
Tship *Ship;

void InitSprites(){
	sheet = esat::SpriteFromFile("./recursos/imagenes/sheet.png");
	
	suelo = (esat::SpriteHandle *)malloc(6*sizeof(esat::SpriteHandle));
	Ship=(Tship*)malloc(10*sizeof(Tship));
	stMap.scale_x = sc;
	stMap.scale_y = sc;
	
	for(int i = 0; i < 6; i++){
		*(suelo+i) = esat::SubSprite(sheet, i*8, 46, 8, 8);
	}
	/////////////////////////////////////////////////////////////////Load Sprite Ship.
	(*(Ship+0)).Sprite_Ship=esat::SubSprite(sheet,48,46,16,11);
	(*(Ship+0)).ts.scale_x = sc;
	(*(Ship+0)).ts.scale_y = sc;

	for(int j = 0; j <9; j++){
		(*(Ship+j+1)).Sprite_Ship=esat::SubSprite(sheet,0+j*16,57,16,16);
		(*(Ship+j+1)).ts.scale_x = sc;
		(*(Ship+j+1)).ts.scale_y = sc;
	}

	(*(Ship+1)).ts.x=173*sc;
	(*(Ship+1)).ts.y=168*sc;
	(*(Ship+4)).ts.x=128*sc;
	(*(Ship+4)).ts.y=80*sc;
	(*(Ship+7)).ts.x=52*sc;
	(*(Ship+7)).ts.y=56*sc;
}

void Fuel(){

	if(Ship_Complete){

		++cont_fuel%=40;

		if(cont_fuel==0){
			Respawn_Fuel=true;
			(*(Ship+0)).ts.x=rand()%765+2;
			(*(Ship+0)).ts.y=10;
			
			
		}

	}	

	if(Respawn_Fuel&&(*(Ship+0)).ts.x>0&&(*(Ship+0)).ts.x+(esat::SpriteWidth((*(Ship+0)).Sprite_Ship)*sc)<(256*sc)&&(*(Ship+0)).ts.x<173*sc||(*(Ship+0)).ts.x>(173+16)*sc){
		esat::DrawSprite((*(Ship+0)).Sprite_Ship,(*(Ship+0)).ts);
	}
	if(Respawn_Fuel){
		
		(*(Ship+0)).ts.y+=3;

	}
	
}

void Spawn_Ship(){

	esat::DrawSprite((*(Ship+1)).Sprite_Ship,(*(Ship+1)).ts);
	esat::DrawSprite((*(Ship+4)).Sprite_Ship,(*(Ship+4)).ts);
	esat::DrawSprite((*(Ship+7)).Sprite_Ship,(*(Ship+7)).ts);

}

void ColShip(){
	
	if(player.st.x<(*(Ship+7)).ts.x+(esat::SpriteWidth((*(Ship+7)).Sprite_Ship)*sc) && player.st.x+14*sc>(*(Ship+7)).ts.x &&
	player.st.y<(*(Ship+7)).ts.y+(esat::SpriteHeight((*(Ship+7)).Sprite_Ship)*sc)  && player.st.y+14*sc>(*(Ship+7)).ts.y){			
			(*(Ship+7)).ts.x=player.st.x;
			(*(Ship+7)).ts.y=player.st.y;				
		}
	
	if(player.st.x<(*(Ship+4)).ts.x+(esat::SpriteWidth((*(Ship+4)).Sprite_Ship)*sc) && player.st.x+14*sc>(*(Ship+4)).ts.x &&
	player.st.y<(*(Ship+4)).ts.y+(esat::SpriteHeight((*(Ship+4)).Sprite_Ship)*sc)  && player.st.y+14*sc>(*(Ship+4)).ts.y){			
			(*(Ship+4)).ts.x=player.st.x;
			(*(Ship+4)).ts.y=player.st.y;			
		}
}

void BuildShip(){

	if((*(Ship+4)).ts.x<(*(Ship+1)).ts.x+(esat::SpriteWidth((*(Ship+1)).Sprite_Ship)*sc)&&(*(Ship+4)).ts.x+(esat::SpriteWidth((*(Ship+4)).Sprite_Ship)*sc)>(*(Ship+1)).ts.x){
		
		(*(Ship+4)).ts.x=(*(Ship+1)).ts.x;
		if((*(Ship+4)).ts.y<(*(Ship+1)).ts.y-48){
			(*(Ship+4)).ts.y+=3;
		}
	}
	if((*(Ship+7)).ts.x<(*(Ship+1)).ts.x+(esat::SpriteWidth((*(Ship+1)).Sprite_Ship)*sc)&&(*(Ship+7)).ts.x+(esat::SpriteWidth((*(Ship+7)).Sprite_Ship)*sc)>(*(Ship+1)).ts.x){
		
		(*(Ship+7)).ts.x=(*(Ship+1)).ts.x;
		if((*(Ship+7)).ts.y<(*(Ship+1)).ts.y-95){
			(*(Ship+7)).ts.y+=3;
		}
	}
	if((*(Ship+7)).ts.y==(*(Ship+1)).ts.y-95){
		Ship_Complete=true;
	}
}

void ShipAll(){
  Fuel();
  Spawn_Ship();
  ColShip();
  BuildShip();
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
	player.st.y=170*sc;
	player.st.scale_x=sc;
	player.st.scale_y=sc;
	player.speed=5;
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

bool Col1Up(){
  if(esat::IsSpecialKeyPressed(esat::kSpecialKey_Up)){
    return player.st.y>(72)*sc && player.st.y<(72+8)*sc &&
    player.st.x+esat::SpriteWidth(*(player.sprite+8))*sc>(32)*sc && player.st.x<(32+48)*sc;
  }
  return false;
}
bool Col1Left(){
  if(esat::IsSpecialKeyPressed(esat::kSpecialKey_Left)){
    return player.st.y+esat::SpriteHeight(*(player.sprite+8))*sc>(72)*sc && player.st.y<(72+7)*sc &&
    player.st.x>(32)*sc && player.st.x<(32+48)*sc;
  }
  return false;
}
bool Col1Right(){
  if(esat::IsSpecialKeyPressed(esat::kSpecialKey_Right)){
    return player.st.y+esat::SpriteHeight(*(player.sprite+8))*sc>(72)*sc && player.st.y<(72+7)*sc &&
    player.st.x+esat::SpriteWidth(*(player.sprite+8))*sc>(32)*sc && player.st.x+esat::SpriteWidth(*(player.sprite+8))*sc<(32+48)*sc;
  }
  return false;
}

bool Col2Up(){
  if(esat::IsSpecialKeyPressed(esat::kSpecialKey_Up)){
    return player.st.y>(96)*sc && player.st.y<(96+8)*sc &&
    player.st.x+esat::SpriteWidth(*(player.sprite+8))*sc>(120)*sc && player.st.x<(120+32)*sc;
  }
  return false;
}
bool Col2Left(){
  if(esat::IsSpecialKeyPressed(esat::kSpecialKey_Left)){
    return player.st.y+esat::SpriteHeight(*(player.sprite+8))*sc>(96)*sc && player.st.y<(96+7)*sc &&
    player.st.x>(120)*sc && player.st.x<(120+32)*sc;
  }
  return false;
}
bool Col2Right(){
  if(esat::IsSpecialKeyPressed(esat::kSpecialKey_Right)){
    return player.st.y+esat::SpriteHeight(*(player.sprite+8))*sc>(96)*sc && player.st.y<(96+7)*sc &&
    player.st.x+esat::SpriteWidth(*(player.sprite+8))*sc>(120)*sc && player.st.x+esat::SpriteWidth(*(player.sprite+8))*sc<(120+32)*sc;
  }
  return false;
}

bool Col3Up(){
  if(esat::IsSpecialKeyPressed(esat::kSpecialKey_Up)){
    return player.st.y>(48)*sc && player.st.y<(48+8)*sc &&
    player.st.x+esat::SpriteWidth(*(player.sprite+8))*sc>(192)*sc && player.st.x<(192+48)*sc;
  } 
  return false;
}
bool Col3Left(){
  if(esat::IsSpecialKeyPressed(esat::kSpecialKey_Left)){
    return player.st.y+esat::SpriteHeight(*(player.sprite+8))*sc>(48)*sc && player.st.y<(48+7)*sc &&
    player.st.x>(192)*sc && player.st.x<(192+48)*sc;
  }
  return false;
}
bool Col3Right(){
  if(esat::IsSpecialKeyPressed(esat::kSpecialKey_Right)){
    return player.st.y+esat::SpriteHeight(*(player.sprite+8))*sc>(48)*sc && player.st.y<(48+7)*sc &&
    player.st.x+esat::SpriteWidth(*(player.sprite+8))*sc>(192)*sc && player.st.x+esat::SpriteWidth(*(player.sprite+8))*sc<(192+48)*sc;
  }
  return false;
}

void PlayerControls(){
		if(esat::IsSpecialKeyDown(esat::kSpecialKey_Space)){
			//player.shoot=true;
			//Disparar();
		}
    
    //flying
		if(esat::IsSpecialKeyPressed(esat::kSpecialKey_Up) && !Col1Up() && !Col2Up() && !Col3Up()){
			player.st.y-=player.speed;
      player.fly=true;
      ++an%=4;
		}else{
			if(!Col1Up() && !Col2Up() && !Col3Up()) player.st.y+=3;
      player.fly=false;
		}
    if(player.st.y<16*sc){
      player.st.y=16*sc;
    }
    
    //walk
    if(player.st.y+esat::SpriteHeight(*(player.sprite+0))*sc>184*sc){
      player.st.y=184*sc-esat::SpriteHeight(*(player.sprite+0))*sc;
      player.inFloor=true;
    }else{
      player.inFloor=false;
    }
    if(player.st.y+esat::SpriteHeight(*(player.sprite+0))*sc>72*sc && player.st.y<(72+6)*sc &&
    player.st.x+esat::SpriteWidth(*(player.sprite+8))*sc>(35)*sc && player.st.x<(32+46)*sc){
      player.st.y=72*sc-esat::SpriteHeight(*(player.sprite+0))*sc;
      player.inFloor=true;
      if(esat::IsSpecialKeyPressed(esat::kSpecialKey_Left)){
			player.st.x-=player.speed;
      player.direccion=0;      
      ++an%=4;
      }
      if(esat::IsSpecialKeyPressed(esat::kSpecialKey_Right)){
			player.st.x+=player.speed;
      player.direccion=1;
      ++an%=4;
		}
    }
    if(player.st.y+esat::SpriteHeight(*(player.sprite+0))*sc>96*sc && player.st.y<(96+6)*sc &&
    player.st.x+esat::SpriteWidth(*(player.sprite+8))*sc>(123)*sc && player.st.x<(120+30)*sc){
      player.st.y=96*sc-esat::SpriteHeight(*(player.sprite+0))*sc;
      player.inFloor=true;
      if(esat::IsSpecialKeyPressed(esat::kSpecialKey_Left)){
			player.st.x-=player.speed;
      player.direccion=0;      
      ++an%=4;
      }
      if(esat::IsSpecialKeyPressed(esat::kSpecialKey_Right)){
			player.st.x+=player.speed;
      player.direccion=1;
      ++an%=4;
		}
    }
    if(player.st.y+esat::SpriteHeight(*(player.sprite+0))*sc>48*sc && player.st.y<(48+6)*sc &&
      player.st.x+esat::SpriteWidth(*(player.sprite+8))*sc>(195)*sc && player.st.x<(192+46)*sc){
      player.st.y=48*sc-esat::SpriteHeight(*(player.sprite+0))*sc;
      player.inFloor=true;
      if(esat::IsSpecialKeyPressed(esat::kSpecialKey_Left)){
			player.st.x-=player.speed;
      player.direccion=0;      
      ++an%=4;
      }
      if(esat::IsSpecialKeyPressed(esat::kSpecialKey_Right)){
			player.st.x+=player.speed;
      player.direccion=1;
      ++an%=4;
		}
    }
    
    //Left
		if(esat::IsSpecialKeyPressed(esat::kSpecialKey_Left) && !Col1Left() && !Col2Left() && !Col3Left()){
			player.st.x-=player.speed;
      player.direccion=0;      
      ++an%=4;
		}
    //Right
		if(esat::IsSpecialKeyPressed(esat::kSpecialKey_Right) && !Col1Right() && !Col2Right() && !Col3Right()){
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
	//PlayerCol();
	DrawPlayer();
}

/*----------------------*/


/*--------Enemies-------*/

void enemySprites(){
	spriteEnemy = (esat::SpriteHandle *)malloc(65*sizeof(esat::SpriteHandle));
		//(enemigo)->sprite2 = (esat::SpriteHandle *)malloc(40*sizeof(esat::SpriteHandle));
		//meteoritos (8-15 al reves)
	for(int i=0;i<8;i++){
		for(int j=0;j<2;j++){
		*(spriteEnemy+((2*i)+j))=esat::SubSprite(sheet,(j*16)+0,(i*11)+137,16,11);
		
		}
	}
		//pompones (sprite 16 hasta 24)
	for(int i=0;i<4;i++){
		for(int j=0;j<2;j++){
		*(spriteEnemy+((2*i)+j+16))=esat::SubSprite(sheet,(j*16)+32,(i*14)+137,16,14);
		}
	}
		//burbujas (sprite 24 hasta 32)
	for(int i=0;i<4;i++){
		for(int j=0;j<2;j++){
		*(spriteEnemy+((2*i)+j+24))=esat::SubSprite(sheet,(j*16)+64,(i*16)+137,16,16);
		}
	}
	//naves1 (sprite 32 hasta 42,del 36 al 41 al reves)
	for(int i=0;i<10;i++){
		*(spriteEnemy+(i+32))=esat::SubSprite(sheet,96,(i*7)+137,16,7);
	}
	//ovnis (sprite 42 hasta 46)
	for(int i=0;i<4;i++){
		*(spriteEnemy+(i+42))=esat::SubSprite(sheet,112,(i*8)+121,16,8);
	}
	//cruces (sprite 46 hasta 50)
	for(int i=0;i<4;i++){
		*(spriteEnemy+(i+46))=esat::SubSprite(sheet,128,(i*15)+121,16,15);
	}
	//naves2 (sprite 50 hasta 58 del 54 al 57 al reves)
	for(int i=0;i<2;i++){
		for(int j=0;j<4;j++){
		*(spriteEnemy+(i*4)+j+50)=esat::SubSprite(sheet,(j*16)+0,(i*14)+225,16,14);
		}
	}
	//bolsas (sprite 58 hasta 62)
	for(int i=0;i<4;i++){
		*(spriteEnemy+(i+58))=esat::SubSprite(sheet,(i*16)+32,201,16,14);
	}
	//printf("entrada enemigos 6 \n");
	//explosion
	for(int i=0;i<3;i++){
		*(spriteEnemy+(i+62))=esat::SubSprite(sheet,(i*24)+32,121,24,16);
	}
}
void initEnemigo(int actual){
	srand(time(NULL));
		(enemigo+actual)->contMuerte=0;
		(enemigo+actual)->lado=rand()%2;
(enemigo+actual)->choqueIzq=false;
		(enemigo+actual)->choqueDer=false;
		(enemigo+actual)->choqueUp=false;
		(enemigo+actual)->choqueDown=false;
		printf("lado %d \n",(enemigo+actual)->lado);
		//(enemigo+actual)->direccion=rand()%2;
		//printf("direccion %d \n",(enemigo+actual)->direccion);
		(enemigo+actual)->sube=rand()%2;
		printf("sube %d \n",(enemigo+actual)->sube);
		(enemigo+actual)->st.scale_x=sc;
		printf("st.scale_x %f \n",(enemigo+actual)->st.scale_x);
		(enemigo+actual)->st.scale_y=sc;
		printf("st.scale_y %f \n",(enemigo+actual)->st.scale_y);
		(enemigo+actual)->anim=0;
		printf("anim %d \n",(enemigo+actual)->anim);
		(enemigo+actual)->st.angle=0.0f;
		printf("st.angle %f \n",(enemigo+actual)->st.angle);
		if((enemigo+actual)->lado==1){
			(enemigo+actual)->rebote=false;
			printf("rebote %d \n",(enemigo+actual)->rebote);
			(enemigo+actual)->st.x=wX-esat::SpriteWidth(*(spriteEnemy+0))*sc;
			printf("st.x %f \n",(enemigo+actual)->st.x);
			(enemigo+actual)->st.y=rand()%wY-200;
			printf("st.y %f \n",(enemigo+actual)->st.y);
			if((enemigo+actual)->st.y<50*sc){
				(enemigo+actual)->st.y=(50*sc)+rand()%100*sc;
				printf("st.y 2%f \n",(enemigo+actual)->st.y);
			}
			(enemigo+actual)->direccion=0;
			printf("direccion %d \n",(enemigo+actual)->direccion);
		}else{
			(enemigo+actual)->rebote=true;
			printf("rebote %d \n",(enemigo+actual)->rebote);
			(enemigo+actual)->st.y=rand()%wY-200;
			printf("st.y %f \n",(enemigo+actual)->st.y);
			if((enemigo+actual)->st.y<50*sc){
				(enemigo+actual)->st.y=(50*sc)+rand()%100*sc;
			}			
			(enemigo+actual)->st.x=0;
			printf("st.x %f \n",(enemigo+actual)->st.x);
			(enemigo+actual)->direccion=1;
			printf("direccion %d \n",(enemigo+actual)->direccion);
		}
}
void colisionesEnemigos(int actual){
	srand(time(NULL));
	if((enemigo+actual)->st.x-8*sc<=(32+48)*sc && (enemigo+actual)->st.x>=(32)*sc 
	&& (enemigo+actual)->st.y<=(72+8)*sc && (enemigo+actual)->st.y+(esat::SpriteHeight((enemigo+actual)->sprite3))*sc>=(72)*sc||
	(enemigo+actual)->st.x-8*sc<=(120+32)*sc && (enemigo+actual)->st.x>=(120)*sc 
	&& (enemigo+actual)->st.y<=(96+8)*sc && (enemigo+actual)->st.y+(esat::SpriteHeight((enemigo+actual)->sprite3))*sc>=(96)*sc ||
	(enemigo+actual)->st.x-8*sc<=(192+48)*sc && (enemigo+actual)->st.x>=(192)*sc 
	&& (enemigo+actual)->st.y<=(48+8)*sc && (enemigo+actual)->st.y+(esat::SpriteHeight((enemigo+actual)->sprite3))*sc>=(48)*sc){
		(enemigo+actual)->choqueIzq=true;
	}else{
				(enemigo+actual)->choqueIzq=false;
	}
	

	if((enemigo+actual)->st.x+esat::SpriteWidth(((enemigo+actual)->sprite3))<=(32+48)*sc && (enemigo+actual)->st.x+(esat::SpriteWidth(((enemigo+actual)->sprite3))+8*sc)*sc>=(32)*sc 
	&& (enemigo+actual)->st.y<=(72+8)*sc && (enemigo+actual)->st.y+(esat::SpriteHeight((enemigo+actual)->sprite3))*sc>=(72)*sc||
	(enemigo+actual)->st.x+esat::SpriteWidth(((enemigo+actual)->sprite3))<=(120+32)*sc && (enemigo+actual)->st.x+(esat::SpriteWidth(((enemigo+actual)->sprite3))+8*sc)*sc>=(120)*sc 
	&& (enemigo+actual)->st.y<=(96+8)*sc && (enemigo+actual)->st.y+(esat::SpriteHeight((enemigo+actual)->sprite3))*sc>=(96)*sc ||
	(enemigo+actual)->st.x+esat::SpriteWidth(((enemigo+actual)->sprite3))<=(192+48)*sc && (enemigo+actual)->st.x+(esat::SpriteWidth(((enemigo+actual)->sprite3))+8*sc)*sc>=(192)*sc 
	&& (enemigo+actual)->st.y<=(48+8)*sc && (enemigo+actual)->st.y+(esat::SpriteHeight((enemigo+actual)->sprite3))*sc>=(48)*sc){
		(enemigo+actual)->choqueDer=true;
	}else{
		(enemigo+actual)->choqueDer=false;
	}

	if((enemigo+actual)->st.x<=(32+48)*sc && (enemigo+actual)->st.x+(esat::SpriteWidth((enemigo+actual)->sprite3))*sc>=(32)*sc 
	&& (enemigo+actual)->st.y-8*sc<=(72+8)*sc && (enemigo+actual)->st.y>=(72)*sc||
	(enemigo+actual)->st.x<=(120+32)*sc && (enemigo+actual)->st.x+(esat::SpriteWidth((enemigo+actual)->sprite3))*sc>=(120)*sc 
	&& (enemigo+actual)->st.y-8*sc<=(96+8)*sc && (enemigo+actual)->st.y>=(96)*sc ||
	(enemigo+actual)->st.x<=(192+48)*sc && (enemigo+actual)->st.x+(esat::SpriteWidth((enemigo+actual)->sprite3))*sc>=(192)*sc 
	&& (enemigo+actual)->st.y-8*sc<=(48+8)*sc && (enemigo+actual)->st.y>=(48)*sc){
		(enemigo+actual)->choqueUp=true;
	}else{
		(enemigo+actual)->choqueUp=false;
	}
		if((enemigo+actual)->st.x<=(32+48)*sc && (enemigo+actual)->st.x+(esat::SpriteWidth((enemigo+actual)->sprite3))*sc>=(32)*sc 
	&& (enemigo+actual)->st.y<=(72+8)*sc && (enemigo+actual)->st.y+(esat::SpriteHeight(((enemigo+actual)->sprite3))+8*sc)*sc>=(72)*sc||
	(enemigo+actual)->st.x<=(120+32)*sc && (enemigo+actual)->st.x+(esat::SpriteWidth((enemigo+actual)->sprite3))*sc>=(120)*sc 
	&& (enemigo+actual)->st.y<=(96+8)*sc && (enemigo+actual)->st.y+(esat::SpriteHeight(((enemigo+actual)->sprite3))+8*sc)*sc>=(96)*sc ||
	(enemigo+actual)->st.x<=(192+48)*sc && (enemigo+actual)->st.x+(esat::SpriteWidth((enemigo+actual)->sprite3))*sc>=(192)*sc 
	&& (enemigo+actual)->st.y+esat::SpriteHeight(((enemigo+actual)->sprite3))<=(48+8)*sc && (enemigo+actual)->st.y+(esat::SpriteHeight(((enemigo+actual)->sprite3))+8*sc)*sc>=(48)*sc){
		(enemigo+actual)->choqueDown=true;
	}else{
		(enemigo+actual)->choqueDown=false;
	}
	if((enemigo+actual)->st.x+esat::SpriteWidth((enemigo+actual)->sprite3)*sc>wX){
		(enemigo+actual)->st.x=0;
	}else if((enemigo+actual)->st.x<0){
		(enemigo+actual)->st.x=wX-esat::SpriteWidth((enemigo+actual)->sprite3)*sc;
	}
	//tipo 1 Explotan
	if((enemigo+actual)->tipo==1){
		if((enemigo+actual)->st.y+esat::SpriteHeight(((enemigo+actual)->sprite3))*sc>wY-8*sc){
			//enemigo se para y reproduce la animacion
			(enemigo+actual)->dead=true;
		}else if((enemigo+actual)->st.y<=16){
			(enemigo+actual)->dead=true;
		}
		if((enemigo+actual)->choqueIzq || (enemigo+actual)->choqueDer ||(enemigo+actual)->choqueDown || (enemigo+actual)->choqueUp){
			(enemigo+actual)->dead=true;
		}
	//tipo 2 Rebotan
	}else if((enemigo+actual)->tipo==2){
		if((enemigo+actual)->st.x>wX){
			(enemigo+actual)->st.x=0;
		}else if((enemigo+actual)->st.x<0){
			(enemigo+actual)->st.x=wX;
		}
		if((enemigo+actual)->st.y+esat::SpriteHeight(((enemigo+actual)->sprite3))*sc>wY-8*sc){
			(enemigo+actual)->danza=0;
			(enemigo+actual)->sube=1;
		}else if((enemigo+actual)->st.y<=16*sc){
				(enemigo+actual)->danza=0;
			(enemigo+actual)->sube=0;
		}
		if((enemigo+actual)->choqueIzq){
			(enemigo+actual)->direccion=1;
			(enemigo+actual)->danza=0;
		}
		if((enemigo+actual)->choqueDer){
			(enemigo+actual)->direccion=0;
			(enemigo+actual)->danza=0;
		}
		if((enemigo+actual)->choqueUp){
			(enemigo+actual)->sube=0;
			(enemigo+actual)->danza=0;
		}
		if((enemigo+actual)->choqueDown){
			(enemigo+actual)->sube=1;
			(enemigo+actual)->danza=0;
		}
	//tipo 3 Persiguen
	}else if((enemigo+actual)->tipo==3){
		
	}
}

void enemigo1(){
	//meteorito
	
	for(int i=0;i<nEnemigos;i++){
	/////////////////////////////////////////////
	//printf("enemigo %d inicio %d \n",i,(enemigo+i)->inicio);
	if(!(enemigo+i)->dead){
	if((enemigo+i)->inicio){
		srand(time(NULL));
	int colores=rand()%4;
	initEnemigo(i);
	(enemigo+i)->inclinacion=(rand()%3);
	(enemigo+i)->speed=6;
	(enemigo+i)->tipo=1;
	switch(colores){
		case 0:
		(enemigo+i)->color=0;
		break;
		case 1:
		(enemigo+i)->color=2;
		break;
		case 2:
		(enemigo+i)->color=4;
		break;
		case 3:
		(enemigo+i)->color=6;
		break;
	}
	if((enemigo+i)->direccion==1){
		//no encuentra el sprite la sintaxis esta correcta
		((enemigo+i)->sprite3)= *(spriteEnemy+(enemigo+i)->color);//((enemigo+i)->color));
		((enemigo+i)->sprite4)= *(spriteEnemy+(enemigo+i)->color+1);//(enemigo+i)->color+1);
	}else{
		((enemigo+i)->sprite3)= *(spriteEnemy+(enemigo+i)->color+8);//(enemigo+i)->color+8);
		((enemigo+i)->sprite4)= *(spriteEnemy+(enemigo+i)->color+9);//(enemigo+i)->color+9);
	}
	(enemigo+i)->inicio=false;
	}
	/////////////////////////////////////////////
	 if((enemigo+i)->direccion==1){
		(enemigo+i)->st.x+=(enemigo+i)->speed;
		 if((enemigo+i)->sube==0){

			(enemigo+i)->st.y+=(enemigo+i)->inclinacion;
		 }else{
			(enemigo+i)->st.y-=(enemigo+i)->inclinacion;
		 }
	 }else{
		
		(enemigo+i)->st.x-=(enemigo+i)->speed;
		if((enemigo+i)->sube==0){
				(enemigo+i)->st.y+=(enemigo+i)->inclinacion;
		}else{

				(enemigo+i)->st.y-=(enemigo+i)->inclinacion;
		}
	 }
	  colisionesEnemigos(i);
	}else{
		(enemigo+i)->contSpawn++;
	}
		}

}
void enemigo2(){
	//pompon
	//rebote cambia con las colisiones y controla si choca
	//de frente para que vuelva hacia atras
	////////////////////////////////////////////
	
	for(int i=0;i<nEnemigos;i++){
		if(!(enemigo+i)->dead){
	if((enemigo+i)->inicio){
	srand(time(NULL));
	int colores=rand()%4;
	(enemigo+i)->tipo=2;
	initEnemigo(i);
	(enemigo+i)->inclinacion=6;
	(enemigo+i)->speed=3;
	switch(colores){
		case 0:
		(enemigo+i)->color=0;
		break;
		case 1:
		(enemigo+i)->color=2;
		break;
		case 2:
		(enemigo+i)->color=4;
		break;
		case 3:
		(enemigo+i)->color=6;
		break;
	}
		((enemigo+i)->sprite3)= *(spriteEnemy+(enemigo+i)->color+16);
		((enemigo+i)->sprite4)= *(spriteEnemy+(enemigo+i)->color+17);
	(enemigo+i)->inicio=false;
	}
	////////////////////////////////////////
	 if((enemigo+i)->direccion==1){

		(enemigo+i)->st.x+=(enemigo+i)->speed;
	 	if((enemigo+i)->sube==0){
		(enemigo+i)->st.y+=(enemigo+i)->inclinacion;
		}else{
		(enemigo+i)->st.y-=(enemigo+i)->inclinacion;
		}
	 
	 }else{
		(enemigo+i)->st.x-=(enemigo+i)->speed;
	 	if((enemigo+i)->sube==0){
		(enemigo+i)->st.y+=(enemigo+i)->inclinacion;
		}else{
		(enemigo+i)->st.y-=(enemigo+i)->inclinacion;
		}
	 }
 colisionesEnemigos(i);
	}else{
		(enemigo+i)->contSpawn++;
	}
	}
}

void enemigo3(){
	//burbuja
	for(int i=0;i<nEnemigos;i++){
	if(!(enemigo+i)->dead){
	if((enemigo+i)->inicio){
		srand(time(NULL));
	int colores=rand()%4;
	(enemigo+i)->danza=0;
	(enemigo+i)->tipo=2;
	initEnemigo(i);
	(enemigo+i)->inclinacion=4;
	(enemigo+i)->speed=4;
	printf("%d \n",colores);
	switch(colores){
		case 0:
		(enemigo+i)->color=0;
		break;
		case 1:
		(enemigo+i)->color=2;
		break;
		case 2:
		(enemigo+i)->color=4;
		break;
		case 3:
		(enemigo+i)->color=6;
		break;
	}
	((enemigo+i)->sprite3)= *(spriteEnemy+(enemigo+i)->color+24);
	((enemigo+i)->sprite4)= *(spriteEnemy+(enemigo+i)->color+25);
		(enemigo+i)->inicio=false;
		}
	if((enemigo+i)->danza>40){
		(enemigo+i)->sube=rand()%3;
	}else{
		(enemigo+i)->danza++;
	}
	if((enemigo+i)->direccion==1){
	(enemigo+i)->st.x+=(enemigo+i)->speed;

	if((enemigo+i)->sube==0){
		(enemigo+i)->st.y+=(enemigo+i)->inclinacion;
	}else if((enemigo+i)->sube==1){
		(enemigo+i)->st.y-=(enemigo+i)->inclinacion;
	}else{
		(enemigo+i)->st.y+=0;
	}
	 }else{
		
		(enemigo+i)->st.x-=(enemigo+i)->speed;
	if((enemigo+i)->sube==0){
		(enemigo+i)->st.y+=(enemigo+i)->inclinacion;
	}else if((enemigo+i)->sube==1){
		(enemigo+i)->st.y-=(enemigo+i)->inclinacion;
	}else{
		(enemigo+i)->st.y+=0;
	}
	 }
	 colisionesEnemigos(i);
	}else{
		(enemigo+i)->contSpawn++;
	}
}
}

void enemigo4(){
	//nave1
	/////////////////////////////////////////////
	for(int i=0;i<nEnemigos;i++){
		if(!(enemigo+i)->dead){
	if((enemigo+i)->inicio){
		srand(time(NULL));
	int colores=rand()%5;
	initEnemigo(i);
	(enemigo+i)->aux=(enemigo+i)->st.y;
	(enemigo+i)->danza=0;
	(enemigo+i)->tipo=1;
	(enemigo+i)->inclinacion=5;
	(enemigo+i)->speed=5;
		switch(colores){
		case 0:
		(enemigo+i)->color=0;
		break;
		case 1:
		(enemigo+i)->color=1;
		break;
		case 2:
		(enemigo+i)->color=2;
		break;
		case 3:
		(enemigo+i)->color=3;
		break;
		case 4:
		(enemigo+i)->color=4;
		break;
	}
	if((enemigo+i)->direccion==1){	
		((enemigo+i)->sprite3)= *(spriteEnemy+(enemigo+i)->color+32);
		((enemigo+i)->sprite4)= *(spriteEnemy+(enemigo+i)->color+32);
	}else{
		((enemigo+i)->sprite3)= *(spriteEnemy+(enemigo+i)->color+37);
		((enemigo+i)->sprite4)= *(spriteEnemy+(enemigo+i)->color+37);
	}
	(enemigo+i)->inicio=false;
	}
	/////////////////////////////////////////////
	 if((enemigo+i)->direccion==1){
	if((enemigo+i)->danza<50){
			if((enemigo+i)->st.y>(enemigo+i)->aux+20){
				(enemigo+i)->rebote=false;
			
			}else if((enemigo+i)->st.y<(enemigo+i)->aux-20){
				(enemigo+i)->rebote=true;
			}
			if((enemigo+i)->rebote){
			(enemigo+i)->st.y+=(enemigo+i)->inclinacion;	
			}else{
			(enemigo+i)->st.y-=(enemigo+i)->inclinacion;
			}
		}else{
			 (enemigo+i)->st.x+=(enemigo+i)->speed;
			 //////CAMBIAR RATON POR POSICION JUGADOR
			 if((enemigo+i)->st.y<player.st.y+(esat::SpriteHeight(*(player.sprite+8))/2)*sc){
				(enemigo+i)->st.y+=(enemigo+i)->inclinacion;
			 }else{
				(enemigo+i)->st.y-=(enemigo+i)->inclinacion;
			 }
		}
	 }else{
	if((enemigo+i)->danza<50){
		if((enemigo+i)->st.y>(enemigo+i)->aux+20){
			(enemigo+i)->rebote=false;
			}else if((enemigo+i)->st.y<(enemigo+i)->aux-20){
				(enemigo+i)->rebote=true;
			}
			if((enemigo+i)->rebote){
			(enemigo+i)->st.y+=(enemigo+i)->inclinacion;	
			}else{
			(enemigo+i)->st.y-=(enemigo+i)->inclinacion;
			}
		}else{
		(enemigo+i)->st.x-=(enemigo+i)->speed;
		if((enemigo+i)->st.y<player.st.y+(esat::SpriteHeight(*(player.sprite+8))/2)*sc){
			(enemigo+i)->st.y+=(enemigo+i)->inclinacion;
		}else{
			(enemigo+i)->st.y-=(enemigo+i)->inclinacion;
		}
		}
	 }
	 colisionesEnemigos(i);

	 (enemigo+i)->danza++;
	}else{
		(enemigo+i)->contSpawn++;
	}
	}
}

void enemigo5(){
	//ovni
	for(int i=0;i<nEnemigos;i++){
	/////////////////////////////////////////////
	if(!(enemigo+i)->dead){
		if((enemigo+i)->inicio){
			srand(time(NULL));
		int colores=rand()%4;
		initEnemigo(i);
		(enemigo+i)->inclinacion=4;
		(enemigo+i)->speed=3;
		(enemigo+i)->tipo=3;
		switch(colores){
			case 0:
			(enemigo+i)->color=0;
			break;
			case 1:
			(enemigo+i)->color=1;
			break;
			case 2:
			(enemigo+i)->color=2;
			break;
			case 3:
			(enemigo+i)->color=3;
			break;
		}
		((enemigo+i)->sprite3)= *(spriteEnemy+(enemigo+i)->color+42);
		((enemigo+i)->sprite4)= *(spriteEnemy+(enemigo+i)->color+42);
		(enemigo+i)->inicio=false;
		}
		/////////////////////////////////////////////
			 if((enemigo+i)->st.x<player.st.x+(esat::SpriteWidth(*(player.sprite+8))/2)*sc){
			(enemigo+i)->st.x+=(enemigo+i)->speed;
			 }else{
			(enemigo+i)->st.x-=(enemigo+i)->speed;
			 }
				 ////////////CAMBIAR RATON POR POSICION JUGADOR
			if((enemigo+i)->st.y<player.st.y+(esat::SpriteHeight(*(player.sprite+8))/2)*sc){
				(enemigo+i)->st.y+=(enemigo+i)->inclinacion;
			 }else{
				(enemigo+i)->st.y-=(enemigo+i)->inclinacion;
			 }
		 colisionesEnemigos(i);
		}else{
			(enemigo+i)->contSpawn++;
		}
		
	}
}

void enemigo6(){
	//cruces
	for(int i=0;i<nEnemigos;i++){
	if(!(enemigo+i)->dead){
	if((enemigo+i)->inicio){
srand(time(NULL));
	int colores=rand()%4;
	(enemigo+i)->tipo=2;
	initEnemigo(i);
	(enemigo+i)->inclinacion=rand()%5;
	(enemigo+i)->speed=4;
	switch(colores){
		case 0:
		(enemigo+i)->color=0;
		break;
		case 1:
		(enemigo+i)->color=1;
		break;
		case 2:
		(enemigo+i)->color=2;
		break;
		case 3:
		(enemigo+i)->color=3;
		break;
	}
	((enemigo+i)->sprite3)= *(spriteEnemy+(enemigo+i)->color+46);
	((enemigo+i)->sprite4)= *(spriteEnemy+(enemigo+i)->color+46);
	(enemigo+i)->inicio=false;
	}
	////////////////////////////////////////
	 if((enemigo+i)->direccion==1){
		if((enemigo+i)->rebote){
		(enemigo+i)->st.x+=(enemigo+i)->speed;
		}else{
		(enemigo+i)->st.x-=(enemigo+i)->speed;
		}
		
	 	if((enemigo+i)->sube==0){
		(enemigo+i)->st.y+=(enemigo+i)->inclinacion;
		}else{
		(enemigo+i)->st.y-=(enemigo+i)->inclinacion;
		}
	 
	 }else{
		 
		if((enemigo+i)->rebote){
		(enemigo+i)->st.x+=(enemigo+i)->speed;
		}else{
		(enemigo+i)->st.x-=(enemigo+i)->speed;
		}
	 	if((enemigo+i)->sube==0){
		(enemigo+i)->st.y+=(enemigo+i)->inclinacion;
		}else{
		(enemigo+i)->st.y-=(enemigo+i)->inclinacion;
		}
	 }
	 colisionesEnemigos(i);
	}else{
		(enemigo+i)->contSpawn++;
	}

	}
}

void enemigo7(){
	//naves2
	for(int i=0;i<nEnemigos;i++){
	/////////////////////////////////////////////
	if(!(enemigo+i)->dead){
	if((enemigo+i)->inicio){
		srand(time(NULL));
	int colores=rand()%4;
	initEnemigo(i);
	(enemigo+i)->tipo=1;
	(enemigo+i)->inclinacion=(rand()%3);
	(enemigo+i)->speed=6;
	switch(colores){
		case 0:
		(enemigo+i)->color=0;
		break;
		case 1:
		(enemigo+i)->color=1;
		break;
		case 2:
		(enemigo+i)->color=2;
		break;
		case 3:
		(enemigo+i)->color=3;
		break;
	}

	if((enemigo+i)->direccion==1){
		
		((enemigo+i)->sprite3)= *(spriteEnemy+(enemigo+i)->color+50);
		((enemigo+i)->sprite4)= *(spriteEnemy+(enemigo+i)->color+50);
	}else{
		((enemigo+i)->sprite3)= *(spriteEnemy+(enemigo+i)->color+54);
		((enemigo+i)->sprite4)= *(spriteEnemy+(enemigo+i)->color+54);
	}
	(enemigo+i)->inicio=false;
	}
	/////////////////////////////////////////////
	 if((enemigo+i)->direccion==1){
		(enemigo+i)->st.x+=(enemigo+i)->speed;
		 if((enemigo+i)->sube==0){

			(enemigo+i)->st.y+=(enemigo+i)->inclinacion;
		 }else{
			(enemigo+i)->st.y-=(enemigo+i)->inclinacion;
		 }
	 }else{
		(enemigo+i)->st.x-=(enemigo+i)->speed;
		if((enemigo+i)->sube==0){
				(enemigo+i)->st.y+=(enemigo+i)->inclinacion;
		}else{

				(enemigo+i)->st.y-=(enemigo+i)->inclinacion;
		}
	 }
	colisionesEnemigos(i);
	}else{

				(enemigo+i)->st.y-=(enemigo+i)->inclinacion;
		}
	}
}
void enemigo8(){

	//bolsas con ojos
		for(int i=0;i<nEnemigos;i++){
	/////////////////////////////////////////////
	if(!(enemigo+i)->dead){
	if((enemigo+i)->inicio){
		srand(time(NULL));
	int colores=rand()%4;
	initEnemigo(i);
	(enemigo+i)->inclinacion=5;
	(enemigo+i)->speed=5;
	(enemigo+i)->tipo=3;
	switch(colores){
		case 0:
		(enemigo+i)->color=0;
		break;
		case 1:
		(enemigo+i)->color=1;
		break;
		case 2:
		(enemigo+i)->color=2;
		break;
		case 3:
		(enemigo+i)->color=3;
		break;
	}
	((enemigo+i)->sprite3)= *(spriteEnemy+(enemigo+i)->color+58);
	((enemigo+i)->sprite4)= *(spriteEnemy+(enemigo+i)->color+58);
	(enemigo+i)->inicio=false;
	}
	/////////////////////////////////////////////
	 if((enemigo+i)->direccion==1){
		 if((enemigo+i)->st.x<(float)esat::MousePositionX()){
		(enemigo+i)->st.x+=(enemigo+i)->speed;
		 }else{
		(enemigo+i)->st.x-=(enemigo+i)->speed;
		 }
			 ////////////CAMBIAR RATON POR POSICION JUGADOR
		if((enemigo+i)->st.y<(float)esat::MousePositionY()){
			(enemigo+i)->st.y+=(enemigo+i)->inclinacion;
		 }else{
			(enemigo+i)->st.y-=(enemigo+i)->inclinacion;
		 }
	 }
	 colisionesEnemigos(i);
	}else{
			(enemigo+i)->contSpawn++;
		}
	}
}
void dibujar(){
	for(int i=0;i<nEnemigos;i++){
	if(!(enemigo+i)->dead){
	if((enemigo+i)->anim<10){
	//printf("st enemigo %d X %f \n",i ,(enemigo+i)->st.x);
	esat::DrawSprite(((enemigo+i)->sprite3),(enemigo+i)->st);
	 }else{
		esat::DrawSprite(((enemigo+i)->sprite4),(enemigo+i)->st);
	 }
	 if((enemigo+i)->anim>20){
		 (enemigo+i)->anim=0;
	 }else{
		 (enemigo+i)->anim++;
	 }
		}else{
			if((enemigo+i)->contMuerte<4){
				esat::DrawSprite(*(spriteEnemy+62),(enemigo+i)->st);
			}else if((enemigo+i)->contMuerte>=4 && (enemigo+i)->contMuerte<7){
				esat::DrawSprite(*(spriteEnemy+63),(enemigo+i)->st);
			}else if((enemigo+i)->contMuerte>=7 && (enemigo+i)->contMuerte<10){
			esat::DrawSprite(*(spriteEnemy+64),(enemigo+i)->st);
			}
			(enemigo+i)->contMuerte++;
			//tiempo que tardan en respawnear
			if((enemigo+i)->contSpawn>70){
			(enemigo+i)->inicio=true;
			(enemigo+i)->dead=false;
			(enemigo+i)->contSpawn=0;
			}
		}
	}
}
void niveles(){
	srand(time(NULL));
	//tiempo que tardan en salir
	if(SpawnEnemy<(rand()%100)+70){
		SpawnEnemy++;
	}else{
		//numero de enemigos que van a salir
		if(nEnemigos<10){
			printf("incremento \n");
			printf("%d \n",nEnemigos);
			if(nEnemigos==0){
				(enemigo+0)->inicio=true;
				(enemigo+0)->dead=false;
			}
			nEnemigos++;
			(enemigo+nEnemigos)->inicio=true;
			(enemigo+nEnemigos)->dead=false;
			printf("incrementado \n");
		}
		printf("reset \n");
		SpawnEnemy=0;
		printf("reseteado \n");
	}
	switch(nivel){
	case 0: 
	enemigo1();
	dibujar();
	break;
	case 1: 
	enemigo2();
	dibujar();
	break;
	case 2: enemigo3();
	dibujar();
	break;
	case 3:
	enemigo4();
	dibujar();
	break;
	case 4:
	enemigo5();
	dibujar();
	break;
	case 5:enemigo6();
	dibujar();
	break;
	case 6:enemigo7();
	dibujar();
	break;
	case 7:enemigo8();
	dibujar();
	break;
	}

}



/*----------------------*/

/*---------Bonus--------*/
void InitBonus(){
	bonus = (Bonus *)malloc(5*sizeof(Bonus));
	
	//verde
	(*(bonus+0)).sprite = (esat::SpriteHandle *)malloc(sizeof(esat::SpriteHandle));
	(*((*(bonus+0)).sprite+0)) = esat::SubSprite(sheet, 70, 46, 16, 9);
	(*(bonus+0)).numS = 1;
	
	//oro
	(*(bonus+1)).sprite = (esat::SpriteHandle *)malloc(sizeof(esat::SpriteHandle));
	(*((*(bonus+1)).sprite+0)) = esat::SubSprite(sheet, 86, 46, 16, 8);
	(*(bonus+1)).numS = 1;
	
	//triforce
	(*(bonus+2)).sprite = (esat::SpriteHandle *)malloc(2*sizeof(esat::SpriteHandle));
	(*((*(bonus+2)).sprite+0)) = esat::SubSprite(sheet, 112, 153, 15, 13);
	(*((*(bonus+2)).sprite+1)) = esat::SubSprite(sheet, 112, 166, 15, 13);
	(*(bonus+2)).numS = 2;
	
	//uranio
	(*(bonus+3)).sprite = (esat::SpriteHandle *)malloc(2*sizeof(esat::SpriteHandle));
	(*((*(bonus+3)).sprite+0)) = esat::SubSprite(sheet, 112, 179, 15, 11);
	(*((*(bonus+3)).sprite+1)) = esat::SubSprite(sheet, 112, 190, 15, 11);
	(*(bonus+3)).numS = 2;
	
	//diamante
	(*(bonus+4)).sprite = (esat::SpriteHandle *)malloc(7*sizeof(esat::SpriteHandle));
	for(int i = 0; i < 7; i++){
		(*((*(bonus+4)).sprite+i)) = esat::SubSprite(sheet, i*14, 253, 14, 12);
	}
	(*(bonus+4)).numS = 7;
	
	stBonus.scale_x = sc;
	stBonus.scale_y = sc;
}

void mostrarBonus(Bonus (*b)){
	esat::DrawSprite((*((*b).sprite+(*b).animacion)), stBonus);
	
	if(stBonus.x == 0){
		r = rand()%5;
		
		switch(r){
			case 0: stBonus.x = 1 + rand()%16; break;
			
			case 1: stBonus.x = 33 + rand()%32; break;
			
			case 2: stBonus.x = 81 + rand()%25; break;
			
			case 3: stBonus.x = 121 + rand()%17; break;
			
			case 4: stBonus.x = 193 + rand()%33; break;
		}
	}
	
	if(animBonus%3 == 0) { (*b).animacion++; (*b).animacion %= (*b).numS; }
	
	animBonus++;
	
	switch(r){
		case 0: if(stBonus.y < (184*sc - esat::SpriteHeight((*((*b).sprite))))){ stBonus.y += 5;} break;
			
		case 1: if(stBonus.y < (72*sc - esat::SpriteHeight((*((*b).sprite))))){ stBonus.y += 5;} break;
		
		case 2: if(stBonus.y < (184*sc - esat::SpriteHeight((*((*b).sprite))))){ stBonus.y += 5;} break;
			
		case 3: if(stBonus.y < (96*sc - esat::SpriteHeight((*((*b).sprite))))){ stBonus.y += 5;} break;
			
		case 4: if(stBonus.y < (48*sc - esat::SpriteHeight((*((*b).sprite))))){ stBonus.y += 5;} break;
	}
}

void SpawnBonus(){
	if(!bonusExist && rand()%1000 < 10) {
		(*(bonus+rand()%5)).spawn = true;
		bonusExist = true;
		stBonus.y = -16;
		stBonus.x = 0;
	}
	if(bonusExist) {
		for(int i = 0; i < 5; i++){
			if((*(bonus+i)).spawn) {
				mostrarBonus((bonus+i));
				//recogerBonus((bonus+i));
			}
		}
	}
}
/*----------------------*/

int esat::main(int argc, char **argv) {
  double current_time,last_time;
  unsigned char fps=25;
 
  esat::WindowInit(wX,wY);
  WindowSetMouseVisibility(true);
  
  enemigo=(object*)malloc(20*sizeof(object));
  InitSprites();
  InitBonus(); //punteros y sprites
  enemySprites();
  InitPlayer();
  while(esat::WindowIsOpened() && !esat::IsSpecialKeyDown(esat::kSpecialKey_Escape)) {
	last_time = esat::Time();
	
    esat::DrawBegin();
    esat::DrawClear(0,0,0);
	
     
      DrawMap();
     ShipAll();
      PlayerAll();
	   niveles();
	//  SpawnBonus();
	
    esat::DrawEnd();
	
	do{
    current_time = esat::Time();
    }while((current_time-last_time)<=1000.0/fps);
    esat::WindowFrame();
  }

  esat::WindowDestroy();
  free(enemigo);
  free(suelo);
  free(player.sprite);
  free(Ship);
  return 0;
}