#include "GP_ONE.h"

#include <immintrin.h>


#define setTileInBuffer(buf,color,alpha)  ( clor&alpha | buf&~alpha )

Sprite   GP_ONE::spriteMemory[MAX_SPRITE_COUNT];
uint16_t GP_ONE::frameBuffer[FRAMEBUFFER_BUF_SIZE];

void GP_ONE::loadSprites(const Sprite *sprites, uint16_t spriteCount) {

    // загружаем спрайты в буфер памяти
    for (uint16_t i = 0; i < spriteCount; i++)
        spriteMemory[i] = sprites[i];
}


void GP_ONE::clearFrameBuffer(BackGroundColor bkgColor) {

    // Очищаем буфер
    uint16_t BGcolor = (uint16_t) bkgColor ? (uint16_t)(0b1111'1111'1111'1111) : (uint16_t)(0b0000'0000'0000'0000);
    for (uint16_t i = 0; i < FRAMEBUFFER_BUF_SIZE; i++)
        frameBuffer[i] = BGcolor;
}

//uint16_t color(uint16_t Sind, uint16_t Sprite_Shift, uint16_t shift_coor_X ) {return GP_ONE::spriteMemory[Sind].color[Sprite_Shift] >> shift_coor_X;}
//uint16_t alpha(uint16_t Sind, uint16_t Sprite_Shift, uint16_t shift_coor_X ) {return GP_ONE::spriteMemory[Sind].alpha[Sprite_Shift] >> shift_coor_X;}

void GP_ONE::drawSpriteInstances(const SpriteInstance *instances, uint16_t instanceCount) {

    // На каждый спрайт по оси Оу и Ох побитово записываем спрайт в буфер по формуле 
    // frameBuffer = (Color & Alpha) | (frameBuffer & ~Alpha)

    uint16_t Sind ;
    uint16_t Sx   ;
    uint16_t Sy   ;

    uint16_t shift_tile_X;
    uint16_t shift_coor_X ;

    uint16_t global_And_Sprite_Shift;
    uint16_t Sprite_Shift;


    __m256i color_m64;
    __m256i alpha_m64 ;
    __m256i buffer_m64 ;
    __m256i result_m64 ;

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


            color_m64  = _mm256_set_epi16( 0,0,0,0,0,0,0,0,0,0,0,0,spriteMemory[Sind].color[Sprite_Shift+3] >> shift_coor_X,spriteMemory[Sind].color[Sprite_Shift+2] >> shift_coor_X,spriteMemory[Sind].color[Sprite_Shift+1] >> shift_coor_X,spriteMemory[Sind].color[Sprite_Shift] >> shift_coor_X);
            alpha_m64  = _mm256_set_epi16( 0,0,0,0,0,0,0,0,0,0,0,0,spriteMemory[Sind].alpha[Sprite_Shift+3] >> shift_coor_X,spriteMemory[Sind].alpha[Sprite_Shift+2] >> shift_coor_X,spriteMemory[Sind].alpha[Sprite_Shift+1] >> shift_coor_X,spriteMemory[Sind].alpha[Sprite_Shift] >> shift_coor_X);
            buffer_m64 = _mm256_set_epi16( 0,0,0,0,0,0,0,0,0,0,0,0,frameBuffer[global_And_Sprite_Shift+3],frameBuffer[global_And_Sprite_Shift+2],frameBuffer[global_And_Sprite_Shift+1],frameBuffer[global_And_Sprite_Shift]);
            
            for (uint16_t k = 0; k < 1; ++k) 
            result_m64 = _mm256_or_si256(_mm256_andnot_si256(alpha_m64, buffer_m64),_mm256_and_si256(alpha_m64, color_m64));

            for (uint16_t j = 0; j < SPRITE_TILES_X; ++j)
                frameBuffer[global_And_Sprite_Shift+j] = result[j]; 
        


            if (shift_coor_X != uint16_t(0)){



                color_m64  = _mm256_set_epi16( 0,0,0,0,0,0,0,0,0,0,0,0,spriteMemory[Sind].color[Sprite_Shift+3] << (uint16_t(16)-shift_coor_X),spriteMemory[Sind].color[Sprite_Shift+2] << (uint16_t(16)-shift_coor_X),spriteMemory[Sind].color[Sprite_Shift+1] << (uint16_t(16)-shift_coor_X),spriteMemory[Sind].color[Sprite_Shift] << (uint16_t(16)-shift_coor_X));
                alpha_m64  = _mm256_set_epi16( 0,0,0,0,0,0,0,0,0,0,0,0,spriteMemory[Sind].alpha[Sprite_Shift+3] << (uint16_t(16)-shift_coor_X),spriteMemory[Sind].alpha[Sprite_Shift+2] << (uint16_t(16)-shift_coor_X),spriteMemory[Sind].alpha[Sprite_Shift+1] << (uint16_t(16)-shift_coor_X),spriteMemory[Sind].alpha[Sprite_Shift] << (uint16_t(16)-shift_coor_X));
                buffer_m64 = _mm256_set_epi16( 0,0,0,0,0,0,0,0,0,0,0,0,frameBuffer[global_And_Sprite_Shift+uint16_t(4)],frameBuffer[global_And_Sprite_Shift+uint16_t(3)],frameBuffer[global_And_Sprite_Shift+uint16_t(2)],frameBuffer[global_And_Sprite_Shift+uint16_t(1)]);
                
                for (uint16_t k = 0; k < 1; ++k) 
                result_m64 = _mm256_or_si256(_mm256_andnot_si256(alpha_m64, buffer_m64),_mm256_and_si256(alpha_m64, color_m64));


                for (uint16_t j = 0; j < SPRITE_TILES_X; ++j)
                    frameBuffer[global_And_Sprite_Shift+j+uint16_t(1)] = result[j]; 

            }

            /*
            color_m64  = _mm_set_epi16(spriteMemory[Sind].color[Sprite_Shift+3] >> shift_coor_X,spriteMemory[Sind].color[Sprite_Shift+2] >> shift_coor_X,spriteMemory[Sind].color[Sprite_Shift+1] >> shift_coor_X,spriteMemory[Sind].color[Sprite_Shift] >> shift_coor_X, 0,0,0,0);
            alpha_m64  = _mm_set_epi16(spriteMemory[Sind].alpha[Sprite_Shift+3] >> shift_coor_X,spriteMemory[Sind].alpha[Sprite_Shift+2] >> shift_coor_X,spriteMemory[Sind].alpha[Sprite_Shift+1] >> shift_coor_X,spriteMemory[Sind].alpha[Sprite_Shift] >> shift_coor_X, 0,0,0,0);
            buffer_m64 = _mm_set_epi16(frameBuffer[global_And_Sprite_Shift+3],frameBuffer[global_And_Sprite_Shift+2],frameBuffer[global_And_Sprite_Shift+1],frameBuffer[global_And_Sprite_Shift],0,0,0,0);
            result_m64 = _mm_or_si128(_mm_andnot_si128(alpha_m64, buffer_m64),_mm_and_si128(alpha_m64, color_m64));

            for (uint16_t j = 0; j < SPRITE_TILES_X; ++j)
                frameBuffer[global_And_Sprite_Shift+j] = result[j+4]; 
        


            if (shift_coor_X != uint16_t(0)){



                color_m64  = _mm_set_epi16(spriteMemory[Sind].color[Sprite_Shift+3] << (uint16_t(16)-shift_coor_X),spriteMemory[Sind].color[Sprite_Shift+2] << (uint16_t(16)-shift_coor_X),spriteMemory[Sind].color[Sprite_Shift+1] << (uint16_t(16)-shift_coor_X),spriteMemory[Sind].color[Sprite_Shift] << (uint16_t(16)-shift_coor_X), 0,0,0,0);
                alpha_m64  = _mm_set_epi16(spriteMemory[Sind].alpha[Sprite_Shift+3] << (uint16_t(16)-shift_coor_X),spriteMemory[Sind].alpha[Sprite_Shift+2] << (uint16_t(16)-shift_coor_X),spriteMemory[Sind].alpha[Sprite_Shift+1] << (uint16_t(16)-shift_coor_X),spriteMemory[Sind].alpha[Sprite_Shift] << (uint16_t(16)-shift_coor_X),0,0,0,0);
                buffer_m64 = _mm_set_epi16(frameBuffer[global_And_Sprite_Shift+uint16_t(4)],frameBuffer[global_And_Sprite_Shift+uint16_t(3)],frameBuffer[global_And_Sprite_Shift+uint16_t(2)],frameBuffer[global_And_Sprite_Shift+uint16_t(1)],0,0,0,0);
                result_m64 = _mm_or_si128(_mm_andnot_si128(alpha_m64, buffer_m64),_mm_and_si128(alpha_m64, color_m64));


                for (uint16_t j = 0; j < SPRITE_TILES_X; ++j)
                    frameBuffer[global_And_Sprite_Shift+j+uint16_t(1)] = result[j+4]; 

            }
            */
        }
    }
    
}


void GP_ONE::saveFrameBuffer(FrameBuffer &outFrameBuffer) {
    for (uint16_t i = 0; i < FRAMEBUFFER_BUF_SIZE; i++)
        outFrameBuffer.color[i] = frameBuffer[i];
}
