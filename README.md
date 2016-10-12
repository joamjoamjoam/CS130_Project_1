##Overview

In this assignment, you will be implementing a simplified 3D rendering pipeline (with flat shading). This will consist of several parts:

1. Vertex and viewing transformations
2. Rasterization
3.Using a z-buffer for hidden surfaces

##Code

Skeleton code is available here.

##Submission Instructions

Deadline TBD.
Your assignment is to implement the functions in minigl.cpp. Submit your minigl.cpp file on iLearn.

##Software Framebuffer

You will have a software framebuffer that stores both a 2D array of pixel colors and a 2D array of z (depth) values. Your rasterization routine will write into this software framebuffer.

##Rasterization and z-buffer depth test

Implement a routine that draws a filled triangle specified by three vertices into your software framebuffer. The routine should write its result to the software framebuffer, doing the appropriate z-buffer check.

##Vertex and viewing transformations

Similar to OpenGL, you will maintain both a projection matrix stack and a modelview stack.

When the user draws a vertex you will apply the modelview and projection ( projection * (modelview * vertex) ) to obtain the transformed geometry. This is followed by a divide by w, and a viewport transform. You will then store this transformed geometry for rasterization. The reason for this is that the transformations are part of the current state and will not necessarily persist.

##Getting Started

If you need help getting started, I suggest you do the following:

1. Get the mglBegin(), mglEnd(), and mglVertex3() calls working first. Make mglReadPixels draw the vertices in the output so you can verify that they are where they should be.

2.Once the pixels are in the right places, modify mglReadPixels to rasterize each element.

3.Fine-tune the placement of your pixels. The -d option will be very helpful here.

4.Go through the list of inputs one at a time, implementing features as they are needed and fixing bugs as you encounter them. The tests are roughly arranged from easy to hard and are intended to assist you in debugging your program.

5. If you fail a test, try pruning down the test by removing or simplifying commands in the test file to get the simplest test case that you still fail. Simpler examples tend to be easier to debug.

6. Each function that you are expected to write in miniGL corresponds to an appropriate OpenGL command. I strongly recommend you look at this before implementing the appropriate functions.

##Grading

Along with the same code is included 25 test cases. Each test case contains a list of commands, which are the OpenGL functions that will be called and the arguments that they will be given. The grading is automated, so you will be able to determine what your score will be before you turn it in. Errors are determined by running with the -d option, and timings are determined by running with the -t option. Both runtime and error will be measured on the delta lab machines (delta-01.cs.ucr.edu, etc.) Here is how your grade will be computed:

Tests |	Points | Error limit | Time limit | Purpose |	Notes |
-------|--------|-------------|------------|---------|-------|
00 |	10 |	1.00% |	1000ms |	Basic rasterization |	Basic stuff is worth a lot of the points | 
01-04  |	5 (each) | 1.00% | 1000ms |	Transforms, basic features |	Each test is 5 points; 20 points total | 
05-23  |	1 (each) | 1.00% | 1000ms |	Remaining features and commands |	Each test is 1 point; 19 points total | 
00-23  |	1 (total) | 0.20% | 1000ms |	Pixel placement accuracy |	All tests must be within this limit to get the point. | 
00-23  |	1 (total) | 0.05% | 1000ms |	Pixel placement accuracy |	Only 15 tests must be within this limit to get the point. | 
00-23  |	1 (total) | 1.00% | 10ms |	Efficiency and optimization |	All tests must be within this limit to get the point. |
24 |	1 |	8.00% |	5000ms |	Scaling, stress test |	Note that more error and time is allowed. |
25 |	1 |	1.00% |	1000ms |	Bug test |	Clipping before leaving homogeneous coordinates. |
24 |	1 |	4.00% |	5000ms |	Pixel placement accuracy | None. |
24 |	1 |	2.00% |	5000ms |	Pixel placement accuracy | None. |	
24 |	1 |	1.00% |	5000ms |	Pixel placement accuracy | None. |	
24 |	1 |	8.00% |	100ms |	Efficiency and optimization | None. | 
24 |	1 |	8.00% |	30ms |	Efficiency and optimization | None. |	
24 |	1 |	8.00% |	15ms |	Efficiency and optimization | None. |	
26 |	1 |	1.00% |	1000ms |	Bug test |	Projection correct interpolation. | 
27 |	1 |	1.00% |	1000ms |	Bug test |	Interpolation and z buffering. |

The project will be graded out of 50 points. The maximum number of points possible is 62; 12 points of extra credit are possible. Implementing enough to pass the first five tests suffices for half of the points. The tests generally fall into one of three types: correctness, accuracy, and efficiency. Correctness tests check that the functionally is implemented and works. Accuracy tests check to see that pixels are being drawn where they should be (rather than off by one pixel or even a fraction of a pixel). Some deviation from the OpenGL implementation is to be expected, but you should be able to get very close. Efficiency tests encourage you to implement various optimizations, such as geometric clipping and incremental computation. It does not matter which if any optimizations you implement; if your code is efficient enough, you get the points.

##Grading script

We are providing you with the grading script that we will be using to grade this project. You can run it like this: "./grading-script.sh . grading-scheme.txt". This will automatically compile your code, run all of the tests in the table above, and compute your score based on the point values in the table. This means you can run it yourself to see your progress. You can also run it before you submit your project so that you know what your score will be before you submit it.

Note that scores can vary from run to run due to hardware and natural variability. For example, if run on a machine slower than delta-01, you may see a lower score by failing some of the efficiency tests that would pass on delta-01. Some runs will be slightly faster and some slightly slower; if you are very close to the time limit on a test, it may pass sometimes and fail others. To give you the benefit of the doubt with timing variability, timing tests are automatically run three times each (or until you pass). If any of the three runs is within the time limit, you will get credit for passing the test.

Be sure to test your project on one of the delta machines. All of the delta machines are identical in hardware and software; if it works on one, it should work on all. It is possible for your project to score well on your own machine but fail on delta-01. For example, the version of the compiler on your machine may accept your code while the version on delta may flag a compilation error and fail.

##Checkpoints

This project has two checkpoints. These are milestones to encourage you to start your project early. At each checkpoint, you will submit your project just as you would when it is due. At the first checkpoint, your program will be graded out of 20 points. At the second checkpoint, your program will be graded out of 35 points. No extra credit is possible at checkpoints. When your project is due, it will be graded out of 50 points, and extra credit will be given for scores higher than that.
