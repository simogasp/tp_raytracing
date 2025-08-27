# Ray tracer

## Table of Content

- [Introduction](#introduction)
- [The GUI](#the-gui)
- [Build](BUILD.md)
- [Step to Reach](#step-to-reach)
- [GLM Most Useful functions](#)
- [To improve your render](#to-improve-your-render)
- [Helpful Ressource](#helpful-ressources)

## Introduction

The goal of the project is to build your first simple ray tracer on CPU.

The given code is a fill-in-the-blank exercise but you have to do the most important part. The given part is a GUI described below.

## The GUI

On the Gui, you have the real-time render in the panel Viewport, the help explained How to move in the scene with the camera. The settings panel allow you to move the camera by cursor or to choose some preset.


## Step to Reach

### Orient the ray

First of all, we have to build our rays. We have a camera point, a lookAt point and a horizontal Field Of View (FOV).

Pixel have (x, y) coordinated from (0, 0) to (width, height) from top left to bottom right. First, we compute a relative value from the (x, y) to a value from (1, 1) (top left) to (-1, -1) (bottom right).

x relative : $
    r_x = -2 * \frac x{\text{width - 1}} + 1
$

y relative : $
    r_y = -2 * \frac y{\text{height - 1}} + 1
$

The ray direction of the screen pixel is given by the uniform vector from the camera origin to the point in the screen $\left (\begin{matrix}r_x \\ r_y \\ focal\end{matrix} \right )$ in the systeme of the camera.

### Manage rays

Now, you have rays directions for your camera, now it's time to render your first scene. In [renderer.cpp](src/raytracing/controller/Renderer.cpp), fill the double for loop to manage each ray/pixel of the screen, in this first step you have to fill it in the same color (we will considere it has the sky #019CE0 for example). This color will be render if nothing is render.


### Intersection with sphere

We now have to find if the ray instersect a sphere.

We suppose the point $h = O_{\text{ray}} + t\cdot D_{\text{ray}}$ where $O_{\text{ray}}$ is the origin of the ray and $D_{\text{ray}}$ is the direction of the ray (uniform).

Now, with $O_{\text{sphere}}$ origin of the sphere and $R_{\text{sphere}}$ the radius of the sphere, we have :

$$
\begin{split}
h \in S(O_{\text{sphere}}, r_{\text{sphere}})
    &\iff \vert\vert h - O_{\text{sphere}} \vert\vert = r_{\text{sphere}} \\
    &\iff \vert\vert h - O_{\text{sphere}} \vert\vert ^2 = r_{\text{sphere}}^2 \\
    &\iff < h - O_{\text{sphere}}| h- O_{\text{sphere}} > - r_{\text{sphere}}^2 = 0 \\
    &\iff <O_{\text{ray}} + t\cdot D_{\text{ray}} - O_{\text{sphere}}| O_{\text{ray}} + t\cdot D_{\text{ray}}- O_{\text{sphere}}> - r_{\text{sphere}}^2 = 0 \\
    &\iff <t\cdot D_{\text{ray}} + O_{\text{ray}} - O_{\text{sphere}}| t\cdot D_{\text{ray}} + O_{\text{ray}} - O_{\text{sphere}}> - r_{\text{sphere}}^2  = 0 \\
    &\iff <t\cdot D_{\text{ray}} | t\cdot D_{\text{ray}}> + <O_{\text{ray}} - O_{\text{sphere}} | O_{\text{ray}} - O_{\text{sphere}}> - r_{\text{sphere}}^2  = 0 \\
    &\iff <t\cdot D_{\text{ray}} | t\cdot D_{\text{ray}}> + <O_{\text{ray}} - O_{\text{sphere}} | O_{\text{ray}} - O_{\text{sphere}}> - r_{\text{sphere}}^2  = 0 \\
    & \iff t^2 || D_{\text{ray}}||^2 + || \overrightarrow{O_{\text{ray}}O_{\text{sphere}}}||^2 - r_{\text{sphere}}^2 = 0 \\
\end{split}
$$

As $ D_{\text{ray}} $ is uniform, we can ignore it. The rest is a quadratic equation. 

A solution to this equation mean a hit from the ray to the sphere. If the solution is negative, it means that the sphere is behind the camera.


### Render your first sphere

In [App.cpp](src/raytracing/gui/App.cpp), in the constructor, you fill your scene with the sphere you want. Start by added your first [material](doc/Material.md) with basics settings (dark red color, midShinyness (0.f), full roughness (1.f) for example) to your scene using `pushMaterial(const glm::vec3 reflectionColor, const float shinyness, const float roughness)`. Now you can add a sphere to your scene with basics settings too (center in origin, radius of 1, material index = 0) using `pushSphere(const glm::vec3 center, const float radius, const uint materialIndex)`.

Now, modify the render script to check the intersection between rays and the sphere. The list of Sphere is available by the call of `getListSphere()`.

### Render a second sphere (maybe a floor)

To simulate the presence of a floor, we can add a material to the scene in [App.cpp](src/raytracing/gui/App.cpp) (color example : gray, shinyness : midShinyness, roughness : fullRoughness) and a floor sphere at `floorPos` and a radius of 1000.

### Render with a simple bounce 

we want to see interaction between the two sphere. We are going to use the constants define `BOUNCES` to known how many bounce a ray should do. But first, we should create a bounce.

For the first modelisation of a bounce we are going to ignore the roughness characteristic of materials and bounce perfectly on sphere. To bounce we need to know where the hit has been done and the normal of the sphere at this point. Then we change the ray origin to the hit position <span style="color:red">shifted by an epsilon times the normal</span> to not hit the same sphere again. The new direction of the ray can be computed by the function `glm::reflect(incident vector, normal vector)`.



## GLM Most Useful functions

 - `glm::vec3` : vector of size 3
 - `glm::normalize` : normalize a vector
 - `glm::reflect` : reflect a incident vector according to a normal
 - `glm::refract` : refract a incident vector according to a normal and $\frac{n_1}{n_2}$
 - `glm::to_string` after add `#define GLM_ENABLE_EXPERIMENTAL` and `#include <glm/gtx/string_cast.hpp>` in the header : print a vector (use : `std :: cout << glm::to_string(glm::vec3(0)) << std :: endl;` prints the 0 vector of dimension 3).


## To improve your render

You can test to replace the floor material by a metalic mirror with the modifiers :
 - color : gray
 - shiny : shiny
 - roughness : noRoughness

You can add a sky blue gradient color according to the y direction of the ray.

You can implement other surface type as torus or plan.

## Helpful ressources

 - A youtube video serie : [Raytracing serie](https://www.youtube.com/watch?v=gfW1Fhd9u9Q&list=PLlrATfBNZ98edc5GshdBtREv5asFW3yXl)
 - A good website : [Raytracing in One Week-end](https://raytracing.github.io/books/RayTracingInOneWeekend.html)
