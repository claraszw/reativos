#include <SDL.h>
#include <stdio.h>

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480
#define BASKET_WIDTH 70
#define BASKET_HEIGHT 30
#define APPLE_HEIGHT 15
#define APPLE_WIDTH 15

SDL_Window* window = NULL;
SDL_Surface* ScreenSurface = NULL;
SDL_Surface* gameover = NULL;
SDL_Renderer* renderer = NULL;


int initialize ()
{
    int success = 1;

	//Initialize SDL
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
		success = 0;
	}
	else
	{
		//Create window
		window = SDL_CreateWindow( "Catch Apples", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
		if( window == NULL )
		{
			printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
			success = 0;
		}
		else
		{
			//Get window surface
			ScreenSurface = SDL_GetWindowSurface( window );
		}
	}

	renderer = SDL_CreateRenderer( window, -1, SDL_RENDERER_ACCELERATED );

    if( renderer == NULL )
    {
        printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
        success = 0;
    }
    else
    {
        //Initialize renderer color
        SDL_SetRenderDrawColor( renderer, 0xFF, 0xFF, 0xFF, 0xFF );
    }

	return success;
}

void close()
{
	//Destroy window
	SDL_DestroyWindow( window );
	window = NULL;

	//Quit SDL subsystems
	SDL_Quit();
}
int loadMedia(char * media)
{
	//Loading success flag
	int success = 1;

	//Load splash image
	gameover = SDL_LoadBMP( media );
	if( gameover == NULL )
	{
		printf( "Unable to load image! SDL Error: %s\n", SDL_GetError() );
		success = 0;
	}

	return success;
}

void move_apple (SDL_Rect * apple, SDL_Rect basket, long * time, long current_time, int * cont_fallen)
{
    if((*apple).y+10>=basket.y && basket.x<(*apple).x && (*apple).x<basket.x+BASKET_WIDTH) //Checa se bateu na basket
            {
                (*apple).y=0;
                (*apple).x=current_time%(SCREEN_WIDTH-APPLE_WIDTH);
            }
            else if((*apple).y+10>=SCREEN_HEIGHT-BASKET_HEIGHT/2)
            {
                (*cont_fallen)++;
                (*apple).y=0;
                (*apple).x=current_time%(SCREEN_WIDTH-APPLE_WIDTH);
            }
            (*apple).y+=10;
            (*time) = SDL_GetTicks();
}
int main (int argc, char *argv[])
{
    int quit =0,start =0, cont_fallen=0, flag_apple2=0,flag_apple3=0;
    SDL_Event e;
    SDL_Rect stretchRect;
    long time = 200;
    long time_velocity = 30000; //conta a cada 30 segundos

    if(!initialize())
    {
        printf("Erro!\n");
    }

      if( !loadMedia("gameinit.bmp") )
    {
        printf( "Failed to load media!\n" );
    }
    else
    {
        //Apply the image stretched
        stretchRect.x = 0;
        stretchRect.y = 0;
        stretchRect.w = SCREEN_WIDTH;
        stretchRect.h = SCREEN_HEIGHT;
        SDL_BlitScaled( gameover, NULL, ScreenSurface, &stretchRect );

        //Update the surface
        SDL_UpdateWindowSurface( window );

        while(!start && !quit)
        {
            while( SDL_PollEvent( &e ) != 0 )
         {
                //User requests quit
            if( e.type == SDL_QUIT )
            {
                quit = 1;
            }

            if( e.type == SDL_MOUSEBUTTONDOWN )
            {
                start = 1;
            }
         }
        }

    }

    SDL_Rect sky = {0,0,SCREEN_WIDTH,SCREEN_HEIGHT-(BASKET_HEIGHT/2)};
    SDL_Rect grass = {0,SCREEN_HEIGHT-(BASKET_HEIGHT/2),SCREEN_WIDTH,BASKET_HEIGHT/2};
    SDL_Rect basket = {SCREEN_WIDTH/2,SCREEN_HEIGHT-BASKET_HEIGHT,BASKET_WIDTH,BASKET_HEIGHT};
    SDL_Rect apple = {SCREEN_WIDTH/2,0,APPLE_WIDTH,APPLE_HEIGHT};
    SDL_Rect apple2 = {SCREEN_WIDTH/2 + 50,0,APPLE_WIDTH,APPLE_HEIGHT};
    SDL_Rect apple3 = {SCREEN_WIDTH/2 - 100,0,APPLE_WIDTH,APPLE_HEIGHT};
    SDL_Rect life1 = {SCREEN_WIDTH-70,20,APPLE_WIDTH,APPLE_HEIGHT};
    SDL_Rect life2 = {SCREEN_WIDTH-50,20,APPLE_WIDTH,APPLE_HEIGHT};
    SDL_Rect life3 = {SCREEN_WIDTH-30,20,APPLE_WIDTH,APPLE_HEIGHT};

    long old_time1 = SDL_GetTicks();
    long old_time2 = SDL_GetTicks() + 2000;
    long old_time3 = SDL_GetTicks() + 4000;

    while(!quit)
    {
       long current_time = SDL_GetTicks();

       if(cont_fallen>=3) //game Over
       {
           if( !loadMedia("gameover.bmp") )
            {
                printf( "Failed to load media!\n" );
            }
            else
            {
               //Apply the image stretched
				stretchRect.x = 0;
				stretchRect.y = 0;
				stretchRect.w = SCREEN_WIDTH;
				stretchRect.h = SCREEN_HEIGHT;
				SDL_BlitScaled( gameover, NULL, ScreenSurface, &stretchRect );

                //Update the surface
                SDL_UpdateWindowSurface( window );

                while(SDL_PollEvent(&e)!=0 || !quit)
                {
                    if( e.type == SDL_QUIT )
                    {
                        quit = 1;
                    }
                    else if( e.type == SDL_KEYDOWN )
                    {
                        if(e.key.keysym.sym == SDLK_SPACE)
                        {
                            cont_fallen=0;
                            flag_apple2=0;
                            flag_apple3=0;
                            time = 200;
                            basket.x=SCREEN_WIDTH/2;
                            apple.x=SCREEN_WIDTH/2;
                            apple2.x=SCREEN_WIDTH/2+50;
                            apple3.x=SCREEN_WIDTH/2-100;
                            apple.y=0;
                            apple2.y=0;
                            apple3.y=0;
                            old_time1 = SDL_GetTicks();
                            old_time2 = SDL_GetTicks() + 2000;
                            old_time3 = SDL_GetTicks() + 4000;
                            break;
                        }
                    }

                }

            }
       }

       if(current_time>time_velocity)//Aumenta a velocidade a cada 30 segundos
       {
           time-=30;
           time_velocity+=30000;
       }


        if(current_time - old_time1>time)
        {//Move apple 1
            move_apple (&apple, basket, &old_time1, current_time, &cont_fallen);
        }

        if(current_time - old_time2>time) //Move apple 2
        {
            if(!flag_apple2)
            {
                flag_apple2=1;
            }

            move_apple (&apple2, basket, &old_time2, current_time, &cont_fallen);
        }

        if(current_time - old_time3>time)
        {//Move apple 3

            if(!flag_apple3)
            {
                flag_apple3=1;
            }

            move_apple (&apple3, basket, &old_time3, current_time, &cont_fallen);
        }

         while( SDL_PollEvent( &e ) != 0 )
         {
                //User requests quit
            if( e.type == SDL_QUIT )
            {
                quit = 1;
            }

            else if(e.type == SDL_KEYDOWN) //Move basket
            {
                switch (e.key.keysym.sym)
                {
                    case SDLK_LEFT:
                        if(basket.x>0)
                            basket.x-=10;
                        break;

                    case SDLK_RIGHT:
                        if(basket.x<SCREEN_WIDTH-BASKET_WIDTH)
                            basket.x+=10;
                        break;
                }
            }
         }

            SDL_SetRenderDrawColor(renderer,0xFF,0xFF,0xFF,0x00); //Desenha tela
            SDL_RenderFillRect(renderer,NULL);
            SDL_SetRenderDrawColor(renderer,0x00,0xAA,0xFF,0xFF);
            SDL_RenderFillRect(renderer,&sky);
            SDL_SetRenderDrawColor(renderer,0x00,0xFF,0x10,0xFF);
            SDL_RenderFillRect(renderer,&grass);
            SDL_SetRenderDrawColor(renderer,0xAA,0x3F,0x00,0xFF);
            SDL_RenderFillRect(renderer,&basket);
            SDL_SetRenderDrawColor(renderer,0xFF,0x00,0x00,0x00);
            if(cont_fallen<3)
                SDL_RenderFillRect(renderer,&life3);
            if(cont_fallen<2)
               SDL_RenderFillRect(renderer,&life2);
            if(cont_fallen<1)
               SDL_RenderFillRect(renderer,&life1);
            SDL_RenderFillRect(renderer,&apple);
            if(flag_apple2)
                SDL_RenderFillRect(renderer,&apple2);
            if(flag_apple3)
                SDL_RenderFillRect(renderer,&apple3);
            SDL_RenderPresent(renderer);

    }

    return 0;

}
