#include <SDL.h>
#include <stdio.h>
#include <SDL_image.h>
#include<iostream>
#include <string>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int WALK_SPEED = 10;
const int GHOST_SPEED = 12;

bool init();
bool loadMedia();
void close();
bool checkCollision(SDL_Rect a, SDL_Rect b);
void dest(SDL_Rect rec);

SDL_Window* gWindow = NULL;
SDL_Surface* gScreenSurface = NULL;
SDL_Surface* gHelloWorld = NULL;
SDL_Surface* pacman = NULL;
SDL_Surface* ghost = NULL;
SDL_Surface*  ghost2 = NULL;
SDL_Surface*  b = NULL;
SDL_Surface*  l = NULL;
SDL_Surface*  e = NULL;
SDL_Surface*  n = NULL;
SDL_Surface*  d = NULL;
SDL_Surface*  i = NULL;
SDL_Surface*  Colon = NULL;
SDL_Surface*  Points = NULL;
SDL_Surface*  DecPoints = NULL;


bool init()
{
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
		success = false;
	}
	else
	{
		//Create window
		gWindow = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (gWindow == NULL)
		{
			printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
			success = false;
		}
		else
		{
			//Get window surface
			gScreenSurface = SDL_GetWindowSurface(gWindow);
		}
	}

	return success;
}

bool loadMedia(std::string filename, SDL_Surface ** destination)
{
	//Loading success flag
	bool success = true;

	//Load splash image
	*destination = IMG_Load(filename.c_str());
	if (*destination == NULL)
	{
		printf("Unable to load image %s! SDL Error: %s\n", filename.c_str(), SDL_GetError());
		success = false;
	}

	return success;
}

void close()
{
	//Deallocate surface
	SDL_FreeSurface(gHelloWorld);
	gHelloWorld = NULL;

	//Destroy window
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;

	//Quit SDL subsystems
	SDL_Quit();
}

void handleKeys(SDL_Event* event, int *dir, int *vel){
	if (event->type == SDL_KEYDOWN){
		switch (event->key.keysym.sym){
		case SDLK_a:
			*dir = 9;
			*vel = WALK_SPEED;
			break;
		case SDLK_w:
			*dir = 8;
			*vel = WALK_SPEED;
			break;
		case SDLK_s:
			*dir = 6;
			*vel = WALK_SPEED;
			break;
		case SDLK_d:
			*dir = 7;
			*vel = WALK_SPEED;
			break;
		}
		
	}
	else {
		if (event->type == SDL_KEYUP){
			*vel = 0;
		}
	}
}

//http://lazyfoo.net/SDL_tutorials/lesson17/index.php

bool checkCollision(SDL_Rect a, SDL_Rect b)
{
	int leftA, leftB;
	int rightA, rightB;
	int topA, topB;
	int bottomA, bottomB;

	leftA = a.x;
	rightA = a.x + a.w;
	topA = a.y;
	bottomA = a.y + a.h;

	leftB = b.x;
	rightB = b.x + b.w;
	topB = b.y;
	bottomB = b.y + b.h;


	if (bottomA <= topB)
	{
		return false;
	}
	if (topA >= bottomB)
	{
		return false;
	}
	if (rightA <= leftB)
	{
		return false;
	}
	if (leftA >= rightB)
	{
		return false;
	}
	return true;

}

void dest(SDL_Rect rec)
{
	rec.y = 0;
	rec.h = 25;
	rec.w = 21;
}


int main(int argc, char* args[])
{
	std::string pacmanFilename = "pacman.png";
	std::string blendiFilename = "Arial.bmp";

	int spacemanMovement = 0;
	int spacemanVelocity = 0;

	//Start up SDL and create window
	if (!init())
	{
		printf("Failed to initialize!\n");
	}
	else
	{
		//Load media
		if (!loadMedia("DefaultBackground.jpg", &gHelloWorld))
		{
			printf("Failed to load media!\n");
		}
		else{

			loadMedia(pacmanFilename, &pacman);
			loadMedia(pacmanFilename, &ghost);
			loadMedia(pacmanFilename, &ghost2);
			loadMedia(blendiFilename, &b);
			loadMedia(blendiFilename, &l);
			loadMedia(blendiFilename, &e);
			loadMedia(blendiFilename, &n);
			loadMedia(blendiFilename, &d);
			loadMedia(blendiFilename, &i);
			loadMedia(blendiFilename, &Colon);
			loadMedia(blendiFilename, &Points);
			loadMedia(blendiFilename, &DecPoints);

			//Apply the image
			SDL_BlitSurface(gHelloWorld, NULL, gScreenSurface, NULL);
			//Update the surface
			SDL_UpdateWindowSurface(gWindow);
		}



		SDL_Event event;
		SDL_Rect sourceRect, gsourceRect, g2sourceRect, sB, sL, sE, sN, sD, sI, sourceColon, sourcePoint, sourceDec;
		SDL_Rect destRect, gdestRect, g2destRect, dB, dL, dE, dN, dD, dI, destColon, destPoint, destDec;

		int quit = 0;
		int rnd = 1;
		int count = 1;
		int countPoints = 0;

		//pacman position
		destRect.x = 30;
		destRect.y = 30;
		destRect.h = 70;
		destRect.w = 70;

		sourceRect.x = 10;
		sourceRect.y = 10;
		sourceRect.h = 70;
		sourceRect.w = 70;

		// ghost 1 position
		gdestRect.x = 100;
		gdestRect.y = 100;
		gdestRect.h = 70;
		gdestRect.w = 70;

		gsourceRect.x = 300;
		gsourceRect.y = 100;
		gsourceRect.h = 70;
		gsourceRect.w = 70;

		//ghost 2 position
		g2destRect.x = 150;
		g2destRect.y = 200;
		g2destRect.h = 70;
		g2destRect.w = 70;

		g2sourceRect.x = 300;
		g2sourceRect.y = 100;
		g2sourceRect.h = 70;
		g2sourceRect.w = 70;

		//Position for "Blendi:d s n" text from Arial.bmp where "d s" are points
		dB.x = 0;
		dest(dB);

		sB.x = 273;
		sB.y = 25;
		sB.h = 25;
		sB.w = 21;

		dL.x = 21;
		dest(dL);

		sL.x = 315;
		sL.y = 75;
		sL.h = 25;
		sL.w = 21;

		dE.x = (42);
		dest(dE);

		sE.x = 168;
		sE.y = 75;
		sE.h = 25;
		sE.w = 21;

		dN.x = (63);
		dest(dN);

		sN.x = 357;
		sN.y = 75;
		sN.h = 25;
		sN.w = 21;

		dD.x = (84);
		dest(dD);

		sD.x = 147;
		sD.y = 75;
		sD.h = 25;
		sD.w = 21;

		dI.x = (105);
		dest(dI);

		sI.x = 252;
		sI.y = 75;
		sI.h = 25;
		sI.w = 21;

		destColon.x = 126;
		dest(destColon);

		sourceColon.x = 105;
		sourceColon.y = 25;
		sourceColon.h = 25;
		sourceColon.w = 21;

		destPoint.x = 168;
		dest(destPoint);

		sourcePoint.x = 315;
		sourcePoint.y = 0;
		sourcePoint.h = 25;
		sourcePoint.w = 21;

		destDec.x = 147;
		dest(destDec);

		sourceDec.x = 315;
		sourceDec.y = 0;
		sourceDec.h = 25;
		sourceDec.w = 21;


		while (!quit)
		{
			//whenever count reaches 20 give me a random number from 1 to 4
			count++;
			if (count >= 20)
			{
				rnd = rand() % 4 + 1;
				count = 0;
			}
			SDL_BlitSurface(gHelloWorld, NULL, gScreenSurface, NULL);

			while (SDL_PollEvent(&event))
			{
				if (event.type == SDL_QUIT){
					quit = 1;
				}
				if (event.type == SDL_KEYDOWN || event.type == SDL_KEYUP){
					handleKeys(&event, &spacemanMovement, &spacemanVelocity);
				}
			}
			// this should only be done when moving.
			sourceRect.x += 70;//move pacman
			gsourceRect.x += 70;//move ghost 1
			g2sourceRect.x += 70;//move ghost 2

			//if it is the last icon back to first
			if (sourceRect.x >= 240)
			{
				sourceRect.x = 0;
			}
			if (gsourceRect.x >= 430)
			{
				gsourceRect.x = 300;
			}
			if (g2sourceRect.x >= 430)
			{
				g2sourceRect.x = 300;
			}


			if (spacemanVelocity > 0)
			{
				switch (spacemanMovement){
				case 7://go right(d)
					if (destRect.x <= 560)//to prevent pacman to go out from right side of screen
					{
						sourceRect.y = 220;
						destRect.x += spacemanVelocity;
					}
					else
						destRect.x = 560;
					break;
				case 8://go up(w)
					sourceRect.y = 80;
					destRect.y -= spacemanVelocity;

					break;
				case 6://go down(s)
					if (destRect.y <= 410)//to prevent pacman to go out from bottom side of screen
					{
						sourceRect.y = 10;
						destRect.y += spacemanVelocity;
					}
					else
						destRect.y = 410;
					break;
				case 9://go left(a)
					sourceRect.y = 150;
					destRect.x -= spacemanVelocity;
				}
			}

			//ghost 1
			switch (rnd)//move first ghost depending from random number
			{
			case 1://go down 
				if (gdestRect.y <= 410)//to prevent ghost to go out from right side of screen
				{
					gsourceRect.y = 10;
					gdestRect.y += GHOST_SPEED;
				}
				else gdestRect.y = 410;
				break;
			case 2://go up
				gsourceRect.y = 80;
				gdestRect.y -= GHOST_SPEED; break;
			case 3://go left
				gsourceRect.y = 150;
				gdestRect.x -= GHOST_SPEED; break;
			case 4://go right
				if (gdestRect.x <= 560)//to prevent ghost to go out from bottom side of screen
				{
					gsourceRect.y = 220;
					gdestRect.x += GHOST_SPEED;
				}
				else  gdestRect.x = 560;
				break;

			}

			//ghost 2
			switch (rnd)
			{
			case 1://go up
				g2sourceRect.y = 80;
				g2destRect.y -= GHOST_SPEED; break;
			case 2://go down
				if (g2destRect.x <= 560)//to prevent pacman to go out from bottom side of screen
				{
					g2sourceRect.y = 220;
					g2destRect.x += GHOST_SPEED;
				}
				else  g2destRect.x = 560;
				break;
			case 3://go right
				if (g2destRect.y <= 410)//to prevent pacman to go out from bottom side of screen
				{
					g2sourceRect.y = 10;
					g2destRect.y += GHOST_SPEED;
				}
				else g2destRect.y = 410;
				break;
			case 4://go left
				g2sourceRect.y = 150;
				g2destRect.x -= GHOST_SPEED; break;

			}

			//check Collosion between pacman and first ghost
			if (checkCollision(destRect, gdestRect))
			{
				countPoints++;//count collisions
				sourcePoint.x += 21;//get numbers from Arial.bmp
				switch (countPoints)
				{
				case 5: sourcePoint.x = 0;
					sourcePoint.y = 25; break;
				case 10: sourcePoint.x = 315;
					sourcePoint.y = 0;
					countPoints = 0;
					sourceDec.x += 21;
					if (sourceDec.x == 420)
					{
						sourceDec.x = 315;
					}
					break;
				}
				//spawn ghost 1 into a new random location on screen
				gdestRect.x = rand() % 400;
				gdestRect.y = rand() % 400;;

			}
			if (checkCollision(destRect, g2destRect))
			{

				countPoints++;
				sourcePoint.x += 21;
				switch (countPoints)
				{
				case 5: sourcePoint.x = 0;
					sourcePoint.y = 25; break;
				case 10: sourcePoint.x = 315;
					sourcePoint.y = 0;
					countPoints = 0;
					sourceDec.x += 21;
					if (sourceDec.x == 420)
					{
						sourceDec.x = 315;
					}break;
				}

				//spawn ghost 2 into a new random location on screen
				g2destRect.x = rand() % 400;
				g2destRect.y = rand() % 400;;
			}

	
			SDL_BlitSurface(pacman, &sourceRect, gScreenSurface, &destRect);
			SDL_BlitSurface(ghost, &gsourceRect, gScreenSurface, &gdestRect);
			SDL_BlitSurface(ghost2, &g2sourceRect, gScreenSurface, &g2destRect);
			SDL_BlitSurface(b, &sB, gScreenSurface, &dB);
			SDL_BlitSurface(l, &sL, gScreenSurface, &dL);
			SDL_BlitSurface(e, &sE, gScreenSurface, &dE);
			SDL_BlitSurface(n, &sN, gScreenSurface, &dN);
			SDL_BlitSurface(d, &sD, gScreenSurface, &dD);
			SDL_BlitSurface(i, &sI, gScreenSurface, &dI);
			SDL_BlitSurface(Colon, &sourceColon, gScreenSurface, &destColon);
			SDL_BlitSurface(Points, &sourcePoint, gScreenSurface, &destPoint);
			SDL_BlitSurface(DecPoints, &sourceDec, gScreenSurface, &destDec);
			SDL_Delay(50);
			SDL_UpdateWindowSurface(gWindow);

		}

	}

	//Free resources and close SDL
	close();
	
	return 0;

	}
	