/*====================================================================*
 -  Copyright (C) 2001 Leptonica.  All rights reserved.
 -
 -  Redistribution and use in source and binary forms, with or without
 -  modification, are permitted provided that the following conditions
 -  are met:
 -  1. Redistributions of source code must retain the above copyright
 -     notice, this list of conditions and the following disclaimer.
 -  2. Redistributions in binary form must reproduce the above
 -     copyright notice, this list of conditions and the following
 -     disclaimer in the documentation and/or other materials
 -     provided with the distribution.
 -
 -  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 -  ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 -  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 -  A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL ANY
 -  CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 -  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 -  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 -  PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 -  OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 -  NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 -  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *====================================================================*/

/*
 *  maze_reg.c
 *
 *    Tests the functions in maze.c: binary and gray maze search
 */

#ifdef HAVE_CONFIG_H
#include <config_auto.h>
#endif  /* HAVE_CONFIG_H */

#include <string.h>
#include "allheaders.h"

#define  NPATHS     6
static const l_int32 x0[NPATHS] = {42, 73, 73, 42, 324, 471};
static const l_int32 y0[NPATHS] = {117, 319, 319, 117, 170, 201};
static const l_int32 x1[NPATHS] = {419, 419, 233, 326, 418, 128};
static const l_int32 y1[NPATHS] = {383, 383, 112, 168, 371, 341};

int main(int    argc,
         char **argv)
{
l_int32       i, w, h;
PIX          *pixm, *pixs, *pixg, *pixt, *pixd;
PIXA         *pixa;
PTA          *pta;
PTAA         *ptaa;
L_REGPARAMS  *rp;

    if (regTestSetup(argc, argv, &rp))
        return 1;
    pixa = pixaCreate(0);

    /* ---------------- Shortest path in binary maze ---------------- */
        /* Generate the maze */
    pixm = generateBinaryMaze(200, 200, 20, 20, 0.65, 0.25);
    pixd = pixExpandBinaryReplicate(pixm, 3, 3);
    pixSaveTiledOutline(pixd, pixa, 1.0, 1, 20, 2, 32);
    pixDestroy(&pixd);

        /* Find the shortest path between two points */
    pta = pixSearchBinaryMaze(pixm, 20, 20, 170, 170, NULL);
    pixt = pixDisplayPta(NULL, pixm, pta);
    pixd = pixScaleBySampling(pixt, 3., 3.);
    pixSaveTiledOutline(pixd, pixa, 1.0, 0, 20, 2, 32);
    regTestWritePixAndCheck(rp, pixd, IFF_PNG);  /* 0 */
    ptaDestroy(&pta);
    pixDestroy(&pixt);
    pixDestroy(&pixd);
    pixDestroy(&pixm);

    /* ---------------- Shortest path in gray maze ---------------- */
    pixg = pixRead("test8.jpg");
    pixGetDimensions(pixg, &w, &h, NULL);
    ptaa = ptaaCreate(NPATHS);
    for (i = 0; i < NPATHS; i++) {
        if (x0[i] >= w || x1[i] >= w || y0[i] >= h || y1[i] >= h) {
            fprintf(stderr, "path %d extends beyond image; skipping\n", i);
            continue;
        }
        pta = pixSearchGrayMaze(pixg, x0[i], y0[i], x1[i], y1[i], NULL);
        ptaaAddPta(ptaa, pta, L_INSERT);
    }

    pixt = pixDisplayPtaa(pixg, ptaa);
    pixd = pixScaleBySampling(pixt, 2., 2.);
    pixSaveTiledOutline(pixd, pixa, 1.0, 1, 20, 2, 32);
    regTestWritePixAndCheck(rp, pixd, IFF_PNG);  /* 1 */
    ptaaDestroy(&ptaa);
    pixDestroy(&pixg);
    pixDestroy(&pixt);
    pixDestroy(&pixd);

        /* Bundle it all up */
    pixd = pixaDisplay(pixa, 0, 0);
    regTestWritePixAndCheck(rp, pixd, IFF_PNG);  /* 2 */
    pixDisplayWithTitle(pixd, 100, 100, NULL, rp->display);
    pixDestroy(&pixd);
    pixaDestroy(&pixa);

    return regTestCleanup(rp);
}
