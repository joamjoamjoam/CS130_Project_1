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
class pixelVec3{
public:
    MGLfloat x;
    MGLfloat y;
};

class vec3{
public:
    MGLfloat x;
    MGLfloat y;
    MGLfloat z;
};
void rasterizeTriangles();
void drawCoodSystem();
pixelVec3 pixelCoordForXCoordinate(float pointX, float pointY);
bool wayToSort(vec3 i, vec3 j) { return i.y > j.y; }
void rasterizeFlatBottomTriangle(pixelVec3 v1, pixelVec3 v2, pixelVec3 v3);
void rasterizeFlatTopTriangle(pixelVec3 v1, pixelVec3 v2, pixelVec3 v3);

std::ostream &operator<<(std::ostream &os, vec3 const &v) {
    return os << "V(x,y,z) = (" << v.x << "," << v.y << "," << v.z << ")";
}




// My Useful Globals
MGLpoly_mode currentDrawingMode;
MGLint currentColor = 0;
int W = 320;
int H = 240;
list<vec3> gloablVertexList;
MGLpixel currentPixelBitmap[320*240];
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
        vec3 triangle[3];
        
        triangle[0] = gloablVertexList.front();
        gloablVertexList.pop_front();
        triangle[1] = gloablVertexList.front();
        gloablVertexList.pop_front();
        triangle[2] = gloablVertexList.front();
        gloablVertexList.pop_front();
        
        sort(triangle, triangle + 3, wayToSort);
        
        cout << triangle[1].x << ", " << triangle[1].y << endl;
        pixelVec3 v1 = pixelCoordForXCoordinate(triangle[0].x, triangle[0].y);
        pixelVec3 v2 = pixelCoordForXCoordinate(triangle[1].x, triangle[1].y);
        pixelVec3 v3 = pixelCoordForXCoordinate(triangle[2].x, triangle[2].y);
        
        
        cout << v2.x << ", " << v2.y <<  endl;
        
        if (triangle[2].y == triangle[1].y) {
            // flat bottom triangle
            rasterizeFlatBottomTriangle(v1, v2, v3);
            
        }
        else if (triangle[0].y == triangle[1].y) {
            // flat top triangle
            rasterizeFlatTopTriangle(v1, v2, v3);
        }
        else{
            // other triangles
            cout << "Other Triangle Detected" << endl;
            
            
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
    tmp.x = x;
    tmp.y = y;
    tmp.z = 0;
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
    tmp.x = x;
    tmp.y = y;
    tmp.z = z;
    gloablVertexList.push_back(tmp);
    
    
    
    //cout << "Vector " << gloablVertexList.back() << "was added." << endl;
}

/**
 * Set the current matrix mode (modelview or projection).
 */
void mglMatrixMode(MGLmatrix_mode mode){
    
}

/**
 * Push a copy of the current matrix onto the stack for the
 * current matrix mode.
 */
void mglPushMatrix(){
    
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
///////////////////////////////////////////////////////////////////////////
//My Functions

pixelVec3 pixelCoordForXCoordinate(float pointX, float pointY){
    pixelVec3 convertedPixelCoord;
    
    convertedPixelCoord.x = (int) roundf((W/2 * pointX) + W/2);
    convertedPixelCoord.y = (int) roundf((H/2 * pointY) + H/2);
    
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
    
    
    float m1 = (v2.x - v1.x)/(v2.y - v1.y);
    float m2 = (v3.x - v1.x)/(v3.y - v1.y);
    
    float pointOnLine1 = v1.x;
    float pointOnLine2 = v1.x;
    
    cout << "m1 = " << m1 << ", m2 = " << m2 << endl;
    
    
    
    for (int i = v1.y; i > v3.y; i--) {
        //cout << "in for" << endl;
        // draw line
        for (float j = pointOnLine1; j < pointOnLine2; j++) {
            currentPixelBitmap[(int) roundf((i * W) + j)] = currentColor;
        }
        
        pointOnLine1 -= m1;
        pointOnLine2 -= m2;
        
    }
    //test verts
    
    currentPixelBitmap[(int)roundf((v1.y * W) + v1.x)] = currentColor;
    currentPixelBitmap[(int)roundf((v2.y * W) + v2.x)] = currentColor;
    currentPixelBitmap[(int)roundf((v3.y * W) + v3.x)] = currentColor;
}

void rasterizeFlatTopTriangle(pixelVec3 v1, pixelVec3 v2, pixelVec3 v3){
    //cout << "Flat Bottom Triangle Detected" << endl;
    float m1 = (v3.x - v1.x)/(v3.y - v1.y);
    float m2 = (v3.x - v2.x)/(v3.y - v2.y);
    
    float pointOnLine1 = v3.x;
    float pointOnLine2 = v3.x;
    
    
    
    
    for (int i = v3.y; i <= v1.y; i++) {
        //cout << "in for" << endl;
        // draw line
        for (int j = pointOnLine1; j <= pointOnLine2; j++) {
            currentPixelBitmap[(i * W) + j] = currentColor;
        }
        
        pointOnLine1 += m1;
        pointOnLine2 += m2;
        
    }
}

