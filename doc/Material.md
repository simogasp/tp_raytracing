# Material

## Helper

The [App](../src/raytracing/gui/App.hpp) object has constants to help you to create your materials.
It includes colors, positions, shininess, roughness, emission powers and archetype refractive index.

Colors :

- black
- white
- magenta
- blue
- orange
- gray

Positions:

- redPos $(-1, 0, 0)$
- floorPos $(0, -1001, 0)$
- lightPos $(0, 10, 30)$
- glassPos $(1, 0, 0)$

Shininess:

- mat $0$
- midShiny $0.5$
- shiny $1$

Roughness:

- noRoughness $0$
- midRoughness $0.5$
- fullRoughness $1$

Emission Power:

- noEmissionPower $0$
- normalEmissionPower $1$
- midEmissionPower $2$
- fullEmissionPower $10$

Refractive:

- opaque $0$
- airTranslucid $1$
- waterTranslucid $1.33$
- plexiGlassTranslucid $1.5$

## Description of materials settings

### Reflection Color

The reflection color is the color that the surface reflects.
A black object reflects any light, a red object reflects the red light.

### Shininess

The shininess is the ability of a material to reflect the color of a ray next bounce.
A full shiny object will be colored by the color of the next hit object.
A no-shiny object (as known as a matte object) is an object whose color isn't affected by the next one.

### Roughness

The roughness is the simulated asperity of a surface; the more the sphere is roughness, the more the ray bounces in a random direction.

### Emission Color

The emission color is used to create an emissive material as a lamp.

### Emission Power

The emission power is a modifier to improve or moderate the power of a light.

### Attenuation Radius

The attenuation radius is used to compute the attenuation of the light depending on the distance.

### Refractive index

The refractive index is the speed of the light in the void over the speed of light in the material.
