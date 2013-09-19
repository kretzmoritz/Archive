#include "syl_particle.h"






void sParticle::dynamicSize()
{
    particleListLeft.info = new m_struct_PARTICLE [streamCount];
    particleListRight.info = new m_struct_PARTICLE [streamCount];
}






void sParticle::drawParticles(m_struct_EFFECT & particleList, m_struct_SETTING & setting, int particleTexture, bool wireOptions, bool billboard, sVector right, sVector up, int polygonCount)
{
    glBindTexture(GL_TEXTURE_2D, particleTexture);

    /* Check if these are entirely new particles */

    for (loop = 0; loop < polygonCount; loop++)
    {
        if (particleList.info[loop].newParticles == true)
        {
            particleList.info[loop].xPosition = setting.xPosition;
            particleList.info[loop].yPosition = setting.yPosition;
            particleList.info[loop].zPosition = setting.zPosition;
            particleList.info[loop].xSpeed = setting.xSpeed[loop];
            particleList.info[loop].ySpeed = setting.ySpeed[loop];
            particleList.info[loop].zSpeed = setting.zSpeed[loop];
            particleList.info[loop].xGravity = setting.xGravity;
            particleList.info[loop].yGravity = setting.yGravity;
            particleList.info[loop].zGravity = setting.zGravity;
            particleList.info[loop].r = setting.r;
            particleList.info[loop].g = setting.g;
            particleList.info[loop].b = setting.b;
            particleList.info[loop].life = setting.life[loop];
            particleList.info[loop].reduce = setting.reduce;
            particleList.info[loop].newParticles = false;
        }
    }

    for (loop = 0; loop < polygonCount; loop++)
    {

        /* Check if particles still live */

        if (particleList.info[loop].life > 0)
        {

            /* Draws all our particles utilizing their current position (textured QUADS) */

            glBegin(GL_QUADS);

            if (wireOptions == false)
            {
                glColor3f(particleList.info[loop].r, particleList.info[loop].g, particleList.info[loop].b);
            }
            else if (wireOptions == true)
            {
                /* Do nothing */
            }

            if (billboard == true)
            {
                glTexCoord2d(0,0);
                glVertex3f(particleList.info[loop].xPosition + (right.m_x + up.m_x) * -0.015, particleList.info[loop].yPosition + (right.m_y + up.m_y) * -0.015, particleList.info[loop].zPosition + (right.m_z + up.m_z) * -0.015);
                glTexCoord2d(1,0);
                glVertex3f(particleList.info[loop].xPosition + (right.m_x - up.m_x) * 0.015, particleList.info[loop].yPosition + (right.m_y - up.m_y) * 0.015, particleList.info[loop].zPosition + (right.m_z - up.m_z) * 0.015);
                glTexCoord2d(1,1);
                glVertex3f(particleList.info[loop].xPosition + (right.m_x + up.m_x) * 0.015, particleList.info[loop].yPosition + (right.m_y + up.m_y) * 0.015, particleList.info[loop].zPosition + (right.m_z + up.m_z) * 0.015);
                glTexCoord2d(0,1);
                glVertex3f(particleList.info[loop].xPosition + (up.m_x - right.m_x) * 0.015, particleList.info[loop].yPosition + (up.m_y - right.m_y) * 0.015, particleList.info[loop].zPosition + (up.m_z - right.m_z) * 0.015);
            }

            if (billboard == false)
            {
                glTexCoord2d(0,0);
                glVertex3f(particleList.info[loop].xPosition - 0.01, particleList.info[loop].yPosition + 0.01, particleList.info[loop].zPosition);
                glTexCoord2d(1,0);
                glVertex3f(particleList.info[loop].xPosition - 0.01, particleList.info[loop].yPosition - 0.01, particleList.info[loop].zPosition);
                glTexCoord2d(1,1);
                glVertex3f(particleList.info[loop].xPosition + 0.01, particleList.info[loop].yPosition - 0.01, particleList.info[loop].zPosition);
                glTexCoord2d(0,1);
                glVertex3f(particleList.info[loop].xPosition + 0.01, particleList.info[loop].yPosition + 0.01, particleList.info[loop].zPosition);
            }

            glEnd();

            /* Sets the direction, speed, color and "life" of our particles (gravity is taken into account) */

            particleList.info[loop].xPosition += particleList.info[loop].xSpeed / 2000;
            particleList.info[loop].yPosition += particleList.info[loop].ySpeed / 2000;
            particleList.info[loop].zPosition += particleList.info[loop].zSpeed / 2000;

            particleList.info[loop].xSpeed += particleList.info[loop].xGravity;
            particleList.info[loop].ySpeed += particleList.info[loop].yGravity;
            particleList.info[loop].zSpeed += particleList.info[loop].zGravity;

            particleList.info[loop].r += setting.colorChangeR;
            particleList.info[loop].g += setting.colorChangeG;
            particleList.info[loop].b += setting.colorChangeB;

            particleList.info[loop].life -= particleList.info[loop].reduce;

            /* Particles are recycled when their "life" reaches 0 */

            if (particleList.info[loop].life <= 0)
            {
                particleList.info[loop].xPosition = setting.xPosition;
                particleList.info[loop].yPosition = setting.yPosition;
                particleList.info[loop].zPosition = setting.zPosition;
                particleList.info[loop].xSpeed = setting.xSpeed[loop];
                particleList.info[loop].ySpeed = setting.ySpeed[loop];
                particleList.info[loop].zSpeed = setting.zSpeed[loop];
                particleList.info[loop].xGravity = setting.xGravity;
                particleList.info[loop].yGravity = setting.yGravity;
                particleList.info[loop].zGravity = setting.zGravity;
                particleList.info[loop].r = setting.r;
                particleList.info[loop].g = setting.g;
                particleList.info[loop].b = setting.b;
                particleList.info[loop].life = setting.lifePersist[loop];
                particleList.info[loop].reduce = setting.reduce;
            }
        }
    }

    /* Reset color to white */

    glColor3f(1.0, 1.0, 1.0);
}
