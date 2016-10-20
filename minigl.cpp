/**
 * minigl.cpp
 * -------------------------------
 * Implement miniGL here.
 *
 * You may include minigl.h and any of the standard C++ libraries.
 * No other includes are permitted.  Other preprocessing directives
 * are also not permitted.  These requirements are strictly
 * enforced.  Be sure to run a test grading to make sure your file
 * passes the sanity tests.
 */

#include "minigl.h"
#include <iostream>
#include <algorithm>
#include <cassert>
#include <cmath>
#include <vector>
#include <list>
#include <cstdio>
using namespace std;

// Instructor Includes

//Vec3.h

#ifndef __vec__
#define __vec__

#include <cmath>
#include <iostream>
#include <cassert>

template<class T, int n> struct vec;
template<class T, int n> T dot(const vec<T,n>& u,const vec<T,n>& v);

template<class T, int n>
struct vec
{
    T x[n];
    
    vec()
    {make_zero();}
    
    vec(const T& a)
    {assert(n == 1);x[0]=a;}
    
    vec(const T& a, const T& b)
    {assert(n == 2);x[0]=a;x[1]=b;}
    
    vec(const T& a, const T& b, const T& c)
    {assert(n == 3);x[0]=a;x[1]=b;x[2]=c;}
    
    void make_zero()
    {for(int i = 0; i < n; i++) x[i] = 0;}
    
    vec& operator += (const vec& v)
    {for(int i = 0; i < n; i++) x[i] += v.x[i]; return *this;}
    
    vec& operator -= (const vec& v)
    {for(int i = 0; i < n; i++) x[i] -= v.x[i]; return *this;}
    
    vec& operator *= (const vec& v)
    {for(int i = 0; i < n; i++) x[i] *= v.x[i]; return *this;}
    
    vec& operator /= (const vec& v)
    {for(int i = 0; i < n; i++) x[i] /= v.x[i]; return *this;}
    
    vec& operator *= (const T& c)
    {for(int i = 0; i < n; i++) x[i] *= c; return *this;}
    
    vec& operator /= (const T& c)
    {for(int i = 0; i < n; i++) x[i] /= c; return *this;}
    
    vec operator + () const
    {return *this;}
    
    vec operator - () const
    {vec r; for(int i = 0; i < n; i++) r[i] = -x[i]; return r;}
    
    vec operator + (const vec& v) const
    {vec r; for(int i = 0; i < n; i++) r[i] = x[i] + v.x[i]; return r;}
    
    vec operator - (const vec& v) const
    {vec r; for(int i = 0; i < n; i++) r[i] = x[i] - v.x[i]; return r;}
    
    vec operator * (const vec& v) const
    {vec r; for(int i = 0; i < n; i++) r[i] = x[i] * v.x[i]; return r;}
    
    vec operator / (const vec& v) const
    {vec r; for(int i = 0; i < n; i++) r[i] = x[i] / v.x[i]; return r;}
    
    vec operator * (const T& c) const
    {vec r; for(int i = 0; i < n; i++) r[i] = x[i] * c; return r;}
    
    vec operator / (const T& c) const
    {vec r; for(int i = 0; i < n; i++) r[i] = x[i] / c; return r;}
    
    const T& operator[] (int i) const
    {return x[i];}
    
    T& operator[] (int i)
    {return x[i];}
    
    T magnitude_squared() const
    {return dot(*this, *this);}
    
    T magnitude() const
    {return sqrt(magnitude_squared());}
    
    // Be careful to handle the zero vector gracefully
    vec normalized() const
    {T mag = magnitude(); if(mag) return *this / mag; vec r; r[0] = 1; return r;};
};

template <class T, int n>
vec<T,n> operator * (const T& c, const vec<T,n>& v)
{return v*c;}

template <class T, int n>
T dot(const vec<T,n> & u, const vec<T,n> & v)
{
    T r  =  0;
    for(int i = 0; i < n; i++) r += u.x[i] * v.x[i];
    return r;
}

template <class T >
vec<T,3> cross(const vec<T,3> & u, const vec<T,3> & v)
{
    return vec<T,3> (
                     u[1] * v[2] - u[2] * v[1],
                     u[2] * v[0] - u[0] * v[2],
                     u[0] * v[1] - u[1] * v[0]);
}

template <class T, int n>
std::ostream& operator << (std::ostream& out, const vec<T,n> & u)
{
    for(int i = 0; i < n; i++)
    {
        if(i) out << ' ';
        out << u[i];
    }
    return out;
}

typedef vec<float,2> vec2;
typedef vec<float,3> vec3;
typedef vec<int,2> ivec2;
typedef vec<float,3> pixelVec3;

#endif

/**
 * Standard macro to report errors
 */
inline void MGL_ERROR(const char* description) {
    printf("%s\n", description);
    exit(1);
}



/*
 To Do list
 1. implent general case for triangle raster
 
 
*/


// classes and functions


void rasterizeTriangles();
void drawCoodSystem();
pixelVec3 pixelCoordForXCoordinate(float pointX, float pointY);
bool wayToSort(vec3 i, vec3 j) { return i[1] > j[1]; }
void rasterizeFlatBottomTriangle(pixelVec3 v1, pixelVec3 v2, pixelVec3 v3);
void rasterizeFlatTopTriangle(pixelVec3 v1, pixelVec3 v2, pixelVec3 v3);

std::ostream &operator<<(std::ostream &os, vec3 const &v) {
    return os << "V(x,y,z) = (" << v[0] << "," << v[1] << "," << v[2] << ")";
}




// My Useful Globals
MGLpoly_mode currentDrawingMode;
MGLmatrix_mode currentMatrixMode;
MGLint currentColor = 0;
int W = 320;
int H = 240;
list<vec3> gloablVertexList;
MGLpixel currentPixelBitmap[320*240];
MGLfloat currentModelViewMatrix[4][4];
MGLfloat currentProjectionMatrix[4][4];
MGLfloat currentUserSetMatrix[4][4];

// pixel (x,y) maps to element [(y*width) + x]





/**
 * Read pixel data starting with the pixel at coordinates
 * (0, 0), up to (width,  height), into the array
 * pointed to by data.  The boundaries are lower-inclusive,
 * that is, a call with width = height = 1 would just read
 * the pixel at (0, 0).
 *
 * Rasterization and z-buffering should be performed when
 * this function is called, so that the data array is filled
 * with the actual pixel values that should be displayed on
 * the two-dimensional screen.
 */
void mglReadPixels(MGLsize width, MGLsize height, MGLpixel *data){
    W = width;
    H = height;
    /* Fill Screen With Red Test
    for (int x = 0; x < width; ++x) {
        for (int y =0; y < height; ++y) {
            currentPixelBitmap[(y*width) + x] = Make_Pixel(255, 0, 0);
        }
    }
    */
    copy(currentPixelBitmap, currentPixelBitmap+(W*H), data);
    
}



/**
 * Start specifying the vertices for a group of primitives,
 * whose type is specified by the given mode.
 */
void mglBegin(MGLpoly_mode mode){
    switch (mode){
        case MGL_TRIANGLES:
            //cout << "Triangle Mode" << endl;
            // expect to start drawing a triangle
            currentDrawingMode = MGL_TRIANGLES;
            break;
        case MGL_QUADS:
            //cout << "Quads Mode"  << endl;
            currentDrawingMode = MGL_QUADS;
    }
}


/**
 * Stop specifying the vertices for a group of primitives.
 */
void mglEnd(){
    switch (currentDrawingMode) {
        case MGL_TRIANGLES:
            // have to classify a scope to initialize var in case statement
            //cout << "Triangles End" << endl;
            rasterizeTriangles();
            break;
            
        case MGL_QUADS:
        {// have to classify a scope to initialize var in case statement
            //cout << "Quads End" << endl;
            break;
        }
            
        default:
            break;
    }
}
void rasterizeTriangles(){
    
    // fill screen black first
    for (int x = 0; x < W; ++x) {
        for (int y =0; y < H; ++y) {
            currentPixelBitmap[(y*W) + x] = Make_Pixel(0, 0, 0);
        }
    }
    
    
    
    //drawCoodSystem();
    int numOfTrianglesToDraw = (int)gloablVertexList.size()/3;
    
    
    
    
    // rasterize triangles
    for (int i = 0; i < numOfTrianglesToDraw; i++) {
        vec3 triangle[4];
        
        triangle[0] = gloablVertexList.front();
        gloablVertexList.pop_front();
        triangle[1] = gloablVertexList.front();
        gloablVertexList.pop_front();
        triangle[2] = gloablVertexList.front();
        gloablVertexList.pop_front();
        
        sort(triangle, triangle + 3, wayToSort);
        
        //cout << triangle[1].x << ", " << triangle[1].y << endl;
        pixelVec3 v1 = pixelCoordForXCoordinate(triangle[0][0], triangle[0][1]);
        pixelVec3 v2 = pixelCoordForXCoordinate(triangle[1][0], triangle[1][1]);
        pixelVec3 v3 = pixelCoordForXCoordinate(triangle[2][0], triangle[2][1]);
        
        
        //cout << v2.x << ", " << v2.y <<  endl;
        
        if (triangle[2][1] == triangle[1][1]) {
            // flat bottom triangle
            rasterizeFlatBottomTriangle(v1, v2, v3);
            
        }
        else if (triangle[0][1] == triangle[1][1]) {
            // flat top triangle
            rasterizeFlatTopTriangle(v1, v2, v3);
        }
        else{
            // other triangles
            cout << "Other Triangle Detected" << endl;
            
            // split triangle up into two flat top or bottom triangles
            vec3 tmpV4;
            tmpV4[0] = triangle[0][0] + (((triangle[1][1] - triangle[0][1])/(triangle[2][1] - triangle[0][1])) * (triangle[2][0] - triangle[0][0]));
            tmpV4[1] = triangle[1][1];
            triangle[3] = tmpV4;
            sort(triangle, triangle + 4, wayToSort);
            
            pixelVec3 v4 = pixelCoordForXCoordinate(tmpV4[0], tmpV4[1]);
            
            rasterizeFlatBottomTriangle(v1, v2, v4);
            rasterizeFlatTopTriangle(v2, v4, v3);
            
        }
        
    }
    // clear any extra verticies
    gloablVertexList.clear();
}


/**
 * Specify a two-dimensional vertex; the x- and y-coordinates
 * are explicitly specified, while the z-coordinate is assumed
 * to be zero.  Must appear between calls to mglBegin() and
 * mglEnd().
 */
void mglVertex2(MGLfloat x, MGLfloat y){
    // should just add a vertex to global list here
    vec3 tmp;
    tmp[0] = x;
    tmp[1] = y;
    tmp[2] = 0;
    gloablVertexList.push_back(tmp);
    
    //cout << "Vector " << gloablVertexList.back() << "was added." << endl;
}

/**
 * Specify a three-dimensional vertex.  Must appear between
 * calls to mglBegin() and mglEnd().
 */
void mglVertex3(MGLfloat x, MGLfloat y, MGLfloat z){
    // should just add a vertex to global list here
    vec3 tmp;
    tmp[0] = x;
    tmp[1] = y;
    tmp[2] = z;
    gloablVertexList.push_back(tmp);
    
    
    
    //cout << "Vector " << gloablVertexList.back() << "was added." << endl;
}

/**
 * Set the current matrix mode (modelview or projection).
 */
void mglMatrixMode(MGLmatrix_mode mode){
    currentMatrixMode = mode;
}

/**
 * Push a copy of the current matrix onto the stack for the
 * current matrix mode.
 */
void mglPushMatrix(){
    if (currentMatrixMode == MGL_PROJECTION){
        //currentProjectionMatrix = currentUserSetMarix;
    }
    else if (currentMatrixMode == MGL_MODELVIEW){
        //*currentModelViewMatrix = *currentUserSetMatrix;
    }
    else{
        // nothing
    }
}

/**
 * Pop the top matrix from the stack for the current matrix
 * mode.
 */
void mglPopMatrix(){
    
}

/**
 * Replace the current matrix with the identity.
 */
void mglLoadIdentity(){
    
}

/**
 * Replace the current matrix with an arbitrary 4x4 matrix,
 * specified in column-major order.  That is, the matrix
 * is stored as:
 *
 *   ( a0  a4  a8  a12 )
 *   ( a1  a5  a9  a13 )
 *   ( a2  a6  a10 a14 )
 *   ( a3  a7  a11 a15 )
 *
 * where ai is the i'th entry of the array.
 */
void mglLoadMatrix(const MGLfloat *matrix){
    
}

/**
 * Multiply the current matrix by an arbitrary 4x4 matrix,
 * specified in column-major order.  That is, the matrix
 * is stored as:
 *
 *   ( a0  a4  a8  a12 )
 *   ( a1  a5  a9  a13 )
 *   ( a2  a6  a10 a14 )
 *   ( a3  a7  a11 a15 )
 *
 * where ai is the i'th entry of the array.
 */
void mglMultMatrix(const MGLfloat *matrix){
    
}

/**
 * Multiply the current matrix by the translation matrix
 * for the translation vector given by (x, y, z).
 */
void mglTranslate(MGLfloat x, MGLfloat y, MGLfloat z){
    
}

/**
 * Multiply the current matrix by the rotation matrix
 * for a rotation of (angle) degrees about the vector
 * from the origin to the point (x, y, z).
 */
void mglRotate(MGLfloat angle, MGLfloat x, MGLfloat y, MGLfloat z){
    
}

/**
 * Multiply the current matrix by the scale matrix
 * for the given scale factors.
 */
void mglScale(MGLfloat x, MGLfloat y, MGLfloat z){
    
}

/**
 * Multiply the current matrix by the perspective matrix
 * with the given clipping plane coordinates.
 */
void mglFrustum(MGLfloat left, MGLfloat right, MGLfloat bottom, MGLfloat top, MGLfloat near, MGLfloat far){
    
}

/**
 * Multiply the current matrix by the orthographic matrix
 * with the given clipping plane coordinates.
 */
void mglOrtho(MGLfloat left, MGLfloat right, MGLfloat bottom, MGLfloat top, MGLfloat near, MGLfloat far){
    
}

/**
 * Set the current color for drawn shapes.
 */
void mglColor(MGLfloat red, MGLfloat green, MGLfloat blue){
    currentColor = Make_Pixel(red*255, green*255, blue*255);
}






#pragma mark My Helper Functions
///////////////////////////////////////////////////////////////////////////
//My Functions

pixelVec3 pixelCoordForXCoordinate(float pointX, float pointY){
    pixelVec3 convertedPixelCoord;
    
    convertedPixelCoord[0] = (int) roundf((W/2 * pointX) + W/2);
    convertedPixelCoord[1] = (int) roundf((H/2 * pointY) + H/2);
    
    return convertedPixelCoord;
}

void drawCoodSystem(){
    // draw seperating line test and x and y axis
    // seperating line and y axis
    for (int c = 0; c < H; c++) {
        currentPixelBitmap[(c*W)] = currentColor;
        currentPixelBitmap[(c*W)+ W/2] = currentColor;
    }
    // draw x axis
    for (int c = 0; c < W; c++) {
        currentPixelBitmap[(H/2 * W) + c] = currentColor;
    }
    // center point
    currentPixelBitmap[(int)(roundf((H/2 * W) + roundf(W/2)))] = currentColor;
}

void rasterizeFlatBottomTriangle(pixelVec3 v1, pixelVec3 v2, pixelVec3 v3){
    //cout << "Flat Bottom Triangle Detected" << endl;
    
    
    float m1 = (v2[0] - v1[0])/(v2[1] - v1[1]);
    float m2 = (v3[0] - v1[0])/(v3[1] - v1[1]);
    
    float pointOnLine1 = v1[0];
    float pointOnLine2 = v1[0];
    
    //cout << "m1 = " << m1 << ", m2 = " << m2 << endl;
    
    
    
    for (int i = v1[1]; i > v3[1]; i--) {
        //cout << "in for" << endl;
        // draw line
        for (float j = pointOnLine1; j < pointOnLine2; j++) {
            currentPixelBitmap[(int) roundf((i * W) + j)] = currentColor;
        }
        
        pointOnLine1 -= m1;
        pointOnLine2 -= m2;
        
    }
    //test verts
    
    currentPixelBitmap[(int)roundf((v1[1] * W) + v1[0])] = currentColor;
    currentPixelBitmap[(int)roundf((v2[1] * W) + v2[0])] = currentColor;
    currentPixelBitmap[(int)roundf((v3[1] * W) + v3[0])] = currentColor;
}

void rasterizeFlatTopTriangle(pixelVec3 v1, pixelVec3 v2, pixelVec3 v3){
    //cout << "Flat Bottom Triangle Detected" << endl;
    float m1 = (v3[0] - v1[0])/(v3[1] - v1[1]);
    float m2 = (v3[0] - v2[0])/(v3[1] - v2[1]);
    
    float pointOnLine1 = v3[0];
    float pointOnLine2 = v3[0];
    
    
    
    
    for (int i = v3[1]; i <= v1[1]; i++) {
        //cout << "in for" << endl;
        // draw line
        for (int j = pointOnLine1; j <= pointOnLine2; j++) {
            currentPixelBitmap[(i * W) + j] = currentColor;
        }
        
        pointOnLine1 += m1;
        pointOnLine2 += m2;
        
    }
}

