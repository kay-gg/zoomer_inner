#define SDL_MAIN_USE_CALLBACKS 1  /* use the callbacks instead of main() */

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#include <stdlib.h>

/* We will use this renderer to draw into this window every frame. */
static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;

struct ClickPos *ClickStart = NULL;
struct ClickPos *ClickEnd = NULL;
struct SDL_FRect *rectangle = NULL;

struct ClickPos {
	float x;
	float y;
};

SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[])
{
    SDL_SetAppMetadata("Example Renderer Clear", "1.0", "com.example.renderer-clear");

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }
    if (!SDL_CreateWindowAndRenderer("proabbly shouldnt name it that lol", 2560, 1440, SDL_WINDOW_TRANSPARENT | SDL_WINDOW_BORDERLESS , &window, &renderer)) {
        SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    return SDL_APP_CONTINUE;  /* carry on with the program! */
}
void Create_Rectangle() {
	struct SDL_FRect *r = (struct SDL_FRect *)malloc(sizeof(struct SDL_FRect));
	r->x = ClickStart->x;
	r->y = ClickStart->y;
	r->w = SDL_abs(ClickStart->x - ClickEnd->x);
	r->h = SDL_abs(ClickStart->y - ClickEnd->y);

	SDL_Log("rect: %f,%f,\n%f,%f", r->x, r->y, r->w, r->h);
	rectangle = r;
	//free(r); // idk weird error when setting rectangle directly
}

/* This function runs when a new event (mouse input, keypresses, etc) occurs. */
SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event)
{
    if (event->type == SDL_EVENT_QUIT) {
        return SDL_APP_SUCCESS;  /* end the program, reporting success to the OS. */
    } else if (event->type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
		SDL_Log("Mouse down at: (%f, %f)", event->motion.x, event->motion.y);

		ClickStart = (struct ClickPos *)malloc(sizeof(struct ClickPos));
		ClickStart->x = event->motion.x;
		ClickStart->y = event->motion.y;
	} else if (event->type == SDL_EVENT_MOUSE_BUTTON_UP) {
		SDL_Log("Mouse up at: (%f, %f)", event->motion.x, event->motion.y);

		ClickEnd = (struct ClickPos *)malloc(sizeof(struct ClickPos));
		ClickEnd->x = event->motion.x;
		ClickEnd->y = event->motion.y;
		
		Create_Rectangle();
	} 
    return SDL_APP_CONTINUE;  /* carry on with the program! */
}



SDL_AppResult SDL_AppIterate(void *appstate)
{
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0); // Fully transparent
	SDL_RenderClear(renderer);

	if (rectangle != NULL)
	{
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		SDL_RenderRect(renderer, rectangle);
	}
	
	// Present the renderer
	SDL_RenderPresent(renderer);
    return SDL_APP_CONTINUE;  /* carry on with the program! */
}

void SDL_AppQuit(void *appstate, SDL_AppResult result)
{
    /* SDL will clean up the window/renderer for us. */
}
