#include "FrUtils.h"

float Frange(float a, float minv, float maxv){
	if (a<minv) 
		a=minv;
	if (a>maxv) 
		a=maxv;
	return a;
}

int  Irange(int a, int minv, int maxv){
	if (a<minv) 
		a=minv;
	if (a>maxv)
		a=maxv;
	return a;
}

void GetLayerSettings(FrSliceViewSettings*  sets, LayerCollection& layers){
    layers.clear();
    layers.push_back(&sets->MainLayer);
    
    LayerCollection::iterator it, itEnd(sets->OtherLayers.end());
    for(it = sets->OtherLayers.begin(); it != itEnd; ++it){
        layers.push_back( (*it) );
    }
}

void GetLayerSettings(FrMosaicViewSettings* sets, LayerCollection& layers){
    layers.clear();
    layers.push_back(&sets->MainLayer);

    LayerCollection::iterator it, itEnd(sets->OtherLayers.end());
    for(it = sets->OtherLayers.begin(); it != itEnd; ++it){
        layers.push_back( (*it) );
    }
}

void GetLayerSettings(FrOrthoViewSettings*  sets, LayerCollection& layers, int viewID){
    layers.clear();
    layers.push_back(&sets->MainLayer[viewID]);

    LayerCollection::iterator it, itEnd(sets->OtherLayers[viewID].end());
    for(it = sets->OtherLayers[viewID].begin(); it != itEnd; ++it){
        layers.push_back( (*it) );
    }
}