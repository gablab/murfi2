#include "FrUtils.h"
#include "math.h"

//void GetLayerSettings(FrSliceViewSettings*  sets, LayerCollection& layers){
//    layers.clear();
//    layers.push_back(&sets->MainLayer);
//    
//    LayerCollection::iterator it, itEnd(sets->OtherLayers.end());
//    for(it = sets->OtherLayers.begin(); it != itEnd; ++it){
//        layers.push_back( (*it) );
//    }
//}
//
//void GetLayerSettings(FrMosaicViewSettings* sets, LayerCollection& layers){
//    layers.clear();
//    layers.push_back(&sets->MainLayer);
//
//    LayerCollection::iterator it, itEnd(sets->OtherLayers.end());
//    for(it = sets->OtherLayers.begin(); it != itEnd; ++it){
//        layers.push_back( (*it) );
//    }
//}
//
//void GetLayerSettings(FrOrthoViewSettings*  sets, LayerCollection& layers, int viewID){
//    layers.clear();
//    layers.push_back(&sets->MainLayer[viewID]);
//
//    LayerCollection::iterator it, itEnd(sets->OtherLayers[viewID].end());
//    for(it = sets->OtherLayers[viewID].begin(); it != itEnd; ++it){
//        layers.push_back( (*it) );
//    }
//}

double GetLength(int x1, int y1, int x2, int y2)
{
	double length;
	
	length = sqrt(pow(double (x1 - x2), 2) + pow(double (y1 - y2), 2));

	return length;
}

void GetRealImagePosition(FrViewDocObj* viewDO, vtkImageData* data, int point[3], int imgNumber){
  double dp[3];
  dp[0] = point[0]; dp[1] = point[1]; dp[2] = point[2];
  GetRealImagePosition(viewDO, data, dp, imgNumber);

  point[0] = (int) rint(dp[0]);
  point[1] = (int) rint(dp[1]);
  point[2] = (int) rint(dp[2]);
}

void GetRealImagePosition(FrViewDocObj* viewDO, vtkImageData* data, double point[3], int imgNumber){
    if(!data) return;

    double oldPoint[2];
    oldPoint[0] = point[0];
    oldPoint[1] = point[1];

    double dSpacing[3];	
    data->GetSpacing(dSpacing);		

    Views view = viewDO->GetActiveView();
    
    switch(view){
        case SliceView:
            {
                // get slice number
                int slice = viewDO->GetSliceViewSettings()->SliceNumber;			
                // set current indices of point
		point[0] = (oldPoint[0]-dSpacing[0]/2) / dSpacing[0] + 0.5;
		point[1] = (oldPoint[1]-dSpacing[1]/2) / dSpacing[1] + 0.5;
                point[2] = slice;
            }
            break;
        case MosaicView:
            {
		point[0] = (oldPoint[0]-dSpacing[0]/2) / dSpacing[0] + 0.5;
		point[1] = (oldPoint[1]-dSpacing[1]/2) / dSpacing[1] + 0.5;
                point[2] = 0;
            }
            break;
        case OrthoView:
            {
                // Renderer number in ortho view
                switch(imgNumber){				
                    case DEF_CORONAL: // coronal 
                    {
                        // get slice number
                        int slice = viewDO->GetOrthoViewSettings()->SliceNumber[DEF_CORONAL];

                        point[0] = (oldPoint[0]-dSpacing[0]/2) / dSpacing[0] + 0.5;
                        point[1] = slice;
                        point[2] = (oldPoint[1]-dSpacing[2]/2) / dSpacing[1] + 0.5;
                        break;
                        }
                    case DEF_SAGITAL: // sagital
                    {
                        // get slice number
                        int slice = viewDO->GetOrthoViewSettings()->SliceNumber[DEF_SAGITAL];

                        point[0] = slice;
                        point[1] = (oldPoint[0]-dSpacing[0]/2) / dSpacing[0] + 0.5;
                        point[2] = (oldPoint[1]-dSpacing[1]/2) / dSpacing[1] + 0.5;
			            break;
                    }
                    case DEF_AXIAL:
                    {
                        int slice = viewDO->GetOrthoViewSettings()->SliceNumber[DEF_AXIAL];


                        point[0] = (oldPoint[0]-dSpacing[0]/2) / dSpacing[0] + 0.5;
			point[1] = (oldPoint[1]-dSpacing[1]/2) / dSpacing[1] + 0.5;
			point[2] = slice;
			            break;
                    }
                }
            }
            break;
    }
}

bool IsPointInsideOfPolygon(std::vector<Pos> Points, Pos point){
     bool inside = false;

     unsigned int xnew, ynew;
     unsigned int xold, yold;
     unsigned int x1, y1;
     unsigned int x2, y2;

     int npoints = Points.size();
     
     if (npoints < 3) {
          return false;
     }

     xold = Points[npoints-1].x;
     yold = Points[npoints-1].y;

     for (int i = 0; i<npoints; i++) 
     {
          xnew = Points[i].x;
          ynew = Points[i].y;
          if (xnew > xold) 
	      {
               x1 = xold;
               x2 = xnew;
               y1 = yold;
               y2 = ynew;
          }
          else 
	      {
               x1 = xnew;
               x2 = xold;
               y1 = ynew;
               y2 = yold;
          }
	      if ((xnew < point.x) == (point.x <= xold)         /* edge "open" at left end */
		      && ((long)point.y-(long)y1)*(long)(x2-x1)
		      < ((long)y2-(long)y1)*(long)(point.x-x1)) 
	      {
               inside =! inside;
          }
          xold = xnew;
          yold = ynew;
     }
     return inside;
}

bool IsPointInsideOfSphere(Pos center, int radius, Pos point){
    bool isInside = false;

    if ( (pow(double(center.x - point.x), 2) + 
          pow(double(center.y - point.y), 2) + 
          pow(double(center.z - point.z), 2)) < pow((double)radius, 2) ){
        isInside = true;
    }
    
    return isInside;
}
