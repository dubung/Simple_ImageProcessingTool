#pragma once

#include "LdhImage.h"
#include "LdhDib.h"


void LdhDibToImage(LdhDib& dib, LdhByteImage& img);
void LdhDibToImage(LdhDib& dib, LdhRGBImage& img);

void LdhImageToDib(LdhByteImage& img, LdhDib& dib);
void LdhImageToDib(LdhRGBImage& img, LdhDib& dib);
void LdhImageToDib(LdhFloatImage& img, LdhDib& dib);