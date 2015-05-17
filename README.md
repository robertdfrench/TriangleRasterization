Triangle Rasterization
======================

Detects what points are inside a triangle.

### Algorithm Outline

Given a triangle `T` with vertices `T.a`, `T.b`, and `T.c`, goal is to identify whether a given pixel `P` with coordinates `P.x` and `P.y` lies inside (or on the border of) `T`.

* Compute the centroid `CT` of `T`:
  * This can be done by averaging the `x` and `y` values of each of the triangle's vertices. For example, `CT.x = (T.a.x + T.b.x + T.c.x) / 3.0`
* Compute slope and y-intercept for lines along each of the triangle's edges
  * for line `Lab` between `T.a` and `T.b`, use the point-slope formula to calculate `Lab.m`
  * Calculate y-intercept as `Lab.b = T.a.y - Lab.m * T.a.x`
* Now calculate the "orientation" of `CT` relative to each line `Lab`, `Lbc`, and `Lca`
  * If `CT.y > Lab.m * CT.x + Lab.b`, then `CT` is "ABOVE" line `Lab`
  * If `CT.y <= Lca.m * CT.x + Lca.b`, then `CT` is "BELOW" line `Lca`
  * This triple of "ABOVE" / "BELOW" values is the canonical orientation for triangle `T`
* Now, for each pixel in the region, calculate the "orientation" of `P` relative to each line `Lab`, `Lbc`, and `Lca`
  * If the orientation of `P` matches that of the centroid `CT`, then `P` is said to be "INSIDE" the triangle `T`
  * If the orientation of `P` differs from that of the centroid `CT`, then `P` is said to be "OUTSIDE" the triangle `T`
* Color the Pixel as appropriate. I like to blend it with the background.

### Text Example
```
+----------------------------------------------------------------------+
|                                                                      |
|                                                                      |
|                                                                      |
|   *                                                                  |
|    **                                                                |
|     ***                                                              |
|       ****                                                           |
|        *****                                                         |
|          *****                                                       |
|           *******                                                    |
|            ********                                                  |
|              *********                                               |
|               **********                                             |
|                ***********                                           |
|                  ************                                        |
|                   *************                                      |
|                     *************                                    |
|                      ***************                                 |
|                       ****************                               |
|                         *****************                            |
|                          ******************                          |
|                           *******************                        |
|                             ********************                     |
|                              *********************                   |
|                                *********************                 |
|                                 ***********************              |
|                                  ************************            |
|                                    *************************         |
|                                     **************************       |
|                                      ***************************     |
|                                        ***********************       |
|                                         *********************        |
|                                           ******************         |
|                                            ****************          |
|                                             *************            |
|                                               **********             |
|                                                ********              |
|                                                 ******               |
|                                                   **                 |
|                                                                      |
+----------------------------------------------------------------------+
```

### Acknowledgements

PNG support is made possible by Lode Vandevenne's excellent [LodePNG](https://lodev.org/lodepng/).
