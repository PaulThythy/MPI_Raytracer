#ifdef _WIN32
	#include "../libs/MPI/Include/mpi.h"
    #include <SDL.h>
#elif __linux__
	#include <mpi.h>
    #include <SDL2/SDL.h>
#endif
#include <iostream>
#include <random>

struct PixelData {
    int x, y;
    int r, g, b;
};

void setPixel(SDL_Renderer* renderer, int x, int y, int r, int g, int b) {
    SDL_SetRenderDrawColor(renderer, r, g, b, 1);
    SDL_RenderDrawPoint(renderer, x, y);
}

int main(int argc, char *argv[]) {
    int num_tasks, rank, len;

    char hostname[MPI_MAX_PROCESSOR_NAME];

    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    MPI_Comm_size(MPI_COMM_WORLD, &num_tasks);

    MPI_Get_processor_name(hostname, &len);

    MPI_Status status;

    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;

    const int screenWidth = 800;
    const int screenHeight = 600;

    /* create a serializable struct for MPI */
    const int nitems = 5;
    int blocklengths[5] = {1,1,1,1,1};
    MPI_Datatype types[5] = {MPI_INT, MPI_INT, MPI_INT, MPI_INT, MPI_INT};
    MPI_Datatype mpi_pixelData_type;
    MPI_Aint offsets[5];
    offsets[0] = offsetof(PixelData, x);
    offsets[1] = offsetof(PixelData, y);
    offsets[2] = offsetof(PixelData, r);
    offsets[3] = offsetof(PixelData, g);
    offsets[4] = offsetof(PixelData, b);
    MPI_Type_create_struct(nitems, blocklengths, offsets, types, &mpi_pixelData_type);
    MPI_Type_commit(&mpi_pixelData_type);


    if(rank == 0) {
        if (SDL_Init(SDL_INIT_VIDEO) != 0) {
            std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
            return 1;
        }

        window = SDL_CreateWindow("test", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screenWidth, screenHeight, SDL_WINDOW_SHOWN);
        if (window == nullptr) {
            std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
            SDL_Quit();
            return 1;
        }
        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
        if (renderer == nullptr) {
            SDL_DestroyWindow(window);
            std::cerr << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
            SDL_Quit();
            return 1;
        }

        bool running = true;
        SDL_Event e;
        while(running) {
            while (SDL_PollEvent(&e) != 0) {
                if (e.type == SDL_QUIT) {
                    running = false;
                }
            }

            SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
            SDL_RenderClear(renderer);
            SDL_RenderPresent(renderer);
            
            PixelData pixel;
            MPI_Recv(&pixel, 1, mpi_pixelData_type, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);
            setPixel(renderer, pixel.x, pixel.y, pixel.r, pixel.g, pixel.b);
        }
    }
    else {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> distribX(0, screenWidth);
        std::uniform_int_distribution<> distribY(0, screenHeight);
        PixelData pixel;
        pixel.x = distribX(gen);
        pixel.y = distribY(gen);
        pixel.r = 255; pixel.g = 0; pixel.b = 0;
        MPI_Send(&pixel, 1, mpi_pixelData_type, 0, 0, MPI_COMM_WORLD);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    MPI_Finalize();
    return 0;
}