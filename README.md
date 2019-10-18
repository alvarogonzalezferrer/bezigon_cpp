# Bezigon routines

Copyright (c) **Kronoman**.
In loving memory of my father.
Made in Argentina.

## Description

A bezigon is a closed 2d shape similar to a polygon, but with Bezier curves as edges.

As bezier curves can be straight lines, a polygon is a special instance of a bezigon. Anyways, if you need to draw polygons, just use the polygon routine that comes with Allegro, is faster.

## How Bezigons are drawn

Bezier curves can be approximated into short straight lines. A good algorithm can select the right step size to give a visually smooth curve without generating too many extra points, as this will affect performance.

Once you have subdivided the bezier curves, you are left with a polygon, which can be drawn like any other polygon, using Allegro functions. 

## License

Released under the MIT license.
