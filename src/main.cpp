#ifdef _WIN32
	#include "../libs/MPI/Include/mpi.h"
    #include <SDL.h>
#elif __linux__
	#include <mpi.h>
    #include <SDL2/SDL.h>
#endif
#include <iostream>
#include <random>

#include "mpi_data_struct.h"

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

    int finished_workers = 0;

    init_mpi_data_structs();

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

            if(finished_workers < num_tasks-1) {
                PixelData pixel;
                MPI_Recv(&pixel, 1, mpi_pixelData_type, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);
                setPixel(renderer, pixel.x, pixel.y, pixel.r, pixel.g, pixel.b);
                SDL_RenderPresent(renderer);
                finished_workers++;
            }
        }
    }
    else {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> distribX(0, screenWidth);
        std::uniform_int_distribution<> distribY(0, screenHeight);
        std::uniform_int_distribution<> distribRGB(0, 255);
        PixelData pixel;
        pixel.x = distribX(gen);
        pixel.y = distribY(gen);
        pixel.r = distribRGB(gen); pixel.g = distribRGB(gen); pixel.b = distribRGB(gen);
        MPI_Send(&pixel, 1, mpi_pixelData_type, 0, 0, MPI_COMM_WORLD);
    }

    if(rank == 0) {
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
    }

    MPI_Finalize();
    return 0;
}