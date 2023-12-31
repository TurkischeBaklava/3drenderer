#include "matrix.h"
#include <math.h>

mat4_t mat4_identity(void)
{
    mat4_t m = { {
      // 1  2  3  4 column
        {1, 0, 0, 0}, // 1
        {0, 1, 0, 0}, // 2
        {0, 0, 1, 0}, // 3
        {0, 0, 0, 1}  // 4 row
        } };
    return m;
}

mat4_t mat4_make_scale(float sx, float sy, float sz)
{
    mat4_t m = mat4_identity();
    m.m[0][0] = sx;
    m.m[1][1] = sy;
    m.m[2][2] = sz;
    return m;
}

vec4_t mat4_mul_vec4(mat4_t m, vec4_t v)
{
    vec4_t result;
    result.x = m.m[0][0] * v.x + m.m[0][1] * v.y + m.m[0][2] * v.z + m.m[0][3] * v.w;
    result.y = m.m[1][0] * v.x + m.m[1][1] * v.y + m.m[1][2] * v.z + m.m[1][3] * v.w;
    result.z = m.m[2][0] * v.x + m.m[2][1] * v.y + m.m[2][2] * v.z + m.m[2][3] * v.w;
    result.w = m.m[3][0] * v.x + m.m[3][1] * v.y + m.m[3][2] * v.z + m.m[3][3] * v.w;
    return result;
}

mat4_t mat4_mul_mat4(mat4_t m1, mat4_t m2)
{
    mat4_t m = {0};
    int i, j, k;
    for (i = 0; i < 4; i++)
    {
        for (j = 0; j < 4; j++)
        {
            for (k = 0; k < 4; k++)
            {
                m.m[i][j] += m1.m[i][k] * m2.m[k][j];
            }
        }
    }
    return m;
}

mat4_t mat4_make_translation(float tx, float ty, float tz)
{
    mat4_t m = mat4_identity();
    m.m[0][3] = tx;
    m.m[1][3] = ty;
    m.m[2][3] = tz;
    return m;
}

mat4_t mat4_make_rotation_x(float angle)
{
    float s = sin(angle);
    float c = cos(angle);

    mat4_t m = mat4_identity();
    // | 1  0  0  0 |
    // | 0  c -s  0 |
    // | 0  s  c  0 |
    // | 0  0  0  1 |

    m.m[1][1] = c;
    m.m[1][2] = -s;
    m.m[2][1] = s;
    m.m[2][2] = c;
    return m;
}

mat4_t mat4_make_rotation_y(float angle)
{
    float s = sin(angle);
    float c = cos(angle);

    mat4_t m = mat4_identity();
    // | c  0  s  0 |   There is a special case for the y axis rotation
    // | 0  1  0  0 |
    // |-s  0  c  0 |
    // | 0  0  0  1 |

    m.m[0][0] = c;
    m.m[0][2] = s;
    m.m[2][0] = -s;
    m.m[2][2] = c;
    return m;
}

mat4_t mat4_make_rotation_z(float angle)
{
    float s = sin(angle);
    float c = cos(angle);

    mat4_t m = mat4_identity();
    // | c -s  0  0 |
    // | s  c  0  0 |
    // | 0  0  1  0 |
    // | 0  0  0  1 |

    m.m[0][0] = c;
    m.m[0][1] = -s;
    m.m[1][0] = s;
    m.m[1][1] = c;
    return m;
}

mat4_t mat4_make_perspective(float fov, float aspect, float znear, float zfar)
{
    mat4_t m = { {{0}} };
    // (h/w)*1/tan(fov/2)     0             0               0
    //    0              1/tan(fov/2)       0               0
    //    0                   0          zf / (zf - zn)     - (zn * zf) / (zf - zn)
    //    0                   0             1               0

    m.m[0][0] = aspect * (1 / tan(fov / 2));
    m.m[1][1] = 1 / tan(fov / 2);
    m.m[2][2] = zfar / (zfar - znear);
    m.m[2][3] = (-znear * zfar) / (zfar - znear);
    m.m[3][2] = 1.0f; // We set this 1 here to store our original z value to do perspective divide
    return m;
}

vec4_t mat4_mult_vec4_project(mat4_t project_matrix, vec4_t v)
{
    vec4_t result = mat4_mul_vec4(project_matrix, v);

    // Perspective divide
    if (result.w != 0.0)
    {
        result.x /= result.w;
        result.y /= result.w;
        result.z /= result.w;
    }
    return result;
}

