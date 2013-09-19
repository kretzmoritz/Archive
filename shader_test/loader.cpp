#include "loader.h"

cLoader::cLoader()
{
    m_iTexture = 0;
}

bool cLoader::checkLittleEndian()
{
    int num = 1;
    bool result = false;

    if(*(char *)&num == 1)
    {
        result = true;
    }

    return result;
}

int cLoader::byteToInt(unsigned char *input)
{
    int value = 0;

    if(checkLittleEndian())
    {
        value = (value << 8) + input[0];
        value = (value << 8) + input[1];
        value = (value << 8) + input[2];
        value = (value << 8) + input[3];
    }
    else
    {
        value = (value << 8) + input[3];
        value = (value << 8) + input[2];
        value = (value << 8) + input[1];
        value = (value << 8) + input[0];
    }

    return value;
}

void cLoader::initGlewLib()
{
    GLenum err = glewInit();

    if (GLEW_OK != err)
    {
        fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
    }
    else
    {
        fprintf(stdout, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));
    }

    if(GLEW_VERSION_2_0)
    {
        printf("OpenGL version 2.0 or higher found!");
    }
    else
    {
        printf("Your OpenGL version is lower than 2.0!");
    }

    std::cout << std::endl;
}

char * cLoader::readShader(char *input)
{
    std::ifstream fileReader;
    char *content = NULL;

    int length = 0;

    if (input != NULL)
	{
	    fileReader.open(input, std::ios::binary);

	    if(fileReader.is_open())
	    {
	        fileReader.seekg(0, std::ios::end);
            length = fileReader.tellg();
            fileReader.seekg(0, std::ios::beg);

            if(length > 0)
            {
                content = new char[length + 1];
                fileReader.read(content, length);
                content[length] = '\0';
            }

            fileReader.close();
	    }
	}

    return content;
}

GLuint cLoader::loadShader(char *input_vert, char *input_frag)
{
	char *vs_plain = NULL;
	char *fs_plain = NULL;

    GLuint vertex = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragment = glCreateShader(GL_FRAGMENT_SHADER);
    GLuint program = glCreateProgram();

	vs_plain = readShader(input_vert);
	fs_plain = readShader(input_frag);

	const char * const_vs_plain = vs_plain;
	const char * const_fs_plain = fs_plain;

	glShaderSource(vertex, 1, &const_vs_plain, NULL);
	glShaderSource(fragment, 1, &const_fs_plain, NULL);

    // Release memory properly

	delete [] vs_plain;
	delete [] fs_plain;

	vs_plain = NULL;
	fs_plain = NULL;
	const_vs_plain = NULL;
	const_fs_plain = NULL;

	glCompileShader(vertex);
	glCompileShader(fragment);

	glAttachShader(program, vertex);
	glAttachShader(program, fragment);

    glLinkProgram(program);

	GLint success = GL_FALSE;
	glGetProgramiv(program, GL_LINK_STATUS, &success);

	if(success == GL_FALSE)
    {
        GLchar *errorLog = new GLchar[1024];
        glGetProgramInfoLog(program, 1024, NULL, errorLog);
        std::cout << errorLog << std::endl;
        delete [] errorLog;
        errorLog = NULL;

        program = 0;
    }

    return program;
}

int cLoader::loadPNGLib(char *input)
{
    std::ifstream fileReader;
    char *content = NULL;
    unsigned char *p_result = NULL;
    std::vector<unsigned char> result;
    std::vector<unsigned char> reverse;
    unsigned long w, h;
    int value = 0;

    unsigned long length = 0;

    if (input != NULL)
	{
	    fileReader.open(input, std::ios::binary);

	    if(fileReader.is_open())
	    {
	        fileReader.seekg(0, std::ios::end);
            length = fileReader.tellg();
            fileReader.seekg(0, std::ios::beg);

            if(length > 0)
            {
                content = new char[length];
                fileReader.read(content, length);
            }

            fileReader.close();
	    }
	}

	int error = decodePNG(result, w, h, (const unsigned char*)content, length);

    delete [] content;
    content = NULL;

	if(error != 0)
	{
	    std::cout << "Error: " << error << std::endl;
	}
	else
	{
	    std::cout << "PNG loading successful..." << std::endl;

        // Reverse top and bottom

        reverse.resize(result.size());

        for(int i = 0; i < (int)h; i++)
        {
            for(int j = 0; j < (int)w * 4; j++)
            {
                reverse[((h - i - 1) * w * 4) + j] = result[(i * w * 4) + j];
            }
        }

        result.clear();
        result = reverse;
        reverse.clear();

        p_result = &result[0];
        value = initTexture(p_result, w, h);

        p_result = NULL;
	}

    result.clear();

    return value;
}

int cLoader::initTexture(unsigned char *input, int w, int h)
{
    m_iTexture++;
    glBindTexture(GL_TEXTURE_2D, m_iTexture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, input);
    gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, w, h, GL_RGBA, GL_UNSIGNED_BYTE, input);

    std::cout << "Texture creation achieved..." << std::endl;

    return m_iTexture;
}

void cLoader::loadOBJ(char *input)
{
    int vertexCount = 0;
    int textureCount = 0;
    int normalCount = 0;
    int faceCount = 0;

    int vertexOffset = 0;
    int textureOffset = 0;
    int normalOffset = 0;
    int faceOffset = 0;

    int objectIndex = 0;

    char readLine[100];

    FILE *fileReader = fopen(input, "r");

    // Error check

    if(fileReader == 0)
    {
        std::cout << "Could not open .obj!" << std::endl;
        fclose(fileReader);
    }
    else
    {
        while(!feof(fileReader))
        {
            fgets(readLine, 100, fileReader);

            if(strncmp("v ", readLine, 2) == 0)
            {
                vertexCount++;
            }
            else if(strncmp("vt ", readLine, 3) == 0)
            {
                textureCount++;
            }
            else if(strncmp("vn ", readLine, 3) == 0)
            {
                normalCount++;
            }
            else if(strncmp("f ", readLine, 2) == 0)
            {
                faceCount += 3;
            }
        }

        if(faceCount > 0)
        {
            delete [] model.vertex;
            delete [] model.const_vertex;
            delete [] model.texture;
            delete [] model.normal;

            model.faceCount = faceCount;

            VECTOR *vertexRAW = new VECTOR [vertexCount];
            TEXTURE *textureRAW = new TEXTURE [textureCount];
            VECTOR *normalRAW = new VECTOR [normalCount];

            int *vertex_index = new int [model.faceCount];
            int *texture_index = new int [model.faceCount];
            int *normal_index = new int [model.faceCount];

            model.vertex = new float [model.faceCount * 3];
            model.const_vertex = new float [model.faceCount * 3];
            model.texture = new float [model.faceCount * 2];
            model.normal = new float [model.faceCount * 3];

            fseek(fileReader, 0, SEEK_SET);

            while(!feof(fileReader))
            {
                fgets(readLine, 100, fileReader);

                if(!strncmp("v ", readLine, 2))
                {
                    sscanf(readLine, "v %f %f %f", &vertexRAW[vertexOffset].x, &vertexRAW[vertexOffset].y, &vertexRAW[vertexOffset].z);
                    vertexOffset++;
                }
                else if(!strncmp("vt ", readLine, 3))
                {
                    sscanf(readLine, "vt %f %f", &textureRAW[textureOffset].u, &textureRAW[textureOffset].v);
                    textureOffset++;
                }
                else if(!strncmp("vn ", readLine, 3))
                {
                    sscanf(readLine, "vn %f %f %f", &normalRAW[normalOffset].x, &normalRAW[normalOffset].y, &normalRAW[normalOffset].z);
                    normalOffset++;
                }
                else if(!strncmp("f ", readLine, 2))
                {
                    sscanf(readLine, "f %d/%d/%d %d/%d/%d %d/%d/%d", &vertex_index[faceOffset], &texture_index[faceOffset], &normal_index[faceOffset], &vertex_index[faceOffset + 1], &texture_index[faceOffset + 1],
                    &normal_index[faceOffset + 1], &vertex_index[faceOffset + 2], &texture_index[faceOffset + 2], &normal_index[faceOffset + 2]);
                    faceOffset += 3;
                }
            }

            fclose(fileReader);

            for(int i = 0; i < model.faceCount; i++)
            {
                model.vertex[objectIndex] = vertexRAW[vertex_index[i] - 1].x;
                model.vertex[objectIndex + 1] = vertexRAW[vertex_index[i] - 1].y;
                model.vertex[objectIndex + 2] = vertexRAW[vertex_index[i] - 1].z;

                model.const_vertex[objectIndex] = vertexRAW[vertex_index[i] - 1].x;
                model.const_vertex[objectIndex + 1] = vertexRAW[vertex_index[i] - 1].y;
                model.const_vertex[objectIndex + 2] = vertexRAW[vertex_index[i] - 1].z;

                model.normal[objectIndex] = normalRAW[normal_index[i] - 1].x;
                model.normal[objectIndex + 1] = normalRAW[normal_index[i] - 1].y;
                model.normal[objectIndex + 2] = normalRAW[normal_index[i] - 1].z;

                objectIndex += 3;
            }

            objectIndex = 0;

            for(int i = 0; i < model.faceCount; i++)
            {
                model.texture[objectIndex] = textureRAW[texture_index[i] - 1].u;
                model.texture[objectIndex + 1] = textureRAW[texture_index[i] - 1].v;

                objectIndex += 2;
            }

            // Releases the allocated memory

            delete [] vertexRAW;
            delete [] textureRAW;
            delete [] normalRAW;

            delete [] vertex_index;
            delete [] texture_index;
            delete [] normal_index;
        }
    }
}

/* --- NOT FINISHED --- */

unsigned char * cLoader::loadPNG(char *input)
{
    std::ifstream fileReader;
    char *content = NULL;
    unsigned char *result = NULL;

    int length = 0;

    if (input != NULL)
	{
	    fileReader.open(input, std::ios::binary);

	    if(fileReader.is_open())
	    {
	        fileReader.seekg(0, std::ios::end);
            length = fileReader.tellg();
            fileReader.seekg(0, std::ios::beg);

            if(length > 0)
            {
                content = new char[length];
                fileReader.read(content, length);
            }

            fileReader.close();
	    }
	}

    if(length > 0)
    {
        unsigned char *colorPalette;

        std::vector<unsigned char> image;
        image.clear();

        unsigned char aByte_header[8];
        unsigned char aByte_length[4];
        unsigned char aByte_name[4];
        unsigned char aByte_width[4];
        unsigned char aByte_height[4];

        int index = 0;

        int chunk_length = 0;
        int width = 0;
        int height = 0;
        int bitDepth = 0;
        int colorType = 0;
        int compression = 0;
        int filter = 0;
        int interlaced = 0;

        // Read the header

        while(index < 8)
        {
            aByte_header[0] = content[index];
            aByte_header[1] = content[index + 1];
            aByte_header[2] = content[index + 2];
            aByte_header[3] = content[index + 3];
            aByte_header[4] = content[index + 4];
            aByte_header[5] = content[index + 5];
            aByte_header[6] = content[index + 6];
            aByte_header[7] = content[index + 7];

            if(aByte_header[0] != 137 || aByte_header[1] != 80 || aByte_header[2] != 78 || aByte_header[3] != 71 || aByte_header[4] != 13 || aByte_header[5] != 10 || aByte_header[6] != 26 || aByte_header[7] != 10)
            {
                std::cout << std::endl << "Error! Wrong file format...";
            }

            index += 8;
        }

        // Read the rest of the file

        while(index < length)
        {
            aByte_length[0] = content[index];
            aByte_length[1] = content[index + 1];
            aByte_length[2] = content[index + 2];
            aByte_length[3] = content[index + 3];

            chunk_length = byteToInt(aByte_length);

            aByte_name[0] = content[index + 4];
            aByte_name[1] = content[index + 5];
            aByte_name[2] = content[index + 6];
            aByte_name[3] = content[index + 7];

            if(aByte_name[0] == 'I' && aByte_name[1] == 'H' && aByte_name[2] == 'D' && aByte_name[3] == 'R')
            {
                aByte_width[0] = content[index + 8];
                aByte_width[1] = content[index + 9];
                aByte_width[2] = content[index + 10];
                aByte_width[3] = content[index + 11];

                width = byteToInt(aByte_width);

                aByte_height[0] = content[index + 12];
                aByte_height[1] = content[index + 13];
                aByte_height[2] = content[index + 14];
                aByte_height[3] = content[index + 15];

                height = byteToInt(aByte_height);

                bitDepth = content[index + 16];
                colorType = content[index + 17];
                compression = content[index + 18];
                filter = content[index + 19];
                interlaced = content[index + 20];

                if(colorType == 0 || colorType == 1)
                {
                    result = new unsigned char[width * height * 3];
                }
                else if(colorType == 3)
                {
                    result = new unsigned char[width * height];
                }
                else
                {
                    result = new unsigned char[width * height * 4];
                }
            }
            else if(aByte_name[0] == 'P' && aByte_name[1] == 'L' && aByte_name[2] == 'T' && aByte_name[3] == 'E' && colorType == 3)
            {
                colorPalette = new unsigned char[chunk_length];

                for(int i = 0; i < chunk_length; i++)
                {
                    colorPalette[i] = content[index + 8 + i];
                }
            }
            else if(aByte_name[0] == 'I' && aByte_name[1] == 'D' && aByte_name[2] == 'A' && aByte_name[3] == 'T')
            {
                for(int i = 0; i < chunk_length; i++)
                {
                    image.push_back(content[index + 8 + i]);
                }
            }
            else if(aByte_name[0] == 'I' && aByte_name[1] == 'E' && aByte_name[2] == 'N' && aByte_name[3] == 'D')
            {
                index = length;
            }

            index += 8 + chunk_length + 4;
        }
    }

    return result;
}

/* --- NOT FINISHED --- */
