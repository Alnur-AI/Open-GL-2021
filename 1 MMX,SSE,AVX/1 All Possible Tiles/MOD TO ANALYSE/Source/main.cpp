#include <iostream>

#include <fstream>
#include <sstream>

#include <Clock.h>
#include <ImageManager.h>
#include <SceneLoader.h>
#include "Scene.h"
#include "GP_ONE_0.h"
#include "GP_ONE_1.h"
#include "GP_ONE_2.h"
#include "GP_ONE_3.h"



const int TEST_ITERATIONS = 100; 
const int COMMAND_SIZE = 100;
const int GPUselect = 0;

bool compareFrameBuffers(FrameBuffer &fBuf1, FrameBuffer &fBuf2) {
    for (uint16_t i = 0; i < FRAMEBUFFER_BUF_SIZE; ++i) {
        if (fBuf1.color[i] != fBuf2.color[i]) {
            return false;
        }
    }

    return true;
}

double testScene(std::ofstream &f, const std::string &sceneName) {
    Clock clock;
    double time;
    FrameBuffer frameBuffer;
    FrameBuffer reference = ImageManager::loadFrameBuffer("Scenes/Render/"+sceneName+".png");
    Scene scene = SceneLoader::loadScene("Scenes/"+sceneName+".txt");

	switch ( GPUselect ) {
	case 0:
	  f << sceneName << std::endl;
		for(int j = 1; j <= COMMAND_SIZE; j++){
		    clock.restart();
		    for (int i = 0; i < TEST_ITERATIONS; ++i) {
		        GP_ONE_0::clearFrameBuffer(scene.bkgColor);
		        GP_ONE_0::loadSprites(scene.sprites.data(), scene.sprites.size());
		        GP_ONE_0::drawSpriteInstances(j,scene.spriteInstances.data(), scene.spriteInstances.size());
		        GP_ONE_0::saveFrameBuffer(frameBuffer);
		    }
		    time = clock.getTime();
		    f << time << std::endl;

		    std::cout << j << std::endl;
		    //std::string result = compareFrameBuffers(frameBuffer, reference) ? "PASSED" : "FAILED";
		    //std::cout <<sceneName<<" : "<<result<<" : "<<time<<" seconds"<<std::endl;
		}
	  break;
	case 1:
	  f << sceneName << std::endl;
		for(int j = 1; j <= COMMAND_SIZE; j++){
		    clock.restart();
		    for (int i = 0; i < TEST_ITERATIONS; ++i) {
		        GP_ONE_1::clearFrameBuffer(scene.bkgColor);
		        GP_ONE_1::loadSprites(scene.sprites.data(), scene.sprites.size());
		        GP_ONE_1::drawSpriteInstances(j,scene.spriteInstances.data(), scene.spriteInstances.size());
		        GP_ONE_1::saveFrameBuffer(frameBuffer);
		    }
		    time = clock.getTime();
		    f << time << std::endl;

		    std::cout << j << std::endl;
		    //std::string result = compareFrameBuffers(frameBuffer, reference) ? "PASSED" : "FAILED";
		    //std::cout <<sceneName<<" : "<<result<<" : "<<time<<" seconds"<<std::endl;
		}
	  break;
	case 2:
	  f << sceneName << std::endl;
		for(int j = 1; j <= COMMAND_SIZE; j++){
		    clock.restart();
		    for (int i = 0; i < TEST_ITERATIONS; ++i) {
		        GP_ONE_2::clearFrameBuffer(scene.bkgColor);
		        GP_ONE_2::loadSprites(scene.sprites.data(), scene.sprites.size());
		        GP_ONE_2::drawSpriteInstances(j,scene.spriteInstances.data(), scene.spriteInstances.size());
		        GP_ONE_2::saveFrameBuffer(frameBuffer);
		    }
		    time = clock.getTime();
		    f << time << std::endl;

		    std::cout << j << std::endl;
		    //std::string result = compareFrameBuffers(frameBuffer, reference) ? "PASSED" : "FAILED";
		    //std::cout <<sceneName<<" : "<<result<<" : "<<time<<" seconds"<<std::endl;
		}
	  break;
	case 3:
	  f << sceneName << std::endl;
		for(int j = 1; j <= COMMAND_SIZE; j++){
		    clock.restart();
		    for (int i = 0; i < TEST_ITERATIONS; ++i) {
		        GP_ONE_3::clearFrameBuffer(scene.bkgColor);
		        GP_ONE_3::loadSprites(scene.sprites.data(), scene.sprites.size());
		        GP_ONE_3::drawSpriteInstances(j,scene.spriteInstances.data(), scene.spriteInstances.size());
		        GP_ONE_3::saveFrameBuffer(frameBuffer);
		    }
		    time = clock.getTime();
		    f << time << std::endl;

		    std::cout << j << std::endl;
		    //std::string result = compareFrameBuffers(frameBuffer, reference) ? "PASSED" : "FAILED";
		    //std::cout <<sceneName<<" : "<<result<<" : "<<time<<" seconds"<<std::endl;
		}
	  break;
	}	

    ImageManager::saveFrameBuffer("Render/"+sceneName+".png", frameBuffer);


    return time;
}


int main() {

    std::ofstream f("Result" + std::to_string(GPUselect) + ".txt");
    if (!f) {
        throw std::runtime_error("Failed to open file ");
    }
    f << ": TEST_ITERATIONS: " << TEST_ITERATIONS << " COMMAND_SIZE: " << COMMAND_SIZE << std::endl;

    double total_time = 0.0;

    for (int i = 0; i < 5; ++i) {
        total_time += testScene(f,"scene_"+std::to_string(i));
    }
    f << "END" << std::endl;
    f << "END" << std::endl;
    f << "END" << std::endl;
    f << "END" << std::endl;
    f << "END" << std::endl;
    f.close();

    //std::cout << "Total time = " << total_time <<" seconds"<< std::endl;

    return 0;
}
