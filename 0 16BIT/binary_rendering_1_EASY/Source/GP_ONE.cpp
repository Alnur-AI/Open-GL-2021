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
    //uint16_t BGcolor = (uint16_t) bkgColor ? (uint16_t)(0b1111'1111'1111'1111) : (uint16_t)(0b0000'0000'0000'0000);
    uint16_t BGcolor = static_cast<uint16_t> (bkgColor) ? (uint16_t)(0b1111'1111'1111'1111) : (uint16_t)(0b0000'0000'0000'0000);
    for (uint16_t i = 0; i < FRAMEBUFFER_BUF_SIZE; i++)
        frameBuffer[i] = BGcolor;
}

uint16_t res(uint16_t buf, uint16_t color, uint16_t alpha){
    
    return buf = color & alpha | buf & ~alpha;
}

void GP_ONE::drawSpriteInstances(const SpriteInstance *instances, uint16_t instanceCount) {

    // На каждый спрайт по оси Оу и Ох побитово записываем спрайт в буфер по формуле 
    // frameBuffer = (Color & Alpha) | (frameBuffer & ~Alpha)
    for (uint16_t currSprite = 0; currSprite < instanceCount; currSprite++){

        uint16_t Sind = instances[currSprite].ind;
        uint16_t Sx   = instances[currSprite].x;
        uint16_t Sy   = instances[currSprite].y;

        for (uint16_t i = 0; i < SPRITE_TILES_Y; ++i) {
            for (uint16_t j = 0; j < SPRITE_TILES_X; ++j) {
                
                if (Sx%16 != 0){
                    uint16_t xx1 = frameBuffer[(i+Sy)*FRAMEBUFFER_TILES_X + j + Sx/16 + 0];
                    uint16_t xx2 = frameBuffer[(i+Sy)*FRAMEBUFFER_TILES_X + j + Sx/16 + 1];

                    uint16_t yy1 = spriteMemory[Sind].color[i*SPRITE_TILES_X + j] >> Sx%16;
                    uint16_t yy2 = spriteMemory[Sind].color[i*SPRITE_TILES_X + j] << (16-Sx%16);

                    uint16_t zz1 = spriteMemory[Sind].alpha[i*SPRITE_TILES_X + j] >> Sx%16;
                    uint16_t zz2 = spriteMemory[Sind].alpha[i*SPRITE_TILES_X + j] << (16-Sx%16);

                    frameBuffer[(i+Sy)*FRAMEBUFFER_TILES_X + j + Sx/16 + 0] = res(xx1,yy1,zz1);
                    frameBuffer[(i+Sy)*FRAMEBUFFER_TILES_X + j + Sx/16 + 1] = res(xx2,yy2,zz2);
                }
                
                if (Sx%16 == 0){
                    uint16_t xx = frameBuffer[(i+Sy)*FRAMEBUFFER_TILES_X + j + Sx/16];
                    uint16_t yy = spriteMemory[Sind].color[i*SPRITE_TILES_X + j];
                    uint16_t zz = spriteMemory[Sind].alpha[i*SPRITE_TILES_X + j];

                    frameBuffer[(i+Sy)*FRAMEBUFFER_TILES_X + j + Sx/16] = res(xx, yy, zz);
                }
                
            }
        }
    }
    
}


void GP_ONE::saveFrameBuffer(FrameBuffer &outFrameBuffer) {
    for (uint16_t i = 0; i < FRAMEBUFFER_BUF_SIZE; i++)
        outFrameBuffer.color[i] = frameBuffer[i];
}