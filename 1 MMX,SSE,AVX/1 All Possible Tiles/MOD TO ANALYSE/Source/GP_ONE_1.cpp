#include "GP_ONE_1.h"

#include <mmintrin.h>


Sprite   GP_ONE_1::spriteMemory[MAX_SPRITE_COUNT];
uint16_t GP_ONE_1::frameBuffer[FRAMEBUFFER_BUF_SIZE];



void GP_ONE_1::loadSprites(const Sprite *sprites, uint16_t spriteCount) {

    // загружаем спрайты в буфер памяти
    for (uint16_t i = 0; i < spriteCount; i++)
        spriteMemory[i] = sprites[i];
}


void GP_ONE_1::clearFrameBuffer(BackGroundColor bkgColor) {

    // Очищаем буфер
    uint16_t BGcolor = (uint16_t) bkgColor ? (uint16_t)(0b1111'1111'1111'1111) : (uint16_t)(0b0000'0000'0000'0000);
    for (uint16_t i = 0; i < FRAMEBUFFER_BUF_SIZE; i++)
        frameBuffer[i] = BGcolor;
}

//uint16_t color(uint16_t Sind, uint16_t Sprite_Shift, uint16_t shift_coor_X ) {return GP_ONE::spriteMemory[Sind].color[Sprite_Shift] >> shift_coor_X;}
//uint16_t alpha(uint16_t Sind, uint16_t Sprite_Shift, uint16_t shift_coor_X ) {return GP_ONE::spriteMemory[Sind].alpha[Sprite_Shift] >> shift_coor_X;}

void GP_ONE_1::drawSpriteInstances(uint16_t command_size, const SpriteInstance *instances, uint16_t instanceCount) {

// На каждый спрайт по оси Оу и Ох побитово записываем спрайт в буфер по формуле 
    // frameBuffer = (Color & Alpha) | (frameBuffer & ~Alpha)

    uint16_t Sind ;
    uint16_t Sx   ;
    uint16_t Sy   ;

    uint16_t shift_tile_X;
    uint16_t shift_coor_X ;

    uint16_t global_And_Sprite_Shift;
    uint16_t Sprite_Shift;


    __m64 color_m64;
    __m64 alpha_m64 ;
    __m64 buffer_m64 ;
    __m64 result_m64 ;

    uint16_t* result = (uint16_t*)&result_m64;


    for (uint16_t currSprite = 0; currSprite < instanceCount; currSprite++){

        Sind = instances[currSprite].ind;
        Sx   = instances[currSprite].x;
        Sy   = instances[currSprite].y;

        shift_tile_X = Sx/uint16_t(16);
        shift_coor_X = Sx%uint16_t(16);


        for (uint16_t i = 0; i < SPRITE_TILES_Y; ++i) {

            //SPRITE_TILES_X==4
            global_And_Sprite_Shift = (i+Sy)*FRAMEBUFFER_TILES_X + (shift_tile_X);
            Sprite_Shift = i*SPRITE_TILES_X;


            // if (shift_coor_X == uint16_t(0)){
                color_m64  = _mm_set_pi16(spriteMemory[Sind].color[Sprite_Shift+3] >> shift_coor_X,spriteMemory[Sind].color[Sprite_Shift+2] >> shift_coor_X,spriteMemory[Sind].color[Sprite_Shift+1] >> shift_coor_X,spriteMemory[Sind].color[Sprite_Shift] >> shift_coor_X);
                alpha_m64  = _mm_set_pi16(spriteMemory[Sind].alpha[Sprite_Shift+3] >> shift_coor_X,spriteMemory[Sind].alpha[Sprite_Shift+2] >> shift_coor_X,spriteMemory[Sind].alpha[Sprite_Shift+1] >> shift_coor_X,spriteMemory[Sind].alpha[Sprite_Shift] >> shift_coor_X);
                buffer_m64 = _mm_set_pi16(frameBuffer[global_And_Sprite_Shift+3],frameBuffer[global_And_Sprite_Shift+2],frameBuffer[global_And_Sprite_Shift+1],frameBuffer[global_And_Sprite_Shift]);
                
                for (uint16_t k = 0; k < command_size; ++k) 
                result_m64 = _m_por(_mm_andnot_si64(alpha_m64, buffer_m64),_mm_and_si64(alpha_m64, color_m64));

                for (uint16_t j = 0; j < SPRITE_TILES_X; ++j)
                    frameBuffer[global_And_Sprite_Shift+j] = result[j]; 
            //}


            if (shift_coor_X != uint16_t(0)){

                color_m64  = _mm_set_pi16(spriteMemory[Sind].color[Sprite_Shift+3] << (uint16_t(16)-shift_coor_X),spriteMemory[Sind].color[Sprite_Shift+2] << (uint16_t(16)-shift_coor_X),spriteMemory[Sind].color[Sprite_Shift+1] << (uint16_t(16)-shift_coor_X),spriteMemory[Sind].color[Sprite_Shift] << (uint16_t(16)-shift_coor_X));
                alpha_m64  = _mm_set_pi16(spriteMemory[Sind].alpha[Sprite_Shift+3] << (uint16_t(16)-shift_coor_X),spriteMemory[Sind].alpha[Sprite_Shift+2] << (uint16_t(16)-shift_coor_X),spriteMemory[Sind].alpha[Sprite_Shift+1] << (uint16_t(16)-shift_coor_X),spriteMemory[Sind].alpha[Sprite_Shift] << (uint16_t(16)-shift_coor_X));
                buffer_m64 = _mm_set_pi16(frameBuffer[global_And_Sprite_Shift+4],frameBuffer[global_And_Sprite_Shift+3],frameBuffer[global_And_Sprite_Shift+2],frameBuffer[global_And_Sprite_Shift+1]);
                
                for (uint16_t k = 0; k < command_size; ++k) 
                result_m64 = _m_por(_mm_andnot_si64(alpha_m64, buffer_m64),_mm_and_si64(alpha_m64, color_m64));

                for (uint16_t j = 0; j < SPRITE_TILES_X; ++j)
                    frameBuffer[global_And_Sprite_Shift+j+uint16_t(1)] = result[j]; 

            }
        }
    }
    
}


void GP_ONE_1::saveFrameBuffer(FrameBuffer &outFrameBuffer) {
    for (uint16_t i = 0; i < FRAMEBUFFER_BUF_SIZE; i++)
        outFrameBuffer.color[i] = frameBuffer[i];
}
