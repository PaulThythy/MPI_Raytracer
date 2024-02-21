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

    const int screenWidth = 800;
    const int screenHeight = 600;

    int num_tasks, rank, len;

    char hostname[MPI_MAX_PROCESSOR_NAME];

    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    MPI_Comm_size(MPI_COMM_WORLD, &num_tasks);

    MPI_Get_processor_name(hostname, &len);

    if(num_tasks > screenHeight * screenWidth || num_tasks < 2) {
        std::cout << "Program needs at least 2 processes and less than the image resolution" << std::endl;
        MPI_Finalize();
        return 1;
    }

    MPI_Status status;
    MPI_Request request;

    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;

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

        PixelData pixel_received;
        MPI_Irecv(&pixel_received, 1, mpi_pixelData_type, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &request);

        bool running = true;
        SDL_Event e;
        while(running) {
            while (SDL_PollEvent(&e) != 0) {
                if (e.type == SDL_QUIT) {
                    running = false;
                }
            }

            if(finished_workers < num_tasks-1) {
                
                
                int flag = 0;
                MPI_Test(&request, &flag, &status);
                if(flag) {
                    setPixel(renderer, pixel_received.x, pixel_received.y, pixel_received.r, pixel_received.g, pixel_received.b);
                    SDL_RenderPresent(renderer);
                    finished_workers++;
                    MPI_Irecv(&pixel_received, 1, mpi_pixelData_type, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &request);
                }
            }
        }
    }
    else {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> distribX(0, screenWidth);
        std::uniform_int_distribution<> distribY(0, screenHeight);
        std::uniform_int_distribution<> distribRGB(0, 255);
        PixelData pixel_to_send;
        pixel_to_send.x = distribX(gen);
        pixel_to_send.y = distribY(gen);
        pixel_to_send.r = distribRGB(gen); pixel_to_send.g = distribRGB(gen); pixel_to_send.b = distribRGB(gen);
        MPI_Isend(&pixel_to_send, 1, mpi_pixelData_type, 0, 0, MPI_COMM_WORLD, &request);
        MPI_Wait(&request, &status);
    }

    if(rank == 0) {
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
    }

    MPI_Finalize();
    return 0;
}