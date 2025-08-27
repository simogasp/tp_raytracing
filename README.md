# Ray tracer

## Table of Content

- [Introduction](#introduction)
- [The GUI](#the-gui)
- [Build](#build)

## Introduction

The goal of the project is to build your first simple ray tracer on CPU.

The given code is a fill-in-the-blank exercise but you have to do the most important part. The given part is a GUI which its uses will be describe in section [The GUI](#the-gui).

## The GUI

On the Gui, you have the real-time render in the panel Viewport, the help explained How to move in the scene with the camera. The settings panel allo023w you to move the camera by cursor or to choose some preset.


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

Then, we can obtain the angle $\theta$ and $\phi$ which are the angle of divergeance of the ray from the lookAt vector.

$$
\begin{matrix}
    \theta =& \frac{fov}2 \cdot r_x \\
    \phi =& \frac{fov}2 \cdot r_y * \frac 1 {aspectRatio}
\end{matrix}
$$

We compute the ray direction of the screen pixel by using the change of basis from a geographical coordinates to a cannonical cartesian basis. At this point we get the orientation vector but in the camera referential and we need to change of basis to get a direction in the real world reference.

#### Astuce

- [geographical coordonates](https://fr.wikipedia.org/wiki/Coordonn%C3%A9es_sph%C3%A9riques#Convention_rayon-longitude-latitude)
- la méthode baseChangement change the given vector from the camera axis to the world axis.

## Build

