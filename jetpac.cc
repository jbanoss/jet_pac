#include <esat/window.h>
#include <esat/draw.h>
#include <esat/sprite.h>
#include <esat/input.h>
#include <esat/time.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

const int sc=3, wX=256*sc, wY=192*sc,nivel=1,nEnemigos=1;
int an=0;

esat::SpriteHandle sheet, *suelo;

esat::SpriteTransform stMap;

struct object{
	int speed,direccion,lado,inclinacion,sube,danza,anim,color;
	float x,y,aux;
	bool shoot, fly, inFloor; 
	bool inicio=true,rebote;
	esat::SpriteHandle *sprite;
	esat::SpriteHandle *sprite2;
	esat::SpriteTransform st;
};
object player;
object *enemigo;
void InitSprites(){
	sheet = esat::SpriteFromFile("./recursos/imagenes/sheet.png");
	
	suelo = (esat::SpriteHandle *)malloc(6*sizeof(esat::SpriteHandle));
	stMap.scale_x = sc;
	stMap.scale_y = sc;
	
	for(int i = 0; i < 6; i++){
		*(suelo+i) = esat::SubSprite(sheet, i*8, 46, 8, 8);
	}
}

/*--------Map-----------Josu*/

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

bool col1(){
	return player.st.y+esat::SpriteHeight(*(player.sprite+8))*sc >(72)*sc && player.st.y < 80*sc && player.st.x+esat::SpriteWidth(*(player.sprite+8))*sc>(32)*sc && player.st.x<(32+42)*sc;
}

void PlayerControls(){
		if(esat::IsSpecialKeyDown(esat::kSpecialKey_Space)){
			//player.shoot=true;
			//Disparar();
		}
    //flying
		if(esat::IsSpecialKeyPressed(esat::kSpecialKey_Up) && !col1()){
			player.st.y-=player.speed;
      player.fly=true;
      ++an%=4;
		}else{
			if(!col1()) player.st.y+=3;
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

void PlayerCol(){
   //platizq
  if(esat::IsSpecialKeyPressed(esat::kSpecialKey_Left) &&
  player.st.x<(32+42)*sc &&
  player.st.y<(72+8)*sc && player.st.y+esat::SpriteHeight(*(player.sprite+8))*sc>(72)*sc){
    player.st.x=(32+42)*sc;
  }
  if(esat::IsSpecialKeyPressed(esat::kSpecialKey_Right) &&
  player.st.x+esat::SpriteWidth(*(player.sprite+8))*sc>(32)*sc &&
  player.st.y<(72+8)*sc && player.st.y+esat::SpriteHeight(*(player.sprite+8))*sc>(72)*sc){
    player.st.x=(32)*sc-esat::SpriteWidth(*(player.sprite+8))*sc;
  }
  /*
  if(player.st.y<(72+8)*sc){
    player.st.y=(72+8)*sc;
  }
  if(player.st.y+esat::SpriteHeight(*(player.sprite+8))*sc>(72)*sc){
    player.st.y=(72)*sc-esat::SpriteHeight(*(player.sprite+8))*sc;
  }*/

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
//inicializacion de los sprites de los enemigos llendo del 0 al 62 *alberto
void enemySprites(){
	(enemigo)->sprite = (esat::SpriteHandle *)malloc(62*sizeof(esat::SpriteHandle));
	(enemigo)->sprite2 = (esat::SpriteHandle *)malloc(2*sizeof(esat::SpriteHandle));
	//meteoritos (8-15 al reves)
for(int i=0;i<8;i++){
	for(int j=0;j<2;j++){
	*(enemigo->sprite+((2*i)+j))=esat::SubSprite(sheet,(j*16)+0,(i*11)+137,16,11);
	
	}
}
	//pompones (sprite 16 hasta 24)
for(int i=0;i<4;i++){
	for(int j=0;j<2;j++){
	*(enemigo->sprite+((2*i)+j+16))=esat::SubSprite(sheet,(j*16)+32,(i*14)+137,16,14);
	}
}
	//burbujas (sprite 24 hasta 32)
for(int i=0;i<4;i++){
	for(int j=0;j<2;j++){
	*(enemigo->sprite+((2*i)+j+24))=esat::SubSprite(sheet,(j*16)+64,(i*16)+137,16,16);
	}
}
//naves1 (sprite 32 hasta 42,del 36 al 41 al reves)
for(int i=0;i<10;i++){
	*(enemigo->sprite+(i+32))=esat::SubSprite(sheet,96,(i*7)+137,16,7);
}
//ovnis (sprite 42 hasta 46)
for(int i=0;i<4;i++){
	*(enemigo->sprite+(i+42))=esat::SubSprite(sheet,112,(i*8)+121,16,8);
}
//cruces (sprite 46 hasta 50)
for(int i=0;i<4;i++){
	*(enemigo->sprite+(i+46))=esat::SubSprite(sheet,128,(i*15)+121,16,15);
}
//naves2 (sprite 50 hasta 58 del 54 al 57 al reves)
for(int i=0;i<2;i++){
	for(int j=0;j<4;j++){
	*(enemigo->sprite+(i*4)+j+50)=esat::SubSprite(sheet,(j*16)+0,(i*14)+225,16,14);
	}
}
//bolsas (sprite 58 hasta 62)
for(int i=0;i<4;i++){
	*(enemigo->sprite+(i+58))=esat::SubSprite(sheet,(i*16)+32,201,16,14);
}
//printf("entrada enemigos 6 \n");
}

//inicializa los enemigos *alberto
void initEnemigo(int actual){
	srand(time(NULL));
		(enemigo+actual)->lado=rand()%2;//en que lado aparece *alberto
		(enemigo+actual)->sube=rand()%2;
		(enemigo+actual)->st.scale_x=sc;
		(enemigo+actual)->st.scale_y=sc;
		(enemigo+actual)->anim=0;
		(enemigo+actual)->st.angle=0.0f;
		if((enemigo+actual)->lado==1){
			(enemigo+actual)->rebote=false;
		//rebote es un boleano que indica cuando choca en el caso de las naves1 se usa para la danza inicial *alberto
			(enemigo+actual)->st.x=wX-20;
			(enemigo+actual)->st.y=rand()%wY-200;
			if((enemigo+actual)->st.y<200){
				(enemigo+actual)->st.y=200;
			}
			(enemigo+actual)->direccion=0;
			//en la direccion a la que se dirige 0=izq 1=der *alberto
		}else{
			(enemigo+actual)->rebote=true;
			(enemigo+actual)->st.y=rand()%wY-200;
			if((enemigo+actual)->st.y<200){
				(enemigo+actual)->st.y=200;
			}			
			(enemigo+actual)->st.x=20;
			(enemigo+actual)->direccion=1;
		}
}

void enemigo1(){
	for(int i=0;i<nEnemigos;i++){
	//el for sirve para recorrer el puntero con todos los enemigos *alberto
	//no funciona bien *alberto
	/////////////////////////////////////////////

	if((enemigo+i)->inicio){
	int colores=rand()%4;
	//colores se usa en el switch mas abajo para poder coger un color aleatorio *alberto
	initEnemigo(i);
	(enemigo+i)->inclinacion=(rand()%3);//velocidad en diagonal *alberto
	(enemigo+i)->speed=6;
	//printf("%d \n",colores);
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
	//printf("%d \n",(enemigo+i)->color);
	if((enemigo+i)->direccion==1){
		//asignacion de los sprites de los colores correspondientes *alberto
		//+0 y +1 son para las animaciones de algunos
		*((enemigo+i)->sprite2+0)= *(enemigo->sprite+(enemigo+i)->color);
		*((enemigo+i)->sprite2+1)= *(enemigo->sprite+(enemigo+i)->color+1);
	}else{
	/*	printf("%d \n",(enemigo+i)->color+8);
		printf("%d \n",(enemigo+i)->color+9);*/
		*((enemigo+i)->sprite2+0)= *(enemigo->sprite+(enemigo+i)->color+8);
		*((enemigo+i)->sprite2+1)= *(enemigo->sprite+(enemigo+i)->color+9);
	}
	(enemigo+i)->inicio=false;
	}
	/////////////////////////////////////////////
	 if((enemigo+i)->direccion==1){
		(enemigo+i)->st.x+=(enemigo+i)->speed;
		 if((enemigo+i)->sube==0){
			 //sube indica si sube o si baja en este caso solo se hace *alberto
			 //al inicio pero en los que rebotan depende de la direccion *alberto
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
	 //temporizador de animacion *alberto
	 if((enemigo+i)->anim<10){
		// printf("error3");
	esat::DrawSprite(*((enemigo+i)->sprite2+0),(enemigo+i)->st);
	 }else{
		// printf("error4");
		esat::DrawSprite(*((enemigo+i)->sprite2+1),(enemigo+i)->st);
	 }
	 if((enemigo+i)->anim>20){
		 (enemigo+i)->anim=0;
	 }else{
		 (enemigo+i)->anim++;
	 }
	 //colisiones con techo y suelo *alberto
	if((enemigo+i)->st.x>wX){
		(enemigo+i)->st.x=0;
	}else if((enemigo+i)->st.x<0){
		(enemigo+i)->st.x=wX;
	}
	if((enemigo+i)->st.y>wY){
		(enemigo+i)->inicio=true;
	}else if((enemigo+i)->st.y<0){
		(enemigo+i)->inicio=true;
	}
	}
}

void enemigo2(){
	//rebote cambia con las colisiones y controla si choca *alberto
	//de frente para que vuelva hacia atras *alberto
	////////////////////////////////////////////
	for(int i=0;i<nEnemigos;i++){
	if((enemigo+i)->inicio){
	int colores=rand()%4;
	//printf("%d \n",colores);
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
	//printf("%d \n",(enemigo+i)->color);
		*((enemigo+i)->sprite2+0)= *(enemigo->sprite+(enemigo+i)->color+16);
		*((enemigo+i)->sprite2+1)= *(enemigo->sprite+(enemigo+i)->color+17);
	//	printf("%d \n",(enemigo+i)->color+16);
	//	printf("%d \n",(enemigo+i)->color+17);
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
	 //temporizador animacion *alberto
	 if((enemigo+i)->anim<10){
	esat::DrawSprite(*((enemigo+i)->sprite2+0),(enemigo+i)->st);
	 }else{
		esat::DrawSprite(*((enemigo+i)->sprite2+1),(enemigo+i)->st);
	 }
	 if((enemigo+i)->anim>20){
		 (enemigo+i)->anim=0;
	 }else{
		 (enemigo+i)->anim++;
	 }
	  //colisiones con techo y suelo *alberto
		if((enemigo+i)->st.x>wX){
			(enemigo+i)->st.x=0;
		}else if((enemigo+i)->st.x<0){
			(enemigo+i)->st.x=wX;
		}
		//al rebotar por arriba sube es igual a 1 lo que quiere decir que baja *alberto
		if((enemigo+i)->st.y>wY){
			(enemigo+i)->direccion=0;
			(enemigo+i)->sube=1;
		}else if((enemigo+i)->st.y<0){
			(enemigo+i)->sube=0;
			(enemigo+i)->direccion=1;
		}
	}
}

void enemigo3(){
	
}
void enemigo4(){
	/////////////////////////////////////////////
	for(int i=0;i<nEnemigos;i++){
	if((enemigo+i)->inicio){
	int colores=rand()%5;
	initEnemigo(i);
	(enemigo+i)->aux=(enemigo+i)->st.y;
	(enemigo+i)->danza=0;
	//danza es el pequeño baile que hacen al principio nada mas salir es un temporizador *alberto
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
		*((enemigo+i)->sprite2+0)= *(enemigo->sprite+(enemigo+i)->color+32);
	}else{
		*((enemigo+i)->sprite2+0)= *(enemigo->sprite+(enemigo+i)->color+37);
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
			 ////////////CAMBIAR RATON POR POSICION JUGADOR *alberto
			 if((enemigo+i)->st.y<(float)esat::MousePositionY()){
				(enemigo+i)->st.y+=(enemigo+i)->inclinacion;
			 }else{
				(enemigo+i)->st.y-=(enemigo+i)->inclinacion;
			 }
		}
	 }else{
		 //temporizador de la danza *alberto
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
		if((enemigo+i)->st.y<(float)esat::MousePositionY()){
			(enemigo+i)->st.y+=(enemigo+i)->inclinacion;
		}else{
			(enemigo+i)->st.y-=(enemigo+i)->inclinacion;
		}
		}
	 }
	esat::DrawSprite(*((enemigo+i)->sprite2+0),(enemigo+i)->st);

	 (enemigo+i)->danza++;
	 //colisiones *alberto
		if((enemigo+i)->st.x>wX){
			(enemigo+i)->st.x=0;
		}else if((enemigo+i)->st.x<0){
			(enemigo+i)->st.x=wX;
		}
		if((enemigo+i)->st.y>wY){
			(enemigo+i)->inicio=true;
		}else if((enemigo+i)->st.y<0){
			(enemigo+i)->inicio=true;
		}
	}
}
void enemigo5(){
	
}
void niveles(){
	//dependiendo de la variable global nivel ejecuta un tipo de enemigos *alberto
	switch(nivel){
	case 0: enemigo1();
	break;
	case 1: enemigo2();
	break;
	case 2: enemigo3();
	break;
	case 3:enemigo4();
	break;
	case 4:enemigo5();
	}
}

/*----------------------*/


int esat::main(int argc, char **argv) {
 
  double current_time,last_time;
  unsigned char fps=25;
 
  esat::WindowInit(wX,wY);
  WindowSetMouseVisibility(true);
 enemigo=(object*)malloc(3*sizeof(object));
  InitSprites();
  enemySprites();
  InitPlayer();
  //por algun motivo no coje inicio como true en el struct *alberto
  for(int i=0;i<nEnemigos;i++){
		(enemigo+i)->inicio=true;
	}
  while(esat::WindowIsOpened() && !esat::IsSpecialKeyDown(esat::kSpecialKey_Escape)) {
	last_time = esat::Time();
	
	esat::DrawBegin();
    esat::DrawClear(0,0,0);
	niveles();
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