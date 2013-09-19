#include "syl_math.h"






/* sVector constructor - sVector name (1, 2, 3) */

sVector::sVector (float a, float b, float c)
{
    m_x = a;
    m_y = b;
    m_z = c;
}






/* sTexture constructor - sTexture name (1, 2, 3) */

sTexture::sTexture (float a, float b)
{
    m_u = a;
    m_v = b;
}






/* Vector addition */

sVector sVector::operator + (sVector m_vArg)
{
    sVector m_vTemp;

    m_vTemp.m_x = m_x + m_vArg.m_x;
    m_vTemp.m_y = m_y + m_vArg.m_y;
    m_vTemp.m_z = m_z + m_vArg.m_z;

    return m_vTemp;
}






/* Vector scalar multiplication */

sVector sVector::operator * (float a)
{
    sVector m_vTemp;

    m_vTemp.m_x = m_x * a;
    m_vTemp.m_y = m_y * a;
    m_vTemp.m_z = m_z * a;

    return m_vTemp;
}






/* Calculate normal vectors for a polygon and normalize (unit length) them */

float * sVector::normalUnit(float input[], int size, int corner, int terrainCalc)
{
    int m_loop;
    int m_arrayCopy = 0;
    int m_countVector = 0;
    int m_countNormal = 0;

    sVector m_vContainerA;
    sVector m_vContainerB;

    float m_crossproduct;
    vector<float> m_outputTemp;

    float m_vectorLength;

    float * m_output = new float [size];

    for (m_loop = 0; m_loop < size / (3 * corner); m_loop++)
    {

        m_vContainerA.m_x = input[m_countVector] - input[m_countVector + 3];
        m_vContainerA.m_y = input[m_countVector + 1] - input[m_countVector + 4];
        m_vContainerA.m_z = input[m_countVector + 2] - input[m_countVector + 5];

        m_vContainerB.m_x = input[m_countVector + 3] - input[m_countVector + 6];
        m_vContainerB.m_y = input[m_countVector + 4] - input[m_countVector + 7];
        m_vContainerB.m_z = input[m_countVector + 5] - input[m_countVector + 8];

        for (m_arrayCopy = 0; m_arrayCopy < corner; m_arrayCopy++)
        {
            m_crossproduct = (m_vContainerA.m_y * m_vContainerB.m_z) - (m_vContainerA.m_z * m_vContainerB.m_y);
            m_outputTemp.push_back(m_crossproduct);

            m_crossproduct = (m_vContainerA.m_z * m_vContainerB.m_x) - (m_vContainerA.m_x * m_vContainerB.m_z);
            m_outputTemp.push_back(m_crossproduct);

            m_crossproduct = (m_vContainerA.m_x * m_vContainerB.m_y) - (m_vContainerA.m_y * m_vContainerB.m_x);
            m_outputTemp.push_back(m_crossproduct);
        }

        m_countVector += (3 * corner);
    }

    m_countVector = 0;

    for (m_loop = 0; m_loop < size / (3 * corner); m_loop++)
    {
        m_vectorLength = sqrt((m_outputTemp[m_countVector] * m_outputTemp[m_countVector]) + (m_outputTemp[m_countVector + 1] * m_outputTemp[m_countVector + 1]) + (m_outputTemp[m_countVector + 2] * m_outputTemp[m_countVector + 2]));

        if (m_vectorLength == 0)
        {
            m_vectorLength = 1;
        }

        m_countVector += 3;

        if (terrainCalc == 0)
        {
            for (m_arrayCopy = 0; m_arrayCopy < corner; m_arrayCopy++)
            {
                m_output[m_countNormal] = m_outputTemp[m_countNormal] / m_vectorLength;
                m_output[m_countNormal + 1] = m_outputTemp[m_countNormal + 1] / m_vectorLength;
                m_output[m_countNormal + 2] = m_outputTemp[m_countNormal + 2] / m_vectorLength;

                m_countNormal += 3;
            }
        }

        if (terrainCalc == 1)
        {
            for (m_arrayCopy = 0; m_arrayCopy < corner; m_arrayCopy++)
            {
                m_output[m_countNormal] = 0;
                m_output[m_countNormal + 1] = 1;
                m_output[m_countNormal + 2] = 0;

                m_countNormal += 3;
            }
        }
    }

    return m_output;
}






/* Creation of the identity matrix */

sMatrix::sMatrix()
{

    /* Set every matrix component to 0 */

    memset(this, 0, sizeof(sMatrix));

    /* Set diagonal components to 1 */

    m_main[0][0] = 1.0;
    m_main[1][1] = 1.0;
    m_main[2][2] = 1.0;
    m_main[3][3] = 1.0;
}






/* Matrix multiplication */

sMatrix sMatrix::operator * (sMatrix m_MatrixArg)
{
    sMatrix m_MatrixTemp;

    for (int row = 0; row < 4; row++)
    {
        for (int column = 0; column < 4; column++)
        {
            float container = 0.0;
            container += (m_main[0][row] * m_MatrixArg.m_main[column][0]);
            container += (m_main[1][row] * m_MatrixArg.m_main[column][1]);
            container += (m_main[2][row] * m_MatrixArg.m_main[column][2]);
            container += (m_main[3][row] * m_MatrixArg.m_main[column][3]);
            m_MatrixTemp.m_main[column][row] = container;
        }
    }

    return m_MatrixTemp;
}






/* ModelView matrix - model part */

sMatrix sMatrix::fillModel(float x, float y, float z)
{
    sMatrix m_MatrixModel;

    m_MatrixModel.m_main[3][0] = x;
    m_MatrixModel.m_main[3][1] = y;
    m_MatrixModel.m_main[3][2] = z;

    return m_MatrixModel;
}






/* ModelView matrix - view part */

sMatrix sMatrix::fillView(sVector & m_vArgPosition, sVector & m_vArgX, sVector & m_vArgY, sVector & m_vArgZ)
{
    sMatrix m_MatrixView;

    m_MatrixView.m_main[0][0] = m_vArgX.m_x;   m_MatrixView.m_main[1][0] = m_vArgX.m_y;    m_MatrixView.m_main[2][0] = m_vArgX.m_z;    m_MatrixView.m_main[3][0] = 0.0;
    m_MatrixView.m_main[0][1] = m_vArgY.m_x;   m_MatrixView.m_main[1][1] = m_vArgY.m_y;    m_MatrixView.m_main[2][1] = m_vArgY.m_z;    m_MatrixView.m_main[3][1] = 0.0;
    m_MatrixView.m_main[0][2] = m_vArgZ.m_x;   m_MatrixView.m_main[1][2] = m_vArgZ.m_y;    m_MatrixView.m_main[2][2] = m_vArgZ.m_z;    m_MatrixView.m_main[3][2] = 0.0;
    m_MatrixView.m_main[0][3] = 0.0;           m_MatrixView.m_main[1][3] = 0.0;            m_MatrixView.m_main[2][3] = 0.0;            m_MatrixView.m_main[3][3] = 1.0;

    /* In order to transform the camera, you need to move the whole scene with the inverse transformation. */

    sMatrix m_MatrixTemp = fillModel(-(m_vArgPosition.m_x), -(m_vArgPosition.m_y), -(m_vArgPosition.m_z));

    /* Matrix multiplication of model- and view matrices in order to create a full ModelView matrix */

    m_MatrixView = m_MatrixView * m_MatrixTemp;

    return m_MatrixView;
}
