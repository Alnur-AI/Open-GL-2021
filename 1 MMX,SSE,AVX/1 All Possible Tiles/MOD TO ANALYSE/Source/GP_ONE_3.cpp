#include "GP_ONE_3.h"

#include <immintrin.h>


#define setTileInBuffer(buf,color,alpha)  ( clor&alpha | buf&~alpha )

Sprite   GP_ONE_3::spriteMemory[MAX_SPRITE_COUNT];
uint16_t GP_ONE_3::frameBuffer[FRAMEBUFFER_BUF_SIZE];

void GP_ONE_3::loadSprites(const Sprite *sprites, uint16_t spriteCount) {

    // загружаем спрайты в буфер памяти
    for (uint16_t i = 0; i < spriteCount; i++)
        spriteMemory[i] = sprites[i];
}


void GP_ONE_3::clearFrameBuffer(BackGroundColor bkgColor) {

    // Очищаем буфер
    uint16_t BGcolor = (uint16_t) bkgColor ? (uint16_t)(0b1111'1111'1111'1111) : (uint16_t)(0b0000'0000'0000'0000);
    for (uint16_t i = 0; i < FRAMEBUFFER_BUF_SIZE; i++)
        frameBuffer[i] = BGcolor;
}

//uint16_t color(uint16_t Sind, uint16_t Sprite_Shift, uint16_t shift_coor_X ) {return GP_ONE::spriteMemory[Sind].color[Sprite_Shift] >> shift_coor_X;}
//uint16_t alpha(uint16_t Sind, uint16_t Sprite_Shift, uint16_t shift_coor_X ) {return GP_ONE::spriteMemory[Sind].alpha[Sprite_Shift] >> shift_coor_X;}

void GP_ONE_3::drawSpriteInstances(uint16_t command_size, const SpriteInstance *instances, uint16_t instanceCount) {

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


        for (uint16_t i = 0; i < SPRITE_TILES_Y>>uint16_t(2); ++i) {

            //SPRITE_TILES_X==4
            global_And_Sprite_Shift = ((i<<uint16_t(2))+Sy)*FRAMEBUFFER_TILES_X + (shift_tile_X);
            Sprite_Shift = (i<<uint16_t(2))*SPRITE_TILES_X;

            color_m64  = _mm256_set_epi16(  spriteMemory[Sind].color[Sprite_Shift+uint16_t(15)] >> shift_coor_X,
                                            spriteMemory[Sind].color[Sprite_Shift+uint16_t(14)] >> shift_coor_X,
                                            spriteMemory[Sind].color[Sprite_Shift+uint16_t(13)] >> shift_coor_X,
                                            spriteMemory[Sind].color[Sprite_Shift+uint16_t(12)] >> shift_coor_X, 
                                            spriteMemory[Sind].color[Sprite_Shift+uint16_t(11)] >> shift_coor_X,
                                            spriteMemory[Sind].color[Sprite_Shift+uint16_t(10)] >> shift_coor_X,
                                            spriteMemory[Sind].color[Sprite_Shift+uint16_t(9) ] >> shift_coor_X,
                                            spriteMemory[Sind].color[Sprite_Shift+uint16_t(8) ] >> shift_coor_X,
                                            spriteMemory[Sind].color[Sprite_Shift+uint16_t(7) ] >> shift_coor_X,
                                            spriteMemory[Sind].color[Sprite_Shift+uint16_t(6) ] >> shift_coor_X,
                                            spriteMemory[Sind].color[Sprite_Shift+uint16_t(5) ] >> shift_coor_X,
                                            spriteMemory[Sind].color[Sprite_Shift+uint16_t(4) ] >> shift_coor_X, 
                                            spriteMemory[Sind].color[Sprite_Shift+uint16_t(3) ] >> shift_coor_X,
                                            spriteMemory[Sind].color[Sprite_Shift+uint16_t(2) ] >> shift_coor_X,
                                            spriteMemory[Sind].color[Sprite_Shift+uint16_t(1) ] >> shift_coor_X,
                                            spriteMemory[Sind].color[Sprite_Shift             ] >> shift_coor_X);
            alpha_m64  = _mm256_set_epi16(  spriteMemory[Sind].alpha[Sprite_Shift+uint16_t(15)] >> shift_coor_X,
                                            spriteMemory[Sind].alpha[Sprite_Shift+uint16_t(14)] >> shift_coor_X,
                                            spriteMemory[Sind].alpha[Sprite_Shift+uint16_t(13)] >> shift_coor_X,
                                            spriteMemory[Sind].alpha[Sprite_Shift+uint16_t(12)] >> shift_coor_X, 
                                            spriteMemory[Sind].alpha[Sprite_Shift+uint16_t(11)] >> shift_coor_X,
                                            spriteMemory[Sind].alpha[Sprite_Shift+uint16_t(10)] >> shift_coor_X,
                                            spriteMemory[Sind].alpha[Sprite_Shift+uint16_t(9) ] >> shift_coor_X,
                                            spriteMemory[Sind].alpha[Sprite_Shift+uint16_t(8) ] >> shift_coor_X,
                                            spriteMemory[Sind].alpha[Sprite_Shift+uint16_t(7) ] >> shift_coor_X,
                                            spriteMemory[Sind].alpha[Sprite_Shift+uint16_t(6) ] >> shift_coor_X,
                                            spriteMemory[Sind].alpha[Sprite_Shift+uint16_t(5) ] >> shift_coor_X,
                                            spriteMemory[Sind].alpha[Sprite_Shift+uint16_t(4) ] >> shift_coor_X, 
                                            spriteMemory[Sind].alpha[Sprite_Shift+uint16_t(3) ] >> shift_coor_X,
                                            spriteMemory[Sind].alpha[Sprite_Shift+uint16_t(2) ] >> shift_coor_X,
                                            spriteMemory[Sind].alpha[Sprite_Shift+uint16_t(1) ] >> shift_coor_X,
                                            spriteMemory[Sind].alpha[Sprite_Shift             ] >> shift_coor_X);
            buffer_m64 = _mm256_set_epi16(  frameBuffer[global_And_Sprite_Shift+uint16_t(3)+3*FRAMEBUFFER_TILES_X],
                                            frameBuffer[global_And_Sprite_Shift+uint16_t(2)+3*FRAMEBUFFER_TILES_X],
                                            frameBuffer[global_And_Sprite_Shift+uint16_t(1)+3*FRAMEBUFFER_TILES_X],
                                            frameBuffer[global_And_Sprite_Shift+            3*FRAMEBUFFER_TILES_X],
                                            frameBuffer[global_And_Sprite_Shift+uint16_t(3)+2*FRAMEBUFFER_TILES_X],
                                            frameBuffer[global_And_Sprite_Shift+uint16_t(2)+2*FRAMEBUFFER_TILES_X],
                                            frameBuffer[global_And_Sprite_Shift+uint16_t(1)+2*FRAMEBUFFER_TILES_X],
                                            frameBuffer[global_And_Sprite_Shift+            2*FRAMEBUFFER_TILES_X],
                                            frameBuffer[global_And_Sprite_Shift+uint16_t(3)+  FRAMEBUFFER_TILES_X],
                                            frameBuffer[global_And_Sprite_Shift+uint16_t(2)+  FRAMEBUFFER_TILES_X],
                                            frameBuffer[global_And_Sprite_Shift+uint16_t(1)+  FRAMEBUFFER_TILES_X],
                                            frameBuffer[global_And_Sprite_Shift+              FRAMEBUFFER_TILES_X],
                                            frameBuffer[global_And_Sprite_Shift+uint16_t(3)],
                                            frameBuffer[global_And_Sprite_Shift+uint16_t(2)],
                                            frameBuffer[global_And_Sprite_Shift+uint16_t(1)],
                                            frameBuffer[global_And_Sprite_Shift            ]);


            
            for (uint16_t k = 0; k < command_size; ++k) 
            result_m64 = _mm256_or_si256(_mm256_andnot_si256(alpha_m64, buffer_m64),_mm256_and_si256(alpha_m64, color_m64));



            for (uint16_t j = 0; j < SPRITE_TILES_X; ++j){
                frameBuffer[global_And_Sprite_Shift+j] = result[j]; 
                frameBuffer[global_And_Sprite_Shift+j+FRAMEBUFFER_TILES_X] = result[j+SPRITE_TILES_X]; 
                frameBuffer[global_And_Sprite_Shift+j+2*FRAMEBUFFER_TILES_X] = result[j+2*SPRITE_TILES_X]; 
                frameBuffer[global_And_Sprite_Shift+j+3*FRAMEBUFFER_TILES_X] = result[j+3*SPRITE_TILES_X]; 
            }


            if (shift_coor_X != uint16_t(0)){

            color_m64  = _mm256_set_epi16(  spriteMemory[Sind].color[Sprite_Shift+uint16_t(15)] << (uint16_t(16)-shift_coor_X),
                                            spriteMemory[Sind].color[Sprite_Shift+uint16_t(14)] << (uint16_t(16)-shift_coor_X),
                                            spriteMemory[Sind].color[Sprite_Shift+uint16_t(13)] << (uint16_t(16)-shift_coor_X),
                                            spriteMemory[Sind].color[Sprite_Shift+uint16_t(12)] << (uint16_t(16)-shift_coor_X), 
                                            spriteMemory[Sind].color[Sprite_Shift+uint16_t(11)] << (uint16_t(16)-shift_coor_X),
                                            spriteMemory[Sind].color[Sprite_Shift+uint16_t(10)] << (uint16_t(16)-shift_coor_X),
                                            spriteMemory[Sind].color[Sprite_Shift+uint16_t(9) ] << (uint16_t(16)-shift_coor_X),
                                            spriteMemory[Sind].color[Sprite_Shift+uint16_t(8) ] << (uint16_t(16)-shift_coor_X),
                                            spriteMemory[Sind].color[Sprite_Shift+uint16_t(7) ] << (uint16_t(16)-shift_coor_X),
                                            spriteMemory[Sind].color[Sprite_Shift+uint16_t(6) ] << (uint16_t(16)-shift_coor_X),
                                            spriteMemory[Sind].color[Sprite_Shift+uint16_t(5) ] << (uint16_t(16)-shift_coor_X),
                                            spriteMemory[Sind].color[Sprite_Shift+uint16_t(4) ] << (uint16_t(16)-shift_coor_X), 
                                            spriteMemory[Sind].color[Sprite_Shift+uint16_t(3) ] << (uint16_t(16)-shift_coor_X),
                                            spriteMemory[Sind].color[Sprite_Shift+uint16_t(2) ] << (uint16_t(16)-shift_coor_X),
                                            spriteMemory[Sind].color[Sprite_Shift+uint16_t(1) ] << (uint16_t(16)-shift_coor_X),
                                            spriteMemory[Sind].color[Sprite_Shift             ] << (uint16_t(16)-shift_coor_X));
            alpha_m64  = _mm256_set_epi16(  spriteMemory[Sind].alpha[Sprite_Shift+uint16_t(15)] << (uint16_t(16)-shift_coor_X),
                                            spriteMemory[Sind].alpha[Sprite_Shift+uint16_t(14)] << (uint16_t(16)-shift_coor_X),
                                            spriteMemory[Sind].alpha[Sprite_Shift+uint16_t(13)] << (uint16_t(16)-shift_coor_X),
                                            spriteMemory[Sind].alpha[Sprite_Shift+uint16_t(12)] << (uint16_t(16)-shift_coor_X), 
                                            spriteMemory[Sind].alpha[Sprite_Shift+uint16_t(11)] << (uint16_t(16)-shift_coor_X),
                                            spriteMemory[Sind].alpha[Sprite_Shift+uint16_t(10)] << (uint16_t(16)-shift_coor_X),
                                            spriteMemory[Sind].alpha[Sprite_Shift+uint16_t(9) ] << (uint16_t(16)-shift_coor_X),
                                            spriteMemory[Sind].alpha[Sprite_Shift+uint16_t(8) ] << (uint16_t(16)-shift_coor_X),
                                            spriteMemory[Sind].alpha[Sprite_Shift+uint16_t(7) ] << (uint16_t(16)-shift_coor_X),
                                            spriteMemory[Sind].alpha[Sprite_Shift+uint16_t(6) ] << (uint16_t(16)-shift_coor_X),
                                            spriteMemory[Sind].alpha[Sprite_Shift+uint16_t(5) ] << (uint16_t(16)-shift_coor_X),
                                            spriteMemory[Sind].alpha[Sprite_Shift+uint16_t(4) ] << (uint16_t(16)-shift_coor_X), 
                                            spriteMemory[Sind].alpha[Sprite_Shift+uint16_t(3) ] << (uint16_t(16)-shift_coor_X),
                                            spriteMemory[Sind].alpha[Sprite_Shift+uint16_t(2) ] << (uint16_t(16)-shift_coor_X),
                                            spriteMemory[Sind].alpha[Sprite_Shift+uint16_t(1) ] << (uint16_t(16)-shift_coor_X),
                                            spriteMemory[Sind].alpha[Sprite_Shift             ] << (uint16_t(16)-shift_coor_X));
            buffer_m64 = _mm256_set_epi16(  frameBuffer[global_And_Sprite_Shift+uint16_t(4)+3*FRAMEBUFFER_TILES_X],
                                            frameBuffer[global_And_Sprite_Shift+uint16_t(3)+3*FRAMEBUFFER_TILES_X],
                                            frameBuffer[global_And_Sprite_Shift+uint16_t(2)+3*FRAMEBUFFER_TILES_X],
                                            frameBuffer[global_And_Sprite_Shift+uint16_t(1)+3*FRAMEBUFFER_TILES_X],
                                            frameBuffer[global_And_Sprite_Shift+uint16_t(4)+2*FRAMEBUFFER_TILES_X],
                                            frameBuffer[global_And_Sprite_Shift+uint16_t(3)+2*FRAMEBUFFER_TILES_X],
                                            frameBuffer[global_And_Sprite_Shift+uint16_t(2)+2*FRAMEBUFFER_TILES_X],
                                            frameBuffer[global_And_Sprite_Shift+uint16_t(1)+2*FRAMEBUFFER_TILES_X],
                                            frameBuffer[global_And_Sprite_Shift+uint16_t(4)+  FRAMEBUFFER_TILES_X],
                                            frameBuffer[global_And_Sprite_Shift+uint16_t(3)+  FRAMEBUFFER_TILES_X],
                                            frameBuffer[global_And_Sprite_Shift+uint16_t(2)+  FRAMEBUFFER_TILES_X],
                                            frameBuffer[global_And_Sprite_Shift+uint16_t(1)+  FRAMEBUFFER_TILES_X],
                                            frameBuffer[global_And_Sprite_Shift+uint16_t(4)],
                                            frameBuffer[global_And_Sprite_Shift+uint16_t(3)],
                                            frameBuffer[global_And_Sprite_Shift+uint16_t(2)],
                                            frameBuffer[global_And_Sprite_Shift+uint16_t(1)]);

                //color_m64  = _mm256_set_epi16( 0,0,0,0,0,0,0,0,0,0,0,0,spriteMemory[Sind].color[Sprite_Shift+3] << (uint16_t(16)-shift_coor_X),spriteMemory[Sind].color[Sprite_Shift+2] << (uint16_t(16)-shift_coor_X),spriteMemory[Sind].color[Sprite_Shift+1] << (uint16_t(16)-shift_coor_X),spriteMemory[Sind].color[Sprite_Shift] << (uint16_t(16)-shift_coor_X));
                //alpha_m64  = _mm256_set_epi16( 0,0,0,0,0,0,0,0,0,0,0,0,spriteMemory[Sind].alpha[Sprite_Shift+3] << (uint16_t(16)-shift_coor_X),spriteMemory[Sind].alpha[Sprite_Shift+2] << (uint16_t(16)-shift_coor_X),spriteMemory[Sind].alpha[Sprite_Shift+1] << (uint16_t(16)-shift_coor_X),spriteMemory[Sind].alpha[Sprite_Shift] << (uint16_t(16)-shift_coor_X));
                //buffer_m64 = _mm256_set_epi16( 0,0,0,0,0,0,0,0,0,0,0,0,frameBuffer[global_And_Sprite_Shift+uint16_t(4)],frameBuffer[global_And_Sprite_Shift+uint16_t(3)],frameBuffer[global_And_Sprite_Shift+uint16_t(2)],frameBuffer[global_And_Sprite_Shift+uint16_t(1)]);
                
                for (uint16_t k = 0; k < command_size; ++k) 
                result_m64 = _mm256_or_si256(_mm256_andnot_si256(alpha_m64, buffer_m64),_mm256_and_si256(alpha_m64, color_m64));


                for (uint16_t j = 0; j < SPRITE_TILES_X; ++j){
                    frameBuffer[global_And_Sprite_Shift+j+uint16_t(1)] = result[j]; 
                    frameBuffer[global_And_Sprite_Shift+j+uint16_t(1)+FRAMEBUFFER_TILES_X] = result[j+SPRITE_TILES_X]; 
                    frameBuffer[global_And_Sprite_Shift+j+uint16_t(1)+2*FRAMEBUFFER_TILES_X] = result[j+2*SPRITE_TILES_X]; 
                    frameBuffer[global_And_Sprite_Shift+j+uint16_t(1)+3*FRAMEBUFFER_TILES_X] = result[j+3*SPRITE_TILES_X]; 
                }
            }

            /*
    for (uint16_t currSprite = 0; currSprite < instanceCount; currSprite++){

        for (uint16_t i = 0; i < SPRITE_TILES_Y>>uint16_t(1); ++i) {

            //SPRITE_TILES_X==4
            //global_And_Sprite_Shift = (2*i+Sy)*FRAMEBUFFER_TILES_X + (shift_tile_X);
            global_And_Sprite_Shift = ((i<<uint16_t(1))+Sy)*FRAMEBUFFER_TILES_X + shift_tile_X;
            Sprite_Shift = (i<<uint16_t(1))*SPRITE_TILES_X;

            color_m64  = _mm_set_epi16( spriteMemory[Sind].color[Sprite_Shift+uint16_t(7)] >> shift_coor_X,
                                        spriteMemory[Sind].color[Sprite_Shift+uint16_t(6)] >> shift_coor_X,
                                        spriteMemory[Sind].color[Sprite_Shift+uint16_t(5)] >> shift_coor_X,
                                        spriteMemory[Sind].color[Sprite_Shift+uint16_t(4)] >> shift_coor_X, 
                                        spriteMemory[Sind].color[Sprite_Shift+uint16_t(3)] >> shift_coor_X,
                                        spriteMemory[Sind].color[Sprite_Shift+uint16_t(2)] >> shift_coor_X,
                                        spriteMemory[Sind].color[Sprite_Shift+uint16_t(1)] >> shift_coor_X,
                                        spriteMemory[Sind].color[Sprite_Shift            ] >> shift_coor_X);
            alpha_m64  = _mm_set_epi16( spriteMemory[Sind].alpha[Sprite_Shift+uint16_t(7)] >> shift_coor_X,
                                        spriteMemory[Sind].alpha[Sprite_Shift+uint16_t(6)] >> shift_coor_X,
                                        spriteMemory[Sind].alpha[Sprite_Shift+uint16_t(5)] >> shift_coor_X,
                                        spriteMemory[Sind].alpha[Sprite_Shift+uint16_t(4)] >> shift_coor_X, 
                                        spriteMemory[Sind].alpha[Sprite_Shift+uint16_t(3)] >> shift_coor_X,
                                        spriteMemory[Sind].alpha[Sprite_Shift+uint16_t(2)] >> shift_coor_X,
                                        spriteMemory[Sind].alpha[Sprite_Shift+uint16_t(1)] >> shift_coor_X,
                                        spriteMemory[Sind].alpha[Sprite_Shift            ] >> shift_coor_X);
            buffer_m64 = _mm_set_epi16( frameBuffer[global_And_Sprite_Shift+uint16_t(3)+FRAMEBUFFER_TILES_X],
                                        frameBuffer[global_And_Sprite_Shift+uint16_t(2)+FRAMEBUFFER_TILES_X],
                                        frameBuffer[global_And_Sprite_Shift+uint16_t(1)+FRAMEBUFFER_TILES_X],
                                        frameBuffer[global_And_Sprite_Shift+            FRAMEBUFFER_TILES_X],
                                        frameBuffer[global_And_Sprite_Shift+uint16_t(3)],
                                        frameBuffer[global_And_Sprite_Shift+uint16_t(2)],
                                        frameBuffer[global_And_Sprite_Shift+uint16_t(1)],
                                        frameBuffer[global_And_Sprite_Shift            ]);
            

            for (uint16_t k = 0; k < uint16_t(500); ++k) 
            result_m64 = _mm_or_si128(_mm_andnot_si128(alpha_m64, buffer_m64),_mm_and_si128(alpha_m64, color_m64));

            for (uint16_t j = 0; j < SPRITE_TILES_X; ++j){
                frameBuffer[global_And_Sprite_Shift+j] = result[j]; 
                frameBuffer[global_And_Sprite_Shift+j+FRAMEBUFFER_TILES_X] = result[j+SPRITE_TILES_X]; 
            }
        


            if (shift_coor_X != uint16_t(0)){

                color_m64  = _mm_set_epi16( spriteMemory[Sind].color[Sprite_Shift+uint16_t(7)] << (uint16_t(16)-shift_coor_X),
                                            spriteMemory[Sind].color[Sprite_Shift+uint16_t(6)] << (uint16_t(16)-shift_coor_X),
                                            spriteMemory[Sind].color[Sprite_Shift+uint16_t(5)] << (uint16_t(16)-shift_coor_X),
                                            spriteMemory[Sind].color[Sprite_Shift+uint16_t(4)] << (uint16_t(16)-shift_coor_X),
                                            spriteMemory[Sind].color[Sprite_Shift+uint16_t(3)] << (uint16_t(16)-shift_coor_X),
                                            spriteMemory[Sind].color[Sprite_Shift+uint16_t(2)] << (uint16_t(16)-shift_coor_X),
                                            spriteMemory[Sind].color[Sprite_Shift+uint16_t(1)] << (uint16_t(16)-shift_coor_X),
                                            spriteMemory[Sind].color[Sprite_Shift            ] << (uint16_t(16)-shift_coor_X));
                alpha_m64  = _mm_set_epi16( spriteMemory[Sind].alpha[Sprite_Shift+uint16_t(7)] << (uint16_t(16)-shift_coor_X),
                                            spriteMemory[Sind].alpha[Sprite_Shift+uint16_t(6)] << (uint16_t(16)-shift_coor_X),
                                            spriteMemory[Sind].alpha[Sprite_Shift+uint16_t(5)] << (uint16_t(16)-shift_coor_X),
                                            spriteMemory[Sind].alpha[Sprite_Shift+uint16_t(4)] << (uint16_t(16)-shift_coor_X),
                                            spriteMemory[Sind].alpha[Sprite_Shift+uint16_t(3)] << (uint16_t(16)-shift_coor_X),
                                            spriteMemory[Sind].alpha[Sprite_Shift+uint16_t(2)] << (uint16_t(16)-shift_coor_X),
                                            spriteMemory[Sind].alpha[Sprite_Shift+uint16_t(1)] << (uint16_t(16)-shift_coor_X),
                                            spriteMemory[Sind].alpha[Sprite_Shift            ] << (uint16_t(16)-shift_coor_X));
                buffer_m64 = _mm_set_epi16( frameBuffer[global_And_Sprite_Shift+uint16_t(4)+FRAMEBUFFER_TILES_X],
                                            frameBuffer[global_And_Sprite_Shift+uint16_t(3)+FRAMEBUFFER_TILES_X],
                                            frameBuffer[global_And_Sprite_Shift+uint16_t(2)+FRAMEBUFFER_TILES_X],
                                            frameBuffer[global_And_Sprite_Shift+uint16_t(1)+FRAMEBUFFER_TILES_X],
                                            frameBuffer[global_And_Sprite_Shift+uint16_t(4)],
                                            frameBuffer[global_And_Sprite_Shift+uint16_t(3)],
                                            frameBuffer[global_And_Sprite_Shift+uint16_t(2)],
                                            frameBuffer[global_And_Sprite_Shift+uint16_t(1)]);
                
                for (uint16_t k = 0; k < uint16_t(500); ++k) 
                result_m64 = _mm_or_si128(_mm_andnot_si128(alpha_m64, buffer_m64),_mm_and_si128(alpha_m64, color_m64));


                for (uint16_t j = 0; j < SPRITE_TILES_X; ++j){
                    frameBuffer[global_And_Sprite_Shift+j+uint16_t(1)] = result[j]; 
                    frameBuffer[global_And_Sprite_Shift+j+FRAMEBUFFER_TILES_X+uint16_t(1)] = result[j+SPRITE_TILES_X]; 
                }
            }

            */
        }
    }
}


void GP_ONE_3::saveFrameBuffer(FrameBuffer &outFrameBuffer) {
    for (uint16_t i = 0; i < FRAMEBUFFER_BUF_SIZE; i++)
        outFrameBuffer.color[i] = frameBuffer[i];
}
