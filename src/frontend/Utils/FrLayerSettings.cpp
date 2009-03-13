#include "FrLayerSettings.h"

bool FrLayerSettings::CopySettings(FrLayerSettings* src, FrLayerSettings* dst){
    if(src && dst && src->GetType() == dst->GetType()){
        switch(src->GetType()){
            case LImage:
                *((FrImageLayerSettings*)dst) = *((FrImageLayerSettings*)src);
                return true;
            case LRoi:
                *((FrRoiLayerSettings*)dst) = *((FrRoiLayerSettings*)src);
                return true;
        }
    }
    return false;
}
