#include "syl_loader.h"






unsigned char * sImageLoader::loadBitmap(char * m_pFileName, m_struct_BITMAPINFOHEADER * BITMAPINFOHEADER, int heightMap)
{

    /* Open connection to our file, "rb" states that we're reading binary */

    m_pFileHandle = fopen(m_pFileName, "rb");

    /* Error check */

    if (m_pFileHandle == 0)
    {
        printf("Could not open .bmp!\n");
        fclose(m_pFileHandle);
    }

    /* Read the bitmap file header */

    fread(&BITMAPFILEHEADER, sizeof(m_struct_BITMAPFILEHEADER), 1, m_pFileHandle);

    /* Error check */

    if (BITMAPFILEHEADER.bfType != 0x4d42)
    {
        printf("This is not a bitmap!\n");
        fclose(m_pFileHandle);
    }

    /* Read the bitmap info header */

    fread(BITMAPINFOHEADER, sizeof(m_struct_BITMAPINFOHEADER), 1, m_pFileHandle);

    /* Error check */

    if (BITMAPINFOHEADER->biBitCount != 8 && BITMAPINFOHEADER->biBitCount != 24)
    {
        printf("Only 8- and 24-bit images are supported!\n");
        fclose(m_pFileHandle);
    }

    if (BITMAPINFOHEADER->biCompression != 0)
    {
        printf("No compression supported!\n");
        fclose(m_pFileHandle);
    }

    /* 8-bit bitmap */

    if (BITMAPINFOHEADER->biBitCount == 8)
    {

        /* Read the color palette */

        m_pColor = new m_struct_RGBA [256];
        fread(m_pColor, sizeof(m_struct_RGBA), 256, m_pFileHandle);

        /* Assign dynamic memory for the reference- and actual image information (RGBA: width * height * 4-byte) */

        m_pReference = new unsigned char [BITMAPINFOHEADER->biWidth * BITMAPINFOHEADER->biHeight];
        m_pBitmap = new unsigned char [BITMAPINFOHEADER->biWidth * BITMAPINFOHEADER->biHeight * 4];

        /* Move to the image data inside our bitmap skipping any unwanted information */

        fseek(m_pFileHandle, BITMAPFILEHEADER.bfOffBits, SEEK_SET);

        /* Read the bitmap image data */

        fread(m_pReference, 1, BITMAPINFOHEADER->biWidth * BITMAPINFOHEADER->biHeight, m_pFileHandle);

        /* Creates the "real" image using the palette, reference and our allocated space in memory */

        m_byteJump = 0;

        for (m_IndexMemory = 0; m_IndexMemory < BITMAPINFOHEADER->biWidth * BITMAPINFOHEADER->biHeight; m_IndexMemory++)
        {
            m_pBitmap[m_byteJump + 0] = m_pColor[m_pReference[m_IndexMemory]].rgbRed;
            m_pBitmap[m_byteJump + 1] = m_pColor[m_pReference[m_IndexMemory]].rgbGreen;
            m_pBitmap[m_byteJump + 2] = m_pColor[m_pReference[m_IndexMemory]].rgbBlue;
            m_pBitmap[m_byteJump + 3] = m_pColor[m_pReference[m_IndexMemory]].rgbReserved;
            m_byteJump += 4;
        }

        /* Releases the allocated memory */

        delete [] m_pColor;
        delete [] m_pReference;
    }

    /* 24-bit bitmap */

    if (BITMAPINFOHEADER->biBitCount == 24)
    {

        /* Move to the image data inside our bitmap skipping any unwanted information */

        fseek(m_pFileHandle, BITMAPFILEHEADER.bfOffBits, SEEK_SET);

        /* Create dynamic memory in order to store our image; width * height * 3-byte (3 * 8-bit = 24-bit = 3-byte) */

        m_pBitmap = new unsigned char [BITMAPINFOHEADER->biWidth * BITMAPINFOHEADER->biHeight * 3];

        /* Read the bitmap image data */

        fread(m_pBitmap, 1, BITMAPINFOHEADER->biWidth * BITMAPINFOHEADER->biHeight * 3, m_pFileHandle);

        /* Swap RGB values so we can see how the colors really look - after we're done: BGR */

        for (m_IndexMemory = 0; m_IndexMemory < BITMAPINFOHEADER->biWidth * BITMAPINFOHEADER->biHeight * 3; m_IndexMemory += 3)
        {
            m_tempRGB = m_pBitmap[m_IndexMemory];
            m_pBitmap[m_IndexMemory] = m_pBitmap[m_IndexMemory + 2];
            m_pBitmap[m_IndexMemory + 2] = m_tempRGB;
        }

        /* Calculate greyscale and then generate a heightmap */

        if (heightMap == 1)
        {
            m_pHeightmap = new float [BITMAPINFOHEADER->biWidth * BITMAPINFOHEADER->biHeight];

            m_byteJump = 0;

            for (m_IndexMemory = 0; m_IndexMemory < BITMAPINFOHEADER->biWidth * BITMAPINFOHEADER->biHeight; m_IndexMemory++)
            {
                m_pHeightmap[m_IndexMemory] = ((0.2990 * m_pBitmap[m_byteJump] + 0.5870 * m_pBitmap[m_byteJump + 1] + 0.1140 * m_pBitmap[m_byteJump + 2]) / 256.0);
                m_byteJump += 3;
            }
        }
    }

    /* Close connection to our bitmap */

    fclose(m_pFileHandle);

    /* Returns pointer to our image data */

    return m_pBitmap;
}






int sImageLoader::initializeTexture(char * m_pFileName, int heightMap)
{

    /* Add 1 to our current image ID */

    m_imageID++;

    /* Call of the bitmap function and specification of a new pointer that holds connection to our image data */

    m_pBitmapContainer = loadBitmap(m_pFileName, &BITMAPINFOHEADERArg, heightMap);

    /* Binds the texture to our image ID so we can call the texture using this number */

    glBindTexture(GL_TEXTURE_2D, m_imageID);

    /* Sets some texture parameters needed to display the bitmap correctly */

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    /* Enables proper lighting for our textures */

    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    /* Creates the texture using our image data stored in memory, width and height are taken from the info header */

    if (BITMAPINFOHEADERArg.biBitCount == 8)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, BITMAPINFOHEADERArg.biWidth, BITMAPINFOHEADERArg.biHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_pBitmapContainer);
        gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, BITMAPINFOHEADERArg.biWidth, BITMAPINFOHEADERArg.biHeight, GL_RGBA, GL_UNSIGNED_BYTE, m_pBitmapContainer);
    }

    if (BITMAPINFOHEADERArg.biBitCount == 24)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, BITMAPINFOHEADERArg.biWidth, BITMAPINFOHEADERArg.biHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, m_pBitmapContainer);
        gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, BITMAPINFOHEADERArg.biWidth, BITMAPINFOHEADERArg.biHeight, GL_RGB, GL_UNSIGNED_BYTE, m_pBitmapContainer);
    }

    /* Releases the allocated memory */

    delete [] m_pBitmap;

    /* Returns the image ID for usage within our program */

    return m_imageID;
}






void sObjectLoader::loadObject (char * m_pFileName, float scale, m_struct_OBJECT & OBJECT)
{

    /* Reset all our values to 0 */

    m_vertexCount = 0;
    m_textureCoordCount = 0;
    m_normalCount = 0;
    m_vertexOffset = 0;
    m_textureCoordOffset = 0;
    m_normalOffset = 0;
    m_faceOffset = 0;
    loopCount = 0;
    objectIndex = 0;

    /* Open connection to our file, "r" states that we're reading text */

    m_pFileHandle = fopen(m_pFileName, "r");

    /* Error check */

    if (m_pFileHandle == 0)
    {
        printf("Could not open .obj!\n");
        fclose(m_pFileHandle);
    }

    /* Counts the vertices, texture coordinates, normals and faces to know how much memory we need to allocate - eof = end of file */

    while (!feof(m_pFileHandle))
    {

        /* Stores the whole line in an array until 100 characters are read or a newline is reached */

        fgets(readLine, 100, m_pFileHandle);

        /* A zero value indicates that the characters compared in both strings are all equal */

        if (strncmp("v ", readLine, 2) == 0)
        {
            m_vertexCount++;
        }

        if (strncmp("vt ", readLine, 3) == 0)
        {
            m_textureCoordCount++;
        }

        if (strncmp("vn ", readLine, 3) == 0)
        {
            m_normalCount++;
        }

        if (strncmp("f ", readLine, 2) == 0)
        {
            OBJECT.m_faceCount += 3;
        }
    }

    /* Create dynamic memory in order to store our 3D object data */

    m_pVertex = new sVector [m_vertexCount];
    m_pTextureCoord = new sTexture [m_textureCoordCount];
    m_pNormal = new sVector [m_normalCount];

    m_pVertexIndex = new int [OBJECT.m_faceCount];
    m_pTextureCoordIndex = new int [OBJECT.m_faceCount];
    m_pNormalIndex = new int [OBJECT.m_faceCount];

    OBJECT.m_pVertex = new float [OBJECT.m_faceCount * 3];
    OBJECT.m_pTextureCoord = new float [OBJECT.m_faceCount * 2];
    OBJECT.m_pNormal = new float [OBJECT.m_faceCount * 3];

    /* Move back to the beginning of our file */

    fseek(m_pFileHandle, 0, SEEK_SET);

    /* Scans through the wavefront object again and stores all data in different arrays */

    while (!feof(m_pFileHandle))
    {
        fgets(readLine, 100, m_pFileHandle);

        if (!strncmp("v ", readLine, 2))
        {
            sscanf(readLine, "v %f %f %f", &m_pVertex[m_vertexOffset].m_x, &m_pVertex[m_vertexOffset].m_y, &m_pVertex[m_vertexOffset].m_z);
            m_vertexOffset++;
        }

        if (!strncmp("vt ", readLine, 3))
        {
            sscanf(readLine, "vt %f %f", &m_pTextureCoord[m_textureCoordOffset].m_u, &m_pTextureCoord[m_textureCoordOffset].m_v);
            m_textureCoordOffset++;
        }

        if (!strncmp("vn ", readLine, 3))
        {
            sscanf(readLine, "vn %f %f %f", &m_pNormal[m_normalOffset].m_x, &m_pNormal[m_normalOffset].m_y, &m_pNormal[m_normalOffset].m_z);
            m_normalOffset++;
        }

        if (!strncmp("f ", readLine, 2))
        {
            sscanf(readLine, "f %d/%d/%d %d/%d/%d %d/%d/%d", &m_pVertexIndex[m_faceOffset], &m_pTextureCoordIndex[m_faceOffset], &m_pNormalIndex[m_faceOffset], &m_pVertexIndex[m_faceOffset + 1], &m_pTextureCoordIndex[m_faceOffset + 1], &m_pNormalIndex[m_faceOffset + 1], &m_pVertexIndex[m_faceOffset + 2], &m_pTextureCoordIndex[m_faceOffset + 2], &m_pNormalIndex[m_faceOffset + 2]);
            m_faceOffset += 3;
        }
    }

    /* Close connection to our file */

    fclose(m_pFileHandle);

    /* Loops through the arrays and creates a structure using the indicies as well as other data collected in order to later display the 3D model using OpenGL */

    for (loopCount = 0; loopCount < OBJECT.m_faceCount; loopCount++)
    {
        OBJECT.m_pVertex[objectIndex] = m_pVertex[m_pVertexIndex[loopCount] - 1].m_x * scale;
        OBJECT.m_pVertex[objectIndex + 1] = m_pVertex[m_pVertexIndex[loopCount] - 1].m_y * scale;
        OBJECT.m_pVertex[objectIndex + 2] = m_pVertex[m_pVertexIndex[loopCount] - 1].m_z * scale;
        objectIndex += 3;
    }

    objectIndex = 0;

    for (loopCount = 0; loopCount < OBJECT.m_faceCount; loopCount++)
    {
        OBJECT.m_pTextureCoord[objectIndex] = m_pTextureCoord[m_pTextureCoordIndex[loopCount] - 1].m_u;
        OBJECT.m_pTextureCoord[objectIndex + 1] = m_pTextureCoord[m_pTextureCoordIndex[loopCount] - 1].m_v;
        objectIndex += 2;
    }

    objectIndex = 0;

    for (loopCount = 0; loopCount < OBJECT.m_faceCount; loopCount++)
    {
        OBJECT.m_pNormal[objectIndex] = m_pNormal[m_pNormalIndex[loopCount] - 1].m_x;
        OBJECT.m_pNormal[objectIndex + 1] = m_pNormal[m_pNormalIndex[loopCount] - 1].m_y;
        OBJECT.m_pNormal[objectIndex + 2] = m_pNormal[m_pNormalIndex[loopCount] - 1].m_z;
        objectIndex += 3;
    }

    /* Releases the allocated memory */

    delete [] m_pVertex;
    delete [] m_pTextureCoord;
    delete [] m_pNormal;

    delete [] m_pVertexIndex;
    delete [] m_pTextureCoordIndex;
    delete [] m_pNormalIndex;
}
