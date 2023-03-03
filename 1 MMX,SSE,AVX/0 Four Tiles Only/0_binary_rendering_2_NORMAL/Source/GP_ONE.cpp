#include "GP_ONE.h"

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

void GP_ONE::drawSpriteInstances(const SpriteInstance *instances, uint16_t instanceCount) {

    // На каждый спрайт по оси Оу и Ох побитово записываем спрайт в буфер по формуле 
    // frameBuffer = (Color & Alpha) | (frameBuffer & ~Alpha)
    
    	uint16_t Sind ;
        uint16_t Sx;
        uint16_t Sy ;

        uint16_t shift_tile_X = Sx/16;
        uint16_t shift_coor_X = Sx%16;

        uint16_t global_And_Sprite_Shift;
        uint16_t Sprite_Shift;

        uint16_t color;
        uint16_t alpha;
    
    for (uint16_t currSprite = 0; currSprite < instanceCount; currSprite++){

        Sind = instances[currSprite].ind;
        Sx   = instances[currSprite].x;
        Sy   = instances[currSprite].y;

        shift_tile_X = Sx/16;
        shift_coor_X = Sx%16;


        for (uint16_t i = 0; i < SPRITE_TILES_Y; ++i) {
            for (uint16_t j = 0; j < SPRITE_TILES_X; ++j) {
                
                global_And_Sprite_Shift = (i+Sy)*FRAMEBUFFER_TILES_X + (j + shift_tile_X);
                Sprite_Shift = i*SPRITE_TILES_X + j;

                color = spriteMemory[Sind].color[Sprite_Shift] >> shift_coor_X;
                alpha = spriteMemory[Sind].alpha[Sprite_Shift] >> shift_coor_X;

		for (uint16_t k = 0; k < 1; ++k) 
                frameBuffer[ global_And_Sprite_Shift ] = color&alpha | frameBuffer[ global_And_Sprite_Shift ]&~alpha;

                if (shift_coor_X != uint16_t(0)){
                    color = spriteMemory[Sind].color[Sprite_Shift] << (uint16_t(16)-shift_coor_X);
                    alpha = spriteMemory[Sind].alpha[Sprite_Shift] << (uint16_t(16)-shift_coor_X);
                    
                    for (uint16_t k = 0; k < 1; ++k) 
                    frameBuffer[global_And_Sprite_Shift + 1] = color&alpha | frameBuffer[ global_And_Sprite_Shift + 1 ]&~alpha;
                }
            }
        }
    }
    
}


void GP_ONE::saveFrameBuffer(FrameBuffer &outFrameBuffer) {
    for (uint16_t i = 0; i < FRAMEBUFFER_BUF_SIZE; i++)
        outFrameBuffer.color[i] = frameBuffer[i];
}
